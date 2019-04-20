//
//  InterpolateColor.m
//  radionics
//
//  Created by asd on 26/08/2018.
//  Copyright © 2018 asd. All rights reserved.
//

#import "InterpolateColor.h"

static double interpolate(CGFloat f1, CGFloat f2, CGFloat p) {
    return f1+(f2-f1)*p;
}

@implementation InterpolateColor

+(NSColor *)RGB_c1: (NSColor *)c1 c2:(NSColor *)c2 percent:(double) percent {
    CGFloat r1, g1, b1, a1;
    CGFloat r2, g2, b2, a2;
    
    [c1 getRed:&r1 green:&g1 blue:&b1 alpha:&a1];
    [c2 getRed:&r2 green:&g2 blue:&b2 alpha:&a2];
    
    double r=interpolate(r1, r2, percent),
    g=interpolate(g1, g2, percent),
    b=interpolate(b1, b2, percent);
    
    return [NSColor colorWithRed:r green:g blue:b alpha:1];
}

+(NSColor *)HSL_c1: (NSColor *)c1 c2:(NSColor *)c2 percent:(double) percent {
    
    CGFloat h1, s1, b1, h2, s2, b2, a1,a2;
    
    [c1 getHue:&h1 saturation:&s1 brightness:&b1 alpha:&a1];
    [c2 getHue:&h2 saturation:&s2 brightness:&b2 alpha:&a2];
    
    double h=interpolate(h1, h2, percent),
    s=interpolate(s1, s2, percent),
    v=interpolate(b1, b2, percent);
    
    return [NSColor colorWithHue:h saturation:s brightness:v alpha:1];
}
@end

@implementation NSColor(Hex)
- (int32_t)toABGR { // abgr in image context 0xff0000ff==red; // abgr
    CGFloat red, green, blue, alpha; // system colors [NSColor redColor] -> alpha==0
    [self getRed:&red green:&green blue:&blue alpha:&alpha];
    int32_t
    redInt      = (int32_t)(red     * 255 + 0.5),
    greenInt    = (int32_t)(green   * 255 + 0.5),
    blueInt     = (int32_t)(blue    * 255 + 0.5),
    alphaInt    = (int32_t)(alpha   * 255 + 0.5);
    
    return (alphaInt << 24) | (blueInt << 16) | (greenInt << 8) | redInt; //ABGR in images
    return 0;
}
@end

