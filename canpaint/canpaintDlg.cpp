
// canpaintDlg.cpp : ʵ���ļ�
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

unsigned int xtime;//���ᵥλ
std::vector<LINE>curve;//����
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CcanpaintDlg �Ի���




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


// CcanpaintDlg ��Ϣ�������

BOOL CcanpaintDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//��ӱ��������
	m_ObjChoice.InsertString(0,_T("��λ����"));
	m_ObjChoice.InsertString(1,_T("�ߵͲ���"));
	m_ObjChoice.SetCurSel(0);

	m_paint_static.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_PAINT_STATIC)->GetWindowRect(rect_paint);
	ScreenToClient(rect_paint);
	m_ctrlpaint.Create(WS_VISIBLE | WS_CHILD, rect_paint, this, IDC_PAINT_STATIC);


	xtime = 20;
	CString timstr;
	timstr.Format(_T("%.2f"), (float)xtime / 20);
	m_xtime.AddString(timstr);
	//Ԥ�����ߣ�����ÿ������ʱ���
	using namespace usrdefcolor;
	curve.push_back(LINE(0x1c5, 0, 0, 1, 256, 0, 0, 0, 0, red, 5, 0, 2));//��ǰλ��

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CcanpaintDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
		
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CcanpaintDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CcanpaintDlg::OnSelchangeObjchoice()//ѡ����ʾ�������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	using namespace usrdefcolor;
	nIndex = m_ObjChoice.GetCurSel();//��ȡ��ǰѡ�б����������
	if(nIndex == 0)
	{		
		curve.clear();
		curve.push_back(LINE(0xcc, 0, 0, 0, 0, 1,256, 0, 0, red, 1, 30000, 1));//��ǰλ��
		curve.push_back(LINE(0xcc, 0, 0, 0, 0, 0, 0, 1,256, blue, 1, 30000, 1));//Ŀ��λ��
		
	}
	else if(nIndex == 1)
	{		
		curve.clear();
		curve.push_back(LINE(0xaa, 0, 0, 1,256, 0, 0, 0, 0, red, 0.1, 0, 2));//��ǰλ��
		curve.push_back(LINE(0xaa, 0, 0, 0, 0, 0, 0, 1,256, blue, 0.1, 0, 2));//Ŀ��λ��
	}	
}

void CcanpaintDlg::OnBnClickedStartButton()
{
	OpenCan dialog;
	INT_PTR nResponse = dialog.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
		AfxBeginThread(ReceiveThread, 0);
		SetTimer(0, 50, NULL);
		//GetDlgItem(IDC_START_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_SETBUT_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(TRUE);
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
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
			//ע�⣺���û�ж��������������ô˺�������ȡ����ǰ�Ĵ����룬
			//ǧ����ʡ����һ������ʹ����ܲ���֪����������ʲô��
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
						for (int j = 0; j < (frameinfo[i].DataLen); j++)	//������Ϣ
						{
							CanData.push_back(frameinfo[i].Data[j]);
						}
						if (!CanData.empty())
						{ 
							curve[LineNum].data.push_back(CanData);//�����ݽ����߳̽������ݽ���
							sp_Receiving = false;
							curve[LineNum].Process();
							sp_Receiving = true;
							CanData.clear();
						}
					}
				}
				//������Ϣ�б���ʾ���
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
		MessageBox(_T("�رճɹ�!"));
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
		MessageBox(_T("�ر�ʧ��!"));
	}
}
float BigestEk;
void CcanpaintDlg::OnBnClickedFreshButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i = 0; i < curve.size(); i++)
	{
		curve[i].data.clear();
		curve[i].point.clear();
	}
	BigestEk = 0;
}

void CcanpaintDlg::OnBnClickedNewButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Newline dialog;
	INT_PTR nResponse = dialog.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������رնԻ���Ĵ���
		curve.push_back(transferline);
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������رնԻ���Ĵ���
	}
}
void CcanpaintDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(!curve.empty())
	{
		CString ST;
		if (curve[1].point.empty() || curve[0].point.empty())
			return;
		float ek = curve[1].point.back() - \
			curve[0].point.back();
		BigestEk = abs(ek)>abs(BigestEk) ? ek : BigestEk;
		ST.Format(_T("��ǰƫ��=%.2f"),ek);
		m_aimpos.DeleteString(9);
		m_aimpos.InsertString(0,ST);
		ST.Format(_T("���ƫ��=%.2f"),BigestEk);
		m_curpos.ResetContent();
		m_curpos.AddString(ST);
		
	}
}



void CcanpaintDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}


void CcanpaintDlg::OnBnClickedSetbutButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetFun dialog;
	INT_PTR nResponse = dialog.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������رնԻ���Ĵ���
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
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������رնԻ���Ĵ���
	}
}


void CcanpaintDlg::OnBnClickedAddtimButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	framesend->SendType = 0;// 0ʱΪ��������,1ʱΪ���η���,2ʱΪ�Է�����,3ʱΪ�����Է�����
	framesend->RemoteFlag = 0;// ����֡
	framesend->ExternFlag = 0;// ��׼֡
	framesend->DataLen = send_fun[1].dat.size();

	if (send_fun[1].type == 0)//����
	{

	}
	else if (send_fun[1].type == 1)//����
	{

	}
	else if(send_fun[1].type == 2)//�Զ���
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
	framesend->SendType = 0;// 0ʱΪ��������,1ʱΪ���η���,2ʱΪ�Է�����,3ʱΪ�����Է�����
	framesend->RemoteFlag = 0;// ����֡
	framesend->ExternFlag = 0;// ��׼֡
	framesend->DataLen = send_fun[2].dat.size();

	if (send_fun[2].type == 0)//����
	{

	}
	else if (send_fun[2].type == 1)//����
	{

	}
	else if (send_fun[2].type == 2)//�Զ���
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
	framesend->SendType = 0;// 0ʱΪ��������,1ʱΪ���η���,2ʱΪ�Է�����,3ʱΪ�����Է�����
	framesend->RemoteFlag = 0;// ����֡
	framesend->ExternFlag = 0;// ��׼֡
	framesend->DataLen = send_fun[0].dat.size();

	if (send_fun[0].type == 0)//����
	{

	}
	else if (send_fun[0].type == 1)//����
	{

	}
	else if (send_fun[0].type == 2)//�Զ���
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	Flag_CanRun = 0;
	VCI_CloseDevice(m_devtype, m_devind);
	CDialogEx::OnClose();
}
