
// virtualParkDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "virtualPark.h"
#include "virtualParkDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CLICK 1

// CvirtualParkDlg �Ի���



CvirtualParkDlg::CvirtualParkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIRTUALPARK_DIALOG, pParent)
    , m_halfParkingSpaceSum(0)
    , m_pSimulator(NULL)
    , m_clickTimer(0)
    , m_clickTimeSum(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CvirtualParkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CvirtualParkDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_GENERATE_GRAPHICS, &CvirtualParkDlg::OnBnClickedGenerateGraphics)
    ON_BN_CLICKED(IDC_START, &CvirtualParkDlg::OnBnClickedStart)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CvirtualParkDlg ��Ϣ�������

BOOL CvirtualParkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// �����￪ʼ���Լ���ӵĳ�ʼ������
    // ���������ͻ�����С
    setClientSize(700,700);
    //// ������ʾ������
    //CWnd* pwnd = GetDlgItem(IDC_DISPLAY);
    //pwnd->MoveWindow(20, 130, 110, 110);
    //// ���þ���������
    //pwnd = GetDlgItem(IDC_ALARM);
    //pwnd->MoveWindow(620, 160, 60, 60);
    //// ���ó���ģ��ͼ����
    //pwnd = GetDlgItem(IDC_PARK);
    //pwnd->MoveWindow(0, 250, 700, 300);
    // ���ü�ʱ��Ϣ������
    CWnd* pwnd = GetDlgItem(IDC_INFORM);
    pwnd->MoveWindow(0, 555, 500, 145);
    //// ����GDI+���ƶ���
    //m_pParkGraphics = ::new Graphics(GetDlgItem(IDC_PARK)->GetSafeHwnd());
    //m_pDisplayGraphics = ::new Graphics(GetDlgItem(IDC_DISPLAY)->GetSafeHwnd());
    //m_pAlarmGraphics = ::new Graphics(GetDlgItem(IDC_ALARM)->GetSafeHwnd());

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CvirtualParkDlg::OnPaint()
{
    //TRACE("\n CvirtualParkDlg::OnPaint()");
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
        if (m_pSimulator)
        {
            // �����ڴ����DC��׼��˫����
            CPaintDC dc(this);
            HDC hMemDC = CreateCompatibleDC(dc);
            HBITMAP hMemBitMap = CreateCompatibleBitmap(dc, 700, 700);
            SelectObject(hMemDC, hMemBitMap);
            // �����ڴ�DC����GDI+���ƶ���
            Graphics graphics(hMemDC);
            // ����
            SolidBrush whiteBrush(Color(255, 255, 255));
            graphics.FillRectangle(&whiteBrush, 0, 0, 700, 700);
            m_pSimulator->draw(&graphics);
            // ���Ƶ�Ŀ��DC
            BitBlt(dc, 0, 130, 700, 421, hMemDC, 0, 130, SRCCOPY);
            DeleteDC(hMemDC);
        }
        
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CvirtualParkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// ���ô��ڿͻ�����С
void CvirtualParkDlg::setClientSize(int width, int height)
{
    RECT rcWindow, rcClient;
    GetWindowRect(&rcWindow);
    GetClientRect(&rcClient);

    auto borderWidth = (rcWindow.right - rcWindow.left) - (rcClient.right - rcClient.left);
    auto borderHight = (rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top);
    SetWindowPos(0, 0, 0, borderWidth + width, borderHight + height, SWP_NOMOVE | SWP_NOZORDER);
}


void CvirtualParkDlg::OnBnClickedGenerateGraphics()
{
    // ��ȡ�û����룬����ͼ��
    int employeeSum = GetDlgItemInt(IDC_EMPLOYEE_SUM);
    if (employeeSum <=0)
    {
        MessageBox(L"��������ʵ�Ա����");
        return;
    }

    m_halfParkingSpaceSum = GetDlgItemInt(IDC_PARKING_SPACE_HALF_SUM);
    if (m_halfParkingSpaceSum < 5 || m_halfParkingSpaceSum > 10)
    {
        MessageBox(L"���ų�λ����������5~10֮��");
        return;
    }

    // ������ؼ�������ͼ�ΰ�ť��Ϊ��Ч�����ǵ�ʹ�������
    GetDlgItem(IDC_EMPLOYEE_SUM)->EnableWindow(FALSE);
    GetDlgItem(IDC_PARKING_SPACE_HALF_SUM)->EnableWindow(FALSE);
    GetDlgItem(IDC_GENERATE_GRAPHICS)->EnableWindow(FALSE);

    // ����ģ��������
    m_pSimulator = new Simulator(employeeSum, m_halfParkingSpaceSum);

    // ֪ͨ���ڸ���
    Invalidate(FALSE);
    UpdateWindow();
}


void CvirtualParkDlg::OnBnClickedStart()
{
    static bool startOrStop = true;

    if (startOrStop)
    {
        // ������ʼģ�⡱��ť��Ϊ"ֹͣģ��"
        SetDlgItemText(IDC_START ,_T("ֹͣģ��"));
        // ����ʱ��
        m_clickTimer = SetTimer(CLICK, 1000, NULL);
    }
    else
    {
        // ����ֹͣģ�⡱��ť��Ϊ����ʼģ�⡱��ť
        SetDlgItemText(IDC_START, _T("��ʼģ��"));
        // �ر�ʱ��
        KillTimer(m_clickTimer);
    }
    
    startOrStop = !startOrStop;
}


void CvirtualParkDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == m_clickTimer)
    {
        m_pSimulator->click();
        m_clickTimeSum++;
        updateDlgData();
        Invalidate(FALSE);
        UpdateWindow();
    }

    CDialogEx::OnTimer(nIDEvent);
}


// ���½�������ʾ������
void CvirtualParkDlg::updateDlgData()
{
    // ���ó���������
    CString s;
    s.Format(L"%d��", m_pSimulator->getCarInSum());
    SetDlgItemText(IDC_CAR_IN_SUM, s);
    // �볡����
    s.Format(L"%d��", m_pSimulator->getCarEnterSum());
    SetDlgItemText(IDC_ENTER_TIMES, s);
    // ��������
    s.Format(L"%d��", m_pSimulator->getCarExitSum());
    SetDlgItemText(IDC_OUT_TIMES, s);
    // ģ��������ʱ��
    s.Format(L"%d��", m_clickTimeSum);
    SetDlgItemText(IDC_TIME_PASSED, s);
    // ƽ��ͣ��ʱ��
    s.Format(L"%d��", m_pSimulator->getAverageParkedTime());
    SetDlgItemText(IDC_AVERAGE_TIME, s);
}
