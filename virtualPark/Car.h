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
�����ļ��ֲ�ͬ״̬���ȴ��볡���ȴ�������ɣ����ڽ��룬����������λ����λ�ƶ��У�
��ͣ�ó����������ʱ�ȴ��г�����ɣ����г����ƶ���������ƶ����ȴ�������ɣ�
���ڳ�������ֹ
*/
enum CarState {INITIALIZATION, WAITING_ENTER_PERMISSION, ENTERING, SERACHING, MOVING_TO_PARKING_SPACE, PARKED, WAITING_ROAD_FREE, MOVING_TO_ROAD, MOVING_TO_EXIT, WAITING_EXIT_PERMISSION,MOVING_OUT, FINAL};

/*
�����ĳ���
UPWARD������
DOWNWRAD������
LEFT������
RIGHT������
*/

enum CarDirection {UPWARD, DOWNWARD, LEFT, RIGHT};

class Car
{
public:
    Car();
    ~Car();
    // ����״̬
    void update();
    // ������ڶ�����
    void setEntranceCardReader(CardReader* pCardReader);
    // ���ó��ڶ�����
    void setExitCardReader(CardReader* pCardReader);
    // ���ó��ڴ�������ָ�����
    void setEntranceSensor(Sensor* pSensor);
    // ���ó��ڴ�������ָ�����
    void setExitSensor(Sensor* pSensor);
    // ���ó�λ��
    void setParkingSpaceNum(int num);
    // �յ���ʼ�볡��Ϣ
    void beginEnter();
    // �յ������볡��Ϣ
    void allowEnter();
    // �յ������ϳ�����Ϣ
    void allowToRoad();
    // �յ�׼�������Ϣ
    void allowExit();
    // �յ��ܾ��볡����
    void rejectEnterRequest();
    // �յ��ܾ���������
    void rejectOutRequest();

    // ��ѯ������ͷ����
    CarDirection getDirection();
    // ��ѯ������λ������
    Point getLocation();
    // ������������״̬
    CarState getState();
    // ��ѯ����
    int getCardNum();
    // ��ѯռ�õĳ�λ��
    int getParkingSpaceNum();
    // ��ȡͣ��ʱ��
    int getParkDuration();

    // ����ͼƬ
    void setImg(Image* (*pImgArray)[4]);
    // ��������    
    void draw(Graphics* pGraphics);
    
private:
    // ����
    int m_cardNum;
    // x������ٶ�
    int m_speedx;
    // y�����ϵ��ٶ�
    int m_speedy;
    // ����λ��
    Point m_location;
    // ��ͷ�ĳ���
    CarDirection m_direction;
    // ������״̬
    CarState m_state;
    // �õ��ĳ�λ���
    int m_parkingSpaceNum;
    // ���ų�λ���������ںͳ�λ����ǰ���㳵λ�ľ�������
    int m_halfParkingSpaceSum;
    // ͣ��ʱ��
    int m_parkDuration;
    // ��ڴ�������
    CardReader* m_pEntranceCardReader;
    // ���ڴ�������
    CardReader* m_pExitCardReader;
    // ��ڴ�����
    Sensor* m_pEntranceSensor;
    // ���ڴ�������
    Sensor* m_pExitSensor;
    // ͼƬ�б��������������ĸ������ͼƬ
    Image* (*m_imgArray)[4];

private:
    // �������볡��ʱ�ĸ��·���
    void enteringUpdate();
    // ��������λ��ʱ�ĸ��·���
    void searchingUpdate();
    // ����λ�ƶ���ʱ�ĸ��·���
    void movingToParkingSpaceUpdate();
    // ����ͣ����״̬�µĸ��·���
    void parkedUpdate();
    // ���򳵵��ƶ���״̬�ĸ��·���
    void movingToRoadUpdate();
    // ��������ƶ���ʱ�ĸ��·���
    void movingToExitUpdate();
    // �����ڳ�����ʱ�ĸ��º���
    void movingOutUpdate();
public:
    // ���ÿ���
    void setCardNum(int num);
    void setHalfParkspaceSum(int sum);
};

