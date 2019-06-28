//CATS CLAW product key protection
//C.Winters 2019 / USA / CA / Culver City
//Original creation 2004 / USA / VA / Newport News
//thechriswinters@gmail.com

#define _CRT_SECURE_NO_WARNINGS

#pragma region RndGen(unsigned int uLowEndRange, unsigned int uHighEndRange) 
/// <summary>Generates a random number</summary>
/// <param name="uLowEndRange"> low end range of numbers to choose from </param>
/// <param name="uHighEndRange"> high end range of numbers to choose from  </param>
/// <returns> returns int of random number </returns>
int RndGen(unsigned int uLowEndRange, unsigned int uHighEndRange)
{
	int iSecretSauceNumber;
	do 
	{
	  iSecretSauceNumber = rand();
	} while (iSecretSauceNumber < ( (unsigned int) (RAND_MAX)+1) % (uHighEndRange + 1 - uLowEndRange) );
	
	return ( iSecretSauceNumber % (uHighEndRange + 1 - uLowEndRange) + uLowEndRange);
}
#pragma endregion



#pragma region int StrToIntW(wchar_t* sString)
/// <summary>This is a private function. Takes string and creates a sum out of it</summary>
/// <param name="sString"> string to create a strange sum </param>
/// <returns> return int of the sum of strange sum </returns>
int StrToIntW(wchar_t* sString)
{
	int iRetNumVal = 0;
	int num2 = 0;

	for (int i = 0; i <= wcslen(sString); i++)
	{
		if (sString[i] == '-') break;
		if (sString[i] == '\0') break;

		//Did we find an actual number? Turn the ASCII value (i.e. 'A'=65) to number actual
		if (sString[i] >= '0' && sString[i] <= '9')
			iRetNumVal = ((iRetNumVal * 10) + sString[i] - '0');

		else
		{
			//Did we find a lower case or upper case char? Get the position minus the A ACSII value and
			//add the previous saved value
			if (sString[i] >= 'a' && sString[i] <= 'z')
				iRetNumVal = iRetNumVal + sString[i] - 'a';
			else
				iRetNumVal = iRetNumVal + sString[i] - 'A';
		}
	}
	return (iRetNumVal);
}
#pragma endregion



//convert the string to number
//udata is string
//udatalen is length of string
//base is numerical base eg. for decimal it is 10
// for hex it is 16
//largest base supported here is upto 36
#pragma region int StrToNum(const TCHAR* sData, int uDataLen, int iBase)
/// <summary>This is a private function. Takes string and creates a sum out of it</summary>
/// <param name="const TCHAR* sData"> string to work with </param>
/// <param name="int uDataLen"> length of sData</param>
/// <param name="int iBase"> base use to help convert 2-26 only </param>
/// <returns> ... </returns>
int StrToNum(const TCHAR* sData, int uDataLen, int iBase)
{
	 //Let's pass in example string "6Chris" and base =10
	const  TCHAR arcDigitAlphas[] = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	TCHAR  sDigitAlphaRes[sizeof(arcDigitAlphas) + 1] = { '\0' };
	TCHAR* sDataVal = { '\0' };
	TCHAR  arData[512] = { '\0' };;

	long   iIndex;
	long   lDigitAlphaValue = 0;
	long   lRetVal = 0;

	ZeroMemory(sDigitAlphaRes, sizeof(sDigitAlphaRes));

	//copy the data to our variable
	wcscpy_s(arData, sData);

	//Turn all strings to upper. Now we will have L"6CHRIS"
	_wcsupr_s(arData);

	//copy the number of digits supported by base in digits

	//Copy 'base' amount from numdigits to digits. If 'base' 10 -> 0123456789. If 'base' 32 then ALL in string.
	// There's only 32 characters and 10 digits in alpha-numerical-bet.
	// digtis will have L"0123456789"
	wcsncpy_s(sDigitAlphaRes, arcDigitAlphas, iBase);

	for (iIndex = 0; iIndex < uDataLen; iIndex++)
	{
		//is the number there? If so, get the index and the rest of what we have in digits. For example if we found 6, the include 67890
		// Using example,, if any alphabet letters, then skip.
		//NOTE: If base 32 is used, then alphabet will be used.
		sDataVal = wcschr(sDigitAlphaRes, arData[iIndex]);

		if (sDataVal != 0)
		{
			//if so, subtract where to start point. Using example '6'
			//dataval = 6789 minus  L"0123456789" = 6
			lDigitAlphaValue = long(sDataVal - sDigitAlphaRes);
			//increment Retval with digitvalue
			//RetVal = 0, times 'base' =10, plus digitvalue = 6 - Total 6.
			lRetVal = lRetVal * iBase + lDigitAlphaValue;
		}
	}
	//return the result. Using example, the return val will be 70
	return (lRetVal);
}
#pragma endregion



