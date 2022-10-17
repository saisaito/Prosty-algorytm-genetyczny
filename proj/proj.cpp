using namespace std;
#include <iostream>
#include <cstdlib>
#include <bitset>
#include <string>
#include <ctime>
#include <random>
#include <cmath>
#include <algorithm>
#include <fstream>


int a = -1;
int b = 50;
int c = 20;
int ile_wyk = 40;
int ile_pop = 14;
int ile_os = 10;
double pr_krzyz = 0.9;
double pr_mutacja = 0.1;
string dziecko[10];
string tab1[10];
double tab[10];
bool spr_rodz[10];
double prawdop;
double f_sum;
double f_x[10];
double p_x[10]; // wynik prawdop.
long long temp;
int tab2[10];
double tab3[10];

void los_lb()
{
	for (int i = 0; i < ile_pop; i++)
	{
		tab[i] = (rand() % 256);
	}
}
void zamiana_bin() //liczba - zmienna
{
	for (int i = 0; i < ile_pop; i++)
	{
		tab1[i] = bitset<8>(tab[i]).to_string();
	}
}

void krzyzowanie()
{
	int rodzic1, rodzic2;
	int pktPc;

	float w_lb_psl = (float)rand() / (float)RAND_MAX;

	rodzic1 = (int)(rand() % ile_os);
	rodzic2 = (int)(rand() % ile_os);
	int i = 0;
	for (; i < ile_os; i += 2)
	{
		if (spr_rodz[rodzic1] == true) {
			rodzic1 = (int)(rand() % ile_os);
			i -= 2;
			continue;
		}
		else if (spr_rodz[rodzic2] == true || rodzic1 == rodzic2) {
			rodzic2 = (int)(rand() % ile_os);
			i -= 2;
			continue;
		}
		else {
			spr_rodz[rodzic1] = true;
			spr_rodz[rodzic2] = true;
			if (w_lb_psl <= pr_krzyz) {
				pktPc = (int)(rand() % 8);
				dziecko[i] = tab1[rodzic1].substr(0, pktPc);
				dziecko[i] += tab1[rodzic2].substr(pktPc, 8);
				dziecko[i + 1] = tab1[rodzic2].substr(0, pktPc);
				dziecko[i + 1] += tab1[rodzic1].substr(pktPc, 8);
			}
			else
			{
				dziecko[i] = tab1[rodzic1];
				dziecko[i + 1] = tab1[rodzic2];
			}
			for (int j = 0; j < 10; j++)
			{
				spr_rodz[j] = false;
			}
		}

	}
}


void mut() {
	double w_lb_psl;
	string tmp;
	for (int i = 0; i < ile_os; i++) {
		tmp = dziecko[i];
		for (int j = 0; j < 8; j++) // tutut
		{
			w_lb_psl = (float)rand() / (float)RAND_MAX;
			if (w_lb_psl <= pr_mutacja)
			{
				if (tmp[j] == '1')
				{
					tmp[j] = '0';
				}
				else
				{
					tmp[j] = '1';
				}
			}
		}
		dziecko[i] = tmp;
	}

}

void dod() {
	double prawdop;
	for (int i = 0; i < ile_os; i++)
	{
		f_sum += f_x[i];
	}
	for (int i = 0; i < ile_os; i++)
	{
		prawdop = f_x[i] / f_sum;
		p_x[i] = prawdop;
	}
}
void uj() {
	double min = 0;
	for (int i = 0; i < ile_os; i++) {
		if (f_x[i] < min) { // jezeli min jes wikeszy to wynik funkcji = min
			min = f_x[i];
		}
	}
	for (int i = 0; i < ile_os; i++) {
		tab3[i] = f_x[i] - min; //tabela dodatnia
		f_sum += tab3[i];
	}
	for (int i = 0; i < ile_os; i++)
	{
		prawdop = tab3[i] / f_sum;
		p_x[i] = prawdop;
	}
}

void sel() {
	double losuj = 0;
	long temp;
	int zero = 0;
	double sumaP; // syma prawdopod
	for (int i = 0; i < ile_os; i++) {
		temp = bitset<8>(dziecko[i]).to_ullong();
		tab2[i] = temp;
		f_x[i] = a * (temp * temp) + (b * temp) + c;
		if (f_x[i] < zero && i == ile_os - 1)
		{
			uj();
		}
		else if (f_x[i] > zero && i == ile_os - 1)
		{
			dod();
		}
		for (int i = 0; i < ile_os; i++) {
			losuj = (float)rand() / (float)RAND_MAX;
			sumaP = 0; // suma prawdopodobienstwa
			for (int j = 0; j < ile_os; j++)
			{
				sumaP += p_x[j]; // sumaryczna wartosc funkcji
				if (sumaP >= losuj) {
					tab1[i] = dziecko[j];
					break;
				}
			}
		}
	}
}

int main()
{
	double max;
	int licznik = 0;

	srand(time(NULL));

	ofstream zapisz("wynik.txt");

	for (int i = 0; i < ile_wyk; i++)
	{
		los_lb();
		for (int j = 0; j < ile_pop; j++)
		{
			zamiana_bin();
			krzyzowanie();
			mut();
			sel();
		}
		max = f_x[0];
		for (int t = 0; t < ile_os; t++)
		{
			if (f_x[t] > max)
			{
				max = f_x[t];
				licznik = t;
			}
		}
		//cout << maxi << " " << tab2[licznik] << endl;
		zapisz << tab2[licznik] << " " << max << endl;
	}
	zapisz.close();
}
