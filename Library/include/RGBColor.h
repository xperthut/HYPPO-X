
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
#include <math.h>

#define MAX2(A, B) ((A>B)?A:B)
#define MAX3(A, B, C) ((MAX2(A, B) > C)?MAX2(A, B):C)
#define MIN2(A, B) (A>B?B:A)

namespace hyppox {
    class RGB{
        private:
        short R,G,B;
        public:
        RGB()=default;
        RGB(short R, short G, short B);
        ~RGB()=default;
        
        // Getter
        short getR();
        short getG();
        short getB();
        
        // Setter
        void setR(short R);
        void setG(short G);
        void setB(short B);
    };
    
    RGB::RGB(short R, short G, short B){this->R=R;this->G=G;this->B=B;}
    
    short RGB::getR(){return R;}
    short RGB::getG(){return G;}
    short RGB::getB(){return B;}
    
    void RGB::setR(short R){this->R=R;}
    void RGB::setG(short G){this->G=G;}
    void RGB::setB(short B){this->B=B;}
    
    
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
        std::string ConvertToRGB(float waveLength);
        std::string GetHexColor(int value);
        std::string DecimalToHexConverter(short value);
        bool GetHeatMapColor(float value, float *red, float *green, float *blue);
        bool GetValueBetweenTwoFixedColors(float value, float *red, float *green, float *blue);
        bool GetValuesOfColors(float value, float *red, float *green, float *blue);
        std::string GetHexValue(short &R, short &G, short &B);
        
        public:
        RGBColor();
        RGBColor(float minLimit, float maxLimit, float gamma);
        ~RGBColor();
        void SetRange(float minLimit, float maxLimit);
        void SetGamma(float gamma);
        std::string GetColor(float value);
        std::string RGBtoHex(short &R, short &G, short &B);
        void ChangeRGB(short &R, short &G, short &B, short m);
    };
    
    RGBColor::RGBColor(){
        this->minLimit = this->maxLimit = 0;
        this->gamma = 1;
    }
    
    RGBColor::RGBColor(float minLimit, float maxLimit, float gamma){
        this->minLimit = minLimit;
        this->maxLimit = maxLimit;
        this->gamma = gamma;
    }
    
    RGBColor::~RGBColor(){}
    
    std::string RGBColor::GetColor(float value){
        
        std::string color = "";
        
        float waveLength = this->GetWaveLength(value);
        color = this->ConvertToRGB(waveLength);
        
        return color;
    }
    
    void RGBColor::SetRange(float minLimit, float maxLimit){
        this->minLimit = minLimit;
        this->maxLimit = maxLimit;
    }
    
    void RGBColor::SetGamma(float gamma){
        this->gamma = gamma;
    }
    
    std::string RGBColor::RGBtoHex(short &R, short &G, short &B){
        R = MAX2(0, MIN2(R, 255));
        G = MAX2(0, MIN2(G, 255));
        B = MAX2(0, MIN2(B, 255));
        
        return GetHexValue(R, G, B);
    }
    
    void RGBColor::ChangeRGB(short &R, short &G, short &B, short m){
        if(R==255&&G<255){
            G = MIN2(255, G+m);
        }else if(R>0&&G==255){
            R = MAX2(0, R-m);
        }else if(G==255&&B<255){
            B = MIN2(255, B+m);
        }else if(G>0&&B==255){
            G = MAX2(0, G-m);
        }
    }
    
    //#pragma Private_Methods
    float RGBColor::GetWaveLength(float value){
        
        float waveLength = (this->maxLimit-this->minLimit)==0?0:((value-this->minLimit)/(this->maxLimit-this->minLimit));
        
        return waveLength;
    }
    
    short RGBColor::AdjustColor(float color, float factor){
        if(color == 0.0) return 0;
        
        return ((short)(this->intensityMax*pow(color*factor, gamma)))&0xFF;
    }
    
    bool RGBColor::GetHeatMapColor(float value, float *red, float *green, float *blue){
        const int NUM_COLORS = 5;
        static float color[NUM_COLORS][3] = { {0,0,1}, {0,1,1}, {0,1,0}, {1,1,0}, {1,0,0} };
        
        int idx1;        // |-- Our desired color will be between these two indexes in "color".
        int idx2;        // |
        float fractBetween = 0;  // Fraction between "idx1" and "idx2" where our value is.
        
        //if(value <= 0)      {  idx1 = idx2 = 0;            }    // accounts for an input <=0
        //else if(value >= 1)  {  idx1 = idx2 = NUM_COLORS-1; }    // accounts for an input >=0
        //else
        //{
        value = value * (NUM_COLORS-1);        // Will multiply value by 3.
        idx1  = floor(value);                  // Our desired color will be after this index.
        idx2  = idx1+1;                        // ... and before this index (inclusive).
        fractBetween = value - float(idx1);    // Distance between the two indexes (0-1).
        //}
        
        *red   = 255*((color[idx2][0] - color[idx1][0])*fractBetween + color[idx1][0]);
        *green = 255*((color[idx2][1] - color[idx1][1])*fractBetween + color[idx1][1]);
        *blue  = 255*((color[idx2][2] - color[idx1][2])*fractBetween + color[idx1][2]);
        
        return true;
    }
    
    bool RGBColor::GetValueBetweenTwoFixedColors(float value, float *red, float *green, float *blue)
    {
        int aR = 0;   int aG = 0; int aB=255;  // RGB for our 1st color (blue in this case).
        int bR = 255; int bG = 0; int bB=0;    // RGB for our 2nd color (red in this case).
        
        *red   = (float)(bR - aR) * value + aR;      // Evaluated as -255*value + 255.
        *green = (float)(bG - aG) * value + aG;      // Evaluates as 0.
        *blue  = (float)(bB - aB) * value + aB;      // Evaluates as 255*value + 0.
        
        return true;
    }
    
    bool RGBColor::GetValuesOfColors(float value, float *red, float *green, float *blue){
        
        //int cVal = 255*value;
        
        
        
        return true;
    }
    
    std::string RGBColor::ConvertToRGB(float waveLength){
        float R, G, B;
        std::string color = "";
        
        this->GetHeatMapColor(waveLength, &R, &G, &B);
        //this->GetValueBetweenTwoFixedColors(waveLength, &R, &G, &B);
        
        color = "#" + this->GetHexColor(R) + this->GetHexColor(G) + this->GetHexColor(B);
        
        return color;
    }
    
    std::string RGBColor::GetHexColor(int value){
        return (this->DecimalToHexConverter((value&0xf0)>>4) + this->DecimalToHexConverter(value&0x0f));
    }
    
    std::string RGBColor::DecimalToHexConverter(short value){
        if(value < 10) return std::to_string(value);
        
        switch (value) {
            case 10:
            return "a";
            break;
            
            case 11:
            return "b";
            break;
            
            case 12:
            return "c";
            break;
            
            case 13:
            return "d";
            break;
            
            case 14:
            return "e";
            break;
            
            case 15:
            return "f";
            break;
            
            default:
            break;
        }
        
        return "";
    }
    
    std::string RGBColor::GetHexValue(short &R, short &G, short &B){
        
        std::string hexValue = "#" + DecimalToHexConverter((R-R%16)/16) + DecimalToHexConverter(R%16);
        hexValue += DecimalToHexConverter((G-G%16)/16) + DecimalToHexConverter(G%16);
        hexValue += DecimalToHexConverter((B-G%16)/16) + DecimalToHexConverter(B%16);
        
        return hexValue;
    }

}

#endif /* RGBColor_h */
