#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>

#define FPS 60
#define winW 1440
#define winH 900
#define PI 3.14159265

using namespace std;

struct hungryShape : public sf::ConvexShape
{
	float health = 100.0f;
	float rotation = 0.0f;
	float speed = 100.0f;
	float radiusPlayer = 0.0f;
	float anglePlayer = 0.0f;
	bool isCombined = false;
	
    float getRotation() const { return rotation; }
	float getSpeed() const { return speed; }
};

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

vector<sf::Vector2f> getGlobalPoints(hungryShape shape)
{
	vector<sf::Vector2f> globalPoints;
	for (int i = 0; i < shape.getPointCount(); i++)
	{
		globalPoints.push_back(shape.getTransform().transformPoint(shape.getPoint(i)));
	}
	
	return globalPoints;
}

bool SATCheck(hungryShape shapeA, hungryShape shapeB)
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
		//cout << i << " " << minProjA - maxProjB << " " << minProjB - maxProjA << endl;
		if ((maxProjB <= minProjA || maxProjA <= minProjB))
			return false;
		
			//cout << i << " " << minProjA - maxProjB << " " << minProjB - maxProjA << endl;
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
		
		if ((maxProjB <= minProjA || maxProjA <= minProjB))
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
	
	if (aMaxX <= bMinX || aMaxY <= bMinY || aMinX >= bMaxX || aMinY >= bMaxY)
		return false;
	else
		return true;
}

float shapeCenterX(hungryShape shape)
{
	float centerX = 0;
	for (int i = 0; i < shape.getPointCount(); i++)
	{
		centerX += shape.getPoint(i).x;
	}
	centerX /= shape.getPointCount();

	return centerX;
}

float shapeCenterY(hungryShape shape)
{
	float centerY = 0;
	for (int i = 0; i < shape.getPointCount(); i++)
	{
		centerY += shape.getPoint(i).y;
	}
	centerY /= shape.getPointCount();
	
	return centerY;
}

