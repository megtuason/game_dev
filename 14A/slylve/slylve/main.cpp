#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <math.h>
#include <vector>

#define FPS 60
#define winW 800
#define winH 600
#define PI 3.14159265

using namespace std;

float dotProduct (sf::Vector2f a, sf::Vector2f b)
{
	return ((a.x*b.x) + (a.y*b.y));
}

sf::Vector2f getLeftNormal (sf::Vector2f v)
{
	sf::Vector2f n;
	float magnitude = sqrt((v.x*v.x) + (v.y*v.y));
	n.x = v.y / magnitude;
	n.y = -v.x / magnitude;
	
	return n;
}

float projection (sf::Vector2f a, sf::Vector2f p)
{
	return dotProduct(a, p);
}

vector<sf::Vector2f> getNormals(vector<sf::Vector2f> points)
{
	vector<sf::Vector2f> normals;
	sf::Vector2f currentNormal;
	
	for (int i = 0; i < points.size() - 1; i++)
	{
		currentNormal.x = points[i + 1].x - points[i].x;
		currentNormal.y = points[i + 1].y - points[i].y;
		currentNormal = getLeftNormal(currentNormal);
		normals.push_back(currentNormal);
	}
	currentNormal.x = points[0].x - points[points.size() - 1].x;
	currentNormal.y = points[0].y - points[points.size() - 1].y;
	currentNormal = getLeftNormal(currentNormal);
	normals.push_back(currentNormal);
	
	return normals;
}

float getMin(vector<sf::Vector2f> a, sf::Vector2f p)
{
	//get mininum projection of a shape along p vector
	float minProj = dotProduct(a[0], p);
	float currentProj;
	
	for (int i = 1; i < a.size(); i++)
	{
		currentProj = dotProduct(a[i],p);
		//get minimum projection by comparison
		if (minProj > currentProj)
			minProj = currentProj;
	}
	
	return minProj;
}

float getMax(vector<sf::Vector2f> a, sf::Vector2f p)
{
	//get mininum projection of a shape along p vector
	float maxProj = dotProduct(a[0], p);
	float currentProj;
	
	for (int i = 1; i < a.size(); i++)
	{
		currentProj = dotProduct(a[i],p);
		//get maximum projection by comparison
		if (currentProj > maxProj)
			maxProj = currentProj;
	}
	
	return maxProj;
}

vector<sf::Vector2f> getGlobalPoints(sf::ConvexShape shape)
{
	vector<sf::Vector2f> globalPoints;
	for (int i = 0; i < shape.getPointCount(); i++)
	{
		globalPoints.push_back(shape.getTransform().transformPoint(shape.getPoint(i)));
	}
	
	return globalPoints;
}

