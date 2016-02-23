
// virtualParkDlg.h : 头文件
//

#pragma once
#include "Simulator.h"

// CvirtualParkDlg 对话框
class CvirtualParkDlg : public CDialogEx
{
// 构造
public:
	CvirtualParkDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIRTUALPARK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
    // 设置窗口客户区大小
    void setClientSize(int width, int height);
    // 单排停车位数
    int m_halfParkingSpaceSum;
public:
    afx_msg void OnBnClickedGenerateGraphics();
private:
    // 模拟器
    Simulator* m_pSimulator;
public:
    afx_msg void OnBnClickedStart();
private:
    // 模拟器的驱动时钟
    UINT_PTR m_clickTimer;
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
    // 更新界面上显示的数据
    void updateDlgData();
    // 模拟器运行总时长，暂停时不算
    int m_clickTimeSum;
public:
    afx_msg void OnBnClickedInsert();
};
