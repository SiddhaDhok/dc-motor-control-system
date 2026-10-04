#pragma once
#include <bits/stdc++.h>

class Controller{
	private:
		float Ki;
		float Kd;
		float Kp;
	protected:
		float time;   //time between measurements
		Controller(float i, float d, float p, float t): Ki(i), Kd(d), Kp(p), time(t){}
		double SumE=0;
		double ep=0;
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
			NS2=((2*P*time)/inertia)+s2;
			if(NS2<0) NS2=0;
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
		double Step(double target){
			double P=GetP(target,speed,inertia);
			speed=NewSpeed(P);
			return P;
		}
		double GetSpeed(){ return speed;}
		double GetInertia(){ return inertia;}
};
