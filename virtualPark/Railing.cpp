#include "stdafx.h"
#include "Railing.h"


Image* Railing::m_pImg = nullptr;


Railing::Railing()
    : m_state(DOWN)
    , m_timer(0)
    , m_upAction(nullptr)
{
}


Railing::~Railing()
{
}


// ̧���
void Railing::liftUp()
{
    // �ж��������ڵ�״̬��ֻ�д��ڷ���״̬ʱ���ܽ���̧������
    if (m_state == DOWN)
    {
        // ������ʱ����Ϊ2����
        m_timer = 2;
        // ״̬��Ϊ����̧��
        m_state = RAISING;
    }
}


// ���¸�
void Railing::putDown()
{
    // ֻ�д�����̧��״̬ʱ���ܽ��ܷ�������
    if (m_state == RAISED)
    {
        // ���½�ʱ����Ϊ2����
        m_timer = 2;
        // ״̬��Ϊ���ڷ���
        m_state = FALLING;
    }
}


// �������˵�״̬��״̬��ʱ��������
void Railing::update()
{
    if (m_state == FALLING)
    {
        // ������������½�
        m_timer--;
        if (m_timer <= 0)
        {
            // ����2�룬֤���Ѿ���ȫ����
            m_state = DOWN;
        }
    }
    else if (m_state == RAISING)
    {
        // �����������̧��
        m_timer--;
        if (m_timer <= 0)
        {
            // ����2�룬֤���Ѿ���ȫ̧��
            m_state = RAISED;
            // ֪ͨ�۲���
            m_upAction();
        }
    }
}


// ����
void Railing::draw(Graphics* pGraphics)
{
    if (m_state == DOWN)
    {
        pGraphics->DrawImage(m_pImg, 0, 0, 9, 100);
    }
    else if (m_state == RAISING ||
             m_state == FALLING)
    {
        Rect destRect(0, 0, 9, 50);
        pGraphics->DrawImage(m_pImg, destRect, 0, 0, 9, 100, Gdiplus::UnitPixel);
    }
    else
    {
        HatchBrush b(HatchStyleHorizontal, Color(255, 255, 0), Color(0, 0, 0));
        pGraphics->FillEllipse(&b, 0, 100, 10, 10);
    }
}


// ��ѯ���˵�״̬
int Railing::getState()
{
    return m_state;
}


// ��������̧�����õĶ���
void Railing::setUpAction(function<void()> upAction)
{
    m_upAction = upAction;
}



void Railing::setImg(Image *pImg)
{
    m_pImg = pImg;
}