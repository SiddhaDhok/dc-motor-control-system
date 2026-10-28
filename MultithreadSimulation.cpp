#include "MultithreadSimulation.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

ThreadedSimulation::ThreadedSimulation(Motor& m, Controller& c, Output& o, SimulationResult& r,double target, double dt): motor(m), controller(c), out(o), result(r),target(target), dt(dt), tol(0.02 * target) {}

void ThreadedSimulation::log(const char* who, int tick, double a, double b) 
{
	lock_guard<mutex> g(printMtx);
	cout<<"["<<who<<"] tick "<<tick<<"  "<<a<<"  "<<b<<endl;
}

void ThreadedSimulation::controllerLoop() 
{
	int tick=0;
	while (running) {
		double p, e;
		{
			lock_guard<std::mutex> g(mtx);
			p=controller.step(target, dt);
			motor.setPower(p);
			e=controller.getError();
		}
		if (tick%10==0) log("controller", tick, e, p);
		tick++;
		this_thread::sleep_for(chrono::milliseconds(sleepMs));
	}
}

void ThreadedSimulation::motorLoop() {
	int tick=0;
	while (running) {
		double s;
		{
			lock_guard<mutex> g(mtx);
			motor.update(dt);
			s=motor.getSpeed();
		}
		if (tick%10==0)
		{
			log("motor     ", tick, s, 0);
		}
		tick++;
		this_thread::sleep_for(chrono::milliseconds(sleepMs));
	}
}

void ThreadedSimulation::outputLoop() 
{
	int k=0, inBand=0;
	while (running) {
		double s, p;
		{
			lock_guard<mutex> g(mtx);
			s=motor.getSpeed();
			p=controller.getPower();
		}
		double t=k*dt;
		out.record(t, target, s, p);
		result.addReading(t, target, s, p);
		if (k%10==0)
		{
			log("output    ", k, s, p);
		}
		if (fabs(target-s)<=tol)
		{
			inBand++;
		}
		else
		{
			inBand=0;
		}
		if (inBand>=holdSteps) 
		{ 
			settled=true; running=false; 
		}
		else if (++k>=maxSamples)
		{
			running=false;
		}
		this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
	}
}
bool ThreadedSimulation::run() {
	running = true;
	thread tc(&ThreadedSimulation::controllerLoop, this);
	thread tm(&ThreadedSimulation::motorLoop, this);
	thread to(&ThreadedSimulation::outputLoop, this);
	tc.join(); tm.join(); to.join();
	return settled;
}
