#pragma once


// OpenCan 对话框

class OpenCan : public CDialogEx
{
	DECLARE_DYNAMIC(OpenCan)

public:
	OpenCan(CWnd* pParent = nullptr);   // 标准构造函数
	CComboBox m_type;
	CComboBox m_ind;
	CComboBox m_num;
	CComboBox m_baud;

	int num;
	int type;
	DWORD ind;
	char baud[2];
	virtual ~OpenCan();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENCAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeDevtyp();
	afx_msg void OnCbnSelchangeDevidx();
	afx_msg void OnCbnSelchangeCannum();
	afx_msg void OnCbnSelchangeBaud();
};
