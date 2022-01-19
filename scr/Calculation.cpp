#include <iostream>

#include "Vars.h"

#include <vector>
#include <array>
#include <thread>
#include <chrono>



void calculate(Body&, Body&); 


extern std::vector<Body> bodies;
extern std::vector<std::array<int, 2>> futurePos;
extern bool running; 


using namespace std::chrono_literals; 

void calcLoop() {
	while (running) {
		std::this_thread::sleep_for(10ms);

		if (bodies.size() > 1) {
			for (int i = 0; i < bodies.size(); i++) {
				bodies[i].acc.clear();

				if (bodies[i].movable && !bodies[i].isBeingCreated) {
					for (int i2 = 0; i2 < bodies.size(); i2++) {
						if (i != i2 && !bodies[i2].isBeingCreated) {
							calculate(bodies[i], bodies[i2]); 
						}
					}
				}

			}
		}

		for (int i = 0; i < bodies.size(); i++) {
			if (!bodies[i].isBeingCreated) {
				bodies[i].pos[0] += bodies[i].vel[0]; 
				bodies[i].pos[1] += bodies[i].vel[1]; 
			}
		}

	}
}


void calculate(Body& b1, Body& b2) {
	double a = b1.pos[0] - b2.pos[0];
	double b = b1.pos[1] - b2.pos[1];

	double c = sqrt(a * a + b * b);

	if (c > 0.1) {
		double gForce = (200) * (((double)b1.mass * (double)b2.mass) / (c * c));//6.67408e-11 //0.00000001

		double acc[2];
		acc[0] = -(gForce * (a / (abs(a) + abs(b)))) / (double)b1.mass;
		acc[1] = -(gForce * (b / (abs(a) + abs(b)))) / (double)b1.mass;

		b1.vel[0] += acc[0]; //-(gForce * ((double)a/((double)Math.abs(a)+(double)Math.abs(b))))/(double)b1.mass;
		b1.vel[1] += acc[1]; //-(gForce * ((double)b/((double)Math.abs(a)+(double)Math.abs(b))))/(double)b1.mass;

		b1.acc.push_back({acc[0], acc[1]});
	}


}