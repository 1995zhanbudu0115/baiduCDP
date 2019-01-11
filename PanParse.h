#ifndef __PANPARSE__
#define __PANPARSE__
#ifndef min
#define min
#endif
#ifndef max
#define max
#endif
#include <atlimage.h>
#undef min
#undef max
#include "Http_Request.h"
#define HOME_URL "https://pan.baidu.com/disk/home?#/all?path=%2F&vmode=list"
#define FILE_LIST_URL "https://pan.baidu.com/api/list?order=name&desc=1&showempty=0&web=1&page=1&num=100&dir=%1%&t=0.1312003882264028&channel=chunlei&web=1&app_id=250528&bdstoken=%2%&logid=%3%&clienttype=0&startLogTime=%4%"
#define DOWN_LOCAL_FILE "http://d.pcs.baidu.com/rest/2.0/pcs/file?app_id=250528&channel=00000000000000000000000000000000&check_blue=1&clienttype=8&devuid=0&dtype=1&ehps=0&err_ver=1.0&es=1&esl=1&method=locatedownload&path=%1%&ver=4.0&version=6.0.0.12&vip=3"
#define SHARE_FILE_URL_1 "https://pan.baidu.com/share/set?channel=chunlei&clienttype=0&web=1&channel=chunlei&web=1&app_id=250528&bdstoken=%1%&logid=%2%&clienttype=0"
#define SHARE_FILE_URL_2 "https://pan.baidu.com/share/pset?channel=chunlei&clienttype=0&web=1&channel=chunlei&web=1&app_id=250528&bdstoken=%1%&logid=%2%&clienttype=0"
#define USER_AGENT "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/69.0.3497.100 Safari/537.36"
extern HINSTANCE g_hInstance;
typedef struct baiduRequestInfo
{
	std::string sign;
	std::string bdstoken;
	std::string timestamp;
	std::string app_id;
	std::string fs_id;
	std::string uk;
	std::string shareid;
	std::string BDCLND;
	std::string server_filename;
}BAIDUREQUESTINFO, *PBAIDUREQUESTINFO;
/*
����ȡ�����ݷ���
*/
typedef std::vector<std::string> REGEXVALUE;
/*
��֤����Ϣ
*/
typedef struct verCodeinfo
{
	std::string image;
	std::string verCode;
}VERCODEINFO;
/*
���ؽ�����������ʵ��ַ�Լ��ļ�������
*/
typedef struct RequestInfo
{
	std::string strDownloadUrl;
	std::string strFileName;
	std::string strCookies;
	std::string strSavePath;
}REQUESTINFO;
/*
��ȡ�û���½��Ļ�����Ϣ
*/
typedef struct userInfo
{
	std::string strUserName;
	std::string strHeadImageUrl;
	std::string bdstoken;
	int is_vip;
}BaiduUserInfo;
/*
�ٶ����û��ļ��б�����
*/
typedef struct fileInfo
{
	int nCategory;	//�ļ����� 6���ļ��л���zip�������������͵��ļ���Ҫ����isdir�ж��Ƿ����ļ��� 3��ͼƬ 2������ 1����Ƶ 5��exe�ļ�
	std::string strPath; //�ļ����ڵ�·��
	std::string server_filename; //�ļ�������
	int nisdir;		//�Ƿ����ļ��� 1�� 0 ����
	ULONG size;		//�ļ��Ĵ�С
	std::string strFileType;	//���ݺ�׺���������ļ�������
	ULONG server_ctime;		//�ļ���ʱ���
	std::string fs_id;		//��������ʹ��
}BaiduFileInfo;
/*
�ļ���Ϣ�б�����
*/
typedef std::vector<BaiduFileInfo> FileTypeArray;
/*
�����ļ���Ϣ
*/
typedef struct shareInfo
{
	std::string strPwd;
	std::string strpath_list;
	int nschannel;
	std::string strperiod;
}SHAREFILEINFO;
class CBaiduParse
{
public:
	explicit CBaiduParse();
	~CBaiduParse();
private:
	CBaiduParse(const CBaiduParse&);
	//��֤��������ַ
	static std::string m_vcCodeUrl;
	//��֤��
	static std::string m_VerCode;
	//��֤�봰�ڻص�
	static INT_PTR CALLBACK ImageProc(
		_In_ HWND   hwndDlg,
		_In_ UINT   uMsg,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
		);
	//����ͼƬ
	static BOOL LodcomImage(LPVOID PmemIm, ULONG dwLen, CImage& ImgObj);
public:
//��ʾ������֤��Ĵ���
	void ShowInputVerCodeDlg();
/*
��ȡ�ٶ������ļ��б���Ϣ
*/
std::string GetBaiduFileListInfo(const std::string& path, const std::string strCookie);
/*
д�ļ�
*/
DWORD WriteFileBuffer(std::string szFileNmae, PVOID pFileBuffer, DWORD dwFileSize);
/*
��ȡ�û���¼��Ļ�����Ϣ
*/
bool GetloginBassInfo(BaiduUserInfo& baseInfo,const std::string strCookie);
/*
����ȡ��ָ������
*/
REGEXVALUE GetRegexValue(const std::string strvalue, const std::string strRegx);
/*
	�����ٶ�����ʵ���ص�ַ
	strUrl �������ַ
	strCookies ��¼���cookies
	���ؽ�����������ʵ��ַ�Լ��ļ�������
*/
REQUESTINFO ParseBaiduAddr(const std::string strUrl, std::string& strCookies);
/*
����֤�������
*/
std::string GetBaiduAddr(BAIDUREQUESTINFO baiduinfo, const std::string strCookies);
/*
��ȡ��֤����Ϣ
*/
VERCODEINFO GetVerCodeinfo(BAIDUREQUESTINFO baiduinfo, const std::string strCookies);
/*
��ȡ BAIDUREQUESTINFO ��Ϣ
*/
BAIDUREQUESTINFO GetBaiduInfo(const std::string strJson);
/*
�����ļ�·����ȡ��Ҫ�����ص���ʵ���ص�ַ
*/
std::string GetBaiduLocalFileAddr(const std::string path, const std::string strCookie);
/*
����ٶ��û����ļ������ļ���
*/
std::string ShareBaiduFile(SHAREFILEINFO shareFileinfo, const std::string strCookie);
/*ɾ���ٶ����ļ�*/
std::string DeleteBaiduFile(const std::string strJsonData, const std::string strCookie);
/*
 ��ȡʱ���
*/
long long getTimeStamp();
/*
ȡ�ı��м�����
*/
std::string GetTextMid(const std::string strSource, const std::string strLeft, const std::string strRight);
/*
��ȡ������Ҫ��Logid
*/
std::string GetLogid(bool isFlag = true);
inline float roundEx(float Floatnum);
/*
��ȡ�ļ���С����
*/
std::string GetFileSizeType(double dSize);
/*
ʱ���ת����
*/
std::string timestampToDate(ULONGLONG ctime);
/*
����ת��
*/
std::string Unicode_To_Ansi(const wchar_t* szbuff);
std::wstring Ansi_To_Unicode(const char* szbuff);
std::string Gbk_To_Utf8(const char* szBuff);
std::string Utf8_To_Gbk(const char* szBuff);
// ���� szSource ��Ҫ��URL������ַ���
//���� isletter �Ƿ񲻱���������ĸ Ĭ��Ϊtrue ������ false �����
//���� isUtf8 �Ƿ����ΪUTF-8��ʽ Ĭ��Ϊtrue UTF-8���� false �򲻱���Ϊutf-8
/***************************************************/
std::string URL_Coding(const char* szSource, bool isletter = true, bool isUtf8 = true);
//����\uxxxx\uxxxx�����ַ�
std::string UnEscape(const char* strSource);
};
#endif
