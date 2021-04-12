#include "app.hpp"

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
    std::getline(model_file, line_buffer); // get filename, if an empty string ("") is read, use no texture
    // get texture filename from line buffer and remove \r and \n characters because they make problems
    std::copy_if(line_buffer.begin(), line_buffer.end(), std::back_inserter(texture_filename),
                [] (char c) {return c != '\r' && c != '\n';});
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
