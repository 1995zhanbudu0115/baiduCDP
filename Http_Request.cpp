#include "Http_Request.h"
#include <set>
#include <stdexcept>
#ifdef _DEBUG
#pragma comment(lib,"libcurld.lib")
#else
#pragma comment(lib,"libcurl.lib")
#endif
HttpRequest::HttpRequest()
:m_CurlHandle(NULL),
headerList(nullptr),
m_Cookies("")
{
	try{
		CURLcode rcode = curl_global_init(CURL_GLOBAL_ALL);
		if (rcode)
			throw rcode;
		m_CurlHandle = curl_easy_init();
		if (m_CurlHandle)
		{
			curl_easy_setopt(m_CurlHandle, CURLOPT_SSL_VERIFYPEER, false);	// ��֤�Է���SSL֤��
			curl_easy_setopt(m_CurlHandle, CURLOPT_SSL_VERIFYHOST, false);	//����������֤֤�������
		}
	}
	catch (CURLcode& errCode)
	{
		//�����־
		throw std::runtime_error("LibCurl initialization environment failed");
	}
}

HttpRequest::~HttpRequest()
{
	if (headerList)
	{
		curl_slist_free_all(headerList);
		headerList = nullptr;
	}
	if (m_CurlHandle)
	{
		curl_easy_cleanup(m_CurlHandle);
		m_CurlHandle = nullptr;
	}
	curl_global_cleanup();	//�ر�curl����
}

void HttpRequest::SetRequestHeader(const std::string strKey, const std::string strValue)
{
	std::string strRequestHeader(strKey + ":" + strValue);
	headerList = curl_slist_append(headerList, strRequestHeader.c_str());
}

void HttpRequest::SetRequestHeader(RequestHeaderValue& HeaderValue)
{
	for (auto &v : HeaderValue)
	{
		std::string strRequestHeader(v.first + ":" + v.second);
		headerList = curl_slist_append(headerList, strRequestHeader.c_str());
	}
}

void HttpRequest::Send(RequestType Enumtype, const std::string strUrl,const std::string strPost)
{
	CURLcode dwCurlCode;
	if (!m_CurlHandle)return;
	m_Request.clear();
	m_ResponseHeader.clear();
	m_Cookies.clear();
	switch (Enumtype)
	{
	case GET:
	{
				AutoAddHeader();
				if (!CheckHeaderExist("Referer:"))
				{
					headerList = curl_slist_append(headerList, ("Referer:" + strUrl).c_str());
				}
				//����URL
				curl_easy_setopt(m_CurlHandle, CURLOPT_URL, strUrl.c_str());
				curl_easy_setopt(m_CurlHandle, CURLOPT_HTTPHEADER, headerList);	//�����Զ���Э��ͷͷ
				curl_easy_setopt(m_CurlHandle, CURLOPT_HEADERFUNCTION, header_callback);	//���ûص�Э��ͷ����
				curl_easy_setopt(m_CurlHandle, CURLOPT_HEADERDATA, &m_ResponseHeader);
				curl_easy_setopt(m_CurlHandle, CURLOPT_WRITEFUNCTION, read_callback);  //����д�Ļص�����
				curl_easy_setopt(m_CurlHandle, CURLOPT_WRITEDATA, &m_Request);	//���շ��ص�����
				curl_easy_setopt(m_CurlHandle, CURLOPT_NOBODY, 0);	//���շ��ص�����
				dwCurlCode = curl_easy_perform(m_CurlHandle);
				if (CURLE_OK != dwCurlCode)
				{
					//�����־
				}
				m_Cookies = GetCookies(GetallResponseHeaders());

	}
	break;
	case POST:
	{
				 AutoAddHeader();
				 if (!CheckHeaderExist("Referer:"))
				 {
					 headerList = curl_slist_append(headerList, ("Referer:" + strUrl).c_str());
				 }
				 if (!CheckHeaderExist("Content-Type:"))
				 {
					 headerList = curl_slist_append(headerList, ("Content-Type: application/x-www-form-urlencoded"));
				 }
				 //����URL
				 curl_easy_setopt(m_CurlHandle, CURLOPT_URL, strUrl.c_str());
				 curl_easy_setopt(m_CurlHandle, CURLOPT_HTTPHEADER, headerList);	//�����Զ���Э��ͷͷ
				 curl_easy_setopt(m_CurlHandle, CURLOPT_POSTFIELDSIZE, (long)strPost.length());	//������Ҫ���͵����ݴ�С
				 curl_easy_setopt(m_CurlHandle, CURLOPT_POSTFIELDS, strPost.c_str());	//������Ҫ���͵�����
				 curl_easy_setopt(m_CurlHandle, CURLOPT_HEADERFUNCTION, header_callback);	//���ûص�Э��ͷ����
				 curl_easy_setopt(m_CurlHandle, CURLOPT_HEADERDATA, &m_ResponseHeader);
				 curl_easy_setopt(m_CurlHandle, CURLOPT_WRITEFUNCTION, read_callback);  //����д�Ļص�����
				 curl_easy_setopt(m_CurlHandle, CURLOPT_WRITEDATA, &m_Request);	//���շ��ص�����
				 curl_easy_setopt(m_CurlHandle, CURLOPT_NOBODY, 0);	//���շ��ص�����
				 dwCurlCode = curl_easy_perform(m_CurlHandle);
				 if (CURLE_OK != dwCurlCode)
				 {
					 //�����־
				 }
				 m_Cookies = GetCookies(GetallResponseHeaders());
	}
	break;
	case HEAD:
	{
				 AutoAddHeader();
				 if (!CheckHeaderExist("Referer:"))
				 {
					 headerList = curl_slist_append(headerList, ("Referer:" + strUrl).c_str());
				 }
				 //����URL
				 curl_easy_setopt(m_CurlHandle, CURLOPT_URL, strUrl.c_str());
				 curl_easy_setopt(m_CurlHandle, CURLOPT_HTTPHEADER, headerList);	//�����Զ���Э��ͷͷ
				 curl_easy_setopt(m_CurlHandle, CURLOPT_HEADERFUNCTION, header_callback);	//���ûص�Э��ͷ����
				 curl_easy_setopt(m_CurlHandle, CURLOPT_HEADERDATA, &m_ResponseHeader);
				 curl_easy_setopt(m_CurlHandle, CURLOPT_WRITEFUNCTION, read_callback);  //����д�Ļص�����
				 curl_easy_setopt(m_CurlHandle, CURLOPT_WRITEDATA, &m_Request);	//���շ��ص�����
				 curl_easy_setopt(m_CurlHandle, CURLOPT_NOBODY, 1);	//���շ��ص�����
				 dwCurlCode = curl_easy_perform(m_CurlHandle);
				 if (CURLE_OK != dwCurlCode)
				 {
					 //�����־
				 }
				 m_Cookies = GetCookies(GetallResponseHeaders());
	}
	default:
		break;
	}
	if (headerList)
	{
		curl_slist_free_all(headerList);
		headerList = nullptr;
	}
}

