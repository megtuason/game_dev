//
//  main.cpp
//  AABB-ex
//
//  Created by Meg Tuason on 05/11/2019.
//  Copyright © 2019 Meg Tuason. All rights reserved.
//
#include <SFML/Graphics.hpp>
#include <iostream>

#define W_HEIGHT 800
#define W_WIDTH 800
#define FPS 200
#define CSIZE 4

using namespace std;


int main(int argc, const char * argv[]) {
    sf::RenderWindow window(sf::VideoMode(W_HEIGHT, W_WIDTH), "AABB Exercise");
    window.setFramerateLimit(FPS);
    sf::Color color[CSIZE];
    color[0] = sf::Color::Red;
    color[1] = sf::Color::Green;
    color[2] = sf::Color::Blue;
    color[3] = sf::Color::Transparent;

    sf::Clock clock;
    float time;
    float speed = 200.0;
    float radius = 100.0f;
    float diag = 0.7071;
    float x,y = radius;
    float x2,y2 = radius;

    bool KeyUp = false;
    bool KeyDown = false;
    bool KeyLeft = false;
    bool KeyRight = false;
    bool controlRight = false;

    sf::CircleShape circ(radius);
    sf::RectangleShape rect1(sf::Vector2f(radius, radius));
    sf::RectangleShape rect2(sf::Vector2f(radius, radius));
    circ.setFillColor(color[0]);
    rect1.setFillColor(color[0]);
    rect2.setFillColor(color[0]);
    circ.setOrigin(radius,radius);
    rect1.setOrigin(rect1.getSize().x*0.5f, rect1.getSize().y*0.5f);
    rect2.setOrigin(rect2.getSize().x*0.5f, rect2.getSize().y*0.5f);
    circ.setPosition(400,500);
    rect1.setPosition(200,200);
    rect2.setPosition(500,200);

    sf::FloatRect rect;
    sf::FloatRect circle;
    sf::RectangleShape brect1(sf::Vector2f(rect1.getGlobalBounds().width, rect1.getGlobalBounds().height));
    brect1.setFillColor(color[3]);
    brect1.setOutlineColor(color[1]);
    brect1.setOutlineThickness(3);

    sf::RectangleShape brect2(sf::Vector2f(rect2.getGlobalBounds().width, rect2.getGlobalBounds().height));
    brect2.setFillColor(color[3]);
    brect2.setOutlineColor(color[1]);
    brect2.setOutlineThickness(3);

    sf::RectangleShape brect3(sf::Vector2f(circ.getGlobalBounds().width, circ.getGlobalBounds().height));
    brect3.setFillColor(color[3]);
    brect3.setOutlineColor(color[1]);
    brect3.setOutlineThickness(3);

    while(window.isOpen()){
        
        time = clock.restart().asSeconds();

        rect = rect1.getGlobalBounds();
        sf::Vector2f topLeft(rect.left, rect.top);
//        sf::Vector2f bottomRight(rect.width, rect.height);
        brect1.setPosition(topLeft);
        brect1.setSize(sf::Vector2f( rect.width,rect.height ));
        
        
//        if (x > bottomRight.x){
//            x -= 1;
//        }
//        else if(y > bottomRight.y) {
//            y -= 1;
//        } else {
//            x += 1;
//            y += 1;
//        }
        

        rect = rect2.getGlobalBounds();
        sf::Vector2f topLeft2(rect.left, rect.top);
//        sf::Vector2f bottomRight2(rect.width, rect.height);
        brect2.setPosition(topLeft2);
        brect2.setSize(sf::Vector2f( rect.width, rect.height ));
        
//        if (x2 > bottomRight2.x){
//            x2 -= 1;
//        }
//        else if (y2 > bottomRight2.y) {
//            y2 -= 1;
//        } else {
//            x2 += 1;
//            y2 += 1;
//        }

        circle = circ.getGlobalBounds();
        sf::Vector2f topLeft3(circle.left, circle.top);
        brect3.setPosition(topLeft3);

        if (rect1.getGlobalBounds().intersects(rect)){
            rect1.setFillColor(color[2]);
            rect2.setFillColor(color[2]);
        } else if(rect1.getGlobalBounds().intersects(circle)){
            rect1.setFillColor(color[2]);
            circ.setFillColor(color[2]);
        } else if(rect2.getGlobalBounds().intersects(circle)){
            rect2.setFillColor(color[2]);
            circ.setFillColor(color[2]);
        }else{
            rect1.setFillColor(color[0]);
            rect2.setFillColor(color[0]);
            circ.setFillColor(color[0]);
        }

        
        sf::Event event;
        while(window.pollEvent(event)){
            switch(event.type){
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch(event.key.code){
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
                        case sf::Keyboard::R:
                            if (controlRight){
                                controlRight = false;
                            } else{
                                controlRight = true;
                            }
                            break;
                        case sf::Keyboard::Escape:
                            window.close();
                            break;
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

        if (KeyUp){
            if(KeyLeft){
                if (controlRight){
                    rect2.move(-speed*time*diag,-speed*time*diag);
                } else{
                    rect1.move(-speed*time*diag,-speed*time*diag);
                }
            } else if (KeyRight){
                if (controlRight){
                    rect2.move(speed*time*diag, -speed*time*diag);
                } else{
                    rect1.move(speed*time*diag, -speed*time*diag);
                }
            } else{
                if (controlRight){
                    rect2.move(0.0f, -speed*time);
                } else{
                    rect1.move(0.0f, -speed*time);
                }
            }
        } else if(KeyDown){
            if(KeyLeft){
                if(controlRight){
                    rect2.move(-speed*time*diag,speed*time*diag);
                } else{
                    rect1.move(-speed*time*diag,speed*time*diag);
                }
            } else if (KeyRight){
                if(controlRight){
                    rect2.move(speed*time*diag, speed*time*diag);
                } else{
                    rect1.move(speed*time*diag, speed*time*diag);
                }
            } else{
                if(controlRight){
                    rect2.move(0.0f, speed*time);
                } else{
                    rect1.move(0.0f, speed*time);
                }
            }
        } else if (KeyLeft){
           if(KeyUp){
               if(controlRight){
                   rect2.move(-speed*time*diag,-speed*time*diag);
                } else{
                    rect1.move(-speed*time*diag,-speed*time*diag);
                }
            } else if (KeyDown){
                if(controlRight){
                    rect2.move(-speed*time*diag, speed*time*diag);
                } else{
                    rect1.move(-speed*time*diag, speed*time*diag);
                }
            } else{
                if(controlRight){
                    rect2.move(-speed*time, 0.0f);
                } else{
                    rect1.move(-speed*time, 0.0f);
                }
            }
        } else if (KeyRight){
            if(KeyUp){
                if(controlRight){
                    rect2.move(speed*time*diag,-speed*time*diag);
                } else{
                    rect1.move(speed*time*diag,-speed*time*diag);
                }
            } else if (KeyDown){
                if(controlRight){
                    rect2.move(speed*time*diag, speed*time*diag);
                } else{
                    rect1.move(speed*time*diag, speed*time*diag);
                }
            } else{
                if(controlRight){
                    rect2.move(speed*time, 0.0f);
                } else{
                    rect1.move(speed*time, 0.0f);
                }
            }
        }

        rect1.rotate(0.5);
        rect2.rotate(0.2);

        window.clear();
        window.draw(circ);
        window.draw(rect1);
        window.draw(rect2);
        window.draw(brect1);
        window.draw(brect2);
        window.draw(brect3);
        window.display();
    }
    return 0;
}
