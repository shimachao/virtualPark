#include "stdafx.h"
#include "Railing.h"



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