std::string HttpRequest::GetResponseText()
{
	std::string strResultText;
	for (auto v: m_Request)
		strResultText.append(1, v);
	return strResultText;
}

responseData HttpRequest::GetResponseBody() const
{
	return m_Request;
}

std::string HttpRequest::GetallResponseHeaders()
{
	std::string strResultText;
	for (auto v : m_ResponseHeader)
		strResultText.append(1, v);
	return strResultText;
}

void HttpRequest::SetRequestCookies(const std::string strCookie)
{
	if (!m_CurlHandle)return;
	curl_easy_setopt(m_CurlHandle, CURLOPT_COOKIE, strCookie.c_str());
	
}

void HttpRequest::SetHttpRedirect(bool val)
{
	if (!m_CurlHandle)return;
	curl_easy_setopt(m_CurlHandle, CURLOPT_FOLLOWLOCATION, (long)val);
}

std::string HttpRequest::GetResponCookie() const
{
	return m_Cookies;
}

void HttpRequest::AutoAddHeader()
{
	if (!headerList || !CheckHeaderExist("Accept:"))
	{
		headerList = curl_slist_append(headerList, "Accept:*/*");
	}
	if (!CheckHeaderExist("Accept-Language:"))
	{
		headerList = curl_slist_append(headerList, "Accept-Language:zh-cn");
	}
	if (!CheckHeaderExist("User-Agent:"))
	{
		headerList = curl_slist_append(headerList, std::string(std::string("User-Agent:") + USER_AGENT).c_str());
	}
}

