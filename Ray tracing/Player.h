#include <SFML/Graphics.hpp>
#include <iostream>
#include"map.h"
using namespace sf;

#define PI  3.14159265

double rotation = 180.0 / PI;

class Player
{
private:

	Vector2f pos;
	Vector2f speed;
	RectangleShape player;
	RectangleShape direction;
	VertexArray ray;
	float rotate;

public:
	
	Player(Vector2f pos)
	{
		rotate = 0;
		this->pos = pos;

		player.setSize(Vector2f(10, 10));
		player.setOrigin(Vector2f(10, 5));
		player.setFillColor(Color(162, 25, 255));

		direction.setSize(Vector2f(15, 3));
		direction.setOrigin(Vector2f(0, 1.5));
		direction.setFillColor(Color(162, 25, 255));

		ray.resize(8);
		ray.setPrimitiveType(LineStrip);
	}

	void setSpeed(Vector2f speed)
	{
		this->speed = speed;
	}

	void setPos(Vector2f pos)
	{
		this->pos = pos;
	}

	void update()
	{
		control();
		player.setPosition(pos);
		direction.setPosition(pos);
	}

	void drawRays3D()
	{
		int r, mx, my, mp, dof; float rx, ry, ra, xo, yo;
		ra = abs(rotate) / rotation;
		for (r = 0; r < 1; r++)
		{
			dof = 0;
			float aTan = -1 / tan(ra);
			if (ra > PI)
			{
				ry = (((int)pos.y >> 6) << 6) - 0.0001;
				rx = (pos.y - ry) * aTan + pos.x;
				yo = -64; xo = -yo * aTan;
			}
			if (ra < PI)
			{
				ry = (((int)pos.y >> 6) << 6) + 64;
				rx = (pos.y - ry) * aTan + pos.x;
				yo = 64; xo = -yo * aTan;
			}
			while (dof < 8)
			{
				mx = (int)(rx) >> 6; my = (int)(ry) >> 6; mp = my * mapX + mx;

				ray[dof].position = Vector2f(rx, ry);
				ray[dof].color = Color(0, 0, 255);

				if (mp < mapX * mapY && map[mp] == 1) 
				{ 
					for (size_t i = dof + 1; i < 8; i++)
					{
						ray[i].position = Vector2f(rx, ry);
						ray[i].color = Color(0, 0, 255);
					}

					dof = 8;
				}
				else
				{
					rx += xo; ry += yo;
				}
				 
			}
		}

	}

	void draw(RenderWindow& window)
	{
		rotate = angelMouse(window);

		
		player.setRotation(rotate);
		direction.setRotation(rotate);
		
		window.draw(player);
		window.draw(direction);
		window.draw(ray);
		
	}

	float angelMouse(RenderWindow& window)
	{
		Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

		float diffX = mousePos.x - pos.x;
		float diffY = mousePos.y - pos.y;

		float rot = atan2(diffX, diffY) * rotation;

		std::cout << rot << std::endl;
		return rot;
	}

	void control()
	{
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			this->pos.y -= 2;
		}
		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			this->pos.x -= 2;
		}
		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			this->pos.y += 2;
		}
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			this->pos.x += 2;
		}
	}
};