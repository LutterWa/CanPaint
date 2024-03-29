// OpenCan.cpp: 实现文件
//

#include "stdafx.h"
#include "canpaint.h"
#include "canpaintDlg.h"
#include "OpenCan.h"
#include "ControlCAN.h"
#include "afxdialogex.h"


// OpenCan 对话框

IMPLEMENT_DYNAMIC(OpenCan, CDialogEx)

OpenCan::OpenCan(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPENCAN, pParent)
{
}

OpenCan::~OpenCan()
{
}

void OpenCan::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DEVTYP_COMBO, m_type);
	DDX_Control(pDX, IDC_DEVIDX_COMBO, m_ind);
	DDX_Control(pDX, IDC_CANNUM_COMBO, m_num);
	DDX_Control(pDX, IDC_BAUD_COMBO, m_baud);
}


BEGIN_MESSAGE_MAP(OpenCan, CDialogEx)
	ON_BN_CLICKED(IDOK, &OpenCan::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_DEVTYP_COMBO, &OpenCan::OnCbnSelchangeDevtyp)
	ON_CBN_SELCHANGE(IDC_DEVIDX_COMBO, &OpenCan::OnCbnSelchangeDevidx)
	ON_CBN_SELCHANGE(IDC_CANNUM_COMBO, &OpenCan::OnCbnSelchangeCannum)
	ON_CBN_SELCHANGE(IDC_BAUD_COMBO, &OpenCan::OnCbnSelchangeBaud)
END_MESSAGE_MAP()


// OpenCan 消息处理程序


BOOL OpenCan::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_type.InsertString(0, _T("USBCAN1"));
	m_type.InsertString(1, _T("USBCAN2"));
	m_type.InsertString(2, _T("CAN232"));
	m_type.SetCurSel(1);
	type = VCI_USBCAN2;

	m_ind.InsertString(0, _T("0"));
	m_ind.InsertString(1, _T("1"));
	m_ind.InsertString(2, _T("2"));
	m_ind.InsertString(3, _T("3"));
	m_ind.InsertString(4, _T("4"));
	m_ind.InsertString(5, _T("5"));
	m_ind.InsertString(6, _T("6"));
	m_ind.InsertString(7, _T("7"));
	m_ind.InsertString(8, _T("8"));
	m_ind.SetCurSel(0);
	ind = 0;

	m_num.InsertString(0, _T("CAN 0"));
	m_num.InsertString(1, _T("CAN 1"));
	m_num.SetCurSel(0);
	num = 0;

	m_baud.InsertString(0, _T("1000k"));
	m_baud.InsertString(1, _T("800k"));
	m_baud.InsertString(2, _T("500k"));
	m_baud.InsertString(3, _T("250k"));
	m_baud.InsertString(4, _T("125k"));
	m_baud.SetCurSel(2);
	baud[0] = 0x00;
	baud[1] = 0x1C;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void OpenCan::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwRel = VCI_OpenDevice(type, ind, 0);
	//打开设备
	if (dwRel != STATUS_OK)
	{
		MessageBox(_T("打开设备失败!\n检查设备是否连接"), _T("警告"));
		return;
	}

	VCI_INIT_CONFIG InitInfo[1];
	//波特率
	InitInfo->Timing0 = baud[0];
	InitInfo->Timing1 = baud[1];
	InitInfo->Filter = 0;
	InitInfo->AccCode = 0x80000008;
	InitInfo->AccMask = 0xFFFFFFFF;
	InitInfo->Mode = 0;
	//初始化通道0
	dwRel = VCI_InitCAN(type, ind, num, InitInfo);
	if (dwRel != STATUS_OK)
	{
		MessageBox(_T("初始化CAN失败!\n检查设备是否被占用"), _T("警告"));
		VCI_CloseDevice(type, ind);
		return;
	}
	Sleep(100);
	//初始化通道0
	if (VCI_StartCAN(type, ind, 0) == 1)
	{
		MessageBox(_T("启动成功"));
	}
	else
	{
		MessageBox(_T("启动失败\n检查设备是否已启动"), _T("警告"));
		return;
	}
	CcanpaintDlg *pdlg = (CcanpaintDlg*)GetParent();
	pdlg->m_devtype = type;
	pdlg->m_devind = ind;
	pdlg->m_cannum = num;

	CDialogEx::OnOK();
}


void OpenCan::OnCbnSelchangeDevtyp()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_type.GetCurSel();
	switch (nIndex)
	{
		case 0:type = VCI_USBCAN1;break;
		case 1:type = VCI_USBCAN2;break;
		case 2:type = VCI_CAN232;break;
	}
}


void OpenCan::OnCbnSelchangeDevidx()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_ind.GetCurSel();
	ind = nIndex;
}


void OpenCan::OnCbnSelchangeCannum()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_num.GetCurSel();
	num = nIndex;
}


void OpenCan::OnCbnSelchangeBaud()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_baud.GetCurSel();
	switch (nIndex)
	{
		case 0:baud[0] = 0x00;baud[1] = 0x14;break;  //1000k
		case 1:baud[0] = 0x00;baud[1] = 0x16;break;  //800k
		case 2:baud[0] = 0x00;baud[1] = 0x1C;break;  //500k
		case 3:baud[0] = 0x01;baud[1] = 0x1C;break;  //250k
		case 4:baud[0] = 0x03;baud[1] = 0x1C;break;  //125k
	}
}
