#include "stdafx.h"
#include "InfoSystem.h"



InfoSystem::InfoSystem(int e, int n, Display* display, Alarm* pAlarm,
    Railing* pEntranceRailing, Railing* pExitRailing)
    : m_freeParkingSum(2*n)
    , m_employeeSum(e)
    , m_halfParkingSum(n)
    , m_parkingLotStateArray(nullptr)
    , m_pDisplay(display)
    , m_pAlarm(pAlarm)
    , m_pEntranceRailing(pEntranceRailing)
    , m_pExitRailing(pExitRailing)
{
    // 设置显示屏
    m_pDisplay->setFreeParkingLotsNum(m_freeParkingSum);
    // 初始状态数组
    m_parkingLotStateArray = new char[m_halfParkingSum * 2];
    for (size_t i = 0; i < m_halfParkingSum * 2; i++)
    {
        m_parkingLotStateArray[i] = FREE;
    }
}


InfoSystem::~InfoSystem()
{
    if (m_parkingLotStateArray)
    {
        delete[] m_parkingLotStateArray;
        m_parkingLotStateArray = nullptr;
    }
}


// 获取离入口最近的空闲车位编号
int InfoSystem::getEntranceNearestParkingLot()
{
    // 从靠近入口的车位开始，寻找一个空位
    int r = -1; // 记录得到的空闲车位号
    for (size_t i = 0; i < m_halfParkingSum; i++)
    {
        if (m_parkingLotStateArray[i] == FREE)
        {
            r = i;
            break;
        }
        else if(m_parkingLotStateArray[i+m_halfParkingSum] == FREE)
        {
            r = i + m_halfParkingSum;
            break;
        }
    }
    // 如果找到了
    if (r != -1)
    {
        m_parkingLotStateArray[r] = USED;
        m_freeParkingSum--;
        m_pDisplay->setFreeParkingLotsNum(m_freeParkingSum);
    }
    
    return r;
}


// 获取离出口最近的空闲车位编号
int InfoSystem::getExitNearestParkingLot()
{
    // 从靠近出口的车位开始寻找空闲车位
    int r = -1; // 记录找到的空闲车位号
    for (size_t i = m_halfParkingSum - 1; i > 0; i--)
    {
        if (m_parkingLotStateArray[i] == FREE)
        {
            r = i;
            break;
        }
        else if (m_parkingLotStateArray[i + m_halfParkingSum] == FREE)
        {
            r = i + m_halfParkingSum;
            break;
        }
    }
    // 如果找到了，做好标记，更新显示屏上的数字
    if (r != -1)
    {
        m_parkingLotStateArray[r] = USED;
        m_freeParkingSum--;
        m_pDisplay->setFreeParkingLotsNum(m_freeParkingSum);
    }
    
    return r;
}


// 归还车位
void InfoSystem::giveBackParkingLot(int i)
{
    // 更新标记，更新显示屏上的数字
    m_parkingLotStateArray[i] = FREE;
    m_freeParkingSum++;
    m_pDisplay->setFreeParkingLotsNum(m_freeParkingSum);
}


// 请求特定编号的车位，如果该车位被占用就返回-1，否则返回编号
int InfoSystem::requestSpecialParkingLot(int i)
{
    if (m_parkingLotStateArray[i] == FREE)
    {
        m_parkingLotStateArray[i] = USED;
        m_freeParkingSum--;
        m_pDisplay->setFreeParkingLotsNum(m_freeParkingSum);
        return i;
    }
    else
    {
        return -1;
    }
}


// 获取单排车位数量
int InfoSystem::getHalfParkingLotSum()
{
    return m_halfParkingSum;
}


// 查询卡号是否合法
bool InfoSystem::isValid(int cardNum)
{
    
    return cardNum>=0 && cardNum<=m_employeeSum &&
        m_cardInCollection.find(cardNum) != m_cardInCollection.end();
}


// 一辆汽车已经通过栏杆，驶入了停车场
void InfoSystem::aCarEntered()
{
    m_pEntranceRailing->putDown();
}


// 一辆汽车通过栏杆，出了停车场
void InfoSystem::aCarExit()
{
    m_pExitRailing->putDown();
}


// 汽车进场时刷卡
bool InfoSystem::readCardToEnter(int num)
{
    // 如果卡号合法，且不在当前场内卡号集合中，则返回true
    if ((num >= 0 && num < m_employeeSum) &&
        m_cardInCollection.find(num) == m_cardInCollection.end())
    {
        TRACE("\n读卡结果合法\n");
        m_cardInCollection.insert(num);
        // 命令栏杆抬起
        m_pEntranceRailing->liftUp();
        return true;
    }
    else
    {
        TRACE("读卡结果不合法");
        return false;
    }
}


// 出场时刷卡
bool InfoSystem::readCardToExit(int num)
{
    // 如果卡号合法，且在当前场内卡号集合中，则返回true
    if ((num >= 0 && num < m_employeeSum) &&
        m_cardInCollection.find(num) != m_cardInCollection.end())
    {
        m_cardInCollection.erase(num);
        m_pExitRailing->liftUp();
        return true;
    }
    else
    {
        return false;
    }
}
