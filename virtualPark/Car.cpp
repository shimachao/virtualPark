#include "stdafx.h"
#include "Car.h"
#include <random>

using std::default_random_engine;
using std::uniform_int_distribution;

Car::Car()
    : m_cardNum(0)
    , m_speedx(0)
    , m_speedy(0)
    , m_location(Point(-1,3))
    , m_direction(RIGHT)
    , m_state(INITIALIZATION)
    , m_parkingSpaceNum(-1)
    , m_halfParkingSpaceSum(0)
    , m_parkDuration(0)
    , m_pEntranceCardReader(NULL)
    , m_pExitCardReader(NULL)
    , m_pEntranceSensor(NULL)
{
    // 随机生成停车时长,5~300秒，即五分钟以内
    static default_random_engine e;
    static uniform_int_distribution<unsigned> u(5, 300);
    m_parkDuration = u(e);
}


Car::~Car()
{
}


void Car::update()
{
    switch (m_state)
    {
    case ENTERING:
        enteringUpdate();
        break;
    case SERACHING:
        searchingUpdate();
        break;
    case MOVING_TO_PARKING_SPACE:
        movingToParkingSpaceUpdate();
        break;
    case PARKED:
        parkedUpdate();
        break;
    case MOVING_TO_ROAD:
        movingToRoadUpdate();
        break;
    case MOVING_TO_EXIT:
        movingToExitUpdate();
        break;
    case MOVING_OUT:
        movingOutUpdate();
        break;
    default:
        break;
    }
    TRACE("\nloaction:x=%d,y=%d\n", m_location.x, m_location.y);
}


// 设置出口处传感器指针变量
void Car::setEntranceSensor(Sensor* pSensor)
{
    m_pEntranceSensor = pSensor;
}


// 设置出口处传感器指针变量
void Car::setExitSensor(Sensor* pSensor)
{
    m_pExitSensor = pSensor;
}


// 收到开始入场消息
void Car::beginEnter()
{
    TRACE("\n beginEnter() \n");
    if (m_state == INITIALIZATION)
    {
        // 刷卡，进入下一状态
        m_pEntranceCardReader->readCard(m_cardNum);
        m_state = WAITING_ENTER_PERMISSION;
    }
}


// 允许入场消息
void Car::allowEnter()
{
    TRACE("\n allowEnter() \n");
    if (m_state == WAITING_ENTER_PERMISSION)
    {
        m_speedx = 1;
        m_state = ENTERING;
    }
}


// 允许上车道消息
void Car::allowToRoad()
{
    TRACE("\n allowToRoad() \n");
    // 在“等待出车道权限”状态时收到“准许出车道”消息，则
    // 修改速度，进入“向车道行驶”状态
    if (m_state == WAITING_ROAD_FREE)
    {
        if (m_direction == UPWARD)
        {
            m_speedy = -1;
        }
        else if(m_direction == DOWNWARD)
        {
            m_speedy = 1;
        }
        m_state = MOVING_TO_ROAD;
    }
}


// 准许出场了
void Car::allowExit()
{
    TRACE("\n allowExit() \n");
    // 如果在“等待出场权限”时收到“允许出场”的消息，则修改速度并进入“正在出场中”状态
    if (m_state == WAITING_EXIT_PERMISSION)
    {
        m_speedx = 1;
        m_state = MOVING_OUT;
    }
}


// 拒绝入场请求的消息
void Car::rejectEnterRequest()
{
    TRACE("\n rejectEnterRequest() \n");
    if (m_state == WAITING_ENTER_PERMISSION)
    {
        m_state = FINAL;
    }
}


// 收到拒绝出场请求的消息
void Car::rejectOutRequest()
{
    TRACE("\n rejectOutRequest() \n");
    // 如果出场请求被拒绝，则原路返回，进入“搜索车位”状态
    if (m_state == WAITING_EXIT_PERMISSION)
    {
        // 速度反向
        m_speedx = -1;
        // 调整朝向
        m_direction = LEFT;
        m_location.x -= 1;
        // 转入“搜索车位状态”
        m_state = SERACHING;
    }
}


