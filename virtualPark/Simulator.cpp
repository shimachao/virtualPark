#include "stdafx.h"
#include "Simulator.h"
#include <random>
#include <algorithm>
using std::random_shuffle;
using std::sort;
using std::default_random_engine;
using std::bernoulli_distribution;

using std::bernoulli_distribution;
using std::default_random_engine;

Simulator::Simulator(int e, int n)
    : m_pDisplay(nullptr)
    , m_pEntranceCardReader(nullptr)
    , m_pExitCardReader(nullptr)
    , m_pEntranceRailing(nullptr)
    , m_pEntranceSensor(nullptr)
    , m_pExitSensor(nullptr)
    , m_pAlarm(nullptr)
    , m_pInfoSystem(nullptr)
    , m_carInParkingLotSum(0)
    , m_totalParkTime(0)
    , m_carEnteredSum(0)
    , m_carExitedSum(0)
{
    // ������ʾ��
    m_pDisplay = new Display();
    // ����������
    m_pAlarm = new Alarm();
    // ������������
    m_pEntranceRailing = new Railing();
    m_pExitRailing = new Railing();
    // ������Ϣϵͳ
    m_pInfoSystem = new InfoSystem(
        e,
        n,
        m_pDisplay,
        m_pAlarm,
        m_pEntranceRailing, // �������
        m_pExitRailing);    // ��������
    // ��������������
    InfoSystem* p = this->m_pInfoSystem;

    m_pEntranceCardReader = new CardReader(
        [p](int num)->bool {return p->readCardToEnter(num); });

    m_pExitCardReader = new CardReader(
        [p](int num)->bool {return p->readCardToExit(num); });

    // ��������������
    m_pEntranceSensor = new Sensor([p, this](Car* pCar) {
        p->aCarEntered(); // ֪ͨϵͳ
        // ��ȡ��λ��
        pCar->setParkingSpaceNum(p->getEntranceNearestParkingLot());
        this->m_carInParkingLotSum++; // ���³��ڳ�����
        this->m_carEnteredSum++; // �����볡����
        // ������ͣ��ʱ��
        this->m_totalParkTime += pCar->getParkDuration(); });

    m_pExitSensor = new Sensor([p, this](Car* pCar) {
        p->aCarExit(); // ֪ͨϵͳ
        p->giveBackParkingLot(pCar->getParkingSpaceNum()); // �黹��λ
        this->m_carInParkingLotSum--; // ���³��ڳ�����
        // ���³�������
        this->m_carExitedSum++; });

    // ����Ա���������ɿ��ţ�����Ϊ[0, e-1]�����ǿ���Ҫ��ɢ
    int *a = new int[e];
    for (size_t i = 0; i < e; i++)
    {
        a[i] = i;
    }
    // ��ɢ
    random_shuffle(a, a + e);
    // ���뿨�Ŷ���
    for (size_t i = 0; i < e; i++)
    {
        m_cardNumQue.push_back(a[i]);
    }
    delete a;

    // ��������ͼƬ
    m_pRailingImage = Image::FromFile(L"img/railing.png");

    // ��������ͼƬ
    loadCarImg();
}


Simulator::~Simulator()
{
    if (m_pAlarm)
    {
        delete m_pAlarm;
        m_pAlarm = nullptr;
    }

    if (m_pDisplay)
    {
        delete m_pDisplay;
        m_pDisplay = nullptr;
    }

    if (m_pEntranceRailing)
    {
        delete m_pEntranceRailing;
        m_pEntranceRailing = nullptr;
    }

    if (m_pExitRailing)
    {
        delete m_pExitRailing;
        m_pExitRailing = nullptr;
    }

    if (m_pEntranceCardReader)
    {
        delete m_pEntranceCardReader;
        m_pEntranceCardReader = nullptr;
    }

    if (m_pExitCardReader)
    {
        delete m_pExitCardReader;
        m_pExitCardReader;
    }

    if (m_pEntranceSensor)
    {
        delete m_pEntranceSensor;
        m_pEntranceSensor = nullptr;
    }

    if (m_pExitSensor)
    {
        delete m_pExitSensor;
        m_pExitSensor = nullptr;
    }

    if (m_pInfoSystem)
    {
        delete m_pInfoSystem;
        m_pInfoSystem = nullptr;
    }

    for each (Car* pCar in m_carList)
    {
        delete pCar;
    }

    for each ( Car* pCar in m_waitQueue)
    {
        delete pCar;
    }

    for each (Car* pCar in m_waitRoadQue)
    {
        delete pCar;
    }
}


