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
