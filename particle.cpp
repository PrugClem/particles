#include "particles.hpp"
#include "app.hpp"

particle::particle::particle() : particle::particle(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec4(0.0f), 0.0f) {}
particle::particle::particle(const glm::vec3& start_pos, const glm::vec3& start_vel, const glm::vec4& color, float width) {this->spawn(start_pos, start_vel, color, width);}
particle::particle::~particle(void) {}

void particle::particle::spawn(const glm::vec3& start_pos, const glm::vec3& start_vel, const glm::vec4& color, float width)
{
    this->alive = true;
    this->_vertex.pos = start_pos;
    this->vel = start_vel;
    this->ttd = std::chrono::system_clock::now() + application::app->config->particle_ttl;//+ std::chrono::seconds(1);
    //for(size_t i=0; i<4; i++)
        //this->vertices[i].color = color;
    this->_vertex.color = color;
    this->_vertex.width = width;
}

const particle::p_vertex_t& particle::particle::vertex()
{
    return this->_vertex;
}

void particle::particle::update_physics(std::chrono::system_clock::time_point now, float dt)
{
    if(this->alive)
    {
        if( this->ttd > now ) // particle died
        {
            this->alive = false;
            //this->spawn(glm::vec3(0), glm::vec3(0), glm::vec4(0), 0);
            //this->ttd = now + application::app->config->particle_ttl;
        }
        // apply velocity
        this->_vertex.pos += this->vel * dt;
        this->vel.y -= (application::app->config->particle_gravity * dt);

        if(this->vel.y < 0) // only run collision detection if particle is moving downward
        {
            // collision detection, fountain, using pythagoras
            if( (this->_vertex.pos.x*this->_vertex.pos.x + this->_vertex.pos.z*this->_vertex.pos.z < application::app->config->fountain_width*application::app->config->fountain_width) &&
                (this->_vertex.pos.y < (application::app->config->fountain_height + this->_vertex.width/2)) )
            {
                // velocity handling
                this->vel.y = -this->vel.y * application::app->config->particle_bounce_friction; 
                // position handling
                this->_vertex.pos.y = this->_vertex.width/2 + application::app->config->fountain_height +
                    (this->_vertex.width/2 - this->_vertex.pos.y) * application::app->config->particle_bounce_friction;
            }
            // collision detection, floor
            else if(this->_vertex.pos.y < 0) // throu floor
            {
                // velocity handling
                this->vel.y = -this->vel.y * application::app->config->particle_bounce_friction; 
                // position handling
                this->_vertex.pos.y = this->_vertex.width/2 + 
                    (this->_vertex.width/2 - this->_vertex.pos.y) * application::app->config->particle_bounce_friction;
            }
        }

        this->_vertex.pos = this->_vertex.pos;
/*
        // update vertices, layout particle facing up

        // vertex winding order
        //           +x +z
        //      1    0 
        //      2    3  
        //   x -z
        
        this->vertices[0].pos.y = this->pos.y;
        this->vertices[1].pos.y = this->pos.y;
        this->vertices[2].pos.y = this->pos.y;
        this->vertices[3].pos.y = this->pos.y;

        this->vertices[0].pos.x = this->pos.x + this->width/2;
        this->vertices[0].pos.z = this->pos.z + this->width/2;
        this->vertices[1].pos.x = this->pos.x - this->width/2;
        this->vertices[1].pos.z = this->pos.z + this->width/2;
        this->vertices[2].pos.x = this->pos.x - this->width/2;
        this->vertices[2].pos.z = this->pos.z - this->width/2;
        this->vertices[3].pos.x = this->pos.x + this->width/2;
        this->vertices[3].pos.z = this->pos.z - this->width/2;

        vec4(+width*0.5f, +0.0f, +width*0.5f, +0.0f)
        vec4(-width*0.5f, +0.0f, +width*0.5f, +0.0f)
        vec4(-width*0.5f, -0.0f, -width*0.5f, +0.0f)
        vec4(+width*0.5f, -0.0f, +width*0.5f, +0.0f)
*/
    }
}
