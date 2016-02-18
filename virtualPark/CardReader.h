#pragma once
#include <functional>
using std::function;

class CardReader
{
public:
    CardReader(function<bool(int)> readAction);
    ~CardReader();
    // 更新状态
    void update();
    // 读卡，参数为卡号
    void readCard(int num);
    // 设置卡号被拒绝后调用的对象
    void setRejectedAction(function<void()> action);
    
private:
    // 每次刷卡耗费的时间
    unsigned int m_duration;
    // 要查询的卡号
    int m_cardNum;
    // 卡号被拒绝后执行的对象
    function<void()> m_rejectedAction;
    // 读卡时的动作
    function<bool(int)> m_readCardAction;
};

