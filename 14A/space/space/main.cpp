#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
#define thresholdMouseFollow 1

using namespace std;

struct myShape : public sf::ConvexShape
{
	float rotation = 0.0f;
	float speed = 100.0f;
	float angle = 0.0f;
	bool isVisible = true;
	bool speedboost = false;
	sf::Vector2i cellIndex;
	sf::RectangleShape bounds;
	sf::Texture texture;
	
	void loadDimensions(string fileName);
	void loadTexture(string fileName);
};

void myShape::loadDimensions(string fileName)
{
	int pointCount;
	float posX, posY;
	
	//shape dimensions and position
	ifstream file(fileName);
	if (file.is_open())
	{
		file >> pointCount;
		setPointCount(pointCount);
		if (pointCount > 0)
		{
			for (int j = 0; j < pointCount; j++)
			{
				file >> posX >> posY;
				setPoint(j, sf::Vector2f(posX, posY));
			}
		}
		file.close();
	}
	else
	{
		cout << "Failed to load dimensions (" + fileName + ")" << endl;
	}
	
	
	return;
}

void myShape::loadTexture(string fileName)
{
	if(!texture.loadFromFile(fileName))
	{
		cout << "Failed to load texture (" + fileName + ")" << endl;
	}
	setTexture(&texture);
}

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

vector<sf::Vector2f> getGlobalPoints(myShape shape)
{
	vector<sf::Vector2f> globalPoints;
	for (int i = 0; i < shape.getPointCount(); i++)
	{
		globalPoints.push_back(shape.getTransform().transformPoint(shape.getPoint(i)));
	}
	
	return globalPoints;
}

