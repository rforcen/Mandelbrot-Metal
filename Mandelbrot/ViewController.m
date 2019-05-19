//
//  ViewController.m
//  Mandelbrot
//
//  Created by asd on 20/04/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#import "ViewController.h"

@implementation ViewController  {
    NSPoint p1, p2;
    BOOL hasDragged;
}

- (IBAction)onFormula:(id)sender {
     [_mandelDisplay compileNDisp:[_formula stringValue]];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    hasDragged=NO;
}

- (void)mouseDown:(NSEvent *)theEvent {
    p1=[theEvent locationInWindow];
    hasDragged=NO;
}

- (void)mouseDragged:(NSEvent *)theEvent {
    hasDragged=YES;
}

- (void)mouseUp:(NSEvent *)theEvent {
    if(hasDragged) {
        p2=[theEvent locationInWindow];
        NSRect rect=CGRectMake(p1.x, p1.y,  fabs(p2.x-p1.x), fabs(p2.y-p1.y));
        [_mandelDisplay setRect:rect];
    } else
        [_mandelDisplay resetView];
}
@end
