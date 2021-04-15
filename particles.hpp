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
{
    struct vertex_t
    {
        glm::vec2 tex_coord;    // texture coordinates
        glm::vec4 color;	    // color information
        glm::vec3 pos;	        // spacial coordinates
        glm::vec3 normal;       // normal vector for the vertex
    };
    struct p_vertex_t
    {
        glm::vec3 pos;
        glm::vec4 color;
        float width;
    };

    class particle
    {
    public:
    private:
        p_vertex_t _vertex;
        glm::vec3 vel;
        bool alive;
        std::chrono::system_clock::time_point ttd;
        
    public:
        static gl::Shader particle_shader;

        particle();
        particle(const glm::vec3& start_pos, const glm::vec3& start_vel, const glm::vec4& color, float width);
        virtual ~particle();

        void spawn(const glm::vec3& start_pos, const glm::vec3& start_vel, const glm::vec4& color, float width);
        void update_physics(std::chrono::system_clock::time_point now, float dt);

        const p_vertex_t& vertex();
    };

    class particle_pool
    {
    private:
        GLuint shader_prog;
        std::vector<particle> particles;
        GLuint vbo{0}; // vertex buffer object, stores the vertices
        GLuint vao{0}; // vertex array object
        GLuint tex{0}; // object texture, stores the texture to map onto the particles

        constexpr static const char* VERTEX_PATH =   "assets/shaders/particle.vert";
        constexpr static const char* GEOMETRY_PATH = "assets/shaders/particle.geom";
        constexpr static const char* FRAGMENT_PATH = "assets/shaders/particle.frag";
    
        bool load_shaders();

    public:
        virtual ~particle_pool(void);

        bool init(const std::string &texture_filename);

        bool load_texture(const char *texture_filename);

        void draw(glm::mat4 view, glm::mat4 projection);

        //const std::vector<particle>& get_particles(void);
    };
} // namespace particle
