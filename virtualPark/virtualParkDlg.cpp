
// virtualParkDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "virtualPark.h"
#include "virtualParkDlg.h"
#include "afxdialogex.h"
#include <cstdlib>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CLICK 1

// CvirtualParkDlg 对话框



CvirtualParkDlg::CvirtualParkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIRTUALPARK_DIALOG, pParent)
    , m_halfParkingSpaceSum(0)
    , m_pSimulator(NULL)
    , m_clickTimer(0)
    , m_clickTimeSum(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    ZeroMemory(&m_startTime, sizeof(m_startTime));
    ZeroMemory(&m_endTime, sizeof(m_endTime));
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
    ON_BN_CLICKED(IDC_INSERT, &CvirtualParkDlg::OnBnClickedInsert)
    ON_BN_CLICKED(IDC_INSERT_WRONG_EXIT_CAR, &CvirtualParkDlg::OnBnClickedInsertWrongExitCar)
    ON_WM_CLOSE()
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
    setClientSize(730,600);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CvirtualParkDlg::OnPaint()
{
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
            // 创建内存兼容DC，准备双缓冲
            CPaintDC dc(this);
            HDC hMemDC = CreateCompatibleDC(dc);
            HBITMAP hMemBitMap = CreateCompatibleBitmap(dc, 720, 450);
            SelectObject(hMemDC, hMemBitMap);
            // 根据内存DC创建GDI+绘制对象
            Graphics graphics(hMemDC);
            // 绘制
            SolidBrush whiteBrush(Color(255, 255, 255));
            graphics.FillRectangle(&whiteBrush, 0, 0, 720, 450);
            m_pSimulator->draw(&graphics);
            // 复制到目标DC
            BitBlt(dc, 5, 100, 720, 450, hMemDC, 0, 0, SRCCOPY);
            DeleteDC(hMemDC);
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

    // 将“开始模拟”、“插入错误的入场车辆”，“插入错误的出场车辆”按钮设为有效
    GetDlgItem(IDC_START)->EnableWindow();
    GetDlgItem(IDC_INSERT)->EnableWindow();
    GetDlgItem(IDC_INSERT_WRONG_EXIT_CAR)->EnableWindow();

    // 通知窗口更新
    Invalidate(FALSE);
    UpdateWindow();
}


void CvirtualParkDlg::OnBnClickedStart()
{
    static bool startOrStop = true;

    if (startOrStop)
    {
        // 将“开始模拟”按钮设为"停止模拟"
        SetDlgItemText(IDC_START ,_T("停止模拟"));
        // 开启时钟
        m_clickTimer = SetTimer(CLICK, 1000, NULL);
    }
    else
    {
        // 将“停止模拟”按钮设为“开始模拟”按钮
        SetDlgItemText(IDC_START, _T("继续模拟"));
        // 关闭时钟
        KillTimer(m_clickTimer);
        // 保存结束时间
        GetLocalTime(&m_endTime);
    }
    
    startOrStop = !startOrStop;

    // 如果是第一次点击，还要保存开始时间
    static bool isFirstClick = true;
    if (isFirstClick)
    {
        isFirstClick = false;
        GetLocalTime(&m_startTime);
    }
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


// 更新界面上显示的数据
void CvirtualParkDlg::updateDlgData()
{
    // 设置场内汽车数
    CString s;
    s.Format(L"%d辆", m_pSimulator->getCarInSum());
    SetDlgItemText(IDC_CAR_IN_SUM, s);
    // 入场车次
    s.Format(L"%d次", m_pSimulator->getCarEnterSum());
    SetDlgItemText(IDC_ENTER_TIMES, s);
    // 出场车次
    s.Format(L"%d次", m_pSimulator->getCarExitSum());
    SetDlgItemText(IDC_OUT_TIMES, s);
    // 模拟器运行时长
    s.Format(L"%d秒", m_clickTimeSum);
    SetDlgItemText(IDC_TIME_PASSED, s);
    // 平均停车时长
    s.Format(L"%d秒", m_pSimulator->getAverageParkedTime());
    SetDlgItemText(IDC_AVERAGE_TIME, s);
}


void CvirtualParkDlg::OnBnClickedInsert()
{
    // 生成一个Car实例，并给其设置一个错误的号码，
    m_pSimulator->generateAWrongCarToEnter();
    // 更新界面
    Invalidate(FALSE);
    UpdateWindow();
}



void CvirtualParkDlg::OnBnClickedInsertWrongExitCar()
{
    m_pSimulator->generateAWrongCarToExit();
    // 更新界面
    Invalidate(FALSE);
    UpdateWindow();
}


// 保存模拟数据到文件
void CvirtualParkDlg::saveDataToFile()
{
    // 文件名
    CHAR fileName[32] = {'\0'};
    sprintf(fileName, "%d%02d%02d-%02d%02d%02d-%02d%02d%02d.rpt", m_startTime.wYear, m_startTime.wMonth, m_startTime.wDay, m_startTime.wHour, m_startTime.wMinute, m_startTime.wSecond, m_endTime.wHour, m_endTime.wMinute, m_endTime.wSecond);
    
    // 打开文件
    FILE *fp = fopen(fileName, "w");
    // 保存数据
    if (fp)
    {
        fprintf(fp, "本次仿真累计入场车次：%d\n本次仿真累计出场车次：%d\n本次仿真汽车的平均停车时间：%d秒\n", m_pSimulator->getCarEnterSum(), m_pSimulator->getCarExitSum(), m_pSimulator->getAverageParkedTime());
        fclose(fp);
    }
}


void CvirtualParkDlg::OnClose()
{
    if (m_pSimulator)
    {
        saveDataToFile();
    }
    CDialogEx::OnClose();
}
