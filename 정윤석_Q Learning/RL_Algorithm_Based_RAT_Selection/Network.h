#pragma once
#include <stdio.h>

class Network
{
private:
	int User_Count;
	double Bandwidth;
	double Throughput;
	double Utility;

	int Area_User_Count[3];
	double Area_Bandwidth[3];

public:
	Network()
	{
		for (int i = 0; i < 3; i++) {
			setArea_Bandwidth(0, i);
			setArea_User_Count(0, i);
		}
	}

	Network(int User_Count, int Bandwidth){
		setUser_Count(User_Count);
		setBandwidth(Bandwidth);
	}

	~Network(){


	}

	void setBandwidth(double Bandwidth) { this->Bandwidth = Bandwidth; }
	double getBandwidth() { return Bandwidth; }

	void setThroughput(double Throughput) { this->Throughput = Throughput; }
	double getThroughput() { return Throughput; }
	
	void setUtility(double Utility) { this->Utility = Utility; }
	double getUtility() { return Utility; }
	
	void setUser_Count(int User_Count) { this->User_Count = User_Count; }
	int getUser_Count() { return User_Count; }

	void setArea_User_Count(int Area_User_Count, int i) { this->Area_User_Count[i] = Area_User_Count; }
	int getArea_User_Count(int i) { return Area_User_Count[i]; }

	void setArea_Bandwidth(double Area_Bandwidth, int i) { this->Area_Bandwidth[i] = Area_Bandwidth; }
	double getArea_Bandwidth(int i) { return Area_Bandwidth[i]; }
};

