#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <vector>

#define FPS 60
#define winW 600
#define winH 500

using namespace std;

vector<float> pascalize(float t, vector<float> R)
{
    //store 1-t so we can keep reusing it without recalculating it again and again
    float a = 1-t;
    int order = R.size() - 1;

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
        R2 = pascalize(t, R); //pascal coefficients gets multiplied by t and 1-t
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

int main( void )
{
    string coords;
    stringstream ss;

    int R; //order
    int points = 3;
    int beziers;
    int step = 100; //smoothness
    int pntDetected; //stores the index of what point/circle the mouse is holding
    int bezDetected; //stores the index of what bezier is being affected by the point being moved
    
    float radius = 5;
    //for checking if mouse is within a circle
    float radius2 = radius*radius;
    float dx, dy;
    
    bool cirDetected = false; //check if a circle is pressed
    bool isMouseLeftButtonPressed = false;
    
    //User Input
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
    vector<float> pascal(R+1); //stores the coefficients of the order
    int coef = 1;
    for(int i = 0; i <= R; i++)
    {
        for(int j = 0; j <= i; j++)
        {
            if (j == 0 || i == 0)
                coef = 1;
            else
                coef = coef * (i - j + 1) / j;
            pascal[j] = coef;
        }
    }
    
    //Beziers
    /*number of beziers.
    for example, I have 7 points,
    that means (7-1)/2 = 3, I have 3 beziers*/
    beziers = (points-1)/R;
    sf::VertexArray bezier[beziers];
    sf::VertexArray bpoints(sf::LineStrip, R+1);
    //initialize each bezier
    for(int i = 0, j = 0; i < beziers; i++, j += R)
    {
        bezier[i].setPrimitiveType(sf::LineStrip); //line type
        bezier[i].resize(step); //array size is the smoothness
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
        /*j increments by R, so the pattern can go like:
        if R=2
        points 0 1 2 for bezier 0, -> 0+R=0+2=2
        points 2 3 4 for bezier 1*/
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
        }
        for(int i = 0; i < points; i++)
        {
            window.draw(cir[i]);
        }
        window.display();
    }

    return 0;
}
