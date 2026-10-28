#ifndef MOTOR_H
#define MOTOR_H

#include <cmath>
#include <random>
using namespace std;

class Motor {
	private:
		double speed;
		double inertia;
		double power;
		double SR(double x);
		double NewSpeed(double P, double dt);
	public:
		Motor(double m);
		mt19937 gen{42};
		uniform_int_distribution<int> dist{0,5};

		void setPower(double P);
		void update(double dt);
		double getSpeed();
		double GetSpeed();
		double GetInertia();
};

#endif