// ����ģ����
void Simulator::click()
{
    // �������ж����״̬
    update();

    // ɾ��FINAL״̬������
    deleteFinalCar();

    // �ռ�����WAIT_RAOD_FREE��������m_waitRaodQue������
    collectWaitingForRoadCar();

    // ����������У�����������У��ȴ���Ҫ�볡������
    if (isRoadFree())
    {
        allowParkedCarToRoad();
    }

    // �����·���ǿ��еľʹ����ſڵȴ��볡������
    if (isRoadFree())
    {
        allowWaitCarToEnter();
    }

    // ��������볡������
    static default_random_engine e;
    static bernoulli_distribution b(0.1); // 0.1�Ŀ��������볡������
    if (!m_cardNumQue.empty() && b(e))
    {
        generateACarToEner();
    }
   /* static bool f = true;
    if (f)
    {
        generateACarToEner();
        f = false;
    }*/
}


// ɾ��FINAL״̬������
void Simulator::deleteFinalCar()
{
    // �Ȳ���û�г�����FINIAL״̬���о�ɾ��
    Car *pCar = nullptr;
    for each (Car* p in m_carList)
    {
        if (p->getState() == FINAL)
        {
            pCar = p;
            break;
        }
    }

    if (pCar)
    {
        m_cardNumQue.push_back(pCar->getCardNum());
        m_carList.remove(pCar);
        delete pCar;
        pCar = nullptr;
    }
}


// �鿴��·�Ƿ����
bool Simulator::isRoadFree()
{
    bool isRoadFree = true;
    for each (Car* pCar in m_carList)
    {
        if (!(pCar->getState() == PARKED ||
            pCar->getState() == WAITING_ROAD_FREE))
        {
            isRoadFree = false;
            break;
        }
    }
    return isRoadFree;
}


// �������ж����״̬
void Simulator::update()
{
    for each (Car* pCar in m_carList)
    {
        pCar->update();
    }
    // ���¶�������״̬
    m_pEntranceCardReader->update();
    m_pExitCardReader->update();
    // �������˵�״̬
    m_pEntranceRailing->update();
    m_pExitRailing->update();
    // ���¾�����״̬
    m_pAlarm->update();
}


// �ռ�׼���볡��Ҫ�ϳ���������
void Simulator::collectWaitingForRoadCar()
{
    list<Car*> waitForRoad;
    for each (Car* p in m_carList)
    {
        if (p->getState() == WAITING_ROAD_FREE)
        {
            waitForRoad.push_back(p);
        }
    }
    // �����ռ���������m_carList��ɾ��
    for each (Car* p in waitForRoad)
    {
        m_carList.remove(p);
    }
    // ��waitForRoad���ݳ�λ���ź�������m_waitQueue
    // ��Ҫ�볡����������λ����ڽ�����ǰ��
    int n = m_pInfoSystem->getHalfParkingLotSum();
    waitForRoad.sort([n](Car*&p1, Car*&p2) {return
        (p1->getParkingSpaceNum() % n) > (p2->getParkingSpaceNum() % n); });
 /*   int n = m_pInfoSystem->getHalfParkingLotSum();
    sort(waitForRoad.begin(), waitForRoad.end(),
        [n](Car*&p1, Car*&p2) {return
        (p1->getParkingSpaceNum() % n) > (p2->getParkingSpaceNum() % n); });*/

    for each (Car* p in waitForRoad)
    {
        m_waitRoadQue.push_back(p);
    }
    waitForRoad.clear();
}


