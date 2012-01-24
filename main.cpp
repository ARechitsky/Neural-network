/* 
 * File:   main.cpp
 * Author: xamejieoh
 *
 * Created on 27 Май 2010 г., 0:49
 */

#include <time.h>

#include "main.h"

/*
 * 
 */
int main(int argc, char** argv)
{
	srand(uint(time(NULL)));
	//	PERCEPTRON TEST
	//
		int n = 4, *c = new int [n];
		c[0] = 1;
		c[1] = 3;
		c[2] = 3;
		c[3] = 1;
		perceptron *b = new perceptron(c, n);
		float **d = new float *[8];
		float **e = new float *[8];
		for (int i = 0; i < 8; i++)
		{
			d[i] = new float;
			e[i] = new float;
		}
		d[0][0] = .2876;
		d[1][0] = .2674;
		d[2][0] = .2657;
		d[3][0] = .2478;
		d[4][0] = .2060;
		d[5][0] = .1385;
		d[6][0] = .0890;
		d[7][0] = .0822;
		e[0][0] = .6907;
		e[1][0] = .5791;
		e[2][0] = .5770;
		e[3][0] = .5461;
		e[4][0] = .4916;
		e[5][0] = .4358;
		e[6][0] = .4076;
		e[7][0] = .4047;
		float **a = new float *[8];
		for (int i = 0; i < 8; i++)
		{
			a[i] = b -> get_result(d[i]);
			printf("%f - %f\n", d[i][0], a[i][0]);
		}
		printf("\n\n");
		b -> learn(d, e, 8, .0001, .2);
		for (int i = 0; i < 8; i++)
		{
			a[i] = b -> get_result(d[i]);
			printf("%f - %f\n", d[i][0], a[i][0]);
		}
		printf("\n\n");

		b -> save("1.dat");
		b -> load("1.dat");

		for (int i = 0; i < 8; i++)
		{
			a[i] = b -> get_result(d[i]);
			printf("%f - %f\n", d[i][0], a[i][0]);
		}
		printf("\n\n");
	//  COUNTER PROPAGATION NET TEST
	//
	//	counter_propagation_net *b = new counter_propagation_net(3, 3, 3);
	//	float **c = new float *[21];
	//	float **d = new float *[21];
	//	for (int i = 0; i < 21; i++)
	//	{
	//		c[i] = new float [2];
	//	}
	//	//	c[0][0] = 0;
	//	//	c[0][1] = 0;
	//	//	c[1][0] = 40;
	//	//	c[1][1] = 40;
	//	//	c[2][0] = 20;
	//	//	c[2][1] = 20;
	//	c[0][0] = 12;
	//	c[0][1] = 30;
	//	c[1][0] = 8;
	//	c[1][1] = 28;
	//	c[2][0] = 12;
	//	c[2][1] = 27;
	//	c[3][0] = 14;
	//	c[3][1] = 25;
	//	c[4][0] = 7;
	//	c[4][1] = 24;
	//	c[5][0] = 10;
	//	c[5][1] = 24;
	//	c[6][0] = 12;
	//	c[6][1] = 21;
	//	c[7][0] = 11;
	//	c[7][1] = 5;
	//	c[8][0] = 7;
	//	c[8][1] = 7;
	//	c[9][0] = 7;
	//	c[9][1] = 9;
	//	c[10][0] = 12;
	//	c[10][1] = 9;
	//	c[11][0] = 11;
	//	c[11][1] = 11;
	//	c[12][0] = 9;
	//	c[12][1] = 9;
	//	c[13][0] = 11;
	//	c[13][1] = 6;
	//	c[14][0] = 27;
	//	c[14][1] = 15;
	//	c[15][0] = 28;
	//	c[15][1] = 20;
	//	c[16][0] = 26;
	//	c[16][1] = 23;
	//	c[17][0] = 29;
	//	c[17][1] = 25;
	//	c[18][0] = 31;
	//	c[18][1] = 22;
	//	c[19][0] = 35;
	//	c[19][1] = 24;
	//	c[20][0] = 32;
	//	c[20][1] = 19;
	//	for (int i = 0; i < 21; i++)
	//	{
	//		d[i] = new float [3];
	//		for (int j = 0; j < 2; j++)
	//		{
	//			d[i][j] = (c[7 * (i % 3) + i / 3][j] - 20) / sqrt(800);
	//		}
	//		d[i][2] = sqrt(1 - d[i][0] * d[i][0] - d[i][1] * d[i][1]);
	//	}
	//	float **a = new float *[21];
	//	for (int i = 0; i < 21; i++)
	//	{
	//		a[i] = b -> get_result(d[i]);
	//		printf("%f, %f - %f\n", d[i][0], d[i][1], a[i][0]);
	//	}
	//	printf("\n\n");
	//	b -> learn(d, d, 21, .7, .5);
	//	for (int i = 0; i < 21; i++)
	//	{
	//		a[i] = b -> get_result(d[i]);
	//		printf("%+f, %+f - %+f, %+f\n", d[i][0], d[i][1], a[i][0], a[i][1]);
	//	}

	// HOPFIELD NET TEST
	//
	//		int n = 100;
	//		int len = 3;
	//		int sqrtn = int(sqrt(n));
	//		n = sqrtn * sqrtn;
	//		hopfield_net *b = new hopfield_net(n);
	//		float **d = new float *[len];
	//		for (int i = 0; i < len; i++)
	//		{
	//			d[i] = new float [n];
	//		}
	//
	//		int tmp;
	//		FILE *a = fopen("image.txt", "r");
	//		for (int k = 0; k < len; k++)
	//		{
	//			for (int i = 0; i < sqrtn; i++)
	//			{
	//				for (int j = 0; j < sqrtn; j++)
	//				{
	//					tmp = fgetc(a);
	//					d[k][i * sqrtn + j] = tmp - '0';
	//				}
	//				fgetc(a);
	//			}
	//			fgetc(a);
	//		}
	//
	//		for (int k = 0; k < len; k++)
	//		{
	//			for (int i = 0; i < sqrtn; i++)
	//			{
	//				for (int j = 0; j < sqrtn; j++)
	//				{
	//					printf("%d ", int(d[k][i * sqrtn + j]));
	//				}
	//				printf("\n");
	//			}
	//			printf("\n");
	//		}
	//
	//		b -> learn(d, 2);
	//		float *c = b -> get_result(d[0]);
	//
	//		for (int i = 0; i < sqrtn; i++)
	//		{
	//			for (int j = 0; j < sqrtn; j++)
	//			{
	//				printf("%d ", (c[i * sqrtn + j]) > .99);
	//			}
	//			printf("\n");
	//		}
	//		printf("\n");

	// COGNITRON_ONE_DIM TEST


	//	int n = 100;
	//	int len = 3;
	//	int sqrtn = int(sqrt(n));
	//	n = sqrtn * sqrtn;
	//	cognitron_one_dim *b = new cognitron_one_dim(n, 5, 7, 7);
	//	float **d = new float *[len];
	//	for (int i = 0; i < len; i++)
	//	{
	//		d[i] = new float [n];
	//	}
	//
	//	int tmp;
	//	FILE *a = fopen("image.txt", "r");
	//	for (int k = 0; k < len; k++)
	//	{
	//		for (int i = 0; i < sqrtn; i++)
	//		{
	//			for (int j = 0; j < sqrtn; j++)
	//			{
	//				tmp = fgetc(a);
	//				d[k][i * sqrtn + j] = tmp - '0';
	//				printf("%d ", int(d[k][i * sqrtn + j]));
	//			}
	//			fgetc(a);
	//			printf("\n");
	//		}
	//		fgetc(a);
	//		printf("\n");
	//	}
	//	float *c, *e;
	//	//	c = b -> get_result(d[0]);
	//	//		for (int k = 0; k < len; k++)
	//	//		{
	//	//			c = b -> get_result(d[k]);
	//	//			for (int i = 0; i < sqrtn; i++)
	//	//			{
	//	//				for (int j = 0; j < sqrtn; j++)
	//	//				{
	//	//					printf("%3d ", int(c[i * sqrtn + j]*100.0));
	//	//				}
	//	//				printf("\n");
	//	//			}
	//	//			printf("\n");
	//	//		}
	//	for (int k = 0; k < len; k++)
	//	{
	//		c = b -> get_result(d[k]);
	//		//			e = b -> get_result(d[k]);
	//		for (int i = 0; i < sqrtn; i++)
	//		{
	//			for (int j = 0; j < sqrtn; j++)
	//			{
	//				printf("%3d ", int(c[i * sqrtn + j]*100));
	//				//				printf("%8.6f ", c[i * sqrtn + j]);
	//				//					if (c[i * sqrtn + j] != e[i * sqrtn + j])
	//				//					{
	//				//						printf("\n\n%6.4f\n\n  WTF?!\n\n", e[i * sqrtn + j]);
	//				//						exit(1);
	//				//					}
	//			}
	//			printf("\n");
	//		}
	//		printf("\n");
	//	}
	//
	//	b->learn(d, 3, 10, .5, .1);
	//	for (int k = 0; k < len; k++)
	//	{
	//		c = b -> get_result(d[k]);
	//		//			e = b -> get_result(d[k]);
	//		for (int i = 0; i < sqrtn; i++)
	//		{
	//			for (int j = 0; j < sqrtn; j++)
	//			{
	//				printf("%3d ", int(c[i * sqrtn + j]*100));
	//				//				printf("%8.6f ", c[i * sqrtn + j]);
	//				//					if (c[i * sqrtn + j] != e[i * sqrtn + j])
	//				//					{
	//				//						printf("\n\n%6.4f\n\n  WTF?!\n\n", e[i * sqrtn + j]);
	//				//						exit(1);
	//				//					}
	//			}
	//			printf("\n");
	//		}
	//		printf("\n");
	//	}
	//	//	int ****a = new int ***[2];
	//	//	for (int i = 0; i < 2; i++)
	//	//	{
	//	//		a[i] = new int **[2];
	//	//		for (int j = 0; j < 2; j++)
	//	//		{
	//	//			a[i][j] = new int *[2];
	//	//			for (int k = 0; k < 2; k++)
	//	//			{
	//	//				a[i][j][k] = new int [2];
	//	//				for (int t = 0; t < 2; t++)
	//	//				{
	//	//					a[i][j][k][t] = 1;
	//	//				}
	//	//			}
	//	//		}
	//	//	}

	// COGNITRON_SQUARE TEST
	//
	//	int n = 10;
	//	int len = 3;
	//	cognitron_square *b = new cognitron_square(n, 5, 7, 7);
	//	float ***d = new float **[len];
	//	for (int i = 0; i < len; i++)
	//	{
	//		d[i] = new float *[n];
	//		for (int j = 0; j < n; j++)
	//		{
	//			d[i][j] = new float [n];
	//		}
	//	}
	//
	//	int tmp;
	//	FILE *a = fopen("image.txt", "r");
	//	for (int k = 0; k < len; k++)
	//	{
	//		for (int i = 0; i < n; i++)
	//		{
	//			for (int j = 0; j < n; j++)
	//			{
	//				tmp = fgetc(a);
	//				d[k][i][j] = tmp - '0';
	//				printf("%d ", int(d[k][i][j]));
	//			}
	//			fgetc(a);
	//			printf("\n");
	//		}
	//		fgetc(a);
	//		printf("\n");
	//	}
	//	float **c, *e;
	//	//	c = b -> get_result(d[0]);
	//	//		for (int k = 0; k < len; k++)
	//	//		{
	//	//			c = b -> get_result(d[k]);
	//	//			for (int i = 0; i < sqrtn; i++)
	//	//			{
	//	//				for (int j = 0; j < sqrtn; j++)
	//	//				{
	//	//					printf("%3d ", int(c[i * sqrtn + j]*100.0));
	//	//				}
	//	//				printf("\n");
	//	//			}
	//	//			printf("\n");
	//	//		}
	//	for (int k = 0; k < len; k++)
	//	{
	//		c = b -> get_result(d[k]);
	//		//			e = b -> get_result(d[k]);
	//		for (int i = 0; i < n; i++)
	//		{
	//			for (int j = 0; j < n; j++)
	//			{
	//				printf("%3d ", int(c[i][j]*100));
	//				//				printf("%8.6f ", c[i * sqrtn + j]);
	//				//					if (c[i * sqrtn + j] != e[i * sqrtn + j])
	//				//					{
	//				//						printf("\n\n%6.4f\n\n  WTF?!\n\n", e[i * sqrtn + j]);
	//				//						exit(1);
	//				//					}
	//			}
	//			printf("\n");
	//		}
	//		printf("\n");
	//	}
	//
	//	printf("\n\n\n\n");
	//	b->learn(d, 3, 100, .5, .1);
	//	for (int k = 0; k < len; k++)
	//	{
	//		c = b -> get_result(d[k]);
	//		//			e = b -> get_result(d[k]);
	//		for (int i = 0; i < n; i++)
	//		{
	//			for (int j = 0; j < n; j++)
	//			{
	//				printf("%3d ", int(c[i][j]*100));
	//				//				printf("%8.6f ", c[i * sqrtn + j]);
	//				//					if (c[i * sqrtn + j] != e[i * sqrtn + j])
	//				//					{
	//				//						printf("\n\n%6.4f\n\n  WTF?!\n\n", e[i * sqrtn + j]);
	//				//						exit(1);
	//				//					}
	//			}
	//			printf("\n");
	//		}
	//		printf("\n");
	//	}
	//	//	int ****a = new int ***[2];
	//	//	for (int i = 0; i < 2; i++)
	//	//	{
	//	//		a[i] = new int **[2];
	//	//		for (int j = 0; j < 2; j++)
	//	//		{
	//	//			a[i][j] = new int *[2];
	//	//			for (int k = 0; k < 2; k++)
	//	//			{
	//	//				a[i][j][k] = new int [2];
	//	//				for (int t = 0; t < 2; t++)
	//	//				{
	//	//					a[i][j][k][t] = 1;
	//	//				}
	//	//			}
	//	//		}
	//	//	}

	// NEOCOGNITRON_SQUARE TEST



	return (EXIT_SUCCESS);
}

