#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <vector>
#include <math.h>

#define FPS 60
#define winW 1000
#define winH 1000

using namespace std;

sf::VertexArray normalize(sf::VertexArray R)
{
	int size = R.getVertexCount();
	float mag = 0.0;
	for(int i = 0; i < size; i++)
	{
		//unit vector = vector/magnitude
		mag = sqrt(R[i].position.x*R[i].position.x + R[i].position.y*R[i].position.y);
		R[i].position.x /= mag;
		R[i].position.y /= mag;
	}
	return R;
}

vector<float> pascalize(float t, vector<float> R, int order)
{
	//store 1-t so we can keep reusing it without recalculating it again and again
	float a = 1-t;

	/*this nested for loop works like this:
	1 3 3 1
	1a 3a 3a 1
	1aa 3aa 3a 1
	1aaa 3aa 3a 1*/
	for(int i = order - 1; i >= 0; i--)
	{
		for(int j = i; j >= 0; j--)
		{
			R[j] *= a;
		}	
	}

	/*this nested for loop works like this:
	1 3 3 1
	1 3t 3t 1t
	1 3t 3tt 1tt
	1 3t 3tt 1ttt*/
	for(int i = 1; i <= order; i++)
	{
		for(int j = i; j <= order; j++)
		{
			R[j] *= t;
		}	
	}
	
	return R;
}

sf::VertexArray pascalBezier(sf::VertexArray points, int step, vector<float> R)
{
	/*working backwards, let's say order is 2
	which gave us R = {1,2,1} which has a size of 3,
	so we retrieve the order by subtracting the size by 1*/
	int order = R.size() - 1;
	//make bezier whose size is the smoothness + 1 (where + 1 is the start point)
	sf::VertexArray bezier(sf::LineStrip, step+1);
	//this is for storing the soon-to-be modified pascal coefficients
	vector<float> R2(order);
	float tmp = step; //store the step
	tmp = 1/tmp; //and get the reciprocal
	float t = tmp; //put it in t
	/*so let's say step=4, t will store 1/4, and tmp will also store 1/4.
	the code below will make it so
	t increases by tmp each iteration so we keep moving by step.
	1/4 + 1/4 = 2/4 -> 2/4 + 1/4 = 3/4*/

	bezier[0] = points[0]; //start point
	bezier[step] = points[order]; //end point
		
	for(int i = 1; i < step; i++)
	{
		R2 = pascalize(t, R, order); //pascal coefficients gets multiplied by t and 1-t
		t += tmp; //increment t by 1/step
		bezier[i].position.x = 0; //make sure they're zero
		bezier[i].position.y = 0;
		for(int j = 0; j <= order; j++)
		{
			//multiply each point by their corresponding coefficients
			//for example, point 0 by gets multiplied by R2[0]
			bezier[i].position.x += (points[j].position.x*R2[j]);
			bezier[i].position.y += (points[j].position.y*R2[j]);
		}
	}
	return bezier;
}

sf::VertexArray reTan(sf::VertexArray tanlines, sf::VertexArray bezier, sf::VertexArray derivative, float factor)
{
	int V = derivative.getVertexCount() - 1;
	/*example: smoothness=100, V=4
	  actual size of bezier would be 101 to account for starting point
	  likewise size of derivative would be 5
	  so we subtract 1 from both of them and get the ratio
	  to get (101-1)/(5-1) = 100/4 = 25
	  which is the step to get the 4 derivatives at 0, 25, 50, 100*/
	int step = (bezier.getVertexCount()-1)/V;
	
	//i increments by 2 since we're doing 2 points (start point and end point) of the tanlines per loop
	//j increments by 1 since 2 points (a line) corresponds to one derivative
	//k increments by step (25 for example) to get the equally spaced derivatives on the bezier
	for(int i = 0, j = 0, k = 0; j < V; i+=2, j++, k += step)
	{
		tanlines[i].position = bezier[k].position; //starting point is the bezier point
		//add the derivative (which is the direction) to the starting point to form the tangent line
		//factor is multiplied to lengthen the line
		tanlines[i+1].position.x = bezier[k].position.x + factor*derivative[j].position.x;
		tanlines[i+1].position.y = bezier[k].position.y + factor*derivative[j].position.y;
	}
	return tanlines;
}

