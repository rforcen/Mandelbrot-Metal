//
//  Mandelbrot.hpp
//  Mandelbrot
//
//  Created by asd on 20/04/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#ifndef Mandelbrotmetal_hpp
#define Mandelbrotmetal_hpp

#include "ColorScale_metal.h"

typedef uint32_t color; // aa bb gg rr  32 bit color
typedef uint8_t byte;
typedef float4 range;

constant const int clRed=0xff, clBlue=0xff0000, maxColors=4096;

class Mandelbrot {
    int width=0, height=0;
    int iter=150;
    
    float xstart = -1, ystart = -1, xend = 1, yend = 1;
    
public:
    void setSize(int w, int h) { this->width=w; this->height=h; }
    void setIter(int iter) { this->iter=iter; }
    void setRange(float x0, float y0, float x1, float y1) {
        xstart=x0; ystart=y0; xend=x1; yend=y1;
    }
    void setRange(float4 range) {
        xstart=range.x; ystart=range.y; xend=range.z; yend=range.w;
    }
    //    int*getPict() { return pic; }
    
    color generate(int i, int j) {
        float col=0;
        
        // these are used for calculating the points corresponding to the pixels
        float xstep = (xend - xstart) / width,
        ystep = (yend - ystart) / height;
        
        float x = xstart + xstep*i,
        y = ystart + ystep*j;
        
        float z = 0, zi = 0;
        
        bool inset = true;
        for (int k = 0; k < iter; k++) {
            // z^2 = (a+bi)(a+bi) = a^2 + 2abi - b^2
            float newz  = (z * z) - (zi * zi) + x,
            newzi = 2 * z * zi + y;
            
            z = newz;
            zi = newzi;
            
            if (((z * z) + (zi * zi)) > 4) { // sqrt(x*x + y*y)>2
                inset = false;
                col = k;
                break;
            }
        }
        
        return 0xff000000 | ((inset) ? 0 : ColorScaleHSL(clBlue, clRed, 60.*col/iter) );
    }
};
#endif /* Mandelbrot_hpp */
