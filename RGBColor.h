
/**************************************************************************************************
 Copyright © 2016-2017 Md.Kamruzzaman. All rights reserved.
 The generated code is released under following licenses:
 GNU GENERAL PUBLIC LICENSE, Version 3, 29 June 2007
 --------------------------------------------------------------------------------------------------
 File name: RGBColor.h
 Objective: Class to create RGB color
 Additional information: NA
 --------------------------------------------------------------------------------------------------
 Contributors                   Date            Task details
 -------------------------    ----------      --------------------
 Md. Kamruzzaman              04/16/2016      Initial version
 
 **************************************************************************************************/

#ifndef RGBColor_h
#define RGBColor_h

#include <string>

#define MAX2(A, B) ((A>B)?A:B)
#define MAX3(A, B, C) ((MAX2(A, B) > C)?MAX2(A, B):C)
#define MIN2(A, B) (A>B?B:A)

using namespace std;

class RGB{
private:
    short R,G,B;
public:
    RGB();
    RGB(short R, short G, short B);
    ~RGB();
    
    // Getter
    short getR();
    short getG();
    short getB();
    
    // Setter
    void setR(short R);
    void setG(short G);
    void setB(short B);
};

class RGBColor{
private:
    float minLimit;
    float maxLimit;
    float gamma;
    
    //short const minVisibleWaveLength = 380.0;
    //short const maxVisibleWaveLength = 780.0;
    short const intensityMax = 255;
    
    float GetWaveLength(float value);
    short AdjustColor(float color, float factor);
    string ConvertToRGB(float waveLength);
    string GetHexColor(int value);
    string DecimalToHexConverter(short value);
    bool GetHeatMapColor(float value, float *red, float *green, float *blue);
    bool GetValueBetweenTwoFixedColors(float value, float *red, float *green, float *blue);
    bool GetValuesOfColors(float value, float *red, float *green, float *blue);
    string GetHexValue(short &R, short &G, short &B);
    
public:
    RGBColor();
    RGBColor(float minLimit, float maxLimit, float gamma);
    ~RGBColor();
    void SetRange(float minLimit, float maxLimit);
    void SetGamma(float gamma);
    string GetColor(float value);
    string RGBtoHex(short &R, short &G, short &B);
    void ChangeRGB(short &R, short &G, short &B, short m);
};

#endif /* RGBColor_h */