// ׼��ͣ�����ϵ�һ�����ϳ���
void Simulator::allowParkedCarToRoad()
{
    if (!m_waitRoadQue.empty())
    {
        Car* pCar = m_waitRoadQue.front();
        // ֪ͨ����������·��
        pCar->allowToRoad();
        // ���������ĳ��ڶ������ʹ�������Ա����
        pCar->setExitCardReader(m_pExitCardReader);
        pCar->setExitSensor(m_pExitSensor);
        // ע�������Ϊ�������Ĺ۲��ߣ��������ܾ���������Ϣ�ɶ�����֪ͨ
        m_pExitCardReader->setRejectedAction([pCar]() {
            pCar->rejectOutRequest(); });
        // ע�������Ϊ�������˵Ĺ۲��ߣ������������ʱ������֪ͨ����
        m_pExitRailing->setUpAction([pCar]() {pCar->allowExit(); });
        // ��m_waitRoadQue�б��ƶ���m_carList
        m_waitRoadQue.pop_front();
        m_carList.push_back(pCar);
    }
}


// ��������ȴ���������ʼ�볡
void Simulator::allowWaitCarToEnter()
{
    if (!m_waitQueue.empty())
    {
        Car* pCar = m_waitQueue.front();
        // ������������ڶ���������
        pCar->setEntranceCardReader(m_pEntranceCardReader);
        // ������������ڴ���������
        pCar->setEntranceSensor(m_pEntranceSensor);
        // ע������Ϊ��ڶ������Ĺ۲��ߣ����ܾ��볡����Ϣ�ɶ�������֪
        m_pEntranceCardReader->setRejectedAction([pCar]() {
            pCar->rejectEnterRequest(); });
        // ע������Ϊ������˵Ĺ۲��ߣ�׼���볡����Ϣ������֪ͨ
        m_pEntranceRailing->setUpAction([pCar]() {pCar->allowEnter(); });

        // ֪ͨ�����볡
        pCar->beginEnter();

        // ��waitQueue�ƶ���m_carList
        m_waitQueue.pop_front();
        m_carList.push_front(pCar);
    }
}


// ��ѯ���ڳ�����
int Simulator::getCarInParkingLotSum()
{
    return m_carInParkingLotSum;
}


// ����
void Simulator::draw(Graphics* pGraphics)
{
    // ����GDI+������λ�ƶ���
    Matrix transform;

    // ������ʾ��
    transform.Translate(20, 135);
    pGraphics->SetTransform(&transform);
    drawDisplay(pGraphics);
    transform.Reset();

    // ���ƾ�����
    transform.Translate(620, 160);
    pGraphics->SetTransform(&transform);
    drawAlarm(pGraphics);
    transform.Reset();

    // ���Ƴ�λ�����ˡ�����������ʹ��ͬһ������
    transform.Translate(0, 250);
    pGraphics->SetTransform(&transform);
    // ���Ƴ�λ
    drawParkingSpace(pGraphics);
    // ��������
    drawRailing(pGraphics);
    // ��������
    for each (Car* pCar in m_carList)
    {
        pCar->draw(pGraphics);
    }
    for each (Car* pCar in m_waitRoadQue)
    {
        pCar->draw(pGraphics);
    }

    transform.Reset();
    pGraphics->SetTransform(&transform);
}


