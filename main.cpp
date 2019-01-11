#include "WkeWindow.h"
Application app;
HINSTANCE g_hInstance;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	g_hInstance = hInstance;
	CWkeWindow* wkeWindow = CWkeWindow::GetInstance();
	wkeWindow->runApp(&app);
	wkeFinalize();
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