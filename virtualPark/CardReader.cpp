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



// ����״̬
void CardReader::update()
{
    // ����п�����
    if (m_cardNum != -1)
    {
        // ���ʱ���ѵ����ͻ�ȡ�������
        if (m_duration == 0)
        {
            // ����������Ϊfalse����֪ͨˢ����
            if (!m_readCardAction(m_cardNum))
            {
                // ֪ͨˢ����
                TRACE("\n֪ͨˢ���߷Ƿ�\n");
                m_rejectedAction();
            }
            // ����������Ϊ�϶������ùܣ�������֪ͨˢ����
            // ��Ϊ-1
            m_cardNum = -1;
        }
        else
        {
            m_duration--;
        }
    }
}


// ����������Ϊ����
void CardReader::readCard(int num)
{
    m_cardNum = num;
    m_duration = 1;
}


// ���ÿ��ű��ܾ�����õĶ���
void CardReader::setRejectedAction(function<void()> action)
{
    m_rejectedAction = action;
}
