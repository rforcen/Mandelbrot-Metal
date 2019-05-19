//
//  ViewController.h
//  Mandelbrot
//
//  Created by asd on 20/04/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MandelDisplay.h"

@interface ViewController : NSViewController

@property (strong) IBOutlet MandelDisplay *mandelDisplay;
@property (weak) IBOutlet NSTextField *formula;

@end

