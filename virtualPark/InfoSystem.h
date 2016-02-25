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
    // Ա����
    int m_employeeSum;
    // ���ų�λ��
    int m_halfParkingSum;
    // ���г�λ��
    int m_freeParkingSum;
    // �������г�λ��״̬
    char* m_parkingLotStateArray;
    // ��ʾ��
    Display* m_pDisplay;
    // ������
    Alarm* m_pAlarm;
    // �������
    Railing* m_pEntranceRailing;
    // ��������
    Railing* m_pExitRailing;
    // ��ǰ���ڿ��ż���
    set<int> m_cardInCollection;

public:
    // ��ȡ���������Ŀ��г�λ���
    int getEntranceNearestParkingLot();
    // ��ȡ���������Ŀ��г�λ���
    int getExitNearestParkingLot();
    // �黹��λ
    void giveBackParkingLot(int i);
    // �����ض���ŵĳ�λ������ó�λ��ռ�þͷ���-1�����򷵻ر��
    int requestSpecialParkingLot(int i);
    // ��ȡ���ų�λ����
    int getHalfParkingLotSum();
    // ��ѯ�����Ƿ�Ϸ�
//    bool isValid(int cardNum);
    // һ�������Ѿ�ͨ�����ˣ�ʻ����ͣ����
    void aCarEntered();
    // һ������ͨ�����ˣ�����ͣ����
    void aCarExit();
    // ��������ʱˢ��
    bool readCardToEnter(int num);
    // ����ʱˢ��
    bool readCardToExit(int num);
    // ��ѯԱ����
    int getEmployeeSum();
};

