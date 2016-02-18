#include "stdafx.h"
#include "Sensor.h"



Sensor::Sensor(function<void(Car*)> action)
    :m_actionAfterCarPassed(action)
{
}


Sensor::~Sensor()
{
}



// 有一辆汽车通过了对应的栏杆
void Sensor::aCarPassed(Car* pCar)
{
    m_actionAfterCarPassed(pCar);
}
