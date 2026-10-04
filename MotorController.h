#ifndef MotorController_H
#define MotorController_H

class Controller{
	private:
		float ki;
		float Kd;
		float Kp;
	protected:
		float time;
		double SumE;
		double ep;
		Controller(float p, float i, float d, float t);
		double GetP(double target, double sp, double in);
};

class motor: public Controller{
	private:
		double speed;
		double inertia;
		double SR(double x);
		double NewSpeed(double P);
	public:
		motor(double m, float p, float i, float d, float t);
		void ChangeSpeed(double s);
		double GetSpeed();
		double GetInertia();
};

#endif
