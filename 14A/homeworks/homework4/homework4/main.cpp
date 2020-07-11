//
//  main.cpp
//  homework4
//
//  Created by Meg Tuason on 24/09/2019.
//  Copyright © 2019 Meg Tuason. All rights reserved.
//
#include <SFML/Graphics.hpp>
#include <iostream>

#define FPS 1000
#define CSIZE 2
#define W_HEIGHT 1400
#define W_WIDTH 1000

using namespace std;

int main(int argc, const char * argv[]) {
    sf::RenderWindow window(sf::VideoMode(W_WIDTH, W_HEIGHT), "Homework 4");
    window.setFramerateLimit(FPS);
    sf::Color color[CSIZE];
    color[0] = sf::Color::Red;
    color[1] = sf::Color::Cyan;
    bool frick = false;

    sf::CircleShape circle(50.0f);
    circle.setPosition(W_WIDTH/2, W_HEIGHT/2);
    float radius = circle.getRadius();
    circle.setOrigin(radius, radius);
    circle.setFillColor(color[0]);
    
    sf::Clock clock;
    float timestepX = 0;
    float timestepY = 0;
    float accel = 1;
	float ela = 1.0f;
    float time;
    float mass = 0.8f;
    float diag = 0.7071;
    
    float xMin = radius;
    float yMin = radius;
    float xMax =  W_WIDTH - radius;
    float yMax =  W_HEIGHT - radius;

    bool KeyUp = false;
	bool KeyDown = false;
	bool KeyLeft= false;
	bool KeyRight = false;
    
    while(window.isOpen())
    {
        time = clock.restart().asSeconds();
        
        if(frick){
            circle.setFillColor(color[1]);
        } else{
            circle.setFillColor(color[0]);
        }
        
        sf::Event event;
        while( window.pollEvent( event ) ){
            switch( event.type ){
                case sf::Event::Closed:
                    window.close();
				    break;
                case sf::Event::KeyPressed:
                    switch( event.key.code ) {
                        case sf::Keyboard::W:
                            KeyUp = true;
                            break;
                        case sf::Keyboard::A:
                            KeyLeft = true;
                            break;
                        case sf::Keyboard::S:
                            KeyDown = true;
                            break;
                        case sf::Keyboard::D:
                            KeyRight = true;
                            break;
                        case sf::Keyboard::F:
                            if(frick){
                                frick=false;
                            } else{
                                frick = true;
                            }
                            break;
                        case sf::Keyboard::Escape:
                            window.close();
                    }
                    break;
                case sf::Event::KeyReleased:
                    switch( event.key.code ) {
                        case sf::Keyboard::W:
                            KeyUp = false;
                            break;
                        case sf::Keyboard::A:
                            KeyLeft = false;
                            break;
                        case sf::Keyboard::S:
                            KeyDown = false;
                            break;
                        case sf::Keyboard::D:
                            KeyRight = false;
                            break;
                    }
            }
        }

        sf::Vector2f pos = circle.getPosition();
        if(pos.x < xMin){ //left
            pos.x = xMin;
            timestepX  *= -ela;
            if(frick){
                timestepX -= accel;
            }
        }
        if(pos.x > xMax){ //right
            pos.x = xMax;
            timestepX  *= -ela;
            if(frick){
                timestepX += accel;
            }
        }
        if(pos.y < yMin){ //top
            pos.y = yMin;
            timestepY *= -ela;
            if(frick){
                timestepY -= accel;
            }
        }
        if(pos.y > yMax){ //bottom
            pos.y = yMax;
            timestepY  *= -ela;
            if(frick){
//                timestepY += accel;
            }
        }
        circle.setPosition(pos);

        if(KeyUp){ //key up
            if(KeyLeft){ //up left
                circle.move(timestepX*time*diag/mass, timestepY*time*diag/mass);
                timestepY -= accel;
                timestepX -= accel;
            }else if(KeyRight){ //up right
                circle.move(timestepX*time*diag/mass, timestepY*time*diag/mass);
                timestepY -= accel;
                timestepX += accel;
            }else{ 
                if (timestepX != 0 && frick){
                    if((-0.2) <= timestepX && timestepX <= 0.2){
                        timestepX = 0;
                    }else if (timestepX > 0.2){
                        timestepX -= accel;
                    } else if (timestepX < (-0.2)){
                        timestepX += accel;
                    }
                }
                circle.move(timestepX*time*diag/mass, timestepY*time*diag/mass);
                timestepY -= accel;
            }
        }else if(KeyDown){ //key down
            if(KeyLeft){
                circle.move(timestepX*time*diag/mass, timestepY*time*diag/mass);
                timestepY += accel;
                timestepX -= accel;
            } else if(KeyRight){
                circle.move(timestepX*time*diag/mass, timestepY*time*diag/mass);
                timestepY += accel;
                timestepX += accel;
            } else {
                if (timestepX != 0 && frick ){
                    if((-0.2) <= timestepX && timestepX <= 0.2){
                        timestepX = 0;
                    }else if (timestepX > 0.2){
                        timestepX -= accel;
                    } else if (timestepX < (-0.2)){
                        timestepX += accel;
                    }
                }
                circle.move(timestepX*time*diag/mass, timestepY*time*diag/mass);
                timestepY += accel;
            }
        }else if(KeyLeft){ //key left
            if(KeyUp){
                circle.move(timestepX*time*diag/mass, timestepY*time*diag/mass);
                timestepY -= accel;
                timestepX -= accel;
            }
            else if(KeyDown){
                circle.move(timestepX*time*diag/mass, timestepY*time*diag/mass);
                timestepY += accel;
                timestepX -= accel;
            }
            else {
                if (timestepY != 0 && frick){
                    if((-0.2) <= timestepY && timestepY <= 0.2){
                        timestepY = 0;
                    }else if (timestepY > 0.2){
                        timestepY -= accel;
                    } else if (timestepY < (-0.2)){
                        timestepY += accel;
                    }
                }
                circle.move(timestepX*time*diag/mass, timestepY*time*diag/mass);
                timestepX -= accel;
            }
        }else if(KeyRight){ //key right
            if(KeyUp){
                circle.move(timestepX*time*diag/mass, timestepY*time*diag/mass);
                timestepY -= accel;
                timestepX += accel;
            }
            else if(KeyDown){
                circle.move(timestepX*time*diag/mass, timestepY*time*diag/mass);
                timestepY += accel;
                timestepX += accel;
            }
            else {
                if (timestepY != 0 && frick){
                    if((-0.2) <= timestepY && timestepY <= 0.2){
                        timestepY = 0;
                    }else if (timestepY > 0.2){
                        timestepY -= accel;
                    } else if (timestepY < (-0.2)){
                        timestepY += accel;
                    }
                }
                circle.move(timestepX*time*diag/mass, timestepY*time*diag/mass);
                timestepX += accel;
            }
        } else { //all released
            if(timestepX != 0 && frick){
                if((-0.2) <= timestepX && timestepX <= 0.2){
                    timestepX = 0;
                }else if (timestepX > 0.2){
                    timestepX -= accel;
                } else if (timestepX < (-0.2)){
                    timestepX += accel;
                }
            }
            if (timestepY != 0 && frick){
                if((-0.2) <= timestepY && timestepY <= 0.2){
                    timestepY = 0;
                }else if (timestepY > 0.2){
                    timestepY -= accel;
                } else if (timestepY < (-0.2)){
                    timestepY += accel;
                }
            }
            circle.move(timestepX*time*diag/mass, timestepY*time*diag/mass);
        }
        cout << "X: " << timestepX << endl;
        cout << "Y: " << timestepY << endl;
        
        window.clear();
        window.draw(circle);
        window.display();
    }
    return 0;
}
