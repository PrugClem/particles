#pragma once

#ifdef _WIN32
 #define GLEW_STATIC
#endif // _WIN32
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <omp.h>

#include <iostream>
#include <cstdlib>
#include <vector>
#include <thread>
#include <chrono>

namespace particle
{
    struct color_t
    {
        float r, b, g, a;
    };
    struct vertex_t
    {
        float s, t;		// texture coordinates
        color_t color;	// color information
        float x, y, z;	// spacial coordinates
    };

    struct position
    {
        float x, y, z;
    };

} // namespace particle

