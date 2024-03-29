// SetFun.cpp: 实现文件
//

#include "stdafx.h"
#include "canpaint.h"
#include "canpaintDlg.h"
#include "SetFun.h"
#include "afxdialogex.h"


// SetFun 对话框

IMPLEMENT_DYNAMIC(SetFun, CDialogEx)

SetFun::SetFun(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETFUN, pParent)
{
	m_sendid = "01";
	m_sendtext = "00 01 02 03 04 05 06 07";
	m_senda = 0;
	m_sendw = 0;
	m_sendpwm = 0;
}

SetFun::~SetFun()
{
}

void SetFun::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SENDBUTTON_COMBO, m_sendbutton);
	DDX_Control(pDX, IDC_SENDIND_COMBO, m_sendmode);
	DDX_Control(pDX, IDC_SENDTYPE_COMBO, m_sendtype);

	DDX_Text(pDX, IDC_SENDID_EDIT, m_sendid);
	DDV_MaxChars(pDX, m_sendid, 3);
	DDX_Text(pDX, IDC_SENDTEXT_EDIT, m_sendtext);
	DDV_MaxChars(pDX, m_sendtext, 23);
	DDX_Text(pDX, IDC_SENDA_EDIT, m_senda);
	DDX_Text(pDX, IDC_SENDT_EDIT, m_sendw);
	DDX_Text(pDX, IDC_PWM_EDIT, m_sendpwm);
}


BEGIN_MESSAGE_MAP(SetFun, CDialogEx)
	ON_CBN_SELCHANGE(IDC_SENDBUTTON_COMBO, &SetFun::OnCbnSelchangeSendbuttonCombo)
	ON_CBN_SELCHANGE(IDC_SENDTYPE_COMBO, &SetFun::OnCbnSelchangeSendtypeCombo)
	ON_CBN_SELCHANGE(IDC_SENDIND_COMBO, &SetFun::OnCbnSelchangeSendindCombo)
	ON_BN_CLICKED(IDOK, &SetFun::OnBnClickedOk)
END_MESSAGE_MAP()


// SetFun 消息处理程序


BOOL SetFun::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_sendbutton.InsertString(0, _T("功能键1"));
	m_sendbutton.InsertString(1, _T("功能键2"));
	m_sendbutton.InsertString(2, _T("功能键3"));
	m_sendbutton.SetCurSel(0);
	send.button = 0;

	m_sendtype.InsertString(0, _T("正弦"));
	m_sendtype.InsertString(1, _T("方波"));
	m_sendtype.InsertString(2, _T("自定义"));
	//m_sendtype.SetCurSel(0);
	send.type = 2;

	m_sendmode.InsertString(0, _T("单次"));
	m_sendmode.InsertString(1, _T("连续"));
	m_sendmode.SetCurSel(0);
	send.mode = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void SetFun::OnCbnSelchangeSendbuttonCombo()//设置功能键i
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_sendbutton.GetCurSel();
	send.button = nIndex;
}


void SetFun::OnCbnSelchangeSendtypeCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_sendtype.GetCurSel();
	send.type = nIndex;
	if (nIndex == 0)
	{
		GetDlgItem(IDC_SENDA_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SENDT_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_PWM_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SENDTEXT_EDIT)->EnableWindow(FALSE);
	}
	else if (nIndex == 1)
	{
		GetDlgItem(IDC_SENDA_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SENDT_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_PWM_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SENDTEXT_EDIT)->EnableWindow(FALSE);
	}
	else if (nIndex == 2)
	{
		GetDlgItem(IDC_SENDA_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SENDT_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_PWM_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SENDTEXT_EDIT)->EnableWindow(TRUE);
	}
}


void SetFun::OnCbnSelchangeSendindCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_sendmode.GetCurSel();
	send.mode = nIndex;
}


void SetFun::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CString str = _T("0x") + m_sendid;//加0x表示16进制
	StrToIntEx(str, STIF_SUPPORT_HEX, &send.id);//转换CString类为int型
	if (send.type == 0)
	{
		send.a = m_senda;
		send.w = m_sendw;
	}
	else if (send.type == 1)
	{
		send.a = m_senda;
		send.w = m_sendw;
		send.pwm = m_sendpwm;
	}
	else if (send.type == 2)
	{
		int trans;
		for (int i = 0; i < m_sendtext.GetLength(); i += 3)
		{
			str = _T("0x") + m_sendtext.Mid(i,2);
			StrToIntEx(str, STIF_SUPPORT_HEX, &trans);
			send.dat.push_back(trans);
		}
	}
	CcanpaintDlg *pdlg = (CcanpaintDlg*)GetParent();
	pdlg->transfersend = send;

	CDialogEx::OnOK();
}
