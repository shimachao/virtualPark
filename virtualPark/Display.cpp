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
    // 绘制背景颜色
    SolidBrush backBrush(Color(60, 60, 60));
    pGraphics->FillRectangle(&backBrush, 0, 0, 110, 110);
    // 绘制上面的线条
    Pen pen(Color(30, 30, 30), 2.0F);
    for (int x = 0; x <= 110; x += 5)
    {
        pGraphics->DrawLine(&pen, x, 0, x, 110);
    }
    for (int y = 0; y <= 110; y += 5)
    {
        pGraphics->DrawLine(&pen, 0, y, 110, y);
    }

    // 绘制文字
    WCHAR string[24] = { '\0' };
    swprintf_s(string, 24, L"当前场内还剩%d个空闲车位", m_numOfFreeParkingLots);
    RectF        rectF(0.0f, 0.0f, 110.0f, 110.0f);
    StringFormat stringFormat;
    SolidBrush   solidBrush(Color(255, 0, 0));

    stringFormat.SetAlignment(StringAlignmentCenter);

    stringFormat.SetLineAlignment(StringAlignmentCenter);
    static Gdiplus::Font* pFont = getQuartzRegularFont();
    pGraphics->DrawString(string, -1, pFont, rectF, &stringFormat, &solidBrush);
}


// 查询空闲车位数
int Display::getNumOfFreeParkingLots()
{
    return m_numOfFreeParkingLots;
}


// 设置空闲车位数，只有系统信息类可以调用该方法
void Display::setFreeParkingLotsNum(int n)
{
    m_numOfFreeParkingLots = n;
}