#pragma region TCHAR* NumToStr(TCHAR* RetData, long lSumNumber, int iBase)
/// <summary>This is a private function. Takes string and -somewhat- reverses the sum StrToIntW created.</summary>
/// <param name="TCHAR* RetData">String to process</param>
/// <param name="long lSumNumber">long returned from StrToIntW()</param>
/// <param name="int iBase">Base to convert from, 2-26</param>
/// <returns> string of converted input string </returns>
TCHAR* NumToStr(TCHAR* RetData, long lSumNumber, int iBase)
{
	const TCHAR arcDigitAlphas[] = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	TCHAR sDigitAlphaRes[sizeof(arcDigitAlphas) + 1] ;
	TCHAR arcRetVal[512] = { '0' };

	long iIndexCount = 0;
	long lDigitAlphaIndexValue = 0;

	ZeroMemory(arcRetVal, sizeof(arcRetVal));
	ZeroMemory(sDigitAlphaRes, sizeof(sDigitAlphaRes));
	
	// only base supported are 2 to 36 (0-9 and A-Z) We could add a-z later.
	if (iBase < 2 || iBase > 36) 
		return NULL;

	wcsncpy_s(sDigitAlphaRes, arcDigitAlphas, iBase);
	while (lSumNumber)
	{
		lDigitAlphaIndexValue = lSumNumber % iBase;
		lSumNumber = lSumNumber / iBase;
		arcRetVal[iIndexCount++] = sDigitAlphaRes[lDigitAlphaIndexValue];
	}
	//since string we have is in reversed format
	//eg 100 will be 001 so we have to reverse it
	//and put the value in our variable
	ZeroMemory(RetData, wcslen(arcRetVal) + 1);

	int iRetDataIndex = 0;
	for (iIndexCount = wcslen(arcRetVal) - 1; iIndexCount > -1; iIndexCount--)
	{
		//start reversing
		RetData[iRetDataIndex++] = arcRetVal[iIndexCount];
	}
	//return the result
	return RetData;


	//Note: This code can also be used as a simple form of encryption. Here is a simple eg.:-

	//convert string "1024" to a number of base 10;
	//	TCHAR Data[512] = { '0' };
	//	wcscpy(Data, L"1024");
	//
	//	int Number = StrToNum(Data, wcslen(Data), 10);
	//	//convert the number to a different base 27;
	//	NumToStr(Data, Number, 27);
	//
	//	//now check the number
	//	std::cout << "\n\nModified String is " << Data << "\n";
	//
	//	//now again get back our original number
	//	Number = StrToNum(Data, wcslen(Data), 27);
	//
	//	NumToStr(Data, Number, 10);
	//
	//	std::cout << "\nOriginal Number is " << Data << "\n";
}
#pragma endregion




//---------------------------------------------------------------------------
// DetermineLicenseValue
// C. Winters created
// Determines or creates a license value.
// The license value is determined by a value inputted created along with
// with the first and third key. Once this is done a numerical value is
// returned. To simply verify, run this function again using the same values.
// iLicValue = the amount of licenses issued.
// i1stKey = the first key int sum value to be used. This value is from CreateFirstKey()
// i3rdKey = the third key int sum value to be used. This value is from CreaeThirdKey()
// iSeedKey = some pre-determined secret number to conjure up license number
//
#pragma region int DetermineLicenseValue(int iLicValue, int i1stKey, int i3rdKey, int iSeedKey)
/// <summary> Determines or creates a license value. 
/// The license value is determined by a value inputted created along with
/// with the first and third key. Once this is done a numerical value is
/// returned. To simply verify, run this function again using the same values.
/// </summary>
/// <param name="int iLicValue"> the amount of licenses issued. </param>
/// <param name="int i1stKey"> the first key int sum value to be used. This value is from CreateFirstKey() </param>
/// <param name="int i3rdKey"> the third key int sum value to be used. This value is from CreaeThirdKey() </param>
/// <param name="int iSeedKey"> some pre-determined secret number to conjure up license number </param>
/// <returns> The amount of licenses a site/individual will have. </returns>
int DetermineLicenseValue(int iLicValue, int i1stKey, int i3rdKey, int iSeedKey)
{
	int iFF = 0;
	iFF = i1stKey + i3rdKey + iLicValue + iSeedKey;

	//I'm not entirely sure why I picked this number to use when adding it to the value. 
	//I'd keep it. Lots of math! (tee hee)
	iFF += 699050;
	return (iFF);
}
#pragma endregion


#pragma region wchar_t ValidateRectRegion1(wchar_t* szKey)
/// <summary>   Validates the FIRST CD key in a license key.
///             ValidateCDKey will take either a regular or 'Other' CD Key
///             and verify of it's authentic CD Key which can be used to unlock
///             software when installing.
/// </summary>
/// <param name="wchar_t* szKey">  Key string to validate </param>
/// <returns> 
///              VALID_KEY          The key is valid
///              INVALID_KEY        The key is invalid
///              INVALID_LENGTH     The key is not a valid CD Key length
///              INVALID_CHAR       The key contains invalid characters
/// </returns>
/// TODO: The Validate(x) functions are redundant. Only thing that needs to change is the MOD amount. TODO is to pass in 
/// the original MOD key value used in the Create(X)Key().
wchar_t ValidateRectRegion1(wchar_t* szKey)
{
	unsigned long ulCheckSum = 0L;  // Hold our checksum value here
	int           iStrLength;       // Length of the CD Key
	int           i = 0;   // dummy counter

	iStrLength = wcslen(szKey);

	// is the key the correct length?
	if (iStrLength != CD_KEY_LENGTH)
		return (INVALID_LENGTH);

	// now move the pointer to the start of the REAL key
	szKey += iStrLength = FIRSTKEY_OFFSET;

	// now loop thru the CD Key, adding each digit to each other
	// and then mod it by 7.  If it's 0 we have a valid key!
	while (i < ACTUAL_KEY_LENGTH)
	{
		//In case we want to do a non-alphanumeric value
		//if (isdigit(*szKey))
		if (*szKey)
		{
			// convert each char to it's actual decimal value
			//and keep accumlating the values
			ulCheckSum += ((int)* szKey - 0);
		}
		else
			// we hit a NON numeric character
			return (INVALID_CHAR);
		szKey++;
		i++;
	}

	if (!(ulCheckSum % 5))
		// This IS a valid CD Key
		return (VALID_KEY);

	// This IS NOT a valid CD Key
	return (INVALID_KEY);
}
#pragma endregion


