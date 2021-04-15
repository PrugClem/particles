#include "particles.hpp"
#include "app.hpp"

particle::particle_pool::~particle_pool(void)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->vao);
    glDeleteTextures(1, &this->tex);
}

bool particle::particle_pool::load_texture(const char *texture_filename)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(texture_filename, &width, &height, &nrChannels, 4); // load
    if(data == nullptr)
    {
        std::cerr << "[static_obj / generate] texture " << texture_filename << " not loaded" << std::endl;
        return false;
    }

    glGenTextures(1, &this->tex);
    glBindTexture(GL_TEXTURE_2D, this->tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    return true;
}

bool particle::particle_pool::init(const std::string &texture_filename)
{
    if(!this->load_shaders())
    {
        std::cerr << "[particle_pool / load_shaders] Error loading shaders" << std::endl;
        return false;
    }

    this->particles.resize(application::app->config->max_particles);

    this->load_texture(texture_filename.c_str());

    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    //glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(this->particles.size()*sizeof( p_vertex_t )), nullptr, GL_STREAM_DRAW);
    glBufferStorage(GL_ARRAY_BUFFER, GLsizeiptr(this->particles.size() * sizeof( p_vertex_t )), &this->particles.at(0).vertex(), GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(p_vertex_t), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(p_vertex_t), (const void*)(sizeof(glm::vec3)) );
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, false, sizeof(p_vertex_t), (const void*)(sizeof(glm::vec3) + sizeof(glm::vec4)) );

    this->data = (p_vertex_t*)glMapBufferRange(GL_ARRAY_BUFFER, 0, GLsizeiptr(this->particles.size()*sizeof( p_vertex_t )), GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
    if (this->data == nullptr)
    {
        std::cerr << "[particle_pool / init] failed to map buffer range" << std::endl;
        //std::cerr << "[particle_pool / init] buffer length: " << GLsizeiptr(this->particles.size() * sizeof(p_vertex_t)) << std::endl;
        return false;
    }

    for(size_t i = 0; i < this->particles.size(); i++)
        memcpy(this->data + i, &this->particles[i].vertex(), sizeof(p_vertex_t));

    glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, GLsizeiptr(this->particles.size()*sizeof( p_vertex_t )));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    

    return true;
}

