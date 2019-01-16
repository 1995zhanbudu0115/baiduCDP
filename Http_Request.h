#ifndef __HTTP_REQUEST__
#define __HTTP_REQUEST__
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <curl/curl.h>
#define USER_AGENT "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:64.0) Gecko/20100101 Firefox/64.0"
/*
 ��������HTTPЭ������
 ����ʹ��libcurl
*/
typedef void* RequestResultPtr;
typedef std::vector<char> responseData;
/*
http ���������
*/
typedef enum EnumRequestType
{
	GET,
	POST,
	HEAD
}RequestType;
/*
��������Э��ͷ��map����
*/
typedef std::map<std::string, std::string> RequestHeaderValue;
class HttpRequest
{
public:
	HttpRequest();
	~HttpRequest();
	/*
	���������Э��ͷ ��KEY value ��ʽ����
	��key = "user-agent"
	value = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.100 Safari/537.36"
	*/
	void SetRequestHeader(const std::string strKey, const std::string strValue);
	/*
	���������Э��ͷ ��map���ͷ�ʽ����
	*/
	void SetRequestHeader(RequestHeaderValue& HeaderValue);
	/*
	����HTTP����
	Enumtype http ���������
	strPost �� Enumtype =POST ʱ������
	*/
	void Send(RequestType Enumtype,const std::string strUrl,const std::string strPost="");
	/*
	��ȡ��ȡhttp������Ӧ����ҳ������
	*/
	std::string GetResponseText();
	/*
	��ȡ��ȡhttp������Ӧ����ҳ�Ķ��������� ��image File��
	*/
	responseData GetResponseBody() const;
	/*
	��ȡhttp������Ӧ��ͷ����
	*/
	std::string GetallResponseHeaders();
	/*
	���������õ���cookie
	*/
	void SetRequestCookies(const std::string strCookie);
	/*
	�������߽�ֹ�ض���
	*/
	void SetHttpRedirect(bool val);
	/*
	 ��ȡ��������ķ��ص�cookies
	*/
	std::string GetResponCookie() const;
	/*
	�ϲ�cookies
	*/
	std::string MergeCookie(const std::string wornCookie, const std::string newCookie);
private:
	/*����Ϊ�ڲ�ʹ�ú���*/
	/*
	�Զ���������Э��ͷ
	*/
	void AutoAddHeader();
	/*
	ȡ����Э��ͷcookies
	*/
	std::string GetCookies(const std::string strHeader);
	/*
	�ϲ�cookie�ڲ�����
	*/
	RequestHeaderValue SplitCookie(const std::string strCookie);
	/*
	�ַ����ָ�
	*/
	std::vector<std::string> StrSplit(const std::string &str, const std::string &pattern);
	/*
	�鿴�Զ�������Э��ͷ�Ƿ����
	*/
	bool CheckHeaderExist(const std::string strHeaderKey);
	static size_t header_callback(char *ptr, size_t size, size_t nmemb, void* userdata);
	static size_t read_callback(char *ptr, size_t size, size_t nmemb, void* userdata);
private:
	/*
	CURL���
	*/
	CURL* m_CurlHandle;
	/*
	 ������������Ľ��
	*/
	responseData m_Request;
	/*
	�������շ��ص�Э��ͷ
	*/
	responseData m_ResponseHeader;
	/*
	Э��ͷ����ָ��
	*/
	struct curl_slist* headerList;
	/*
	�ڲ�����cookie �û��ϲ�����cookiesʹ��
	*/
	std::string m_Cookies;
};
#endif

