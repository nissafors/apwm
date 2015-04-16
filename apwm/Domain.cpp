#include <string>
#include <exception>
#include <fstream>
#include <vector>
#include "Blowfish.h"
#include "Domain.h"
using namespace std;


// Public constructors
Domain::Domain()
{
	_name = "";
	_userPadded = _passPadded = _userEncrypted = _passEncrypted = nullptr;
}

Domain::Domain(const string& name, const string& user, const string& pass) : _name(name)
{
	// Pad string to a length that is a multiple of 8 bytes, which is a CBlowfish requirement
	_userPadded = pkcs5pad(user);
	_passPadded = pkcs5pad(pass);

	// Dim, create and empty buffers
	int userBufLen = strlen((char*)_userPadded);
	int passBufLen = strlen((char*)_passPadded);
	_userEncrypted = new unsigned char[userBufLen + 1];
	_passEncrypted = new unsigned char[passBufLen + 1];
	memset(_userEncrypted, 0, userBufLen + 1);
	memset(_passEncrypted, 0, passBufLen + 1);

	// Encrypt
	// TODO - master password
	CBlowFish blowfish((unsigned char*)"default_key23456", 16);
	blowfish.Encrypt(_userPadded, _userEncrypted, userBufLen);
	blowfish.Encrypt(_passPadded, _passEncrypted, passBufLen);
}

Domain::~Domain()
{
	// Clear and delete buffers
	if (_userPadded != nullptr)
	{
		memset(_userPadded, 0, strlen((char*)_userPadded));
		delete _userPadded;
	}
	if (_passPadded != nullptr)
	{
		memset(_passPadded, 0, strlen((char*)_passPadded));
		delete _passPadded;
	}
	if (_userEncrypted != nullptr)
	{
		memset(_userEncrypted, 0, strlen((char*)_userEncrypted));
		delete _userEncrypted;
	}
	if (_passEncrypted != nullptr)
	{
		memset(_passEncrypted, 0, strlen((char*)_passEncrypted));
		delete _passEncrypted;
	}
}

bool Domain::writeDomain(const string& path)
{
	bool success = false;

	// Write this domain to file
	if (_userEncrypted != nullptr && _passEncrypted != nullptr)
	{
		// Dim and create buffers
		int userBufLen = strlen((char*)_userEncrypted);
		int passBufLen = strlen((char*)_passEncrypted);
		char* userHex = new char[userBufLen * 2 + 1];
		char* passHex = new char[passBufLen * 2 + 1];

		// Convert encrypted strings to hex-strings
		CharStr2HexStr(_userEncrypted, userHex, userBufLen);
		CharStr2HexStr(_passEncrypted, passHex, passBufLen);

		// Open file and append this domain
		ofstream outFile;
		outFile.open(path, ios::out | ios::app);
		if (outFile.is_open())
		{
			outFile << "Domain: " << _name << endl << userHex << endl << passHex << endl;
			success = true;
			outFile.close();
		}

		// Clean up buffers
		memset(userHex, 0, userBufLen * 2);
		memset(passHex, 0, userBufLen * 2);
		delete userHex;
		delete passHex;
	}

	return success;
}

bool Domain::readDomain(const string& domain, const string& path)
{
	bool success = false;

	// Make sure this is an empty domain
	if (_userPadded != nullptr || _passPadded != nullptr)
		return success;

	string name, userHex, passHex;
	ifstream inFile;
	inFile.open(path);
	if (inFile.is_open())
	{
		while (getline(inFile, name))
		{
			if (name.erase(0, 8).compare(domain) == 0)
			{
				// Found our domain in file. Get username and password and close the file.
				_name = name;
				getline(inFile, userHex);
				getline(inFile, passHex);
				inFile.close();

				// Prepare buffers
				int userBufLen = userHex.length() / 2;
				int passBufLen = passHex.length() / 2;
				unsigned char* user = new unsigned char[userBufLen + 1];
				unsigned char* pass = new unsigned char[passBufLen + 1];
				_userPadded = new unsigned char[userBufLen + 1];
				_passPadded = new unsigned char[passBufLen + 1];
				memset(user, 0, userBufLen + 1);
				memset(pass, 0, passBufLen + 1);
				memset(_userPadded, 0, userBufLen + 1);
				memset(_passPadded, 0, passBufLen + 1);

				// Convert from hex string to unsigned char string that can be used by blowfish
				HexStr2CharStr(userHex.c_str(), user, userBufLen);
				HexStr2CharStr(passHex.c_str(), pass, passBufLen);

				// Decrypt
				// TODO: Master password
				CBlowFish blowfish((unsigned char*)"default_key23456", 16);
				blowfish.Decrypt(user, _userPadded, userBufLen);
				blowfish.Decrypt(pass, _passPadded, passBufLen);

				// Done. Clean up.
				delete user;
				delete pass;
				success = true;
				break;
			}
		}
		// Domain not found. Close file.
		inFile.close();
	}

	return success;
}