#pragma region wchar_t ValidateRectRegion2(wchar_t* szKey)
/// <summary>   Validates the SECOND CD key in a license key.
///             ValidateCDKey will take either a regular or 'Other' CD Key
///             and verify of it's authentic CD Key which can be used to unlock
///             software when installing.
/// </summary>
/// <param name="wchar_t* szKey">  Key string to validate </param>
/// <returns> 
///              VALID_KEY          The key is valid
///              INVALID_KEY        The key is invalid
///              INVALID_LENGTH     The key is not a valid CD Key length
///              INVALID_CHAR       The key contains invalid characters
/// </returns>
wchar_t ValidateRectRegion2(wchar_t* szKey)
{
	unsigned long ulCheckSum = 0L;  // Hold our checksum value here
	int           iStrLength;       // Length of the CD Key
	int           i = 0;   // dummy counter

	iStrLength = wcslen(szKey);

	// is the key the correct length?
	if (iStrLength != CD_KEY_LENGTH)
		return (INVALID_LENGTH);

	// now move the pointer to the start of the REAL key
	szKey += iStrLength = SECONDKEY_OFFSET;

	// now loop thru the CD Key, adding each digit to each other
	// and then mod it by 7.  If it's 0 we have a valid key!
	while (i < ACTUAL_KEY_LENGTH)
	{
		//In case we want to do a non-alphanumeric value
		//if (isdigit(*szKey))
		if (*szKey)
		{
			// convert each char to it's actual decimal value
			//and keep accumlating the values
			ulCheckSum += ((int)* szKey - 0);
		}
		else
			// we hit a NON numeric character
			return (INVALID_CHAR);
		szKey++;
		i++;
	}

	if (!(ulCheckSum % 10))
		// This IS a valid CD Key
		return (VALID_KEY);

	// This IS NOT a valid CD Key
	return (INVALID_KEY);
}
#pragma endregion



#pragma region wchar_t ValidateRectRegion3(wchar_t* szKey)
/// <summary>  Validates the THIRD CD key in a license key.
///             ValidateCDKey will take either a regular or 'Other' CD Key
///             and verify of it's authentic CD Key which can be used to unlock
///             software when installing.
/// </summary>
/// <param name="wchar_t* szKey">  Key string to validate </param>
/// <returns> 
///              VALID_KEY          The key is valid
///              INVALID_KEY        The key is invalid
///              INVALID_LENGTH     The key is not a valid CD Key length
///              INVALID_CHAR       The key contains invalid characters
/// </returns>
wchar_t ValidateRectRegion3(wchar_t* szKey)
{
	unsigned long ulCheckSum = 0L;  // Hold our checksum value here
	int           iStrLength;       // Length of the CD Key
	int           i = 0;   // dummy counter

	iStrLength = wcslen(szKey);

	// is the key the correct length?
	if (iStrLength != CD_KEY_LENGTH)
		return (INVALID_LENGTH);

	// now move the pointer to the start of the REAL key
	szKey += iStrLength = THIRDKEY_OFFSET;

	// now loop thru the CD Key, adding each digit to each other
	// and then mod it by 7.  If it's 0 we have a valid key!
	while (i < ACTUAL_KEY_LENGTH)
	{
		//In case we want to do a non-alphanumeric value
		//if (isdigit(*szKey))
		if (*szKey)
		{
			// convert each char to it's actual decimal value
			//and keep accumlating the values
			ulCheckSum += ((int)* szKey - 0);
		}
		else
			// we hit a NON numeric character
			return (INVALID_CHAR);
		szKey++;
		i++;
	}

	if (!(ulCheckSum % 7))
		// This IS a valid CD Key
		return (VALID_KEY);

	// This IS NOT a valid CD Key
	return (INVALID_KEY);
}
#pragma endregion


