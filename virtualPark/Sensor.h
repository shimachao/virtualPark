#pragma once
#include <functional>
using std::function;

class Car;
class Sensor
{
public:
    Sensor(function<void(Car*)> action);
    ~Sensor();
    // ��һ������ͨ���˶�Ӧ������
    void aCarPassed(Car*);

private:
    // �����������aCarPassedʱִ�еĿɵ��ö���
    function<void(Car*)> m_actionAfterCarPassed;
};

