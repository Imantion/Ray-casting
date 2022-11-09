#include <SFML/Graphics.hpp>

int mapX = 8, mapY = 8, mapS = 64;
int map[] =
{
	1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,1,1,1,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,1,1,0,0,1,0,1,
	1,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,
};

void drawMap(sf::RenderWindow& window)
{
	sf::RectangleShape mapBlock(sf::Vector2f(64 - 1, 64 - 1));
	for (size_t y = 0; y < mapY; y++)
	{
		for (size_t x = 0; x < mapX; x++)
		{
			mapBlock.setPosition(sf::Vector2f(x * 64, y * 64));
			if (map[y * mapX + x] == 1)
			{
				mapBlock.setFillColor(sf::Color(50,50,50));
			}
			else
			{
				mapBlock.setFillColor(sf::Color::White);
			}
			
			window.draw(mapBlock);
		}
	}
}