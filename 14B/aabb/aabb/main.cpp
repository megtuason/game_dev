//
//  main.cpp
//  aabb
//
//  Created by Meg Tuason on 02/03/2020.
//  Copyright © 2020 Meg Tuason. All rights reserved.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

#define FPS 60
#define winW 800
#define winH 600

using namespace std;

bool colCheck(sf::RectangleShape a, sf::RectangleShape b)
{
    float aMinX = a.getPosition().x - a.getSize().x/2;
    float aMinY = a.getPosition().y - a.getSize().y/2;
    float bMinX = b.getPosition().x - b.getSize().x/2;
    float bMinY = b.getPosition().y - b.getSize().y/2;
    float aMaxX = aMinX + a.getSize().x;
    float aMaxY = aMinY + a.getSize().y;
    float bMaxX = bMinX + b.getSize().x;
    float bMaxY = bMinY + b.getSize().y;
    
    if(aMaxX < bMinX || aMaxY < bMinY || aMinX > bMaxX || aMinY > bMaxY)
        return false;
    else
        return true;
}


int main(int argc, const char * argv[]) {
    sf::Clock clock;
    float time;
    
    int rectCount = 5;
    sf::RectangleShape rect[rectCount];
    bool isColliding[rectCount];
    sf::RectangleShape bound[rectCount];
    
    sf::Color clrNoCol = sf::Color::Red;
    sf::Color clrCol = sf::Color::Green;
    
    rect[0].setPosition(winW/2,winH/2);
    rect[0].setSize(sf::Vector2f(50.0f,50.0f));
    
    rect[1].setPosition(winW/2 - 100, winH/2 - 100);
    rect[1].setSize(sf::Vector2f(50.0f,50.0f));
    
    rect[2].setPosition(winW/2 + 100, winH/2 + 100);
    rect[2].setSize(sf::Vector2f(50.0f,100.0f));
    
    rect[3].setPosition(winW/2 - 100, winH/2 + 100);
    rect[3].setSize(sf::Vector2f(75.0f,45.0f));
    
    rect[4].setPosition(winW/2 + 100, winH/2 - 100);
    rect[4].setSize(sf::Vector2f(10.0f,45.0f));
    
    for (int i = 0; i < rectCount; i++)
    {
        rect[i].setOrigin(rect[i].getSize().x/2, rect[i].getSize().y/2);
        rect[i].setFillColor(clrNoCol);
        bound[i].setFillColor(sf::Color::Transparent);
        bound[i].setOutlineThickness(1);
        bound[i].setOutlineColor(sf::Color::White);
        isColliding[i] = false;
    }
    
    float speed = 100.0f; //pixels per second
    float rot[5] = {90.0f, 180.0f, 60.0f, 30.0f, 270.0f};
    bool iskeyUpPressed = false;
    bool iskeyLeftPressed = false;
    bool iskeyDownPressed = false;
    bool iskeyRightPressed = false;
    bool isKeyCirUpPressed = false;
    bool isKeyCirLeftPressed = false;
    bool isKeyCirDownPressed = false;
    bool isKeyCirRightPressed = false;
    bool isShapeOn[5] = {true,false,false,false,false};
    
    int counter = 0;
    
    sf::Keyboard::Key keyNext = sf::Keyboard::Space;
    sf::Keyboard::Key keyUp = sf::Keyboard::W;
    sf::Keyboard::Key keyLeft = sf::Keyboard::A;
    sf::Keyboard::Key keyDown = sf::Keyboard::S;
    sf::Keyboard::Key keyRight = sf::Keyboard::D;
    sf::Keyboard::Key keyHelp = sf::Keyboard::H;
    
    sf::RenderWindow window( sf::VideoMode( winW, winH ), "SFML works!" );
    window.setFramerateLimit( FPS );
    
    cout << "Control Next Shape: Space" << endl;
    cout << "Up: W" << endl;
    cout << "Right: A" << endl;
    cout << "Down: S" << endl;
    cout << "Left: D" << endl;
    
    while(window.isOpen())
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
                if (event.key.code == keyNext)
                {
                    isShapeOn[counter] = false;
                    counter++;
                    if(counter > rectCount - 1)
                        counter = 0;
                    isShapeOn[counter] = true;
                }
                else if (event.key.code == keyUp)
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
                break;
            }
        }
        time = clock.restart().asSeconds();
        
        for (int i = 0; i < rectCount; i++)
        {
            //Rotate
            rect[i].rotate(rot[i]*time);
            
            //WASD
            if(isShapeOn[i])
            {
                if(iskeyUpPressed && rect[i].getPosition().y > 0.0f)
                {
                    if(iskeyLeftPressed && rect[i].getPosition().x > 0.0f)
                        rect[i].move(-speed*time*0.7071, -speed*time*0.7071);
                    else if(iskeyRightPressed && (rect[i].getPosition().x + rect[i].getSize().x) < winW)
                        rect[i].move(speed*time*0.7071, -speed*time*0.7071);
                    else
                        rect[i].move(0.0f, -speed*time);
                }
                else if(iskeyDownPressed && (rect[i].getPosition().y + rect[i].getSize().y) < winH)
                {
                    if(iskeyLeftPressed && rect[i].getPosition().x > 0.0f)
                        rect[i].move(-speed*time*0.7071, speed*time*0.7071);
                    else if(iskeyRightPressed && (rect[i].getPosition().x + rect[i].getSize().x) < winW)
                        rect[i].move(speed*time*0.7071, speed*time*0.7071);
                    else
                        rect[i].move(0.0f, speed*time);
                }
                else if(iskeyLeftPressed && rect[i].getPosition().x > 0.0f)
                {
                    if(iskeyUpPressed && rect[i].getPosition().y > 0.0f)
                        rect[i].move(-speed*time*0.7071, -speed*time*0.7071);
                    else if(iskeyDownPressed && (rect[i].getPosition().y + rect[i].getSize().y) < winH)
                        rect[i].move(-speed*time*0.7071, speed*time*0.7071);
                    else
                        rect[i].move(-speed*time, 0.0f);
                }
                else if(iskeyRightPressed && (rect[i].getPosition().x + rect[i].getSize().x) < winW)
                {
                    if(iskeyUpPressed && rect[i].getPosition().y > 0.0f)
                        rect[i].move(speed*time*0.7071, -speed*time*0.7071);
                    else if(iskeyDownPressed && (rect[i].getPosition().y + rect[i].getSize().y) < winH)
                        rect[i].move(speed*time*0.7071, speed*time*0.7071);
                    else
                        rect[i].move(speed*time, 0.0f);
                }
            }
            
            //Bounding Box
            bound[i].setSize(sf::Vector2f(rect[i].getGlobalBounds().width, rect[i].getGlobalBounds().height));
            bound[i].setPosition(rect[i].getPosition().x, rect[i].getPosition().y);
            bound[i].setOrigin(bound[i].getSize().x/2, bound[i].getSize().y/2);
            
            //Collision
            for (int j = i + 1; j < rectCount; j++)
            {
                if(colCheck(bound[i],bound[j]))
                {
                    rect[i].setFillColor(clrCol);
                    rect[j].setFillColor(clrCol);
                    isColliding[i] = true;
                    isColliding[j] = true;
                }
            }
            if(!isColliding[i])
                rect[i].setFillColor(clrNoCol);
        }
        window.clear();
        for (int i = 0; i < rectCount; i++)
        {
            window.draw(rect[i]);
            window.draw(bound[i]);
            isColliding[i] = false;
        }
        window.display();
        
    }
    
    return 0;
}
