#pragma once

#include "particles.hpp"

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
            std::size_t max_particles;
            bool particle_correct_alpha;

            std::size_t fountain_sweep_steps;
            double fountain_height;
            double fountain_width;

            std::chrono::system_clock::duration particle_ttl;
            float particle_gravity;
            float particle_bounce_friction;

            float cam_sensitivity;
            float cam_speed;
            float cam_fov;
        };
        struct camera // struct to store camera stuff
        {
            glm::vec3 pos;
            double yaw, pitch;
        };
        struct static_obj // "struct" to store a static obj, i.e the fountain
        {
            using index_t = GLuint;
        private:
            GLuint vbo{0}; // vertex buffer object, stores the vertices
            GLuint vao{0}; // vertex array object
            GLuint ebo{0}; // element buffer object, stores the vertex indices
            GLuint tex{0}; // object texture, stores the texture to map onto the triangles
            std::size_t num_idx{0};
        public:
            std::vector<vertex_t> vertices; // strore the vertices for generating
            std::vector<index_t> indices; // store the indices for the triangles
            bool read_from_file(const std::string &filename);
            bool generate(const char *texture_file); // generate vertex buffer object and vertex attribute object from vertices
            void draw(); // draw vertex buffer object
            ~static_obj()
            {
                glDeleteBuffers(1, &this->vbo);
                glDeleteBuffers(1, &this->ebo);
                glDeleteVertexArrays(1, &this->vao);
                if(this->tex != 0) glDeleteTextures(1, &this->tex); // dont delete default texture
            }
        };
    private:
        GLFWwindow* window;
        configuration *config;
        camera cam;
        gl::Shader object_shader;
        static_obj fountain, plane, bricks;
        particle_pool particles;
        inline static application *app;

        std::chrono::system_clock::time_point t_start, t_cur_frame, t_last_frame;
        std::chrono::system_clock::duration dt_frame;

        size_t frame_count{0};

        // private functions to make the code more readable
        void render_frame();
        bool setup_structures();
        bool fountain_setup(const char* texture_filename);
        void fountain_generate_normals_last_ring();
    public:
        application() = default;
        ~application();
        bool init(configuration &config);
        void run();

        friend class particle;
        friend class particle_pool;
    };
} // namespace particle
