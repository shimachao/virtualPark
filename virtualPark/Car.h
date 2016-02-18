#pragma once
#include "Sensor.h"
#include "InfoSystem.h"
#include "CardReader.h"

struct Point
{
    Point(int x, int y) :x(x), y(y) {}
    int x;
    int y;
};

/*
汽车的几种不同状态：等待入场，等待进入许可，正在进入，正在搜索车位，向车位移动中，
已停好车，申请出场时等待行车道许可，向行车道移动，向出口移动，等待出场许可，
正在出场，终止
*/
enum CarState {INITIALIZATION, WAITING_ENTER_PERMISSION, ENTERING, SERACHING, MOVING_TO_PARKING_SPACE, PARKED, WAITING_ROAD_FREE, MOVING_TO_ROAD, MOVING_TO_EXIT, WAITING_EXIT_PERMISSION,MOVING_OUT, FINAL};

/*
汽车的朝向
UPWARD：向上
DOWNWRAD：向下
LEFT：向左
RIGHT：向右
*/

enum CarDirection {UPWARD, DOWNWARD, LEFT, RIGHT};

class Car
{
public:
    Car();
    ~Car();
    // 更新状态
    void update();
    // 设置入口读卡器
    void setEntranceCardReader(CardReader* pCardReader);
    // 设置出口读卡器
    void setExitCardReader(CardReader* pCardReader);
    // 设置出口处传感器指针变量
    void setEntranceSensor(Sensor* pSensor);
    // 设置出口处传感器指针变量
    void setExitSensor(Sensor* pSensor);
    // 设置车位号
    void setParkingSpaceNum(int num);
    // 收到开始入场消息
    void beginEnter();
    // 收到允许入场消息
    void allowEnter();
    // 收到允许上车道消息
    void allowToRoad();
    // 收到准许出场消息
    void allowExit();
    // 收到拒绝入场请求
    void rejectEnterRequest();
    // 收到拒绝出场请求
    void rejectOutRequest();

    // 查询汽车车头朝向
    CarDirection getDirection();
    // 查询汽车的位置坐标
    Point getLocation();
    // 返回汽车所处状态
    CarState getState();
    // 查询卡号
    int getCardNum();
    // 查询占用的车位号
    int getParkingSpaceNum();
    // 获取停车时长
    int getParkDuration();

    // 设置图片
    void setImg(Image* (*pImgArray)[4]);
    // 绘制汽车    
    void draw(Graphics* pGraphics);
    
private:
    // 卡号
    int m_cardNum;
    // x方向的速度
    int m_speedx;
    // y方向上的速度
    int m_speedy;
    // 车的位置
    Point m_location;
    // 车头的朝向
    CarDirection m_direction;
    // 汽车的状态
    CarState m_state;
    // 得到的车位编号
    int m_parkingSpaceNum;
    // 单排车位数量，用于和车位号以前计算车位的具体坐标
    int m_halfParkingSpaceSum;
    // 停车时长
    int m_parkDuration;
    // 入口处读卡器
    CardReader* m_pEntranceCardReader;
    // 出口处读卡器
    CardReader* m_pExitCardReader;
    // 入口传感器
    Sensor* m_pEntranceSensor;
    // 出口处传感器
    Sensor* m_pExitSensor;
    // 图片列表，包括上下左右四个朝向的图片
    Image* (*m_imgArray)[4];

private:
    // “正在入场”时的更新方法
    void enteringUpdate();
    // “搜索车位”时的更新方法
    void searchingUpdate();
    // “向车位移动”时的更新方法
    void movingToParkingSpaceUpdate();
    // “已停车”状态下的更新方法
    void parkedUpdate();
    // “向车道移动”状态的更新方法
    void movingToRoadUpdate();
    // “向出口移动”时的更新方法
    void movingToExitUpdate();
    // “正在出场”时的更新函数
    void movingOutUpdate();
public:
    // 设置卡号
    void setCardNum(int num);
    void setHalfParkspaceSum(int sum);
};

