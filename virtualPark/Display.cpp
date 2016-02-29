#include "stdafx.h"
#include "Display.h"



Display::Display()
    : m_numOfFreeParkingLots(0)
{
}


Display::~Display()
{
}


Gdiplus::Font* getQuartzRegularFont()
{
    PrivateFontCollection fontCollection;
    fontCollection.AddFontFile(L"res/Quartz Regular.ttf");
    FontFamily* pFontFamily = new FontFamily[1];

    int found = 0;
    fontCollection.GetFamilies(1, pFontFamily, &found);
    WCHAR familyName[LF_FACESIZE + 22];
    pFontFamily[0].GetFamilyName(familyName);

    Gdiplus::Font* pFont = ::new Gdiplus::Font(familyName, 22, FontStyleBold, UnitPixel, &fontCollection);

    return pFont;
}


void Display::draw(Graphics* pGraphics)
{
    // ���Ʊ�����ɫ
    SolidBrush backBrush(Color(60, 60, 60));
    pGraphics->FillRectangle(&backBrush, 0, 0, 110, 110);
    // �������������
    Pen pen(Color(30, 30, 30), 2.0F);
    for (int x = 0; x <= 110; x += 5)
    {
        pGraphics->DrawLine(&pen, x, 0, x, 110);
    }
    for (int y = 0; y <= 110; y += 5)
    {
        pGraphics->DrawLine(&pen, 0, y, 110, y);
    }

    // ��������
    WCHAR string[24] = { '\0' };
    swprintf_s(string, 24, L"��ǰ���ڻ�ʣ%d�����г�λ", m_numOfFreeParkingLots);
    RectF        rectF(0.0f, 0.0f, 110.0f, 110.0f);
    StringFormat stringFormat;
    SolidBrush   solidBrush(Color(255, 0, 0));

    stringFormat.SetAlignment(StringAlignmentCenter);

    stringFormat.SetLineAlignment(StringAlignmentCenter);
    static Gdiplus::Font* pFont = getQuartzRegularFont();
    pGraphics->DrawString(string, -1, pFont, rectF, &stringFormat, &solidBrush);
}


// ��ѯ���г�λ��
int Display::getNumOfFreeParkingLots()
{
    return m_numOfFreeParkingLots;
}


// ���ÿ��г�λ����ֻ��ϵͳ��Ϣ����Ե��ø÷���
void Display::setFreeParkingLotsNum(int n)
{
    m_numOfFreeParkingLots = n;
}
