#include "stdafx.h"
#include "Simulator.h"
#include <random>
#include <algorithm>
using std::random_shuffle;
using std::sort;
using std::default_random_engine;
using std::bernoulli_distribution;

using std::bernoulli_distribution;
using std::default_random_engine;

Simulator::Simulator(int e, int n)
    : m_pDisplay(nullptr)
    , m_pEntranceCardReader(nullptr)
    , m_pExitCardReader(nullptr)
    , m_pEntranceRailing(nullptr)
    , m_pEntranceSensor(nullptr)
    , m_pExitSensor(nullptr)
    , m_pAlarm(nullptr)
    , m_pInfoSystem(nullptr)
    , m_carInParkingLotSum(0)
    , m_totalParkTime(0)
    , m_carEnteredSum(0)
    , m_carExitedSum(0)
{
    // 创建显示屏
    m_pDisplay = new Display();
    // 创建警报器
    m_pAlarm = new Alarm();
    // 创建两个栏杆
    m_pEntranceRailing = new Railing();
    m_pExitRailing = new Railing();
    // 创建信息系统
    m_pInfoSystem = new InfoSystem(
        e,
        n,
        m_pDisplay,
        m_pAlarm,
        m_pEntranceRailing, // 入口栏杆
        m_pExitRailing);    // 出口栏杆
    // 创建两个读卡器
    InfoSystem* p = this->m_pInfoSystem;

    m_pEntranceCardReader = new CardReader(
        [p](int num)->bool {return p->readCardToEnter(num); });

    m_pExitCardReader = new CardReader(
        [p](int num)->bool {return p->readCardToExit(num); });

    // 创建两个传感器
    m_pEntranceSensor = new Sensor([p, this](Car* pCar) {
        p->aCarEntered(); // 通知系统
        // 获取车位号
        pCar->setParkingSpaceNum(p->getEntranceNearestParkingLot());
        this->m_carInParkingLotSum++; // 更新场内车数量
        this->m_carEnteredSum++; // 更新入场车次
        // 更新总停车时间
        this->m_totalParkTime += pCar->getParkDuration(); });

    m_pExitSensor = new Sensor([p, this](Car* pCar) {
        p->aCarExit(); // 通知系统
        p->giveBackParkingLot(pCar->getParkingSpaceNum()); // 归还车位
        this->m_carInParkingLotSum--; // 更新场内车数量
        // 更新出场车次
        this->m_carExitedSum++; });

    // 根据员工人数生成卡号，卡号为[0, e-1]，但是卡号要打散
    int *a = new int[e];
    for (size_t i = 0; i < e; i++)
    {
        a[i] = i;
    }
    // 打散
    random_shuffle(a, a + e);
    // 插入卡号队列
    for (size_t i = 0; i < e; i++)
    {
        m_cardNumQue.push_back(a[i]);
    }
    delete a;

    // 加载栏杆图片
    m_pRailingImage = Image::FromFile(L"img/railing.png");

    // 加载汽车图片
    loadCarImg();
}


Simulator::~Simulator()
{
    if (m_pAlarm)
    {
        delete m_pAlarm;
        m_pAlarm = nullptr;
    }

    if (m_pDisplay)
    {
        delete m_pDisplay;
        m_pDisplay = nullptr;
    }

    if (m_pEntranceRailing)
    {
        delete m_pEntranceRailing;
        m_pEntranceRailing = nullptr;
    }

    if (m_pExitRailing)
    {
        delete m_pExitRailing;
        m_pExitRailing = nullptr;
    }

    if (m_pEntranceCardReader)
    {
        delete m_pEntranceCardReader;
        m_pEntranceCardReader = nullptr;
    }

    if (m_pExitCardReader)
    {
        delete m_pExitCardReader;
        m_pExitCardReader;
    }

    if (m_pEntranceSensor)
    {
        delete m_pEntranceSensor;
        m_pEntranceSensor = nullptr;
    }

    if (m_pExitSensor)
    {
        delete m_pExitSensor;
        m_pExitSensor = nullptr;
    }

    if (m_pInfoSystem)
    {
        delete m_pInfoSystem;
        m_pInfoSystem = nullptr;
    }

    for each (Car* pCar in m_carList)
    {
        delete pCar;
    }

    for each ( Car* pCar in m_waitQueue)
    {
        delete pCar;
    }

    for each (Car* pCar in m_waitRoadQue)
    {
        delete pCar;
    }
}