std::string HttpRequest::GetCookies(const std::string strHeader)
{
	std::string strResult, strTwoResult;
	std::vector<std::string> headerList;
	std::set<std::string> resultList;
	if (!strHeader.empty())
	{
		std::string split = strHeader + "\r\n";
		std::string strsub;
		size_t pos = 0;
		size_t npos = 0;
		npos = split.find("\r\n", pos);
		while (npos != std::string::npos)
		{
			strsub = split.substr(pos, npos - pos);
			pos = npos + strlen("\r\n");
			npos = split.find("\r\n", pos);
			headerList.push_back(strsub);
		}
	}
	for (auto & v : headerList)
	{
		std::string strTmp;
		int nPos = v.find("Set-Cookie:");
		if (nPos != std::string::npos)
		{
			std::string strTmp = v.substr(nPos + lstrlenA("Set-Cookie:"), v.length() - (nPos + lstrlenA("Set-Cookie:")));;
			if (strTmp.at(strTmp.length() - 1) != ';')
			{
				strTmp += ";";
			}
			strResult += strTmp;
		}
	}
	headerList.clear();
	if (!strResult.empty())
	{
		if (strResult.at(strResult.length() - 1) != ';')
			strResult += ";";
		std::string split = strResult;
		std::string strsub;
		size_t pos = 0;
		size_t npos = 0;
		npos = split.find(";", pos);
		while (npos != std::string::npos)
		{
			strsub = split.substr(pos, npos - pos);
			pos = npos + strlen(";");
			npos = split.find(";", pos);
			resultList.insert(strsub);
		}
	}
	else
	{
		return strResult;
	}

	for (auto& s : resultList)
		strTwoResult += s + ";";
	return strTwoResult;
}

std::vector<std::string> HttpRequest::StrSplit(const std::string &str, const std::string &pattern)
{
	std::vector<std::string> resultVec;
	std::string split = str;
	std::string strsub;
	size_t pos = 0;
	size_t npos = 0;
	npos = split.find(pattern, pos);
	while (npos != std::string::npos)
	{
		strsub = split.substr(pos, npos - pos);
		pos = npos + pattern.length();
		npos = split.find(pattern, pos);
		resultVec.push_back(strsub);
	}
	return resultVec;
}
RequestHeaderValue HttpRequest::SplitCookie(const std::string strCookie)
{
	RequestHeaderValue result;
	std::vector<std::string> strCookieArr = StrSplit(strCookie, ";");
	for (auto v : strCookieArr)
	{
		int nPos = std::string::npos;
		int nNext = std::string::npos;
		nPos = v.find("=");
		if (nPos!=std::string::npos)
		{
			std::string name = v.substr(0, nPos);
			std::string value = v.substr(nPos + 1, v.length() - (nPos + 1));
			result.insert({ name,value });

		}
	}
	return result;
}
std::string HttpRequest::MergeCookie(const std::string wornCookie, const std::string newCookie)
{
	std::string strCookies;
	std::string strSource;
	std::string strDest;
	strSource = wornCookie;
	strDest = newCookie;
	if (strSource.at(strSource.length() - 1) != ';')
		strSource += ";";
	if (strDest.at(strDest.length() - 1) != ';')
		strDest += ";";
	RequestHeaderValue strResult = SplitCookie(strSource);
	RequestHeaderValue strNewCookie = SplitCookie(strDest);
	for (auto v : strNewCookie)
	{
		std::string key = v.first;
		auto it = strResult.find(key);
		if (it!=strResult.end())
		{
			strResult.find(key)->second = v.second;
		}
		else
		{
			strResult.insert({ v.first,v.second });
		}
	}
	for (auto k: strResult)
		strCookies += k.first + "=" + k.second + ";";
#if 0
	for (auto v : strCookieArr)
	{
		int npos = std::string::npos;
		int nNextPos = std::string::npos;
		RequestHeaderValue item;
		item = SplitCookie(v);
		if (item.size()==2)
		{
			std::string strName = item.at("name");
			std::string strKey = item.at("key");
			npos = strResult.find(strName);
			if (npos != std::string::npos)
			{
				nNextPos = strResult.find(";", npos);
				if (nNextPos!=std::string::npos)
				{
					strResult = strResult.replace(npos + strName.length(), nNextPos, strKey, 0, strKey.length());
				}
				
			}
			else
			{
				strResult += v + ";";
			}
		}
	}
#endif
	return strCookies;
}

bool HttpRequest::CheckHeaderExist(const std::string strHeaderKey)
{
	bool isBresult = false;
	struct curl_slist* TmpheaderList = headerList;
	while (TmpheaderList)
	{
		std::string tmpHeaderstr = TmpheaderList->data;
		if (tmpHeaderstr.find(strHeaderKey)!=std::string::npos)
		{
			isBresult = true;
			break;
		}
		TmpheaderList = TmpheaderList->next;
	}
	return isBresult;
}

size_t HttpRequest::header_callback(char *ptr, size_t size, size_t nmemb, void* userdata)
{
	size_t lsize = size * nmemb;
	for (size_t i = 0; i < lsize; i++)
		((responseData*)userdata)->push_back(ptr[i]);
	return lsize;
}

size_t HttpRequest::read_callback(char *ptr, size_t size, size_t nmemb, void* userdata)
{
	size_t lsize = size * nmemb;
	for (size_t i = 0; i < lsize; i++)
		((responseData*)userdata)->push_back(ptr[i]);
	return lsize;
}

