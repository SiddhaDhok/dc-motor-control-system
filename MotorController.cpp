#include <bits/stdc++.h>

class Controller{
	private:
		float Ki;
		float Kd;
		float Kp;
		float time;   //time between measurements
	protected:
		Controller(float i, float d, float p, float t): Ki(i), Kd(d), Kp(p), time(t){}
		double SumE;
		double ep;
		double GetP(double target, double sp, double in){
			double NS;
			double e=target-sp;
			double P=Kp*e+Ki*SumE+Kd*(e-ep);
			ep=e;
			SumE+=e;
			return P;
		}       //Reference: https://industrialmonitordirect.com/blogs/knowledgebase/understanding-pid-control-kp-ki-kd-parameters-explained
};

class motor: public Controller{
	private:
		double speed;
		double inertia;
		double f(double sp, double pow);
		double NewSpeed(double P){
			double NS2;
			double s2=speed*speed;
			NS2=((2*P*t)/inertia)+s2;
			double NS=sqrt(NS2);
			return NS;
		}
	public:
		motor(double m, float i, float d, float p,float t): speed(0), inertia(m), Controller(i,d,p,t){}
		void ChangeSpeed(double s){
			SumE=0;
			ep=0;
			while(speed!=s){
				double P=GetP(speed,s,inertia);
				speed=NewSpeed(P);
			}
		}
		double GetSpeed(){ return speed;}
		double GetInertia(){ return inertia;}
};
