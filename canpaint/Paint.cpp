#include "stdafx.h"
#include "Paint.h"
#include "Resource.h"
#include "canpaint.h"
#include "canpaintDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

unsigned int Clock;
int Paint_TimerControl = 0;//中间转化，控制SendMessage(WM_PAINT,0,0)
int p_x = 0,p_y = 0;//定义标志圆左上角坐标
CPaint::CPaint()
{
	m_nFirstLinePos = 13;

	m_pMemDC = NULL;
	m_pList = NULL;
	m_uSpeed = HIGH_SPEED;
	
	m_crBackGround = RGB(0, 0, 0);
	m_crGrids = RGB(0, 130, 66);

	SetPen(1, RGB(0,255,0));

	SetRange(1, 100);
}

CPaint::~CPaint()
{
	if(m_pMemDC)
		delete m_pMemDC;

	if(m_pList)
		delete m_pList;
}

BEGIN_MESSAGE_MAP(CPaint, CWnd)
	//{{AFX_MSG_MAP(CPaint)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPaint::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT uID)
{
	BOOL bRet = CWnd::CreateEx(WS_EX_CLIENTEDGE, 
								AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW ),
								NULL,
								dwStyle,
								rect.left,
								rect.top,
								rect.right - rect.left,
								rect.bottom - rect.top,
								pParentWnd->GetSafeHwnd(),
								(HMENU)uID);

	if(!bRet)
		return FALSE;

	m_pMemDC = new CDC;
	if(!m_pMemDC)
		return FALSE;

	m_pList = new CList<UINT, UINT&>;
	if(!m_pList)
		return FALSE;

	GetClientRect(m_rcClient);//获取控件大小范围

	if(!InvalidateCtrl())
		return FALSE;

	SetSpeed(CPaint::NORMAL_SPEED);
	//AfxBeginThread(TimerThread, 0);

	m_PrevPos.x = rect.left -10;
	m_PrevPos.y = rect.top + (rect.bottom - rect.top)/2;
	return TRUE;
}

void CPaint::SetRange(UINT uLower, UINT uUpper)
{
	ASSERT(uLower && uUpper && uLower < uUpper);
	//Postcondition:
	//	Sets the upper and lower (limits) range
	m_uUpper = uUpper - uLower + 1;
	m_uLower = 1;
	m_uOffset = uLower - 1;
}

BOOL CPaint::InvalidateCtrl()
{
	//Postcondition:
	//	Paints the entire client area of the control
	//	Returns TRUE if it's done successfuly or FALSE if it fails

	CClientDC dc(this);
//	HBITMAP hBitmap;

	if(m_pMemDC->GetSafeHdc())
		return FALSE;

	if(!m_pMemDC->CreateCompatibleDC(&dc))
		return FALSE;	

	bmp.LoadBitmap(IDB_BITMAP1);
	bmp.GetObject(sizeof(BM),&BM);
	widepic = BM.bmWidth;//获取图形宽度
	highpic = BM.bmHeight;//获取图形高度
	m_pMemDC->SelectObject(bmp);
	bmp.DeleteObject();
	//Set the background color of the control
	CBrush bkBrush;
	if(!bkBrush.CreateSolidBrush(m_crBackGround))
		return FALSE;
	//Select a specified pen to the device context to draw background lines
	CPen bkLinesPen;
	if(!bkLinesPen.CreatePen(PS_SOLID, 1, m_crGrids))
		return FALSE;
	
	if(!m_pMemDC->SelectObject(bkLinesPen))
		return FALSE;
	m_yPreviousPos = m_yPos = m_rcClient.bottom + 1;

	InvalidateRect(m_rcClient);

	return TRUE;
}

void CPaint::OnPaint() 
{
	CPaintDC dc(this);
	if(m_pMemDC->GetSafeHdc())
	{
		//m_rcClient为矩形区域大小，widepic为图形大小
		dc.StretchBlt(0, 0, m_rcClient.Width(), m_rcClient.Height(), m_pMemDC, 0, 0, widepic, highpic, SRCCOPY);
	}

	//绘制直角坐标系
	CString str;
	int xz_i,xz_j;
	int xz_x,xz_y;

	UpdateWindow();
	m_pMemDC->Rectangle(0,0,widepic, highpic);
	CPen pPenXY,pPenGray;
	pPenXY.CreatePen(PS_SOLID, 1, RGB(51,51,51));
	pPenGray.CreatePen(PS_DASH, 1, RGB(225, 225, 225));
	m_pMemDC->SelectObject(pPenGray);
	for(xz_x=50;xz_x<=850;xz_x+=50)
	{
		m_pMemDC->MoveTo(xz_x, 0);//画灰色x轴虚线刻度
		m_pMemDC->LineTo(xz_x, 660);
	}
	for (xz_y = 20;xz_y <= 620;xz_y += 50)
	{
		m_pMemDC->MoveTo(0,xz_y);//画灰色y轴虚线刻度
		m_pMemDC->LineTo(850,xz_y);
	}

	m_pMemDC->SelectObject(pPenXY);
	m_pMemDC->MoveTo(30,20);
	m_pMemDC->LineTo(30,320);
	m_pMemDC->MoveTo(0, 320);
	m_pMemDC->LineTo(860,320);
	m_pMemDC->MoveTo(30, 320);
	m_pMemDC->LineTo(30, 620);
	str.Format(_T("850x650"));
	m_pMemDC->TextOut(780, 625, str);//数字
	//写x轴刻度
    for(xz_i=0;xz_i<850;xz_i+=10)
	{
		str.Format(_T("%d"),xz_i/50);
		if(xz_i%50==0)//只显示50的倍数的数字
		{
			m_pMemDC->TextOut(xz_i - 10,325,str);//数字，其中乘以20为每个刻度的宽度
			m_pMemDC->MoveTo(xz_i,320);//画刻度
			m_pMemDC->LineTo(xz_i,305);//有数字的刻度线画更长
		}
		else
		{
			m_pMemDC->MoveTo(xz_i,320);//利用纵向y轴差画一般短刻度线
			m_pMemDC->LineTo(xz_i,315);
		}
	}
//写y轴刻度
	for (xz_j = 0;xz_j <= 600; xz_j += 10)
	{
		str.Format(_T("%d"),xz_j/100 - 3);
		if(xz_j % 100==0 && xz_j / 100 != 3)//只显示50的倍数的数字
		{
			m_pMemDC->TextOut(5,596-xz_j+16,str);//数字
			m_pMemDC->MoveTo(30,600-xz_j+20);//画刻度
			m_pMemDC->LineTo(45,600-xz_j+20);//有数字的刻度线画更长
		}
		else if(xz_j!=0)
		{
			m_pMemDC->MoveTo(30,600-xz_j+20);//画刻度
			m_pMemDC->LineTo(35,600-xz_j+20);
		}

	}
}