//---------------------------------------------------------------------------
#pragma region wchar_t ValidateRectRegion4(wchar_t* szKey, int iLvn)
/// <summary>    Validates the FOURTH CD key in a license key.
///             ValidateCDKey will take either a regular or 'Other' CD Key
///             and verify of it's authentic CD Key which can be used to unlock
///             software when installing.
/// </summary>
/// <param name="wchar_t* szKey">  Key string to validate </param>
/// <param name=" int iLvn"> the first key int sum value to be used. This value is from CreateFirstKey() </param>
/// <param name="i3rdKey"> the third key int sum value to be used. This value is from CreaeThirdKey() </param>
/// <param name="iSeedKey"> Some pre-determined secret number to conjure up license number </param>
/// <returns> 
///              VALID_KEY          The key is valid
///              INVALID_KEY        The key is invalid
///              INVALID_LENGTH     The key is not a valid CD Key length
///              INVALID_CHAR       The key contains invalid characters
/// </returns>
bool ValidateRectRegion4(wchar_t* szKey, int iLvn)
{
	int           iStrLength;
	int           i, ii, iiii, iFF, fk = 0;

	if (iLvn == 0)
		return (false);

	iStrLength = wcslen(szKey);
	wchar_t* FirstKey, * ThirdKey, * FourthKey; 
	
	// is the key the correct length?
	if (iStrLength != CD_KEY_LENGTH)
		return (INVALID_LENGTH);

	FirstKey = ThirdKey = FourthKey = szKey;
	// now move the pointer to the start of the REAL key
	FirstKey += iStrLength = FIRSTKEY_OFFSET;
	ThirdKey += iStrLength = THIRDKEY_OFFSET;
	FourthKey += iStrLength = FOURTHKEY_OFFSET;

	 //First, we generate the fourth key and we ned to get an INT from it.
	 //Then we get the INT from the 1ST and 3RD key
	 // Add the LICENSE value to the 1ST key
	i = StrToIntW(ThirdKey);
	ii = StrToIntW(FirstKey);
	iiii = StrToIntW(FourthKey);

	fk = StrToNum(FourthKey, wcslen(FourthKey), 16);
	iFF = DetermineLicenseValue(iLvn, ii, i, LICENSE_SEED_KEY);

	if (iFF == fk) 
		return (true); 
	else 
		return (false);
}
#pragma endregion


#pragma region int CreateFirstKey(std::wstring & RandomString, std::wstring& RandomNumbers)
/// <summary>Creates the FIRST part of the product key. 
/// This function should NOT be included in the application as a key can be created. 
/// The key is in this format: 01234-01234-01234-01234-01234 
/// </summary>
/// <param name="wstring & RandomString">Address to string to return the first part of the key </param>
/// <param name="wstring& RandomNumbers"> </param>
/// <returns> Sum of the first key created. This value will be used with the DetermineLicenseValue() </returns>
/// TODO: The Create(X)Key() functions are redundant. Only thing that needs to change is the MOD amount of last lines. TODO is to pass in 
/// the user defined MOD key value.
int CreateFirstKey(std::wstring & RandomString, std::wstring& RandomNumbers)
{
	int Sum = 0;
	int RandomNumber;

	// srand(GetTickCount());
	srand((unsigned int)(23 + 1 * 45 * GetTickCount64()));

	//Sleep(TICK_DELAY);

		//lower case %26+ +97
	for (BYTE i = 0; i < 5; i++)
	{
		if (i % 2 == 0)
		{
			do 
			{
				//Sleep(50);
				//rand() % (x+x+x) =10 digits, 26 upper, 26 lower
				//Notice the  10 MINUS 26+26? This generates characters
				RandomNumber = RndGen(0, 500) % (10 - 26 + 26);
				//RandomNumber = rand() % (10+26+26);

				if (RandomNumber >= 10 + 26)
					RandomNumber += 'a' - 10 - 26;
				//RandomNumber += 'A' - 10 +26;

				else if (RandomNumber >= 10)
					RandomNumber += 'A' - 10;
				else
					RandomNumber += '0';
			}
			// repeat last number till ok
			while (i == 4 && (RandomNumber + Sum) % 5);
		}
		else
		{
			do 
			{
				//rand() % (x+x+x) =10 digits, 26 upper, 26 lower
				//Notice the  10 PLUS 26+26? This generates characters

				//This will generate lower-case with uppercase
				//	RandomNumber = RndGen (0, 500) % (10+26+26);

				//Only interested in uppercase for now...
				RandomNumber = RndGen(0, 500) % (10 + 26);

				//RandomNumber = rand() % (10+26+26);
				if (RandomNumber >= 10 + 26)
					RandomNumber += 'a' - 10 - 26;
				else if (RandomNumber >= 10)
					RandomNumber += 'A' - 10;
				else
					RandomNumber += '0';
			}
			// repeat last number till ok
			while (i == 4 && (RandomNumber + Sum) % 5);
		}

		RandomString += (char)RandomNumber;
		RandomNumbers += (int)RandomNumber;
		RandomNumbers += '-';
		Sum += RandomNumber;
	}
	return (Sum);
}
#pragma endregion



