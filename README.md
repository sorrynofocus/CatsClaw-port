# Cat's Claw

An older, lightweight product-key (CD-key style) generation and validation library for C++ Windows applications, originally written in 2004 and ported to Visual Studio 2019 in 2019.

## Description

Cat's Claw generates license/serial keys in the classic 4-segment format (`XXXXX-XXXXX-XXXXX-XXXXX`) and validates them against a given license count (number of seats/sites a key was issued for). It was originally built as copy-protection for a shareware application, using techniques common to that era: checksum-based segments, a deterministic segment tied to a license quantity, and validation logic that's deliberately split across multiple functions so it can be sprinkled throughout an app rather than checked in one easy-to-patch spot.

Three of the four key segments (`CreateFirstKey`, `CreateSecondKey`, `CreateThirdKey`) are randomly generated, each with its own checksum rule that a matching `ValidateRectRegion*` function re-checks. The fourth segment (`CreateFourthKey`) isn't random at all — it's derived deterministically from the first and third segments plus the requested license count, via `DetermineLicenseValue()`. That's what binds a specific key to a specific license count: regenerate `DetermineLicenseValue()` with the same inputs during validation (`ValidateRectRegion4`) and it reproduces the same value only if the key and license count match.

The library ships as a single header (`CatsClawLib.h`) that pulls in its own implementation file (`CatsClawLib.cpp`), so dropping both files into a project and including the header is enough to use it — no separate compilation step or project reference needed. It currently builds as a statically-linked part of your app (`_IS_NOT_DLL`); DLL export/import code paths exist in the header but are not the default and are unverified.

## TL;DR

- Include `CatsClawLib.h` in your app, call `GenerateSerialKey()` to create a key for N licenses, and call `ValidateRectRegion1`–`4` (or your own `CheckALicenseKey`-style wrapper) to check a key against a license count later.
- Keys look like `6F3A1-B2C9D-8E4F0-01A3C7` — four 5-character segments, hyphen-separated.
- A key is only valid for the license count it was generated with.
- **Key generation should not ship inside your end-user application** — only validation should. See [Security note](#security-note).

## Building

Requirements: Windows, Visual Studio 2019+ with the **v140 (VS2015) platform toolset** component installed, Windows SDK `10.0.17763.0` or newer.

1. Open `CatsClaw.sln` in Visual Studio and build (`Debug`/`Release`, `Win32`/`x64`), **or** build from the command line:
   ```
   msbuild CatsClaw.sln /p:Configuration=Release /p:Platform=x64
   ```
2. The output binary lands in `CatsClaw\<Configuration>\CatsClaw.exe`.

This builds the included demo console app, which generates a key and runs it through validation twice (once with a deliberately wrong license count to show a failure, once with the correct count to show success).

Run it:
```
CatsClaw.exe                    REM interactive — prompts for a license count (1-100000)
CatsClaw.exe -a <licenseCount>   REM non-interactive / scriptable mode
```

## Using it as a library in your own application

Cat's Claw isn't packaged as a separate .lib/.dll today — you consume it as source.

1. Copy `CatsClaw/CatsClawLib.h` and `CatsClaw/CatsClawLib.cpp` into your project (keep them in the same directory — the header `#include`s the .cpp).
2. `#include "CatsClawLib.h"` wherever you need key generation or validation.
3. **Generate a key** (typically done once, out-of-band — e.g. in an internal tool you use to issue keys to customers, not inside the shipped app):

   ```cpp
   std::wstring key;          // e.g. "6F3A1-B2C9D-8E4F0-01A3C7"
   std::wstring keyArrays;    // diagnostic array of raw ASCII values, not needed at runtime
   int sum = 0;               // diagnostic checksum, not needed at runtime
   int licenseCount = 5;      // number of licenses/seats this key grants

   GenerateSerialKey(key, sum, keyArrays, licenseCount);
   ```

4. **Validate a key** against the license count the user claims to have (or the count you expect), inside your shipped application:

   ```cpp
   bool CheckALicenseKey(wchar_t* key, int licenseCount)
   {
       int score = 0;
       if (ValidateRectRegion1(key)) score += 100;
       if (ValidateRectRegion2(key)) score += 100; else return false;
       if (ValidateRectRegion3(key)) score += 100; else return false;
       if (ValidateRectRegion4(key, licenseCount)) score += 100; else return false;
       return score == 400;
   }

   if (CheckALicenseKey(&key[0], licenseCount))
       // key is valid for this license count
   else
       // key is invalid, or valid for a different license count
   ```

   The four checks are intentionally kept as separate calls (see `CatsClaw/CatsClaw.cpp`) rather than one combined function — this is a deliberate anti-tamper choice from the original design, making it harder to defeat every check with a single patched instruction. Keep that shape if you extend the validation logic.

### Security note

This is a decades-old, checksum-based scheme, not cryptographic license protection. Treat it as a basic deterrent against casual key sharing, not as protection against a determined attacker — the validation logic and constants (`LICENSE_SEED_KEY`, the modulus per segment) are visible to anyone who can read or disassemble your binary. Key **generation** code in particular should stay out of any binary you distribute to end users, since it lets you produce arbitrarily many valid keys.

## Status / known limitations

See `CLAUDE.md` for architecture notes and in-code TODOs. Notable ones:
- `Create*Key`/`ValidateRectRegion*` functions are intentionally near-duplicates (differ only by checksum modulus); a parameterized refactor is planned but not done.
- Only the plain-linked (`_IS_NOT_DLL`) build path is exercised; DLL export/import declarations exist but aren't verified.
- Key length is fixed at 20 significant characters (4 x 5); extending to 25 is a stated TODO.

## Updated

- 2024-06-05: Finally added README and CLAUDE.md documentation. I got Claude to write a CLAUDE.md file summarizing the architecture and usage notes.