extern unsigned int xtime;//横轴坐标
extern std::vector<LINE>curve;//全局曲线变量
void CPaint::DrawLine()
{	
	static float x = 0;
	for (int i = 0; i < curve.size(); i++)
	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, curve[i].color);
		m_pMemDC->SelectObject(pen);
		for (int j = 1; j < curve[i].data.size(); j++)
		{
			while (sp_Receiving == false);//等待CAN接收线程释放数据
			float bgn = curve[i].point[j-1];
			float end = curve[i].point[j];
			//如何应用Clock将横轴时间引入系统？
			m_pMemDC->MoveTo(j*((float)curve[i].tms / xtime), -bgn + 320);
			m_pMemDC->LineTo((j + 1)*((float)curve[i].tms / xtime), -end + 320);
			if (curve[i].data.size()*((float)curve[i].tms / xtime) >= 850)
			{
				curve[i].data.clear();
				curve[i].point.clear();
			}
		}
	}
	Invalidate();//重画矩形
}

void CPaint::SetPos(UINT uPos)
{
	uPos -= m_uOffset;
	ASSERT(uPos <= m_uUpper && uPos >= m_uLower);

	//Postcondition:
	//	Adds the specified point to a list, so that we
	//	would be able to draw the histogram within the
	//	specified intervals.
	m_pList->AddHead(uPos);
	DrawLine();
	
}

void CPaint::OnTimer(UINT nIDEvent) 
{
	DrawLine();	
}

CPaint::SPEED CPaint::SetSpeed(enum SPEED uSpeed)
{
	enum SPEED oldSpeed = m_uSpeed;
	m_uSpeed = uSpeed;

	KillTimer(0);

	if(uSpeed != CPaint::IDLE)
		SetTimer(0, 50, NULL);

	return oldSpeed;
}

void CPaint::SetPen(int nWidth, COLORREF crColor)
{
	m_colorPen.DeleteObject();
	m_colorPen.CreatePen(PS_SOLID, nWidth, crColor);
}

BOOL CPaint::SetBkColor(COLORREF cr)
{
	BOOL bRet = FALSE;
	enum SPEED oldSpeed = SetSpeed(CPaint::IDLE);

	m_crBackGround = cr;

	CClientDC dc(this);

	int oldPos = m_nFirstLinePos;
	m_nFirstLinePos = 13;
	if(m_pMemDC->DeleteDC())
		if(InvalidateCtrl())
			bRet = TRUE;

	if(!bRet)
		m_nFirstLinePos = oldPos;

	SetSpeed(oldSpeed);

	return bRet;
}

BOOL CPaint::SetGridsColor(COLORREF cr)
{
	BOOL bRet = FALSE;
	enum SPEED oldSpeed = SetSpeed(CPaint::IDLE);

	m_crGrids = cr;

	CClientDC dc(this);

	int oldPos = m_nFirstLinePos;
	m_nFirstLinePos = 13;
	if(m_pMemDC->DeleteDC())
		if(InvalidateCtrl())
			bRet = TRUE;

	if(!bRet)
		m_nFirstLinePos = oldPos;

	SetSpeed(oldSpeed);

	return bRet;
}

UINT CPaint::GetAverage()
{
	UINT uCounter = 0, uSum = 0;
	while(!m_pList->IsEmpty())
	{
		uSum += m_pList->RemoveTail();
		uCounter++;
	}

	return uCounter ? uSum / uCounter : 0;
}

void CALLBACK TimerAPCProc(
	LPVOID lpArg,
	DWORD dwTimerLowValue,
	DWORD dwTimerHighValue)
{	
	Clock / xtime <= 850 ? Clock += 2 : Clock = 0;//每毫秒自加1
}
UINT CPaint::TimerThread(LPVOID v)
{
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = 1000000;
	HANDLE hTimer = CreateWaitableTimer(NULL, FALSE, _T("WaitableTimer"));
	if (hTimer)
	{
		__try
		{
			BOOL bSuccess = SetWaitableTimer(hTimer,
				&liDueTime,
				2,//定时单位：ms
				TimerAPCProc,
				NULL,
				FALSE);
			if (bSuccess)
			{
				while (1)
				{
					SleepEx(INFINITE,
						TRUE);
				}
			}
		}
		__finally
		{
			CloseHandle(hTimer);
		}
	}
	return 1;
}