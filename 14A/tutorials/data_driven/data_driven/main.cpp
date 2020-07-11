//
//  main.cpp
//  homework4
//
//  Created by Meg Tuason on 24/09/2019.
//  Copyright © 2019 Meg Tuason. All rights reserved.
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <math.h>

#define FPS 1000
#define CSIZE 4
#define W_HEIGHT 1400
#define W_WIDTH 2000

using namespace std;

int main(int argc, const char * argv[]) {
    ifstream file_reader;
    string file = "/Users/Meg/Desktop/gamedev/tutorials/data_driven/data_driven/var.txt";
    file_reader.open(file);
    if(!file_reader){
        cout<<"Error in opening file..";
        return 0;
    }

    sf::RenderWindow window(sf::VideoMode(W_WIDTH, W_HEIGHT), "Data Driven");
    window.setFramerateLimit(FPS);
    sf::Clock clock;
    sf::Color color[CSIZE];
    int r, g, b;
    for(int i =0; i < CSIZE; i++){
        file_reader >> r >> g >> b;
        color[i] = sf::Color(r, g, b);
    }

    bool frick = false;

    int input;
    cout << "Enter number of circles: ";
    cin >> input;

    float radius;
    float xpos, ypos;
    float accel;
    float ela;
    float time;
    float mass;
    float diag;
    file_reader >> radius >> xpos >> ypos >> accel >> ela >> mass >> diag;
    
    sf::CircleShape circles[input+1];
    sf::CircleShape circle(radius);
    circle.setFillColor(color[0]);
    circle.setOrigin(radius, radius);
    circles[0] = circle;
    circles[0].setPosition(xpos, ypos);

    int x = 100;
    int y = 100;
    int z;
    circle.setFillColor(color[2]);
    for(int i = 1; i <= input; i++){
        circle.setPosition(x, y);
        circles[i] = circle;
        if(i > 10 && i % 13 == 0){
            z += 150;
            y = 0;
            x = z;
        }
        x+=100;
        y+=100;
    }
    
    float overlap = 0;
    float speedsX[input+1];
    float speedsY[input+1];
    for(int i = 0; i <= input; i++){
        speedsX[i] = 0;
        speedsY[i] = 0;
    }
    
    float xMin = radius;
    float yMin = radius;
    float xMax =  W_WIDTH - radius;
    float yMax =  W_HEIGHT - radius;

    bool KeyUp = false;
    bool KeyDown = false;
    bool KeyLeft= false;
    bool KeyRight = false;

    float x1;
    float y1;
    float r1;
    float x2;
    float y2;
    float r2;
    float dis;
    
    while(window.isOpen())
    {
        time = clock.restart().asSeconds();
        
        if(frick){
            circles[0].setFillColor(color[1]);
            for (int i = 1; i <= input; i++){
                circles[i].setFillColor(color[3]);
            }
        } else{
            circles[0].setFillColor(color[0]);
            for (int i = 1; i <= input; i++){
                circles[i].setFillColor(color[2]);
            }
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

        sf::Vector2f pos;
        for (int i = 0; i <= input; i++){
            pos = circles[i].getPosition();
            if(pos.x < xMin){ //left
                pos.x = xMin;
                speedsX[i]  *= -ela;
                if(frick){
                    speedsX[i] -= accel;
                }
            }
            if(pos.x > xMax){ //right
                pos.x = xMax;
                speedsX[i]  *= -ela;
                if(frick){
                    speedsX[i] += accel;
                }
            }
            if(pos.y < yMin){ //top
                pos.y = yMin;
                speedsY[i] *= -ela;
                if(frick){
                    speedsY[i] -= accel;
                }
            }
            if(pos.y > yMax){ //bottom
                pos.y = yMax;
                speedsY[i]  *= -ela;
                if(frick){
                    speedsY[i] += accel;
                }
            }
            circles[i].setPosition(pos);
        }

        for (int i = 0; i <= input; i++){
            x1 = circles[i].getPosition().x;
            y1 = circles[i].getPosition().y;
            r1 = circles[i].getRadius();
            for (int j = 0; j <= input; j++){
                if (i == j){
                    continue;
                } else{
                    x2 = circles[j].getPosition().x;
                    y2 = circles[j].getPosition().y;
                    r2 = circles[j].getRadius();

                    dis = sqrt(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)));
                    if(dis <= r1+r2) {
                        overlap = ((r1+r2)-dis)/2;
                        speedsX[i] = -speedsX[i]; //
                        speedsY[i] = -speedsY[i]; //
                        speedsX[j] = -speedsX[i]; //
                        speedsY[j] = -speedsY[i]; //
                        
                        if (speedsX[i] < 0){
                            speedsX[i] -= overlap;
                        } else if (speedsX[i] > 0){
                            speedsX[i] += overlap;
                        }
                        if (speedsY[i] < 0){
                            speedsY[i] -= overlap;
                        } else if (speedsY[i] > 0){
                            speedsY[i] += overlap;
                        }
                        if (speedsX[j] < 0){
                            speedsX[j] -= overlap;
                        } else if (speedsX[j] > 0){
                            speedsX[j] += overlap;
                        }
                        if (speedsY[j] < 0){
                            speedsY[j] -= overlap;
                        } else if (speedsY[j] > 0){
                            speedsY[j] += overlap;
                        }
                        circles[i].move(speedsX[i]*time*diag/mass,speedsY[i]*time*diag/mass);
                        circles[j].move(speedsX[j]*time*diag/mass,speedsY[j]*time*diag/mass);
                        
                        if (speedsX[i] < 0){
                            speedsX[i] += overlap;
                        } else if (speedsX[i] > 0){
                            speedsX[i] -= overlap;
                        }
                        if (speedsY[i] < 0){
                            speedsY[i] += overlap;
                        } else if (speedsY[i] > 0){
                            speedsY[i] -= overlap;
                        }
                        if (speedsX[j] < 0){
                            speedsX[j] += overlap;
                        } else if (speedsX[j] > 0){
                            speedsX[j] -= overlap;
                        }
                        if (speedsY[j] < 0){
                            speedsY[j] += overlap;
                        } else if (speedsY[j] > 0){
                            speedsY[j] -= overlap;
                        }
                    }
                }
            }
        }
        
        for (int i = 1; i <= input; i++){
            if(speedsX[i] != 0 && frick){
                if((-0.2) <= speedsX[i] && speedsX[i] <= 0.2){
                    speedsX[i] = 0;
                }else if (speedsX[i] > 0.2){
                    speedsX[i] -= accel;
                } else if (speedsX[i] < (-0.2)){
                    speedsX[i] += accel;
                }
            }
            if (speedsY[i] != 0 && frick){
                if((-0.2) <= speedsY[i] && speedsY[i] <= 0.2){
                    speedsY[i] = 0;
                }else if (speedsY[i] > 0.2){
                    speedsY[i] -= accel;
                } else if (speedsY[i] < (-0.2)){
                    speedsY[i] += accel;
                }
            }
            circles[i].move(speedsX[i]*time*diag/mass, speedsY[i]*time*diag/mass);
        }
        

        if(KeyUp){ //key up
            if(KeyLeft){ //up left
                circles[0].move(speedsX[0]*time*diag/mass, speedsY[0]*time*diag/mass);
                speedsY[0] -= accel;
                speedsX[0] -= accel;
            }else if(KeyRight){ //up right
                circles[0].move(speedsX[0]*time*diag/mass, speedsY[0]*time*diag/mass);
                speedsY[0] -= accel;
                speedsX[0] += accel;
            }else{
                if (speedsX[0] != 0 && frick){
                    if((-0.2) <= speedsX[0] && speedsX[0] <= 0.2){
                        speedsX[0] = 0;
                    }else if (speedsX[0] > 0.2){
                        speedsX[0] -= accel;
                    } else if (speedsX[0] < (-0.2)){
                        speedsX[0] += accel;
                    }
                }
                circles[0].move(speedsX[0]*time*diag/mass, speedsY[0]*time*diag/mass);
                speedsY[0] -= accel;
            }
        }else if(KeyDown){ //key down
            if(KeyLeft){
                circles[0].move(speedsX[0]*time*diag/mass, speedsY[0]*time*diag/mass);
                speedsY[0] += accel;
                speedsX[0] -= accel;
            } else if(KeyRight){
                circles[0].move(speedsX[0]*time*diag/mass, speedsY[0]*time*diag/mass);
                speedsY[0] += accel;
                speedsX[0] += accel;
            } else {
                if (speedsX[0] != 0 && frick ){
                    if((-0.2) <= speedsX[0] && speedsX[0] <= 0.2){
                        speedsX[0] = 0;
                    }else if (speedsX[0] > 0.2){
                        speedsX[0] -= accel;
                    } else if (speedsX[0] < (-0.2)){
                        speedsX[0] += accel;
                    }
                }
                circles[0].move(speedsX[0]*time*diag/mass, speedsY[0]*time*diag/mass);
                speedsY[0] += accel;
            }
        }else if(KeyLeft){ //key left
            if(KeyUp){
                circles[0].move(speedsX[0]*time*diag/mass, speedsY[0]*time*diag/mass);
                speedsY[0] -= accel;
                speedsX[0] -= accel;
            }
            else if(KeyDown){
                circles[0].move(speedsX[0]*time*diag/mass, speedsY[0]*time*diag/mass);
                speedsY[0] += accel;
                speedsX[0] -= accel;
            }
            else {
                if (speedsY[0] != 0 && frick){
                    if((-0.2) <= speedsY[0] && speedsY[0] <= 0.2){
                        speedsY[0] = 0;
                    }else if (speedsY[0] > 0.2){
                        speedsY[0] -= accel;
                    } else if (speedsY[0] < (-0.2)){
                        speedsY[0] += accel;
                    }
                }
                circles[0].move(speedsX[0]*time*diag/mass, speedsY[0]*time*diag/mass);
                speedsX[0] -= accel;
            }
        }else if(KeyRight){ //key right
            if(KeyUp){
                circles[0].move(speedsX[0]*time*diag/mass, speedsY[0]*time*diag/mass);
                speedsY[0] -= accel;
                speedsX[0] += accel;
            }
            else if(KeyDown){
                circles[0].move(speedsX[0]*time*diag/mass, speedsY[0]*time*diag/mass);
                speedsY[0] += accel;
                speedsX[0] += accel;
            }
            else {
                if (speedsY[0] != 0 && frick){
                    if((-0.2) <= speedsY[0] && speedsY[0] <= 0.2){
                        speedsY[0] = 0;
                    }else if (speedsY[0] > 0.2){
                        speedsY[0] -= accel;
                    } else if (speedsY[0] < (-0.2)){
                        speedsY[0] += accel;
                    }
                }
                circles[0].move(speedsX[0]*time*diag/mass, speedsY[0]*time*diag/mass);
                speedsX[0] += accel;
            }
        } else { //all released
            if(speedsX[0] != 0 && frick){
                if((-0.2) <= speedsX[0] && speedsX[0] <= 0.2){
                    speedsX[0] = 0;
                }else if (speedsX[0] > 0.2){
                    speedsX[0] -= accel;
                } else if (speedsX[0] < (-0.2)){
                    speedsX[0] += accel;
                }
            }
            if (speedsY[0] != 0 && frick){
                if((-0.2) <= speedsY[0] && speedsY[0] <= 0.2){
                    speedsY[0] = 0;
                }else if (speedsY[0] > 0.2){
                    speedsY[0] -= accel;
                } else if (speedsY[0] < (-0.2)){
                    speedsY[0] += accel;
                }
            }
            circles[0].move(speedsX[0]*time*diag/mass, speedsY[0]*time*diag/mass);
        }
//        cout << "X: " << speedsX[0] << endl;
//        cout << "Y: " << speedsY[0] << endl;
        
        window.clear();
        for(int i = 0; i <= input; i++){
            window.draw(circles[i]);
        }

        window.display();
    }
    return 0;
}
