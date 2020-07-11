//
//  main.cpp
//  pong
//
//  Created by Meg Tuason on 24/09/2019.
//  Copyright © 2019 Meg Tuason. All rights reserved.
//
#include <SFML/Graphics.hpp>
#include <iostream>

#define FPS 1000
#define CSIZE 1
#define W_HEIGHT 1000
#define W_WIDTH 2000

using namespace std;

int main(int argc, const char * argv[]) {
    sf::RenderWindow window(sf::VideoMode(W_WIDTH, W_HEIGHT), "Pong Exercise");
    window.setFramerateLimit(FPS);
    sf::Color color[CSIZE];
    color[0] = sf::Color::White;

    float radius = 100.0f;
    sf::CircleShape circle(radius);
    circle.setPosition(W_WIDTH/2, W_HEIGHT/2);
    circle.setOrigin(radius, radius);
    circle.setFillColor(color[0]);

    sf::RectangleShape rects[2];
    sf::RectangleShape rect(sf::Vector2f( 200.0f, 20.0f ));
    rect.setFillColor(color[0]);
    rect.setOrigin(rect.getSize().x*0.5f, rect.getSize().y*0.5f);
    rect.rotate(90);
    rect.setPosition(100,500);
    rects[0] = rect;
    rect.setPosition(1900,500);
    rects[1] = rect;
    
    sf::CircleShape circle_lighting(20);
    circle_lighting.setRadius(radius*2);
    circle_lighting.setPosition(110, 110);
    circle_lighting.setFillColor(sf::Color(255, 0, 255, 1));
    circle_lighting.setOrigin(radius*2, radius*2);
    
    sf::RenderTexture rtex;
    rtex.create(W_WIDTH, W_HEIGHT);
    
    sf::Sprite sprite_lighting;
    sprite_lighting.setTexture(rtex.getTexture(), true);
    sprite_lighting.setPosition(0,0);
    
    sf::Texture texture;
    if(!texture.loadFromFile("/Users/Meg/Desktop/gamedev/179.14B/luk-pong/luk-pong/smoth.jpg"))
    {
        std::cout <<"Load Failed" << std::endl;
        system("pause");
    }

    circle.setTexture(&texture);
    
    sf::Clock clock;
    float timestepX = 500.0f;
    float timestepY = 0;
    float timestepXOne = 0;
    float timestepYOne = 0;
    float timestepXTwo = 0;
    float timestepYTwo = 0;
    
    float accel = 1;
	float ela = 1.0f;
    float time;
    float mass = 0.8f;
    float diag = 0.7071;
    

    float yMinC = radius;
    float yMaxC =  W_HEIGHT - radius;
    
    float yMinR = rect.getSize().y*0.5f;
    float yMaxR =  W_HEIGHT - rect.getSize().y*0.5f;

    bool POneKeyUp = false;
	bool POneKeyDown = false;
    bool PTwoKeyUp = false;
    bool PTwoKeyDown = false;
    
    while(window.isOpen())
    {
        time = clock.restart().asSeconds();
        
        sf::Event event;
        while( window.pollEvent( event ) ){
            switch( event.type ){
                case sf::Event::Closed:
                    window.close();
				    break;
                case sf::Event::KeyPressed:
                    switch( event.key.code ) {
                        case sf::Keyboard::W:
                            POneKeyUp = true;
                            break;
                        case sf::Keyboard::S:
                            POneKeyDown = true;
                            break;
                        case sf::Keyboard::I:
                            PTwoKeyUp = true;
                            break;
                        case sf::Keyboard::K:
                            PTwoKeyDown = true;
                            break;
                        case sf::Keyboard::Escape:
                            window.close();
                    }
                    break;
                case sf::Event::KeyReleased:
                    switch( event.key.code ) {
                        case sf::Keyboard::W:
                            POneKeyUp = false;
                            break;
                        case sf::Keyboard::S:
                            POneKeyDown = false;
                            break;
                        case sf::Keyboard::I:
                            PTwoKeyUp = false;
                            break;
                        case sf::Keyboard::K:
                            PTwoKeyDown = false;
                            break;
                    }
            }
        }
        
        //window bounds

        sf::Vector2f posc = circle.getPosition();
        if(posc.y < yMinC){ //top
            posc.y = yMinC;
            timestepY *= -ela;
        }
        if(posc.y > yMaxC){ //bottom
            posc.y = yMaxC;
            timestepY  *= -ela;
        }
        circle.setPosition(posc);

        sf::Vector2f pos = rects[0].getPosition();
        if(pos.y < yMinR){ //top
            pos.y = yMinR;
            timestepYOne = 0;
        }
        if(pos.y > yMaxR){ //bottom
            pos.y = yMaxR;
            timestepYOne = 0;
        }
        rects[0].setPosition(pos);

        sf::Vector2f pos1 = rects[1].getPosition();
        if(pos1.y < yMinR){ //top
            pos1.y = yMinR;
            timestepYTwo = 0;
        }
        if(pos1.y > yMaxR){ //bottom
            pos1.y = yMaxR;
            timestepYTwo = 0;
        }
        rects[1].setPosition(pos1);
        
        //collision
        if(rects[0].getGlobalBounds().intersects(circle.getGlobalBounds())){
            timestepX *= -ela;
        } else if (rects[1].getGlobalBounds().intersects(circle.getGlobalBounds())){
            timestepX *= -ela;
        }
        
        //moving player one

        if(POneKeyUp){ //key up
            rects[0].move(timestepXOne*time*diag/mass, timestepYOne*time*diag/mass);
            timestepYOne = -450.0f;
        }else if(POneKeyDown){ //key down
            rects[0].move(timestepXOne*time*diag/mass, timestepYOne*time*diag/mass);
            timestepYOne = 450.0f;
        }
        
        //moving player two

        if(PTwoKeyUp){ //key up
            rects[1].move(timestepXTwo*time*diag/mass, timestepYTwo*time*diag/mass);
            timestepYTwo = -450.0f;
        }else if(PTwoKeyDown){ //key down
            rects[1].move(timestepXTwo*time*diag/mass, timestepYTwo*time*diag/mass);
            timestepYTwo = 450.0f;
        }
        
        //ball movement

        circle.move(timestepX*time*diag/mass, timestepY*time*diag/mass);
        
        circle_lighting.setPosition(static_cast<sf::Vector2f>(circle.getPosition()));
        rtex.clear();
        rtex.draw(circle_lighting, sf::BlendNone);
        rtex.display();

        sprite_lighting.setTexture(rtex.getTexture(), true);
        window.clear(sf::Color::Blue);
        window.draw(circle);
        window.draw(rects[0]);
        window.draw(rects[1]);
        window.draw(sprite_lighting);
        window.display();
    }
    return 0;
}
