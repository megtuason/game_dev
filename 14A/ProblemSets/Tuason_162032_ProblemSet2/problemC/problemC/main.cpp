
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
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Problem C");
    window.setFramerateLimit(FPS);
    sf::Color color[CSIZE];
    color[0] = sf::Color::White;
    color[1] = sf::Color::Red;
    color[2] = sf::Color::Blue;
    color[3] = sf::Color::Transparent;
    
    bool draw = false;
    int counter = 9;
    
    sf::CircleShape circle(70.0f);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setOutlineThickness(5);
    circle.setFillColor(color[3]);
    circle.setOutlineColor(color[1]);
    
    sf::RectangleShape cross[2];
    sf::RectangleShape crossx(sf::Vector2f(150.0f,5.0f));
    crossx.setFillColor(color[2]);
    crossx.setOrigin(crossx.getSize().x*0.5f, crossx.getSize().y*0.5f);
    crossx.rotate(45);
    cross[0] = crossx;
    crossx.rotate(90);
    cross[1] = crossx;
    
    sf::RectangleShape lines[8];
    sf::RectangleShape line(sf::Vector2f(750.0f, 5.0f));
    line.setFillColor(color[0]);
    int x = 100;
    int y = 100;
    for (int i = 0; i < 8; i++){
        if(i > 3){
            line.setPosition(x, 100);
            lines[i] = line;
            x += 250;
        }else{
            line.setPosition(100, y);
            lines[i] = line;
            y += 250;
        }
        if(i == 3){
            line.rotate(90);
        }
    }
    
    while(window.isOpen()){
        sf::Event event;
        sf::Vector2i mPos = sf::Mouse::getPosition( window );
        while( window.pollEvent( event ) )
        {
            switch( event.type ){
                case sf::Event::MouseButtonPressed:
                    switch( event.mouseButton.button ) {
                        case sf::Mouse::Left:
                            counter -= 1;
                            draw = true;
                            break;
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    switch( event.mouseButton.button ) {
                        case sf::Mouse::Left:
                            draw = false;
                            break;
                    }
                    break;
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }
        
        if(draw){
            if (counter >= 0 && counter % 2 == 0){
                circle.setPosition(window.mapPixelToCoords(mPos));
                window.draw(circle);
            }else if (counter >= 0 && counter % 2 != 0){
                cross[0].setPosition(window.mapPixelToCoords(mPos));
                cross[1].setPosition(window.mapPixelToCoords(mPos));
                window.draw(cross[0]);
                window.draw(cross[1]);
                
            }
        }

        for (int i = 0; i<8; i++){
            window.draw(lines[i]);
        }
        window.display();
    }
    return 0;
}
