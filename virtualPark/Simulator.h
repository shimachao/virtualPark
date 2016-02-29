#pragma once
#include "InfoSystem.h"
#include "Display.h"
#include "CardReader.h"
#include "Railing.h"
#include "Sensor.h"
#include "Alarm.h"
#include "Car.h"
#include <list>
#include <deque>
#include "gdiplusheaders.h"
using std::list;
using std::deque;

//模拟器类
class Simulator
{
public:
    Simulator(int e, int n);
    ~Simulator();
    // 驱动模拟器
    void click();
    // 绘制
    void draw(Graphics* pGraphics);
    // 查询场内汽车数
    int getCarInSum();
    // 查询入场车次
    int getCarEnterSum();
    // 查询出场车次
    int getCarExitSum();
    // 获取平均停车时长
    int getAverageParkedTime();
    // 生成错误错误的入场车辆
    void generateAWrongCarToEnter();
    // 生成错误的出场车辆
    void generateAWrongCarToExit();

private:
    // 删除FINAL状态的汽车
    void deleteFinalCar();
    // 查看道路是否空闲
    bool isRoadFree();
    // 更新所有对象的状态
    void update();
    // 收集准备离场需要上车道的汽车
    void collectWaitingForRoadCar();
    // 准许停车场上的一辆车上车道
    void allowParkedCarToRoad();
    // 允许在外等待的汽车开始入场
    void allowWaitCarToEnter();
    // 绘制栏杆
    void drawRailing(Graphics* pParkGraphics);
    // 绘制车位
    void drawParkingSpace(Graphics* pParkGraphics);
    // 绘制警报器
    void drawAlarm(Graphics* pAlarmGraphics);
    // 加载汽车图片
    void loadCarImg();
    // 生成一辆入场的汽车
    void generateACarToEner();

private:
    // 显示屏
    Display* m_pDisplay;
    // 入口读卡器
    CardReader* m_pEntranceCardReader;
    // 出口读卡器
    CardReader* m_pExitCardReader;
    // 入口栏杆
    Railing* m_pEntranceRailing;
    // 出口栏杆
    Railing* m_pExitRailing;
    // 入口传感器
    Sensor* m_pEntranceSensor;
    // 出口传感器
    Sensor* m_pExitSensor;
    // 报警器
    Alarm* m_pAlarm;
    // 信息系统
    InfoSystem* m_pInfoSystem;
    // 刷卡后的汽车列表
    list<Car*> m_carList;
    // 卡号队列
    deque<int> m_cardNumQue;
    // 在场外等待的汽车队列
    deque<Car*> m_waitQueue;
    // 想出场并在等待车道空闲的汽车队列
    deque<Car*> m_waitRoadQue;
    // 场内车数量
    int m_carInParkingLotSum;
    // 所有汽车的总停车时间
    int m_totalParkTime;
    // 入场车次
    int m_carEnteredSum;
    // 出场车次
    int m_carExitedSum;
    // 汽车图片数组
    Image* m_carImgArray[4][4];
    // 栏杆图片
    Image* m_pRailingImage;
};

