#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iterator>

//define creates a constant value to be used by the whole code
//rather than get the variable_value, code gets raw VALUE
#define FPS 60
#define ELAS 1.f
#define EULER 2.5f
#define speed 15.f
//#define ballSpeed 3.f

int main()
{
    float ballSpeed = 3.f;

    float windowH = 500;
    float windowW = 900;

    float playerW = 10.f;
    float playerH = 100.f;

    float circY = 25.f;
    //bool to check if object can move

    sf::RenderWindow window(sf::VideoMode(windowW, windowH), "pong smoth");

    std::vector<sf::CircleShape> circleArray;
    //makes array of circles

    std::vector<sf::RectangleShape> playerArray;
    //makes array of circles

    //make array of colors
    sf::Color colors[6];
    //assigns colors
    colors[0] = sf::Color::Red;
    colors[1] = sf::Color::Green;
    colors[2] = sf::Color::Blue;
    colors[3] = sf::Color::Yellow;
    colors[4] = sf::Color::Cyan;
    colors[5] = sf::Color::White;

    sf::Clock clock;

    sf::Texture texture;
    if(!texture.loadFromFile("/Users/Meg/Desktop/gamedev/179.14B/luk-pong/luk-pong/smoth.jpg"))
    {
        std::cout <<"Load Failed" << std::endl;
        system("pause");
    }

    window.setFramerateLimit(FPS);

    //give circle radius
    float radius = 50.f;

    bool ballMove = false;

    int selectColor = 5;
    sf::Color ballColor = colors[selectColor];

    //creates variable of velocity as a VECTOR
    sf::Vector2f velocity(5,0);
    
    ballColor = colors[selectColor];

    //making circle
    sf::CircleShape circle(radius);
    //giving circle attributes
    circle.setPosition(windowW / 2, windowH / 2);
    //circle.setFillColor(ballColor);

    circle.setTexture(&texture);

    //push circle into array
    circleArray.push_back(circle);

    //MAKING PLAYER VALUES
    //makes array of rectangles
    sf::RectangleShape rect(sf::Vector2f(playerW,playerH));
    playerArray.push_back(rect);
    playerArray.push_back(rect);

    playerArray[0].setPosition( (windowW / 12)  , windowH / 2);
    playerArray[1].setPosition(windowW - (windowW / 12), windowH / 2);

    playerArray[0].setFillColor(sf::Color::Blue);
    playerArray[1].setFillColor(sf::Color::Red);

    //CIRCLE HITBOX
    //get diameter of circle. diameter = 2 * radius
    //ballHitBox is the diameter of the circle
    float ballHitBox = radius * 2;
    float playerHitBox = playerH;

    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();

        sf::Event event;

        while(window.pollEvent(event))
        {
            switch(event.type) //different possible switches depending on type of event
            {
                case sf::Event::Closed:
                window.close();
                break;

                //cases for when key is pressed
                case sf::Event::KeyPressed:
                    std::cout << "Key is being pressed" << std::endl;
                    switch (event.key.code)
                        {
                            case sf::Keyboard::Escape:
                                std::cout <<"escape" << std::endl;
                                window.close();
                                break;

                            case sf::Keyboard::W:
                                std::cout<<"W"<<std::endl;
                                if(playerArray[0].getPosition().y < (windowH * 0))
                                {
                                    playerArray[0].move(0,0);
                                }
                                else
                                {
                                    playerArray[0].move(0,-speed);
                                }
                                break;
                            case sf::Keyboard::S:
                                //move down
                                std::cout<<"S"<<std::endl;
                                if(playerArray[0].getPosition().y + playerH > windowH)
                                {
                                    playerArray[0].move(0,0);
                                }
                                else
                                {
                                    playerArray[0].move(0,speed);
                                }
                                break;
                            case sf::Keyboard::Up:
                                std::cout<<"Up"<<std::endl;
                                if(playerArray[1].getPosition().y < (windowH * 0))
                                {
                                    playerArray[1].move(0,0);
                                }
                                else
                                {
                                    playerArray[1].move(0,-speed);
                                }
                                break;
                            case sf::Keyboard::Down:
                                //move down
                                std::cout<<"Down"<<std::endl;
                                if(playerArray[1].getPosition().y + playerH > windowH)
                                {
                                    playerArray[1].move(0,0);
                                }
                                else
                                {
                                    playerArray[1].move(0,speed);
                                }
                                break;
                            case sf::Keyboard::Space:
                                //move ball
                                ballMove = true;
                                // std::cout<<"Space"<<std::endl;
                                std::cout<<"Ball is moving"<<std::endl;
                                break;
                        }
                    break;
                //end of key pressed case

                case sf::Event::KeyReleased:
                    std::cout <<"key has been released" << std::endl; //prints when ANY key is released
                    switch(event.key.code)
                    {
                        case sf::Keyboard::W:
                            //move left
                            std::cout<<"W has been released"<<std::endl;
                            break;
                        case sf::Keyboard::S:
                            //move left
                            std::cout<<"S has been released"<<std::endl;
                            break;
                    }
                    break;
                //end of key released case

            }
                
        }

        //get positions of entities
        sf::Vector2f ballPos = circleArray[0].getPosition(); //ball position
        sf::Vector2f P1Pos = playerArray[0].getPosition();    //player 1 position
        sf::Vector2f P2Pos = playerArray[1].getPosition();    //player 2 position

        //BALL MOVE
        if(ballMove)
        {
            //ELASTICITY
            if(ballPos.x < (windowW * 0) || ballPos.x + ballHitBox > (windowW * 1))
            {
                velocity.x = velocity.x * (-1 * ELAS);
            }
            if(ballPos.y < (windowH * 0) || ballPos.y + ballHitBox > (windowH * 1))
            {
                velocity.y = velocity.y * (-1 * ELAS);
            }

            //COLLISION FOR PLAYER 2
                //Playerhitbox added to P2 because get the very bottom of player area
                //ballhitbox added to ball because get the very right of ball for checking
            if(ballPos.y < P2Pos.y + playerHitBox && ballPos.y + ballHitBox > P2Pos.y && ballPos.x + ballHitBox > P2Pos.x)
            {
                velocity.x = velocity.x * (-1 * ELAS);
                //ballColor = colors[0];
                circleArray[0].setFillColor(ballColor);
            }
            //COLLISION FOR PLAYER 1
                //playerhitbox added to P1 because get very bottom of player area
                //player Width added to P1 to get very right side of player area
            if(ballPos.y < P1Pos.y + playerHitBox && ballPos.y + ballHitBox > P1Pos.y && ballPos.x < P1Pos.x + playerW)
            {
                velocity.x = velocity.x * (-1 * ELAS);
                //ballColor = colors[2];
                circleArray[0].setFillColor(ballColor);
            }

            circleArray[0].move(velocity.x, velocity.y);
        }
        
        


        


        window.clear(); //no idea what this does
                        //"Clear the entire target with a single color.
                        //This function is usually called once every frame, to clear the previous contents of the target."
        
        for(std::vector<sf::CircleShape>::iterator i = circleArray.begin(); i != circleArray.end(); i++)
                                                        //.begin() gets the beginning of the shape array and .end() gets the end of the shape array
        {
            window.draw(*i);
        }

        //drawing the rectangles
        //start of the array, not equal the to the end of the array, draw per iteration
        for(std::vector<sf::RectangleShape>::iterator i = playerArray.begin(); i != playerArray.end(); i++)
        {
            window.draw(*i);
        }

        window.display();
    }

    return 0;

}
