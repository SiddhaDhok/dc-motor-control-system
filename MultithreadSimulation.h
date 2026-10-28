#ifndef THREADEDSIMULATION_H
#define THREADEDSIMULATION_H

#include <atomic>
#include <mutex>
#include "Controller.h"
#include "Motor.h"
#include "output.h"

class ThreadedSimulation {
	private:
		Motor& motor;
		Controller& controller;
		Output& out;
		SimulationResult& result;
		double target, dt;
		const double tol;
		const int holdSteps=20;
		const int maxSamples=5000;
		const int sleepMs=10;
		std::mutex mtx;
		std::mutex printMtx;
		std::atomic<bool> running{false};
		std::atomic<bool> settled{false};
		void log(const char* who, int tick, double a, double b);
		void controllerLoop();
		void motorLoop();
		void outputLoop();
	public:
		ThreadedSimulation(Motor& m, Controller& c, Output& o, SimulationResult& r, double target, double dt);
		bool run();
};

#endif
