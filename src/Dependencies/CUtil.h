
#include<string>
#include<Windows.h>
using namespace std;


class CUtil
{
public:
	CUtil();
	~CUtil();
	wstring UTF8ToUnicode(const string & str);
	string UnicodeToUTF8(const wstring & str);
	wstring ANSIToUnicode(const string & str);
	string UnicodeToANSI(const wstring & str);
	LPCWSTR stringToLPCWSTR(std::string orig);
	wstring StringToWString(const std::string& str);
	string WStringToString(const std::wstring &wstr);
};


