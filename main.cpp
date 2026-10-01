#include<iostream>
#include "motor.h"
#include "controller.h"
#include "CSVOutput.h"
using namespace std;

int main()
{
    double target=0; //target speed
    double dt=0; //step
    double totalTime=0 //duration of simulation
    double loadStartTime=0 //time when disturbance starts
    double loadValue=0 //disturbance size

    Motor motor;
    Controller controller; // Kp/Ki/Kd  inside Controller's constructor
    CSVOutput output("simulation_res.csv");
    int steps=static_cast<int>(totalTime/dt);
    for(int i=0; i<=steps; i++)
    {
        double t=i*dt;
        if(t>=loadStartTime)
        {
            motor.setLoad(loadValue);
        }
        double speed=motor.getSpeed();
        double power=controller.step(target, speed, dt);
        motor.update(power, dt);
        output.record(t, target, speed, power);
    }
    return 0;
}