// 驱动模拟器
void Simulator::click()
{
    // 更新所有对象的状态
    update();

    // 删除FINAL状态的汽车
    deleteFinalCar();

    // 收集处于WAIT_RAOD_FREE的汽车到m_waitRaodQue队列中
    collectWaitingForRoadCar();

    // 如果车道空闲，如果车道空闲，先处理要离场的汽车
    if (isRoadFree())
    {
        allowParkedCarToRoad();
    }

    // 如果道路还是空闲的就处理门口等待入场的汽车
    if (isRoadFree())
    {
        allowWaitCarToEnter();
    }

    // 随机生成入场的汽车
    static default_random_engine e;
    static bernoulli_distribution b(0.1); // 0.1的可能生成入场的汽车
    if (!m_cardNumQue.empty() && b(e))
    {
        generateACarToEner();
    }
   /* static bool f = true;
    if (f)
    {
        generateACarToEner();
        f = false;
    }*/
}


// 删除FINAL状态的汽车
void Simulator::deleteFinalCar()
{
    // 先查有没有车处于FINIAL状态，有就删除
    Car *pCar = nullptr;
    for each (Car* p in m_carList)
    {
        if (p->getState() == FINAL)
        {
            pCar = p;
            break;
        }
    }

    if (pCar)
    {
        m_cardNumQue.push_back(pCar->getCardNum());
        m_carList.remove(pCar);
        delete pCar;
        pCar = nullptr;
    }
}


// 查看道路是否空闲
bool Simulator::isRoadFree()
{
    bool isRoadFree = true;
    for each (Car* pCar in m_carList)
    {
        if (!(pCar->getState() == PARKED ||
            pCar->getState() == WAITING_ROAD_FREE))
        {
            isRoadFree = false;
            break;
        }
    }
    return isRoadFree;
}


// 更新所有对象的状态
void Simulator::update()
{
    for each (Car* pCar in m_carList)
    {
        pCar->update();
    }
    // 更新读卡器的状态
    m_pEntranceCardReader->update();
    m_pExitCardReader->update();
    // 更新栏杆的状态
    m_pEntranceRailing->update();
    m_pExitRailing->update();
    // 更新警报的状态
    m_pAlarm->update();
}


// 收集准备离场需要上车道的汽车
void Simulator::collectWaitingForRoadCar()
{
    list<Car*> waitForRoad;
    for each (Car* p in m_carList)
    {
        if (p->getState() == WAITING_ROAD_FREE)
        {
            waitForRoad.push_back(p);
        }
    }
    // 将刚收集的汽车从m_carList中删除
    for each (Car* p in waitForRoad)
    {
        m_carList.remove(p);
    }
    // 将waitForRoad根据车位号排好序后加入m_waitQueue
    // 都要离场的汽车，车位离出口近排在前面
    int n = m_pInfoSystem->getHalfParkingLotSum();
    waitForRoad.sort([n](Car*&p1, Car*&p2) {return
        (p1->getParkingSpaceNum() % n) > (p2->getParkingSpaceNum() % n); });
 /*   int n = m_pInfoSystem->getHalfParkingLotSum();
    sort(waitForRoad.begin(), waitForRoad.end(),
        [n](Car*&p1, Car*&p2) {return
        (p1->getParkingSpaceNum() % n) > (p2->getParkingSpaceNum() % n); });*/

    for each (Car* p in waitForRoad)
    {
        m_waitRoadQue.push_back(p);
    }
    waitForRoad.clear();
}


// 准许停车场上的一辆车上车道
void Simulator::allowParkedCarToRoad()
{
    if (!m_waitRoadQue.empty())
    {
        Car* pCar = m_waitRoadQue.front();
        // 通知汽车可以上路了
        pCar->allowToRoad();
        // 设置汽车的出口读卡器和传感器成员变量
        pCar->setExitCardReader(m_pExitCardReader);
        pCar->setExitSensor(m_pExitSensor);
        // 注册该汽车为读卡器的观察者，汽车被拒绝出场的消息由读卡器通知
        m_pExitCardReader->setRejectedAction([pCar]() {
            pCar->rejectOutRequest(); });
        // 注册该汽车为出口栏杆的观察者，汽车允许出场时由栏杆通知汽车
        m_pExitRailing->setUpAction([pCar]() {pCar->allowExit(); });
        // 从m_waitRoadQue列表移动到m_carList
        m_waitRoadQue.pop_front();
        m_carList.push_back(pCar);
    }
}


// 允许在外等待的汽车开始入场
void Simulator::allowWaitCarToEnter()
{
    if (!m_waitQueue.empty())
    {
        Car* pCar = m_waitQueue.front();
        // 设置汽车的入口读卡器变量
        pCar->setEntranceCardReader(m_pEntranceCardReader);
        // 设置汽车的入口传感器变量
        pCar->setEntranceSensor(m_pEntranceSensor);
        // 注册汽车为入口读卡器的观察者，被拒绝入场的消息由读卡器告知
        m_pEntranceCardReader->setRejectedAction([pCar]() {
            pCar->rejectEnterRequest(); });
        // 注册汽车为入口栏杆的观察者，准许入场的消息由栏杆通知
        m_pEntranceRailing->setUpAction([pCar]() {pCar->allowEnter(); });

        // 通知汽车入场
        pCar->beginEnter();

        // 从waitQueue移动到m_carList
        m_waitQueue.pop_front();
        m_carList.push_front(pCar);
    }
}


