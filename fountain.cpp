#include "app.hpp"

void particle::application::fountain_generate_normals_last_ring()
{
    //for(size_t ring = )
}

bool particle::application::fountain_setup(const char *texture_filename)
{
    glm::vec4 fountain_color = {1.0, 1.0, 1.0, 1.0}; // color for the fountain
    glm::vec4 fountain_outlet_color = {0.33, 0.33, 0.33, 1.0}; // color for the fountain outlet
    size_t first_index = -1, prev_first_index = -1;
    this->fountain.vertices.clear();
    this->fountain.indices.clear();
    this->fountain.vertices.push_back({glm::vec2(0.0, 0.0), fountain_color, glm::vec3(0, float(config().fountain_height*0.975), 0)}); // top point [0]
    this->fountain.vertices.push_back({glm::vec2(0.0, 0.0), fountain_color, glm::vec3(0, 0, 0)}); // bottom point [1]
    this->fountain.vertices.push_back({glm::vec2(0.0, 0.0), fountain_outlet_color, glm::vec3(0, float(config().fountain_height), 0)}); // outlet point
    for(size_t i=0; i<config().fountain_sweep_steps; i++) // rotate around (bottom plane)
    {
        double angle = i * (2*M_PI / float(config().fountain_sweep_steps)); // calculate angle
        double x = sin(angle), z = cos(angle);
        this->fountain.vertices.push_back({glm::vec2(x*0.9, z*0.9), fountain_color, glm::vec3(x*config().fountain_width*0.9, 0, z*config().fountain_width*0.9)});
        if(i==0) {prev_first_index = first_index; first_index = this->fountain.vertices.size()-1;} // set first vertex of current plane and previous first index
        // TRIANGLE #0.0: make bottom plane
        /*this->fountain.indices.push_back(1); // triangle: 1, current, current+1 or 1
        this->fountain.indices.push_back(this->fountain.vertices.size()-1);
        if(i == float(config->fountain_sweep_steps)-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
        */
    }
    for(size_t i=0; i<config().fountain_sweep_steps; i++) // rotate around (bottom socket plane)
    {
        double angle = i * (2*M_PI / float(config().fountain_sweep_steps)); // calculate angle
        double x = sin(angle), z = cos(angle);
        this->fountain.vertices.push_back({glm::vec2(x*0.5f, z*0.5f), fountain_color, glm::vec3(x * config().fountain_width * 0.5f, config().fountain_height*0.2f, z*config().fountain_width*0.5f)});
        if(i==0) {prev_first_index = first_index; first_index = this->fountain.vertices.size()-1;} // set first vertex of current plane and previous first index
        // TRIANGLE #1.0: make socket, half 1
        this->fountain.indices.push_back(this->fountain.vertices.size()-1);
        this->fountain.indices.push_back(this->fountain.vertices.size()-1-float(config().fountain_sweep_steps));
        if(i == float(config().fountain_sweep_steps)-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
        // TRIANGLE #1.1: make socket, half 2
        this->fountain.indices.push_back(this->fountain.vertices.size()-1-float(config().fountain_sweep_steps));
        if(i == float(config().fountain_sweep_steps)-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
        if(i == float(config().fountain_sweep_steps)-1) // is last?
            this->fountain.indices.push_back(prev_first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()-float(config().fountain_sweep_steps)); // ther triangles -> 0, current, current+1
    }
    for(size_t i=0; i<config().fountain_sweep_steps; i++) // rotate around (Pillar)
    {
        double angle = i * (2*M_PI / float(config().fountain_sweep_steps)); // calculate angle
        double x = sin(angle), z = cos(angle);
        this->fountain.vertices.push_back({glm::vec2(x*0.1f, z*0.1f), fountain_color, glm::vec3(x * config().fountain_width * 0.5f, config().fountain_height*0.7f, z*config().fountain_width*0.5f)});
        if(i==0) {prev_first_index = first_index; first_index = this->fountain.vertices.size()-1;} // set first vertex of current plane and previous first index
        // TRIANGLE #2.1: make pillar, half 1
        this->fountain.indices.push_back(this->fountain.vertices.size()-1);
        this->fountain.indices.push_back(this->fountain.vertices.size()-1-float(config().fountain_sweep_steps));
        if(i == float(config().fountain_sweep_steps)-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
        // TRIANGLE #2.2: make pillar, half 2
        this->fountain.indices.push_back(this->fountain.vertices.size()-1-float(config().fountain_sweep_steps));
        if(i == float(config().fountain_sweep_steps)-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
        if(i == float(config().fountain_sweep_steps)-1) // is last?
            this->fountain.indices.push_back(prev_first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()-float(config().fountain_sweep_steps)); // ther triangles -> 0, current, current+1
    }
    for(size_t i=0; i<config().fountain_sweep_steps; i++) // rotate around (top plane supports)
    {
        double angle = i * (2*M_PI / float(config().fountain_sweep_steps)); // calculate angle
        double x = sin(angle), z = cos(angle);
        this->fountain.vertices.push_back({glm::vec2(x, z) * 0.9f, fountain_color, glm::vec3(x*config().fountain_width,config().fountain_height*0.95, z*config().fountain_width)});
        if(i==0) {prev_first_index = first_index; first_index = this->fountain.vertices.size()-1;} // set first vertex of current plane and previous first index
        // TRIANGLE #3.1: make to plane support, half 1
        this->fountain.indices.push_back(this->fountain.vertices.size()-1);
        this->fountain.indices.push_back(this->fountain.vertices.size()-1-float(config().fountain_sweep_steps));
        if(i == float(config().fountain_sweep_steps)-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
        // TRIANGLE #3.2: make to plane support, half 2
        this->fountain.indices.push_back(this->fountain.vertices.size()-1-float(config().fountain_sweep_steps));
        if(i == float(config().fountain_sweep_steps)-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
        if(i == float(config().fountain_sweep_steps)-1) // is last?
            this->fountain.indices.push_back(prev_first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()-float(config().fountain_sweep_steps)); // ther triangles -> 0, current, current+1
    }
    for(size_t i=0; i<config().fountain_sweep_steps; i++) // rotate around (top plane + top plane thickness)
    {
        double angle = i * (2*M_PI / float(config().fountain_sweep_steps)); // calculate angle
        double x = sin(angle), z = cos(angle);
        this->fountain.vertices.push_back({glm::vec2(x, z), fountain_color, glm::vec3(x*config().fountain_width, config().fountain_height, z*config().fountain_width)});
        if(i==0) {prev_first_index = first_index; first_index = this->fountain.vertices.size()-1;} // set first vertex of current plane and previous first index
        // TRIANGLE #4.1: make to plane thickness, half 1
        this->fountain.indices.push_back(this->fountain.vertices.size()-1);
        this->fountain.indices.push_back(this->fountain.vertices.size()-1-float(config().fountain_sweep_steps));
        if(i == float(config().fountain_sweep_steps)-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
        // TRIANGLE #4.2: make to plane thickness, half 2
        this->fountain.indices.push_back(this->fountain.vertices.size()-1-float(config().fountain_sweep_steps));
        if(i == float(config().fountain_sweep_steps)-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
        if(i == float(config().fountain_sweep_steps)-1) // is last?
            this->fountain.indices.push_back(prev_first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()-float(config().fountain_sweep_steps)); // ther triangles -> 0, current, current+1
        // TRIANGLE #4.3: make top plane
        this->fountain.indices.push_back(0); // triangle: 0, current, current+1 or 1
        this->fountain.indices.push_back(this->fountain.vertices.size()-1);
        if(i == float(config().fountain_sweep_steps)-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
    }
    for(size_t i=0; i<config().fountain_sweep_steps; i++)
    {
        double angle = i * (2*M_PI / float(config().fountain_sweep_steps)); // calculate angle
        double x = sin(angle), z = cos(angle);
        this->fountain.vertices.push_back({glm::vec2(x, z), fountain_outlet_color, glm::vec3(x*config().fountain_width*0.5, config().fountain_height*0.965, z*config().fountain_width*0.5)});
        if(i==0) {first_index = this->fountain.vertices.size()-1;}
        // TRIANGLE #5: fountain output
        this->fountain.indices.push_back(2); // triangle: 2, current, current+1 or 1
        this->fountain.indices.push_back(this->fountain.vertices.size()-1);
        if(i == float(config().fountain_sweep_steps)-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
    }
    bool generated = this->fountain.generate(texture_filename);

    this->fountain.vertices.clear();
    this->fountain.indices.clear();
    this->fountain.vertices.shrink_to_fit();
    this->fountain.indices.shrink_to_fit();

    return generated;
}
