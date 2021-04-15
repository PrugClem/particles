#include "particles.hpp"
#include "app.hpp"

particle::particle::particle() : particle::particle(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec4(0.0f), 0.0f) {}
particle::particle::particle(const glm::vec3& start_pos, const glm::vec3& start_vel, const glm::vec4& color, float width) {this->spawn(start_pos, start_vel, color, width);}
particle::particle::~particle(void) {}

void particle::particle::spawn(const glm::vec3& start_pos, const glm::vec3& start_vel, const glm::vec4& color, float width)
{
    using namespace std::chrono_literals;
    this->alive = true;
    this->_vertex.pos = start_pos;
    this->vel = start_vel;
    this->ttd = std::chrono::system_clock::now() + application::app->config->particle_ttl + 
        std::chrono::milliseconds( uint64_t(abs( random::normal(0, (application::app->config->particle_ttl / 1ms) ) )) );
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
        if( this->ttd < now ) // particle died
        {
#if 1
            this->alive = false;
            this->_vertex.width = 0.05 + abs(random::normal(0, 0.05));
            glm::vec3 start(0, application::app->config->fountain_height + this->_vertex.width, 0);
            glm::vec3 vel( random::normal(0, 2), abs(random::normal(0, 4)), random::normal(0, 2) );
            glm::vec4 color( random::uniform(0,1), random::uniform(0,1), random::uniform(0,1), 1 );
            this->spawn(start, vel, color, this->_vertex.width);
#else
            this->spawn(glm::vec3(0, application::app->config->fountain_height + this->_vertex.width, 0),
                        glm::vec3(1, 1, 0), glm::vec4(1), 0.3);
#endif
            //std::clog << "[DEBUG] particle respawned" << std::endl;
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
                //this->vel.y = -this->vel.y;
                //this->vel *= application::app->config->particle_bounce_friction;
                // position handling
                this->_vertex.pos.y = this->_vertex.width/2 + application::app->config->fountain_height +
                    (this->_vertex.pos.y - (this->_vertex.width / 2 + application::app->config->fountain_height)) * application::app->config->particle_bounce_friction;
            }
            // collision detection, floor
            else if(this->_vertex.pos.y < this->_vertex.width / 2) // throu floor
            {
                // velocity handling
                //this->vel.y = -this->vel.y * application::app->config->particle_bounce_friction; 
                this->vel.y = -this->vel.y;
                this->vel *= application::app->config->particle_bounce_friction;
                // position handling
                this->_vertex.pos.y = this->_vertex.width/2 + 
                    (this->_vertex.width/2 - this->_vertex.pos.y) * application::app->config->particle_bounce_friction;
            }
        }
    }
}

bool particle::particle::is_further_from_cam(const particle& other)
{
    return (glm::distance(this->_vertex.pos, application::app->cam.pos ) >
            glm::distance(other._vertex.pos, application::app->cam.pos) );
}
