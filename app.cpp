#include "app.hpp"

void __error_callback(int code, const char* msg)
{
    std::cerr << "[ERROR / GLFW] (code " << code << "): " << msg << std::endl;
}

void __key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if( (key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS) )
    {
        std::clog << "[GLFW / key input] Pressed ESC Key" << std::endl;
        glfwSetWindowShouldClose(window, 1);
    }
}

void mouse_action(GLFWwindow* window, int sx, int sy, double& rotx, double& roty, float sensetivity)
{
    constexpr double roty_max = 89.9/180.0*M_PI;
    const double sx_half = sx / 2;
    const double sy_half = sy / 2;

    double mx, my;
    glfwGetCursorPos(window, &mx, &my);

    const double deltax = sx_half - mx;
    const double deltay = sy_half - my;

    rotx += deltax * sensetivity;
    roty += deltay * sensetivity;

    if(rotx >= 2*M_PI)
        rotx -= 2*M_PI;
    else if(rotx <= -2*M_PI)
        rotx += 2*M_PI;

    if(roty > roty_max)
        roty = roty_max;
    else if(roty < -roty_max)
        roty = -roty_max;

    glfwSetCursorPos(window, sx_half, sy_half);
}
void move_action(GLFWwindow* window, double& posx, double& posy, double& posz, double rotx, float speed)
{
    static double old_time = glfwGetTime();
    double deltatime = glfwGetTime() - old_time;
    old_time = glfwGetTime();

    if(glfwGetKey(window, 'W') == GLFW_PRESS)
    {
        posx += speed * deltatime * sin(rotx);
        posz += speed * deltatime * cos(rotx);
    }
    if(glfwGetKey(window, 'A') == GLFW_PRESS)
    {
        posx += speed * deltatime * sin(rotx + M_PI / 2);
        posz += speed * deltatime * cos(rotx + M_PI / 2);
    }
    if(glfwGetKey(window, 'S') == GLFW_PRESS)
    {
        posx += speed * deltatime * sin(rotx + M_PI);
        posz += speed * deltatime * cos(rotx + M_PI);
    }
    if(glfwGetKey(window, 'D') == GLFW_PRESS)
    {
        posx += speed * deltatime * sin(rotx + M_PI * 1.5);
        posz += speed * deltatime * cos(rotx + M_PI * 1.5);
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        posy += speed * deltatime;
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        posy -= speed * deltatime;
}

particle::application::~application()
{
    glfwTerminate();
}

void particle::application::render_frame()
{
    int width, height;

    // set camera
    glfwGetWindowSize(this->window, &width, &height);
    mouse_action(this->window, width, height, cam.yaw, cam.pitch, config->cam_sensitivity);
    move_action(this->window, cam.x, cam.y, cam.z, cam.yaw, config->cam_speed);

    // set up viewport
    glViewport(0, 0, width, height);

    // clear color and Z-buffer
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


    // camera matrices
    glm::mat4 view = glm::lookAt(glm::dvec3(this->cam.x, this->cam.y, this->cam.z),
                     glm::dvec3(this->cam.x + sin(this->cam.yaw) * cos(this->cam.pitch),
                                  this->cam.y + sin(this->cam.pitch),
                                  this->cam.z + cos(this->cam.yaw) * cos(this->cam.pitch)),
                     glm::dvec3(0.0, 1.0, 0.0));
    glm::mat4 projection = glm::perspective(glm::radians(this->config->cam_fov), ((float)width / (float)height), 0.001f, 500.0f);

    // enable depth test
    glEnable(GL_DEPTH_TEST);

    this->shader.use();
    this->shader.uniform_matrix_4x4f("mvp", 1, false, glm::value_ptr(projection * view) );
    //glBindVertexArray(this->fountain.vao);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    this->fountain.draw();
    return;
}

void particle::application::static_obj::draw()
{
    if(this->count > 0) // ognore if no verticeshave to be drawn
    {
        glBindVertexArray(this->vbo);
        glDrawArrays(GL_TRIANGLES, 0, this->count);
    }
}

void particle::application::static_obj::generate()
{
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(this->triangles.size()*sizeof( triangle_t )), this->triangles.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(vertex_t), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(vertex_t), (const void*)(2*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(vertex_t), (const void*)(6*sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    this->count = 3 * this->triangles.size();
    std::clog << "[static_obj / generate] vao: " << vao << ", vbo: " << vbo << std::endl;
}

void particle::application::setup_structures()
{
    // testing code
    this->fountain.triangles.resize(3);
    this->fountain.triangles.at(0).first =  {0.0, 0.0, {1.0, 1.0, 1.0, 1.0}, -1.0, -1.0, +1.0};
    this->fountain.triangles.at(0).second = {0.0, 0.0, {1.0, 1.0, 1.0, 1.0}, -1.0, +1.0, +1.0};
    this->fountain.triangles.at(0).third =  {0.0, 0.0, {1.0, 1.0, 1.0, 1.0}, +1.0, -1.0, +1.0};

    this->fountain.triangles.at(1).first =  {0.0, 0.0, {1.0, 0.0, 0.0, 1.0}, -1.0, -1.0, -1.0};
    this->fountain.triangles.at(1).second = {0.0, 0.0, {0.0, 1.0, 0.0, 1.0}, -1.0, +1.0, -1.0};
    this->fountain.triangles.at(1).third =  {0.0, 0.0, {0.0, 0.0, 1.0, 1.0}, +1.0, -1.0, -1.0};

    this->fountain.triangles.at(2).first =  {0.0, 0.0, {0.0, 1.0, 1.0, 1.0}, -1.0, +1.0, +1.0};
    this->fountain.triangles.at(2).second = {0.0, 1.0, {0.0, 1.0, 1.0, 1.0}, +1.0, -1.0, +1.0};
    this->fountain.triangles.at(2).third =  {1.0, 0.0, {0.0, 1.0, 1.0, 1.0}, +1.0, +1.0, +1.0};
    this->fountain.generate();
}

void particle::application::run()
{
    using namespace std::chrono_literals; // 1s, 1us
    std::clog << "[application / run] application started" << std::endl;
    this->t_last_frame = this->t_start = std::chrono::system_clock::now();
    while (!glfwWindowShouldClose(this->window))
    {
        // get the new frame time and calculate time difference
        this->t_cur_frame = std::chrono::system_clock::now();
        this->dt_frame = t_cur_frame - t_last_frame;
        t_last_frame = t_cur_frame;

        this->render_frame();
        glfwSwapBuffers(this->window);
        glfwPollEvents();
        this->frame_count++;

        //std::clog << "[application / run] camera x:" << this->cam.x << ",y:"<< this->cam.y << ",z:" << this->cam.z << " yaw:" << this->cam.yaw << ",pitch:" << this->cam.pitch << std::flush;
        std::clog << "[application / run] current frame time: " << (this->dt_frame / 1us) << "us  \r" << std::flush;
    }
    std::chrono::system_clock::duration dt_total = (std::chrono::system_clock::now() - this->t_start);
    std::clog << "[application / run] total number of frames: " << this->frame_count << std::endl
              << "[application / run] total time elapsed: "<< (dt_total / 1s) << "s" << std::endl
              << "[application / run] avg. FPS: " << (this->frame_count / (dt_total / 1s)) << std::endl;
}

bool particle::application::init(configuration& config)
{
#if 0 // Disable this code to not set the DISPLAY environment variable for WSL2
    setenv("DISPLAY", "LAPTOP-EH8FTC7B.mshome.net:0", 1);
#endif
    //omp_set_num_threads(omp_get_max_threads()); // OpenMP initialisation
    glfwSetErrorCallback(__error_callback);      // GLFW Error handling
    this->config = &config;                     // get configuration

    glfwInit();
    glfwWindowHint(GLFW_RED_BITS, 24);              // set red bits
    glfwWindowHint(GLFW_GREEN_BITS, 24);            // set green bits
    glfwWindowHint(GLFW_BLUE_BITS, 24);             // set blue bits
    glfwWindowHint(GLFW_ALPHA_BITS, 24);            // set alpha bits
    glfwWindowHint(GLFW_DEPTH_BITS, 24);            // set depth bits
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // use OpenGL 4.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* vid_mode = glfwGetVideoMode(monitor);

    // create window
    window = glfwCreateWindow(vid_mode->width / 2, vid_mode->height / 2, "", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        std::cerr << "[application / init] failed to open Window" << std::endl;
        return false;
    }
    glfwSetWindowPos(window, vid_mode->width / 4, vid_mode->height / 4);
    glfwSetWindowTitle(window, "particles");
    glfwMakeContextCurrent(window);

    // set GLFW Key callback
    glfwSetKeyCallback(window, __key_callback);

    glfwSetCursorPos(window, vid_mode->width / 2 + vid_mode->width / 4, vid_mode->height / 2 + vid_mode->height / 4);      // set mousepos to the center
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);   // disable cursor

    glfwSwapInterval(0);
    glewInit();

    // load shaders
    gl::ShaderLoadError error = this->shader.load("assets/shaders/main.vert", "assets/shaders/main.frag");
    if(error & gl::ShaderLoadErrorType::SHADER_ALREADY_LOADED)
        std::cerr << "[application / init] Shader Programs already loaded!" << std::endl;
    if(error & gl::ShaderLoadErrorType::INVALID_FILE_PATH)
        std::cerr << "[application / init] Shader Programs not found!" << std::endl;
    if(error & gl::ShaderLoadErrorType::VERTEX_SHADER_ERROR)
        std::cerr << "[application / init] Vertex Shader: " << shader.get_last_vertex_msg() << std::endl;
    if(error & gl::ShaderLoadErrorType::FRAGMENT_SHADER_ERROR)
        std::cerr << "[application / init] Fragment Shader: " << shader.get_last_fragment_msg() << std::endl;
    if(error & gl::ShaderLoadErrorType::SHADER_LINK_ERROR)
        std::cerr << "[application / init] Linker: " << shader.get_last_link_msg() << std::endl;
    if(error != gl::ShaderLoadErrorType::NONE)
    {
        std::cerr << "[application / init] Error compiling Shaders (code " << std::hex << error << ")" << std::endl;
        return false;
    }

    // reset camera
    this->cam = {0.0, 0.0, 0.0, 0.0, 0.0};

    this->setup_structures();
    
    std::cerr << "OpenGL error: " << glGetError() << std::endl;

    return true;
}
