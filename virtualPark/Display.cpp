#include "stdafx.h"
#include "Display.h"



Display::Display()
    : m_numOfFreeParkingLots(0)
{
}


Display::~Display()
{
}


// ��ѯ���г�λ��
int Display::getNumOfFreeParkingLots()
{
    return m_numOfFreeParkingLots;
}


// ���ÿ��г�λ����ֻ��ϵͳ��Ϣ����Ե��ø÷���
void Display::setFreeParkingLotsNum(int n)
{
    m_numOfFreeParkingLots = n;
}
