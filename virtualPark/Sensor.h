#pragma once
#include <functional>
using std::function;

class Car;
class Sensor
{
public:
    Sensor(function<void(Car*)> action);
    ~Sensor();
    // 有一辆汽车通过了对应的栏杆
    void aCarPassed(Car*);

private:
    // 用来保存调用aCarPassed时执行的可调用对象
    function<void(Car*)> m_actionAfterCarPassed;
};

