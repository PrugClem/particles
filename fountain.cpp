#include "app.hpp"

bool particle::application::setup_fountain()
{
    color_t fountain_color = {1.0, 1.0, 1.0, 1.0}; // color for the faountain
    size_t first_index = -1, prev_first_index = -1;
    this->fountain.vertices.clear();
    this->fountain.indices.clear();
    this->fountain.vertices.push_back({0.0, 0.0, fountain_color, 0, fountain_height, 0}); // top point [0]
    this->fountain.vertices.push_back({0.0, 0.0, fountain_color, 0, 0, 0}); // bottom point [1]
    for(size_t i=0; i<fountain_sweep_steps; i++) // rotate around (bottom plane)
    {
        double angle = i * (2*M_PI / fountain_sweep_steps); // calculate angle
        double x = sin(angle), z = cos(angle);
        this->fountain.vertices.push_back({float(x*0.9), float(z*0.9), fountain_color, float(x*fountain_width*0.9), 0, float(z*fountain_width*0.9)});
        if(i==0) {prev_first_index = first_index; first_index = this->fountain.vertices.size()-1;} // set first vertex of current plane and previous first index
        // TRIANGLE #0.0: make bottom plane
        /*this->fountain.indices.push_back(1); // triangle: 1, current, current+1 or 1
        this->fountain.indices.push_back(this->fountain.vertices.size()-1);
        if(i == fountain_sweep_steps-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
        */
    }
    for(size_t i=0; i<fountain_sweep_steps; i++) // rotate around (bottom socket plane)
    {
        double angle = i * (2*M_PI / fountain_sweep_steps); // calculate angle
        double x = sin(angle), z = cos(angle);
        this->fountain.vertices.push_back({float(x*0.5), float(z*0.5), fountain_color, float(x*fountain_width*0.5), fountain_height*0.2, float(z*fountain_width*0.5)});
        if(i==0) {prev_first_index = first_index; first_index = this->fountain.vertices.size()-1;} // set first vertex of current plane and previous first index
        // TRIANGLE #1.0: make socket, half 1
        this->fountain.indices.push_back(this->fountain.vertices.size()-1);
        this->fountain.indices.push_back(this->fountain.vertices.size()-1-fountain_sweep_steps);
        if(i == fountain_sweep_steps-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
        // TRIANGLE #1.1: make socket, half 2
        this->fountain.indices.push_back(this->fountain.vertices.size()-1-fountain_sweep_steps);
        if(i == fountain_sweep_steps-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
        if(i == fountain_sweep_steps-1) // is last?
            this->fountain.indices.push_back(prev_first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()-fountain_sweep_steps); // ther triangles -> 0, current, current+1
    }
    for(size_t i=0; i<fountain_sweep_steps; i++) // rotate around (Pillar)
    {
        double angle = i * (2*M_PI / fountain_sweep_steps); // calculate angle
        double x = sin(angle), z = cos(angle);
        this->fountain.vertices.push_back({float(x*0.1), float(z*0.1), fountain_color, float(x*fountain_width*0.5), fountain_height*0.7, float(z*fountain_width*0.5)});
        if(i==0) {prev_first_index = first_index; first_index = this->fountain.vertices.size()-1;} // set first vertex of current plane and previous first index
        // TRIANGLE #2.1: make pillar, half 1
        this->fountain.indices.push_back(this->fountain.vertices.size()-1);
        this->fountain.indices.push_back(this->fountain.vertices.size()-1-fountain_sweep_steps);
        if(i == fountain_sweep_steps-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
        // TRIANGLE #2.2: make pillar, half 2
        this->fountain.indices.push_back(this->fountain.vertices.size()-1-fountain_sweep_steps);
        if(i == fountain_sweep_steps-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
        if(i == fountain_sweep_steps-1) // is last?
            this->fountain.indices.push_back(prev_first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()-fountain_sweep_steps); // ther triangles -> 0, current, current+1
    }
    for(size_t i=0; i<fountain_sweep_steps; i++) // rotate around (top plane supports)
    {
        double angle = i * (2*M_PI / fountain_sweep_steps); // calculate angle
        double x = sin(angle), z = cos(angle);
        this->fountain.vertices.push_back({float(x*0.9), float(z*0.9), fountain_color, float(x*fountain_width), fountain_height*0.95, float(z*fountain_width)});
        if(i==0) {prev_first_index = first_index; first_index = this->fountain.vertices.size()-1;} // set first vertex of current plane and previous first index
        // TRIANGLE #3.1: make to plane support, half 1
        this->fountain.indices.push_back(this->fountain.vertices.size()-1);
        this->fountain.indices.push_back(this->fountain.vertices.size()-1-fountain_sweep_steps);
        if(i == fountain_sweep_steps-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
        // TRIANGLE #3.2: make to plane support, half 2
        this->fountain.indices.push_back(this->fountain.vertices.size()-1-fountain_sweep_steps);
        if(i == fountain_sweep_steps-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
        if(i == fountain_sweep_steps-1) // is last?
            this->fountain.indices.push_back(prev_first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()-fountain_sweep_steps); // ther triangles -> 0, current, current+1
    }
    for(size_t i=0; i<fountain_sweep_steps; i++) // rotate around (top plane + top plane thickness)
    {
        double angle = i * (2*M_PI / fountain_sweep_steps); // calculate angle
        double x = sin(angle), z = cos(angle);
        this->fountain.vertices.push_back({float(x), float(z), fountain_color, float(x*fountain_width), fountain_height, float(z*fountain_width)});
        if(i==0) {prev_first_index = first_index; first_index = this->fountain.vertices.size()-1;} // set first vertex of current plane and previous first index
        // TRIANGLE #4.1: make to plane thickness, half 1
        this->fountain.indices.push_back(this->fountain.vertices.size()-1);
        this->fountain.indices.push_back(this->fountain.vertices.size()-1-fountain_sweep_steps);
        if(i == fountain_sweep_steps-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
        // TRIANGLE #4.2: make to plane thickness, half 2
        this->fountain.indices.push_back(this->fountain.vertices.size()-1-fountain_sweep_steps);
        if(i == fountain_sweep_steps-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
        if(i == fountain_sweep_steps-1) // is last?
            this->fountain.indices.push_back(prev_first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()-fountain_sweep_steps); // ther triangles -> 0, current, current+1
        // TRIANGLE #4.3: make top plane
        this->fountain.indices.push_back(0); // triangle: 0, current, current+1 or 1
        this->fountain.indices.push_back(this->fountain.vertices.size()-1);
        if(i == fountain_sweep_steps-1) // is last?
            this->fountain.indices.push_back(first_index); // last triangle -> 0, current, first
        else
            this->fountain.indices.push_back(this->fountain.vertices.size()); // ther triangles -> 0, current, current+1
    }
    bool generated = this->fountain.generate("assets/textures/metal2.jpg");

    this->fountain.vertices.clear();
    this->fountain.indices.clear();
    this->fountain.vertices.shrink_to_fit();
    this->fountain.indices.shrink_to_fit();

    return generated;
}
