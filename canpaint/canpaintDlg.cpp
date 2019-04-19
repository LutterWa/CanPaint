
// canpaintDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "canpaint.h"
#include "canpaintDlg.h"
#include "ControlCAN.h"
#include <cstring>
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool sp_Receiving = true;

unsigned int xtime;//横轴单位
std::vector<LINE>curve;//曲线
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CcanpaintDlg 对话框




CcanpaintDlg::CcanpaintDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcanpaintDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	Flag_CanRun = 1;
}

void CcanpaintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJCHOICE_COMBO, m_ObjChoice);
	DDX_Control(pDX, IDC_PAINT_STATIC, m_paint_static);
	DDX_Control(pDX, IDC_AIMPOS_LIST, m_aimpos);
	DDX_Control(pDX, IDC_CURPOS_LIST, m_curpos);
	DDX_Control(pDX, IDC_XTIM_LIST, m_xtime);
}

BEGIN_MESSAGE_MAP(CcanpaintDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_OBJCHOICE_COMBO, &CcanpaintDlg::OnSelchangeObjchoice)
	ON_BN_CLICKED(IDC_START_BUTTON, &CcanpaintDlg::OnBnClickedStartButton)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_FRESH_BUTTON, &CcanpaintDlg::OnBnClickedFreshButton)
	ON_BN_CLICKED(IDC_STOP_BUTTON, &CcanpaintDlg::OnBnClickedStopButton)
	ON_BN_CLICKED(IDC_NEW_BUTTON, &CcanpaintDlg::OnBnClickedNewButton)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_SETBUT_BUTTON, &CcanpaintDlg::OnBnClickedSetbutButton)
	ON_BN_CLICKED(IDC_ADDTIM_BUTTON, &CcanpaintDlg::OnBnClickedAddtimButton)
	ON_BN_CLICKED(IDC_MINTIM_BUTTON, &CcanpaintDlg::OnBnClickedMintimButton)
	ON_BN_CLICKED(IDC_FUN1_BUTTON, &CcanpaintDlg::OnBnClickedFun1Button)
	ON_BN_CLICKED(IDC_FUN2_BUTTON, &CcanpaintDlg::OnBnClickedFun2Button)
	ON_BN_CLICKED(IDC_FUN3_BUTTON, &CcanpaintDlg::OnBnClickedFun3Button)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CcanpaintDlg 消息处理程序

BOOL CcanpaintDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//添加被测对象行
	m_ObjChoice.InsertString(0,_T("方位测试"));
	m_ObjChoice.InsertString(1,_T("高低测试"));
	m_ObjChoice.SetCurSel(0);

	m_paint_static.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_PAINT_STATIC)->GetWindowRect(rect_paint);
	ScreenToClient(rect_paint);
	m_ctrlpaint.Create(WS_VISIBLE | WS_CHILD, rect_paint, this, IDC_PAINT_STATIC);


	xtime = 20;
	CString timstr;
	timstr.Format(_T("%.2f"), (float)xtime / 20);
	m_xtime.AddString(timstr);
	//预设曲线，避免每次启动时添加
	using namespace usrdefcolor;
	curve.push_back(LINE(0x1c5, 0, 0, 1, 256, 0, 0, 0, 0, red, 5, 0, 2));//当前位置

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CcanpaintDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CcanpaintDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
		
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CcanpaintDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CcanpaintDlg::OnSelchangeObjchoice()//选择显示被测对象
{
	// TODO: 在此添加控件通知处理程序代码
	using namespace usrdefcolor;
	nIndex = m_ObjChoice.GetCurSel();//获取当前选中被测对象序列
	if(nIndex == 0)
	{		
		curve.clear();
		curve.push_back(LINE(0xcc, 0, 0, 0, 0, 1,256, 0, 0, red, 1, 30000, 1));//当前位置
		curve.push_back(LINE(0xcc, 0, 0, 0, 0, 0, 0, 1,256, blue, 1, 30000, 1));//目标位置
		
	}
	else if(nIndex == 1)
	{		
		curve.clear();
		curve.push_back(LINE(0xaa, 0, 0, 1,256, 0, 0, 0, 0, red, 0.1, 0, 2));//当前位置
		curve.push_back(LINE(0xaa, 0, 0, 0, 0, 0, 0, 1,256, blue, 0.1, 0, 2));//目标位置
	}	
}

