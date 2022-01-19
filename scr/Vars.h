#pragma once

#include <vector>
#include <array>

volatile struct Body {
    double pos[2];
    int mass;
    int radius;
    double vel[2] = { 0.0, 0.0 };
    bool movable = false;
    std::vector<std::array<double, 2>> acc;
    bool isBeingCreated = false;

    Body(int x, int y, int radius, int mass, bool movable) {
        this->pos[0] = x;
        this->pos[1] = y;
        this->radius = radius;
        this->mass = mass;
        this->movable = movable;
    }

    Body(int x, int y, int radius, int mass, double setVel[2]) {
        this->pos[0] = x;
        this->pos[1] = y;
        this->radius = radius;
        this->mass = mass;

        this->vel[0] = setVel[0];
        this->vel[1] = setVel[1];
    }
};