vector<string> Domain::listDomains(const string& path)
{
	vector<string> domains;
	string line;
	ifstream inFile;
	inFile.open(path);
	if (inFile.is_open())
	{
		while (getline(inFile, line))
		{
			if (line.substr(0, 8).compare("Domain: ") == 0)
				domains.push_back(line.erase(0, 8));
		}
		inFile.close();
	}

	return domains;
}


string Domain::getName()
{
	return _name;
}

string Domain::getUser()
{
	if (_userPadded == nullptr)
		return "";
	else
		return pkcs5unpad(_userPadded);
}

string Domain::getPass()
{
	if (_passPadded == nullptr)
		return "";
	else
		return pkcs5unpad(_passPadded);
}

unsigned char* Domain::pkcs5pad(const string& unpadded)
{
	// The original string will be padded by 8-(strlen mod 8) chars of value 8-(strlen mod 8)
	int mLen = unpadded.length();
	int psLenVal = 8 - (mLen % 8);
	unsigned char* padded = new unsigned char[mLen + psLenVal + 1];
	strcpy((char*)padded, unpadded.c_str());
	// Add padding
	memset(padded + mLen, psLenVal, psLenVal);
	// Terminate c-string
	memset(padded + mLen + psLenVal, 0, 1);

	return padded;
}

string Domain::pkcs5unpad(const unsigned char* padded)
{
	// Remove padding from a string passed through pkcs5pad()
	string unpadded = (char*)padded;
	int lastCharVal = (int)unpadded.back();
	if (lastCharVal > 0 && lastCharVal < 8)
		unpadded.erase(unpadded.length() - lastCharVal, lastCharVal);

	return unpadded;
}

// The following four functions are borrowed from test.cpp from "A C++ Implementation of the Blowfish
// Encryption/Decryption method" by George Anescu. See BLOWFISH_LICENSE for licensing details.
// http://www.codeproject.com/Articles/1400/A-C-Implementation-of-the-Blowfish-Encryption-Decr


//Function to convert unsigned char to string of length 2
void Domain::Char2Hex(const unsigned char ch, char* szHex)
{
	unsigned char byte[2];
	byte[0] = ch / 16;
	byte[1] = ch % 16;
	for (int i = 0; i<2; i++)
	{
		if (byte[i] >= 0 && byte[i] <= 9)
			szHex[i] = '0' + byte[i];
		else
			szHex[i] = 'A' + byte[i] - 10;
	}
	szHex[2] = 0;
}

//Function to convert string of length 2 to unsigned char
void Domain::Hex2Char(const char* szHex, unsigned char& rch)
{
	rch = 0;
	for (int i = 0; i<2; i++)
	{
		if (*(szHex + i) >= '0' && *(szHex + i) <= '9')
			rch = (rch << 4) + (*(szHex + i) - '0');
		else if (*(szHex + i) >= 'A' && *(szHex + i) <= 'F')
			rch = (rch << 4) + (*(szHex + i) - 'A' + 10);
		else
			break;
	}
}

//Function to convert string of unsigned chars to string of chars
void Domain::CharStr2HexStr(const unsigned char* pucCharStr, char* pszHexStr, int iSize)
{
	int i;
	char szHex[3];
	pszHexStr[0] = 0;
	for (i = 0; i<iSize; i++)
	{
		Char2Hex(pucCharStr[i], szHex);
		strcat(pszHexStr, szHex);
	}
}

//Function to convert string of chars to string of unsigned chars
void Domain::HexStr2CharStr(const char* pszHexStr, unsigned char* pucCharStr, int iSize)
{
	int i;
	unsigned char ch;
	for (i = 0; i<iSize; i++)
	{
		Hex2Char(pszHexStr + 2 * i, ch);
		pucCharStr[i] = ch;
	}
}