#pragma once
#include <functional>
using std::function;

#define DOWN 0      // ����
#define RAISING 1   // ����̧��
#define RAISED 2    // ��̧��
#define FALLING 3   //  ���ڷ���

class Railing
{
public:
    Railing();
    ~Railing();
    static void setImg(Image *pImg);

    // �������˵�״̬��״̬��ʱ��������
    void update();
    // ����
    void draw(Graphics* pGraphics);
    // ̧���
    void liftUp();
    // ���¸�
    void putDown();
    // ��ѯ���˵�״̬
    int getState();
    // ��������̧�����õĶ���
    void setUpAction(function<void()> upAction);

private:
    // ���˵�״̬��0Ϊ����״̬��1Ϊ����̧��2Ϊ��̧��״̬��3Ϊ���ڷ���
    int m_state;
    // ��ʱ����������̧��ͷ��µĹ�����������ʱ
    int m_timer;
    // ������ȫ̧�����õĶ���
    function<void()> m_upAction;

private:
    static Image *m_pImg;
};

