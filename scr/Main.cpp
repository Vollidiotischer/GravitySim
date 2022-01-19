#include <iostream>

#include "Vars.h"

#include <thread>
#include <vector>
#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


void drawLoop();
void calcLoop(); 
void futureCalcLoop(); 


std::vector<Body> bodies; 
std::vector<std::array<int, 2>> futurePos; 

bool running = true; 

int main() {

    std::thread t1(drawLoop);

    std::thread t2(calcLoop); 

    std::thread t3(futureCalcLoop); 

    t1.join(); 

    t2.join(); 

    t3.join(); 

    return 0;
}



/*

forcelines

*/





/*
Code Graveyard


void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++) {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component
        glVertex2f(x + cx, y + cy);//output vertex
    }
    glEnd();
}



*/