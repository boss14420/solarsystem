
// windows.h must be included before GL headers
#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#include <windows.h>
#endif

//#include <GL/glew.h>
#include <GLES2/gl2.h>
//#include <GLFW/glfw3.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL.h>
//#include <GL/glfw.h>
//#include <GL/gl.h>
//#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>
#include <deque>
#include <cstdint>
#include <random>
#include <algorithm>
#include <functional>
#include <unordered_set>


#include "config.hh"
#include "constants.h"
//#include "rendering.h"
//#include "bmp_loader.h"
//#include "planet.h"
#include "objects/solarsystem.hh"
#include "objects/planetdata.hh"

//#include "common/shader.hpp"
////#include "common/controls.hpp"
//#include "common/texture.hpp"
//#include "common/objloader.hpp"
//#include "common/vboindexer.hpp"

static Float xpos = 0.0f, ypos = 3.0f, zpos = 5.5f;
static Float sight_x = 0.0f, sight_y = -0.43f, sight_z = -0.9f;
static Float up_x = 0.0f, up_y = 1.0f;
static Float cam_xz_angle = 0.0f, cam_y_angle = -0.45f;
static Float cam_z_angle = M_PI_2;
static bool orbits = false;
static unsigned last_time = 0, frames = 0;
//static SDL_Window *window = NULL;
//static TTF_Font *font = NULL;

static mat4 ProjectionMatrix;

void renderScene(const SolarSystem &solarsystem) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glLoadIdentity();
    mat4 ModelMatrix, ViewMatrix, MVP;

//    gluLookAt(xpos,           ypos,           zpos,
//              xpos + sight_x, ypos + sight_y, zpos + sight_z,
//              up_x,           up_y,           0.0f);

    ModelMatrix = mat4(1.0);

    ViewMatrix = glm::lookAt(vec3(xpos, ypos, zpos),
                             vec3(xpos+sight_x, xpos+sight_y, xpos+sight_z),
                             vec3(up_x, up_y, 0.0f));

//    ProjectionMatrix = glm::perspective(45.0f, (float) 800/600, 0.1f, 50.0f);
    
    MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
    solarsystem.render(ModelMatrix, ViewMatrix, MVP);

//    Float sun_p[] = {0.0f, 0.0f, 0.0f, 1.0f};
////    glLightfv(GL_LIGHT0, GL_POSITION, sun_p);
//    glUniform4fv(sunlight_positionID, 1, sun_p);

    //drawAxes();
//    drawSky();

//    drawSun();
//    drawPlanets(orbits);
//    if (font)
//        drawStats(font, 1000 * frames / SDL_GetTicks());

//    SDL_GL_SwapWindow(window);
//    glfwSwapBuffers(window);
}

void reshape(int w, int h) {
    if (!h)
        h = 1;
//
    glViewport(0, 0, (GLint) w, (GLint) h);
//
////    glMatrixMode(GL_PROJECTION);
////    glLoadIdentity();
////
////    gluPerspective(45.0f, (float) w/h, 0.1f, 50.0f);
    ProjectionMatrix = glm::perspective(45.0f, (float) w/h, 0.1f, 50.0f);
//
////    glMatrixMode(GL_MODELVIEW);
}

