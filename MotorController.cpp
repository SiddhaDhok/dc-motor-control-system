#include <bits/stdc++.h>
using namespace std;

class Controller{
	private:
		float Ki;
		float Kd;
		float Kp;
	protected:
		float time;   //time between measurements
		Controller(float p, float i, float d, float t): Ki(i), Kd(d), Kp(p), time(t){}
		double SumE;
		double ep;
		double GetP(double target, double sp, double in){
			double NS;
			double e=target-sp;
			double P=Kp*e+Ki*SumE+Kd*((e-ep)/time);
			ep=e;
			SumE+=e*time;
			return P;
		}       //Reference: https://industrialmonitordirect.com/blogs/knowledgebase/understanding-pid-control-kp-ki-kd-parameters-explained
};

class motor: public Controller{
	private:
		double speed;
		double inertia;
		float load;
		double f(double sp, double pow);
		double SR(double x){
			if (x>=0) return sqrt(x);
			return 0-sqrt(-x);
		}
		double NewSpeed(double P){
			double NS2;
			double s2=speed*speed;
			double DE=(2*P*time)/inertia;
			NS2=s2+DE;
			return SR(NS2);
		}
	public:
		motor(double m, float p, float i, float d,float t): speed(0), inertia(m), Controller(p,i,d,t){}
		void ChangeSpeed(double s){
			SumE=0;
			ep=0;
			while(s-speed>0.01 || speed-s>0.01){
				double P=GetP(s,speed,inertia);
				speed=NewSpeed(P);
				cout<<GetSpeed()<<endl;
			}
		}
		double GetSpeed(){ return speed;}
		double GetInertia(){ return inertia;}
};

int main(){
	motor m(50, 1, 0.1, 0.5, 1);
	m.ChangeSpeed(20);
	return 0;
}
