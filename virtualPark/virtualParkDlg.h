
// virtualParkDlg.h : ͷ�ļ�
//

#pragma once
#include "Simulator.h"

// CvirtualParkDlg �Ի���
class CvirtualParkDlg : public CDialogEx
{
// ����
public:
	CvirtualParkDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIRTUALPARK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
    // ���ô��ڿͻ�����С
    void setClientSize(int width, int height);
    // ����ͣ��λ��
    int m_halfParkingSpaceSum;
public:
    afx_msg void OnBnClickedGenerateGraphics();
private:
    // ģ����
    Simulator* m_pSimulator;
public:
    afx_msg void OnBnClickedStart();
private:
    // ģ����������ʱ��
    UINT_PTR m_clickTimer;
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
    // ���½�������ʾ������
    void updateDlgData();
    // ģ����������ʱ������ͣʱ����
    int m_clickTimeSum;
public:
    afx_msg void OnBnClickedInsert();
};
