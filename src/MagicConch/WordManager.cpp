#include <algorithm>
#include "WordManager.h"

#pragma comment(lib,"winhttp.lib")
#pragma comment(lib,"user32.lib")

using namespace std;

/*WString转String*/
string WStringToString(const std::wstring &wstr)
{
	string str;
	int nLen = (int)wstr.length();
	str.resize(nLen, ' ');
	int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);
	if (nResult == 0)
	{
		return "";
	}
	return str;
}

/*String转Wstring*/
std::wstring String2WString(const std::string& s)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const char* chSrc = s.c_str();
	size_t nDestSize = mbstowcs(NULL, chSrc, 0) + 1;
	wchar_t* wchDest = new wchar_t[nDestSize];
	wmemset(wchDest, 0, nDestSize);
	mbstowcs(wchDest, chSrc, nDestSize);
	std::wstring wstrResult = wchDest;
	delete[]wchDest;
	setlocale(LC_ALL, strLocale.c_str());
	return wstrResult;
}

void WordManager::add(const string &w, const string &meaning, string &st, const int &stage)
{
	Word word = { w,meaning, MTime::to_MTime(st, YMD_MODE),stage };
	list.push_back(word);
}

WordManager::WordManager()
{
	//cout << "start" << endl;
}

string WordManager::searchword(string word)
{
	CUtil c;
	wstring wsword = L"translate?&doctype=json&type=AUTO&i=" + c.StringToWString( word);
	LPCWSTR pwszVerb = wsword.c_str();
	string restr;

	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer = NULL;
	HINTERNET  hSession = NULL, hConnect = NULL, hRequest = NULL;
	BOOL  bResults = FALSE;
	hSession = WinHttpOpen(L"User Agent", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);//不使用user agent建立会话
	if (hSession)
	{
		hConnect = WinHttpConnect(hSession, L"fanyi.youdao.com", INTERNET_DEFAULT_HTTP_PORT, 0);
	}//会话(session)建立成功，开始连接（connect）
	if (hConnect)
	{
	    //hRequest = WinHttpOpenRequest(hConnect, L"GET", L"translate?&doctype=json&type=AUTO&i=中国", L"HTTP/1.1", WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
		hRequest = WinHttpOpenRequest(hConnect, L"GET", pwszVerb, L"HTTP/1.1", WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
	}//连接（connect）建立成功，开始发送请求（request）
	if (hRequest)
	{
		bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
	}//请求（request）被服务器响应，发出真正请求
	if (bResults)
	{
		bResults = WinHttpReceiveResponse(hRequest, NULL);
	}//接受服务器返回结果
	if (bResults)
	{
		do
		{
			// 检查可用数据
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
			{
				//cout << "Error" << GetLastError() << " in WinHttpQueryDataAvailable.\n";
				break;
			}
			if (!dwSize)
				break;

			pszOutBuffer = new char[dwSize + 1];

			if (!pszOutBuffer)
			{
				//cout << "Out of memory\n";
				break;
			}

			ZeroMemory(pszOutBuffer, dwSize + 1);

			if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
			{
				//cout << "Error" << GetLastError() << "in WinHttpReadData.\n";
			}
			else
			{
				//cout<<pszOutBuffer;  
				Json::Reader reader;
				Json::Value root;
				if (reader.parse(pszOutBuffer, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
				{
					//cout << root["type"].asString()<<endl;
					const Json::Value arrayObj = root["translateResult"][0][0];
					
					if (root["type"] == "ZH_CN2EN")
					{
						restr = arrayObj["tgt"].asString();
					}
					else if(root["type"] == "EN2ZH_CN")
					{
						restr = arrayObj["tgt"].asString();
					}
					else
					{
						restr = "Error Type!";
					}
					
					//restr = "jiadehailuo";
					//restr = c.UnicodeToANSI(c.UTF8ToUnicode(arrayObj["tgt"].asString()));
					//wstring out = c.UTF8ToUnicode(arrayObj["tgt"].asString());
					//restr = c.UnicodeToUTF8(out);
					//cout << "翻译结果：";
					//wcout.imbue(locale("chs"));
					//wcout << out << endl;
				
				}

			}



			delete[] pszOutBuffer;

			if (!dwDownloaded)
				break;

		} while (dwSize > 0);
	}


	return restr;
}

void WordManager::saveword(MTime time, string word, string meaning, int stage)
{
	Word newword = { word,meaning ,time,stage };
	list.push_back(newword);
}

string WordManager::returnword(int num)
{
	string temp;
	int count = 0;

	for (auto w = list.begin(); w != list.end(); w++)
	{
		if (count< num)
		{
			if (w != list.begin())
			{
				temp += "\n";
			}
			temp += w->word;
			temp += ": ";
			temp += w->meaning;
			w->stage++;
		}
		else
		{
			break;
		}
	}

	return temp;
}

void WordManager::sortList()
{
	sort(list.begin(), list.end(), WordManager::sortFunc);
}

bool WordManager::sortFunc(Word &w1, Word &w2)
{
	return w1.stage > w1.stage;
}

void WordManager::update()
{
	for (vector<Word>::iterator iter = list.begin(); iter != list.end();)
	{
		if (iter->stage > MAX_STAGE)
		{
			iter = list.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	sortList();

	return;
}

WordManager::~WordManager()
{
	//cout << "end" << endl;
}



