#include "WordManager.h"
#include "Dependencies/json.h"
#include "Dependencies/CUtil.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <winhttp.h>
#include <vector>

#pragma comment(lib,"winhttp.lib")
#pragma comment(lib,"user32.lib")
using namespace std;

WordManager::WordManager()
{
	//cout << "start" << endl;
}

string WordManager::searchword(string word)
{
	CUtil c;
	wstring wsword = L"translate?&doctype=json&type=AUTO&i=" + c.StringToWString( word);
	LPCWSTR pwszVerb = wsword.c_str();
	string restr="";

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
				cout << "Error" << GetLastError() << " in WinHttpQueryDataAvailable.\n";
				break;
			}
			if (!dwSize)
				break;

			pszOutBuffer = new char[dwSize + 1];

			if (!pszOutBuffer)
			{
				cout << "Out of memory\n";
				break;
			}

			ZeroMemory(pszOutBuffer, dwSize + 1);

			if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
			{
				cout << "Error" << GetLastError() << "in WinHttpReadData.\n";
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
					wstring out = c.UTF8ToUnicode(arrayObj["tgt"].asString());
					restr = "翻译结果："+c.UnicodeToANSI(out);
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

void WordManager::saveword(MTime time, string word, int stage=0)
{
	Word newword = { word,time,stage };
	list.push_back(newword);
	//string restr = word+"已保存";
	//return restr;
}

vector<Word> * WordManager::returnword(int num)
{
	int t = 0, s3 = 0, s2 = 0, s1 = 0, s0 = 0;
	vector<Word> *p = new vector<Word>;
	for (vector<Word>::iterator iter = list.begin(); iter != list.end(); iter++)
	{
		while (iter->stage==3)
		{
			p->push_back(*iter); 
			s3++;
			if (s3 >= num)break;
			else 
			{
				while (iter->stage == 2)
				{
					p->push_back(*iter);
					s2++;
					if (s3 + s2 >= num)break;
					else
					{
						while (iter->stage == 1)
						{
							p->push_back(*iter);
							s1++;
							if (s3 + s2 + s1 >= num)break;
							else
							{
								while (iter->stage == 0)
								{
									p->push_back(*iter);
									s0++;
									if (s3 + s2 + s1 + s0 >= num)break;
								}
							}
							break;
						}
					}
					break;
				}
			}
			break;
		}
	}
	return p;
}

void WordManager::update()
{
	for (vector<Word>::iterator iter = list.begin(); iter != list.end(); iter++)
	{
		if (iter->stage > 3)
			list.erase(iter);
	}
	return;
}

WordManager::~WordManager()
{
	//cout << "end" << endl;
}



