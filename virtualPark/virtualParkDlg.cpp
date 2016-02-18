
// virtualParkDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "virtualPark.h"
#include "virtualParkDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CLICK 1

// CvirtualParkDlg 对话框



CvirtualParkDlg::CvirtualParkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIRTUALPARK_DIALOG, pParent)
    , m_pParkGraphics(NULL)
    , m_pDisplayGraphics(NULL)
    , m_pAlarmGraphics(NULL)
    , m_halfParkingSpaceSum(0)
    , m_pSimulator(NULL)
    , m_clickTimer(0)
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


// CvirtualParkDlg 消息处理程序

BOOL CvirtualParkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 从这里开始是自己添加的初始化代码
    // 设置整个客户区大小
    setClientSize(700,700);
    // 设置显示屏坐标
    CWnd* pwnd = GetDlgItem(IDC_DISPLAY);
    pwnd->MoveWindow(20, 130, 110, 110);
    // 设置警报器坐标
    pwnd = GetDlgItem(IDC_ALARM);
    pwnd->MoveWindow(620, 160, 60, 60);
    // 设置车场模拟图坐标
    pwnd = GetDlgItem(IDC_PARK);
    pwnd->MoveWindow(0, 250, 700, 300);
    // 设置及时消息框坐标
    pwnd = GetDlgItem(IDC_INFORM);
    pwnd->MoveWindow(0, 550, 500, 150);
    // 创建GDI+绘制对象
    m_pParkGraphics = ::new Graphics(GetDlgItem(IDC_PARK)->GetSafeHwnd());
    m_pDisplayGraphics = ::new Graphics(GetDlgItem(IDC_DISPLAY)->GetSafeHwnd());
    m_pAlarmGraphics = ::new Graphics(GetDlgItem(IDC_ALARM)->GetSafeHwnd());

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CvirtualParkDlg::OnPaint()
{
    //TRACE("\n CvirtualParkDlg::OnPaint()");
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
        if (m_pSimulator)
        {
            SolidBrush whiteBrush(Color(255, 255, 255));
            m_pParkGraphics->FillRectangle(&whiteBrush, 0, 0, 700, 300);
            m_pSimulator->draw(m_pParkGraphics, m_pDisplayGraphics, m_pAlarmGraphics);
        }
        
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CvirtualParkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// 设置窗口客户区大小
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
    // 获取用户输入，生成图形
    int employeeSum = GetDlgItemInt(IDC_EMPLOYEE_SUM);
    if (employeeSum <=0)
    {
        MessageBox(L"请输入合适的员工数");
        return;
    }

    m_halfParkingSpaceSum = GetDlgItemInt(IDC_PARKING_SPACE_HALF_SUM);
    if (m_halfParkingSpaceSum < 5 || m_halfParkingSpaceSum > 10)
    {
        MessageBox(L"单排车位数量必须在5~10之间");
        return;
    }

    // 将输入控件和生成图形按钮设为无效，它们的使命已完成
    GetDlgItem(IDC_EMPLOYEE_SUM)->EnableWindow(FALSE);
    GetDlgItem(IDC_PARKING_SPACE_HALF_SUM)->EnableWindow(FALSE);
    GetDlgItem(IDC_GENERATE_GRAPHICS)->EnableWindow(FALSE);

    // 创建模拟器对象
    m_pSimulator = new Simulator(employeeSum, m_halfParkingSpaceSum);
}


void CvirtualParkDlg::OnBnClickedStart()
{
    // 将“开始模拟”按钮设为不可用
    GetDlgItem(IDC_START)->EnableWindow(FALSE);
    // 开启时钟
    m_clickTimer = SetTimer(CLICK, 1000, NULL);
}


void CvirtualParkDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == m_clickTimer)
    {
        m_pSimulator->click();
        Invalidate(FALSE);
        UpdateWindow();
    }

    CDialogEx::OnTimer(nIDEvent);
}