#pragma region int CreateSecondKey(std::wstring & RandomString, std::wstring& RandomNumbers)
/// <summary>Creates the SECOND part of the product key. 
/// This function should NOT be included in the application as a key can be created. 
/// The key is in this format: 01234-01234-01234-01234-01234 
/// </summary>
/// <param name="wstring & RandomString">Address to string to return the first part of the key </param>
/// <param name="wstring& RandomNumbers"> </param>
/// <returns> Sum of the first key created. This value will be used with the DetermineLicenseValue() </returns>
/// TODO: The Create(X)Key() functions are redundant. Only thing that needs to change is the MOD amount of last lines. TODO is to pass in 
/// the user defined MOD key value.
int CreateSecondKey(std::wstring & RandomString, std::wstring & RandomNumbers)
{
	int Sum = 0;
	int RandomNumber;

	//lower case %26+ +97
	for (BYTE i = 0; i < 5; i++)
	{
		if (i % 2 == 0)
		{
			do 
			{
				//rand() % (x+x+x) =10 digits, 26 upper, 26 lower
				//Notice the  10 PLUS 26+26? This generates characters

				//This will generate lower-case with uppercase
				//RandomNumber = RndGen (0, 500) % (10+26+26);

				//Only interested in uppercase for now...
				RandomNumber = RndGen(0, 500) % (10 + 26);

				//RandomNumber = rand() % (10+26+26);
				if (RandomNumber >= 10 + 26)
					RandomNumber += 'a' - 10 - 26;
				else if (RandomNumber >= 10)
					RandomNumber += 'A' - 10;
				else
					RandomNumber += '0';
			}
			// repeat last number till ok
			while (i == 4 && (RandomNumber + Sum) % 10);
		}
		else
		{
			do 
			{
				//rand() % (x+x+x) =10 digits, 26 upper, 26 lower
				//Notice the  10 MINUS 26+26? This generates characters
				RandomNumber = RndGen(0, 500) % (10 - 26 + 26);
				//RandomNumber = rand() % (10+26+26);
				if (RandomNumber >= 10 + 26)
					RandomNumber += 'a' - 10 - 26;
				else if (RandomNumber >= 10)
					RandomNumber += 'A' - 10;
				else
					RandomNumber += '0';
			}
			// repeat last number till ok
			while (i == 4 && (RandomNumber + Sum) % 10);
		}

		RandomString += (char)RandomNumber;
		RandomNumbers += (int)RandomNumber;
		RandomNumbers += '-';
		Sum += RandomNumber;
	}
	return (Sum);
}
#pragma endregion



#pragma region int CreateThirdKey(std::wstring & RandomString, std::wstring& RandomNumbers)
/// <summary>Creates the THIRD part of the product key. 
/// This function should NOT be included in the application as a key can be created. 
/// The key is in this format: 01234-01234-01234-01234-01234 
/// </summary>
/// <param name="wstring & RandomString">Address to string to return the first part of the key </param>
/// <param name="wstring& RandomNumbers"> </param>
/// <returns> Sum of the first key created. This value will be used with the DetermineLicenseValue() </returns>
/// TODO: The Create(X)Key() functions are redundant. Only thing that needs to change is the MOD amount of last lines. TODO is to pass in 
/// the user defined MOD key value.
int CreateThirdKey(std::wstring &RandomString, std::wstring &RandomNumbers)
{
	int Sum = 0;
	int RandomNumber =0;

	// srand(GetTickCount());

	//Sleep(TICK_DELAY);

	//lower case %26+ +97
	for (BYTE i = 0; i < 5; i++)
	{
		if (i % 2 == 0)
		{
			do 
			{
				//rand() % (x+x+x) =10 digits, 26 upper, 26 lower
				//Notice the  10 MINUS 26+26? This generates characters

				//This will generate lower-case with uppercase
				//	RandomNumber = RndGen (0, 500) % (10+26+26);

				//Only interested in uppercase for now...
				RandomNumber = RndGen(0, 500) % (10 + 26);

				//RandomNumber = rand() % (10+26+26);
				if (RandomNumber >= 10 + 26)
					RandomNumber += 'a' - 10 - 26;
				else if (RandomNumber >= 10)
					RandomNumber += 'A' - 10;
				else
					RandomNumber += '0';
			}
			// repeat last number till ok
			while (i == 4 && (RandomNumber + Sum) % 7);
		}
		else
		{
			do {
				//Sleep(50);
				//rand() % (x+x+x) =10 digits, 26 upper, 26 lower
				//Notice the  10 PLUS 26+26? This generates characters

								//This will generate lower-case with uppercase
								//	RandomNumber = RndGen (0, 500) % (10+26+26);

								//Only interested in uppercase for now...
				RandomNumber = RndGen(0, 500) % (10 + 26);

				//RandomNumber = rand() % (10+26+26);
				if (RandomNumber >= 10 + 26)
					RandomNumber += 'a' - 10 - 26;
				else if (RandomNumber >= 10)
					RandomNumber += 'A' - 10;
				else
					RandomNumber += '0';
			}
			// repeat last number till ok
			while (i == 4 && (RandomNumber + Sum) % 7);
		}

		RandomString += (char)RandomNumber;
		RandomNumbers += (int)RandomNumber;
		RandomNumbers += '-';
		Sum += RandomNumber;
	}
	return (Sum);
}
#pragma endregion



#pragma region void CreateFourthKey(std::wstring sz1stKey, std::wstring sz3rdKey, std::wstring &szValue, int iLicenseValue)
/// <summary>Creates the FOURTH part of the product key.
/// Uses FIRST and THIRD part of key and license value to create a FOURTH pat of key
/// This function should NOT be included in the application as a key can be created. 
/// The key is in this format: 01234-01234-01234-01234-01234 
/// </summary>
/// <param name="wstring sz1stKey">string of the first part of the key </param>
/// <param name="wstring sz3rdKey">string of the third part of the key </param>
/// <param name="wstring &szValue">returns the fourth part of the key</param>
/// <param name="int iLicenseValue">The amount of licenses to the site/individual </param>
/// <returns> none</returns>
/// TODO: The Create(X)Key() functions are redundant. Only thing that needs to change is the MOD amount of last lines. TODO is to pass in 
/// the user defined MOD key value.
void CreateFourthKey(std::wstring sz1stKey, std::wstring sz3rdKey, std::wstring &szValue, int iLicenseValue)
{

	int i1stKey, i3rdKey, iReturnNum = 0;
	int iFF = 0;

	wchar_t szCData[10] = { '\0' };
	ZeroMemory(szCData, sizeof(szCData));

	if (iLicenseValue == 0)
		iLicenseValue = 1;

	//Get the INTs from 1st and 3rd key
	//i1stKey = _wtoi(sz1stKey.c_str());
	i1stKey = StrToIntW (  & sz1stKey[0] );
	//i3rdKey = _wtoi(sz3rdKey.c_str());
	i3rdKey = StrToIntW( & sz3rdKey[0] );

	//Add the license value to the 1st key
	//i1stKey += Sum;
	//iReturnNum = i1stKey + i3rdKey + iLicenseValue;

	iFF = DetermineLicenseValue(iLicenseValue, i1stKey, i3rdKey, LICENSE_SEED_KEY);
	NumToStr(szCData, iFF, 16);

	// iFF = iFF +  StrToNum(szCData, wcslen(szCData), 16);

	szValue = szCData;
	// return (szValue);

}
#pragma endregion




