#pragma once
#include "Display.h"
#include "Alarm.h"
#include "Railing.h"

#include <set>
using std::set;

#define FREE 0
#define USED 1

class InfoSystem
{
public:
    InfoSystem(int e, int n, Display* display, Alarm* pAlarm,
        Railing* pEntranceRailing, Railing* pExitRailing);
    ~InfoSystem();

private:
    // 员工数
    int m_employeeSum;
    // 单排车位数
    int m_halfParkingSum;
    // 空闲车位数
    int m_freeParkingSum;
    // 管理所有车位的状态
    char* m_parkingLotStateArray;
    // 显示屏
    Display* m_pDisplay;
    // 警报器
    Alarm* m_pAlarm;
    // 入口栏杆
    Railing* m_pEntranceRailing;
    // 出口栏杆
    Railing* m_pExitRailing;
    // 当前场内卡号集合
    set<int> m_cardInCollection;

public:
    // 获取离入口最近的空闲车位编号
    int getEntranceNearestParkingLot();
    // 获取离出口最近的空闲车位编号
    int getExitNearestParkingLot();
    // 归还车位
    void giveBackParkingLot(int i);
    // 请求特定编号的车位，如果该车位被占用就返回-1，否则返回编号
    int requestSpecialParkingLot(int i);
    // 获取单排车位数量
    int getHalfParkingLotSum();
    // 查询卡号是否合法
//    bool isValid(int cardNum);
    // 一辆汽车已经通过栏杆，驶入了停车场
    void aCarEntered();
    // 一辆汽车通过栏杆，出了停车场
    void aCarExit();
    // 汽车进场时刷卡
    bool readCardToEnter(int num);
    // 出场时刷卡
    bool readCardToExit(int num);
    // 查询员工数
    int getEmployeeSum();
};

