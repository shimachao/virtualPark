#pragma once
class Display
{
public:
    Display();
    ~Display();
    // ��ѯ���г�λ��
    int getNumOfFreeParkingLots();
    // ���ÿ��г�λ����ֻ��ϵͳ��Ϣ����Ե��ø÷���
    void setFreeParkingLotsNum(int n);

private:
    // ���г�λ��
    int m_numOfFreeParkingLots;
};

