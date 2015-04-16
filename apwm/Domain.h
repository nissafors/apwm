#ifndef DOMAIN_H
#define DOMAIN_H
#include <vector>
using namespace std;

class Domain
{
public:
	Domain();
	Domain(const string& name, const string& user, const string& pass);
	~Domain();
	bool writeDomain(const string& path);
	bool readDomain(const string& domain, const string& path);
	static vector<string> listDomains(const string& path);
	string getName();
	string getUser();
	string getPass();
private:
	unsigned char * pkcs5pad(const string& unpadded);
	string pkcs5unpad(const unsigned char* padded);
	string _name;
	unsigned char* _userPadded;
	unsigned char* _passPadded;
	unsigned char* _userEncrypted;
	unsigned char* _passEncrypted;
	
	void Char2Hex(const unsigned char ch, char* szHex);
	void Hex2Char(const char* szHex, unsigned char& rch);
	void CharStr2HexStr(const unsigned char* pucCharStr, char* pszHexStr, int iSize);
	void HexStr2CharStr(const char* pszHexStr, unsigned char* pucCharStr, int iSize);
};


#endif /* DOMAIN_H */