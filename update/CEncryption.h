#ifndef __CENCRYTPION__H
#define __CENCRYTPION__H
#include <windows.h>
#include <string>
namespace aip {
	std::string base64_encode(const char * bytes_to_encode, unsigned int in_len);
	std::string base64_decode(std::string const & encoded_string);
}

typedef enum {
	GENERAL = 0,
	ECB,
	CBC,
	CFB,
	OFB,
	TRIPLE_ECB,
	TRIPLE_CBC
}CRYPTO_MODE;
class CCEncryption
{
public:
	CCEncryption();
	~CCEncryption();
	/////////////////////////////////////////////////////////
	/*						MD5���ܺ���						*/
	/////////////////////////////////////////////////////////
	/*
	@�����ַ�����MD5ֵ
	*/
	static std::string MD5_Str(const std::string strData);
	/*
	@�����ڴ����ݵ�MD5ֵ
	*/
	static std::string Md5_Memory(PVOID pBuffer, DWORD dwlens);
	/*
	@ �����ļ�·����ȡ�ļ������ļ���MD5ֵ
	*/
	static std::string Md5_Files(const std::string strFiles);
	/////////////////////////////////////////////////////////
	/*						DES�ӽ��ܺ���					*/
	/////////////////////////////////////////////////////////
	/*
	@ DES����
	@ cleartext ���ӽ����ı�
	@ key �ܳ״�
	@ mode �ӽ��ܷ�ʽ
	*/
	static std::string DES_Encrypt(const std::string cleartext, const std::string key, CRYPTO_MODE mode);
	/*
	@ DES����
	@ ciphertext ���ӽ����ı�
	@ key �ܳ״�
	@ mode �ӽ��ܷ�ʽ
	*/
	static std::string DES_Decrypt(const std::string ciphertext, const std::string key, CRYPTO_MODE mode);
	/////////////////////////////////////////////////////////
	/*						AES�ӽ��ܺ���					*/
	/////////////////////////////////////////////////////////
	/*
	@ AES���ܺ���
	@ strKey �����õ��ܳ�
	@ strData �����ܵ�����
	@ mode  ���ܵķ�ʽ
	*/
	static std::string AES_Encrypt(const std::string strKey, const std::string strData);
	/*
	@ AES���ܺ���
	@ strKey �����õ��ܳ�
	@ strData �����ܵ�����
	@ mode  ���ܵķ�ʽ 
	*/
	static std::string AES_Decrypt(const std::string strKey, const std::string strData);
	static std::string Gbk_To_Utf8(const char* szBuff);
private:
	#define MD5_LENS 16
	static BYTE cbc_iv[8];
	static std::string HexToStr(PBYTE pBuffer, DWORD dwLens);
	static std::string StrToHex(const std::string strBuffer);
};

#endif