// 查询场内车数量
int Simulator::getCarInParkingLotSum()
{
    return m_carInParkingLotSum;
}


// 绘制
void Simulator::draw(Graphics* pGraphics)
{
    // 创建GDI+的坐标位移对象
    Matrix transform;

    // 绘制显示屏
    transform.Translate(20, 135);
    pGraphics->SetTransform(&transform);
    drawDisplay(pGraphics);
    transform.Reset();

    // 绘制警报器
    transform.Translate(620, 160);
    pGraphics->SetTransform(&transform);
    drawAlarm(pGraphics);
    transform.Reset();

    // 绘制车位、栏杆、汽车，它们使用同一套坐标
    transform.Translate(0, 250);
    pGraphics->SetTransform(&transform);
    // 绘制车位
    drawParkingSpace(pGraphics);
    // 绘制栏杆
    drawRailing(pGraphics);
    // 绘制汽车
    for each (Car* pCar in m_carList)
    {
        pCar->draw(pGraphics);
    }
    for each (Car* pCar in m_waitRoadQue)
    {
        pCar->draw(pGraphics);
    }

    transform.Reset();
    pGraphics->SetTransform(&transform);
}


void Simulator::drawRailing(Graphics* pParkGraphics)
{
    int halfParkingSpaceSum = m_pInfoSystem->getHalfParkingLotSum();
    // 绘制入口栏杆
    int railingState = m_pEntranceRailing->getState();
    if (railingState == DOWN)
    {
        pParkGraphics->DrawImage(m_pRailingImage, 0, 100, 9, 100);
    }
    else if (railingState == RAISING ||
        railingState == FALLING)
    {
        Rect destRect(0, 100, 9, 50);
        pParkGraphics->DrawImage(m_pRailingImage, destRect, 0, 0, 9, 100, Gdiplus::UnitPixel);
    }
    else
    {
        HatchBrush b(HatchStyleHorizontal, Color(255, 255, 0), Color(0, 0, 0));
        pParkGraphics->FillEllipse(&b, 0, 100, 10, 10);
    }
    // 绘制出口栏杆
    railingState = m_pExitRailing->getState();
    if (railingState == DOWN)
    {
        pParkGraphics->DrawImage(m_pRailingImage, (halfParkingSpaceSum + 4) * 50 , 100, 9, 100);
    }
    else if (railingState == RAISING ||
        railingState == FALLING)
    {
        Rect destRect((halfParkingSpaceSum + 4) * 50, 100, 9, 50);
        pParkGraphics->DrawImage(m_pRailingImage, destRect, 0, 0, 9, 100, Gdiplus::UnitPixel);
    }
    else
    {
        HatchBrush b(HatchStyleHorizontal, Color(255, 255, 0), Color(0, 0, 0));
        pParkGraphics->FillEllipse(&b, (halfParkingSpaceSum + 4) * 50, 100, 10, 10);
    }
}


// 绘制车位
void Simulator::drawParkingSpace(Graphics* pParkGraphics)
{
    Pen pen(Color(0, 0, 0));
    // 绘制边框
    int halfParkingSpaceSum = m_pInfoSystem->getHalfParkingLotSum();
    pParkGraphics->DrawRectangle(&pen, 0, 0, (halfParkingSpaceSum + 4) * 50, 300);
    // 绘制车位
    for (size_t i = 2; i <= halfParkingSpaceSum + 2; i++)
    {
        pParkGraphics->DrawLine(&pen, i * 50, 0, i * 50, 100);
        pParkGraphics->DrawLine(&pen, i * 50, 200, i * 50, 300);
    }
    pParkGraphics->DrawLine(&pen, 100, 100, (halfParkingSpaceSum + 2) * 50, 100);
    pParkGraphics->DrawLine(&pen, 100, 200, (halfParkingSpaceSum + 2) * 50, 200);
}

Gdiplus::Font* getQuartzRegularFont()
{
    PrivateFontCollection fontCollection;
    fontCollection.AddFontFile(L"res/Quartz Regular.ttf");
    FontFamily* pFontFamily = new FontFamily[1];

    int found = 0;
    fontCollection.GetFamilies(1, pFontFamily, &found);
    WCHAR familyName[LF_FACESIZE + 22];
    pFontFamily[0].GetFamilyName(familyName);

    TRACE(familyName);
    TRACE("\n%d\n",pFontFamily[0].IsStyleAvailable(FontStyleRegular));
    TRACE("\n%d\n", pFontFamily[0].IsStyleAvailable(FontStyleBold));
    Gdiplus::Font* pFont = ::new Gdiplus::Font(familyName, 18, FontStyleBold, UnitPixel, &fontCollection);

    return pFont;
}