// “正在入场”时的更新方法
void Car::enteringUpdate()
{
    TRACE("enteringUpdate()");
    // 更新坐标
    m_location.x += m_speedx;
    // 检测是否通过了栏杆
    if (m_location.x == 1)
    {
        // 如果通过了，就进入下一状态
        m_state = SERACHING;
        // 通知传感器
        m_pEntranceSensor->aCarPassed(this);
    }
}


// “搜索车位”时的更新方法
void Car::searchingUpdate()
{
    TRACE("searchingUpdate()");
    // 更新坐标
    m_location.x += m_speedx;
    // 检测是否到达车位跟前
    if (m_location.x == m_parkingSpaceNum % m_halfParkingSpaceSum + 2)
    {
        // 如果已到达，则修改速度和朝向并进入“进入车位”状态
        m_speedx = 0;
        if (m_parkingSpaceNum < m_halfParkingSpaceSum)
        {
            m_speedy = -1;
            m_direction = UPWARD;
        }
        else
        {
            m_speedy = 1;
            m_direction = DOWNWARD;
        }

        m_state = MOVING_TO_PARKING_SPACE;
    }
}


// “向车位移动”时的更新方法
void Car::movingToParkingSpaceUpdate()
{
    // 更新坐标
    m_location.y += m_speedy;
    // 检测是否已经到达车位
    if (m_direction == UPWARD && m_location.y == 0)
    {
        // 修改速度和朝向，进入下一状态
        m_speedy = 0;
        m_location.y = 1;
        m_direction = DOWNWARD;
        m_state = PARKED;
    }
    else if (m_direction == DOWNWARD && m_location.y == 5)
    {
        m_speedy = 0;
        m_direction = UPWARD;
        m_location.y = 4;
        m_state = PARKED;
    }
}


// “已停车”状态下的更新方法
void Car::parkedUpdate()
{
    // 递减停车时长
    m_parkDuration--;
    // 如果时间到了就像系统申请上车道离开
    if (m_parkDuration == 0)
    {
        m_state = WAITING_ROAD_FREE;
    }
}


// “向车道移动”状态的更新方法
void Car::movingToRoadUpdate()
{
    // 更新坐标
    m_location.y += m_speedy;
    // 检测是否到达车道，到达了就进入“向出口移动”状态
    if ((m_direction == DOWNWARD && m_location.y == 3)||
        (m_direction == UPWARD && m_location.y == 2))
    {
        m_direction = RIGHT;
        m_speedy = 0;
        m_speedx = 1;
        m_state = MOVING_TO_EXIT;
    }
}


// “向出口移动”时的更新方法
void Car::movingToExitUpdate()
{
    // 更新坐标
    m_location.x += m_speedx;
    // 判断是否到达出口跟前
    if (m_location.x == m_halfParkingSpaceSum+3)
    {
        // 如果到达了出口跟前，修改速度，刷卡，进入下一状态
        m_speedx = 0;
        m_pExitCardReader->readCard(m_cardNum);
        m_state = WAITING_EXIT_PERMISSION;
    }
}


// “正在出场”时的更新函数
void Car::movingOutUpdate()
{
    // 更新坐标
    m_location.x += m_speedx;
    // 判断是否已经通过出口栏杆
    if (m_location.x == m_halfParkingSpaceSum + 5)
    {
        // 如果已经通过，则通知传感器，并进入下一状态
        m_pExitSensor->aCarPassed(this);
        m_state = FINAL;
    }
}


// 查询汽车的位置坐标
Point Car::getLocation()
{
    return m_location;
}


// 查询汽车车头朝向
CarDirection Car::getDirection()
{
    return m_direction;
}


// 查询卡号
int Car::getCardNum()
{
    return m_cardNum;
}



// 查询占用的车位号
int Car::getParkingSpaceNum()
{
    return m_parkingSpaceNum;
}


