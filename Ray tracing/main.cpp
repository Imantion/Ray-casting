#include <SFML/Graphics.hpp>
#include "Player.h"
using namespace sf;

int main()
{

	
	RenderWindow window(VideoMode(1024, 512), "Ray Tracing");
	window.setFramerateLimit(60);

	Player a(Vector2f(100,200));
	float time;
	Clock cl;
	while (window.isOpen())
	{
		
		Mouse::setPosition(Vector2i(512, 512));
		time = cl.getElapsedTime().asMicroseconds();
		cl.restart();
		time = time / 800;
		
		
		

		std::cout << "X: " << Mouse::getPosition(window).x << " Y: " << Mouse::getPosition(window).y << std::endl;
		Event event;
		while (window.pollEvent(event))
		{
			
			if (Event::MouseMoved)
			{
				if (Mouse::getPosition().x > 512) a.rotatePlayer(time*0.1);
				if (Mouse::getPosition().x < 512) a.rotatePlayer(-0.1*time);
			}

			if (event.type == Event::Closed)
				window.close();
		}

		window.clear();
		a.update(time, window);
		drawMap(window);
		a.draw(window);
		a.drawRays3D(window);
		window.display();
	}

	return 0;
}