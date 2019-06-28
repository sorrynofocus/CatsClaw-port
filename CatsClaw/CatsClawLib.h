/*
CATS CLAW product key protection
C. Winters 2019 / USA / CA / Culver City
Original creation 2004 / USA / VA / Newport News
thechriswinters@gmail.com

CATS CLAW HEADER FILE
This is hte header file to include in your MAIN application. CATS CLAW has been ported to VS2019 using 2015 V140 toolset. 

You'll notice below that the function are defined many time. This is because it was used as a library and a DLL.
In time I will fix the function prototypes because they do not match now with the functions ported in the catsclawlib.cpp

As a matter of fact read the TODOs



TODO:
- Convert proper function names into protypes and fix header file.
- SPLIT the key generation and key validation into DLLs and libraries (.DLL and .LIB)
- Able to compile and use in C++ and C#
- Convert functions into a class after refactoring the functions. There are redundant ones and the seed values can be used in a singular fashion.
- Extend the product key as 25 characters instead of 20.

*/


#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#define _IS_NOT_DLL

//To see pragma message below, - Messages are only displayed in the IDE if Show general messages is checked on the C++ Project Properties under Project > Options > Project Propertes.

//CD Key Format - CDKey 01234-01234-01234-01234\n"
//                5 elements each in 4 keys
//remember:
//day to hours hours = day*24
//hours to days = days=hours/24
//minutes = totalseconds /60 %60
//hours = totalseconds/3600 %24
//days = totalsseconds/3600 /24
//60seconds= 1 minute
//3600 sec = 1 hour
//86,400 sec = 1 day
//604,800 sec = 1 week

//Definitions for ValidateCDKey function
//Determine validity
#define VALID_KEY      1
#define INVALID_KEY    0
#define INVALID_CHAR   0
#define INVALID_LENGTH 0

//Length of WHOLE CD key -> 01234-01234-01234-01234-01234 =29
#define CD_KEY_LENGTH 23

//Length of the key in each element is 5 chars. There are four key elements.
#define ACTUAL_KEY_LENGTH  5

// The keys offset (ref:  0- first character, 6th character of the key, and so on)
#define FIRSTKEY_OFFSET  0
#define SECONDKEY_OFFSET 6
#define THIRDKEY_OFFSET  12
#define FOURTHKEY_OFFSET 18
#define FIFTHKEY_OFFSET  24

//Seed key to help determine a licesne against a key generated. [default: 777]
#define LICENSE_SEED_KEY 777

//Timer Tick delay to generate a random number for creating key [default: 1000]
#define TICK_DELAY 1000

//This is the license value. Simply assign a license number to this
//for amount of licenses used. (default 1)
//If you assign this var a '0', then the License Value number will reset back to a single license (1);
int iLVNValue = 1;


#define DllImport   __declspec( dllimport )
#define DllExport   __declspec( dllexport )

#ifdef __cplusplus

extern "C"
{
#endif

#ifdef _IS_DLL_EXPORT
#pragma message ("IS A DLL -EXPORT")
	wchar_t DllExport ValidateRectRegion1(wchar_t* szKey);
	wchar_t DllExport ValidateRectRegion2(wchar_t* szKey);
	wchar_t DllExport ValidateRectRegion3(wchar_t* szKey);
	bool DllExport ValidateRectRegion4(wchar_t* szKey, int iLvn);
	unsigned __int64 DllExport e0a3(wchar_t* szFileName);
	bool DllExport nR20Po(wchar_t* szFileName, wchar_t* szReturn);
	int DllExport DetermineLicenseValue(int iLicValue, int i1stKey, int i3rdKey, int iSeedKey);
	String DllExport GetMonthDayLastAccess(String szFileName);

	int DllExport CreateFirstKey(String& RandomString, String& RandomNumbers);
	int DllExport CreateSecondKey(String& RandomString, String& RandomNumbers);
	int DllExport CreateThirdKey(String& RandomString, String& RandomNumbers);
	void DllExport CreateFourthKey(String sz1stKey, String sz3rdKey, String& szValue, int iLicenseValue);
	bool DllExport ValidateFourthCDKey(char* szKey, int iLicenseValue);
	void DllExport GenerateSerialKey(String& SKey, int& iSum, String& szKeyArrays, int iLicenseValueNumber);


#elif _IS_DLL_IMPORT
#pragma message ("IS A DLL -IMPORT")
	wchar_t DllImport ValidateRectRegion1(wchar_t* szKey);
	wchar_t DllImport ValidateRectRegion2(wchar_t* szKey);
	wchar_t DllImport ValidateRectRegion3(wchar_t* szKey);
	bool DllImport ValidateRectRegion4(wchar_t* szKey, int iLvn);
	unsigned __int64 DllImport e0a3(wchar_t* szFileName);
	bool DllImport nR20Po(wchar_t* szFileName, wchar_t* szReturn);
	int DllImport DetermineLicenseValue(int iLicValue, int i1stKey, int i3rdKey, int iSeedKey);
	String DllExport GetMonthDayLastAccess(String szFileName);

	int DllImport CreateFirstKey(String& RandomString, String& RandomNumbers);
	int DllImport CreateSecondKey(String& RandomString, String& RandomNumbers);
	int DllImport CreateThirdKey(String& RandomString, String& RandomNumbers);
	void DllImport CreateFourthKey(String sz1stKey, String sz3rdKey, String& szValue, int iLicenseValue);
	bool DllImport ValidateFourthCDKey(char* szKey, int iLicenseValue);
	void DllImport GenerateSerialKey(String& SKey, int& iSum, String& szKeyArrays, int iLicenseValueNumber);

#endif



#ifdef __cplusplus
	//End of the 'extern "C"' block
}
#endif

#ifdef _IS_NOT_DLL
#pragma message ("IS NOT A DLL -LIB or SRC")
wchar_t ValidateRectRegion1(wchar_t* szKey);
wchar_t ValidateRectRegion2(wchar_t* szKey);
wchar_t ValidateRectRegion3(wchar_t* szKey);
bool ValidateRectRegion4(wchar_t* szKey, int iLvn);
unsigned __int64 e0a3(wchar_t* szFileName);
bool nR20Po(wchar_t* szFileName, wchar_t* szReturn);
int DetermineLicenseValue(int iLicValue, int i1stKey, int i3rdKey, int iSeedKey);
//std::wstring DllExport GetMonthDayLastAccess(std::wstring szFileName);
std::wstring GetMonthDayLastAccess(std::wstring szFileName);
int  CreateFirstKey(std::wstring& RandomString, std::wstring& RandomNumbers);
int  CreateSecondKey(std::wstring& RandomString, std::wstring& RandomNumbers);
int  CreateThirdKey(std::wstring& RandomString, std::wstring& RandomNumbers);
void CreateFourthKey(std::wstring sz1stKey, std::wstring sz3rdKey, std::wstring& szValue, int iLicenseValue);
bool ValidateFourthCDKey(wchar_t* szKey, int iLicenseValue);
void GenerateSerialKey(std::wstring& SKey, int& iSum, std::wstring& szKeyArrays, int iLicenseValueNumber);
#endif


#pragma once

#include "CatsClawLib.cpp"

