
//
//  main.cpp
//  problemC
//
//  Created by Meg Tuason on 18/09/2019.
//  Copyright © 2019 Meg Tuason. All rights reserved.
//
#include <SFML/Graphics.hpp>
#include <iostream>

#define FPS 60
#define CSIZE 4

using namespace std;

int main(int argc, const char * argv[]) {
    sf::RenderWindow window(sf::VideoMode(3000, 1000), "P6");
    window.setFramerateLimit(FPS);
    sf::Color color[CSIZE];
    color[0] = sf::Color::Red;
    
    sf::Clock clock;
    
    sf::RectangleShape rect(sf::Vector2f(150.0f,150.0f));
    rect.setPosition(0, 100);
    rect.setFillColor(color[0]);
    
    bool move = false;
    int step = 5;
    
    while(window.isOpen()){
        sf::Event event;
        while( window.pollEvent( event ) )
        {
            switch( event.type ){
                case sf::Event::KeyPressed:
                    switch( event.key.code ) {
                        case sf::Keyboard::D:
                            if(rect.getPosition().x >= 2400){
                                rect.setPosition(0, 100);
                            }else{
                                move = true;
                            }
                            break;
                        case sf::Keyboard::Escape:
                            window.close();
                    }
                    break;
                case sf::Event::KeyReleased:
                    switch( event.key.code ) {
                        case sf::Keyboard::D:
                            move = false;
                            break;
                    }
            }
        }
        if(move){
            rect.move(step, 0);
            step+=(1);
        }
        
        window.clear();
        window.draw(rect);
        window.display();
    }
    return 0;
}
