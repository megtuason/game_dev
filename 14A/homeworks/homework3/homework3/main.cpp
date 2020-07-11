//
//  main.cpp
//  homework3
//
//  Created by Meg Tuason on 09/09/2019.
//  Copyright © 2019 Meg Tuason. All rights reserved.
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>

#define FPS 200
#define CSIZE 6

using namespace std;


int main(int argc, const char * argv[]) {
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(500, 500), "Homework 3");
    window.setFramerateLimit(FPS);
    sf::Clock clock;
    sf::Color color[CSIZE];
    color[0] = sf::Color::Red;
    color[1] = sf::Color::Green;
    color[2] = sf::Color::Blue;
    color[3] = sf::Color::Yellow;
    color[4] = sf::Color::Cyan;
    color[5] = sf::Color::White;
    sf::CircleShape circles[60];
    sf::RectangleShape rects[40];
    sf::CircleShape circle(30.0f);
    sf::RectangleShape rect(sf::Vector2f( 50.0f, 50.0f ));
    
    int colorcount = 0;
    for(int i = 0; i < 60; i++){
        if (colorcount == 6){
            colorcount = 0;
        }
        if (i < 40){
            rect.setFillColor(color[colorcount]);
            rect.setPosition(rand() % 499, rand() % 499);
            rects[i] = rect;
        }
        circle.setFillColor(color[colorcount]);
        circle.setPosition(rand() % 499, rand() % 499);
        circles[i] = circle;
        colorcount++;
    }
    
    char move = 'i';
    while(window.isOpen()){
        
        for(int i = 1; i < 60; i++){
            if (i < 40){
                float x = rects[i].getPosition().x;
                float y = rects[i].getPosition().y;
                rects[i].setPosition(x+1, y);
            }
            float x = circles[i].getPosition().x;
            float y = circles[i].getPosition().y;
            circles[i].setPosition(x, y+1);
        }
        
        float x = circles[0].getPosition().x;
        float y = circles[0].getPosition().y;
        if (move == 'w'){
            circles[0].setPosition(x, y-1);
        } else if (move == 'a'){
            circles[0].setPosition(x-1, y);
        } else if (move == 's'){
            circles[0].setPosition(x, y+1);
        } else if (move == 'd'){
            circles[0].setPosition(x+1, y);
        }
        sf::Event event;
        sf::Vector2i mPos = sf::Mouse::getPosition( window );
        while(window.pollEvent(event)){
            switch (event.type){
//                case sf::Event::MouseButtonPressed:
//                    switch( event.mouseButton.button ) {
//                        case sf::Mouse::Left:
//                            rects[0].move(0.200*elapsed.asMilliseconds(),)
//                            break;
//                    }
//                    break;
//
                case sf::Event::Closed:
                    window.close();
                    break;
                    
                case sf::Event::KeyPressed:
                    switch( event.key.code ) {
                        case sf::Keyboard::W:
                            move = 'w';
                            break;
                        case sf::Keyboard::A:
                            move = 'a';
                            break;
                        case sf::Keyboard::S:
                            move = 's';
                            break;
                        case sf::Keyboard::D:
                            move = 'd';
                            break;
                        case sf::Keyboard::Escape:
                            window.close();
                    }
                    break;
                case sf::Event::KeyReleased:
                    switch( event.key.code ) {
                        case sf::Keyboard::W:
                            move = 'i';
                            break;
                        case sf::Keyboard::A:
                            move = 'i';
                            break;
                        case sf::Keyboard::S:
                            move = 'i';
                            break;
                        case sf::Keyboard::D:
                            move = 'i';
                            break;
                    }
                    break;
            }
        }
        window.clear();
        for(int i = 0; i < 60; i++){
            if(i < 40){
                window.draw(rects[i]);
            }
            window.draw(circles[i]);
        }
        window.display();
        
    }
    
    return 0;
}
