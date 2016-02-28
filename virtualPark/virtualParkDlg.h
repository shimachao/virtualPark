
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

    afx_msg void OnBnClickedStart();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedInsert();
    afx_msg void OnBnClickedInsertWrongExitCar();
    afx_msg void OnBnClickedGenerateGraphics();
    afx_msg void OnClose();

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
    // ���½�������ʾ������
    void updateDlgData();
    // ����ģ�����ݵ��ļ�
    void saveDataToFile();

private:
    // ��ʼģ���ʱ��
    SYSTEMTIME m_startTime;
    // ģ�����ʱ��
    SYSTEMTIME m_endTime;
    // ����ͣ��λ��
    int m_halfParkingSpaceSum;
    // ģ����������ʱ������ͣʱ����
    int m_clickTimeSum;
    // ģ����������ʱ��
    UINT_PTR m_clickTimer;
    // ģ����
    Simulator* m_pSimulator;
};
