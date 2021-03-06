#include "stdafx.h"
#include "Railing.h"


Image* Railing::s_pImg = nullptr;


Railing::Railing()
    : m_state(DOWN)
    , m_timer(0)
    , m_upAction(nullptr)
{
}


Railing::~Railing()
{
}


// 抬起杆
void Railing::liftUp()
{
    // 判断栏杆现在的状态，只有处于放下状态时才能接受抬起命令
    if (m_state == DOWN)
    {
        // 将上升时间设为2秒钟
        m_timer = 2;
        // 状态改为正在抬起
        m_state = RAISING;
    }
}


// 放下杆
void Railing::putDown()
{
    // 只有处于已抬起状态时才能接受放下命令
    if (m_state == RAISED)
    {
        // 将下降时间设为2秒钟
        m_timer = 2;
        // 状态改为正在放下
        m_state = FALLING;
    }
}


// 更新栏杆的状态，状态是时间驱动的
void Railing::update()
{
    if (m_state == FALLING)
    {
        // 如果栏杆正在下降
        m_timer--;
        if (m_timer <= 0)
        {
            // 过了2秒，证明已经完全放下
            m_state = DOWN;
        }
    }
    else if (m_state == RAISING)
    {
        // 如果栏杆正在抬起
        m_timer--;
        if (m_timer <= 0)
        {
            // 过了2秒，证明已经完全抬起
            m_state = RAISED;
            // 通知观察者
            m_upAction();
        }
    }
}


// 绘制
void Railing::draw(Graphics* pGraphics)
{
    if (m_state == DOWN)
    {
        pGraphics->DrawImage(s_pImg, 0, 0, 9, 100);
    }
    else if (m_state == RAISING ||
             m_state == FALLING)
    {
        Rect destRect(0, 0, 9, 50);
        pGraphics->DrawImage(s_pImg, destRect, 0, 0, 9, 100, Gdiplus::UnitPixel);
    }
    else
    {
        SolidBrush backBrush(Color(60, 60, 60));
        pGraphics->FillEllipse(&backBrush, 0, 0, 10, 10);
    }
}


// 查询栏杆的状态
int Railing::getState()
{
    return m_state;
}


// 设置栏杆抬起后调用的对象
void Railing::setUpAction(function<void()> upAction)
{
    m_upAction = upAction;
}



void Railing::setImg(Image *pImg)
{
    s_pImg = pImg;
}