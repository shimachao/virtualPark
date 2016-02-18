#include "stdafx.h"
#include "Car.h"
#include <random>

using std::default_random_engine;
using std::uniform_int_distribution;

Car::Car()
    : m_cardNum(0)
    , m_speedx(0)
    , m_speedy(0)
    , m_location(Point(-3,3))
    , m_direction(RIGHT)
    , m_state(INITIALIZATION)
    , m_parkingSpaceNum(-1)
    , m_halfParkingSpaceSum(0)
    , m_parkDuration(0)
    , m_pEntranceCardReader(NULL)
    , m_pExitCardReader(NULL)
    , m_pEntranceSensor(NULL)
{
    // �������ͣ��ʱ��,5~300�룬�����������
    static default_random_engine e;
    static uniform_int_distribution<unsigned> u(5, 300);
    m_parkDuration = u(e);
}


Car::~Car()
{
}


void Car::update()
{
    switch (m_state)
    {
    case ENTERING:
        enteringUpdate();
        break;
    case SERACHING:
        searchingUpdate();
        break;
    case MOVING_TO_PARKING_SPACE:
        movingToParkingSpaceUpdate();
        break;
    case PARKED:
        parkedUpdate();
        break;
    case MOVING_TO_ROAD:
        movingToRoadUpdate();
        break;
    case MOVING_TO_EXIT:
        movingToExitUpdate();
        break;
    case MOVING_OUT:
        movingOutUpdate();
        break;
    default:
        break;
    }
}


// ���ó��ڴ�������ָ�����
void Car::setEntranceSensor(Sensor* pSensor)
{
    m_pEntranceSensor = pSensor;
}


// ���ó��ڴ�������ָ�����
void Car::setExitSensor(Sensor* pSensor)
{
    m_pExitSensor = pSensor;
}


// �յ���ʼ�볡��Ϣ
void Car::beginEnter()
{
    TRACE("\n beginEnter() \n");
    if (m_state == INITIALIZATION)
    {
        // ˢ����������һ״̬
        m_pEntranceCardReader->readCard(m_cardNum);
        m_state = WAITING_ENTER_PERMISSION;
    }
}


// �����볡��Ϣ
void Car::allowEnter()
{
    TRACE("\n allowEnter() \n");
    if (m_state == WAITING_ENTER_PERMISSION)
    {
        m_speedx = 1;
        m_state = ENTERING;
    }
}


// �����ϳ�����Ϣ
void Car::allowToRoad()
{
    TRACE("\n allowToRoad() \n");
    // �ڡ��ȴ�������Ȩ�ޡ�״̬ʱ�յ���׼������������Ϣ����
    // �޸��ٶȣ����롰�򳵵���ʻ��״̬
    if (m_state == WAITING_ROAD_FREE)
    {
        if (m_direction == UPWARD)
        {
            m_speedy = -1;
        }
        else if(m_direction == DOWNWARD)
        {
            m_speedy = 1;
        }
        m_state = MOVING_TO_ROAD;
    }
}


// ׼��������
void Car::allowExit()
{
    TRACE("\n allowExit() \n");
    // ����ڡ��ȴ�����Ȩ�ޡ�ʱ�յ�����������������Ϣ�����޸��ٶȲ����롰���ڳ����С�״̬
    if (m_state == WAITING_EXIT_PERMISSION)
    {
        m_speedy = 1;
        m_state = MOVING_OUT;
    }
}


// �ܾ��볡�������Ϣ
void Car::rejectEnterRequest()
{
    TRACE("\n rejectEnterRequest() \n");
    if (m_state == WAITING_ENTER_PERMISSION)
    {
        m_state = FINAL;
    }
}


// �յ��ܾ������������Ϣ
void Car::rejectOutRequest()
{
    TRACE("\n rejectOutRequest() \n");
    // ����������󱻾ܾ�����ԭ·���أ����롰������λ��״̬
    if (m_state == WAITING_EXIT_PERMISSION)
    {
        // �ٶȷ���
        m_speedx = -1;
        // ��������
        m_direction = LEFT;
        m_location.x -= 1;
        // ת�롰������λ״̬��
        m_state = SERACHING;
    }
}


// �������볡��ʱ�ĸ��·���
void Car::enteringUpdate()
{
    // ��������
    m_location.x += m_speedx;
    // ����Ƿ�ͨ��������
    if (m_location.x - 0 >= 2)
    {
        // ���ͨ���ˣ��ͽ�����һ״̬
        m_state = SERACHING;
        // ֪ͨ������
        m_pEntranceSensor->aCarPassed(this);
    }
}


// ��������λ��ʱ�ĸ��·���
void Car::searchingUpdate()
{
    // ��������
    m_location.x += m_speedx;
    // ����Ƿ񵽴ﳵλ��ǰ
    if (m_location.x >= m_parkingSpaceNum % m_halfParkingSpaceSum + 2)
    {
        // ����ѵ�����޸��ٶȺͳ��򲢽��롰���복λ��״̬
        m_speedx = 0;
        if (m_parkingSpaceNum < m_halfParkingSpaceSum)
        {
            m_speedy = -1;
            m_direction = UPWARD;
        }
        else
        {
            m_speedy = 1;
            m_direction = DOWNWARD;
        }

        m_state = MOVING_TO_PARKING_SPACE;
    }
}


