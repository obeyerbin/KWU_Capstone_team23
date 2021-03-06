// KJH.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N_user 30
#define N_RAT 3

struct rat {
	double Bandwidth;
	double user = 0;
	double Throughput = 0;
	double Utility;
	double Parameter = 3;
	double Pi_1 = 0.15;
	double Pi_2 = 0.15;
};

double Utilize_Through_ItoJ(const rat RATi, const rat RATj);
void print_excuse(rat now_RAT[], int count);

int main()
{
	rat RAT[N_RAT];

	int user[N_user];
	int i = 0;
	int temp;
	int count = 1;
	srand((unsigned)time(NULL));

	for (i = 0; i < N_RAT; i++) { // Bandwidth setup
			RAT[i].Bandwidth = 10000 * (i+1);
	}
	
	for (i = 0; i < N_user; i++) { // First, User select Random RAT and Throughput setup
		temp = (rand() % N_RAT);
		RAT[temp].Throughput = RAT[temp].Bandwidth / ++RAT[temp].user;
		user[i] = temp;
	}

	for (i = 0; i < N_RAT; i++) { // Utility setup
		if (RAT[i].Throughput == 0) // if RAT is not selected anybody
			RAT[i].Throughput = INFINITY;

		RAT[i].Utility = (RAT[i].Parameter * RAT[i].Throughput) - (RAT[i].Pi_1 * RAT[i].Bandwidth) - RAT[i].Pi_2;
	}
	print_excuse(RAT, 0); // print now event

	while(count) { // select RAT which more fast RAT
		int key = 0;
		double maxUtility = 0;
		rat *RAT_min = &RAT[0];
		rat *RAT_max = &RAT[0];

			for (i = 0; i < N_RAT; i++) { 
				for (int j = i + 1; j < N_RAT; j++) {
						if (Utilize_Through_ItoJ(RAT[i], RAT[j]) > maxUtility){
							maxUtility = Utilize_Through_ItoJ(RAT[i], RAT[j]);
							RAT_min = &RAT[i];
							RAT_max = &RAT[j];
						}
						else
							key++;
				}
			}
			for (int i = N_RAT - 1; i >= 0; i--) {
				for (int j = i - 1; j >= 0; j--)
					if (Utilize_Through_ItoJ(RAT[i], RAT[j]) > maxUtility) {
						maxUtility = Utilize_Through_ItoJ(RAT[i], RAT[j]);
						RAT_min = &RAT[i];
						RAT_max = &RAT[j];
					}
					else
						key++;;
			}

			// change RAT
			RAT_min->Throughput = RAT_min->Bandwidth / --RAT_min->user;
			RAT_min->Utility = (RAT_min->Parameter * RAT_min->Throughput) - (RAT_min->Pi_1 * RAT_min->Bandwidth) - RAT_min->Pi_2;

			RAT_max->Throughput = RAT_max->Bandwidth / ++RAT_max->user;
			RAT_max->Utility = (RAT_max->Parameter * RAT_max->Throughput) - (RAT_max->Pi_1 * RAT_max->Bandwidth) - RAT_max->Pi_2;


		if (key == N_RAT * (N_RAT-1))
			count = 0;
		else
			print_excuse(RAT,count++); // print now event
		
	}

    return 0;
}

double Utilize_Through_ItoJ(const rat RATi, const rat RATj) {

	rat RATj_Plus_1 = RATj;

	RATj_Plus_1.Throughput = RATj_Plus_1.Bandwidth / ++RATj_Plus_1.user;
	RATj_Plus_1.Utility = (RATj_Plus_1.Parameter * RATj_Plus_1.Throughput) - (RATj_Plus_1.Pi_1 * RATj_Plus_1.Bandwidth) - RATj_Plus_1.Pi_2;

	return RATj_Plus_1.Utility - RATi.Utility;
}

void print_excuse(rat now_RAT[],int count) {
	printf("%dth_excuse\n				user	Percent_of_Users_in_RAT		Bandwidth	Throughput		Utility\n",count);
	for (int i = 0; i < N_RAT; i++) {
		printf("%dth_RAT 			%d	%.3lf				%d		%.3lf			%.3lf\n", i + 1, (int)now_RAT[i].user, now_RAT[i].user / (double) N_user, (int)now_RAT[i].Bandwidth, now_RAT[i].Throughput, now_RAT[i].Utility);
	}
	printf("*****************************************************\n");
	for (int i = 0; i < N_RAT; i++) {
		for(int j = i+1; j < N_RAT; j++)
			printf("%dth_RAT -->	%dth_RAT  %.3lf\n", i + 1, j+1, Utilize_Through_ItoJ(now_RAT[i],now_RAT[j]));
	}
	for (int i = N_RAT-1; i >= 0; i--) {
		for (int j = i - 1; j >=0; j--)
			printf("%dth_RAT -->	%dth_RAT  %.3lf\n", i + 1, j + 1, Utilize_Through_ItoJ(now_RAT[i], now_RAT[j]));
	}
	printf("----------------------------------------------------------------------------------------------------\n");
}