#include <iostream>

#include "Vars.h"

#include <vector>
#include <array>
#include <mutex>

#include <GL/glew.h>
#include <GLFW/glfw3.h>





// Function Declarations
void init(); 
void drawLoop();

void draw(); 
void events(); 

void drawRect(const float x, const float y, const float w, const float h);
void drawCircle(float, float, float, int); 

void getCursorPos(int (&a)[2]); 



// Variables
GLFWwindow* window;

const int width = 2250; 
const int height = 1500; 
 
extern std::vector<Body> bodies;
extern std::vector<std::array<int, 2>> futurePos;
extern bool running; 

double globalVel = 0.5; 

std::mutex mtx; 


// Game Functions
void drawLoop() {

    // Start of programm
    init();

    while (!glfwWindowShouldClose(window)) {

        events(); 

        draw(); 

        /* Poll for and process events */
        glfwPollEvents();
    }

    // End of programm
    
    running = false; 

    glfwTerminate();
}









// Game Utility Functions
void draw() {

    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < bodies.size(); i++) {
        drawCircle(bodies[i].pos[0], bodies[i].pos[1], bodies[i].radius, 10);
    }

    mtx.lock(); 
    for (int i = 0; i < futurePos.size(); i++) {
        drawCircle(futurePos[i][0], futurePos[i][1], 2, 10);
    }
    mtx.unlock(); 

    glfwSwapBuffers(window);
}


void events() {
    static char pressed = ' '; 

    if (glfwGetMouseButton(window, 0) == GL_TRUE) {

        static int clickPos[2]; 

        if (pressed == ' ') { 

            getCursorPos(clickPos); 

            bodies.push_back(Body(clickPos[0], clickPos[1], 30, 100, true));
            bodies[bodies.size() - 1].isBeingCreated = true; 
            pressed = 'l';
        }

        int pos[2]; 
        getCursorPos(pos); 
        bodies[bodies.size() - 1].vel[0] = ((double)clickPos[0] - (double)pos[0]) / 50.0; 
        bodies[bodies.size() - 1].vel[1] = ((double)clickPos[1] - (double)pos[1]) / 50.0; 

    }
    else {
        if (glfwGetMouseButton(window, 1) == GL_TRUE) {
            if (pressed == ' ') {
                int pos[2];

                getCursorPos(pos);

                bodies.push_back(Body(pos[0], pos[1], 30, 100, false));
                futurePos.clear(); 
                pressed = 'r';

            }
        }
        else {
            if (pressed == 'l') {
                int currentPos[2]; 
                getCursorPos(currentPos); 
                bodies[bodies.size() - 1].isBeingCreated = false; 
            }
            pressed = ' ';

        }
    }


    if (bodies.size() > 0) {
        if (glfwGetKey(window, 'W') == GL_TRUE) {
            bodies[bodies.size() - 1].vel[1] -= globalVel; 
        }

        if (glfwGetKey(window, 'S') == GL_TRUE) {
            bodies[bodies.size() - 1].vel[1] += globalVel; 
        }

        if (glfwGetKey(window, 'A') == GL_TRUE) {
            bodies[bodies.size() - 1].vel[0] -= globalVel; 
        }

        if (glfwGetKey(window, 'D') == GL_TRUE) {
            bodies[bodies.size() - 1].vel[0] += globalVel; 
        }


        if (glfwGetKey(window, 'C') == GL_TRUE) {
            if (!bodies[bodies.size() - 1].isBeingCreated) {
                mtx.lock();
                bodies.clear();
                futurePos.clear();
                mtx.unlock();
            }
        }


        int move[2] = { 0, 0 };

        if (glfwGetKey(window, GLFW_KEY_UP) == GL_TRUE) {
            move[0] = 1; 
            move[1] = 20; 
        }
        else {
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GL_TRUE) {
                move[0] = 1;
                move[1] = -20;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GL_TRUE) {
            move[0] = 0; 
            move[1] = -20; 
        }
        else {
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GL_TRUE) {
                move[0] = 0; 
                move[1] = 20; 
            }
        }

        for (int i = 0; i < bodies.size(); i++) {
            bodies[i].pos[move[0]] += move[1]; 
        }

    }
}









// Drawing Utility
void drawRect(const float x, const float y, const float w, const float h) {
    glBegin(GL_QUADS); 
        glColor4f(1.0, 1.0, 1.0, 1.0);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd(); 
}


void drawCircle(float x, float y, float r, int num_segments) {
    float i = 0.0f;

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(x, y); // Center
    for (i = 0.0f; i <= num_segments; i++)
        glVertex2f(r * cos(3.141592 * (double)i / ((double)num_segments/2.0)) + x, r * sin(3.141592 * (double)i / ((double)num_segments / 2.0)) + y);

    glEnd();
}






// Functional Utility
void getCursorPos(int (&a)[2]) {

    double x; 
    double y; 

    glfwGetCursorPos(window, &x, &y);
    
    a[0] = (int)x; 
    a[1] = (int)y; 
}






// Single Time Functions
void init() {

    /* Initialize the library */
    if (!glfwInit()) {
        throw "Failed to initialize";
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "GravitySim", NULL, NULL);
    if (!window){
        glfwTerminate();
        throw "Failed to create window"; 
    }


    /* Make the window's context current */
    glfwShowWindow(window);
    glfwMakeContextCurrent(window);

    glewInit();

    glOrtho(0.f, width, height, 0.f, 0.f, 1.f);

    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
}