// 返回汽车所处状态
CarState Car::getState()
{
    return m_state;
}


// 设置入口读卡器
void Car::setEntranceCardReader(CardReader* pCardReader)
{
    m_pEntranceCardReader = pCardReader;
}


// 设置出口读卡器
void Car::setExitCardReader(CardReader* pCardReader)
{
    m_pExitCardReader = pCardReader;
}


// 获取停车时长
int Car::getParkDuration()
{
    return m_parkDuration;
}



// 设置车位号
void Car::setParkingSpaceNum(int num)
{
    m_parkingSpaceNum = num;
}


// 设置汽车图片
void Car::setImg(Image* (*pImgArray)[4])
{
    m_imgArray = pImgArray;
}


// 绘制汽车
void Car::draw(Graphics* pGraphics)
{
    // 计算坐标
    RectF rcDest;
    Image *pImg = nullptr;
    
    switch (m_direction)
    {
    case UPWARD:
        rcDest.X = m_location.x * 50;
        rcDest.Width = 50;
        rcDest.Y = m_location.y * 50;
        rcDest.Height = 100;
        pImg = (*m_imgArray)[0];
        break;
    case DOWNWARD:
        rcDest.X = m_location.x * 50;
        rcDest.Width = 50;
        rcDest.Y = (m_location.y - 1) * 50;
        rcDest.Height = 100;
        pImg = (*m_imgArray)[1];
        break;
    case LEFT:
        rcDest.X = m_location.x * 50;
        rcDest.Width = 100;
        rcDest.Y = m_location.y * 50;
        rcDest.Height = 50;
        pImg = (*m_imgArray)[2];
        break;
    case RIGHT:
        rcDest.X = (m_location.x - 1) * 50;
        rcDest.Width = 100;
        rcDest.Y = m_location.y * 50;
        rcDest.Height = 50;
        pImg = (*m_imgArray)[3];
        break;
    }
    // 绘制图形
    TRACE("\nlocation=(%d,%d)\n rcDest=[%f,%f,%f,%f]\n", m_location.x, m_location.y, rcDest.X, rcDest.Y, rcDest.Width, rcDest.Height);
    pGraphics->DrawImage(pImg, rcDest);
    

    // 绘制编号
    WCHAR string[4] = { '\0' };
    swprintf_s(string, 4, L"%d", m_cardNum);
    FontFamily   fontFamily(L"Arial");
    Gdiplus::Font         font(&fontFamily, 16, FontStyleBold, UnitPoint);
    StringFormat stringFormat;
    SolidBrush   solidBrush(Color(0, 0, 0));
    stringFormat.SetAlignment(StringAlignmentCenter);
    stringFormat.SetLineAlignment(StringAlignmentCenter);

    pGraphics->DrawString(string, -1, &font, rcDest, &stringFormat, &solidBrush);
}


// 设置卡号
void Car::setCardNum(int num)
{
    m_cardNum = num;
}


void Car::setHalfParkspaceSum(int sum)
{
    m_halfParkingSpaceSum = sum;
}


// 生成一辆已停好的汽车，但其车牌号是错误的
Car* Car::GenerateAWrongParkedCar(int parkSpaceNum, int halfSpaceSum)
{
    Car* pCar = new Car();
    // 为了及时看到效果设置其停车时长为0秒
    pCar->m_parkDuration = 0;
    pCar->m_state = WAITING_ROAD_FREE;
    pCar->m_parkingSpaceNum = parkSpaceNum;
    pCar->m_halfParkingSpaceSum = halfSpaceSum;
    // 设置位置和方向
    if (parkSpaceNum < halfSpaceSum)
    {
        pCar->m_direction = DOWNWARD;
        pCar->m_location.x = parkSpaceNum + 2;
        pCar->m_location.y = 1;
    }
    else
    {
        pCar->m_direction = UPWARD;
        pCar->m_location.x = parkSpaceNum % halfSpaceSum + 2;
        pCar->m_location.y = 4;
    }

    return pCar;
}
