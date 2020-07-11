//
//  main.cpp
//  practice
//
//  Created by Meg Tuason on 05/09/2019.
//  Copyright © 2019 Meg Tuason. All rights reserved.
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>

#define FPS 60
#define CSIZE 5

using namespace std;

int main(int argc, const char * argv[]) {
    
    sf::RenderWindow window(sf::VideoMode( 480, 320 ), "Practice" );
    sf::RectangleShape rect;
    sf::RectangleShape rect2;
    rect2.setPosition( 150, 150 );
    rect.setSize( sf::Vector2f( 100.0f, 60.0f ) );
    rect.setFillColor( sf::Color( 0, 0, 255 ) );
    sf::Clock clock;
    window.setFramerateLimit( FPS );
    int x = 100;
    int y = 60;
    string dir = "Right";
    
    while( window.isOpen() )
    {
        rect.setPosition( x, 50 );
        rect2.setSize( sf::Vector2f( x, y ) );
        rect2.setFillColor( sf::Color( 0, x, y ) );

        sf::Event event;
        while( window.pollEvent( event ) )
        {
            if( event.type == sf::Event::Closed )
            {
                window.close();
            }
        }

        window.clear();
        window.draw(rect);
        window.draw(rect2);
        window.display();
        
        if (x == 300){
            dir = "Left";
        } else if (x == 100){
            dir = "Right";
        }
        
        if (dir == "Right"){
            x += 1;
            y += 1;
        } else{
            x -= 1;
            y -= 1;
        }
        
    }
    return 0;
}