sf::VertexArray reNom(sf::VertexArray tanlines)
{
	int points = tanlines.getVertexCount();
	float rad = 0.0;
	float dx, dy, cs, sn;
	//start with i=1 so previousp point is point 0
	for(int i = 1; i < points; i+=2)
	{
		//get x and y distance from current point and previous point
		dx = tanlines[i].position.x - tanlines[i-1].position.x;
		dy = tanlines[i].position.y - tanlines[i-1].position.y;
		rad = atan2(dy,dx); //get the angle in radians
		cs = cos(rad);
		sn = sin(rad);
		//math stuff to rotate a point around another point
		tanlines[i].position.x = (dx*cs) + (dy*sn) + tanlines[i-1].position.x;
		tanlines[i].position.y = (-dy*cs) - (dx*sn) + tanlines[i-1].position.y;
		tanlines[i-1].color = sf::Color::Red;
		tanlines[i].color = sf::Color::Red;
	}
	return tanlines;
}

sf::VertexArray deriBezier(sf::VertexArray points, int step, vector<float> R, int V)
{
	/*working backwards, let's say order is 2
	which gave us R = {1,2,1} which has a size of 3,
	so we retrieve the order by subtracting the size by 1*/
	int size = V;
	int order = R.size() - 1;
	//make bezier whose size is the smoothness + 1 (where + 1 is the start point)
	sf::VertexArray bezier(sf::Points, V+1);
	//this is for storing the soon-to-be modified pascal coefficients
	vector<float> R2(order);
	/*for example, step=100, v=4, so
	  100/4 = 25 -> 25/100 -> .25
	  that way, t can increment by .25*/
	float tmp = step/V;
	tmp /= step;
	float t = 0;
	/*so let's say step=4, t will store 1/4, and tmp will also store 1/4.
	the code below will make it so
	t increases by tmp each iteration so we keep moving by step.
	1/4 + 1/4 = 2/4 -> 2/4 + 1/4 = 3/4*/
	
	for(int i = 0; i < size; i++)
	{
		/*the pattern for the derivative is:
		  original: order 4, derivative: order 3
		  the pascal sent to this function is pascalD which is order 3
		  pascalizing it multiplies the pascal by the t's and the (1-t)'s
		  in the next for loop
		  multiply by (P1-P0) and so on
		  then multiply each point by 3 indicated by the derivative order
		  */		
		R2 = pascalize(t, R, order);
		t += tmp; //increment t by 1/step
		bezier[i].position.x = 0;
		bezier[i].position.y = 0;
		

		for(int j = 0; j <= order; j++)
		{
			bezier[i].position.x += ((points[j+1].position.x-points[j].position.x)*R2[j]);
			bezier[i].position.y += ((points[j+1].position.y-points[j].position.y)*R2[j]);
		}
		bezier[i].position.x *= order;
		bezier[i].position.y *= order;
	}
	bezier = normalize(bezier);
	
	return bezier;
}

