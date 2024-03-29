// Newline.cpp: 实现文件
//

#include "stdafx.h"
#include "canpaint.h"
#include "canpaintDlg.h"
#include "Newline.h"
#include "afxdialogex.h"

// Newline 对话框

IMPLEMENT_DYNAMIC(Newline, CDialogEx)

Newline::Newline(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NEWLINE, pParent)
{
	m_canid = "01";
	m_7 = 0;
	m_6 = 0;
	m_5 = 0;
	m_4 = 0;
	m_3 = 0;
	m_2 = 0;
	m_1 = 0;
	m_0 = 0;
	m_ex = 1;
	m_mv = 0;
	m_tms = 20;
}

Newline::~Newline()
{
}

void Newline::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ID_EDIT, m_canid);
	DDV_MaxChars(pDX, m_canid, 3);

	DDX_Text(pDX, IDC_7_EDIT, m_7);
	DDX_Text(pDX, IDC_6_EDIT, m_6);
	DDX_Text(pDX, IDC_5_EDIT, m_5);
	DDX_Text(pDX, IDC_4_EDIT, m_4);
	DDX_Text(pDX, IDC_3_EDIT, m_3);
	DDX_Text(pDX, IDC_2_EDIT, m_2);
	DDX_Text(pDX, IDC_1_EDIT, m_1);
	DDX_Text(pDX, IDC_0_EDIT, m_0);
	DDX_Text(pDX, IDC_MV_EDIT, m_mv);
	DDX_Text(pDX, IDC_EX_EDIT, m_ex);
	DDX_Text(pDX, IDC_TMS_EDIT, m_tms);

	DDX_Control(pDX, IDC_COLOR_COMBO, m_color);
}


BEGIN_MESSAGE_MAP(Newline, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COLOR_COMBO, &Newline::OnCbnSelchangeColorCombo)
	ON_BN_CLICKED(IDOK, &Newline::OnBnClickedOk)
END_MESSAGE_MAP()


// Newline 消息处理程序

void Newline::OnCbnSelchangeColorCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	using namespace usrdefcolor;
	int nIndex = m_color.GetCurSel();
	switch (nIndex)
	{
		case 0: pen = red;   break;
		case 1: pen = green; break;
		case 2: pen = blue;  break;
		case 3: pen = yellow;break;
		case 4: pen = purple;break;
		case 5: pen = cyan;  break;
		case 6: pen = brown; break;
		case 7: pen = orange;break;
		case 8: pen = gold;  break;
		case 9: pen = pink;  break;
		case 10:pen = rose;  break;
	}
}


BOOL Newline::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_color.InsertString(0, _T("红色"));
	m_color.InsertString(1, _T("绿色"));
	m_color.InsertString(2, _T("蓝色"));
	m_color.InsertString(3, _T("黄色"));
	m_color.InsertString(4, _T("紫色"));
	m_color.InsertString(5, _T("青色"));
	m_color.InsertString(6, _T("褐色"));
	m_color.InsertString(7, _T("橙色"));
	m_color.InsertString(8, _T("金色"));
	m_color.InsertString(9, _T("粉红"));
	m_color.InsertString(10, _T("玫红"));
	m_color.SetCurSel(0);
	pen = usrdefcolor::red;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void Newline::OnBnClickedOk()
{
	UpdateData();
	CString str = _T("0x") + m_canid;//加0x表示16进制
	//数据采集
	StrToIntEx(str, STIF_SUPPORT_HEX, &line.id);//转换CString类为int型
	line.ex = m_ex;//放大系数
	line.mv = m_mv;//平移系数
	line.tms = m_tms;
	line.mu7 = m_7;
	line.mu6 = m_6;
	line.mu5 = m_5;
	line.mu4 = m_4;
	line.mu3 = m_3;
	line.mu2 = m_2;
	line.mu1 = m_1;
	line.mu0 = m_0;
	line.color = pen;
	//子窗口数据传输至父窗口
	CcanpaintDlg *pdlg = (CcanpaintDlg*)GetParent();
	pdlg->transferline = line;

	CDialogEx::OnOK();
}

//曲线相关类函数方法
LINE::LINE()
{
}

LINE::~LINE()
{
}
//用于初始化时预置曲线，无需新建曲线
LINE::LINE(int tid, int t7, int t6, int t5, int t4, int t3, int t2, int t1, int t0, COLORREF tcolor, float tex, int tmv,int ms)
{
	id = tid;
	mu7 = t7;
	mu6 = t6;
	mu5 = t5;
	mu4 = t4;
	mu3 = t3;
	mu2 = t2;
	mu1 = t1;
	mu0 = t0;

	ex = tex;
	mv = tmv;
	tms = ms;
	color = tcolor;
}
//这个函数存在BUG，如何消除？
float LINE::Process(void)
{
	std::vector<unsigned char>pdata = data.back();
	int zerocount = 0;//等于零的系数计数
	long long tmpsum = 0;
	switch (pdata.size())
	{
		case 0:return 0;
		case 1:mu1 = 0;
		case 2:mu2 = 0;
		case 3:mu3 = 0;
		case 4:mu4 = 0;
		case 5:mu5 = 0;
		case 6:mu6 = 0;
		case 7:mu7 = 0;
		default:break;
	}
	//计算整型的字节之和
	if (mu0 != 0)//byte0
	{
		tmpsum += (pdata[0] * mu0);
		zerocount++;
	}
	if (mu1 != 0)//byte1
	{
		tmpsum += (pdata[1] * mu1);
		zerocount++;
	}
	if (mu2 != 0)//byte2
	{
		tmpsum += (pdata[2] * mu2);
		zerocount++;
	}
	if (mu3 != 0)//byte3
	{
		tmpsum += (pdata[3] * mu3);
		zerocount++;
	}
	if (mu4 != 0)//byte4
	{
		tmpsum += (pdata[4] * mu4);
		zerocount++;
	}
	if (mu5 != 0)//byte5
	{
		tmpsum += (pdata[5] * mu5);
		zerocount++;
	}
	if (mu6 != 0)//byte6
	{
		tmpsum += (pdata[6] * mu6);
		zerocount++;
	}
	if (mu7 != 0)//byte7
	{
		tmpsum += (pdata[7] * mu7);
		zerocount++;
	}
	//根据等于零的系数个数确定整型长度
	if (zerocount == 0)
	{
		tmpsum = 0;
	}
	else if (zerocount <= 2)
	{
		tmpsum = (short)tmpsum;
	}
	else if (zerocount <= 4)
	{
		tmpsum = (long)tmpsum;
	}
	else
	{
		tmpsum = (long long)tmpsum;
	}

	point.push_back((float)(tmpsum - mv) * ex);
	return point.back();
}
