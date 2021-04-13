#pragma once

#ifdef _WIN32
 #define GLEW_STATIC
#endif // _WIN32
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <omp.h>

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <string>
#include <cmath>

#include "GLshader/Shader.h"

#ifdef _WIN32
#include "lib/stb/stb_image.h"
#else
#include "stb/stb_image.h"
#endif


namespace particle
{    struct vertex_t
    {
        glm::vec2 tex_coord;    // texture coordinates
        glm::vec4 color;	    // color information
        glm::vec3 pos;	        // spacial coordinates
    };

    class particle
    {
    private:
        vertex_t vertices[4];
    };
} // namespace particle
