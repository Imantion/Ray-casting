#include <SFML/Graphics.hpp>
#include <iostream>
#include"map.h"
using namespace sf;

#define PI  3.14159265

double rotation = 180.0 / PI;

float distance(float x1, float y1, float x2, float y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

class Player
{
private:


	Vector2f pos;
	Vector2f acceleration;
	RectangleShape player;
	RectangleShape direction;
	std::vector<VertexArray> ray;
	float rotate;

public:

	Vector2i mousePos;

	Player(Vector2f pos)
	{

		rotate = 0;
		this->pos = pos;

		player.setSize(Vector2f(10, 10));
		player.setOrigin(Vector2f(5, 5));
		player.setFillColor(Color(162, 25, 255));

		direction.setSize(Vector2f(15, 3));
		direction.setOrigin(Vector2f(0, 1.5));
		direction.setFillColor(Color(162, 25, 255));

		ray.resize(60);
		for (size_t i = 0; i < ray.size(); i++)
		{
			ray[i].resize(2);
			ray[i].setPrimitiveType(LineStrip);
		}
	}

	void setSpeed(Vector2f speed)
	{
		this->acceleration = speed;
	}

	void setPos(Vector2f pos)
	{
		this->pos = pos;
	}
	void rotatePlayer(int ang)
	{
		rotate += ang;
	}
	void update(const float& time, RenderWindow& window)
	{
		/*rotate = angleMouse(window);*/


		control(time);

		player.setPosition(pos);

		direction.setPosition(pos);

		for (size_t i = 0; i < ray.size(); i++)
		{
			ray[i][0].position = pos;
		}

	}

	void drawRays3D(RenderWindow& window)
	{
		int r, mx, my, mp, dof; float rx, ry, ra, xo, yo;
		float dist;
		RectangleShape wall;

		RectangleShape floor;
		floor.setFillColor(Color(164, 12, 240));

		RectangleShape roof;
		roof.setFillColor(/*Color(12, 240, 233)*/Color::Blue);


		for (r = 0; r < 60; r++)
		{
			float distH = 10000;
			ra = rotate - 30 + r;
			if (ra < 0) ra += 360;
			else if (ra > 360) ra -= 360;
			// vertical
			dof = 0;
			float aTan = 1 / tan(ra / rotation);
			if (ra < 180)
			{
				ry = int(pos.y / 64) * 64 - 0.0001;
				rx = (pos.y - ry) * aTan + pos.x;
				yo = -64; xo = -yo * aTan;
			}
			if (ra > 180)
			{
				ry = int(pos.y / 64) * 64 + 64;
				rx = (pos.y - ry) * aTan + pos.x;
				yo = 64; xo = -yo * aTan;
			}
			if (ra == 0 || ra == 360 || ra == 180) { rx = pos.x; ry = pos.y; dof = 8; }
			while (dof < 8)
			{
				mx = (int)(rx) >> 6; my = (int)(ry) >> 6; mp = my * mapX + mx;


				if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
				{
					distH = distance(pos.x, pos.y, rx, ry);

					ray[r][1].position = Vector2f(rx, ry);
					ray[r][1].color = Color::Blue;

					dof = 8;
				}
				else
				{
					rx += xo; ry += yo;
					++dof;
				}

			}

			// horizontal


			float nTan = tan(ra / rotation);
			float distW = 10000;
			dof = 0;
			if (ra > 90 && ra < 270)
			{
				rx = int(pos.x / 64) * 64 - 0.0001;
				ry = (pos.x - rx) * nTan + pos.y;
				xo = -64; yo = -xo * nTan;
			}

			if (ra < 90 || ra > 270)
			{
				rx = int(pos.x / 64) * 64 + 64;
				ry = (pos.x - rx) * nTan + pos.y;
				xo = 64; yo = -xo * nTan;
			}

			if (ra == 90 || ra == 270) { rx = pos.x; ry = pos.y; dof = 8; }
			while (dof < 8)
			{
				mx = (int)(rx) >> 6; my = (int)(ry) >> 6; mp = my * mapX + mx;


				if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
				{
					distW = distance(pos.x, pos.y, rx, ry);
					dof = 8;
				}
				else
				{
					rx += xo; ry += yo;
					++dof;
				}
			}
			if (distH > distW)
			{
				ray[r][1].position = Vector2f(rx, ry);
				ray[r][1].color = Color::Blue;
				dist = distW;

				wall.setFillColor(Color(255, 0, 0));
			}
			else { dist = distH; wall.setFillColor(Color(240, 0, 0)); }
			window.draw(ray[r]);


			// Draw 3D
			float ca = rotate - ra; if (ca < 0) ca += 360; if (ca > 360) ca -= 360;
			dist = dist * cos(ca / rotation);

			float lineH = (mapS * 512) / dist; if (lineH > 512) lineH = 512;
			float lineO = 256 - lineH / 2;

			wall.setSize(Vector2f(10, lineH));
			wall.setPosition(Vector2f(10 * r + 512, lineO));
			window.draw(wall);

			floor.setSize(Vector2f(10, 512 - lineH - lineO));
			floor.setPosition(Vector2f(10 * r + 512, lineH + lineO));
			window.draw(floor);

			roof.setSize(Vector2f(10, lineO));
			roof.setPosition(Vector2f(10 * r + 512, 0));
			window.draw(roof);


		}

	}

	void draw(RenderWindow& window)
	{
		player.setRotation(-rotate);
		direction.setRotation(-rotate);

		window.draw(player);
		window.draw(direction);
	}

	float angleMouse(RenderWindow& window)
	{
		Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

		float diffX = mousePos.x - this->pos.x;
		float diffY = mousePos.y - this->pos.y;

		/*rotate = diffY / diffX;

		if (rotate < 0) rotate += 2 * PI;
		else if (rotate > 2 * PI) rotate -= 2 * PI;*/

		float rot = atan2(diffY, diffX);

		if (rot < 0) rot *= -1;
		else rot = 2 * PI - rot;

		std::cout << rot * rotation << std::endl;
		return rot * rotation;
	}

	void control(const float& time)
	{
		float rad = rotate / rotation;


		/*	float pdx = 1.5 * cos(rad);
			float pdy = 1.5 * sin(rad);*/
		float deltaTime = time * 0.1;

		float cs = deltaTime * cos(rad);
		float sn = deltaTime * sin(rad);

		float xo = 0, yo = 0;

		if (cs > 0) xo = 20; else xo = -20;
		if (sn < 0) yo = 20; else yo = -20;





		int ipx = pos.x / 64, ipx_sub = (pos.x - xo) / 64, ipx_add = (pos.x + xo) / 64;
		int ipy = pos.y / 64, ipy_sub = (pos.y - yo) / 64, ipy_add = (pos.y + yo) / 64;


		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			if (map[ipy * mapX + ipx_add] == 0 && !checkCollision(ipx_add, ipy))
				this->pos.x += cs;

			if (map[ipy_add * mapX + ipx] == 0 && !checkCollision(ipx, ipy_add))
				this->pos.y -= sn;

		}
		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			if (cs > 0) xo = 20; else xo = -20;
			if (sn > 0) yo = 20; else yo = -20;

			ipx = pos.x / 64, ipx_sub = (pos.x - xo) / 64, ipx_add = (pos.x + xo) / 64;
			ipy = pos.y / 64, ipy_sub = (pos.y - yo) / 64, ipy_add = (pos.y + yo) / 64;

			if (map[ipy * mapX + ipx_add] == 0 && !checkCollision(ipx_add, ipy))
				this->pos.x += sn;
			if (map[ipy_add * mapX + ipx] == 0 && !checkCollision(ipx, ipy_add))
				this->pos.y += cs;

		}
		if (Keyboard::isKeyPressed(Keyboard::S))
		{

			if (map[ipy * mapX + ipx_sub] == 0 && !checkCollision(ipx_sub, ipy))
				this->pos.x -= cs;
			if (map[ipy_sub * mapX + ipx] == 0 && !checkCollision(ipx, ipy_sub))
				this->pos.y += sn;


		}
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			if (cs > 0) xo = 20; else xo = -20;
			if (sn > 0) yo = 20; else yo = -20;

			ipx = pos.x / 64, ipx_sub = (pos.x - xo) / 64, ipx_add = (pos.x + xo) / 64;
			ipy = pos.y / 64, ipy_sub = (pos.y - yo) / 64, ipy_add = (pos.y + yo) / 64;

			if (map[ipy * mapX + ipx_add] == 0 && !checkCollision(ipx_add, ipy))
				this->pos.x -= sn;
			if (map[ipy_add * mapX + ipx] == 0 && !checkCollision(ipx, ipy_add))
				this->pos.y -= cs;

		}

		Vector2i mouseSecondPosition = Mouse::getPosition();



		if (mouseSecondPosition.x < mousePos.x) rotate += 1.5 * deltaTime;
		else if (mouseSecondPosition.x > mousePos.x) rotate -= 1.5 * deltaTime;

		if (rotate < 0) rotate += 360;
		else if (rotate > 360) rotate -= 360;
	}


	bool checkCollision(int x, int y)
	{
		return map[y * mapX + x] == 1;
	}
};