void Simulator::drawRailing(Graphics* pParkGraphics)
{
    int halfParkingSpaceSum = m_pInfoSystem->getHalfParkingLotSum();
    // �����������
    int railingState = m_pEntranceRailing->getState();
    if (railingState == DOWN)
    {
        pParkGraphics->DrawImage(m_pRailingImage, 0, 100, 9, 100);
    }
    else if (railingState == RAISING ||
        railingState == FALLING)
    {
        Rect destRect(0, 100, 9, 50);
        pParkGraphics->DrawImage(m_pRailingImage, destRect, 0, 0, 9, 100, Gdiplus::UnitPixel);
    }
    else
    {
        HatchBrush b(HatchStyleHorizontal, Color(255, 255, 0), Color(0, 0, 0));
        pParkGraphics->FillEllipse(&b, 0, 100, 10, 10);
    }
    // ���Ƴ�������
    railingState = m_pExitRailing->getState();
    if (railingState == DOWN)
    {
        pParkGraphics->DrawImage(m_pRailingImage, (halfParkingSpaceSum + 4) * 50 , 100, 9, 100);
    }
    else if (railingState == RAISING ||
        railingState == FALLING)
    {
        Rect destRect((halfParkingSpaceSum + 4) * 50, 100, 9, 50);
        pParkGraphics->DrawImage(m_pRailingImage, destRect, 0, 0, 9, 100, Gdiplus::UnitPixel);
    }
    else
    {
        HatchBrush b(HatchStyleHorizontal, Color(255, 255, 0), Color(0, 0, 0));
        pParkGraphics->FillEllipse(&b, (halfParkingSpaceSum + 4) * 50, 100, 10, 10);
    }
}


// ���Ƴ�λ
void Simulator::drawParkingSpace(Graphics* pParkGraphics)
{
    Pen pen(Color(0, 0, 0));
    // ���Ʊ߿�
    int halfParkingSpaceSum = m_pInfoSystem->getHalfParkingLotSum();
    pParkGraphics->DrawRectangle(&pen, 0, 0, (halfParkingSpaceSum + 4) * 50, 300);
    // ���Ƴ�λ
    for (size_t i = 2; i <= halfParkingSpaceSum + 2; i++)
    {
        pParkGraphics->DrawLine(&pen, i * 50, 0, i * 50, 100);
        pParkGraphics->DrawLine(&pen, i * 50, 200, i * 50, 300);
    }
    pParkGraphics->DrawLine(&pen, 100, 100, (halfParkingSpaceSum + 2) * 50, 100);
    pParkGraphics->DrawLine(&pen, 100, 200, (halfParkingSpaceSum + 2) * 50, 200);
}

Gdiplus::Font* getQuartzRegularFont()
{
    PrivateFontCollection fontCollection;
    fontCollection.AddFontFile(L"res/Quartz Regular.ttf");
    FontFamily* pFontFamily = new FontFamily[1];

    int found = 0;
    fontCollection.GetFamilies(1, pFontFamily, &found);
    WCHAR familyName[LF_FACESIZE + 22];
    pFontFamily[0].GetFamilyName(familyName);

    TRACE(familyName);
    TRACE("\n%d\n",pFontFamily[0].IsStyleAvailable(FontStyleRegular));
    TRACE("\n%d\n", pFontFamily[0].IsStyleAvailable(FontStyleBold));
    Gdiplus::Font* pFont = ::new Gdiplus::Font(familyName, 18, FontStyleBold, UnitPixel, &fontCollection);

    return pFont;
}

// ������ʾ��
void Simulator::drawDisplay(Graphics* pGraphics)
{
    // ���Ʊ���
    SolidBrush backBrush(Color(20, 20, 20));
    pGraphics->FillRectangle(&backBrush, 0, 0, 110, 110);
    
    Pen pen(Color(100,100,100));
    for (int x = 0; x <= 110; x += 5)
    {
        pGraphics->DrawLine(&pen, x, 0, x, 110);
    }
    for (int y = 0; y <= 110; y += 5)
    {
        pGraphics->DrawLine(&pen, 0, y, 110, y);
    }

    // ��������
    WCHAR string[24] = { '\0' };
    swprintf_s(string, 24, L"��ǰ���ڻ�ʣ%d�����г�λ", m_pDisplay->getNumOfFreeParkingLots());
    FontFamily   fontFamily(L"Arial");
    RectF        rectF(0.0f, 10.0f, 110.0f, 110.0f);
    StringFormat stringFormat;
    SolidBrush   solidBrush(Color(255, 0, 0));

    stringFormat.SetAlignment(StringAlignmentCenter);

    stringFormat.SetLineAlignment(StringAlignmentCenter);
    static Gdiplus::Font* pFont = getQuartzRegularFont();
    pGraphics->DrawString(string, -1, pFont, rectF, &stringFormat, &solidBrush);
}