void CcanpaintDlg::OnBnClickedStartButton()
{
	OpenCan dialog;
	INT_PTR nResponse = dialog.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
		AfxBeginThread(ReceiveThread, 0);
		SetTimer(0, 50, NULL);
		//GetDlgItem(IDC_START_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_SETBUT_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(TRUE);
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
}

UINT CcanpaintDlg::ReceiveThread(LPVOID v)
{
	CcanpaintDlg *dlg = (CcanpaintDlg*)AfxGetApp()->GetMainWnd();
	std::vector<unsigned char>CanData;
	VCI_CAN_OBJ frameinfo[50];
	VCI_ERR_INFO errinfo;
	int len = 1;
	int i = 0;
	int length;
	while (dlg->Flag_CanRun)
	{		
		length = VCI_GetReceiveNum(dlg->m_devtype, dlg->m_devind, dlg->m_cannum);
		if (length <= 0)
		{
			continue;
		}
		PVCI_CAN_OBJ frameinfo = new VCI_CAN_OBJ[length];
		len = VCI_Receive(dlg->m_devtype, dlg->m_devind, dlg->m_cannum, frameinfo, length, 200);
		if (len <= 0)
		{
			//注意：如果没有读到数据则必须调用此函数来读取出当前的错误码，
			//千万不能省略这一步（即使你可能不想知道错误码是什么）
			VCI_ReadErrInfo(dlg->m_devtype, dlg->m_devind, dlg->m_cannum, &errinfo);
		}
		else
		{
			for (i = 0;i < len; i++)
			{
				for (int LineNum = 0;LineNum < curve.size();LineNum++)
				{
					if (frameinfo[i].ID == curve[LineNum].id)
					{
						for (int j = 0; j < (frameinfo[i].DataLen); j++)	//数据信息
						{
							CanData.push_back(frameinfo[i].Data[j]);
						}
						if (!CanData.empty())
						{ 
							curve[LineNum].data.push_back(CanData);//在数据接收线程进行数据解析
							sp_Receiving = false;
							curve[LineNum].Process();
							sp_Receiving = true;
							CanData.clear();
						}
					}
				}
				//接收信息列表显示完毕
			}
		}
		delete[] frameinfo;
	}
	Sleep(10);
	return 0;
}
void CcanpaintDlg::OnBnClickedStopButton()
{
	DWORD bRef = VCI_CloseDevice(m_devtype, m_devind);
	if(bRef == STATUS_OK)
	{ 
		MessageBox(_T("关闭成功!"));
		KillTimer(0);
		GetDlgItem(IDC_START_BUTTON)->EnableWindow(TRUE);
		//GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_SETBUT_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_FUN1_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_FUN2_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_FUN3_BUTTON)->EnableWindow(FALSE);
	}
	else
	{
		MessageBox(_T("关闭失败!"));
	}
}
float BigestEk;
void CcanpaintDlg::OnBnClickedFreshButton()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < curve.size(); i++)
	{
		curve[i].data.clear();
		curve[i].point.clear();
	}
	BigestEk = 0;
}

void CcanpaintDlg::OnBnClickedNewButton()
{
	// TODO: 在此添加控件通知处理程序代码
	Newline dialog;
	INT_PTR nResponse = dialog.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用“确定”来关闭对话框的代码
		curve.push_back(transferline);
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭对话框的代码
	}
}
void CcanpaintDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!curve.empty())
	{
		CString ST;
		if (curve[1].point.empty() || curve[0].point.empty())
			return;
		float ek = curve[1].point.back() - \
			curve[0].point.back();
		BigestEk = abs(ek)>abs(BigestEk) ? ek : BigestEk;
		ST.Format(_T("当前偏差=%.2f"),ek);
		m_aimpos.DeleteString(9);
		m_aimpos.InsertString(0,ST);
		ST.Format(_T("最大偏差=%.2f"),BigestEk);
		m_curpos.ResetContent();
		m_curpos.AddString(ST);
		
	}
}



void CcanpaintDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