// ����λ�ƶ���ʱ�ĸ��·���
void Car::movingToParkingSpaceUpdate()
{
    // ��������
    m_location.y += m_speedy;
    // ����Ƿ��Ѿ����ﳵλ
    if (m_direction == UPWARD && m_location.y == 0)
    {
        // �޸��ٶȺͳ��򣬽�����һ״̬
        m_speedy = 0;
        m_direction = DOWNWARD;
        m_state = PARKED;
    }
    else if (m_direction == DOWNWARD && m_location.y == 5)
    {
        m_speedy = 0;
        m_direction = UPWARD;
        m_state = PARKED;
    }
}


// ����ͣ����״̬�µĸ��·���
void Car::parkedUpdate()
{
    // �ݼ�ͣ��ʱ��
    m_parkDuration--;
    // ���ʱ�䵽�˾���ϵͳ�����ϳ����뿪
    // todo
}


// ���򳵵��ƶ���״̬�ĸ��·���
void Car::movingToRoadUpdate()
{
    // ��������
    m_location.y += m_speedy;
    // ����Ƿ񵽴ﳵ���������˾ͽ��롰������ƶ���״̬
    if ((m_direction == DOWNWARD && m_location.y == 3)||
        (m_direction == UPWARD && m_location.y == 2))
    {
        m_direction = RIGHT;
        m_speedy = 0;
        m_speedx = 1;
    }
}


// ��������ƶ���ʱ�ĸ��·���
void Car::movingToExitUpdate()
{
    // ��������
    m_location.x += m_speedx;
    // �ж��Ƿ񵽴���ڸ�ǰ
    if (m_location.x == m_halfParkingSpaceSum+1)
    {
        // ��������˳��ڸ�ǰ���޸��ٶȣ�ˢ����������һ״̬
        m_speedx = 0;
        m_pExitCardReader->readCard(m_cardNum);
        m_state = WAITING_EXIT_PERMISSION;
    }
}


// �����ڳ�����ʱ�ĸ��º���
void Car::movingOutUpdate()
{
    // ��������
    m_location.x += m_speedx;
    // �ж��Ƿ��Ѿ�ͨ����������
    if (m_location.x == m_halfParkingSpaceSum + 3)
    {
        // ����Ѿ�ͨ������֪ͨ����������������һ״̬
        m_pExitSensor->aCarPassed(this);
        m_state = FINAL;
    }
}


// ��ѯ������λ������
Point Car::getLocation()
{
    return m_location;
}


// ��ѯ������ͷ����
CarDirection Car::getDirection()
{
    return m_direction;
}


// ��ѯ����
int Car::getCardNum()
{
    return m_cardNum;
}



// ��ѯռ�õĳ�λ��
int Car::getParkingSpaceNum()
{
    return m_parkingSpaceNum;
}


// ������������״̬
CarState Car::getState()
{
    return m_state;
}


// ������ڶ�����
void Car::setEntranceCardReader(CardReader* pCardReader)
{
    m_pEntranceCardReader = pCardReader;
}


// ���ó��ڶ�����
void Car::setExitCardReader(CardReader* pCardReader)
{
    m_pExitCardReader = pCardReader;
}


// ��ȡͣ��ʱ��
int Car::getParkDuration()
{
    return m_parkDuration;
}



// ���ó�λ��
void Car::setParkingSpaceNum(int num)
{
    m_parkingSpaceNum = num;
}


// ��������ͼƬ
void Car::setImg(Image* (*pImgArray)[4])
{
    m_imgArray = pImgArray;
}


// ��������
void Car::draw(Graphics* pGraphics)
{
    // ��������
    RectF rcDest;
    Image *pImg = nullptr;
    
    switch (m_direction)
    {
    case UPWARD:
        rcDest.X = m_location.x * 50;
        rcDest.Width = 50;
        rcDest.Y = (m_location.y - 1) * 50;
        rcDest.Height = 100;
        pImg = (*m_imgArray)[0];
        break;
    case DOWNWARD:
        rcDest.X = m_location.x * 50;
        rcDest.Width = 50;
        rcDest.Y = m_location.y * 50;
        rcDest.Height = 100;
        pImg = (*m_imgArray)[1];
        break;
    case LEFT:
        rcDest.X = (m_location.x - 1) * 50;
        rcDest.Width = 100;
        rcDest.Y = m_location.y * 50;
        rcDest.Height = 50;
        pImg = (*m_imgArray)[2];
        break;
    case RIGHT:
        rcDest.X = m_location.x * 50;
        rcDest.Width = 100;
        rcDest.Y = m_location.y * 50;
        rcDest.Height = 50;
        pImg = (*m_imgArray)[3];
        break;
    }
    // ����ͼ��
    pGraphics->DrawImage(pImg, rcDest);

    // ���Ʊ��
    WCHAR string[2] = { '\0' };
    swprintf_s(string, 2, L"%d", m_cardNum);
    FontFamily   fontFamily(L"Arial");
    Gdiplus::Font         font(&fontFamily, 16, FontStyleBold, UnitPoint);
    StringFormat stringFormat;
    SolidBrush   solidBrush(Color(0, 0, 0));
    stringFormat.SetAlignment(StringAlignmentCenter);
    stringFormat.SetLineAlignment(StringAlignmentCenter);

    pGraphics->DrawString(string, -1, &font, rcDest, &stringFormat, &solidBrush);
}


// ���ÿ���
void Car::setCardNum(int num)
{
    m_cardNum = num;
}


void Car::setHalfParkspaceSum(int sum)
{
    m_halfParkingSpaceSum = sum;
}