//---------------------------------------------------------------------------
#pragma region bool ValidateFourthCDKey(wchar_t* szKey, int iLicenseValue)
/// <summary>    Validates the FOURTH CD key in a license key.
///             ValidateCDKey will take either a regular or 'Other' CD Key
///             and verify of it's authentic CD Key which can be used to unlock
///             software when installing.
/// </summary>
/// <param name="wchar_t* szKey">  Key string to validate </param>
/// <param name="int iLicenseValue"> the first key int sum value to be used. This value is from CreateFirstKey() </param>
/// <returns> true if the fourth key is valid, false if not
/// </returns>
bool ValidateFourthCDKey(wchar_t* szKey, int iLicenseValue)
{
	int           iStrLength;
	int           i, ii, iFF, fk = 0;

	if (iLicenseValue == 0)
		return (false);

	iStrLength = wcslen(szKey);
	wchar_t* FirstKey, * ThirdKey, * FourthKey;

	// is the key the correct length?
	if (iStrLength != CD_KEY_LENGTH)
		return (INVALID_LENGTH);

	FirstKey = ThirdKey = FourthKey = szKey;
	// now move the pointer to the start of the REAL key
	FirstKey += iStrLength = FIRSTKEY_OFFSET;
	ThirdKey += iStrLength = THIRDKEY_OFFSET;
	FourthKey += iStrLength = FOURTHKEY_OFFSET;

	i = _wtoi(ThirdKey);
	ii = _wtoi(FirstKey);
	fk = _wtoi(FourthKey);

	iFF = DetermineLicenseValue(iLicenseValue, ii, i, LICENSE_SEED_KEY);

	if (iFF >= 99999)
		iFF -= 1000;
	if (iFF == fk)
		return (true);
	else
		return (false);
}
#pragma endregion

//---------------------------------------------------------------------------
#pragma region void GenerateSerialKey(std::wstring& SKey, int& iSum, std::wstring& szKeyArrays, int iLicenseValueNumber)
/// <summary>   Generate a full fledge product number key
/// </summary>
/// <param name="wstring &SKey"> Returned string of the key created </param>
/// <param name="int& iSum"> The sum of the created key </param>
/// <param name="wstring& szKeyArrays"> The formatted array of the created key </param>
/// <param name="int iLicenseValueNumber"> The amount of licenses for the individual or site</param>
/// <returns> none 
/// </returns>
void GenerateSerialKey(std::wstring &SKey, int& iSum, std::wstring & szKeyArrays, int iLicenseValueNumber)
{
	//Generate a full fledge CD serial number key...
	//create key: aaaaa-bbb-CCCCCCC-XXXXX
	//where a,b,c are created, but X is not.

	//Assign default license if one isn't passed.
	//If the default iLVNValue (LicenseValueNumber is also 0 then default everything to a sincel license.
	if (iLicenseValueNumber == 0)
	{
		if (iLVNValue == 0)
		{
			iLVNValue = 1;
			iLicenseValueNumber = iLVNValue;
		}
	}

	//Returns
	//SKey = Serial Number generated
	//iSum - Check value  (summed up values all all generated keys (excluding '-'s)
	//       This could be used as a 'license key'
	//szKeyArrays - array of ASCII decimals values of each keys in elements.
	//              AAA =65-65-65. the '-'s are added to each bit to seperate values.

	std::wstring FirstSKeyArray;
	std::wstring SecondSKey;
	std::wstring SecondSKeyArray;
	std::wstring ThirdSKey;
	std::wstring ThirdSKeyArray;
	std::wstring FourthKey;
	std::wstring FirstSKey;

	int FirstRandomNumber = CreateFirstKey(FirstSKey, FirstSKeyArray);
	int SecondRandomNumber = CreateSecondKey(SecondSKey, SecondSKeyArray);
	int ThirdRandomNumber = CreateThirdKey(ThirdSKey, ThirdSKeyArray);
	CreateFourthKey(FirstSKey, ThirdSKey, FourthKey, iLicenseValueNumber);

	Sleep(50);

	//SKey += FirstSKey + "-" + SecondSKey + "-" + ThirdSKey + "-" + FourthKey;
	SKey.append( FirstSKey);
	SKey.append(L"-");
	SKey.append(SecondSKey);
	SKey.append(L"-");
	SKey.append(ThirdSKey);
	SKey.append(L"-");
	SKey.append(FourthKey);


	szKeyArrays += FirstSKeyArray + SecondSKeyArray + ThirdSKeyArray;
	iSum += FirstRandomNumber + SecondRandomNumber + ThirdRandomNumber;
}
#pragma endregion
//---------------------------------------------------------------------------



