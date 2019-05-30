#include "Mandelbrot.h"


Mandelbrot::Mandelbrot(unsigned _width, unsigned _height) :
	width(_width), height(_height)
{
	x_inc = (x_max - x_min) / (double)width;
	y_inc = (y_max - y_min) / (double)height;
}

Mandelbrot::~Mandelbrot()
{
}

void Mandelbrot::update_img(sf::Image* s_img, int dir)
{
	if (dir != 0) update_min_max(dir);
	calc(s_img);
}

void Mandelbrot::set_center(int _x, int _y)
{
	x_center = map_onto_double(_x, 0, width, x_min, x_max);
	y_center = map_onto_double(_y, 0, height, y_max, y_min);
	cout << "Center coordinates updated: " << endl;
	cout << "x: " << x_center << " | ";
	cout << "y: " << y_center << endl;
}

void Mandelbrot::update_min_max(int dir)
{
	double x_inc_fact = (zoom_inc / abs(x_max - x_min));
	double y_inc_fact = (zoom_inc / abs(y_max - y_min));
	//Zoomer inn
	if(dir == 1)
	{
		x_min += x_inc_fact * abs(x_center-x_min);
		x_max -= x_inc_fact * abs(x_center-x_max);
		x_inc = abs(x_max - x_min)/(double)width;
		y_min += y_inc_fact * abs(y_center-y_min);
		y_max -= y_inc_fact * abs(y_center-y_max);
		y_inc = abs(y_max - y_min)/(double)height;
		zoom_stack.push(zoom_inc);
		cout << "zoom_inc used: " << zoom_inc << endl;
		if ((zoom_inc > abs(y_center - y_max) && zoom_inc > abs(y_center - y_min)) ||
			(zoom_inc > abs(x_center - x_max) && zoom_inc > abs(x_center - x_min)))
		{
			zoom_inc /= 2;
			cout << "new zoom_inc: " << zoom_inc << endl;
		}
	}
	//Zoomer ut
	else if (dir == -1)
	{
		if (!zoom_stack.empty())
		{
			zoom_inc = zoom_stack.top();
			zoom_stack.pop();
			x_inc_fact = (zoom_inc / abs(x_max - x_min));
			y_inc_fact = (zoom_inc / abs(y_max - y_min));
		}
		x_min -= x_inc_fact * abs(x_center - x_min);
		x_max += x_inc_fact * abs(x_center - x_max);
		x_inc = abs(x_max - x_min) / (double)width;
		y_min -= y_inc_fact * abs(y_center - y_min);
		y_max += y_inc_fact * abs(y_center - y_max);
		y_inc = abs(y_max - y_min) / (double)height;
	}
}

void Mandelbrot::calc(sf::Image* s_img)
{
	for(double y = y_max; y >= y_min; y -= y_inc)
	{
		for(double x = x_min; x <= x_max; x += x_inc)
		{
			iterate(x, y, s_img);
		}
	}
}

void Mandelbrot::iterate(double x, double y, sf::Image* s_img)
{
	//Tegner aksekors, ikke nødvendig.
	//Blir stygt så fort du kommer langt inn
	/*if (abs(x) < 0.0001 || abs(y) < 0.0001)
	{
		long x_pix = map_onto(x, x_min, x_max, 0, width);
		long y_pix = map_onto(y, y_max, y_min, 0, height);
		s_img->setPixel(x_pix, y_pix, sf::Color(255, 255, 255, 100));
		return;
	}*/

	//Det gikk kjappere uten bruk av complex
	//complex<double> c(x, y);
	//complex<double> z(0, 0);
	double z_real = 0;
	double z_imag = 0;
	int no_of_iterations = 0;
	//Itererer over z, hopper ut hvis z eksploderer.
	for(no_of_iterations; no_of_iterations <= iterations; no_of_iterations++)
	{
		double old_z_real = z_real;
		z_real = pow(z_real, 2) - pow(z_imag, 2) + x;
		z_imag = 2*old_z_real*z_imag + y;
		if(pow(z_real,2)+pow(z_imag,2) > magic_number) break;
	}
	//Finner rett pixel for x og y-verdi
	long x_pix = map_onto(x, x_min, x_max, 0, width);
	long y_pix = map_onto(y, y_max, y_min, 0, height);
	//Farger svart
	if(no_of_iterations > 84)
	{
		s_img->setPixel(x_pix, y_pix, sf::Color(0, 0, 0, 255));
	}
	//Gir annen farge
	else
	{
		s_img->setPixel(x_pix, y_pix, sf::Color(
			get_color(no_of_iterations, 1, 0),
			get_color(no_of_iterations, 1, 120),
			get_color(no_of_iterations, 1, 240), 255));
	}
}

long Mandelbrot::map_onto(double x, double a, double b, double c, double d)
{
	return (x-a)*((d-c)/(b-a))+c;
}

int Mandelbrot::get_color(int x,int f,int p)
{
	return 255*pow((cos(sqrt(x)*f + p)), 2);
}

double Mandelbrot::map_onto_double(double x, double a, double b, double c, double d)
{
	return (x - a)*((d - c) / (b - a)) + c;
}

