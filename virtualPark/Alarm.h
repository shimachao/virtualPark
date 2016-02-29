#pragma once

enum AlarmState {ON, OFF};

class Alarm
{
public:
    Alarm();
    ~Alarm();
    // 打开警报器
    void open();
    // 查询状态
    AlarmState getState();
    // 更新状态
    void update();
    // 绘制
    void draw(Graphics* pGraphics);

private:
    // 警报器的状态
    AlarmState m_state;
    // 报警时长
    unsigned int m_duration;
};

