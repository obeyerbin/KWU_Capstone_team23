#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable:4996)
//시뮬레이션 비교를 위해 3번영역 ce는 선택하지 못하도록 수정된 상태입니다.
#define N_user 50
#define pi 0.01
#define ui 1

#define wm 0
#define ce 1
#define wl 2

#define wm_total_capacity 10e6
#define ce_total_capacity 2e6
#define wl_total_capacity 7e6

typedef struct rat
{
	int rat_name;
	double Capacity;
	double utility;
	int user;
	int area;
}rat;

typedef struct user
{
	int rat_name;
	int area;
	double utility;
}user;


void compute_avg_utility(user user[], int a0_users, int a1_users, int a2_users);
void compute_utility(rat RAT[], user user[], int wm_users, int ce_users, int wl_users);
void print_result(rat RAT[], user user[]);


double avg_0 = 0;
double avg_1 = 0;
double avg_2 = 0;

int main()
{
	int i;
	rat RAT[6];
	user user[N_user];

	int a0_users = 0;
	int a1_users = 0;
	int a2_users = 0;

	int wm_users = 0;
	int ce_users = 0;
	int wl_users = 0;


	FILE *fp = fopen("result.txt", "w");

	if (fp == NULL) exit(1);

	srand((unsigned)time(NULL));

	RAT[0].rat_name = wm;
	RAT[0].area = 0;
	RAT[0].user = 0;

	RAT[1].rat_name = wm;
	RAT[1].area = 1;
	RAT[1].user = 0;

	RAT[2].rat_name = ce;
	RAT[2].area = 1;
	RAT[2].user = 0;

	RAT[3].rat_name = wm;
	RAT[3].area = 2;
	RAT[3].user = 0;

	RAT[4].rat_name = ce;
	RAT[4].area = 2;
	RAT[4].user = 0;

	RAT[5].rat_name = wl;
	RAT[5].area = 2;
	RAT[5].user = 0;

	for (i = 0; i < 10; i++)//area set, randomly choose
	{
		user[i].area = 0;
		user[i].rat_name = wm;
		RAT[0].user++;
		wm_users++;
		a0_users++;
	}

	for (i = 10; i < 20; i++)
	{
		user[i].area = 1;
		//user[i].rat_name = rand() % 2;

		if (i >= 10 && 16 >= i)	user[i].rat_name = wm; //Initial x^(2)_wm = 0.7
		else user[i].rat_name = ce;

		if (user[i].rat_name == wm)
		{
			RAT[1].user++;
			wm_users++;
		}
		else
		{
			RAT[2].user++;
			ce_users++;
		}
		a1_users++;
	}

	for (i = 20; i < 50; i++)
	{
		user[i].area = 2;
		//user[i].rat_name = rand() % 2;

		if (i >= 20 && 40 >= i)	user[i].rat_name = wm; //initial x^(3)_wm = 0.7
		else user[i].rat_name = wl;

		if (user[i].rat_name == wm)
		{
			RAT[3].user++;
			wm_users++;
		}
		else if (user[i].rat_name == ce)
		{
			user[i].rat_name = wl;
			RAT[5].user++;
			wl_users++;
			//RAT[4].user++;
			//ce_users++;
		}
		else
		{
			RAT[5].user++;
			wl_users++;
		}
		a2_users++;
	}

	compute_utility(RAT, user, wm_users, ce_users, wl_users);
	compute_avg_utility(user, a0_users, a1_users, a2_users);

	int j;
	//fprintf(fp, "%lf\t %lf\t\n", (double)RAT[1].user / a1_users, (double)RAT[3].user / a2_users);
	print_result(RAT, user);
	//fprintf(fp, "%lf\t %lf\t\n", (double)RAT[1].user / a1_users, (double)RAT[3].user / a2_users);
	fprintf(fp, "%lf\t%lf\n", RAT[0].utility, RAT[2].utility);
	printf("area 1 avg_utility: %.3lf\n", avg_0);
	printf("area 2 avg_utility: %.3lf\n", avg_1);
	printf("area 3 avg_utility: %.3lf\n\n\n", avg_2);
	for (j = 0; j < 14; j++)
	{
		for (i = 0; i < N_user; i++)
		{
			if (user[i].area == 1)
			{
				if (user[i].utility < avg_1)
				{
					if (rand() / (double)RAND_MAX < (avg_1 - user[i].utility) / avg_1)
					{
						if (user[i].rat_name == wm)
						{
							if (user[i].utility < RAT[2].utility && RAT[1].user > 0)
							{
								RAT[1].user--;
								RAT[2].user++;
								wm_users--;
								ce_users++;
								user[i].rat_name = ce;
							}
						}
						else
						{
							if (user[i].utility < RAT[1].utility && RAT[2].user > 0)
							{
								RAT[2].user--;
								RAT[1].user++;
								ce_users--;
								wm_users++;
								user[i].rat_name = wm;
							}
						}
					}
				}
			}

			else if(user[i].area == 2)
			{
				if (user[i].utility < avg_2)
				{
					if (rand() / (double)RAND_MAX < (avg_2 - user[i].utility) / avg_2)
					{
						
						if (user[i].rat_name == wm)
						{
							if (user[i].utility < RAT[5].utility && RAT[3].user > 0)
							{
								RAT[3].user--;
								RAT[5].user++;
								wm_users--;
								wl_users++;
								user[i].rat_name = wl;
							}
						}
						else
						{
							if (user[i].utility < RAT[3].utility && RAT[5].user > 0)
							{
								RAT[5].user--;
								RAT[3].user++;
								wl_users--;
								wm_users++;
								user[i].rat_name = wm;
							}
						}
						

						/*
						if (user[i].rat_name == wm)
						{
							if (user[i].utility < RAT[5].utility && RAT[3].user > 0)
							{
								RAT[3].user--;
								RAT[5].user++;
								wm_users--;
								wl_users++;
								user[i].rat_name = wl;
							}
							else if (user[i].utility < RAT[4].utility && RAT[3].user > 0)
							{
								RAT[3].user--;
								RAT[4].user++;
								wm_users--;
								ce_users++;
								user[i].rat_name = ce;
							}
						}

						else  if (user[i].rat_name == ce)
						{
							if (user[i].utility < RAT[5].utility && RAT[3].user > 0)
							{
								RAT[4].user--;
								RAT[5].user++;
								ce_users--;
								wl_users++;
								user[i].rat_name = wl;
							}
							else if (user[i].utility < RAT[3].utility && RAT[3].user > 0)
							{
								RAT[4].user--;
								RAT[3].user++;
								ce_users--;
								wm_users++;
								user[i].rat_name = wm;
							}
						}

						else
						{
							if (user[i].utility < RAT[3].utility && RAT[5].user > 0)
							{
								RAT[5].user--;
								RAT[3].user++;
								wl_users--;
								wm_users++;
								user[i].rat_name = wm;
							}
							else if (user[i].utility < RAT[4].utility && RAT[5].user > 0)
							{
								RAT[5].user--;
								RAT[4].user++;
								wl_users--;
								ce_users++;
								user[i].rat_name = ce;
							}
						}
						*/
					}
				}
			}

			compute_utility(RAT, user, wm_users, ce_users, wl_users);
			compute_avg_utility(user, a0_users, a1_users, a2_users);
			//print_result(RAT, user);
			//fprintf(fp, "%lf\t %lf\t %lf\t %lf\t %lf\t %lf\n", (double)RAT[0].user / a0_users, (double)RAT[1].user / a1_users, (double)RAT[2].user / a1_users, (double)RAT[3].user / a2_users, (double)RAT[4].user / a2_users, (double)RAT[5].user / a2_users);

			//printf("area 1 avg_utility: %.3lf\n", avg_0);
			//printf("area 2 avg_utility: %.3lf\n", avg_1);
			//printf("area 3 avg_utility: %.3lf\n\n\n", avg_2);
		}
		print_result(RAT, user);
		//fprintf(fp, "%lf\t %lf\t\n", (double)RAT[1].user / a1_users, (double)RAT[3].user / a2_users);
		fprintf(fp, "%lf\t%lf\n", RAT[0].utility, RAT[2].utility);
		printf("area 1 avg_utility: %.3lf\n", avg_0);
		printf("area 2 avg_utility: %.3lf\n", avg_1);
		printf("area 3 avg_utility: %.3lf\n\n\n", avg_2);
	}
	
	//fprintf(fp, "%lf\t%lf\n", (double)RAT[1].user / a1_users, (double)RAT[3].user / a2_users);
	fclose(fp);
	return 0;
}

