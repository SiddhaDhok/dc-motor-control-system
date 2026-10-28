#include "Motor.h"

Motor::Motor(double m): speed(0), inertia(m), power(0) {}

double Motor::SR(double x){
	if (x>=0) return sqrt(x);
	return sqrt(-x);
}

double Motor::NewSpeed(double P, double dt){
	double NS2;
	double s2=speed*speed;
	double DE=(2*P*dt)/inertia;
	NS2=s2+DE;
	if (NS2<0) return 0;
	return SR(NS2);
}

void Motor::setPower(double P){ power=P; }

void Motor::update(double dt){
	double load=(dist(gen))*speed/100;
	double P=power;
	P-=load*speed;
	speed=NewSpeed(P,dt);
}

double Motor::getSpeed(){ return speed; }
double Motor::GetSpeed(){ return speed; }
double Motor::GetInertia(){ return inertia; }