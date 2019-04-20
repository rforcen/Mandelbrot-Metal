//
//  MandelDisplay.m
//  Mandelbrot
//
//  Created by asd on 20/04/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#import "MandelDisplay.h"
#import "ImageBuffer.h"
#import "Mandelbrot.hpp"
#import "MetalDevice.h"

@implementation MandelDisplay {
    Mandelbrot mandelbrot;
    MetalDevice*dev;
    
    float4 range;
    int iterations;
    float oversample;
}

-(void)awakeFromNib {
    dev = [MetalDevice init];
    
    range={-2.5, -2, 2, 2}; // {-1,-1,1,1}; //
    iterations=256*2; // per pixel
    oversample=2; // generate a 'oversample x oversample' pixel size to gain resolution
}

-(void)generateCPU: (ImageBuffer*)ibuffMetal w:(int)w h:(int)h {
    mandelbrot.setRange(range);
    mandelbrot.setIter(iterations);
    mandelbrot.setSize(w, h);
    mandelbrot.generate();
    
    memcpy(ibuffMetal.imgBuff, mandelbrot.getPict(), w*h*sizeof(uint32));
}

-(void)generateMetal: (ImageBuffer*)imgBuff size:(int)size width:(int)width height:(int)height {
    [dev compileFunc:@"Mandelbrot"];
    
    id<MTLBuffer>picBuff=[dev createBuffer:imgBuff.imgBuff length:size];
    [dev setBufferParam: picBuff index:0]; // shader parameters: picBuff, range(-x-y,x,y), w, h, iters
    [dev setBytesParam:&range        length:sizeof(range)      index:1];
    [dev setBytesParam:&width        length:sizeof(width)      index:2];
    [dev setBytesParam:&height       length:sizeof(height)     index:3];
    [dev setBytesParam:&iterations   length:sizeof(iterations) index:4];
    
    [dev runThreadsWidth:width height:height];               // setup threads & run in a w x h grid
    [dev copyContentsOn:imgBuff.imgBuff  buffer:picBuff];    // copy result
}

- (void)drawRect:(NSRect)rect {
    [super drawRect:rect];
    
    // over sample 'mf'
    uint w=oversample * rect.size.width,
         h=oversample * rect.size.height, size=w*h*sizeof(uint32);
    
    ImageBuffer*ibuffMetal=[ImageBuffer initWithWidth:w Height:h];
    
 
    NSTimeInterval tCPU = [MetalDevice timeIt:^{
//                [self generateCPU:ibuffMetal w:w h:h];
    }];
    NSTimeInterval tGPU = [MetalDevice timeIt:^{
        [self generateMetal:ibuffMetal size:size width:w height:h];
    }];
    
    [[ibuffMetal getimage] drawInRect:rect];
    
    NSLog(@"image size: %dx%d, iterations: %d time CPU: %g, time GPU: %g, ration CPU/GPU: %g",
          w,h, iterations, tCPU, tGPU, tCPU/tGPU);
}

@end
