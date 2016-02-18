#include "stdafx.h"
#include "Display.h"



Display::Display()
    : m_numOfFreeParkingLots(0)
{
}


Display::~Display()
{
}


// 查询空闲车位数
int Display::getNumOfFreeParkingLots()
{
    return m_numOfFreeParkingLots;
}


// 设置空闲车位数，只有系统信息类可以调用该方法
void Display::setFreeParkingLotsNum(int n)
{
    m_numOfFreeParkingLots = n;
}
