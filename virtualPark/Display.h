#pragma once
class Display
{
public:
    Display();
    ~Display();
    // 查询空闲车位数
    int getNumOfFreeParkingLots();
    // 设置空闲车位数，只有系统信息类可以调用该方法
    void setFreeParkingLotsNum(int n);

private:
    // 空闲车位数
    int m_numOfFreeParkingLots;
};