bool SATCheck(sf::ConvexShape shapeA, sf::ConvexShape shapeB)
{
	sf::Vector2f p;
	vector<sf::Vector2f> globalPointsA, globalPointsB;
	vector<sf::Vector2f> normsA, normsB;
	float minProjA, minProjB, maxProjA, maxProjB;
	
	globalPointsA = getGlobalPoints(shapeA);
	globalPointsB = getGlobalPoints(shapeB);
	
	normsA = getNormals(globalPointsA);
	normsB = getNormals(globalPointsB);
	
	//for drawing normals and origin-to-point lines
	/*
	vector<sf::VertexArray>  lineA, lineB, lineAP, lineBP;

	//A
	for (int i = 0; i < shapeA.getPointCount(); i++)
	{
		sf::VertexArray pusherLine(sf::LinesStrip, 2);
		pusherLine[0].position = sf::Vector2f(shapeA.getPosition().x, shapeA.getPosition().y);
		pusherLine[1].position = sf::Vector2f(shapeA.getPosition().x + 50*normsA[i].x, shapeA.getPosition().y + 50*normsA[i].y);
		lineA.push_back(pusherLine);
		
		sf::VertexArray pusherLine2(sf::LinesStrip, 2);
		pusherLine2[0].position = sf::Vector2f(0, 0);
		pusherLine2[1].position = sf::Vector2f(globalPointsA[i].x, globalPointsA[i].y);
		lineAP.push_back(pusherLine2);
	}

	//B
	for (int i = 0; i < shapeB.getPointCount(); i++)
	{	
		sf::VertexArray pusherLine(sf::LinesStrip, 2);
		pusherLine[0].position = sf::Vector2f(shapeB.getPosition().x, shapeB.getPosition().y);
		pusherLine[1].position = sf::Vector2f(shapeB.getPosition().x + 50*normsB[i].x, shapeB.getPosition().y + 50*normsB[i].y);
		lineB.push_back(pusherLine);
		
		sf::VertexArray pusherLine2(sf::LinesStrip, 2);
		pusherLine2[0].position = sf::Vector2f(0, 0);
		pusherLine2[1].position = sf::Vector2f(globalPointsB[i].x, globalPointsB[i].y);
		lineBP.push_back(pusherLine2);
	}
	
    sf::RenderWindow window2( sf::VideoMode( winW, winH ), "SFML works!" );
	window2.setFramerateLimit( FPS );

    while (window2.isOpen())
    {
        sf::Event event;
        while (window2.pollEvent( event ))
        {
            switch( event.type )
			{
			case sf::Event::Closed:
                window2.close();
				break;
			case sf::Event::KeyPressed:
				if( event.key.code == sf::Keyboard::Escape )
				{
					window2.close();
				}
				break;
			}
        }
        window2.clear();
		window2.draw(shapeA);
		window2.draw(shapeB);
		for (int i = 0; i < shapeA.getPointCount(); i++)
		{
			window2.draw(lineA[i]);
			window2.draw(lineAP[i]);
		}
		for (int i = 0; i < shapeB.getPointCount(); i++)
		{
			window2.draw(lineB[i]);
			window2.draw(lineBP[i]);
		}
        window2.display();
	}

	*/
	//A projects on B normals
	for (int i = 0; i < globalPointsB.size(); i++)
	{
		//go through every normals of the shape
		p = normsB[i];
		
		//get min max of a shape along p vector
		minProjA = getMin(globalPointsA, p);
		minProjB = getMin(globalPointsB, p);
		maxProjA = getMax(globalPointsA, p);
		maxProjB = getMax(globalPointsB, p);
		
		if ((maxProjB < minProjA || maxProjA < minProjB))
			return false;
	}
	
	//B projects on A normals
	for (int i = 0; i < globalPointsA.size(); i++)
	{
		//go through every normals of the shape
		p = normsA[i];
		
		//get min max of a shape along p vector
		minProjA = getMin(globalPointsA, p);
		minProjB = getMin(globalPointsB, p);
		maxProjA = getMax(globalPointsA, p);
		maxProjB = getMax(globalPointsB, p);
		
		if ((maxProjB < minProjA || maxProjA < minProjB))
			return false;
	}
	
	return true;
}

bool AABBCheck(sf::RectangleShape a, sf::RectangleShape b)
{
	float aMinX = a.getPosition().x;
	float aMinY = a.getPosition().y;
	float bMinX = b.getPosition().x;
	float bMinY = b.getPosition().y;
	float aMaxX = aMinX + a.getSize().x;
	float aMaxY = aMinY + a.getSize().y;
	float bMaxX = bMinX + b.getSize().x;
	float bMaxY = bMinY + b.getSize().y;
	
	if (aMaxX < bMinX || aMaxY < bMinY || aMinX > bMaxX || aMinY > bMaxY)
		return false;
	else
		return true;
}

int shapeCenterX(sf::ConvexShape shape)
{
	int centerX = 0;
	for (int i = 0; i < shape.getPointCount(); i++)
	{
		centerX += shape.getPoint(i).x;
	}
	centerX /= shape.getPointCount();
	
	return centerX;
}

int shapeCenterY(sf::ConvexShape shape)
{
	int centerY = 0;
	for (int i = 0; i < shape.getPointCount(); i++)
	{
		centerY += shape.getPoint(i).y;
	}
	centerY /= shape.getPointCount();
	
	return centerY;
}

sf::RectangleShape getAABB(sf::ConvexShape shape)
{
	sf::RectangleShape bound;
	vector<sf::Vector2f> globalPoints = getGlobalPoints(shape);
	float minX, minY, maxX, maxY;
	minX = maxX = globalPoints[0].x;
	minY = maxY = globalPoints[0].y;
	
	for (int i = 1; i < globalPoints.size(); i++)
	{
		if (minX > globalPoints[i].x)
			minX = globalPoints[i].x;
		if (minY > globalPoints[i].y)
			minY = globalPoints[i].y;
		if (maxX < globalPoints[i].x)
			maxX = globalPoints[i].x;
		if (maxY < globalPoints[i].y)
			maxY = globalPoints[i].y;
	}
	
	bound.setPosition(minX, minY);
	bound.setSize(sf::Vector2f(maxX - minX, maxY - minY));
	bound.setFillColor(sf::Color::Transparent);
	bound.setOutlineThickness(1);
	bound.setOutlineColor(sf::Color::White);

	return bound;
}

