//
//  InterpolateColor.h
//  radionics
//
//  Created by asd on 26/08/2018.
//  Copyright © 2018 asd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

@interface InterpolateColor : NSObject
+(NSColor *)RGB_c1: (NSColor *)c1 c2:(NSColor *)c2 percent:(double) percent;
+(NSColor *)HSL_c1: (NSColor *)c1 c2:(NSColor *)c2 percent:(double) percent;
@end


@interface NSColor(Hex)
-(int32_t)toABGR;
@end
