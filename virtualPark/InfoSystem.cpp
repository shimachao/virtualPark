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
    // ������ʾ��
    m_pDisplay->setFreeParkingLotsNum(m_freeParkingSum);
    // ��ʼ״̬����
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


// ��ȡ���������Ŀ��г�λ���
int InfoSystem::getEntranceNearestParkingLot()
{
    // �ӿ�����ڵĳ�λ��ʼ��Ѱ��һ����λ
    int r = -1; // ��¼�õ��Ŀ��г�λ��
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
    // ����ҵ���
    if (r != -1)
    {
        m_parkingLotStateArray[r] = USED;
        m_freeParkingSum--;
        m_pDisplay->setFreeParkingLotsNum(m_freeParkingSum);
    }
    
    return r;
}


// ��ȡ���������Ŀ��г�λ���
int InfoSystem::getExitNearestParkingLot()
{
    // �ӿ������ڵĳ�λ��ʼѰ�ҿ��г�λ
    int r = -1; // ��¼�ҵ��Ŀ��г�λ��
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
    // ����ҵ��ˣ����ñ�ǣ�������ʾ���ϵ�����
    if (r != -1)
    {
        m_parkingLotStateArray[r] = USED;
        m_freeParkingSum--;
        m_pDisplay->setFreeParkingLotsNum(m_freeParkingSum);
    }
    
    return r;
}


// �黹��λ
void InfoSystem::giveBackParkingLot(int i)
{
    // ���±�ǣ�������ʾ���ϵ�����
    m_parkingLotStateArray[i] = FREE;
    m_freeParkingSum++;
    m_pDisplay->setFreeParkingLotsNum(m_freeParkingSum);
}


// �����ض���ŵĳ�λ������ó�λ��ռ�þͷ���-1�����򷵻ر��
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


// ��ȡ���ų�λ����
int InfoSystem::getHalfParkingLotSum()
{
    return m_halfParkingSum;
}


// ��ѯ�����Ƿ�Ϸ�
bool InfoSystem::isValid(int cardNum)
{
    
    return cardNum>=0 && cardNum<=m_employeeSum &&
        m_cardInCollection.find(cardNum) != m_cardInCollection.end();
}


// һ�������Ѿ�ͨ�����ˣ�ʻ����ͣ����
void InfoSystem::aCarEntered()
{
    m_pEntranceRailing->putDown();
}


// һ������ͨ�����ˣ�����ͣ����
void InfoSystem::aCarExit()
{
    m_pExitRailing->putDown();
}


// ��������ʱˢ��
bool InfoSystem::readCardToEnter(int num)
{
    // ������źϷ����Ҳ��ڵ�ǰ���ڿ��ż����У��򷵻�true
    if ((num >= 0 && num < m_employeeSum) &&
        m_cardInCollection.find(num) == m_cardInCollection.end())
    {
        TRACE("\n��������Ϸ�\n");
        m_cardInCollection.insert(num);
        // ��������̧��
        m_pEntranceRailing->liftUp();
        return true;
    }
    else
    {
        TRACE("����������Ϸ�");
        return false;
    }
}


// ����ʱˢ��
bool InfoSystem::readCardToExit(int num)
{
    // ������źϷ������ڵ�ǰ���ڿ��ż����У��򷵻�true
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
