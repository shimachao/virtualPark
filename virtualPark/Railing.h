#pragma once
#include <functional>
using std::function;

#define DOWN 0      // 放下
#define RAISING 1   // 正在抬起
#define RAISED 2    // 已抬起
#define FALLING 3   //  正在放下

class Railing
{
public:
    Railing();
    ~Railing();
    static void setImg(Image *pImg);

    // 更新栏杆的状态，状态是时间驱动的
    void update();
    // 绘制
    void draw(Graphics* pGraphics);
    // 抬起杆
    void liftUp();
    // 放下杆
    void putDown();
    // 查询栏杆的状态
    int getState();
    // 设置栏杆抬起后调用的对象
    void setUpAction(function<void()> upAction);

private:
    // 栏杆的状态：0为放下状态，1为正在抬起，2为已抬起状态，3为正在放下
    int m_state;
    // 计时器，在栏杆抬起和放下的过程中用来计时
    int m_timer;
    // 栏杆完全抬起后调用的对象
    function<void()> m_upAction;

private:
    static Image *m_pImg;
};

