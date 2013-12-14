
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
#include <deque>
#include <cstdint>
#include <random>
#include <algorithm>
#include <functional>
#include <unordered_set>


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
static double  day_per_second = 1.0; static std::size_t speed = 16;
static bool orbits = true;
static bool moving = true, spinning = true;
static unsigned last_time = 0, frames = 0;
//static SDL_Window *window = NULL;
//static TTF_Font *font = NULL;

static mat4 ProjectionMatrix;


void printState() {
    std::fprintf(stderr, "xpos = %f, ypos = %f, zpos = %f\n", xpos, ypos, zpos);
    std::fprintf(stderr, "sight_z = %f, sight_y = %f, sight_z = %f\n", sight_x, sight_y, sight_z);
    std::fputc('\n', stderr);
}

void renderScene(const SolarSystem &solarsystem, const Background &sky) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

    solarsystem.render(ModelMatrix, ViewMatrix, MVP, orbits);
//    glDepthFunc(GL_NEVER);
    sky.render();
//    glDepthFunc(GL_LESS);
}

void reshape(int w, int h) {
    if (!h) h = 1;
    glViewport(0, 0, (GLint) w, (GLint) h);
    ProjectionMatrix = glm::perspective(45.0f, (float) w/h, 0.0001f, 20.0f);
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
            speed = 16, day_per_second = 1;
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
        case SDL_SCANCODE_M:
            moving = !moving;
            break;
        case SDL_SCANCODE_P:
            spinning = !spinning;
            break;
        case SDL_SCANCODE_KP_PLUS:
            log_speed = 0, tmp_speed = speed;
            while (tmp_speed) tmp_speed >>= 1, ++log_speed;
            if (speed < 1600) {
                speed += log_speed;
                day_per_second = speed / 16.0;
            }
            break;
        case SDL_SCANCODE_KP_MINUS:
            log_speed = 0, tmp_speed = speed;
            while (tmp_speed) tmp_speed >>= 1, ++log_speed;
            if (speed > log_speed) {
                speed -= log_speed;
                day_per_second = speed / 16.0;
            }
            break;
        default:
            return;
    }
//    printState();
}

int main(int argc, char **argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
//    TTF_Init();
    std::atexit(SDL_Quit);
#ifdef USE_OPENGLES
    SDL_SetHintWithPriority(SDL_HINT_RENDER_DRIVER, "opengles2", SDL_HINT_OVERRIDE);
#endif
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

    std::fprintf(stderr, "Using OpenGL version: %s\n", glGetString(GL_VERSION));

    glClearColor(0.0f, 0.4f, 0.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClear(GL_DEPTH_BUFFER_BIT);


    ///////////////////////////////////////
    // Init objects

    SolarSystem solarsystem;
    Background sky("textures/starmap");
//    Text2DPrinter text_printer("Vera.ttf", 10);
//    sky._texture = text_printer.render_text2D("hello world", 3, 4);

    last_time = SDL_GetTicks();
    unsigned dt = 10, delta = 0;
    unsigned last_fps_print = last_time;

    bool running = true;
    bool print_fps = (argc >= 2 && argv[1][0] == 'f');
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
            }
        }

        renderScene(solarsystem, sky);
    
        unsigned time = SDL_GetTicks();
        delta += time - last_time;
        solarsystem.physical_step(((delta/dt)*dt) * day_per_second, moving, spinning);
        delta %= dt;
//        while (delta >= dt) { // Maintain constant physics step and free framerate
//            solarsystem.physical_step(dt * day_per_second, moving, spinning);
//            delta -= dt;
//        }
        last_time = time;
        frames++;
        if (time - last_fps_print >= 1000) {
            if (print_fps)
                std::fprintf(stdout, "FPS: %f frame/s\n", (float)frames);
//            std::snprintf(message, 100, "FPS: %f frame/s\n", (float)frames);
//            glDeleteTextures(1, &sky._texture);
//            sky._texture = text_printer.render_text2D(message, 0, 0);
            last_fps_print += 1000;
            frames = 0;
        }
//        text_printer.render_text2D("A", 0, 0);
//          drawText("sfjklsdgafjgsdjskfjkadsfla", font, 20, 20);
//        SDL_Color color = { 255, 255, 255 };
//        SDL_Surface *surf = TTF_RenderText_Blended(font, message, color);
//        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
//        SDL_RenderCopy(renderer, texture, NULL, NULL);
//        SDL_RenderPresent(renderer);
//
//        SDL_FreeSurface(surf);
//        SDL_DestroyTexture(texture);

        SDL_GL_SwapWindow(window);
    } 

    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);

    return 0;
}
