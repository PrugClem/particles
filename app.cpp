#include "app.hpp"

void __error_callback(int code, const char* msg)
{
    std::cerr << "[ERROR / GLFW] (code " << code << "): " << msg << std::endl;
}

void __key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if( (key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS) )
    {
        std::clog << "[GLFW / key input] Pressed ESC Key  " << std::endl;
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
    this->test.draw();
    return;
}

void particle::application::static_obj::draw()
{
    glBindVertexArray(this->vao);
    glBindTexture(GL_TEXTURE_2D, this->tex);
    glDrawElements(GL_TRIANGLES, this->num_idx, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

bool particle::application::static_obj::read_from_file(const std::string &model_filename)
{
    std::string texture_filename;
    std::string line_buffer;
    std::ifstream model_file;
    std::istringstream ss;
    std::size_t sz;
    vertex_t vertex_buffer;
    index_t idx;
    float f;
    bool generated;

    model_file.open(model_filename);
    if(!model_file.is_open())
    {
        std::cerr << "[static_obj / read_from_file] unable to open object model file " << model_filename << std::endl;
        return false;
    }
    std::getline(model_file, texture_filename); // get filename, if an empty string ("") is read, use no texture
    std::getline(model_file, line_buffer); // get indices as string
    // read indices
    this->indices.clear();
    this->vertices.clear();
    ss.clear();
    ss.str(line_buffer);
    while(!ss.eof())
    {
        ss >> idx;
        this->indices.push_back(idx);
    }
    // read vertex information
    while (!model_file.eof())
    {
        std::getline(model_file, line_buffer); // get vertex information
        ss.clear();
        ss.str(line_buffer);
        ss  >> vertex_buffer.s >> vertex_buffer.t 
            >> vertex_buffer.color.r >> vertex_buffer.color.g >> vertex_buffer.color.b >> vertex_buffer.color.a 
            >> vertex_buffer.x >> vertex_buffer.y >> vertex_buffer.z;
        this->vertices.push_back(vertex_buffer);
    }

    // generate model
    generated = this->generate( (texture_filename == "") ? nullptr : texture_filename.c_str() );

    this->vertices.clear();
    this->vertices.shrink_to_fit();
    this->indices.clear();
    this->indices.shrink_to_fit();
    
    return generated;
}

bool particle::application::static_obj::generate(const char *texture_file)
{
    if(texture_file != nullptr) // dont generate texture if no texture is given, if no texture is given, use default texture (0)
    {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(texture_file, &width, &height, &nrChannels, 4); // load
        if(data == nullptr)
        {
            std::cerr << "[static_obj / generate] texture " << texture_file << " not loaded" << std::endl;
            return false;
        }
        glGenTextures(1, &this->tex);
        glBindTexture(GL_TEXTURE_2D, this->tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    }
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ebo);

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(this->vertices.size()*sizeof( vertex_t )), this->vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLsizeiptr(this->indices.size()*sizeof(index_t)), this->indices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(vertex_t), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(vertex_t), (const void*)(2*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(vertex_t), (const void*)(6*sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    this->num_idx = this->indices.size();
    return true;
}

bool particle::application::setup_structures()
{
    // testing code
    this->fountain.vertices.resize(10);
    this->fountain.vertices.at(0) = {NAN, NAN, {1.0, 1.0, 1.0, 1.0}, -1.0, -1.0, +1.0};
    this->fountain.vertices.at(1) = {NAN, NAN, {1.0, 1.0, 1.0, 1.0}, -1.0, +1.0, +1.0};
    this->fountain.vertices.at(2) = {NAN, NAN, {1.0, 1.0, 1.0, 1.0}, +1.0, -1.0, +1.0};

    this->fountain.vertices.at(3) = {10.0, 10.0, {1.0, 0.0, 0.0, 1.0}, -1.0, -1.0, -1.0};
    this->fountain.vertices.at(4) = {10.0, 00.0, {0.0, 1.0, 0.0, 1.0}, -1.0, +1.0, -1.0};
    this->fountain.vertices.at(5) = {00.0, 10.0, {0.0, 0.0, 1.0, 1.0}, +1.0, -1.0, -1.0};
    this->fountain.vertices.at(9) = {00.0, 00.0, {1.0, 1.0, 1.0, 1.0}, +1.0, +1.0, -1.0};

    this->fountain.vertices.at(6) = {1.0, 0.0, {1.0, 1.0, 1.0, 1.0}, -1.0, +1.0, +1.0};
    this->fountain.vertices.at(7) = {0.0, 1.0, {1.0, 1.0, 1.0, 1.0}, +1.0, -1.0, +1.0};
    this->fountain.vertices.at(8) = {0.0, 0.0, {1.0, 1.0, 1.0, 1.0}, +1.0, +1.0, +1.0};

    this->fountain.indices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 4, 5, 9};

    this->fountain.generate("assets/textures/minecraft_bricks.png");

    return this->test.read_from_file("assets/models/bricks.txt");
}

void particle::application::run()
{
    using namespace std::chrono_literals; // 1s, 1us
    std::clog << std::endl << "[application / run] application started" << std::endl;
    this->t_last_frame = this->t_start = std::chrono::system_clock::now();
    while (!glfwWindowShouldClose(this->window))
    {
        // get the new frame time and calculate time difference
        this->t_cur_frame = std::chrono::system_clock::now();
        this->dt_frame = t_cur_frame - t_last_frame;
        t_last_frame = t_cur_frame;

        // render current frame, dra frame to display and process input events
        this->render_frame();
        glfwSwapBuffers(this->window);
        glfwPollEvents();
        this->frame_count++;

        //std::clog << "[application / run] camera x:" << this->cam.x << ",y:"<< this->cam.y << ",z:" << this->cam.z << " yaw:" << this->cam.yaw << ",pitch:" << this->cam.pitch << std::flush;
        std::clog << "[application / run] current frame time: " << (this->dt_frame / 1us) << "us  \r" << std::flush;
    }
    std::chrono::system_clock::duration dt_total = (std::chrono::system_clock::now() - this->t_start);
    std::clog << "[application / run] total number of frames: " << this->frame_count << std::endl
              << "[application / run] total time elapsed: "<< 0.001*(dt_total / 1ms) << "s" << std::endl
              << "[application / run] avg. FPS: " << (this->frame_count / (0.001*(dt_total / 1ms)) ) << std::endl;
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

    if(!this->setup_structures())
    {
        std::cerr << "[application / init] error setting up strucuture fountain" << std::endl;
        return false;
    }
    
    std::cerr << "OpenGL error: " << glGetError() << std::endl;

    return true;
}
