#include "stdafx.h"
#include "CardReader.h"



CardReader::CardReader(function<bool(int)> action)
    : m_duration(0)
    , m_cardNum(-1)
    , m_readCardAction(action)
    , m_rejectedAction(nullptr)
{
}


CardReader::~CardReader()
{
}



// 更新状态
void CardReader::update()
{
    // 如果有卡待查
    if (m_cardNum != -1)
    {
        // 如果时间已到，就获取读卡结果
        if (m_duration == 0)
        {
            // 如果读卡结果为false，则通知刷卡者
            if (!m_readCardAction(m_cardNum))
            {
                // 通知刷卡者
                TRACE("\n通知刷卡者非法\n");
                m_rejectedAction();
            }
            // 如果读卡结果为肯定，则不用管，让栏杆通知刷卡者
            // 置为-1
            m_cardNum = -1;
        }
        else
        {
            m_duration--;
        }
    }
}


// 读卡，参数为卡号
void CardReader::readCard(int num)
{
    m_cardNum = num;
    m_duration = 1;
}


// 设置卡号被拒绝后调用的对象
void CardReader::setRejectedAction(function<void()> action)
{
    m_rejectedAction = action;
}
