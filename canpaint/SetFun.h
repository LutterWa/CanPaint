#pragma once

#include<vector>

// SetFun 对话框
struct SendFrame
{
	int id;//发送ID
	std::vector<char>dat;//发送内容
	int a;//幅值
	int w;//周期
	int pwm;//占空比
	int button;//功能键
	int type;//数据类型
	int mode;//发送方式
};

class SetFun : public CDialogEx
{
	DECLARE_DYNAMIC(SetFun)

public:
	SetFun(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SetFun();

	CComboBox m_sendbutton;//功能键
	CComboBox m_sendtype;//数据类型
	CComboBox m_sendmode;//发送方式
	CString m_sendid;//发送帧id
	CString m_sendtext;//自定义发送内容
	int m_senda;//幅值
	int m_sendw;//周期
	int m_sendpwm;

	SendFrame send;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETFUN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeSendbuttonCombo();
	afx_msg void OnCbnSelchangeSendtypeCombo();
	afx_msg void OnCbnSelchangeSendindCombo();
	afx_msg void OnBnClickedOk();
};