// 绘制显示屏
void Simulator::drawDisplay(Graphics* pGraphics)
{
    // 绘制背景
    SolidBrush backBrush(Color(20, 20, 20));
    pGraphics->FillRectangle(&backBrush, 0, 0, 110, 110);
    
    Pen pen(Color(100,100,100));
    for (int x = 0; x <= 110; x += 5)
    {
        pGraphics->DrawLine(&pen, x, 0, x, 110);
    }
    for (int y = 0; y <= 110; y += 5)
    {
        pGraphics->DrawLine(&pen, 0, y, 110, y);
    }

    // 绘制文字
    WCHAR string[24] = { '\0' };
    swprintf_s(string, 24, L"当前场内还剩%d个空闲车位", m_pDisplay->getNumOfFreeParkingLots());
    FontFamily   fontFamily(L"Arial");
    RectF        rectF(0.0f, 10.0f, 110.0f, 110.0f);
    StringFormat stringFormat;
    SolidBrush   solidBrush(Color(255, 0, 0));

    stringFormat.SetAlignment(StringAlignmentCenter);

    stringFormat.SetLineAlignment(StringAlignmentCenter);
    static Gdiplus::Font* pFont = getQuartzRegularFont();
    pGraphics->DrawString(string, -1, pFont, rectF, &stringFormat, &solidBrush);
}


// 绘制警报器
void Simulator::drawAlarm(Graphics* pGraphics)
{
    // 绘制边框
    Pen pen(Color(0, 0, 0));
    pGraphics->DrawRectangle(&pen, 0, 0, 60, 60);
    AlarmState state = m_pAlarm->getState();
    if (state == OFF)
    {
        SolidBrush brush(Color(200, 200, 200));
        pGraphics->FillEllipse(&brush, 1, 1, 58, 58);
    }
    else
    {
        SolidBrush brush(Color(255, 0, 0));
        pGraphics->FillEllipse(&brush, 1, 1, 58, 58);
    }
}


// 加载汽车图片
void Simulator::loadCarImg()
{
    m_carImgArray[0][0] = Image::FromFile(L"img/1up.png");
    m_carImgArray[0][1] = Image::FromFile(L"img/1down.png");
    m_carImgArray[0][2] = Image::FromFile(L"img/1left.png");
    m_carImgArray[0][3] = Image::FromFile(L"img/1right.png");

    m_carImgArray[1][0] = Image::FromFile(L"img/2up.png");
    m_carImgArray[1][1] = Image::FromFile(L"img/2down.png");
    m_carImgArray[1][2] = Image::FromFile(L"img/2left.png");
    m_carImgArray[1][3] = Image::FromFile(L"img/2right.png");

    m_carImgArray[2][0] = Image::FromFile(L"img/3up.png");
    m_carImgArray[2][1] = Image::FromFile(L"img/3down.png");
    m_carImgArray[2][2] = Image::FromFile(L"img/3left.png");
    m_carImgArray[2][3] = Image::FromFile(L"img/3right.png");

    m_carImgArray[3][0] = Image::FromFile(L"img/4up.png");
    m_carImgArray[3][1] = Image::FromFile(L"img/4down.png");
    m_carImgArray[3][2] = Image::FromFile(L"img/4left.png");
    m_carImgArray[3][3] = Image::FromFile(L"img/4right.png");
}


// 生成一辆入场的汽车
void Simulator::generateACarToEner()
{
    TRACE("\n生成一辆入场汽车\n");
    // 生成入场的汽车
    Car* pCar = new Car();
    // 设置卡号
    pCar->setCardNum(m_cardNumQue.front());
    m_cardNumQue.pop_front();
    // 告知其车场一排车位的数量
    int sum = m_pInfoSystem->getHalfParkingLotSum();
    pCar->setHalfParkspaceSum(sum);
    // 设置汽车的图片
    int index = (pCar->getCardNum()) % 4;
    pCar->setImg(&m_carImgArray[index]);
    // 加入等待队列
    m_waitQueue.push_back(pCar);
}


// 查询场内汽车数
int Simulator::getCarInSum()
{
    return m_carInParkingLotSum;
}


// 查询入场车次
int Simulator::getCarEnterSum()
{
    return m_carEnteredSum;
}


// 查询出场车次
int Simulator::getCarExitSum()
{
    return m_carExitedSum;
}


// 获取平均停车时长
int Simulator::getAverageParkedTime()
{
    if (m_carEnteredSum != 0)
    {
        return m_totalParkTime / m_carEnteredSum;
    }
    else
    {
        return 0;
    }
}
