#include "stdafx.h"
#include "Alarm.h"



Alarm::Alarm():m_state(OFF)
, m_duration(0)
{
}


Alarm::~Alarm()
{
}


// 打开警报器
void Alarm::open()
{
    m_state = ON;
    m_duration = 3; // 警报持续3秒
}


// 查询状态
AlarmState Alarm::getState()
{
    return m_state;
}


// 更新状态
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
