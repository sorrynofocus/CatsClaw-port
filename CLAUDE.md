# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

Cat's Claw is an old CD-key/product-license generation and validation library, originally written around 2004 and ported to Visual Studio 2019 in 2019. It's a small, self-contained C++ Windows console app (`CatsClaw.exe`) that demonstrates generating a 4-segment license key and then validating it. See `README.md` for the user-facing description, TL;DR, and build/usage instructions — this file covers architecture and dev-workflow details for working in the code, and shouldn't duplicate what's already in the README.

## Build

Windows-only (uses `<windows.h>`, `GetTickCount64`, `GetDriveType`, etc.). Build via Visual Studio or MSBuild:

```
msbuild CatsClaw.sln /p:Configuration=Debug /p:Platform=x64
msbuild CatsClaw.sln /p:Configuration=Release /p:Platform=x64
```

- Solution: `CatsClaw.sln`, single project `CatsClaw/CatsClaw.vcxproj`.
- Platform toolset: `v140` (VS2015), targeting Windows SDK `10.0.17763.0`. Open with VS2019+ which has the v140 toolset component installed, or retarget if it's missing.
- Configurations: Debug/Release x `Win32`/`x64`. Output goes to `CatsClaw\<Configuration>\`.
- No test suite, linter, or CI config exists in this repo.

Run the built exe from a terminal:
```
CatsClaw.exe                 # interactive: prompts for a site/user license value (1-100000)
CatsClaw.exe -a <licenseNum>  # non-interactive/automation mode
```

## Architecture

Two files hold all the logic:

- **`CatsClaw/CatsClaw.cpp`** — `main()`, a CLI demo harness. Parses `-a <n>` for automation mode, calls `GenerateSerialKey()` to produce a key, then calls `CheckALicenseKey()` twice (once with a deliberately wrong license value to show failure, once with the real value to show success).
- **`CatsClaw/CatsClawLib.h`** — the "header" that declares the public API. Unusually, it `#include`s `CatsClawLib.cpp` directly at the bottom (`#pragma once` guards it), so this is effectively a single-translation-unit header-only library, not a normal .h/.cpp pair. The file's own top-of-file TODOs describe known cleanup debt (fix prototypes vs actual definitions, split into DLL/LIB, support C#, refactor into a class, extend key length) — read them before making structural changes.
- **`CatsClaw/CatsClawLib.cpp`** — the actual implementation, organized in `#pragma region` blocks. Compilation is gated by preprocessor defines in the header: `_IS_DLL_EXPORT` / `_IS_DLL_IMPORT` / `_IS_NOT_DLL` select between DLL-exported, DLL-imported, or plain-linked declarations of the same functions. Currently only `_IS_NOT_DLL` is defined, so this always builds as a plain app, not a DLL.

### Key format

A license key is a string like `01234-01234-01234-01234` (four 5-character segments joined by `-`, total length `CD_KEY_LENGTH` = 23). Each segment has a fixed offset into the un-hyphenated key: `FIRSTKEY_OFFSET`=0, `SECONDKEY_OFFSET`=6, `THIRDKEY_OFFSET`=12, `FOURTHKEY_OFFSET`=18 (offsets count the dashes).

- `CreateFirstKey` / `CreateSecondKey` / `CreateThirdKey` each generate a random 5-char alphanumeric segment and return the checksum sum of its characters. Each has its own required checksum modulus baked in (mod 5, mod 10, mod 7 respectively for their last-character retry loop) — these three functions are near-duplicates of each other by design (see the TODOs).
- `CreateFourthKey` is **not** random: it derives the fourth segment deterministically from the 1st/3rd key sums and the requested license count via `DetermineLicenseValue()`, then encodes that value in base 16 via `NumToStr`. This is what ties a key to a specific license quantity.
- `DetermineLicenseValue(iLicValue, i1stKey, i3rdKey, iSeedKey)` combines those inputs plus a constant seed (`LICENSE_SEED_KEY` = 777) and a magic offset (`699050`) into a single int. Calling it again with the same inputs reproduces the same value — that's the basis of validation.
- `ValidateRectRegion1/2/3` re-sum each segment's characters and check the matching modulus (5/10/7) — mirrors of the `Create*Key` generation logic.
- `ValidateRectRegion4` (used in `CatsClaw.cpp`) and `ValidateFourthCDKey` (declared in the header but effectively superseded) both recompute `DetermineLicenseValue` from the key's embedded 1st/3rd segments and a caller-supplied license count, and compare it against the 4th segment. A key only validates for the specific license count it was generated with.
- `StrToIntW` / `StrToNum` / `NumToStr` are the low-level string↔number helpers (different bases, alphanumeric digit sets) used throughout key generation/validation.
- `e0a3`, `GetMonthDayLastAccess`, `CheckFileExistW` are legacy file-timestamp/anti-tamper helpers from the original 2004 use case (e.g. checking swap-file age via the registry for copy protection); they're not part of the core key generate/validate flow and are only lightly connected to the rest of the code.

### Design intent worth knowing before changing validation logic

The header comment above `CheckALicenseKey` in `CatsClaw.cpp` explains *why* the four validation checks are deliberately split into separate function calls rather than one combined check: it's meant to make it harder to defeat the whole check with a single patched instruction/NOP. Keep new validation logic similarly decomposed rather than collapsing it into one function.

### Known rough edges (from in-code TODOs — don't silently "fix" without checking intent)

- Function prototypes in `CatsClawLib.h` don't all match the definitions in `CatsClawLib.cpp` yet.
- The `Create*Key`/`Validate*Region*` functions are intentionally redundant (differ only by modulus); a planned refactor is to parameterize the modulus instead of duplicating the function body.
- Only `_IS_NOT_DLL` is currently wired up/tested; the DLL export/import paths are unverified.
- Key length is fixed at 20 significant chars (4x5); there's a stated intent to extend to 25.