sf::RectangleShape getAABB(hungryShape shape)
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
	int test;
	sf::Clock clock;
	float time;
	string stringInput = "";
	stringstream ss;
	
	int shapeCount, pointCount;
	float posX, posY;
	float radius;
	string junk;
	
	sf::Color clrNoCol = sf::Color::Red;
	sf::Color clrAABBCol = sf::Color::Green;
	sf::Color clrSATCol = sf::Color::Blue;
	
	float speed = 100.0f; //pixels per second
	bool iskeyUpPressed = false;
	bool iskeyLeftPressed = false;
	bool iskeyDownPressed = false;
	bool iskeyRightPressed = false;
	bool isTestPressed = false;
	bool isRotating = true;

	int mov[shapeCount];
	int count = 0;
	
	sf::Keyboard::Key keyNext = sf::Keyboard::Space;
	sf::Keyboard::Key keyUp = sf::Keyboard::W;
	sf::Keyboard::Key keyLeft = sf::Keyboard::A;
	sf::Keyboard::Key keyDown = sf::Keyboard::S;
	sf::Keyboard::Key keyRight = sf::Keyboard::D;
	sf::Keyboard::Key keyRotate = sf::Keyboard::R;
	
	string inputFile;
	//cin >> inputFile;
	
	ifstream myFile("values.txt");
	if (myFile.is_open())
	{
		myFile >> shapeCount;
		myFile.close();
	}
	else 
	{
		cout << "Unable to open values.txt file. Loading default values...";
		shapeCount = 3;
	}
	
	hungryShape shapes[shapeCount];
	sf::RectangleShape bounds[shapeCount];
	int rot[shapeCount];
	bool isAABBColliding[shapeCount];
	bool isSATColliding[shapeCount];
	
    sf::Texture texture;
    sf::Texture partsTexture[6];
    string file = "/Users/Meg/Desktop/gamedev/AgarIO/AgarIO/images/parts";
    for (int i = 0; i < 6; i++){
        file += (to_string(i) + ".png");
        if(!partsTexture[i].loadFromFile(file)){
            cout << "Failed to load image " << i << endl;
        }
        file = "/Users/Meg/Desktop/gamedev/AgarIO/AgarIO/images/parts";
    }
    
    if(!texture.loadFromFile("/Users/Meg/Desktop/gamedev/AgarIO/AgarIO/images/spaceboi.png")){
        cout << "Failed to load spaceboi" << endl;
    }
    shapes[0].setTexture(&texture);
    if(!texture.loadFromFile("/Users/Meg/Desktop/gamedev/AgarIO/AgarIO/images/space.png")){
        cout << "Failed to load background" << endl;
    }
    sf::Sprite sprite(texture);
    
    int j = 0;
    for(int i = 1; i < shapeCount; i++){
        if (j == 6){
            j = 0;
        }
        shapes[i].setTexture(&partsTexture[j]);
        j += 1;
    }

	//text file console input
	ifstream myFile2("values.txt");
	if (myFile2.is_open())
	{
		myFile2 >> junk;
		if (shapeCount > 0)
		{
			for (int i = 0; i < shapeCount; i++)
			{
				myFile2 >> pointCount;
				shapes[i].setPointCount(pointCount);
				if (pointCount > 0)
				{
					for (int j = 0; j < pointCount; j++)
					{
						myFile2 >> posX >> posY;
						shapes[i].setPoint(j, sf::Vector2f(posX, posY));
					}
					myFile2 >> posX >> posY;
					shapes[i].setPosition(posX, posY);
				}
				pointCount = 0;
			}
		}
		myFile2.close();
	}
	else 
	{
		cout << "Unable to open values.txt file. Loading default values...";
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
	}
	
	for (int i = 0; i < shapeCount; i++)
	{
		shapes[i].setOrigin(shapeCenterX(shapes[i]), shapeCenterY(shapes[i]));
		shapes[i].setFillColor(clrNoCol);
		isAABBColliding[i] = false;
		isSATColliding[i] = false;
		shapes[i].rotation = rand() % 360 + 1;	
	}
	shapes[0].isCombined = true;
	
	cout << "WASD" << endl;
	cout << "Rotate: R" << endl;
	cout << "Combine: F" << endl;
	
    sf::RenderWindow window( sf::VideoMode( winW, winH ), "SFML works!" );
	window.setFramerateLimit( FPS );

    while (window.isOpen())
    {
		count += 1;
		if (count == 100){
			for (int i = 1; i < shapeCount; i++){
				mov[i] = rand() % 9; 
			}
			count = 0;
		}

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
//				else if (event.key.code == keyRotate)
//				{
//					isRotating = !isRotating;
//				}
				else if (event.key.code = sf::Keyboard::Q)
				{
					isTestPressed = true;
				}
				break;
			}
        }
		
		time = clock.restart().asSeconds();
		
		if (iskeyUpPressed)
		{
			if(iskeyLeftPressed)
				shapes[0].move(-speed*time*0.7071, -speed*time*0.7071);
			else if(iskeyRightPressed)
				shapes[0].move(speed*time*0.7071, -speed*time*0.7071);
			else
				shapes[0].move(0.0f, -speed*time);
		}
		else if (iskeyDownPressed)
		{
			if(iskeyLeftPressed)
				shapes[0].move(-speed*time*0.7071, speed*time*0.7071);
			else if(iskeyRightPressed)
				shapes[0].move(speed*time*0.7071, speed*time*0.7071);
			else
				shapes[0].move(0.0f, speed*time);
		}
		else if (iskeyLeftPressed)
		{
			if(iskeyUpPressed)
				shapes[0].move(-speed*time*0.7071, -speed*time*0.7071);
			else if(iskeyDownPressed)
				shapes[0].move(-speed*time*0.7071, speed*time*0.7071);
			else
				shapes[0].move(-speed*time, 0.0f);
		}
		else if (iskeyRightPressed)
		{
			if(iskeyUpPressed)
				shapes[0].move(speed*time*0.7071, -speed*time*0.7071);
			else if(iskeyDownPressed)
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
					isAABBColliding[i] = true;
					isAABBColliding[j] = true;
					if (SATCheck(shapes[i],shapes[j]))
					{
						isSATColliding[i] = true;
						isSATColliding[j] = true;
					}
				}
			}
		}
		
		//Combine
		if(isTestPressed)
		{
			for (int i = 0; i < shapeCount; i++)
			{
				for (int j = i + 1; j < shapeCount; j++)
				{
					if (isSATColliding[i] && isSATColliding[j])
					{
						if ((shapes[i].isCombined || shapes[j].isCombined) && SATCheck(shapes[i], shapes[j]))
						{
							if(!shapes[i].isCombined)
							{
								shapes[i].rotation = shapes[0].rotation;
								posX = shapes[i].getPosition().x - shapes[0].getPosition().x;
								posY = shapes[i].getPosition().y - shapes[0].getPosition().y;
								shapes[i].radiusPlayer = sqrt(posX*posX + posY*posY);
								shapes[i].anglePlayer = atan(posY/posX) * 180/PI;
								if(posX < 0)
									shapes[i].radiusPlayer *= -1;
								test = shapes[i].anglePlayer;
								cout << posX << endl;
							}
							else if(!shapes[j].isCombined)
							{
								shapes[j].rotation = shapes[0].rotation;
								posX = shapes[j].getPosition().x - shapes[0].getPosition().x;
								posY = shapes[j].getPosition().y - shapes[0].getPosition().y;
								shapes[j].radiusPlayer = sqrt(posX*posX + posY*posY);
								shapes[j].anglePlayer = atan(posY/posX) * 180/PI;
								if(posX < 0)
									shapes[j].radiusPlayer *= -1;
								test = shapes[i].anglePlayer;
								cout << posX << endl;
							}
							shapes[i].isCombined = true;
							shapes[j].isCombined = true;
						}
					}
				}
			}
			isTestPressed = false;
		}

		for(int i = 1; i < shapeCount; i++)
		{
			if (mov[i] == 1){
				shapes[i].move(-speed*time*0.7071, -speed*time*0.7071);
			}
			else if (mov[i] == 2){
				shapes[i].move(0.0f, -speed*time);
			}
			else if (mov[i] == 3){
				shapes[i].move(speed*time*0.7071, -speed*time*0.7071);
			}
			else if (mov[i] == 4){
				shapes[i].move(-speed*time, 0.0f);
			}
			else if (mov[i] == 5){
				shapes[i].move(speed*time, 0.0f);
			}
			else if (mov[i] == 6){
				shapes[i].move(-speed*time*0.7071, speed*time*0.7071);
			}
			else if (mov[i] == 7){
				shapes[i].move(0.0f, speed*time);
			}
			else if (mov[i] == 8){
				shapes[i].move(speed*time*0.7071, speed*time*0.7071);
			}
		}

		
        window.clear();
        window.draw(sprite);
		for (int i = 0; i < shapeCount; i++)
		{
			if(isAABBColliding[i])
			{
				if(isSATColliding[i])
					shapes[i].setFillColor(clrSATCol);
				else
					shapes[i].setFillColor(clrAABBCol);
			}
			else
				shapes[i].setFillColor(clrNoCol);
				
			if(isRotating)
			{
				shapes[i].rotate(shapes[i].rotation*time);
				if(shapes[i].isCombined)
				{
					shapes[i].setPosition(shapes[0].getPosition().x + cos(shapes[i].anglePlayer * PI/180)*shapes[i].radiusPlayer,
											shapes[0].getPosition().y + sin(shapes[i].anglePlayer * PI/180)*shapes[i].radiusPlayer);
					shapes[i].anglePlayer += shapes[0].rotation*time;
				}
			}
			bounds[i] = getAABB(shapes[i]);
			//window.draw(bounds[i]);
			window.draw(shapes[i]);
			isAABBColliding[i] = false;
			isSATColliding[i] = false;
		}
        window.display();
    }
	
    return 0;
}
