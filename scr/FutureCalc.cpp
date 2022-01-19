#include <iostream>

#include "Vars.h"

#include <vector>
#include <array>
#include <thread>
#include <chrono>
#include <mutex>



std::vector<std::array<int, 2>> getNextPos(int, int); 



extern std::vector<Body> bodies;
extern std::vector<std::array<int, 2>> futurePos;
extern bool running;

extern std::mutex mtx; 

using namespace std::chrono_literals;

void futureCalcLoop() {
	while (running) {
		std::this_thread::sleep_for(16ms);

		if (bodies.size() > 0) {
			if (bodies[bodies.size() - 1].movable) {
				mtx.lock(); 
				futurePos = getNextPos(bodies.size()-1, 1000); 
				mtx.unlock(); 
			}
		}

	}
}

std::vector<std::array<int, 2>> getNextPos(int bodyPos, int anz) {

	std::vector<Body> copy(bodies); 

	std::vector<std::array<int, 2>> fp(anz); 

	for (int i = 0; i < anz; i++) {
		for (int i2 = 0; i2 < copy.size(); i2++) {
			if (copy[i2].movable) {
				for (int i3 = 0; i3 < copy.size(); i3++) {
					if (i3 != i2) {


						double a = copy[i2].pos[0] - copy[i3].pos[0];
						double b = copy[i2].pos[1] - copy[i3].pos[1];

						double c = sqrt(a * a + b * b);

						if (c > 0.1) {

							double gForce = (200) * (((double)copy[i2].mass * (double)copy[i3].mass) / (c * c));//6.67408e-11

							copy[i2].vel[0] += -(gForce * ((double)a / ((double)abs(a) + (double)abs(b)))) / (double)copy[i2].mass; //-(gForce * ((double)a/((double)Math.abs(a)+(double)Math.abs(b))))/(double)b1.mass;
							copy[i2].vel[1] += -(gForce * ((double)b / ((double)abs(a) + (double)abs(b)))) / (double)copy[i2].mass; //-(gForce * ((double)b/((double)Math.abs(a)+(double)Math.abs(b))))/(double)b1.mass;
						}

					}
				}
			}
		}

		for (int i2 = 0; i2 < copy.size(); i2++) {
			if (copy[i2].movable) {
				copy[i2].pos[0] += copy[i2].vel[0]; 
				copy[i2].pos[1] += copy[i2].vel[1]; 
			}
		}

		fp[i][0] = (int)copy[bodyPos].pos[0]; 
		fp[i][1] = (int)copy[bodyPos].pos[1]; 

	}

	
	return fp;
}
