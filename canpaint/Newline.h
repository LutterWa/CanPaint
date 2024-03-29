#pragma once

#include <vector>

class LINE
{
public:
	int id;//CAN帧ID
	//各byte系数
	int mu7;
	int mu6;
	int mu5;
	int mu4;
	int mu3;
	int mu2;
	int mu1;
	int mu0;

	float ex;//放大参数
	int mv;//平移参数
	unsigned int tms;
	std::vector<std::vector<unsigned char>>data;//各byte
	std::vector<float>point;//byte乘系数后存储于point
	COLORREF color;//曲线颜色

	LINE();//构造函数
	~LINE();//析构函数
	LINE(int tid, int t7, int t6, int t5, int t4, int t3, int t2, int t1, int t0, COLORREF tcolor, float tex, int tmv, int ms);

	float Process(void);//byte乘系数函数
};

// Newline 对话框
class Newline : public CDialogEx
{
	DECLARE_DYNAMIC(Newline)

public:
	Newline(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Newline();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWLINE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeColorCombo();
	CComboBox m_color;
	CString m_canid;

	int m_7;
	int m_6;
	int m_5;
	int m_4;
	int m_3;
	int m_2;
	int m_1;
	int m_0;

	float m_ex;
	int m_mv;
	unsigned int m_tms;
	COLORREF pen;
	LINE line;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};

//COLOR LIST
namespace usrdefcolor
{
	const auto red = RGB(255, 0, 0);    //红色
	const auto pink = RGB(255, 0, 255);  //粉红
	const auto rose = RGB(255, 253, 204);//玫红
	const auto brown = RGB(153, 51, 0);   //褐色
	const auto orange = RGB(255, 102, 0);  //橙色
	const auto gold = RGB(255, 204, 0);  //金色
	const auto yellow = RGB(255, 255, 0);  //黄色
	const auto green = RGB(0, 128, 0);    //绿色
	const auto cyan = RGB(0, 128, 128);  //青色
	const auto blue = RGB(0, 0, 255);    //蓝色
	const auto purple = RGB(128, 0, 128);  //紫色
	const auto gray80 = RGB(51, 51, 51);   //80%灰
	const auto gray50 = RGB(128, 128, 128);//50%灰
	const auto gray40 = RGB(153, 153, 153);//40%灰
	const auto gray25 = RGB(192, 192, 192);//25%灰
}
