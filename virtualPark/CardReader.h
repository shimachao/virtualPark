#pragma once
#include <functional>
using std::function;

class CardReader
{
public:
    CardReader(function<bool(int)> readAction);
    ~CardReader();
    // ����״̬
    void update();
    // ����������Ϊ����
    void readCard(int num);
    // ���ÿ��ű��ܾ�����õĶ���
    void setRejectedAction(function<void()> action);
    
private:
    // ÿ��ˢ���ķѵ�ʱ��
    unsigned int m_duration;
    // Ҫ��ѯ�Ŀ���
    int m_cardNum;
    // ���ű��ܾ���ִ�еĶ���
    function<void()> m_rejectedAction;
    // ����ʱ�Ķ���
    function<bool(int)> m_readCardAction;
};

