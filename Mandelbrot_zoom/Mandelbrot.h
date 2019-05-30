#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <iostream>
#include <vector>
#include <complex>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <Windows.h>
#include <thread>
#include <stack>

using namespace std;

class Mandelbrot
{
public:

	Mandelbrot(unsigned _width, unsigned _height);
	~Mandelbrot();
	//Oppdaterert x, y (min/max) etter zoom
	void update_min_max(int dir);
	//Oppdaterer bilde, hvis dir er positiv zoomer inn, negativ zoomer ut
	void update_img(sf::Image* s_img, int dir);
	//Endrer sentrum for zoom utafra museklikk
	void set_center(int x, int y);
	//Antall iterasjoner for å si at Z er stabil (svart)
	//Må settes veldig høy for å kunne zoome langt inn
	const int iterations = 1000;
	unsigned width;
	unsigned height;
	double zoom_inc = 1;
	double x_center = (x_min + x_max) / 2;
	double y_center = (y_min - y_max) / 2;
	double x_min = -2;
	double x_max = 2;
	double y_min = -2;
	double y_max = 2;
	double x_inc;
	double y_inc;
	//Verdi som definerer om Z har eksplodert
	int magic_number = 4;
	
private:
	void calc(sf::Image* s_img);
	void iterate(double x, double y, sf::Image* s_img);
	long map_onto(double x, double a, double b, double c, double d);
	double map_onto_double(double x, double a, double b, double c, double d);
	int get_color(int x,int f,int p);
	//Brukes for å ha riktig zoom_inc vet utzooming. Kunne sikkert vært løst bedre
	stack<double> zoom_stack;
};