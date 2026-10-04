#include "MotorController.h"
#include "output.h"
using namespace std;

int main() {
    double target, inertia, Kp, Ki, Kd, dt;
    cout<<"Enter target speed: ";
    cin>>target;
    cout<<"Enter motor inertia: ";
    cin>>inertia;
    cout<<"Enter Kp, Ki, Kd (space-separated): ";
    cin>>Kp>>Ki>>Kd;
    cout<<"Enter time step dt (e.g. 0.01): ";
    cin>>dt;

    motor m(inertia, Kp, Ki, Kd, dt);
    CSVOutput csv("sim_results.csv");
    SimulationResult result;

    m.ChangeSpeed(target, csv, result);

    cout<<"\nSimulation complete. Final speed: "<<m.GetSpeed()<<endl;
    result.print();

    return 0;
}