bool SATCheck(myShape shapeA, myShape shapeB)
{
	sf::Vector2f p;
	vector<sf::Vector2f> globalPointsA, globalPointsB;
	vector<sf::Vector2f> normsA, normsB;
	float minProjA, minProjB, maxProjA, maxProjB;
	
	globalPointsA = getGlobalPoints(shapeA);
	globalPointsB = getGlobalPoints(shapeB);
	
	normsA = getNormals(globalPointsA);
	normsB = getNormals(globalPointsB);
	
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

float shapeCenterX(myShape shape)
{
	float centerX = 0;
	for (int i = 0; i < shape.getPointCount(); i++)
	{
		centerX += shape.getPoint(i).x;
	}
	centerX /= shape.getPointCount();

	return centerX;
}

float shapeCenterY(myShape shape)
{
	float centerY = 0;
	for (int i = 0; i < shape.getPointCount(); i++)
	{
		centerY += shape.getPoint(i).y;
	}
	centerY /= shape.getPointCount();
	
	return centerY;
}

sf::RectangleShape getAABB(myShape shape)
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
 
myShape configureShape(myShape shape)
{
	shape.setOrigin(shapeCenterX(shape), shapeCenterY(shape));
	shape.rotation = rand() % 360 + 1;	
	shape.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
	shape.bounds = getAABB(shape);
	
	return shape;
}

sf::Vector2i findEmptyCell(vector<vector<bool>> isCellOccupied, int cellX, int cellY)
{
	sf::Vector2i emptyCell; 
	int cellXi, cellYi;
	while (true)
	{
		cellXi = rand() % cellX;
		cellYi = rand() % cellY;
		if (!isCellOccupied[cellXi][cellYi])
		{
			break;
		}
	}
	emptyCell.x = cellXi;
	emptyCell.y = cellYi;
	return emptyCell;
}

int main( void )
{
	srand(time(NULL));

	int counter;
	int collectedSpaceParts = 0;
	float oxygen = 100.0f;
	int alienRadius;
	string junk;

	//config variables
	bool isViewOn = true;
	
	//difficulty variables
	int neededSpaceParts = 0;
	int alienCount = 1;
	int mapWMultiplier = 1;
	int mapHMultiplier = 1;
	float depletionRate = 0.001;
	int refillSpawnTime = 25000;
	float alienDamage = 20;
	
	//screen
	bool isStartScreenOn = true;
	bool gameWin = false;
	bool gameLose = false;
	bool isHelpScreenOn = false;

	//config loader
	ifstream fileConfig("/Users/Meg/Desktop/gamedev/space/space/res/texts/config.txt");
	if (fileConfig.is_open())
	{
		fileConfig >> junk;
		fileConfig >> isViewOn;
		fileConfig.close();
	}
	else
	{
		cout << "Failed to load config (/Users/Meg/Desktop/gamedev/space/space/res/texts/config.txt)" << endl;
	}

	//difficulty loader
	int difficulty;
	cout << "Enter difficulty (1 - Easiest -> 5 Hardest): " << endl;
	cin >> difficulty;
	ifstream fileDifficulty("/Users/Meg/Desktop/gamedev/space/space/res/texts/difficulty_" + to_string(difficulty) + ".txt");
	if (fileDifficulty.is_open())
	{
		fileDifficulty >> junk;
		fileDifficulty >> neededSpaceParts;
		fileDifficulty >> junk;
		fileDifficulty >> alienCount;
		fileDifficulty >> junk;
		fileDifficulty >> mapWMultiplier;
		fileDifficulty >> junk;
		fileDifficulty >> mapHMultiplier;
		fileDifficulty >> junk;
		fileDifficulty >> depletionRate;
		fileDifficulty >> junk;
		fileDifficulty >> refillSpawnTime;
		fileDifficulty >> junk;
		fileDifficulty >> alienDamage;
		fileDifficulty.close();
	}
	else
	{
		cout << "Failed to load difficulty (/Users/Meg/Desktop/gamedev/space/space/res/texts/difficulty_X.txt)" << endl;
	}
	
	sf::Texture background;
	background.setRepeated(true);
	sf::Texture alienTexture;
	sf::Texture helpScreen;
	sf::Texture partsTexture[6];
	if(!background.loadFromFile("/Users/Meg/Desktop/gamedev/space/space/res/images/space.png"))
	{
        cout << "Failed to load background texture (/Users/Meg/Desktop/gamedev/space/space/res/images/space.png)" << endl;
    }
    sf::Sprite backgroundsprite(background);
	

	if(!helpScreen.loadFromFile("/Users/Meg/Desktop/gamedev/space/space/res/images/help.png"))
	{
        cout << "Failed to load help image (/Users/Meg/Desktop/gamedev/space/space/res/images/help.png)" << endl;
    }
    sf::Sprite helpSprite(helpScreen);
	
	//time for player movement
	sf::Clock clock;
	float time;
	//time for oxygen bar depletion
	sf::Clock tclock;
	sf::Time tank_time;
	sf::Time tank_thresh = sf::milliseconds(1);
	//time for oxygen tank refill
	sf::Clock rclock;
	sf::Time refill_time;
	sf::Time refill_thresh = sf::milliseconds(refillSpawnTime);
	//counter for speedboost
	sf::Clock sclock;
	sf::Time speed_time;
	sf::Time speed_thresh = sf::milliseconds(7500);
	//counter for alien movement
	sf::Clock aclock;
	sf::Time alien_time[alienCount];
	sf::Time alien_mastertime;
	sf::Time alien_thresh[alienCount];
	for (int i = 0; i < alienCount; i++)
	{
		alien_thresh[i] = sf::milliseconds(1000*(rand()%5+3));
	}

	//----- text for displaying collected space parts, and win, lose, start prompt
	int fontSize = 24;
	sf::Font font;
	if (!font.loadFromFile("/Users/Meg/Desktop/gamedev/space/space/res/fonts/FontFile.ttf"))
	{
		cout << "Failed to load font (/Users/Meg/Desktop/gamedev/space/space/res/fonts/FontFile.ttf)" << endl;
	}
	sf::Text textCollected;
	sf::Text textHelp;
	sf::Text WinPrompt;
	sf::Text LosePrompt;
	sf::Text StartPrompt;
	textCollected.setFont(font);
	textCollected.setCharacterSize(fontSize);
	textCollected.setFillColor(sf::Color::White);
	textCollected.setFont(font);
	textCollected.setCharacterSize(fontSize);
	textCollected.setFillColor(sf::Color::White);	
	textHelp.setFont(font);
	textHelp.setCharacterSize(fontSize);
	textHelp.setFillColor(sf::Color::White);
	textHelp.setString("H: Help");		
	WinPrompt.setFont(font);
	WinPrompt.setCharacterSize(150);
	WinPrompt.setFillColor(sf::Color::Green);
	WinPrompt.setString("You Win!");
	WinPrompt.setPosition(sf::Vector2f((winW/2)-300, (winH/2)-100));
	LosePrompt.setFont(font);
	LosePrompt.setCharacterSize(150);
	LosePrompt.setFillColor(sf::Color::Red);
	LosePrompt.setString("You Lose!");	
	LosePrompt.setPosition(sf::Vector2f((winW/2)-320, (winH/2)-100));	
	StartPrompt.setFont(font);
	StartPrompt.setCharacterSize(150);
	StartPrompt.setFillColor(sf::Color::Blue);
	StartPrompt.setString("Click Here to Start!");	
	StartPrompt.setPosition(sf::Vector2f(100, (winH/2)-100));	
	//-----
	
	//key binds
	sf::Keyboard::Key keySonar = sf::Keyboard::S;
	sf::Keyboard::Key keySonarRefill = sf::Keyboard::A;
	sf::Keyboard::Key keySpeedBoost = sf::Keyboard::Space;
	sf::Keyboard::Key keyHelp = sf::Keyboard::H;
	sf::Keyboard::Key keyClose = sf::Keyboard::Escape;
	
	
	//variables for reading text file's shape characteristics
	int pointCount;
	float posX, posY;
	
	//map size and dividing the map by cells to spawn shapes
	int mapW = winW*mapWMultiplier;
	int mapH = winH*mapHMultiplier;
	backgroundsprite.setTextureRect(sf::IntRect(0,0,mapW,mapH));
	int cellX = 10*mapWMultiplier;
	int cellY = 5*mapHMultiplier;
	int cellWidth = winW/(10*mapWMultiplier);
	int cellHeight = winH/(5*mapHMultiplier);
	sf::Vector2i cellIndex;
	vector<vector<bool>> isCellOccupied(cellX, vector<bool>(cellY));
	
	//initialize text for displaying collected space parts
	textCollected.setString("0/" + to_string(neededSpaceParts));
	
	//declare shapes
	myShape spaceParts[neededSpaceParts];
	myShape oRefill;
	myShape astronaut;
	myShape spaceship;
	myShape aliens[alienCount];
	
	//----- load dimensions and randomize position for unique shapes
	astronaut.loadDimensions("/Users/Meg/Desktop/gamedev/space/space/res/texts/astronaut.txt");
	cellIndex = findEmptyCell(isCellOccupied, cellX, cellY);
	isCellOccupied[cellIndex.x][cellIndex.y] = true;
	astronaut.setPosition(cellWidth*cellIndex.x, cellHeight*cellIndex.y);
	
	spaceship.loadDimensions("/Users/Meg/Desktop/gamedev/space/space/res/texts/spaceship.txt");
	cellIndex = findEmptyCell(isCellOccupied, cellX, cellY);
	isCellOccupied[cellIndex.x][cellIndex.y] = true;
	spaceship.setPosition(cellWidth*cellIndex.x, cellHeight*cellIndex.y);
	
	oRefill.loadDimensions("/Users/Meg/Desktop/gamedev/space/space/res/texts/gasrefill.txt");
	cellIndex = findEmptyCell(isCellOccupied, cellX, cellY);
	isCellOccupied[cellIndex.x][cellIndex.y] = true;
	oRefill.setPosition(cellWidth*cellIndex.x, cellHeight*cellIndex.y);
	//-----
	
	//----- dimensions and position for shape arrays
	//space parts dimensions and position
	counter = -1;
	ifstream fileSpaceParts("/Users/Meg/Desktop/gamedev/space/space/res/texts/spaceparts.txt");
	if (fileSpaceParts.is_open())
	{
		if (neededSpaceParts > 0)
		{
			while (!fileSpaceParts.eof() && counter + 1 < neededSpaceParts)
			{
				counter++;
				fileSpaceParts >> pointCount;
				spaceParts[counter].setPointCount(pointCount);
				if (pointCount > 0)
				{
					for (int j = 0; j < pointCount; j++)
					{
						fileSpaceParts >> posX >> posY;
						spaceParts[counter].setPoint(j, sf::Vector2f(posX, posY));
					}
					cellIndex = findEmptyCell(isCellOccupied, cellX, cellY);
					isCellOccupied[cellIndex.x][cellIndex.y] = true;
					spaceParts[counter].setPosition(cellWidth*cellIndex.x, cellHeight*cellIndex.y);
					spaceParts[counter].cellIndex = cellIndex;
					spaceParts[counter].setScale(0.5f, 0.5f);
				}
				pointCount = 0;
			}
			
		}
		fileSpaceParts.close();
	}
	else
	{
		cout << "Failed to load space parts' dimensions (/Users/Meg/Desktop/gamedev/space/space/res/texts/values.txt)" << endl;
	}
	/*
	if the number of space parts is greater than the number of dimensions in the file,
	cycle through existing space parts for the excess space parts
	*/
	if (counter + 1 < neededSpaceParts)
	{
		for (int i = counter , j = 0; i < neededSpaceParts; i++)
		{
			if (j == counter)
			{
				j = 0;
			}
			spaceParts[i] = spaceParts[j];
			cellIndex = findEmptyCell(isCellOccupied, cellX, cellY);
			isCellOccupied[cellIndex.x][cellIndex.y] = true;
			spaceParts[i].setPosition(cellWidth*cellIndex.x, cellHeight*cellIndex.y);
			spaceParts[i].cellIndex = cellIndex;
			spaceParts[i].setScale(0.5f, 0.5f);
			j++;
		}
	}
	
	//aliens dimensions and position
	counter = -1;
	ifstream fileAliens("/Users/Meg/Desktop/gamedev/space/space/res/texts/aliens.txt");
	if (fileAliens.is_open())
	{
		if (alienCount > 0)
		{
			while (!fileAliens.eof() && counter + 1 < alienCount)
			{
				counter++;
				fileAliens >> pointCount;
				aliens[counter].setPointCount(pointCount);
				if (pointCount > 0)
				{
					for (int j = 0; j < pointCount; j++)
					{
						fileAliens >> posX >> posY;
						aliens[counter].setPoint(j, sf::Vector2f(posX, posY));
					}
					cellIndex = findEmptyCell(isCellOccupied, cellX, cellY);
					isCellOccupied[cellIndex.x][cellIndex.y] = true;
					aliens[counter].setPosition(cellWidth*cellIndex.x, cellHeight*cellIndex.y);
					aliens[counter].cellIndex = cellIndex;
					aliens[counter].setScale(0.5f, 0.5f);
				}
				pointCount = 0;
			}
			
		}
		fileAliens.close();
	}
	else
	{
		cout << "Failed to load aliens' dimensions (/Users/Meg/Desktop/gamedev/space/space/res/texts/aliens.txt)" << endl;
	}
	/*
	if the number of aliens is greater than the number of dimensions in the file,
	cycle through existing aliens for the excess aliens
	*/
	if (counter + 1 < alienCount)
	{
		for (int i = counter , j = 0; i < alienCount; i++)
		{
			if (j == counter)
			{
				j = 0;
			}
			aliens[i] = aliens[j];
			cellIndex = findEmptyCell(isCellOccupied, cellX, cellY);
			isCellOccupied[cellIndex.x][cellIndex.y] = true;
			aliens[i].setPosition(cellWidth*cellIndex.x, cellHeight*cellIndex.y);
			aliens[i].cellIndex = cellIndex;
			aliens[i].setScale(0.5f, 0.5f);
			j++;
		}
	}
	//-----

	astronaut.setFillColor(sf::Color::Transparent);

	//load textures for unique shapes
	spaceship.loadTexture("/Users/Meg/Desktop/gamedev/space/space/res/images/spaceship.png");
	astronaut.loadTexture("/Users/Meg/Desktop/gamedev/space/space/res/images/spaceboi.png");
	oRefill.loadTexture("/Users/Meg/Desktop/gamedev/space/space/res/images/refill.png");
	
	//space parts textures
	string textureSpacePart = "/Users/Meg/Desktop/gamedev/space/space/res/images/parts";
    for (int i = 0; i < 6; i++){
        textureSpacePart += (to_string(i) + ".png");
        if(!partsTexture[i].loadFromFile(textureSpacePart)){
            cout << "Failed to load image " << i << endl;
        }
        textureSpacePart = "/Users/Meg/Desktop/gamedev/space/space/res/images/parts";
    }
	//cycle through textures for the space parts
	for(int i = 0, j = 0; i < neededSpaceParts; i++)
	{
		if (j == 6)
		{
			j = 0;
		}
		spaceParts[i].setTexture(&partsTexture[j]);
		j++;
	}

	//alien texture
	if(!alienTexture.loadFromFile("/Users/Meg/Desktop/gamedev/space/space/res/images/alien.png")){
		cout << "Failed to load alien texture (/Users/Meg/Desktop/gamedev/space/space/res/images/alien.png)" << endl;
	}
	//set texture to all aliens
	for(int i = 0; i < alienCount; i++)
	{
		aliens[i].setTexture(&alienTexture);
	}

	//initialize oxygen bar
	sf::RectangleShape oTank;
	sf::RectangleShape oTankMax;
	oTank.setFillColor(sf::Color::Green);
	oTank.setPosition(astronaut.getPosition().x,astronaut.getPosition().y);
	oTank.setSize(sf::Vector2f(oxygen*4, 10));
	oTankMax.setFillColor(sf::Color::Transparent);
	oTankMax.setOutlineColor(sf::Color::White);
	oTankMax.setOutlineThickness(1);
	oTankMax.setPosition(100,850);
	oTankMax.setSize(sf::Vector2f(oxygen*4, 10));

	//configure shape
	astronaut = configureShape(astronaut);
	spaceship = configureShape(spaceship);
	oRefill = configureShape(oRefill);
	for (int i = 0; i < neededSpaceParts; i++)
	{
		spaceParts[i] = configureShape(spaceParts[i]);
	}
	for (int i = 0; i < alienCount; i++)
	{
		aliens[i] = configureShape(aliens[i]);
	}
	
	//sonar
	int soundCounter = 0;
	int spacePartDistance = mapW*mapW + mapH*mapH;
	int currentSpacePartDistance;
	int closestSpacePartIndex;
	sf::Sound soundSonar[249];
	
	//sonar for space parts
	sf::SoundBuffer bufferSonar;
	sf::Listener::setPosition(astronaut.getPosition().x, 0, astronaut.getPosition().y);
	if( !bufferSonar.loadFromFile("/Users/Meg/Desktop/gamedev/space/space/res/audio/sonar.wav") )
		cout << "Unable to load sonar file (/Users/Meg/Desktop/gamedev/space/space/res/audio/sonar.wav)" << endl;
	
	//sonar for refill
	sf::SoundBuffer bufferSonarRefill;
	if( !bufferSonarRefill.loadFromFile("/Users/Meg/Desktop/gamedev/space/space/res/audio/sonar_refill.wav") )
		cout << "Unable to load sonar_refill file (/Users/Meg/Desktop/gamedev/space/space/res/audio/sonar_refill.wav)" << endl;
	
	//sound for parts
	sf::SoundBuffer bufferParts;
	if( !bufferParts.loadFromFile("/Users/Meg/Desktop/gamedev/space/space/res/audio/parts.wav"))
		cout <<  "Unable to load refill file (/Users/Meg/Desktop/gamedev/space/space/res/audio/parts.wav)" << endl;
	sf::Sound soundParts;
	soundParts.setBuffer(bufferParts);
	soundParts.setRelativeToListener(true);
	soundParts.setPosition(0.0f, 0.0f, 0.0f);
	soundParts.setVolume(50.0f);

	//sound for refill
	sf::SoundBuffer bufferRefill;
	if( !bufferRefill.loadFromFile("/Users/Meg/Desktop/gamedev/space/space/res/audio/refill.wav"))
		cout <<  "Unable to load refill file (/Users/Meg/Desktop/gamedev/space/space/res/audio/refill.wav)" << endl;
	sf::Sound soundRefill;
	soundRefill.setBuffer(bufferRefill);
	soundRefill.setRelativeToListener(true);
	soundRefill.setPosition(0.0f, 0.0f, 0.0f);
	soundRefill.setVolume(50.0f);

	//sound for alien attack
	sf::SoundBuffer bufferAlienAttack;
	if( !bufferAlienAttack.loadFromFile("/Users/Meg/Desktop/gamedev/space/space/res/audio/oof.wav"))
		cout <<  "Unable to load alien attack file (/Users/Meg/Desktop/gamedev/space/space/res/audio/oof.wav)" << endl;
	sf::Sound soundAlien;
	soundAlien.setBuffer(bufferAlienAttack);
	soundAlien.setRelativeToListener(true);
	soundAlien.setPosition(0.0f, 0.0f, 0.0f);
	soundAlien.setVolume(50.0f);
	
	//sound for complete parts
	sf::SoundBuffer bufferComplete;
	if( !bufferComplete.loadFromFile("/Users/Meg/Desktop/gamedev/space/space/res/audio/o.wav"))
		cout <<  "Unable to load complete parts file (/Users/Meg/Desktop/gamedev/space/space/res/audio/o.wav)" << endl;
	sf::Sound soundComplete;
	soundComplete.setBuffer(bufferComplete);
	soundComplete.setRelativeToListener(true);
	soundComplete.setPosition(0.0f, 0.0f, 0.0f);
	soundComplete.setVolume(50.0f);
	soundComplete.setPitch(2.0f);

	//sound for incomplete parts
	sf::SoundBuffer bufferIncomplete;
	if( !bufferIncomplete.loadFromFile("/Users/Meg/Desktop/gamedev/space/space/res/audio/x.wav"))
		cout <<  "Unable to load incomplete parts file (/Users/Meg/Desktop/gamedev/space/space/res/audio/x.wav)" << endl;
	sf::Sound soundIncomplete;
	soundIncomplete.setBuffer(bufferIncomplete);
	soundIncomplete.setRelativeToListener(true);
	soundIncomplete.setPosition(0.0f, 0.0f, 0.0f);
	soundIncomplete.setVolume(50.0f);
	soundIncomplete.setPitch(2.0f);
	
	//background music
	sf::Sound soundBG;
	sf::SoundBuffer bufferBG;
	if (!bufferBG.loadFromFile("/Users/Meg/Desktop/gamedev/space/space/res/audio/get.wav"))
	{
		cout << "Unable to load bacground music file (/Users/Meg/Desktop/gamedev/space/space/res/audio/get.wav)" << endl;
	}
	soundBG.setBuffer(bufferBG);
	soundBG.setRelativeToListener(true);
	soundBG.setPosition(0.0f, 0.0f, 0.0f);
	soundBG.setLoop(true);
	soundBG.setVolume(30);
	soundBG.play();
	
	//User Guide
	cout << "How to Play:" << endl;
	cout << "Help: H" << endl;
	cout << "Sonar for Oxygen Refill: A" << endl;
	cout << "Speed Boost: Space" << endl;
	
	//set view
	sf::View view(sf::Vector2f(astronaut.getPosition().x, astronaut.getPosition().y), sf::Vector2f(300.f, 200.f));
	view.zoom(1.5f);
	sf::Vector2i realPos;
	sf::Vector2f viewPos;
	
	//bounding map rectangle
	sf::mapBounds;
	mapBounds.setFillColor(sf::Color::Transparent);
	mapBounds.setOutlineColor(sf::Color::White);
	mapBounds.setOutlineThickness(1);
	mapBounds.setPosition(0,0);
	mapBounds.setSize(sf::Vector2f(mapW, mapH));
	
    sf::RenderWindow window( sf::VideoMode( winW, winH ), "spaceboi" );
	window.setFramerateLimit( FPS );
	
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch	(event.type)
			{
			case sf::Event::Closed:
                window.close();
				break;
			case sf::Event::MouseButtonReleased:
				if (isStartScreenOn)
				{
					isStartScreenOn = false;
					clock.restart();
					tclock.restart();
					sclock.restart();
					aclock.restart();
				}
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == keyClose)
				{
					window.close();
				}
				break;
			case sf::Event::KeyReleased:
				if (event.key.code == keySonar)
				{
					if (collectedSpaceParts < neededSpaceParts)
					{
						//search closest space part to astronaut
						spacePartDistance = mapW*mapW + mapH*mapH; //reset distance to maximum distance
						for (int i = 0; i < neededSpaceParts; i++)
						{
							if (spaceParts[i].isVisible)
							{
								posX = astronaut.getPosition().x - spaceParts[i].getPosition().x;
								posY = astronaut.getPosition().y - spaceParts[i].getPosition().y;
								currentSpacePartDistance = posX*posX + posY*posY;
								if (currentSpacePartDistance < spacePartDistance)
								{
									spacePartDistance = currentSpacePartDistance;
									closestSpacePartIndex = i;
								}
							}
						}
						for (int i = 0; i < 256; i++)
						{
							if(soundSonar[i].getStatus() != sf::SoundSource::Playing)
							{
								soundCounter = i;
								break;
							}
							soundCounter = 0;
						}
						soundSonar[soundCounter].setPosition(spaceParts[closestSpacePartIndex].getPosition().x, 0, spaceParts[closestSpacePartIndex].getPosition().y);
						soundSonar[soundCounter].setBuffer(bufferSonar);
						soundSonar[soundCounter].setVolume(50.0f);
						soundSonar[soundCounter].setMinDistance(mapW*mapH);
						soundSonar[soundCounter].setAttenuation(5.0f);
						soundSonar[soundCounter].play();
					}
				}
				if (event.key.code == keySonarRefill)
				{
					if (oRefill.isVisible)
					{
						for (int i = 0; i < 256; i++)
						{
							if(soundSonar[i].getStatus() != sf::SoundSource::Playing)
							{
								soundCounter = i;
								break;
							}
							soundCounter = 0;
						}
						posX = astronaut.getPosition().x - oRefill.getPosition().x;
						posY = astronaut.getPosition().y - oRefill.getPosition().y;
						spacePartDistance = posX*posX + posY*posY;
						soundSonar[soundCounter].setPosition(oRefill.getPosition().x, 0, oRefill.getPosition().y);
						soundSonar[soundCounter].setBuffer(bufferSonarRefill);
						soundSonar[soundCounter].setVolume(50.0f);
						soundSonar[soundCounter].setMinDistance(mapW*mapH);
						soundSonar[soundCounter].setAttenuation(5.0f);
						soundSonar[soundCounter].play();
					}
				}
				if(event.key.code == keySpeedBoost)
				{
					if(oxygen > 20 && !astronaut.speedboost)
					{
						astronaut.speedboost = true;
						oxygen -= 20;
						sclock.restart();
					}
					else
					{
						cout << "Not enough oxygen!" << endl; 
					}
				}
				if (event.key.code == keyHelp)
				{
					isHelpScreenOn = !isHelpScreenOn;
					clock.restart();
					tclock.restart();
					sclock.restart();
					aclock.restart();
				}
				break;
			}
        }
		
		time = clock.restart().asSeconds();
		
		if (gameLose)
		{
			window.clear();
			window.draw(backgroundsprite);
			window.setView(window.getDefaultView());
			window.draw(LosePrompt);
			window.display();
		}
		else if (gameWin)
		{
			window.clear();
			window.draw(backgroundsprite);
			window.setView(window.getDefaultView());
			window.draw(WinPrompt);
			window.display();
		}
		else if (isHelpScreenOn)
		{
			window.clear();
			window.draw(helpSprite);
			window.setView(window.getDefaultView());
			window.display();
		}
		else if(!isStartScreenOn)
		{
			//alien random movement
			alien_mastertime += aclock.restart();
			for (int i = 0; i < alienCount; i++)
			{
				alien_time[i] += alien_mastertime;
				if(alien_time[i] >= alien_thresh[i])
				{
					alien_time[i] = sf::milliseconds(0);
					alien_thresh[i] = sf::milliseconds(1000*(rand()%5+3));
					aliens[i].angle = rand() % 360;
				}
			}
			alien_mastertime = sf::milliseconds(0);
			
			//alien follow astronaut if within radius
			for (int i = 0; i < alienCount; i++)
			{
				posX = astronaut.getPosition().x - aliens[i].getPosition().x;
				posY = astronaut.getPosition().y - aliens[i].getPosition().y;
				alienRadius = posX*posX + posY*posY;
				if (alienRadius < 30000)
				{
					aliens[i].angle = (atan2(posY, posX)) * 180 / PI;
				}
			}
			
			//astronaut follows mouse
			realPos = sf::Mouse::getPosition(window); //get mouse window coordinates
			viewPos = window.mapPixelToCoords(realPos); //convert coordinates to view coordinates
			posX = astronaut.getPosition().x - viewPos.x; //get x distance from astronaut to mouse
			posY = astronaut.getPosition().y - viewPos.y; //get y distance from astronaut to mouse
			//follow if mouse is within map
			if (viewPos.x > 0 && viewPos.y > 0 && viewPos.x < mapW && viewPos.y < mapH)
			{
				if(abs(posX) > thresholdMouseFollow && abs(posY) > thresholdMouseFollow) //stop following when threshold is reached
				{
					astronaut.angle = (atan2(posY, posX)) * 180 / PI; //get angle from astronaut to mouse
					astronaut.move(-cos(PI*astronaut.angle/180.f)*astronaut.speed*time, -sin(PI*astronaut.angle/180.f)*astronaut.speed*time); //move astronaut towards mouse
				}
			}

			//listener follows astronaut
			sf::Listener::setPosition(astronaut.getPosition().x, 0, astronaut.getPosition().y);

			//alien movement with map bounds
			for(int i = 0; i < alienCount; i++)
			{
				if (aliens[i].getPosition().x < 0 || aliens[i].getPosition().x > mapW || aliens[i].getPosition().y < 0 || aliens[i].getPosition().y > mapH)
				{
					aliens[i].angle += 180;
				}
				aliens[i].move(cos(PI*aliens[i].angle/180.f)*aliens[i].speed*time, sin(PI*aliens[i].angle/180.f)*aliens[i].speed*time);
			}

			//oxygen bar depletion
			oTank.setPosition(astronaut.getPosition().x-200,astronaut.getPosition().y+125);
			oTankMax.setPosition(astronaut.getPosition().x-200, astronaut.getPosition().y+125);
			tank_time += tclock.restart();
			while(tank_time >= tank_thresh && oxygen > 0)
			{
				oxygen -= depletionRate;
				oTank.setSize(sf::Vector2f(oxygen*4, 10));
				tank_time -= tank_thresh;
			}
			if(oxygen > 15 && oxygen <= 50)
			{
				oTank.setFillColor(sf::Color::Yellow);
			}
			if(oxygen <= 15)
			{
				oTank.setFillColor(sf::Color::Red);
			}
			if(oxygen <= 0)
			{
				gameLose = true;
			}

			//oxygen refill timer
			if(!oRefill.isVisible)
			{
				refill_time += rclock.restart();
				if(refill_time >= refill_thresh)
				{
					oRefill.isVisible = true;
					refill_time = sf::milliseconds(0);
					cout << "Refill Available!" << endl;
				}
			}
			
			//speedboost
			if(astronaut.speedboost)
			{
				astronaut.speed = 300;
				speed_time += sclock.restart();
				if (speed_time >= speed_thresh)
				{
					astronaut.speedboost = false;
					speed_time = sf::milliseconds(0);
					astronaut.speed = 100;
				}

			}

			if(astronaut.speedboost && astronaut.speed != 300)
			{
				oxygen += 20;
			}

			else if(!astronaut.speedboost)
			{
				astronaut.speed = 100;
			}	
			
			//Collision Checker: astronaut vs aliens
			for (int i = 0; i < alienCount; i++)
			{
				if (aliens[i].isVisible)
				{
					if (AABBCheck(astronaut.bounds, aliens[i].bounds))
					{
						if (SATCheck(astronaut, aliens[i]))
						{
							aliens[i].isVisible = false;
							soundAlien.play();
							isCellOccupied[aliens[i].cellIndex.x][aliens[i].cellIndex.y] = false; //set the cell previously occupied free for spawning
							oxygen -= alienDamage;
							cout << "Attacked by Alien!" << endl;
							cellIndex = findEmptyCell(isCellOccupied, cellX, cellY);
							isCellOccupied[cellIndex.x][cellIndex.y] = true;
							aliens[i].setPosition(cellWidth*cellIndex.x, cellHeight*cellIndex.y);
							aliens[i].cellIndex = cellIndex;
						}
					}
				}
			}
			
			//Collision Checker: astronaut vs space parts
			for (int i = 0; i < neededSpaceParts; i++)
			{
				if (spaceParts[i].isVisible)
				{
					if (AABBCheck(astronaut.bounds, spaceParts[i].bounds))
					{
						if (SATCheck(astronaut, spaceParts[i]))
						{
							soundParts.play();
							spaceParts[i].isVisible = false;
							isCellOccupied[spaceParts[i].cellIndex.x][spaceParts[i].cellIndex.y] = false; //set the cell previously occupied free for spawning
							collectedSpaceParts++;
							textCollected.setString(to_string(collectedSpaceParts) + "/" + to_string(neededSpaceParts)); //update text for collected space parts
							spacePartDistance = mapW*mapW + mapH*mapH; //reset closest distance to a space part
						}
					}
				}
			}

			//Collision Checker: astronaut vs spaceship
			if (AABBCheck(astronaut.bounds, spaceship.bounds))
			{
				if (SATCheck(astronaut, spaceship))
				{
					if (collectedSpaceParts >= neededSpaceParts)
					{
						soundComplete.play();
						cout << "You've fixed the spaceship" << endl;
						gameWin = true;
					}
					else
					{
						soundIncomplete.play();
						//cout << "Incomplete parts" << endl;
					}
				}
			}
			
			//Collision Checker: astronaut vs refill
			if (oRefill.isVisible)
			{
				if (AABBCheck(astronaut.bounds, oRefill.bounds))
				{
					if(SATCheck(astronaut, oRefill))
					{
						soundRefill.play();
						oRefill.isVisible = false;
						isCellOccupied[oRefill.cellIndex.x][oRefill.cellIndex.y] = false; //set the cell previously occupied free for spawning
						if(oxygen < 50)
						{
							oxygen += 50;
							oTank.setFillColor(sf::Color::Green);
						}
						else
						{
							oxygen += 100-oxygen;
							oTank.setFillColor(sf::Color::Green);
						}
						cout << "Refilled!" << endl;
						cellIndex = findEmptyCell(isCellOccupied, cellX, cellY);
						isCellOccupied[cellIndex.x][cellIndex.y] = true;
						oRefill.setPosition(cellWidth*cellIndex.x, cellHeight*cellIndex.y);
						oRefill.cellIndex = cellIndex;
					}
				}
			}
			
			window.clear();
			window.draw(backgroundsprite);
			view.setCenter(astronaut.getPosition().x, astronaut.getPosition().y); //center view to astronaut
			if (isViewOn)
			{
				window.setView(view); //comment this out to disable view
			}
			else
			{
				window.setView(window.getDefaultView());
			}
			textHelp.setPosition(astronaut.getPosition().x-195, astronaut.getPosition().y-150); //display text for collected space parts
			textCollected.setPosition(astronaut.getPosition().x+165, astronaut.getPosition().y-150); //display text for collected space parts
			spaceship.rotate(spaceship.rotation*time);
			spaceship.bounds = getAABB(spaceship); //get AABB bounds of spaceship
			window.draw(spaceship);
			for (int i = 0; i < neededSpaceParts; i++)
			{
				if (spaceParts[i].isVisible)
				{
					spaceParts[i].rotate(spaceParts[i].rotation*time); //rotate space parts
					spaceParts[i].bounds = getAABB(spaceParts[i]); //get AABB bounds of space parts
					window.draw(spaceParts[i]);
				}
			}
			for (int i = 0; i < alienCount; i++)
			{
				if (aliens[i].isVisible)
				{
					aliens[i].bounds = getAABB(aliens[i]); //get AABB bounds of aliens
					window.draw(aliens[i]);
				}
			}
			astronaut.rotate(astronaut.rotation*time);
			astronaut.bounds = getAABB(astronaut); //get AABB bounds of astronaut
			window.draw(astronaut);
			if(oRefill.isVisible)
			{
				oRefill.rotate(oRefill.rotation*time);
				oRefill.bounds = getAABB(oRefill);
				window.draw(oRefill);
			}
			window.draw(textHelp);
			window.draw(textCollected);
			window.draw(oTank);
			window.draw(oTankMax);
			window.draw(mapBounds);
			window.display();
		}
		else
		{
			window.clear();
			window.draw(backgroundsprite);
			window.setView(window.getDefaultView());
			window.draw(StartPrompt);
			window.display();
		}
    }
	
    return 0;
}
