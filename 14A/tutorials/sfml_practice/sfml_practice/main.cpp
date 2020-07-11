//
//  main.cpp
//  sfml_practice
//
//  Created by Meg Tuason on 12/09/2019.
//  Copyright © 2019 Meg Tuason. All rights reserved.
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>

#define FPS 1000
#define CSIZE 6

using namespace std;

#include <iostream>

int main(int argc, const char * argv[]) {
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Practice");
    window.setFramerateLimit(FPS);
    sf::Color color[CSIZE];
    color[0] = sf::Color::Red;
    color[1] = sf::Color::Green;
    color[2] = sf::Color::Blue;
    color[3] = sf::Color::White;
    sf::RectangleShape rects[3];
    sf::RectangleShape rect(sf::Vector2f( 100.0f, 100.0f ));
    int x = 150;
    for (int i = 0; i < 3; i++){
        rect.setPosition(x, 300);
        rects[i] = rect;
        x += 300;
    }
    int flag = 0;
    char move = 'i';
    
    while(window.isOpen())
    {
        if (flag == 1){
            rects[0].setFillColor(color[3]);
            rects[1].setFillColor(color[1]);
            rects[2].setFillColor(color[2]);
            float x = rects[0].getPosition().x;
            float y = rects[0].getPosition().y;
            if (move == 'w'){
                rects[0].setPosition(x, y-1);
            } else if (move == 'a'){
                rects[0].setPosition(x-1, y);
            } else if (move == 's'){
                rects[0].setPosition(x, y+1);
            } else if (move == 'd'){
                rects[0].setPosition(x+1, y);
            }
        } else if(flag ==2){
            rects[0].setFillColor(color[0]);
            rects[1].setFillColor(color[3]);
            rects[2].setFillColor(color[2]);
            float x = rects[1].getPosition().x;
            float y = rects[1].getPosition().y;
            if (move == 'w'){
                rects[1].setPosition(x, y-1);
            } else if (move == 'a'){
                rects[1].setPosition(x-1, y);
            } else if (move == 's'){
                rects[1].setPosition(x, y+1);
            } else if (move == 'd'){
                rects[1].setPosition(x+1, y);
            }
        } else if (flag == 3){
            rects[0].setFillColor(color[0]);
            rects[1].setFillColor(color[1]);
            rects[2].setFillColor(color[3]);
            float x = rects[2].getPosition().x;
            float y = rects[2].getPosition().y;
            if (move == 'w'){
                rects[2].setPosition(x, y-1);
            } else if (move == 'a'){
                rects[2].setPosition(x-1, y);
            } else if (move == 's'){
                rects[2].setPosition(x, y+1);
            } else if (move == 'd'){
                rects[2].setPosition(x+1, y);
            }
        } else{
            rects[0].setFillColor(color[0]);
            rects[1].setFillColor(color[1]);
            rects[2].setFillColor(color[2]);

        }
        sf::Event event;
        sf::Vector2i mPos = sf::Mouse::getPosition( window );
        while( window.pollEvent( event ) )
        {
            switch( event.type )
            {
                case sf::Event::MouseButtonPressed:
                    switch( event.mouseButton.button ) {
                        case sf::Mouse::Left:
                            if (rects[0].getGlobalBounds().contains(window.mapPixelToCoords(mPos))){
                                flag = 1;
                                
                            } else if (rects[1].getGlobalBounds().contains(window.mapPixelToCoords(mPos))){
                                flag = 2;
                            } else if (rects[2].getGlobalBounds().contains(window.mapPixelToCoords(mPos))){
                                flag = 3;
                            } else {
                                flag = 0;
                            }
                            break;
                    }
                    break;
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
        for (int i = 0; i<3; i++){
            window.draw(rects[i]);
        }
        window.display();
    }
    

    
    
    return 0;
}
