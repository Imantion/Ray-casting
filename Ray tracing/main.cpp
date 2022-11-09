#include <SFML/Graphics.hpp>
#include "Player.h"
using namespace sf;

int main()
{

	
	RenderWindow window(VideoMode(1024, 512), "Ray Tracing");
	window.setFramerateLimit(60);

	//VertexArray bebra(PrimitiveType::LineStrip, 3);

	//bebra[0] = Vertex(Vector2f(500, 500));
	//bebra[1] = Vertex(Vector2f(800, 500));
	//bebra[2] = Vertex(Vector2f(100, 700));
	

	Player a(Vector2f(100,200));
	while (window.isOpen())
	{
		
		a.update();
		

		std::cout << "X: " << Mouse::getPosition(window).x << " Y: " << Mouse::getPosition(window).y << std::endl;
		Event event;
		while (window.pollEvent(event))
		{
			
			if (event.type == Event::Closed)
				
				window.close();
		}

		window.clear();
		drawMap(window);
		a.draw(window);
		a.drawRays3D();
		window.display();
	}

	return 0;
}