bool particle::particle_pool::load_shaders()
{
    std::fstream vertex_file, geometry_file, fragment_file;
    std::string vertex_code, geometry_code, fragment_code;
    GLuint vertex_shader, geometry_shader, fragment_shader;
    char shader_log[1024];

    std::clog << "[GL ERROR / particle_pool / load_shaders] before shaders loading code: " << glGetError() << std::endl;

    vertex_file.open(VERTEX_PATH, std::ios::in);
    if(!vertex_file.is_open())
    {
        std::cerr << "[particle_pool / load_shaders] unable to load vertex shader program " << VERTEX_PATH << std::endl;
        return false;
    }
    geometry_file.open(GEOMETRY_PATH, std::ios::in);
    if(!geometry_file.is_open())
    {
        std::cerr << "[particle_pool / load_shaders] unable to load geometry shader program " << VERTEX_PATH << std::endl;
        return false;
    }
    fragment_file.open(FRAGMENT_PATH, std::ios::in);
    if(!fragment_file.is_open())
    {
        std::cerr << "[particle_pool / load_shaders] unable to load fragment shader program " << VERTEX_PATH << std::endl;
        return false;
    }

    while(!vertex_file.eof())
    {
        char buf[1];
        vertex_file.read(buf, sizeof(buf));
        if(!vertex_file.eof())
            vertex_code.insert(vertex_code.end(), std::begin(buf), std::end(buf));
    }
    while(!geometry_file.eof())
    {
        char buf[1];
        geometry_file.read(buf, sizeof(buf));
        if(!geometry_file.eof())
            geometry_code.insert(geometry_code.end(), std::begin(buf), std::end(buf));
    }
    while(!fragment_file.eof())
    {
        char buf[1];
        fragment_file.read(buf, sizeof(buf));
        if(!fragment_file.eof())
            fragment_code.insert(fragment_code.end(), std::begin(buf), std::end(buf));
    }

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* const _vertex_code = vertex_code.c_str();
    const char* const _geometry_code = geometry_code.c_str();
    const char* const _fragment_code = fragment_code.c_str();

    memset(shader_log, 0, sizeof(shader_log));
    glShaderSource(vertex_shader, 1, &_vertex_code, nullptr);
    glCompileShader(vertex_shader);
    glGetShaderInfoLog(vertex_shader, sizeof(shader_log), nullptr, shader_log);
    if(strlen(shader_log) > 0)
    {
        std::cerr << "[particle_pool / load_shaders] Error compiling vertex_shader " << std::endl << shader_log << std::endl;
        return false;
    }
    
    memset(shader_log, 0, sizeof(shader_log));
    glShaderSource(geometry_shader, 1, &_geometry_code, nullptr);
    glCompileShader(geometry_shader );
    glGetShaderInfoLog(geometry_shader, sizeof(shader_log), nullptr, shader_log);
    if(strlen(shader_log) > 0)
    {
        std::cerr << "[particle_pool / load_shaders] Error compiling geometry_shader " << std::endl << shader_log << std::endl;
        //return false;
    }

    memset(shader_log, 0, sizeof(shader_log));
    glShaderSource(fragment_shader, 1, &_fragment_code, nullptr);
    glCompileShader(fragment_shader);
    glGetShaderInfoLog(fragment_shader, sizeof(shader_log), nullptr, shader_log);
    if(strlen(shader_log) > 0)
    {
        std::cerr << "[particle_pool / load_shaders] Error compiling fragment_shader " << std::endl << shader_log << std::endl;
        return false;
    }

    memset(shader_log, 0, sizeof(shader_log));
    this->shader_prog = glCreateProgram();
    glAttachShader(this->shader_prog, vertex_shader);
    glAttachShader(this->shader_prog, geometry_shader);
    glAttachShader(this->shader_prog, fragment_shader);
    glLinkProgram(this->shader_prog);
    glGetProgramInfoLog(this->shader_prog, sizeof(shader_log), nullptr, shader_log);
    if(strlen(shader_log) > 0)
    {
        std::cerr << "[particle_pool / load_shaders] Error linking shader: " << std::endl << shader_log << std::endl;
        return false;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(geometry_shader);    
    glDeleteShader(fragment_shader);

    return true;
}

void particle::particle_pool::draw(glm::mat4 view, glm::mat4 projection)
{
    glDepthMask(GL_FALSE);
    glUseProgram(this->shader_prog);
    //GLint test_loc = glGetUniformLocation(this->shader_prog, "test");
    GLint view_loc = glGetUniformLocation(this->shader_prog, "view");
    GLint projection_loc = glGetUniformLocation(this->shader_prog, "projection");

    //std::clog << "[DEBUG] shader program: " << this->shader_prog << ", view:" << view_loc << ", projection: " << projection_loc << ", OpenGL Error: " << glGetError() << std::endl;

    glUniformMatrix4fv(view_loc, 1, false, glm::value_ptr(view));
    glUniformMatrix4fv(projection_loc, 1, false, glm::value_ptr(projection));

    glBindVertexArray(this->vao);
    glBindTexture(GL_TEXTURE_2D, this->tex);
    glDrawArrays(GL_POINTS, 0, this->particles.size());

    glBindVertexArray(0);
    glUseProgram(0);
    glDepthMask(GL_TRUE);
}

void particle::particle_pool::run_engine(std::chrono::system_clock::time_point now, std::chrono::system_clock::duration dt)
{
    using namespace std::chrono_literals;
    for (size_t i = 0; i < this->particles.size(); i++)
        particles.at(i).update_physics(now, (dt / 1us)*1e-6f );
    std::sort(particles.begin(), particles.end(), [&](particle lhs, particle rhs) {return (lhs.is_further_from_cam(rhs));});
    for (size_t i = 0; i < this->particles.size(); i++)
        memcpy(this->data + i, &this->particles[i].vertex(), sizeof(p_vertex_t));
}