void compute_utility(rat RAT[], user user[], int wm_users, int ce_users, int wl_users)
{
	int i;

	for (i = 0; i < N_user; i++)
	{
		if (user[i].rat_name == wm)
		{
			if (user[i].area == 0)
			{
				user[i].utility = ui * (wm_total_capacity / wm_users) - pi * wm_users;
				RAT[0].utility = user[i].utility;
			}
			else if (user[i].area == 1)
			{
				user[i].utility = ui * (wm_total_capacity / wm_users) - pi * (wm_users - RAT[0].user);
				RAT[1].utility = user[i].utility;
			}
			else
			{
				user[i].utility = ui * (wm_total_capacity / wm_users) - pi * (wm_users - RAT[0].user - RAT[1].user);
				RAT[3].utility = user[i].utility;
			}
		}

		else if (user[i].rat_name == ce)
		{
			if (user[i].area == 1)
			{
				user[i].utility = ui * (ce_total_capacity / ce_users) - pi * ce_users;
				RAT[2].utility = user[i].utility;
			}
			//else//ce_3
			//{
			//	user[i].utility = ui * (ce_total_capacity / (ce_users) - pi * (ce_users - RAT[2].user);
			//	RAT[4].utility = user[i].utility;
			RAT[4].utility = 0;
			//}
		}

		else
		{
			user[i].utility = ui * (wl_total_capacity / wl_users) - pi * wl_users;
			RAT[5].utility = user[i].utility;
		}
	}

}

void compute_avg_utility(user user[], int a0_users, int a1_users, int a2_users)
{
	int i;
	double a0_total = 0;
	double a1_total = 0;
	double a2_total = 0;

	for (i = 0; i < N_user; i++)
	{
		if (user[i].area == 0)
			a0_total = a0_total + user[i].utility;
		else if (user[i].area == 1)
			a1_total = a1_total + user[i].utility;
		else
			a2_total = a2_total + user[i].utility;
	}

	avg_0 = a0_total / (double)a0_users;
	avg_1 = a1_total / (double)a1_users;
	avg_2 = a2_total / (double)a2_users;
}

void print_result(rat RAT[], user user[])
{
	int i = 0;
	printf("                  user\n");
	for (i = 0; i < 6; i++)
	{
		if (RAT[i].rat_name == wm)
		{
			printf("WM ");
			printf("in area %d    ", RAT[i].area + 1);
		}
		else if (RAT[i].rat_name == ce)
		{
			printf("CE ");
			printf("in area %d    ", RAT[i].area + 1);
		}
		else
		{
			printf("WL ");
			printf("in area %d    ", RAT[i].area + 1);
		}
		printf("%d	%.3lf\n", RAT[i].user	, RAT[i].utility);
	}
	printf("-----------------------------------------------\n\n\n");
}