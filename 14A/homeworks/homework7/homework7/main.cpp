//
//  main.cpp
//  AABB-ex
//
//  Created by Meg Tuason on 05/11/2019.
//  Copyright © 2019 Meg Tuason. All rights reserved.
//
#include <SFML/Graphics.hpp>
#include <iostream>

#define W_HEIGHT 2000
#define W_WIDTH 2000
#define FPS 200
#define CSIZE 5

using namespace std;


int main(int argc, const char * argv[]) {
    sf::RenderWindow window(sf::VideoMode(W_HEIGHT, W_WIDTH), "AABB Exercise");
    window.setFramerateLimit(FPS);
    sf::Color color[CSIZE];
    color[0] = sf::Color::Red;
    color[1] = sf::Color::Green;
    color[2] = sf::Color::Blue;
    color[3] = sf::Color::Transparent;
    color[4] = sf::Color::Cyan;

    sf::Clock clock;
    float time;
    float speed = 200.0;
    float radius = 100.0f;
    float diag = 0.7071;
    float x = radius;
    float y = radius;
    float rotate = 0.1;

    sf::RectangleShape rects[5];
    sf::RectangleShape rect(sf::Vector2f(radius, radius));
    for (int i = 0; i < 5; i++){
        rects[i] = rect;
        rects[i].setSize(sf::Vector2f( x, y ));
        rects[i].setFillColor(color[0]);
        rects[i].setOrigin(rects[i].getSize().x+0.5f, rects[i].getSize().y*0.5f);
        if(i % 2 == 0){
            x+=150;
        } else{
            y+=150;
        }
    }

    rects[0].setPosition(800, 800);
    rects[1].setPosition(850, 600);
    rects[2].setPosition(1300, 200);
    rects[3].setPosition(1000, 400);
    rects[4].setPosition(1200, 800);

    sf::FloatRect recttemp;
    sf::RectangleShape brects[5];
    sf::RectangleShape brect(sf::Vector2f(radius, radius));
    for (int i =0; i < 5; i++){
        brects[i] = brect;
        brects[i].setFillColor(color[3]);
        brects[i].setOutlineColor(color[1]);
        brects[i].setOutlineThickness(3);
    }

    while(window.isOpen()){
        
        time = clock.restart().asSeconds();

        for(int i =0; i < 5; i++){
            recttemp = rects[i].getGlobalBounds();
            sf::Vector2f topLeft(recttemp.left, recttemp.top);
            brects[i].setPosition(topLeft);
            brects[i].setSize(sf::Vector2f(recttemp.width, recttemp.height));
        }
        
        for(int i = 0; i < 5; i++){
            for(int j = i+1; j < 5; j++){
                if(rects[i].getGlobalBounds().intersects(rects[j].getGlobalBounds())){
                    rects[i].setFillColor(color[4]);
                    rects[j].setFillColor(color[4]);
                } else{
                    rects[i].setFillColor(color[0]);
                    rects[j].setFillColor(color[0]);
                }
            }
        }

        
        sf::Event event;
        while(window.pollEvent(event)){
            switch(event.type){
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch(event.key.code){
                        case sf::Keyboard::Escape:
                            window.close();
                            break;
                    }
                    break;
            }
        }

        for(int i = 0; i < 5; i++){
            rects[i].rotate(rotate);
            rotate+= 0.1;
        }
        rotate = 0.1;
        window.clear();
        for(int i=0; i< 5; i++){
            window.draw(rects[i]);
            window.draw(brects[i]);
        }
        window.display();
    }
    return 0;
}
