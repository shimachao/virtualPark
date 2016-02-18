#include "stdafx.h"
#include "Sensor.h"



Sensor::Sensor(function<void(Car*)> action)
    :m_actionAfterCarPassed(action)
{
}


Sensor::~Sensor()
{
}



// ��һ������ͨ���˶�Ӧ������
void Sensor::aCarPassed(Car* pCar)
{
    m_actionAfterCarPassed(pCar);
}
