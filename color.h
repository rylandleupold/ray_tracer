#ifndef COLOR_H
#define COLOR_H

#include "utilities.h"
#include "interval.h"
#include "vec3.h"
#include <vector>
#include <fstream>
#include <iostream>

using color = vec3;

inline double linear_to_gamma(double linear_component) {
    // Using gamma-2, meaning 2 is the exponent. For linear to gamma transformation 
    // we need the inverse, so 1/gamma, or exponent 1/2, which is just sqrt
    if (linear_component > 0) {
        return sqrt(linear_component);
    }
    return 0;
}


void write_color(std::ofstream& file, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Apply linear to gamma transform
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Translate the [0,1] component values to the byte range [0,255].
    static const interval intensity = interval(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // Write out the pixel color components.
    file << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif