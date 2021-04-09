#include "particles.hpp"
#include "app.hpp"

int main()
{
    particle::application app;
    particle::application::configuration config;

    config.max_particles = 100;
    config.cam_sensitivity = 0.002;
    config.cam_speed = 1;
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
