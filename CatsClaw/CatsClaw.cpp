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

