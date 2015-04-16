#include <string>
#include "Blowfish.h"
#include "Domain.h"
using namespace std;

Domain::Domain(const string& name, const string& user, const string& pass) : _name(name)
{
	// Pad
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
	if (_userPadded != nullptr)
	{
		delete _userPadded;
	}

	if (_passPadded != nullptr)
	{
		delete _passPadded;
	}

	if (_userEncrypted != nullptr)
	{
		delete _userEncrypted;
	}

	if (_passEncrypted != nullptr)
	{
		delete _passEncrypted;
	}
}

Domain* Domain::getDomain(const string& name)
{
	return new Domain("dummy", "dummy", "dummy");
}

string* Domain::listDomains(int& domainCount)
{
	domainCount = 2;
	string* dummy = new string[domainCount];
	dummy[0] = "String one";
	dummy[1] = "Second string";
	return dummy;
}


string Domain::getName()
{
	return string("dummy");
}

string Domain::getuser()
{
	return string("dummy");
}

string Domain::getPass()
{
	return string("dummy");
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
	return string("dummy");
}

void Domain::writeDomain()
{
}


void Domain::readDomain()
{
}

// The following four functions are borrowed from test.cpp from "A C++ Implementation of the Blowfish
// Encryption/Decryption method" by George Anescu
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