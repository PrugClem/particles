#pragma once

#include "particles.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GLshader/Shader.h"

#include <atomic>
#include <cmath>

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286
#endif

namespace particle
{
    class application
    {
    public:
        struct configuration // configuration structure, will be call-by-reference
        {
            std::uint32_t max_particles;

            float cam_sensitivity;
            float cam_speed;
            float cam_fov;
        };
        struct camera // struct to store camera stuff
        {
            double x, y, z;
            double yaw, pitch;
        };
        struct static_obj // "struct" to store a static obj, i.e the fountain
        {
            std::vector<vertex_t> vertices;
            std::uint32_t vbo;
            std::uint32_t vao;
            void generate_from_vertices(); // generate vertex buffer object and vertex attribute object from vertices
            ~static_obj()
            {
                glDeleteBuffers(1, &this->vbo);
                glDeleteVertexArrays(1, &this->vao);
            }
        };
    private:
        GLFWwindow* window;
        configuration *config;
        camera cam;
        gl::Shader shader;
        static_obj fountain;

        std::chrono::system_clock::time_point t_start, t_cur_frame, t_last_frame;
        std::chrono::system_clock::duration dt_frame;

        size_t frame_count{0};

        // private functions to make the code more readable
        void render_frame();
        void update_particles();
    public:
        application() = default;
        ~application();
        bool init(configuration &config);
        void run();
    };
} // namespace particle
