// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NoticeClient.h"
#include "LoginDlg.h"
#include "afxdialogex.h"

extern TCHAR g_ExePath[MAX_PATH];
// LoginDlg dialog

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

LoginDlg::LoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent)
{

}

LoginDlg::~LoginDlg()
{
}

HWINDOW LoginDlg::get_hwnd()
{
	return this->GetSafeHwnd();
}

HINSTANCE LoginDlg::get_resource_instance()
{
	return theApp.m_hInstance;
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// LoginDlg message handlers


LRESULT LoginDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	LRESULT lResult;
	BOOL    bHandled;

	lResult = SciterProcND(this->GetSafeHwnd(), message, wParam, lParam, &bHandled);
	if (bHandled)      // if it was handled by the Sciter
		return lResult; // then no further processing is required.

	return __super::WindowProc(message, wParam, lParam);
}




int LoginDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd,GWL_EXSTYLE)|WS_EX_APPWINDOW|WS_CHILD);
	this->setup_callback(); // attach sciter::host callbacks
	sciter::attach_dom_event_handler(this->get_hwnd(), this); // attach this as a DOM events 
	return 0;
}


BOOL LoginDlg::OnInitDialog()
{
	__super::OnInitDialog();
	ShowWindow(SW_NORMAL);
	TCHAR loginpage[MAX_PATH] = { 0 };
	wsprintf(loginpage, _T("%sLayout\\login.htm"), g_ExePath);
	TCHAR szURL[MAX_PATH] = { 0 };
	wsprintf(szURL, _T("file:///%s"), loginpage);
	if (-1 == (_taccess(loginpage, 0)))
	{
		return FALSE;
	}
	bool sg=load_file(loginpage);
	UpdateWindowSize();
	// TODO:  Add extra initialization here
	//bool suc=this->load_file(L"default.htm");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL LoginDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return __super::PreCreateWindow(cs);
}


void LoginDlg::UpdateWindowSize()
{
	sciter::dom::element root = get_root();
	sciter::dom::element DisplaySet = root.get_element_by_id(L"display");
	CRect rt;
	if (DisplaySet.is_valid())
	{
		rt.left = DisplaySet.get_attribute_int("left");

		rt.top = DisplaySet.get_attribute_int("top");

		rt.right = DisplaySet.get_attribute_int("right");

		rt.bottom = DisplaySet.get_attribute_int("bottom");
	}

	if (rt.right == -1 && rt.bottom == -1)
	{
		//
		rt.right = GetSystemMetrics(SM_CXSCREEN);
		rt.bottom = GetSystemMetrics(SM_CYSCREEN);
	}

	int nWidth = GetPrivateProfileInt(_T("System"), _T("Width"), 1024, g_ExePath);
	int nHeight = GetPrivateProfileInt(_T("System"), _T("Height"), 700, g_ExePath);
	int iWidth = rt.Width() == 0 ? nWidth : rt.Width();
	int iHeight = rt.Height() == 0 ? nHeight : rt.Height();
	int iScreenCX = GetSystemMetrics(SM_CXSCREEN);
	int iScreenCY = GetSystemMetrics(SM_CYSCREEN);
	int iLeft = (iScreenCX - iWidth) / 2;
	int iTop = (iScreenCY - iHeight) / 2;
	RECT rt1;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rt1, 0);    // 获得工作区大小
	if (iHeight == iScreenCY)
	{
		iHeight = rt1.bottom;
	}
	::SetWindowPos(this->GetSafeHwnd(), HWND_NOTOPMOST, iLeft, iTop, iWidth, iHeight, SWP_SHOWWINDOW | SWP_NOZORDER);
}
