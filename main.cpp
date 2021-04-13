#define STB_IMAGE_IMPLEMENTATION // define here to only implement stm once
#include "particles.hpp"
#include "app.hpp"

int main()
{
    particle::application app;
    particle::application::configuration config;

    config.max_particles = 100;

    config.fountain_height = 1;
    config.fountain_width = 0.5;
    config.fountain_sweep_steps = 6;

    config.cam_sensitivity = 0.0025;
    config.cam_speed = 2;
    config.cam_fov = 100;


    if(app.init(config))
    {
        std::clog << "[main] starting application" << std::endl;
        app.run();
        std::clog << "[main] application ended" << std::endl;
    }
    else
    {
        std::clog << "[main] failed to start application" << std::endl;
    }


    return 0;
}
