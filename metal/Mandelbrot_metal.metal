//
//  Mandelbrot.metal
//  Mandelbrot
//
//  Created by asd on 20/04/2019.
//  Copyright © 2019 voicesync. All rights reserved.
//

#include <metal_stdlib>
using namespace metal;

#include "Mandelbrot_metal.hpp"

kernel void Mandelbrot( device color*pixels[[buffer(0)]], //
                       const device range &rng[[buffer(1)]],    // range
                       const device uint &width[[buffer(2)]],   // width
                       const device uint &height[[buffer(3)]],  // height
                       const device uint &iters[[buffer(4)]],  // iters

                       uint2 position [[thread_position_in_grid]] )
{
    class Mandelbrot mb;
    
    mb.setRange(rng);
    mb.setSize(width, height);
    mb.setIter(iters);
    
    pixels[position.x + position.y * width] = mb.generate(position.x, position.y);
}
