#include "WkeWindow.h"
#include <dbghelp.h>
#pragma comment(lib,"Dbghelp.lib")
Application app;
HINSTANCE g_hInstance;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	int argCount;
	LPWSTR* szArglist = nullptr;
	szArglist = CommandLineToArgvW(lpCmdLine, &argCount);
	if (!szArglist)
	{
		::MessageBox(NULL, _T("��ʼ��ʧ�ܳ����޷�����ִ��"), _T("error"), MB_ICONERROR);
		return 0;
	}
	CBaiduParse Temp;
	std::string strApplication = Temp.Unicode_To_Ansi(szArglist[0]);
	google::InitGoogleLogging(strApplication.c_str());
	char szPath[MAX_PATH];
	ZeroMemory(szPath, MAX_PATH);
	CopyMemory(szPath, strApplication.c_str(), strApplication.length());
	if (PathRemoveFileSpecA(szPath))
	{
		strcat_s(szPath, "\\log\\");
		std::string szRoot(szPath);
		if (MakeSureDirectoryPathExists(szPath))
		{
			FLAGS_log_dir = szPath;
			FLAGS_max_log_size = 10;
			FLAGS_stop_logging_if_full_disk = true;
			FLAGS_logtostderr = false;
			google::SetLogDestination(google::GLOG_INFO, std::string(szRoot + "info_").c_str());
			google::SetLogDestination(google::GLOG_WARNING, std::string(szRoot + "warning_").c_str());
			google::SetLogDestination(google::GLOG_ERROR, std::string(szRoot + "error_").c_str());
			google::SetLogDestination(google::GLOG_FATAL, std::string(szRoot + "info_").c_str());
			google::SetLogFilenameExtension("log");
		}
	}
	g_hInstance = hInstance;
	CWkeWindow* wkeWindow = CWkeWindow::GetInstance();
	wkeWindow->runApp(&app);
	wkeFinalize();
	google::ShutdownGoogleLogging();
	LocalFree(szArglist);
	return 0;
}
#if 0
//�����2013����ͻ������������
/*���ǹ���VS2012�������⣬���ô�����Ȼ������VS2013�д���
c++11 thread join()���������̷߳�����������
��ϸ�μ� https://stackoverflow.com/questions/10915233/stdthreadjoin-hangs-if-called-after-main-exits-when-using-vs2012-rc
*/
#pragma warning(disable:4073) 
#pragma init_seg(lib)
#if _MSC_VER < 1900
struct VS2013_threading_fix
{
	VS2013_threading_fix()
	{
		_Cnd_do_broadcast_at_thread_exit();
	}
} threading_fix;
#endif
#endif