int main( void )
{	
	string coords;
	stringstream ss;

	int R; //order
	int V; //number of tangents/normals
	int points = 3;
	int beziers;
	int step = 100; //smoothness
	int pntDetected; //stores the index of what point/circle the mouse is holding
	int bezDetected; //stores the index of what bezier is being affected by the point being moved
	
	float factor = 30; //line length of tangents/normals
	float radius = 5;
	//for checking if mouse is within a circle
	float radius2 = radius*radius;
	float dx, dy;
	
	bool cirDetected = false; //check if a circle is pressed
	bool isMouseLeftButtonPressed = false;
	
	//User Input
	cout << "Tangents/Normals: ";
	cin >> V;
	cout << "Order: ";
	cin >> R;
	cout << "Steps: ";
	cin >> step;
	cout << "Points: ";
	cin >> points;
	//these two lines are here so getline works
	cin.ignore(256, '\n');
	cin.clear();
	
	//Point Input
	sf::Vertex pnt[points];
	for(int i = 0; i < points; i++)
	{
		getline(cin, coords); //get input like 10 24
		ss.str(coords); //store that here
		ss >> pnt[i].position.x >> pnt[i].position.y; //put 10 in x and 24 in y
		ss.clear();
	}
	
	//Pascal's Triangle
	/*float array instead of int array,
	because this will be multiplied by the t and t-1 which are floats*/
	vector<float> pascal(R+1, 1); //stores the coefficients of the order
	vector<float> pascalD(R, 1); //stores the coefficients of the order-1 for the derivative
	int coef = 1;
	//this loop goes until order-1
    for(int i = 1; i < R; i++)
    {
        for(int j = 1; j <= i; j++)
        {
			coef = coef * (i - j + 1) / j;
            pascal[j] = coef;
            pascalD[j] = coef;
        }
    }
	//this is the last loop for the actual order
	for(int j = 1; j <= R; j++)
	{
		coef = coef * (R - j + 1) / j;
		pascal[j] = coef;
	}
	
	
	//Beziers
	/*number of beziers.
	for example, I have 7 points,
	that means (7-1)/2 = 3, I have 3 beziers*/
	beziers = (points-1)/R;
	sf::VertexArray bezier[beziers];
	sf::VertexArray derivative[beziers];
	sf::VertexArray bpoints(sf::LineStrip, R+1);
	sf::VertexArray tanlines[beziers];
	sf::VertexArray norlines[beziers];
	//initialize each bezier
	for(int i = 0, j = 0; i < beziers; i++, j += R)
	{
		bezier[i].setPrimitiveType(sf::LineStrip); //connected line type
		bezier[i].resize(step); //array size is the smoothness
		derivative[i].setPrimitiveType(sf::Points); //point type
		derivative[i].resize(V); //array size is the smoothness
		tanlines[i].setPrimitiveType(sf::Lines); //separated line type (eg 0-1 is one line, 2-3 is another line)
		tanlines[i].resize(V*2); //double because we're connecting from bezier's points to derivative's points
		norlines[i].setPrimitiveType(sf::Lines);
		norlines[i].resize(V*2);
		/*R or the order is equal to points+1 of a bezier.
		for example, order R=2 means each bezier has 2+1=3 points.
		so, we loop for R+1=2+1=3 times to get the 3 points we need.
		We get the points from the bpoints array which stores all the points.*/
		for(int k = 0; k <= R; k++)
		{
			bpoints[k] = pnt[j+k];
		}
		//create bezier by sending the points, step/smoothness, and the pascal coefficients
		bezier[i] = pascalBezier(bpoints, step, pascal);
		derivative[i] = deriBezier(bpoints, step, pascalD, V);
		/*j increments by R, so the pattern can go like:
		if R=2
		points 0 1 2 for bezier 0, -> 0+R=0+2=2
		points 2 3 4 for bezier 1*/
	}
	
	for(int i = 0; i < beziers; i++)
	{
		for(int j = 0; j < V*2; j++)
		{
			tanlines[i][j].color = sf::Color::Green;
		}
		tanlines[i] = reTan(tanlines[i], bezier[i], derivative[i], factor);
		norlines[i] = reNom(tanlines[i]);
	}

	//Circles
	sf::CircleShape cir[points];
	
	for(int i = 0; i < points; i++)
	{
		cir[i].setRadius(radius);
		cir[i].setOrigin(radius,radius);
		cir[i].setPosition(pnt[i].position);
		cir[i].setFillColor(sf::Color::Transparent);
		cir[i].setOutlineColor(sf::Color::Blue);
		cir[i].setOutlineThickness(2);
	}
	
    sf::RenderWindow window( sf::VideoMode( winW, winH ), "SFML works!" );
	window.setFramerateLimit( FPS );

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
				case sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Left)
						isMouseLeftButtonPressed = true;
					break;
				case sf::Event::MouseButtonReleased:
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						isMouseLeftButtonPressed = false;
					}
					break;
			}
        }
		
		if(isMouseLeftButtonPressed)
		{
			if(cirDetected)
			{
				//update point position
				pnt[pntDetected].position = sf::Vector2f(sf::Mouse::getPosition(window));
				
				/* for example R = 2 which means we have 2+1=3 points per bezier,
				let's say the point detected is point 3,
				so 3 / 2 = 1 (there are no remainders because these are ints)
				which means this is bezier 1 which contains points 2 3 4.
				so, let's say pntDetected = 2
				2 / 2 = 1; still bezier 1
				however if get the last point of the bezier
				point 4
				4 / 2 = 2, that's bezier 2 which contains points 4 5 6
				but that point also affects bezier 1.
				we will accomodate for this later*/
				bezDetected = pntDetected/R;
				
				//if the point isn't the last of all points
				if(pntDetected != points-1)
				{
					//get the points for the bezier
					/*
					for example, R=3 and point=5, so this is bezier 1 which contains points 3 4 5 6.
					j = 5 - 5%3 = 5 - 2 = 3
					(% works like these
					3%3=0, 4%3=1, 5%3=2
					repeat: 6%3 = 0 (because it's divisible), 7%3=2, 8%3=2
					so it acts like the distance from 3)
					what about point=4
					j = 4 - 4%3 = 4 - 1 = 3
					j always becomes the first point of a bezier.
					bezier's 1st point is pnt[3].
					loop for R more times by incrementing by 1,
					we get the rest of the points*/
					for(int i = 0, j = pntDetected - pntDetected%R; i <= R; i++)
					{
						bpoints[i] = pnt[j+i];
					}
					//recalculate bezier
					bezier[bezDetected] = pascalBezier(bpoints, step, pascal);
					derivative[bezDetected] = deriBezier(bpoints, step, pascalD, V);
					tanlines[bezDetected] = reTan(tanlines[bezDetected], bezier[bezDetected], derivative[bezDetected], factor);
					norlines[bezDetected] = reNom(tanlines[bezDetected]);
				}
				
				//if the point is a point that affects two beziers and isn't the very first point
				/*pntDetected%R == 0 is the situation above when
				4 / 2 = 2 which gives us bezier 2 but it also affects bezier 1
				getting 4%2=0 tells us that it affects two beziers.
				Also, the point shouldn't be the very first point where 0%0=0 since it only affects one bezier*/
				if(pntDetected%R == 0 && pntDetected != 0)
				{
					/*let's say it was 4/2=2 -> bezier 2,
					that means we've already dealt with bezier 2 above,
					so this code is for bezier 1.
					j = pntDetected - pntDetected%R - R
					is similar to above with j = pntDetected - pntDetected%R
					but we minus it with R so go down to the previous bezier*/
					for(int i = 0, j = pntDetected - pntDetected%R - R; i <= R; i++)
					{
						bpoints[i] = pnt[j+i];
					}
					//recalculate the previous bezier
					bezier[bezDetected-1] = pascalBezier(bpoints, step, pascal);
					derivative[bezDetected-1] = deriBezier(bpoints, step, pascalD, V);
					tanlines[bezDetected-1] = reTan(tanlines[bezDetected-1], bezier[bezDetected-1], derivative[bezDetected-1], factor);
					norlines[bezDetected-1] = reNom(tanlines[bezDetected-1]);
				}

				cir[pntDetected].setPosition(pnt[pntDetected].position); //move circle to the updated point
			}
			else
			{
				for(int i = 0; i < points; i++)
				{
					//get distance from mouse to current circle
					dx = sf::Mouse::getPosition(window).x - cir[i].getPosition().x;
					dy = sf::Mouse::getPosition(window).y - cir[i].getPosition().y;
					/*
					dx and dy is the x and y distances between mouse and circle center.
					depending if the mouse if to the left or to the right of the circle,
					dx could be positive or negative.
					since we're comparing the distance to the circle radius (which is always positive),
					we want the absolute value of the distance,
					but calling the abs(value) function is costly,
					so we make them positive by squaring them,
					then compare the squared dx and dy with the squared radius*/
					//square them so they're all positive
					dx *= dx;
					dy *= dy;
					//check if within radius^2
					if (dx <= radius2 && dy <= radius2)
					{
						cirDetected = true; 
						pntDetected = i; //store the detected point's index
						break;
					}
				}
			}
		}
		
		if(cirDetected && !isMouseLeftButtonPressed)
		{
			cirDetected = false; //reset
		}

        window.clear();
		for(int i = 0; i < beziers; i++)
		{
			window.draw(bezier[i]);
			window.draw(tanlines[i]);
			window.draw(norlines[i]);
		}
		for(int i = 0; i < points; i++)
		{
			window.draw(cir[i]);
		}
        window.display();
    }

    return 0;
}
