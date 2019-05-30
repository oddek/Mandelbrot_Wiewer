#include "Mandelbrot.h"
#include <cassert>

int s_width = GetSystemMetrics(SM_CYSCREEN);
int s_height = GetSystemMetrics(SM_CYSCREEN);

int main()
{
	Mandelbrot* m = new Mandelbrot(s_width, s_height);

	//Vindu
	sf::RenderWindow window(sf::VideoMode(s_width, s_height), "Mandelbrot");
	window.setKeyRepeatEnabled(false);
	//Mandelbrot bilde
	sf::Image img;
	img.create(m->width, m->height);
	m->update_img(&img, 0);
	sf::Texture texture;
	texture.loadFromImage(img);
	sf::Sprite sprite;
	sprite.setTexture(texture);

	//Timeglass
	sf::Image hour_img;
	hour_img.loadFromFile("hourglass.png");
	sf::Vector2u v = hour_img.getSize();
	sf::Texture hour_texture;
	hour_texture.loadFromImage(hour_img);
	sf::Sprite hour_sprite;
	hour_sprite.setTexture(hour_texture);
	hour_sprite.setPosition(((s_width-v.x)/2), ((s_height-v.y) / 2));

	//Rektangel for sentrum
	sf::RectangleShape rect;
	rect.setSize({ 130, 60});
	rect.setFillColor(sf::Color::Black);
	rect.setOutlineThickness(1);
	rect.setOutlineColor(sf::Color::Red);



	//Text
	sf::Font font;
	assert(font.loadFromFile("OpenSans-Light.ttf"));
	sf::Color text_color;
	text_color = sf::Color::Red;
	int text_size{15};

	sf::Text pos_text;
	pos_text.setFont(font);
	pos_text.setString("Center position:");
	pos_text.setCharacterSize(text_size);
	pos_text.setStyle(sf::Text::Bold);
	pos_text.setFillColor(text_color);
	pos_text.setPosition(2, 0);

	sf::Text x_text;
	x_text.setFont(font);
	x_text.setString(to_string(m->x_center));
	x_text.setCharacterSize(text_size);
	x_text.setStyle(sf::Text::Regular);
	x_text.setFillColor(text_color);
	x_text.setPosition(2, 20);

	sf::Text y_text;
	y_text.setFont(font);
	y_text.setString(to_string(m->y_center));
	y_text.setCharacterSize(text_size);
	y_text.setStyle(sf::Text::Regular);
	y_text.setFillColor(text_color);
	y_text.setPosition(2, 40);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;

					
				case sf::Event::MouseWheelScrolled:
					if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
					{
						window.draw(hour_sprite);
						window.display();
						m->update_img(&img, event.mouseWheelScroll.delta);
						texture.update(img);
					}	
					break;
				case sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						x_text.setString(to_string(m->x_center));
						m->set_center(event.mouseButton.x, event.mouseButton.y);
						y_text.setString(to_string(m->y_center));
					}
					break;
				case sf::Event::KeyPressed:
					if(event.key.code == sf::Keyboard::S)
						if(img.saveToFile("img.png")) cout << "Image saved as: img.png" << endl;
					if (event.key.code == sf::Keyboard::Up)
					{
						window.draw(hour_sprite);
						window.display();
						window.draw(hour_sprite);
						window.display();
						m->update_img(&img, 1);
						texture.update(img);
						
					}
					if (event.key.code == sf::Keyboard::Down)
					{
						
						m->update_img(&img, -1);
						texture.update(img);
					}
				default:
					break;
			}
		}
	
		window.clear();

		//const sf::Texture& tex = renderTexture.getTexture();
		//sf::Sprite s(tex);
		
		window.draw(sprite);
		window.draw(rect);
		window.draw(pos_text);
		window.draw(x_text);
		window.draw(y_text);
		window.display();
	}
	return 0;

}


