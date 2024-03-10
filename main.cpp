#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "rk4.h"

//void vrk4( double x0, double y0[], double h, int n, void (*fun)(double, double*, double*), double y1[] )
void fun(double, double*, double*);

double h = 0.01;  //krok calkowania
int n = 2;		
double H = 1;	//wysokosc stozka
double z0 = 2;	//poczatkowe wychylenie z rownowagi punktu stozka ktory jest w 2/3 wysokosci stozka
double v0 = 0.5;  //poczatkowa predkosc domyslnie bedzie rowna 0
double t0=0;
double tk = 2;		//koncowy czas do kiedy ma sie program wykonywac

int main()
{
	//y[0] <- z
	//y[1] <- v
	FILE *p;
	p = fopen("plik.txt", "w");
	if (p == NULL) printf("BLAD");
	
	printf("Podaj krok calkowania (zalecane wartosci od 0.001 do 0.1)");
	scanf("%lf",&h);
	printf("Podaj poczatkowe wychylenie z rownowagi geometrycznego srodka stozka");
	scanf("%lf", &z0);
	printf("Podaj poczatkowe predkosc stozka");
	scanf("%lf", &v0);
	printf("Podaj wysokosc stozka");
	scanf("%lf", &H);
	printf("Podaj czas w sekundach do ktorego program ma liczyc kolejne wartosci polozenia i predkosci stozka");
	scanf("%lf", &tk);

	double E=0.0;
	double y0[2], y1[2];  //wektory poczatkowych wartosci y0 oraz wartosci po kroku calkowania y1
	y0[0] = z0;
	y0[1] = v0;
										//nie wiem jak z energia
	
	printf("t0=%lf, tk=%lf, h=%lf, H=%lf, \tv0=%lf,    z0=%lf,\n\n", t0, tk, h, H, v0, z0);
	fprintf(p, "t0=%lf, tk=%lf, h=%lf, H=%lf, \tv0=%lf,    z0=%lf,\n\n", t0, tk, h, H, v0, z0);
	
	printf("czas\t      z(t)\t       v(t)\t   \n");
	
	printf("%lf\t%lf\t%lf\n", t0, y0[0], y0[1]);
	fprintf(p,"%lf\t%lf\t%lf\n", t0, y0[0], y0[1]);
	double k = (tk - t0) / h;	//ile razy ma policzyc rozwiazanie zagadnienia
	double tablicaz[10000];	//tablica przechowujaca wartosci y
	tablicaz[0] = y0[0];
	double tablicat[10000]; //tablica przechowujaca chwile czasu 
	tablicat[0]=t0; 
	double A,tzero=0,tamplitudy=0;
	for (int i = 0; i < k; i++)
	{
		vrk4(t0, y0, h, n, fun, y1);
		t0 = t0 + h;
	
		y0[0] = y1[0];
		y0[1] = y1[1];
		tablicaz[i] = y1[0];//zapamiêtuje wartoœæ maksymaln¹
		tablicat[i] = t0;
		//E = pow(y1[1], 2) / 2 + 9.81*(y1[0])+ 1 * pow((2 / 3 * H - y1[0]), 4)*3.14 / H / H * 9.81;
		printf("%lf\t%lf\t%lf\t\n", t0, y1[0], y1[1]); //y1[0] to nowa wartosc v; y1[1] to nowa wartosc dla z
		fprintf(p,"%lf\t%lf\t%lf\t\n", t0, y1[0], y1[1]);
	}
	//liczenie energii
	//szukam polozenia minimalnego  
	double wyn = tablicaz[0];
	for (int i = 0; i < k; i++)
	{
		if (wyn > tablicaz[i])
		{
			wyn = tablicaz[i]; //zapisuje wartosc minimalna
			tzero = i;//przechowuje numer komorki dla maksymalnej aplitudy
		}
	}
	//szukam polozenia maksymalnego
	double wyn2 = tablicaz[0];
	for (int i = 1; i < n; i++)
	{
		if (wyn2 < tablicaz[i])
		{
			wyn2 = tablicaz[i];//zapisuje wartosc maksymalna
			tamplitudy = i;
		}
	}
	tzero = tablicat[int(tzero)]; //uzyskuje czas dla minimalnego wychylenia z po³o¿enia równowagi
	tamplitudy = tablicat[int(tamplitudy)]; //czas dla maksymalnego wychylenia z po³o¿enia równowagi
	//printf("MAKS/MIN T1/T2 %lf\t%lf\n%lf\t%lf", wyn, wyn2,tzero,tamplitudy);
	E = 3.14*3.14/2*1/(tamplitudy-tzero)*(tamplitudy - tzero)*(wyn-wyn2)/2* (wyn - wyn2) / 2; //energia mechaniczna
	printf("\nenergia\t%lf", E);
	fclose(p);
	getchar();
	return 0;
}

void fun(double t, double y[], double f[])
{
	f[0] = y[1];
	f[1] = -9.81 + 9.81 * 27 / 8 * pow((2 / 3 - y[0]/H), 3);
}