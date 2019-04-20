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
    int iters;
}

-(void)awakeFromNib {
    dev = [MetalDevice init];
    
    range={-2.5, -2, 2, 2}; // {-1,-1,1,1}; //
    iters=2048;
}

-(void)generateCPU: (ImageBuffer*)ibuffMetal w:(int)w h:(int)h {
    mandelbrot.setRange(range);
    mandelbrot.setIter(iters);
    mandelbrot.setSize(w, h);
    mandelbrot.generate();
    
    memcpy(ibuffMetal.imgBuff, mandelbrot.getPict(), w*h*sizeof(uint32));
}

-(void)generateMetal: (ImageBuffer*)imgBuff size:(int)size width:(int)width height:(int)height {
    
    [dev compileFunc:@"Mandelbrot"];
    
    id<MTLBuffer>picBuff=[dev createBuffer:imgBuff.imgBuff length:size];
    [dev setBufferParam: picBuff index:0]; // shader parameters:
    [dev setBytesParam:&range        length:sizeof(range)     index:1];
    [dev setBytesParam:&width        length:sizeof(width)     index:2];
    [dev setBytesParam:&height       length:sizeof(height)    index:3];
    [dev setBytesParam:&iters        length:sizeof(iters)     index:4];
    
    [dev runThreadsWidth:width height:height];                // setup threads & run in a w x h grid
    [dev copyContentsOn:imgBuff.imgBuff  buffer:picBuff];    // get result
}

- (void)drawRect:(NSRect)rect {
    [super drawRect:rect];
    
    uint w=rect.size.width, h=rect.size.height, size=w*h*sizeof(uint32);
    
    ImageBuffer*ibuffMetal=[ImageBuffer initWithWidth:w Height:h];
    
 
    NSTimeInterval tCPU = [MetalDevice timeIt:^{
//                [self generateCPU:ibuffMetal w:w h:h];
    }];
    NSTimeInterval tGPU = [MetalDevice timeIt:^{
        [self generateMetal:ibuffMetal size:size width:w height:h];
    }];
    
    [[ibuffMetal getimage] drawInRect:rect];
    
    NSLog(@"image size: %dx%d, iterations: %d time CPU: %g, time GPU: %g, ration CPU/GPU: %g",
          w,h, iters, tCPU, tGPU, tCPU/tGPU);
}

@end