int main( void )
{
	sf::Clock clock;
	float time;
	string stringInput = "";
	stringstream ss;
	
	int shapeCount, pointCount;
	float posX, posY;
	
	sf::Color clrNoCol = sf::Color::Red;
	sf::Color clrAABBCol = sf::Color::Green;
	sf::Color clrSATCol = sf::Color::Blue;
	/*
	cout << "Input Number of Shapes: ";
	cin >> shapeCount;*/ shapeCount = 3;
	sf::ConvexShape shapes[shapeCount];
	sf::RectangleShape bounds[shapeCount];
	int rot[shapeCount];
	bool isAABBColliding[shapeCount];
	bool isSATColliding[shapeCount];
	
	float speed = 100.0f; //pixels per second
	bool iskeyUpPressed = false;
	bool iskeyLeftPressed = false;
	bool iskeyDownPressed = false;
	bool iskeyRightPressed = false;
	bool isRotating = false;
	
	sf::Keyboard::Key keyNext = sf::Keyboard::Space;
	sf::Keyboard::Key keyUp = sf::Keyboard::W;
	sf::Keyboard::Key keyLeft = sf::Keyboard::A;
	sf::Keyboard::Key keyDown = sf::Keyboard::S;
	sf::Keyboard::Key keyRight = sf::Keyboard::D;
	sf::Keyboard::Key keyRotate = sf::Keyboard::R;
	/*
	//manual console input
	for (int i = 0; i < shapeCount; i++)
	{
		cout << "Input Number of Points for Shape " << i+1 << " : ";
		cin >> pointCount;
		shapes[i].setPointCount(pointCount);
		cout << pointCount << endl;
		cin.ignore(256, '\n');
		cin.clear();
		for (int j = 0; j < pointCount; j++)
		{
			cout << "Input x y coordinates for Point " << j+1 << " (e.g. 1 2): ";
			getline(cin, stringInput);
			ss.str(stringInput);
			ss >> posX >> posY;
			shapes[i].setPoint(j, sf::Vector2f(posX, posY));
			ss.clear();
		}
		cout << "Input x y coordinates for Position (e.g. 1 2): ";
		getline(cin, stringInput);
		ss.str(stringInput);
		ss >> posX >> posY;
		shapes[i].setPosition(posX, posY);
		shapes[i].setFillColor(clrNoCol);
		ss.clear();
		
		bounds[i].setSize(sf::Vector2f(shapes[i].getGlobalBounds().width, shapes[i].getGlobalBounds().height));
		bounds[i].setPosition(shapes[i].getPosition().x, shapes[i].getPosition().y);
		bounds[i].setFillColor(sf::Color::Transparent);
		bounds[i].setOutlineThickness(1);
		bounds[i].setOutlineColor(sf::Color::White);
		
		rot[i] = rand() % 360 + 1;
	}
	*/
	
	shapes[0].setPointCount(3);
	shapes[0].setPoint(0, sf::Vector2f(0, 0));
	shapes[0].setPoint(1, sf::Vector2f(50, 50));
	shapes[0].setPoint(2, sf::Vector2f(-50, 80));
	shapes[0].setPosition(winW/2, winH/2);
	
	shapes[1].setPointCount(4);
	shapes[1].setPoint(0, sf::Vector2f(0, 0));
	shapes[1].setPoint(1, sf::Vector2f(80, 25));
	shapes[1].setPoint(2, sf::Vector2f(70, 60));
	shapes[1].setPoint(3, sf::Vector2f(-25, 70));
	shapes[1].setPosition(winW/2 - 200, winH/2);
	
	shapes[2].setPointCount(5);
	shapes[2].setPoint(0, sf::Vector2f(0, 0));
	shapes[2].setPoint(1, sf::Vector2f(80, 50));
	shapes[2].setPoint(2, sf::Vector2f(90, 100));
	shapes[2].setPoint(3, sf::Vector2f(-10, 150));
	shapes[2].setPoint(4, sf::Vector2f(-70, 120));
	shapes[2].setPosition(winW/2 + 100, winH/2 - 200);
	
	for (int i = 0; i < shapeCount; i++)
	{
		shapes[i].setOrigin(shapeCenterX(shapes[i]), shapeCenterY(shapes[i]));
		shapes[i].setFillColor(clrNoCol);
		isAABBColliding[i] = false;
		isSATColliding[i] = false;
		
		rot[i] = rand() % 360 + 1;
	}
	
	cout << "WASD" << endl;
	cout << "Rotate: R" << endl;
	
    sf::RenderWindow window( sf::VideoMode( winW, winH ), "SFML works!" );
	window.setFramerateLimit( FPS );

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent( event ))
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
					iskeyUpPressed = true;
				}
				else if (event.key.code == keyLeft)
				{
					iskeyLeftPressed = true;
				}
				else if (event.key.code == keyDown)
				{
					iskeyDownPressed = true;
				}
				else if (event.key.code == keyRight)
				{
					iskeyRightPressed = true;
				}
				break;
			case sf::Event::KeyReleased:
				if (event.key.code == keyUp)
				{
					iskeyUpPressed = false;
				}
				else if (event.key.code == keyLeft)
				{
					iskeyLeftPressed = false;
				}
				else if (event.key.code == keyDown)
				{
					iskeyDownPressed = false;
				}
				else if (event.key.code == keyRight)
				{
					iskeyRightPressed = false;
				}
				else if (event.key.code == keyRotate)
				{
					isRotating = !isRotating;
				}
				else if (event.key.code = sf::Keyboard::Q)
				{
					if (SATCheck(shapes[0],shapes[1]))
					{
						shapes[0].setFillColor(clrSATCol);
						shapes[1].setFillColor(clrSATCol);
					}
				}
				break;
			}
        }
		
		time = clock.restart().asSeconds();
		
		if (iskeyUpPressed && shapes[0].getPosition().y > 0.0f)
		{
			if(iskeyLeftPressed && shapes[0].getPosition().x > 0.0f)
				shapes[0].move(-speed*time*0.7071, -speed*time*0.7071);
			else if(iskeyRightPressed && (shapes[0].getPosition().x + bounds[0].getSize().x) < winW)
				shapes[0].move(speed*time*0.7071, -speed*time*0.7071);
			else
				shapes[0].move(0.0f, -speed*time);
		}
		else if (iskeyDownPressed && (shapes[0].getPosition().y + bounds[0].getSize().y) < winH)
		{
			if(iskeyLeftPressed && shapes[0].getPosition().x > 0.0f)
				shapes[0].move(-speed*time*0.7071, speed*time*0.7071);
			else if(iskeyRightPressed && (shapes[0].getPosition().x + bounds[0].getSize().x) < winW)
				shapes[0].move(speed*time*0.7071, speed*time*0.7071);
			else
				shapes[0].move(0.0f, speed*time);
		}
		else if (iskeyLeftPressed && shapes[0].getPosition().x > 0.0f)
		{
			if(iskeyUpPressed && shapes[0].getPosition().y > 0.0f)
				shapes[0].move(-speed*time*0.7071, -speed*time*0.7071);
			else if(iskeyDownPressed && (shapes[0].getPosition().y + bounds[0].getSize().y) < winH)
				shapes[0].move(-speed*time*0.7071, speed*time*0.7071);
			else
				shapes[0].move(-speed*time, 0.0f);
		}
		else if (iskeyRightPressed && (shapes[0].getPosition().x + bounds[0].getSize().x) < winW)
		{
			if(iskeyUpPressed && shapes[0].getPosition().y > 0.0f)
				shapes[0].move(speed*time*0.7071, -speed*time*0.7071);
			else if(iskeyDownPressed && (shapes[0].getPosition().y + bounds[0].getSize().y) < winH)
				shapes[0].move(speed*time*0.7071, speed*time*0.7071);
			else
				shapes[0].move(speed*time, 0.0f);
		}
		
		//Collision Checker
		for (int i = 0; i < shapeCount; i++)
		{
			for (int j = i + 1; j < shapeCount; j++)
			{
				if (AABBCheck(bounds[i], bounds[j]))
				{
					shapes[i].setFillColor(clrAABBCol);
					shapes[j].setFillColor(clrAABBCol);
					isAABBColliding[i] = true;
					isAABBColliding[j] = true;
					if (SATCheck(shapes[i],shapes[j]))
					{
						shapes[i].setFillColor(clrSATCol);
						shapes[j].setFillColor(clrSATCol);
						isSATColliding[i] = true;
						isSATColliding[j] = true;
					}
				}
			}
			if(!isSATColliding[i])
			{
				shapes[i].setFillColor(clrAABBCol);
				if(!isAABBColliding[i])
					shapes[i].setFillColor(clrNoCol);
			}
				
		}
		
        window.clear();
		for (int i = 0; i < shapeCount; i++)
		{
			if(isRotating)
				shapes[i].rotate(rot[i]*time);
			bounds[i] = getAABB(shapes[i]);
			window.draw(shapes[i]);
			window.draw(bounds[i]);
			isAABBColliding[i] = false;
			isSATColliding[i] = false;
		}
        window.display();
    }
	
    return 0;
}