// ���ƾ�����
void Simulator::drawAlarm(Graphics* pGraphics)
{
    // ���Ʊ߿�
    Pen pen(Color(0, 0, 0));
    pGraphics->DrawRectangle(&pen, 0, 0, 60, 60);
    AlarmState state = m_pAlarm->getState();
    if (state == OFF)
    {
        SolidBrush brush(Color(200, 200, 200));
        pGraphics->FillEllipse(&brush, 1, 1, 58, 58);
    }
    else
    {
        SolidBrush brush(Color(255, 0, 0));
        pGraphics->FillEllipse(&brush, 1, 1, 58, 58);
    }
}


// ��������ͼƬ
void Simulator::loadCarImg()
{
    m_carImgArray[0][0] = Image::FromFile(L"img/1up.png");
    m_carImgArray[0][1] = Image::FromFile(L"img/1down.png");
    m_carImgArray[0][2] = Image::FromFile(L"img/1left.png");
    m_carImgArray[0][3] = Image::FromFile(L"img/1right.png");

    m_carImgArray[1][0] = Image::FromFile(L"img/2up.png");
    m_carImgArray[1][1] = Image::FromFile(L"img/2down.png");
    m_carImgArray[1][2] = Image::FromFile(L"img/2left.png");
    m_carImgArray[1][3] = Image::FromFile(L"img/2right.png");

    m_carImgArray[2][0] = Image::FromFile(L"img/3up.png");
    m_carImgArray[2][1] = Image::FromFile(L"img/3down.png");
    m_carImgArray[2][2] = Image::FromFile(L"img/3left.png");
    m_carImgArray[2][3] = Image::FromFile(L"img/3right.png");

    m_carImgArray[3][0] = Image::FromFile(L"img/4up.png");
    m_carImgArray[3][1] = Image::FromFile(L"img/4down.png");
    m_carImgArray[3][2] = Image::FromFile(L"img/4left.png");
    m_carImgArray[3][3] = Image::FromFile(L"img/4right.png");
}


// ����һ���볡������
void Simulator::generateACarToEner()
{
    TRACE("\n����һ���볡����\n");
    // �����볡������
    Car* pCar = new Car();
    // ���ÿ���
    pCar->setCardNum(m_cardNumQue.front());
    m_cardNumQue.pop_front();
    // ��֪�䳵��һ�ų�λ������
    int sum = m_pInfoSystem->getHalfParkingLotSum();
    pCar->setHalfParkspaceSum(sum);
    // ����������ͼƬ
    int index = (pCar->getCardNum()) % 4;
    pCar->setImg(&m_carImgArray[index]);
    // ����ȴ�����
    m_waitQueue.push_back(pCar);
}


// ��ѯ����������
int Simulator::getCarInSum()
{
    return m_carInParkingLotSum;
}


// ��ѯ�볡����
int Simulator::getCarEnterSum()
{
    return m_carEnteredSum;
}


// ��ѯ��������
int Simulator::getCarExitSum()
{
    return m_carExitedSum;
}


// ��ȡƽ��ͣ��ʱ��
int Simulator::getAverageParkedTime()
{
    if (m_carEnteredSum != 0)
    {
        return m_totalParkTime / m_carEnteredSum;
    }
    else
    {
        return 0;
    }
}
