
// canpaintDlg.h : 头文件
//

#pragma once

#include "Paint.h"
#include "Newline.h"
#include "OpenCan.h"
#include "SetFun.h"
#include <vector>
#include <iterator>


// CcanpaintDlg 对话框
class CcanpaintDlg : public CDialogEx
{
// 构造
public:
	CcanpaintDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CANPAINT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	CPaint  m_ctrlpaint;
	int		m_nSendFrameFormat;
	int		m_nSendFrameType;
	CString	m_strSendData;
	CString	m_strSendID;
	int		m_radioIDFormat;
	BOOL	m_bCanRxEn;
	int		m_nCanIndex;
	int		m_nDevType;

	CFont font;//定义字体
	int nIndex;//定义得到选取被测对象顺序
	CRect rect_paint;//建立static对象
	
	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ObjChoice;
	CStatic m_paint_static;
	int m_cannum;
	int m_devtype;
	DWORD m_devind;

	LINE transferline;
	SendFrame transfersend;
	SendFrame send_fun[3];

	CListBox m_aimpos;
	CListBox m_curpos;
	int Flag_CanRun;
	CListBox m_xtime;
	static UINT ReceiveThread(void *param);
	afx_msg void OnSelchangeObjchoice();
	afx_msg void OnBnClickedStartButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedFreshButton();
	afx_msg void OnBnClickedStopButton();
	afx_msg void OnBnClickedNewButton();
//	virtual HRESULT accDoDefaultAction(VARIANT varChild);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedSetbutButton();
	afx_msg void OnBnClickedAddtimButton();
	afx_msg void OnBnClickedMintimButton();
	afx_msg void OnBnClickedFun1Button();
	afx_msg void OnBnClickedFun2Button();
	afx_msg void OnBnClickedFun3Button();
	afx_msg void OnClose();
};

extern bool sp_Receiving;
