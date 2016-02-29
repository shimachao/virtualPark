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

//ģ������
class Simulator
{
public:
    Simulator(int e, int n);
    ~Simulator();
    // ����ģ����
    void click();
    // ����
    void draw(Graphics* pGraphics);
    // ��ѯ����������
    int getCarInSum();
    // ��ѯ�볡����
    int getCarEnterSum();
    // ��ѯ��������
    int getCarExitSum();
    // ��ȡƽ��ͣ��ʱ��
    int getAverageParkedTime();
    // ���ɴ��������볡����
    void generateAWrongCarToEnter();
    // ���ɴ���ĳ�������
    void generateAWrongCarToExit();

private:
    // ɾ��FINAL״̬������
    void deleteFinalCar();
    // �鿴��·�Ƿ����
    bool isRoadFree();
    // �������ж����״̬
    void update();
    // �ռ�׼���볡��Ҫ�ϳ���������
    void collectWaitingForRoadCar();
    // ׼��ͣ�����ϵ�һ�����ϳ���
    void allowParkedCarToRoad();
    // ��������ȴ���������ʼ�볡
    void allowWaitCarToEnter();
    // ��������
    void drawRailing(Graphics* pParkGraphics);
    // ���Ƴ�λ
    void drawParkingSpace(Graphics* pParkGraphics);
    // ���ƾ�����
    void drawAlarm(Graphics* pAlarmGraphics);
    // ��������ͼƬ
    void loadCarImg();
    // ����һ���볡������
    void generateACarToEner();

private:
    // ��ʾ��
    Display* m_pDisplay;
    // ��ڶ�����
    CardReader* m_pEntranceCardReader;
    // ���ڶ�����
    CardReader* m_pExitCardReader;
    // �������
    Railing* m_pEntranceRailing;
    // ��������
    Railing* m_pExitRailing;
    // ��ڴ�����
    Sensor* m_pEntranceSensor;
    // ���ڴ�����
    Sensor* m_pExitSensor;
    // ������
    Alarm* m_pAlarm;
    // ��Ϣϵͳ
    InfoSystem* m_pInfoSystem;
    // ˢ����������б�
    list<Car*> m_carList;
    // ���Ŷ���
    deque<int> m_cardNumQue;
    // �ڳ���ȴ�����������
    deque<Car*> m_waitQueue;
    // ��������ڵȴ��������е���������
    deque<Car*> m_waitRoadQue;
    // ���ڳ�����
    int m_carInParkingLotSum;
    // ������������ͣ��ʱ��
    int m_totalParkTime;
    // �볡����
    int m_carEnteredSum;
    // ��������
    int m_carExitedSum;
    // ����ͼƬ����
    Image* m_carImgArray[4][4];
    // ����ͼƬ
    Image* m_pRailingImage;
};

