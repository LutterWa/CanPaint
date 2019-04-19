#if !defined(AFX_CPaint_H__A1446DDE_3456_47A9_BA8E_D4F048F3325A__INCLUDED_)
#define AFX_CPaint_H__A1446DDE_3456_47A9_BA8E_D4F048F3325A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPaint.h : header file
//

#include "AfxTempl.h"
/////////////////////////////////////////////////////////////////////////////
// CPaint window



class CPaint : public CWnd
{
// Construction
public:
	CPaint();
	typedef enum {LOW_SPEED = 3500, NORMAL_SPEED = 1000, HIGH_SPEED = 2000, IDLE = 0} SPEED;
	static UINT TimerThread(LPVOID v);
	BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT uID);
	void SetRange(UINT uLower, UINT uUpper);
	BOOL SetGridsColor(COLORREF cr);
	BOOL SetBkColor(COLORREF cr);
	void SetPen(int nWidth, COLORREF crColor);
	CPaint::SPEED SetSpeed(CPaint::SPEED uSpeed);
	void SetPos(UINT uPos);
	virtual ~CPaint();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CPaint)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	enum SPEED m_uSpeed;
	UINT m_yPos;
	UINT m_yPreviousPos;
	UINT m_uOffset;
	POINT m_PrevPos;
	POINT m_Pos;

	void DrawLine();
	
	BOOL InvalidateCtrl();
	UINT GetAverage();

	CDC *m_pMemDC;
	
	UINT m_uLower;
	UINT m_uUpper;

	CRect m_rcClient;
	CPen m_colorPen;

	int m_nFirstLinePos;

	COLORREF m_crBackGround;
	COLORREF m_crGrids;

	CList<UINT, UINT&> *m_pList;

	int widepic,highpic;
	CBitmap bmp;
	BITMAP BM;
};

#endif // !defined(AFX_CPaint_H__A1446DDE_3456_47A9_BA8E_D4F048F3325A__INCLUDED_)
