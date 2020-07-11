//
//  main.cpp
//  homework8
//
//  Created by Meg Tuason on 11/11/2019.
//  Copyright © 2019 Meg Tuason. All rights reserved.
//

#include <iostream>
#include <SFML/Graphics.hpp>

#define W_HEIGHT 800
#define W_WIDTH 800
#define FPS 200
#define CSIZE 3

using namespace std;

float dotProduct (sf::Vector2f a, sf::Vector2f b)
{
	return ((a.x*b.x) + (a.y*b.y));
} 

int main(int argc, const char * argv[]) {
    sf::RenderWindow window(sf::VideoMode(W_HEIGHT, W_WIDTH), "Convex Polygons");
    window.setFramerateLimit(FPS);
    sf::Color color[CSIZE];
    color[0] = sf::Color::Blue;
    color[1] = sf::Color::Red;
    color[2] = sf::Color::Yellow;

    sf::Clock clock;
    float time;
    float speed = 200.0;
    float diag = 0.7071;
    float rspeed = 0.2;

    bool KeyUp = false;
    bool KeyDown = false;
    bool KeyLeft = false;
    bool KeyRight = false;
    bool rotate = false;

    int n;
    cout << "Enter number of shapes: ";
    cin >> n;
    sf::ConvexShape polygons[n];

    int pnum;
    float x;
    float y;
    float xPos;
    float yPos;
    for (int i = 0; i < n; i++){
        cout << "Enter number of points followed by coordinates and position (ex. 3 0 0 1 3 1 2 400 500): ";
        cin >> pnum;
        polygons[i].setPointCount(pnum);
        for(int j = 0; j < pnum; j++){
            cin >> x >> y;
            polygons[i].setPoint(j, sf::Vector2f(x,y));
        }
        polygons[i].setFillColor(color[0]);
        cin >> xPos >> yPos;
        polygons[i].setPosition(sf::Vector2f(xPos, yPos));
    }

    sf::Shape recttemp;
    sf::RectangleShape brects[n];
    sf::RectangleShape brect(sf::Vector2f(100,100));
    for (int i =0; i < n; i++){
        brects[i] = brect;
        brects[i].setFillColor(color[3]);
        brects[i].setOutlineColor(color[1]);
        brects[i].setOutlineThickness(3);
    }

    while(window.isOpen()){
        time = clock.restart().asSeconds();

        for(int i =0; i < 5; i++){
            recttemp = polygons[i].getGlobalBounds();
            sf::Vector2f topLeft(recttemp.left, recttemp.top);
            brects[i].setPosition(topLeft);
            brects[i].setSize(sf::Vector2f(recttemp.width, recttemp.height));
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
                            if (rotate){
                                rotate = false;
                            } else{
                                rotate = true;
                            }
                            break;
                        case::sf::Keyboard::Escape:
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
                polygons[0].move(-speed*time*diag, -speed*time*diag);
            } else if (KeyRight){
                polygons[0].move(speed*time*diag, -speed*time*diag);
            } else{
                polygons[0].move(0.0f, -speed*time);
            }
        } else if(KeyDown){
            if(KeyLeft){
                polygons[0].move(-speed*time*diag, speed*time*diag);
            } else if (KeyRight){
                polygons[0].move(speed*time*diag, speed*time*diag);
            } else{
                polygons[0].move(0.0f, speed*time);
            }
        } else if (KeyLeft){
           if(KeyUp){
                polygons[0].move(-speed*time*diag, -speed*time*diag);
            } else if (KeyDown){
                polygons[0].move(-speed*time*diag, speed*time*diag);
            } else{
                polygons[0].move(-speed*time, 0.0f);
            }
        } else if (KeyRight){
            if(KeyUp){
                polygons[0].move(speed*time*diag, -speed*time*diag);
            } else if (KeyDown){
                polygons[0].move(speed*time*diag, speed*time*diag);
            } else{
                polygons[0].move(speed*time, 0.0f);
            }
        }

        if(rotate){
            for(int i = 0; i < n; i++){
                polygons[i].rotate(rspeed);
                rspeed += 0.1;
            }
            rspeed = 0;
        }


        window.clear();
        for(int i =0; i<n; i++){
            window.draw(polygons[i]);
        }
        window.display();
    }
    return 0;
}