void CcanpaintDlg::OnBnClickedSetbutButton()
{
	// TODO: 在此添加控件通知处理程序代码
	SetFun dialog;
	INT_PTR nResponse = dialog.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用“确定”来关闭对话框的代码
		if (transfersend.button == 0)
		{
			GetDlgItem(IDC_FUN1_BUTTON)->EnableWindow(TRUE);
			send_fun[0] = transfersend;
		}
		else if (transfersend.button == 1)
		{
			GetDlgItem(IDC_FUN2_BUTTON)->EnableWindow(TRUE);
			send_fun[1] = transfersend;
		}
		else if (transfersend.button == 2)
		{
			GetDlgItem(IDC_FUN3_BUTTON)->EnableWindow(TRUE);
			send_fun[2] = transfersend;
		}
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭对话框的代码
	}
}


void CcanpaintDlg::OnBnClickedAddtimButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if (xtime < 100)
	{
		xtime += 2;
	}
	else
	{
		xtime = 100;
	}
	CString timstr;
	timstr.Format(_T("%.2f"), (float)xtime / 20);
	m_xtime.ResetContent();
	m_xtime.AddString(timstr);
}


void CcanpaintDlg::OnBnClickedMintimButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if (xtime > 2)
	{
		xtime -= 2;
	}
	else
	{
		xtime = 2;
	}
	CString timstr;
	timstr.Format(_T("%.2f"), (float)xtime / 20);
	m_xtime.ResetContent();
	m_xtime.AddString(timstr);
}


void CcanpaintDlg::OnBnClickedFun1Button()
{
	
	PVCI_CAN_OBJ framesend;

	framesend->ID = send_fun[1].id;
	framesend->SendType = 0;// 0时为正常发送,1时为单次发送,2时为自发自收,3时为单次自发自收
	framesend->RemoteFlag = 0;// 数据帧
	framesend->ExternFlag = 0;// 标准帧
	framesend->DataLen = send_fun[1].dat.size();

	if (send_fun[1].type == 0)//正弦
	{

	}
	else if (send_fun[1].type == 1)//方波
	{

	}
	else if(send_fun[1].type == 2)//自定义
	{
		for (int k = 0;k < send_fun[1].dat.size();k++)
		{
			framesend->Data[k] = send_fun[1].dat[k];
		}
	}

	VCI_Transmit(m_devtype, m_devind, m_cannum, framesend, framesend->DataLen);
}


void CcanpaintDlg::OnBnClickedFun2Button()
{
	PVCI_CAN_OBJ framesend;

	framesend->ID = send_fun[2].id;
	framesend->SendType = 0;// 0时为正常发送,1时为单次发送,2时为自发自收,3时为单次自发自收
	framesend->RemoteFlag = 0;// 数据帧
	framesend->ExternFlag = 0;// 标准帧
	framesend->DataLen = send_fun[2].dat.size();

	if (send_fun[2].type == 0)//正弦
	{

	}
	else if (send_fun[2].type == 1)//方波
	{

	}
	else if (send_fun[2].type == 2)//自定义
	{
		for (int k = 0;k < send_fun[2].dat.size();k++)
		{
			framesend->Data[k] = send_fun[2].dat[k];
		}
	}

	VCI_Transmit(m_devtype, m_devind, m_cannum, framesend, 1);
}


void CcanpaintDlg::OnBnClickedFun3Button()
{
	PVCI_CAN_OBJ framesend;

	framesend->ID = send_fun[0].id;
	framesend->SendType = 0;// 0时为正常发送,1时为单次发送,2时为自发自收,3时为单次自发自收
	framesend->RemoteFlag = 0;// 数据帧
	framesend->ExternFlag = 0;// 标准帧
	framesend->DataLen = send_fun[0].dat.size();

	if (send_fun[0].type == 0)//正弦
	{

	}
	else if (send_fun[0].type == 1)//方波
	{

	}
	else if (send_fun[0].type == 2)//自定义
	{
		for (int k = 0;k < send_fun[0].dat.size();k++)
		{
			framesend->Data[k] = send_fun[0].dat[k];
		}
	}

	VCI_Transmit(m_devtype, m_devind, m_cannum, framesend, framesend->DataLen);
}


void CcanpaintDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Flag_CanRun = 0;
	VCI_CloseDevice(m_devtype, m_devind);
	CDialogEx::OnClose();
}
