
// windows.h must be included before GL headers
#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#include <windows.h>
#endif

//#include <GL/glew.h>
#include <GLES2/gl2.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>
#include <cstdint>
#include <random>
#include <cstdlib>
#include <cstring>
#include <algorithm>

#include "common/text2D.hpp"
#include "config.hh"
//#include "constants.h"
#include "graphics/background.hh"
//#include "graphics/text2Dprinter.hh"
#include "objects/solarsystem.hh"
//#include "rendering.h"

static double xpos = -10.979846, ypos = 5.430511, zpos = 10.554169;
//static GLfloat sight_x = 0.0f, sight_y = -0.43f, sight_z = -0.9f;
static double sight_x = 0.3f, sight_y = -0.30f, sight_z = -0.3f;
static double  zoom_step = 0.08;
static double up_x = 0.0f, up_y = 1.0f;
static double  cam_xz_angle = 0.0f, cam_y_angle = -0.45f;
static double  cam_z_angle = M_PI_2;
static double  seconds_per_day = 1.0; static std::size_t speed = 16;
static bool orbits = true;
static bool display_stat = true;
static bool moving = true, spinning = true;
static unsigned last_time = 0, frames = 0;
//static SDL_Window *window = NULL;
//static TTF_Font *font = NULL;

static mat4 ProjectionMatrix;


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////// Rendering functions
/////////////////////////////////////////////////////////////////////////////////////////////////

void printState() {
    std::fprintf(stderr, "xpos = %f, ypos = %f, zpos = %f\n", xpos, ypos, zpos);
    std::fprintf(stderr, "sight_z = %f, sight_y = %f, sight_z = %f\n", sight_x, sight_y, sight_z);
    std::fputc('\n', stderr);
}

void renderStat(SDL_Window *window, std::size_t frames)
{
    if (!display_stat)
        return;

    char message[100];

    int width, height;
    int fontsize = 15;
    SDL_GetWindowSize(window, &width, &height);

    // FPS
    std::snprintf(message, 100, "Framerate: %lu FPS", frames);
    printText2D(message, 10, height - fontsize, fontsize, width, height);

    // Eye
    std::snprintf(message, 100, "Eye possition: (%.2f, %.2f, %.2f)", xpos, ypos, zpos);
    printText2D(message, 10, height - 2*fontsize, fontsize, width, height);

    // Zoom step
    std::snprintf(message, 100, "Zoom step: %.5f", zoom_step);
    printText2D(message, 10, height - 3*fontsize, fontsize, width, height);

    // Speed
    std::snprintf(message, 100, "Speed: %lu (1 second = %.3f days)", speed, seconds_per_day);
    printText2D(message, 10, height - 4*fontsize, fontsize, width, height);
}

void renderHelp(SDL_Window *window)
{
    
}

void renderScene(const SolarSystem &solarsystem, const Background &sky) {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glLoadIdentity();
    mat4 ModelMatrix, ViewMatrix, MVP;

//    gluLookAt(xpos,           ypos,           zpos,
//              xpos + sight_x, ypos + sight_y, zpos + sight_z,
//              up_x,           up_y,           0.0f);

    ModelMatrix = mat4(1.0);

    ViewMatrix = glm::lookAt(vec3(xpos, ypos, zpos),
                             vec3(xpos+sight_x, ypos+sight_z, zpos+sight_z),
                             vec3(up_x, up_y, 0.0f));
//    ViewMatrix = glm::lookAt(vec3(0., 0., 1.),
//                             vec3(0., 0., 0.),
//                             vec3(0., 1., 0.));

//    ProjectionMatrix = glm::perspective(45.0f, (float) 800/600, 0.1f, 50.0f);
    
//    drawAxes(ViewMatrix, MVP);

    MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

    // TODO: draw axes

    sky.render();
    solarsystem.render(ModelMatrix, ViewMatrix, MVP, orbits);
}

void reshape(int w, int h) {
    if (!h) h = 1;
    glViewport(0, 0, (GLint) w, (GLint) h);
    ProjectionMatrix = glm::perspective(45.0f, (float) w/h, 0.0001f, 50.0f);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////// Input handling
/////////////////////////////////////////////////////////////////////////////////////////////////

void mouse(SDL_Window *window, SDL_Event const &event) {
    static Sint32 last_x = 0, last_y = 0;
    Sint32 dx, dy;

    switch(event.type) {
        case SDL_MOUSEWHEEL:
            xpos += sight_x * event.wheel.y * zoom_step;
            ypos += sight_y * event.wheel.y * zoom_step;
            zpos += sight_z * event.wheel.y * zoom_step;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                last_x = event.button.x;
                last_y = event.button.y;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                dx = event.button.x - last_x;
                dy = event.button.y - last_y;
                int width, height;
                SDL_GetWindowSize(window, &width, &height);
                xpos -= 20 * dx / (float)width;
                ypos += 20 * dy / (float)height;
            }
            break;
        default:
            break;
    }
}

