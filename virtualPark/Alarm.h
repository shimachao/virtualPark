#pragma once

enum AlarmState {ON, OFF};

class Alarm
{
public:
    Alarm();
    ~Alarm();
    // �򿪾�����
    void open();
    // ��ѯ״̬
    AlarmState getState();
    // ����״̬
    void update();
    // ����
    void draw(Graphics* pGraphics);

private:
    // ��������״̬
    AlarmState m_state;
    // ����ʱ��
    unsigned int m_duration;
};