#pragma region bool CheckFileExistW(wchar_t* szFileName, wchar_t* szReturn)
/// <summary>  Checks the existence of a file on all available fixed drives, including CD-ROMs
/// This ws a really old style to check if a file is around or not. FindFirst() FindNext supercedes this.
/// </summary>
/// <param name="wchar_t* szFileName"> The source filename to check/test </param>
/// <param name="wchar_t* szReturn"> return value of where the file is</param>
/// <returns> true if found, false otherswise. 
/// </returns>
bool CheckFileExistW(wchar_t* szFileName, wchar_t* szReturn)
{
	std::wstring  strPath;
	wchar_t cDrive = L'A';
	do
	{
		strPath = (cDrive) + L":\\";
		int iRes = GetDriveType(strPath.c_str());

		if ((iRes != 0) && (iRes != 1) && (iRes != 2) && (iRes != 5))
		{
			wchar_t szSoonRtn[255] = { '\0' };
			wcscpy(szSoonRtn, strPath.c_str());
			wcscat(szSoonRtn, szFileName);

			if (_waccess(szSoonRtn, 0) != -1)
			{
				wcscpy(szReturn, szSoonRtn);
				return (true);
			}
		}
		cDrive++;
	} while (cDrive <= 'Z');
	wcscpy(szReturn, L"\0");
	return (false);
}
#pragma endregion



// Getting serial number for any drives listed...
//  int no;
// DWORD dwno;
// GetVolumeInformation( (cdrompaths+no*4), NULL,NULL,&dwno,NULL,NULL,NULL,NULL



#pragma region unsigned __int64 e0a3(wchar_t* szFileName)
/// <summary>  computes how many hours a file was on a system
/// This was used within some applications to determine the age of the swap file (TASMan)
/// When TASMan was installed, the age of the swap file was stored in the registry after encrypted.
/// Then, used with the cat's claw product key validations, if the age was different then the "copy protection" would fail.
/// Since swap files hardly changed or removed, this was good for additional copy potection. Of course, this was so 2003.
/// But this function CAN be used for other purposes. The function name was "mangled" to keep from finding it easy.
/// </summary>
/// <param name="wchar_t* szFileName"> The source filename to determine the amount of hours it has been on the system </param>
/// <returns> returns unsigned int amount of hours the file has been on the system
/// </returns>
unsigned __int64 e0a3(wchar_t* szFileName)
{
	wchar_t buffer[255] = { '\0' };
	unsigned __int64 ui64Now, ui64Then, ui64Hours;
	ui64Now = ui64Then = ui64Hours = 0;

	HANDLE fHandle;
	FILETIME ftCreateTime, ftLastAccess, ftLastWrite, fSysTime ;
	SYSTEMTIME stLastWrite;
	WIN32_FIND_DATA ftWriteLast;
	fHandle = FindFirstFile(szFileName, &ftWriteLast);

	GetSystemTime(&stLastWrite);
	SystemTimeToFileTime(&stLastWrite, &fSysTime);

	ui64Then = (((unsigned __int64)ftWriteLast.ftCreationTime.dwHighDateTime) << 32) + ftWriteLast.ftCreationTime.dwLowDateTime;
	ui64Now = (((unsigned __int64)fSysTime.dwHighDateTime) << 32) + fSysTime.dwLowDateTime;

	ui64Hours = (ui64Now - ui64Then) / 10000000 / 60 / 60;
	FindClose(fHandle);
	return (ui64Hours);
}
#pragma endregion



