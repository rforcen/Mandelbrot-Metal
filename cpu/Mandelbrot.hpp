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
#include "Thread.h"
#include <complex>

using std::complex;

typedef struct { float x,y,z,w; } float4;
typedef uint32 color;
typedef uint8 byte;

class Mandelbrot {
    int width=0, height=0;
    int iter=150;
    
    complex<float>zstart={-1,-1}, zend={1,1}, zdiff, zstep;
    int *bmp=nullptr;
    ColorIndex *colIndex;
    
    
public:
    Mandelbrot() {
        colIndex = new ColorIndex(4096);
    }
    ~Mandelbrot() {
        if(bmp) delete[]bmp;
    }
  
    void setSize(int size) { this->width=size; this->height=size; }
    void setSize(int w, int h) { this->width=w; this->height=h; }
    void setIter(int iter) { this->iter=iter; }
    void setRange(float x0, float y0, float x1, float y1) {
        zstart={x0,y0}; zend={x1,y1};
    }
    void setRange(float4 range) {
        zstart={range.x, range.y}; zend={range.z, range.w};
    }
    void recalc() {
        zdiff = zend-zstart;
        zstep = { zdiff.real() / width,  zdiff.imag() / height };
    }
    int*getPict() { return bmp; }
    
    void generate() { // single thread
        complex<float>Z, zinc;
        float C=2;
        
        if(bmp) delete[]bmp;
        bmp = new int[height*width];
        
        recalc();
        zinc = zstart;
        
        for (int i = 0; i < height ; i++) {
            for (int j = 0; j < width ; j++) {
                Z=0;
                
                bool inset = true;
                float color=0;
                
                for (int k = 0; k < iter; k++) {
                    Z = Z*Z + zinc;
                    
                    if(abs(Z) > C) {
                        inset = false;
                        color = k;
                        break;
                    }
                }
                
                if (inset)     bmp[i*width+j] = 0xff000000;
                else           bmp[i*width+j] = colIndex->getColorA(20. * color /iter);
                
                zinc += zstep.real();
            }
            zinc={ zstart.real(), zinc.imag() + zstep.imag() };
        }
    }
    
    inline complex<float>Zformula(complex<float>Z) {
        return Z*Z*Z*Z;
    }
    
    void generateMT() { //  multi thread
        float C=2;
        
        if(bmp) delete[]bmp;
        bmp = new int[height*width];
        
        recalc();
        
        Thread(height).run([this, C](int i) {
            
            complex<float> zinc={ zstart.real(), zstart.imag() + i * zstep.imag() };
            
            for (int j = 0; j < width ; j++, zinc += zstep.real()) {
                complex<float>Z=0;
                bool inset = true;
                float color=0;
                
                for (int k = 0; k < iter; k++) {
                    Z = Zformula(Z) + zinc;
                    
                    if(abs(Z) > C) {
                        inset = false;
                        color = k;
                        break;
                    }
                }
                bmp[ i * width + j ] = (inset) ? 0xff000000 : colIndex->getColorA(20. * color /iter);
            }
            
        });
    }
    
    
    void resample(color *a, color *b, int oldw, int oldh, int neww,  int newh) {
        
        for (int i = 0; i < newh; i++) {
            for (int j = 0; j < neww; j++) {
                
                float tmp = (float) (i) / (float) (newh - 1) * (oldh - 1);
                
                int l = (int) floor(tmp);
                if (l < 0) l = 0;
                else
                if (l >= oldh - 1)
                l = oldh - 2;
                
                float u = tmp - l;
                tmp = (float) (j) / (float) (neww - 1) * (oldw - 1);
                
                int c = (int) floor(tmp);
                if (c < 0) c = 0;
                else
                if (c >= oldw - 1)
                c = oldw - 2;
                float t = tmp - c;
                
                // coefficients
                float d1 = (1 - t) * (1 - u),
                d2 = t * (1 - u),
                d3 = t * u,
                d4 = (1 - t) * u;
                
                // nearby pixels: a[i][j]
                color p1 = a[(l * oldw) + c],
                p2 = a[(l * oldw) + c + 1],
                p3 = a[((l + 1)* oldw) + c + 1],
                p4 = a[((l + 1)* oldw) + c];
                
                // color components
                byte blue = (byte)p1 * d1 + (byte)p2 * d2 + (byte)p3 * d3 + (byte)p4 * d4,
                green = (byte)(p1 >> 8) * d1 + (byte)(p2 >> 8) * d2 + (byte)(p3 >> 8) * d3 + (byte)(p4 >> 8) * d4,
                red = (byte)(p1 >> 16) * d1 + (byte)(p2 >> 16) * d2 + (byte)(p3 >> 16) * d3 + (byte)(p4 >> 16) * d4;
                
                // new pixel R G B
                b[ (i * neww) + j] = (red << 16) | (green << 8) | (blue);
            }
        }
    }
};
#endif /* Mandelbrot_hpp */
