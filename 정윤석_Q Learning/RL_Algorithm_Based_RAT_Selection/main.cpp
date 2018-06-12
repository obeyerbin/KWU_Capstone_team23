#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "User.h"
#include "Network.h"
#pragma warning (disable:4996)

// Parameter Set
#define NETWORK_COUNT 3													// M,	 1. WMAN, 2. cellular netwoark, 3. WLAN
#define N_USER 50														// N,	 N1 = 10, N2 = 10, N3 = 30
#define P_I 0.01
#define U_I 1
#define BETA 0.2
#define GAMMA 1000
#define LAMBDA 0.1
#define SIGMA 1
#define WMAN 10e6														// 10Mb/s
#define CELL 2e6														// 2Mb/s
#define WLAN 7e6														// 7Mb/s

#define WARMING_UP 1000

void print(Network network[], double avg_utility, int move_N);			//print rat info
double net_Utility(Network network, int Area);

int main(void) {

	int checkContinuity = 0;												// Check User Class's continuity
	int gamma = GAMMA;

	FILE *fp = fopen("result.txt", "w");								// File Open		
	if (fp == NULL)														//file open error
	{
		fprintf(stdout, "file open error");
		exit(1);
	}

	Network* network[3];												// 3 array Network class
	network[0] = new Network;
	network[0]->setUser_Count(0);										// WMAN
	network[0]->setBandwidth(WMAN);
	network[1] = new Network;
	network[1]->setUser_Count(0);										// Celluler_Network
	network[1]->setBandwidth(CELL);
	network[2] = new Network;
	network[2]->setUser_Count(0);										// WLAN
	network[2]->setBandwidth(WLAN);

	User user[N_USER];													// 50 of number User class 
	for (int i = 0; i < N_USER; i++) {
		user[i].setM(NETWORK_COUNT);
		user[i].setNetwork_Number(-1);
		user[i].setQValue_Table();
		user[i].setQValue(0);														// 1) Q_i(0) = 0, Initialization Q value & Q Table
	}
	bool convergence = false;
	int count = 0;
	int* change_User_count[3];
	change_User_count[0] = new int;
	change_User_count[1] = new int;
	change_User_count[2] = new int;

	srand((unsigned)time(NULL));										// Random Value

	for (int i = 0; i < N_USER; i++) {
		int j = 0;

		if (i < 10)
			j = 0;
		else if (i < 20)
			j = 1;
		else
			j = 2;

		user[i].setNetwork_Number(j);
		user[i].setArea(j);
		user[i].setArea_Table(j);
		network[j]->setUser_Count(network[j]->getUser_Count() + 1);
		network[j]->setArea_User_Count(network[j]->getArea_User_Count(j) + 1, j);
	}

	for (int i = 0; i < NETWORK_COUNT; i++)
	{
		if (network[i]->getUser_Count() == 0)									//not selected rat
			network[i]->setThroughput(network[i]->getBandwidth());
		else
			network[i]->setThroughput(network[i]->getBandwidth() / network[i]->getUser_Count());

		*change_User_count[i] = network[i]->getUser_Count();
	}

	// 2) lop, 10) endloop
	while (!convergence || checkContinuity != 1)
	{
		fprintf(fp, "%lf\t%lf\t%lf\n", net_Utility(*network[0], 0), net_Utility(*network[1], 0), net_Utility(*network[2], 0));
	
		for (int i = 0; i < N_USER; i++)
		{
			double randomNumber = rand() % 10000 + 1;
			int random = 0;
			int chosen_Network;
			if (randomNumber <= gamma || count == 0) 								// 3) if rand() <= GAMMA
			{
				random = user[i].getArea();
				chosen_Network = (rand() % (random + 1));
				/*
				if (user[i].getArea() == 2)
				{
					chosen_Network = (rand() % 2);									// 4)
					if (chosen_Network == 1)
						chosen_Network = 2;
				}*/
			}
			else																	// 5)
				chosen_Network = user[i].argmaxQValue_NetworkNumber();				// 6)

			user[i].update_QValue(network, chosen_Network, U_I, P_I, BETA, LAMBDA, change_User_count);		// 8), 9)
		}
		///////////////////////////////////////////////////////////////////////////////////////

		for (int i = 0; i < N_USER; i++)
		{
			if (!user[i].getifFixed())
				break;
			if (i == 49)
				convergence = true;

			if (user[i].continuity >= 10)											// User don't move while 10 time
				checkContinuity = 1;
			else
				break;
		}

		count++;
	
		if (gamma != 0)
			gamma -= 1;
		
		printf("%d \n", count);
		printf("WMAN => Area 1 = %d명,		Area 2 = %d명,		Area 3 = %d명\n", network[0]->getArea_User_Count(0), network[0]->getArea_User_Count(1), network[0]->getArea_User_Count(2));
		printf("CELL => Area 1 = %d명,		Area 2 = %d명,		Area 3 = %d명\n", network[1]->getArea_User_Count(0), network[1]->getArea_User_Count(1), network[1]->getArea_User_Count(2));
		printf("WLAN => Area 1 = %d명,		Area 2 = %d명,		Area 3 = %d명\n", network[2]->getArea_User_Count(0), network[2]->getArea_User_Count(1), network[2]->getArea_User_Count(2));

		printf("WMAN의 접속자 수 : %d 명,	Net Utility : %lf\n", network[0]->getUser_Count(), net_Utility(*network[0], 0));
		printf("CELL의 접속자 수 : %d 명,	Net Utility : %lf\n", network[1]->getUser_Count(), net_Utility(*network[1], 0));
		printf("WLAN의 접속자 수 : %d 명,	Net Utility : %lf\n", network[2]->getUser_Count(), net_Utility(*network[2], 0));

		if (count < WARMING_UP)
			convergence = false;

		printf("----------------------------------------------------------------------------\n");
	}
	
	printf("X1_wm = %lf\tX2_wm = %lf\tX3_wm = %lf\t\nX1_ce = %lf\tX2_ce = %lf\tX3_ce = %lf\t\nX1_wl = %lf\tX2_wl = %lf\tX3_wl = %lf\n", (double)network[0]->getArea_User_Count(0) /10, (double)network[0]->getArea_User_Count(1) / 10, (double)network[0]->getArea_User_Count(2) / 30, (double)network[1]->getArea_User_Count(0) / 10, (double)network[1]->getArea_User_Count(1) / 10, (double)network[1]->getArea_User_Count(2) / 30, (double)network[2]->getArea_User_Count(0) / 10, (double)network[2]->getArea_User_Count(1) / 10, (double)network[2]->getArea_User_Count(2) /30);

	fclose(fp);
	free(change_User_count[0]);
	free(change_User_count[1]);
	free(change_User_count[2]);

	return 0;
}

void print(Network network[], double avg_utility, int move_N)
{
	int i = 0;
	printf("            user          bandwidth           throughput               utility\n");
	for (i = 0; i < NETWORK_COUNT; i++)
	{
		printf("RAT %d :       %d            %d              %.3lf                %.3lf\n", i + 1, (int)network[i].getUser_Count(), (int)network[i].getBandwidth(), network[i].getThroughput(), network[i].getUtility());
	}
	printf("avg_utility : %.3lf\n", avg_utility);
	printf("move user : %d\n", move_N);
	printf("-----------------------------------------------\n\n\n");
}

double net_Utility(Network network, int Area)			// PI_i = u(C_i/N) - p_i*N
{
	double Capacity = 0;
	int n_ia = 0;

	for (int k = Area; k < 3; k++)
		n_ia += network.getArea_User_Count(k);

	// 1)

	if (network.getUser_Count() != 0)
		return U_I * (network.getBandwidth() / network.getUser_Count()) - (P_I*  n_ia);
	else
		return U_I * network.getBandwidth() - (P_I*  n_ia);
}