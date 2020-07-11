#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>

#define FPS 60
#define CSIZE 6
#define winW 480
#define winH 320
#define PI 3.14159265
#define threshold 0.7

using namespace std;

sf::CircleShape circ[60];
sf::RectangleShape rect[40];

void createCircle(int i, sf::Color color)
{
	int ranX, ranY;

	ranX = rand() % winW;
	ranY = rand() % winH;
	circ[i].setPosition(ranX,ranY);
	circ[i].setRadius(30.0f);
	circ[i].setFillColor(color);
}

void createRectangle(int i, sf::Color color)
{
	int ranX, ranY;

	ranX = rand() % winW;
	ranY = rand() % winH;
	rect[i].setPosition(ranX,ranY);
	rect[i].setSize(sf::Vector2f(50.0f,50.0f));
	rect[i].setFillColor(color);
}

int main( void )
{
	srand(time(NULL));

	int counter = 0;
	float speed = 20.0f; //pixels per second
	float specialSpeed = 200.0f; //pixels per second
	float time;
	sf::Clock clock;
	
	sf::Keyboard::Key keyUp = sf::Keyboard::W;
	sf::Keyboard::Key keyDown = sf::Keyboard::S;
	sf::Keyboard::Key keyLeft = sf::Keyboard::A;
	sf::Keyboard::Key keyRight = sf::Keyboard::D;
	
	bool isKeyUpPressed = false;
	bool isKeyDownPressed = false;
	bool isKeyLeftPressed = false;
	bool isKeyRightPressed = false;
	bool isMouseLeftButtonPressed = false;
	
	float rectXmouse = 0;
	float rectYmouse = 0;
	float mouseAngle = 0;
	
    sf::RenderWindow window( sf::VideoMode( winW, winH ), "SFML works!" );


	sf::Color color[CSIZE];
	color[0] = sf::Color::Red;
	color[1] = sf::Color::Green;
	color[2] = sf::Color::Blue;
    color[3] = sf::Color::Yellow;
    color[4] = sf::Color::Cyan;
    color[5] = sf::Color::White;

	window.setFramerateLimit( FPS );

	for (int i = 0; i < 60; i++)
	{
		if (counter == CSIZE)
			counter = 0;
		createCircle(i, color[counter]);
		if (i < 40)
			createRectangle(i, color[counter]);
		counter++;
	}

    while( window.isOpen() )
    {
        sf::Event event;
        while( window.pollEvent( event ) )
        {
            switch( event.type )
			{
			case sf::Event::Closed:
                window.close();
				break;
			case sf::Event::KeyPressed:
				if( event.key.code == sf::Keyboard::Escape )
				{
					window.close();
				}
				else if (event.key.code == keyUp)
				{
					isKeyUpPressed = true;
				}
				else if (event.key.code == keyDown)
				{
					isKeyDownPressed = true;
				}
				else if (event.key.code == keyLeft)
				{
					isKeyLeftPressed = true;
				}
				else if (event.key.code == keyRight)
				{
					isKeyRightPressed = true;
				}
				break;
			case sf::Event::KeyReleased:
				if (event.key.code == keyUp)
				{
					isKeyUpPressed = false;
				}
				else if (event.key.code == keyDown)
				{
					isKeyDownPressed = false;
				}
				else if (event.key.code == keyLeft)
				{
					isKeyLeftPressed = false;
				}
				else if (event.key.code == keyRight)
				{
					isKeyRightPressed = false;
				}
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
					isMouseLeftButtonPressed = true;
				break;
			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Left)
					isMouseLeftButtonPressed = false;
				break;
			}
        }

		time = clock.restart().asSeconds();
		
		//Circle WASD
		if(isKeyUpPressed)
		{
			if(isKeyLeftPressed)
				circ[0].move(-specialSpeed*time*0.7071, -specialSpeed*time*0.7071);
			else if(isKeyRightPressed)
				circ[0].move(specialSpeed*time*0.7071, -specialSpeed*time*0.7071);
			else
				circ[0].move(0.0f, -specialSpeed*time);
		}
		else if(isKeyDownPressed)
		{
			if(isKeyLeftPressed)
				circ[0].move(-specialSpeed*time*0.7071, specialSpeed*time*0.7071);
			else if(isKeyRightPressed)
				circ[0].move(specialSpeed*time*0.7071, specialSpeed*time*0.7071);
			else
				circ[0].move(0.0f, specialSpeed*time);
		}
		else if(isKeyLeftPressed)
		{
			if(isKeyUpPressed)
				circ[0].move(-specialSpeed*time*0.7071, -specialSpeed*time*0.7071);
			else if(isKeyDownPressed)
				circ[0].move(-specialSpeed*time*0.7071, specialSpeed*time*0.7071);
			else
				circ[0].move(-specialSpeed*time, 0.0f);
		}
		else if(isKeyRightPressed)
		{
			if(isKeyUpPressed)
				circ[0].move(specialSpeed*time*0.7071, -specialSpeed*time*0.7071);
			else if(isKeyDownPressed)
				circ[0].move(specialSpeed*time*0.7071, specialSpeed*time*0.7071);
			else
				circ[0].move(specialSpeed*time, 0.0f);
		}
		
		//Square Follow
		rectXmouse = rect[0].getPosition().x - sf::Mouse::getPosition(window).x;
		rectYmouse = rect[0].getPosition().y - sf::Mouse::getPosition(window).y;
		if(isMouseLeftButtonPressed && abs(rectXmouse) > threshold && abs(rectYmouse) > threshold)
		{
			mouseAngle = (atan2(rectYmouse, rectXmouse)) * 180 / PI;
			rect[0].move(-cos(PI*mouseAngle/180.f)*specialSpeed*time, -sin(PI*mouseAngle/180.f)*specialSpeed*time);
		}
		
		
		for (int i = 1; i < 60; i++)
		{
			circ[i].move(0.0f, speed*time);
			if (i < 40)
				rect[i].move(speed*time, 0.0f);
		}		

        window.clear();
		
		window.draw(circ[0]);
		window.draw(rect[0]);		
		/*
		for (int i = 0; i < 60; i++)
		{
			window.draw(circ[i]);
			if (i < 40)
				window.draw(rect[i]);
		}
		*/
        window.display();
    }

    return 0;
}