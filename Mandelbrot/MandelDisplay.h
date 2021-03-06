//
//  MandelDisplay.h
//  Mandelbrot
//
//  Created by asd on 20/04/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface MandelDisplay : NSView
-(void)setPoint: (NSPoint) point;
-(void)setRect: (NSRect) rect;
-(void)resetView;
-(void)compileNDisp: (NSString*)formula;
@end

NS_ASSUME_NONNULL_END
