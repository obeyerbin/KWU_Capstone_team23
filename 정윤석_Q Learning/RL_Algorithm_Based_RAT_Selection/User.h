#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Network.h"

class User
{
private:
	int M;															// Number of Network 
	int Network_Number;
	int Area;
	double QValue;
public:
	double* QValue_Table;
	bool isFixed = false;
	int continuity;
public:
	User() {}

	~User() { freeQValue_Table(); }

	double* getQValue_table() { return QValue_Table; }
	void setQValue_Table()
	{
		continuity = 0;
		QValue_Table = (double*)malloc(sizeof(double)*M);
		for (int i = 0; i < M; i++)
		{
			QValue_Table[i] = 0;
		}
	}
	void setArea_Table(int A) {

		for (int i = 0; i < M; i++) {
			if (i > A)
				QValue_Table[i] = -1000;
		}
		//if (Area == 2)
		//	QValue_Table[1] = -1000;
	}

	void freeQValue_Table()
	{
		free(QValue_Table);
	}

	bool getifFixed() { return isFixed; }

	void setM(int M) { this->M = M; }
	int getM() { return M; }

	void setArea(int Area) { this->Area = Area; }
	int getArea() { return Area; }

	void setQValue(double QValue) { this->QValue = QValue; }
	double getQValue() { return QValue; }

	void setNetwork_Number(int Network_Number) { this->Network_Number = Network_Number; }
	int getNetwork_Number() { return Network_Number; }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int argmaxQValue_NetworkNumber() {
		double max_QValue = QValue_Table[0];
		int max_QValue_Network_Number = 0;

		for (int i = 0; i <= getArea(); i++) {
			if (max_QValue < QValue_Table[i])
			{
				max_QValue = QValue_Table[i];
				max_QValue_Network_Number = i;
			}
		}

		return max_QValue_Network_Number;
	}

	void update_QValue(Network** network, int chosen_Network_Number, double U_I, double P_I, double BETA, double LAMBDA, int* change_User_count[]) {
		double max_QValue = 0;
		double payoff = 0, temp_payoff = 0;

		if (chosen_Network_Number > getArea())
			chosen_Network_Number = getArea();

		payoff = net_Utility(*network[chosen_Network_Number], getArea(), chosen_Network_Number, U_I, P_I);


		max_QValue = QValue_Table[chosen_Network_Number];
		for (int i = 0; i <= getArea(); i++) {
			if (max_QValue < QValue_Table[i])
				max_QValue = QValue_Table[i];
		}

		setQValue(QValue_Table[chosen_Network_Number]);

		QValue_Table[chosen_Network_Number] = (1 - LAMBDA)*QValue_Table[chosen_Network_Number] + LAMBDA * (payoff + BETA * max_QValue);

		if (QValue_Table[chosen_Network_Number] - QValue <= -1000 || QValue_Table[chosen_Network_Number] - QValue >= 1000)
			isFixed = false;
		else
			isFixed = true;
		

		if (chosen_Network_Number != getNetwork_Number()) {
			network[chosen_Network_Number]->setUser_Count(network[chosen_Network_Number]->getUser_Count() + 1);
			network[getNetwork_Number()]->setUser_Count(network[getNetwork_Number()]->getUser_Count() - 1);

			network[chosen_Network_Number]->setArea_User_Count(network[chosen_Network_Number]->getArea_User_Count(getArea()) + 1, getArea());
			network[getNetwork_Number()]->setArea_User_Count(network[getNetwork_Number()]->getArea_User_Count(getArea()) - 1, getArea());
			setNetwork_Number(chosen_Network_Number);
			continuity = 0;
		}
		else
			continuity++;
	}

	double net_Utility(Network network, int Area, int chosen_Network_Number, double U_I, double P_I)			// PI_i = u(C_i/N) - p_i*N
	{
		double Capacity = 0;
		int n_ia = 0;
		
		for (int k = Area; k < 3; k++)
		n_ia += network.getArea_User_Count(k);
		
		// 1)
		
		if (network.getUser_Count() != 0)
			return U_I*(network.getBandwidth() / network.getUser_Count()) - (P_I*  n_ia);
		else
			return U_I * network.getBandwidth() - (P_I*  n_ia);
	}
};