void keyboard(SDL_Window *window, SDL_Scancode scancode) {
    Uint32 flags;
    std::size_t log_speed, tmp_speed;

    switch (scancode) {
//        case SDL_SCANCODE_Q:
//            glfwSetWindowShouldClose(window, GL_TRUE);
//            break;
        case SDL_SCANCODE_W:
            xpos += sight_x * zoom_step;
            ypos += sight_y * zoom_step;
            zpos += sight_z * zoom_step;
            break;
        case SDL_SCANCODE_S:
            xpos -= sight_x * zoom_step;
            ypos -= sight_y * zoom_step;
            zpos -= sight_z * zoom_step;
            break;
        case SDL_SCANCODE_A:
            xpos -= -sight_z * zoom_step;
            zpos -= sight_x * zoom_step;
            break;
        case SDL_SCANCODE_D:
            xpos += -sight_z * zoom_step;
            zpos += sight_x * zoom_step;
            break;
        case SDL_SCANCODE_Z:
            ypos += -sight_z * zoom_step;
            zpos += sight_y * zoom_step;
            break;
        case SDL_SCANCODE_X:
            ypos -= -sight_z * zoom_step;
            zpos -= sight_y * zoom_step;
            break;
        case SDL_SCANCODE_EQUALS:
            zoom_step *= 1.1;
            break;
        case SDL_SCANCODE_MINUS:
            zoom_step /= 1.1;
            break;
        case SDL_SCANCODE_R:
//            xpos = 0.0f, ypos = 1.0f, zpos = 2.5f;
            xpos = -10.979846, ypos = 5.430511, zpos = 10.554169;
//            sight_x = 0.0f, sight_y = -0.43f, sight_z = -0.9f;
            sight_x = 0.3, sight_y = -0.30, sight_z = -0.3;
            up_x = 0.0f, up_y = 1.0f;
            cam_xz_angle = 0.0f, cam_y_angle = -0.45f;
            cam_z_angle = M_PI_2;
            zoom_step = 0.08;
            speed = 16, seconds_per_day = 1;
            break;
        case SDL_SCANCODE_F:
            flags = SDL_GetWindowFlags(window);
            SDL_SetWindowFullscreen(window, flags ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
            break;
        case SDL_SCANCODE_O:
            orbits = !orbits;
            break;
        case SDL_SCANCODE_T:
            display_stat = !display_stat;
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
        case SDL_SCANCODE_M:
            moving = !moving;
            break;
        case SDL_SCANCODE_P:
            spinning = !spinning;
            break;
        case SDL_SCANCODE_KP_PLUS:
            log_speed = 0, tmp_speed = speed;
            while (tmp_speed) tmp_speed >>= 1, ++log_speed;
//            if (speed < 1600) {
                speed += log_speed;
                seconds_per_day = speed / 16.0;
//            }
            break;
        case SDL_SCANCODE_KP_MINUS:
            log_speed = 0, tmp_speed = speed;
            while (tmp_speed) tmp_speed >>= 1, ++log_speed;
            if (speed > log_speed) {
                speed -= log_speed;
                seconds_per_day = speed / 16.0;
            }
            break;
        default:
            return;
    }
//    printState();
}


////////////////////////////////////////////////////////////////////////////////////
///////// main

int main(int argc, char **argv) {
    /////////////////////////////////////////
    ///// Init SDL2

    SDL_Init(SDL_INIT_EVERYTHING);
//    TTF_Init();
    std::atexit(SDL_Quit);

#ifdef USE_OPENGLES
    SDL_SetHintWithPriority(SDL_HINT_RENDER_DRIVER, "opengles2", SDL_HINT_OVERRIDE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#endif
    int multisampling = 4;
    if (argc > 2 && !std::strcmp(argv[1], "-m")) {
        multisampling = std::atoi(argv[2]);
    }
    if (multisampling > 0)
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, multisampling);

    SDL_Window *window = SDL_CreateWindow("Solar system", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 400, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
    reshape(800, 400); // SDL does not send resize event on startup

//    TTF_Font *font = TTF_OpenFont("Vera.ttf", 16);
//    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

//    if (glewInit() != GLEW_OK) {
//        std::fputs("Failed to initialize GLEW\n", stderr);
//        return -1;
//    }

    /////////////////////////////////////////
    ///// Init OpenGL

    std::fprintf(stderr, "Using OpenGL version: %s\n", glGetString(GL_VERSION));

    glClearColor(0.0f, 0.4f, 0.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClear(GL_DEPTH_BUFFER_BIT);


    ///////////////////////////////////////
    // Init objects
    SolarSystem solarsystem;
    Background sky("textures/starmap");
//    Text2DPrinter text_printer("Vera.ttf", 10);
//    sky._texture = text_printer.render_text2D("hello world", 3, 4);

    // Initialize little text library with the Holstein font
    initText2D( "textures/LinuxBiolinum.png" );
 

    ///////////////////////////////////////////////////
    ///////  Render loop

    last_time = SDL_GetTicks();
    unsigned dt = 10, delta = 0;
    unsigned last_fps_print = last_time, last_fps = 0;

    bool running = true;
//    bool print_fps = (argc >= 2 && argv[1][0] == 'f');
//    char message[100] = "FPS = sfdkasfjhkahdsflahglashdhs";
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
                case SDL_MOUSEWHEEL:
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEMOTION:
                    mouse(window, event);
                    break;
            }
        }

        renderScene(solarsystem, sky);
    
        unsigned time = SDL_GetTicks();
        delta += time - last_time;
        solarsystem.physical_step(((delta/dt)*dt/1000.0) * seconds_per_day, moving, spinning);
        delta %= dt;
//        while (delta >= dt) { // Maintain constant physics step and free framerate
//            solarsystem.physical_step(dt * seconds_per_day, moving, spinning);
//            delta -= dt;
//        }
        last_time = time;
        frames++;
        if (time - last_fps_print >= 1000) {
            last_fps = frames;
//            std::snprintf(message, 100, "FPS: %.2f frame/s\n", (float)frames);
//            if (print_fps)
//                std::fputs(message, stderr);
            last_fps_print += 1000;
            frames = 0;
        }
        renderStat(window, last_fps);

        SDL_GL_SwapWindow(window);
    }

    ///////////////////////////////////////////////
    ///// Clean up
    cleanupText2D();
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);

    return 0;
}
