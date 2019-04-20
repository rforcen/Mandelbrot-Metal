//
//  Mandelbrot.hpp
//  Mandelbrot
//
//  Created by asd on 20/04/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#ifndef Mandelbrot_hpp
#define Mandelbrot_hpp

#include "ColorIndex.h"
typedef struct { float x,y,z,w; } float4;

class Mandelbrot {
    int width=0, height=0;
    int iter=150;
    
    float xstart = -1, ystart = -1, xend = 1, yend = 1;
    int *pic=nullptr;
    ColorIndex *colIndex;
    
    
public:
    Mandelbrot() {
        colIndex = new ColorIndex(4096);
    }
    ~Mandelbrot() {
        if(pic) delete[]pic;
    }
  
    void setSize(int size) { this->width=size; this->height=size; }
    void setSize(int w, int h) { this->width=w; this->height=h; }
    void setIter(int iter) { this->iter=iter; }
    void setRange(float x0, float y0, float x1, float y1) {
        xstart=x0; ystart=y0; xend=x1; yend=y1;
    }
    void setRange(float4 range) {
         xstart=range.x; ystart=range.y; xend=range.z; yend=range.w;
    }
    int*getPict() { return pic; }
    
    void generate() {
        float x, y, xstep, ystep;
        float z, zi, newz, newzi;
        float color=0;
        bool inset;
        
        if(pic) delete[]pic;
        pic = new int[height*width];
        
        // these are used for calculating the points corresponding to the pixels
        xstep = (xend - xstart) / width;
        ystep = (yend - ystart) / height;
        
        x = xstart;    y = ystart;
        for (int i = 0; i < height ; i++) {
            for (int j = 0; j < width ; j++) {
                z = zi = 0;
                
                inset = true;
                for (int k = 0; k < iter; k++) {
                    // z^2 = (a+bi)(a+bi) = a^2 + 2abi - b^2
                    newz  = (z * z) - (zi * zi) + x;
                    newzi = 2 * z * zi + y;
                    
                    z = newz;
                    zi = newzi;
                    
                    if (((z * z) + (zi * zi)) > 4) { // sqrt(x*x + y*y)>2
                        inset = false;
                        color = k;
                        break;
                    }
                }
                
                if (inset)     pic[i*width+j] = 0xff000000;
                else           pic[i*width+j] = colIndex->getColorA(20. * color /iter);
                
                x += xstep;
            }
            y += ystep;
            x = xstart;
        }
    }
};
#endif /* Mandelbrot_hpp */