//Gets the last accessed date of a file
#pragma region std::wstring GetMonthDayLastAccess(std::wstring szFileName)
/// <summary>  Gets the last accessed date of a file
/// There are easier functions that can do this now, but this gets the last accessed date of a file
/// </summary>
/// <param name="wstring szFileName"> The source filename to determine the last accessed date </param>
/// <returns> returns string of month-day
/// </returns>
std::wstring GetMonthDayLastAccess(std::wstring szFileName)
{
	wchar_t buffer[50] = { '\0' };
	std::wstring szBuffer;
	szBuffer.clear();
	//wcscpy(buffer, L"\0");
	//szBuffer = "";
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	FILETIME ft;
	ft.dwLowDateTime = 0;
	ft.dwHighDateTime = 0;

	SYSTEMTIME st;

	HANDLE hFile;
	hFile = CreateFile(szFileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	GetFileTime(hFile, &ftCreationTime, &ftLastAccessTime, &ftLastWriteTime);
	//  ft = ftCreationTime;
	ft = ftLastAccessTime;

	FileTimeToLocalFileTime(&ft, &ft);
	FileTimeToSystemTime(&ft, &st);
	CloseHandle(hFile);

	//swprintf(buffer, "%d-%d", st.wMonth, st.wDay);

	swprintf_s(buffer, 50, L"%d-%d", st.wMonth, st.wDay);
	szBuffer = buffer;
	//010307 - suspicious pointer conversion. Fixed. -cw
	return (buffer);
}
#pragma endregion



/*
// EXAMPLE APPLICATION TO TEST CATS CLAW KEYGEN/VALIDATION
//
// CatsClaw.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

//Include Cat's Claw functionality
#include "CatsClawLib.h"

// Command line main program stuff.
char* szInput;
bool  bAutomation = false;

// Global site license value. This value is the amount of liceneses issued to a site or individual
int iUserLicenseVal = 1;

//////////////////////////////////////////////////////////////////////////////
// Quick function to test our generated license product key.
// If the key checks out, then return TRUE, if not return FALSE
// This is broken up so you can later sprinkle it throughout your application.
// That way it would be hard to track down and later you can check the "iKeychkVal" value later.
// If this was all done in one place, then someone could always issue a NOOP instruction during app patching
// causing to skip the checks in that one particular place.
//////////////////////////////////////////////////////////////////////////////
bool CheckALicenseKey(wchar_t* sKey, int iSiteLic)
{
	int iKeychkVal = 0;
	bool bGoodKey = ValidateRectRegion1(sKey);

	if (bGoodKey == true)
		iKeychkVal = 100;

	bGoodKey = ValidateRectRegion2(sKey);
	if (bGoodKey == true)
		iKeychkVal += 100;

	bGoodKey = ValidateRectRegion3(sKey);
	if (bGoodKey == true)
		iKeychkVal += 100;
	else
		iKeychkVal = 0;

	bGoodKey = ValidateRectRegion4(sKey, iSiteLic);
	if (bGoodKey == true)
		iKeychkVal += 100;
	else
		iKeychkVal = 0;

	if (iKeychkVal == 400)
		return (true);
	else
		return (false);
}


int main(int argc, char* argv[])
{
	int iArgsLoopCounter;
	for (iArgsLoopCounter = 1; iArgsLoopCounter < argc; iArgsLoopCounter++)
	{

		if ((*argv[iArgsLoopCounter] != '-'))
			break;

		else if (strcmp(argv[iArgsLoopCounter], "-") == 0)
		{
			std::cout << "If you are going to use -a for automation, then you must enter a number.";
			exit(1);
		}

		else if (strcmp(argv[iArgsLoopCounter], "-a") == 0)
		{
			szInput = argv[++iArgsLoopCounter];
			if (szInput == NULL)
			{
				std::cout << "If you are going to use -a for automation, then you must enter a number.";
				exit(1);
			}
			else
			{
				iUserLicenseVal = atoi(szInput);
				bAutomation = true;
			}
		}
	}

	std::cout << "\n\nThis is the Cat's Claw security license key gen/validator. Written in early 2000, is is now ported ";
	std::cout << "to 2019. \nSimply enter a site license value. This value is how many licenses a person or company has. ";
	std::cout << "\nA key is generated from the amount of licenses.\n";

	if (!bAutomation)
	{
		std::cout << "\n\nEnter a site/user license value (1-100000)\n";
		std::cout << "NOTE: You can run this program again with -a {license value} to automate.\n";
		std::cin >> iUserLicenseVal;
	}


	// Store the license key array here. Really not needed in real world.
	std::wstring sKeyAr;

	// Store the license key in this value. This will be used in real world.
	std::wstring sTest;

	//This is the sum of the license key. Not really used in real world as of yet.
	int lSumValue = 0;

	//////////////////////////////////////////////////////////////////////////////
	// // // // // // // // // // // // // // // // // // // // // // // // // //
	//WE GENERATE THE LICENSE KEY HERE!
	GenerateSerialKey(sTest, lSumValue, sKeyAr, iUserLicenseVal);
	// // // // // // // // // // // // // // // // // // // // // // // // // //
	//////////////////////////////////////////////////////////////////////////////

	std::wcout << "Key             : " << sTest << "\nSum check value : " << lSumValue << "\nKey Array       : " << sKeyAr << "\nSite license    : " << iUserLicenseVal << "\n";
	std::wcout << "\n\nNow, we shall fail the license key test (on purpose), and then pass. We will use a site license of 1 as failure (unless you entered a 1) and then we shall pass it by user entry\n";


	// // // // // // // // // // // // // // // // // // // // // // // // // //
	//////////////////////////////////////////////////////////////////////////////
	// TEST OUR GENERATED LICENSE KEY WITH FALSE VALUE AND THEN POSTIVE ONE.
	std::wcout << "\n\nTEST 1\n";

	// latest c-lang uses &sTest[0] instead of c_str()
	if (CheckALicenseKey(&sTest[0], 1))
		std::wcout << "This is a VALID key";
	else
		std::wcout << "This is an INVALID key";

	std::wcout << "\n\nTEST 2\n";
	if (CheckALicenseKey(&sTest[0], iUserLicenseVal))
		std::wcout << "This is a VALID key";
	else
		std::wcout << "This is an INVALID key";
	//////////////////////////////////////////////////////////////////////////////
	// // // // // // // // // // // // // // // // // // // // // // // // // //

}

*/



/*
Experimental - old
	int iRefund = 0;

	wchar_t sTestStr1[521] = { '\0' };
	const size_t sizeBufferLength = 15;

	//wcsncpy(sTestStr1, L"Chris", sizeBufferLength + 1);
	wcscpy(sTestStr1, L"6Chris");

//	iRefund = StrToIntW((wchar_t *)L"6Chris");

	iRefund = StrToNum( (wchar_t*)L"6Chris", 7, 10);

	wcscpy(sTestStr1, NumToStr(sTestStr1, 6411, 10) );
*/