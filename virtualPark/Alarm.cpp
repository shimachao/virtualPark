#include "stdafx.h"
#include "Alarm.h"



Alarm::Alarm():m_state(OFF)
, m_duration(0)
{
}


Alarm::~Alarm()
{
}


// �򿪾�����
void Alarm::open()
{
    m_state = ON;
    m_duration = 3; // ��������3��
}


// ��ѯ״̬
AlarmState Alarm::getState()
{
    return m_state;
}


// ����״̬
void Alarm::update()
{
    if (m_state == ON)
    {
        if (m_duration == 0)
        {
            m_state = OFF;
        }
        else
        {
            m_duration--;
        }
    }
}


// ����
void Alarm::draw(Graphics* pGraphics)
{
    // ���Ʊ߿�
    Pen pen(Color(0, 0, 0));
    pGraphics->DrawRectangle(&pen, 0, 0, 60, 60);
    if (m_state == OFF)
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