void keyboard(SDL_Window *window, SDL_Scancode scancode) {
//void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
//    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, GL_TRUE);

    Uint32 flags;
//
//    if (action != GLFW_PRESS && action != GLFW_REPEAT)
//        return;

    switch (scancode) {
//        case GLFW_KEY_ESCAPE:
//        case SDL_SCANCODE_Q:
//            glfwSetWindowShouldClose(window, GL_TRUE);
//            break;
        case SDL_SCANCODE_W:
            xpos += sight_x * 0.08f;
            ypos += sight_y * 0.08f;
            zpos += sight_z * 0.08f;
            break;
        case SDL_SCANCODE_S:
            xpos -= sight_x * 0.08f;
            ypos -= sight_y * 0.08f;
            zpos -= sight_z * 0.08f;
            break;
        case SDL_SCANCODE_A:
            xpos -= -sight_z * 0.08f;
            zpos -= sight_x * 0.08f;
            break;
        case SDL_SCANCODE_D:
            xpos += -sight_z * 0.08f;
            zpos += sight_x * 0.08f;
            break;
        case SDL_SCANCODE_Z:
            ypos += -sight_z * 0.08f;
            zpos += sight_y * 0.08f;
            break;
        case SDL_SCANCODE_X:
            ypos -= -sight_z * 0.08f;
            zpos -= sight_y * 0.08f;
            break;
        case SDL_SCANCODE_R:
            xpos = 0.0f, ypos = 1.0f, zpos = 2.5f;
            sight_x = 0.0f, sight_y = -0.43f, sight_z = -0.9f;
            up_x = 0.0f, up_y = 1.0f;
            cam_xz_angle = 0.0f, cam_y_angle = -0.45f;
            cam_z_angle = M_PI_2;
            break;
        case SDL_SCANCODE_F:
            flags = SDL_GetWindowFlags(window);
            SDL_SetWindowFullscreen(window, flags ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
            break;
        case SDL_SCANCODE_O:
            orbits = !orbits;
            break;
        case SDL_SCANCODE_LEFT:
            cam_xz_angle -= 0.02f;
            sight_x = cosf(cam_y_angle) * sinf(cam_xz_angle);
            sight_z = -cosf(cam_y_angle) * cosf(cam_xz_angle);
            break;
        case SDL_SCANCODE_RIGHT:
            cam_xz_angle += 0.02f;
            sight_x = cosf(cam_y_angle) * sinf(cam_xz_angle);
            sight_z = -cosf(cam_y_angle) * cosf(cam_xz_angle);
            break;
        case SDL_SCANCODE_UP:
            if (cam_y_angle > M_PI / 3) // Do not look up to avoid gimbal locks and other problems
                break;
            cam_y_angle += 0.02;
            sight_x = cosf(cam_y_angle) * sinf(cam_xz_angle);
            sight_y = sinf(cam_y_angle);
            sight_z = -cosf(cam_y_angle) * cosf(cam_xz_angle);
            break;
        case SDL_SCANCODE_DOWN:
            if (cam_y_angle < -M_PI / 3) // And do not look down
                break;
            cam_y_angle -= 0.02f;
            sight_x = cosf(cam_y_angle) * sinf(cam_xz_angle);
            sight_y = sinf(cam_y_angle);
            sight_z = -cosf(cam_y_angle) * cosf(cam_xz_angle);
            break;
        case SDL_SCANCODE_PAGEUP:
            cam_z_angle += 0.02f;
            up_x = cosf(cam_z_angle);
            up_y = sinf(cam_z_angle);
            break;
        case SDL_SCANCODE_PAGEDOWN:
            cam_z_angle -= 0.02f;
            up_x = cosf(cam_z_angle);
            up_y = sinf(cam_z_angle);
            break;
        default:
            break;
    }
}

int main(int argc, char **argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
//    TTF_Init();
    std::atexit(SDL_Quit);
#ifdef USE_OPENGLES
    SDL_SetHintWithPriority(SDL_HINT_RENDER_DRIVER, "opengles2", SDL_HINT_OVERRIDE);
#endif
    SDL_Window *window = SDL_CreateWindow("Solar system", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 400, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        SDL_Log("Couldn't create SDL Window: %s\n", SDL_GetError());
        return 1;
    }
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    if (glcontext == nullptr) {
        SDL_Log("Coudn't create SDL GL contex: %s\n", SDL_GetError());
        return 1;
    }
//    SDL_GL_SetSwapInterval(1); // Enable VSYNC
    reshape(600, 400); // SDL does not send resize event on startup

//    font = TTF_OpenFont("Vera.ttf", 16);


//    // Initialise GLFW
//    if (!glfwInit()) {
//        std::fputs("Failed to initialize GLFW\n", stderr);
//        return -1;
//    }
//
//    glfwDefaultWindowHints();
//    glfwWindowHint(GLFW_SAMPLES, 4);
//#ifdef USE_OPENGLES
//    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//#else
//    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//#endif
//    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
//    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
//    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);

    // Open a window and create its OpenGL context
//    window = glfwOpenWindow( 800, 600, 0,0,0,0, 32,0, GLFW_WINDOW );
//    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
//    int width = mode->width * 2 / 3;
//    int height = mode->height * 2 / 3;
//    GLFWwindow *window = glfwCreateWindow(width, height, "Solar system", nullptr, nullptr); 
//    if (!window) {
//        std::fputs( "Failed to open GLFW window.\n", stderr );
//        glfwTerminate();
//        return -1;
//    }
//    ProjectionMatrix = glm::perspective(45.0f, (float) width/height, 0.1f, 50.0f);
//
//    glfwMakeContextCurrent(window);
//    glfwSetKeyCallback(window, keyboard);
//    glfwSetFramebufferSizeCallback(window, reshape);

//    if (glewInit() != GLEW_OK) {
//        std::fputs("Failed to initialize GLEW\n", stderr);
//        return -1;
//    }

    std::fprintf(stderr, "Using OpenGL version: %s\n", glGetString(GL_VERSION));

//    glfwEnable (GLFW_STICKY_KEYS);

    glClearColor(0.0f, 0.4f, 0.0f, 0.0f);

//    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
//    glEnable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//    Float sun_d[] = {7.0f, 7.0f, 7.0f, 1.0f};
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_d);
//    glEnable(GL_LIGHT0);

//    glClearDepth(1.0);
    glClear(GL_DEPTH_BUFFER_BIT);

    SolarSystem solarsystem;
//    solarsystem.add_planet(1.0f, 1.0f, 0.016f, SIDERIAL_YEAR, 1.0f, 0.0f, EARTH_AXIS_INCLINATION, 0.0f, 0.0f, "textures/earth.bmp", -M_PI);
//    solarsystem.set_sun(   EARTH_RADIUS * 5,   0.0f,  0.0f,   1.0f,          25.0f, 0.0f,  0.0f,                   0.0f,   0.0f,  "textures/sun.bmp",  -M_PI);
////    solarsystem.add_planet(EARTH_RADIUS,        1.0f,  0.016f, SIDERIAL_YEAR, 1.0f,  0.0f,  EARTH_AXIS_INCLINATION, 0.0f,   0.0f,  "textures/earth.bmp",-M_PI);
//    auto &earth = solarsystem.add_planet(EARTH_RADIUS,        1.0f,  0.016f, SIDERIAL_YEAR, 1.0f,  0.0f,  EARTH_AXIS_INCLINATION, 0.0f,   0.0f,  "textures/earth.bmp",-M_PI);
//    earth.add_moon(        MOON_RADIUS,         MOON_ORBIT_RADIUS, 0.05f, SIDERIAL_MONTH, SIDERIAL_MONTH, MOON_INCLINATION, 6.68f, 0.0f, 0.0f, "textures/moon.bmp", 0.0f);
//    solarsystem.add_planet(EARTH_RADIUS * 0.53, 1.52f, 0.09f,  686.9f,        1.02f, 1.85f, 25.19f,                 49.5f,  286.5, "textures/mars.bmp", -M_PI); // Mars

    solarsystem.set_sun(sun_data);
    auto &earth = solarsystem.add_planet(planets_data[0]);
    earth.add_moon(moon_data);
    solarsystem.add_planet(planets_data[1]);


    last_time = SDL_GetTicks();
    unsigned dt = 10, delta = 0;
    unsigned last_fps_print = last_time;

    bool running = true;
    bool print_fps = (argc >= 2 && argv[1][0] == 'f');
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                        reshape(event.window.data1, event.window.data2);
                    else if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                        running = false;
                    break;
                case SDL_KEYDOWN:
                    keyboard(window, event.key.keysym.scancode);
                    break;
            }
        }

        renderScene(solarsystem);
    
        unsigned time = SDL_GetTicks();
        delta += time - last_time;
        while (delta >= dt) { // Maintain constant physics step and free framerate
            solarsystem.move(dt);
//            solarsystem.move(50);
            delta -= dt;
        }
        last_time = time;
//        solarsystem.move(30);
        frames++;
        if (time - last_fps_print >= 1000) {
            if (print_fps)
                std::fprintf(stderr, "FPS: %f frame/s\n", (float)frames);
            last_fps_print += 1000;
            frames = 0;
        }
//        glfwPollEvents();
        SDL_GL_SwapWindow(window);
    } 


//    glfwDestroyWindow(window);
//    glfwTerminate();
    
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);

    return 0;
}
