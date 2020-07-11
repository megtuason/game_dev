//
//  main.cpp
//  platformer_controls
//
//  Created by Meg Tuason on 29/02/2020.
//  Copyright © 2020 Meg Tuason. All rights reserved.
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

#define FPS 60
#define CSIZE 4
#define W_HEIGHT 600
#define W_WIDTH 800

using namespace std;

bool colCheck(sf::RectangleShape a, sf::RectangleShape b)
{
    float aMinX = a.getPosition().x - a.getSize().x/2;
    float aMinY = a.getPosition().y - a.getSize().y/2;
    float bMinX = b.getPosition().x - b.getSize().x/2;
    float bMinY = b.getPosition().y - b.getSize().y/2;
    float aMaxX = aMinX + a.getSize().x;
    float aMaxY = aMinY + a.getSize().y;
    float bMaxX = bMinX + b.getSize().x;
    float bMaxY = bMinY + b.getSize().y;
    
    if(aMaxX < bMinX || aMinX > bMaxX || aMaxY < bMinY || aMinY > bMaxY){
        return false;
    } else{
        return true;
    }
}


int colCheckSide(sf::RectangleShape a, sf::RectangleShape b)
{
    float aMinX = a.getPosition().x - a.getSize().x/2;
    float aMinY = a.getPosition().y - a.getSize().y/2;
    float bMinX = b.getPosition().x - b.getSize().x/2;
    float bMinY = b.getPosition().y - b.getSize().y/2;
    float aMaxX = aMinX + a.getSize().x;
    float aMaxY = aMinY + a.getSize().y;
    float bMaxX = bMinX + b.getSize().x;
    float bMaxY = bMinY + b.getSize().y;
    
    if(aMaxX > bMinX && aMinX < bMinX){
        return 1;
    }else if (aMinX < bMaxX && aMaxX > bMaxX){
        return 2;
    } else if (aMaxY > bMinY && aMinY < bMinY){
        return 3;
    } else if ( aMinY < bMaxY && aMaxY > bMaxY){
        return 4;
    } else{
        return 0;
    }
}

float getDistanceRight(sf::RectangleShape a, sf::RectangleShape b){
    float aMinX = a.getPosition().x - a.getSize().x/2;
    float bMinX = b.getPosition().x - b.getSize().x/2;
    float aMaxX = aMinX + a.getSize().x;
    
    float distance = abs(aMaxX-bMinX);
    return distance;
}

float getDistanceLeft(sf::RectangleShape a, sf::RectangleShape b){
    float aMinX = a.getPosition().x - a.getSize().x/2;
    float bMinX = b.getPosition().x - b.getSize().x/2;
    float bMaxX = bMinX + b.getSize().x;
    
    float distance = abs(bMaxX-aMinX);
    return distance;
}

float getDistanceBottom(sf::RectangleShape a, sf::RectangleShape b){
    float aMinY = a.getPosition().y - a.getSize().y/2;
    float bMinY = b.getPosition().y - b.getSize().y/2;
    float aMaxY = aMinY + a.getSize().y;
    
    float distance = abs(aMaxY-bMinY);
    return distance;
}

float getDistanceTop(sf::RectangleShape a, sf::RectangleShape b){
    float aMinY = a.getPosition().y - a.getSize().y/2;
    float bMinY = b.getPosition().y - b.getSize().y/2;
    float bMaxY = bMinY + b.getSize().y;
    
    float distance = abs(aMinY-bMaxY);
    return distance;
}

int main(int argc, const char * argv[]) {

    sf::RenderWindow window(sf::VideoMode(W_WIDTH, W_HEIGHT), "Platformer Controls");
    window.setFramerateLimit(FPS);
    sf::Clock clock;
    sf::Color color[CSIZE];
    color[0] = sf::Color::White;
    color[1] = sf::Color::Cyan;

    float time;
    float timestepX = 0;
    float timestepY = 0;

    float h_accel; //horizontal accel
    float h_coeff; //multiplier for no input
    float h_opposite; //opposite accel
    float h_air; //accel in air
    float min_h_vel; //horizontal stop
    float max_h_vel; //max speed
    float gravity; //vertical accel from gravity
    float v_accel; //vertical accel from input
    float v_hold;
    float v_safe;
    float cut_v_vel;
    float max_v_vel;
    float gap;
//    int currFrame = 0;
    int jumpFrame = 0;
    
    string filename;
    string junk;

    cout << "Enter file name: ";
    cin >> filename;
    ifstream fileConfig("/Users/Meg/Desktop/gamedev/179.14B/platformer_controls/platformer_controls/" + filename );
	if (fileConfig.is_open())
	{
		fileConfig >> junk;
		fileConfig >> h_accel;
        fileConfig >> junk;
        fileConfig >> h_coeff;
        fileConfig >> junk;
        fileConfig >> h_opposite;
        fileConfig >> junk;
        fileConfig >> h_air;
        fileConfig >> junk;
        fileConfig >> min_h_vel;
        fileConfig >> junk;
        fileConfig >> max_h_vel;
        fileConfig >> junk;
        fileConfig >> gravity;
        fileConfig >> junk;
        fileConfig >> v_accel;
        fileConfig >> junk;
        fileConfig >> v_hold;
        fileConfig >> junk;
        fileConfig >> v_safe;
        fileConfig >> junk;
        fileConfig >> cut_v_vel;
        fileConfig >> junk;
        fileConfig >> max_v_vel;
        fileConfig >> junk;
        fileConfig >> gap;
		fileConfig.close();
	}
	else{
		cout << "Failed to load config (/Users/Meg/Desktop/gamedev/space/space/res/texts/first.txt)" << endl;
	}
//    h_accel /= FPS;
//    max_h_vel /= FPS;
//    gravity  /= FPS;
//    cut_v_vel /= FPS;
//    max_v_vel /= FPS;
    v_accel /= FPS;

    float Px; //player x pos
    float Py; //player y pos
    int p_width = 24; //player width
    int p_height = 32; //player height
    int N; //number of walls
    int Wx; //line x pos
    int Wy; //line y pos
    int Wwidth; //line width
    int Wheight; //line height

    sf::RectangleShape player(sf::Vector2f(p_width,p_height));
    sf::RectangleShape rect(sf::Vector2f(100.0f, 100.0f));
    
    ifstream fileSetup("/Users/Meg/Desktop/gamedev/179.14B/platformer_controls/platformer_controls/second.txt" );
	if (fileSetup.is_open())
	{
		fileSetup >> Px;
        fileSetup >> Py;
        fileSetup >> N;
		fileSetup.close();
	}
	else{
		cout << "Failed to load config (/Users/Meg/Desktop/gamedev/space/space/res/texts/second.txt)" << endl;
	}

    sf::RectangleShape platforms[N];

    ifstream fileSet("/Users/Meg/Desktop/gamedev/179.14B/platformer_controls/platformer_controls/second.txt" );
	if (fileSet.is_open())
	{
		fileSet >> Px;
        fileSet >> Py;
        fileSet >> N;
        for(int i = 0; i < N; i++){
            fileSet >> Wx;
            fileSet >> Wy;
            fileSet >> Wwidth;
            fileSet >> Wheight;
            rect.setPosition(Wx, Wy);
            rect.setSize(sf::Vector2f(Wwidth, Wheight));
            rect.setOrigin(rect.getSize().x*0.5f, rect.getSize().y*0.5f);
            rect.setFillColor(color[0]);
            platforms[i] = rect;
        }
		fileSet.close();
	}
	else{
		cout << "Failed to load config (/Users/Meg/Desktop/gamedev/space/space/res/texts/secondpart.txt)" << endl;
	}

    player.setOrigin(player.getSize().x*0.5f, player.getSize().y*0.5f);
    player.setPosition(Px,Py);
    player.setFillColor(color[1]);

    bool KeyUp = false;
    bool KeyDown = false;
    bool KeyLeft = false;
    bool KeyRight = false;
    bool isGrounded = false;
    bool isJumping = false;

    while(window.isOpen()){

        time = clock.restart().asSeconds();
        
//        if(currFrame == FPS-1){
//            cout << "RESET" << endl;
//            currFrame = 0;
//        }
//        currFrame += 1;

        sf::Event event;
        while(window.pollEvent(event)){
            switch(event.type){
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch( event.key.code ){
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
                        case sf::Keyboard::Escape:
                            window.close();
                    }
                    break;
                case sf::Event::KeyReleased:
                    switch(event.key.code){
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
    
    timestepY += gravity*time;
    if(abs(timestepY) >= max_v_vel){
        timestepY = -max_v_vel;
    }
    player.move(0.0f, timestepY*time);
        
    for(int i = 0; i < N; i++){
        if(colCheck(player, platforms[i])){
            int result = colCheckSide(player, platforms[i]);
            float currentPosX = player.getPosition().x;
            float currentPosY = player.getPosition().y;
            if(result == 1){
                player.setPosition(currentPosX-(getDistanceRight(player, platforms[i]) + gap), currentPosY);
                timestepX = 0;
            } else if (result == 2){
                player.setPosition(currentPosX+(getDistanceLeft(player, platforms[i])+gap), currentPosY);
                timestepX = 0;
            } else if (result == 3){
                isGrounded = true;
                isJumping = false;
                jumpFrame = 0;
                player.setPosition(currentPosX, currentPosY-(getDistanceBottom(player, platforms[i]))+gap);
                timestepY = 0;
            } else if (result == 4){
                player.setPosition(currentPosX, currentPosY+(getDistanceTop(player, platforms[i])+gap));
                timestepY = 0;
            }
        }
    }
    

    if(KeyLeft){
        timestepX -= h_accel;
        if(abs(timestepX) >= max_h_vel){
            timestepX = -max_h_vel;
        }
    } else if(KeyRight){
        timestepX += h_accel;
        if(abs(timestepX) >= max_h_vel){
            timestepX = max_h_vel;
        }
    }else if (KeyUp && (isGrounded || isJumping)){
        if(jumpFrame < v_hold){
            timestepY += v_accel;
            if(abs(timestepY) > cut_v_vel){
                timestepY = cut_v_vel;
            }
            isJumping = true;
            isGrounded = false;
            jumpFrame += 1;
            cout << "jumpin" << endl;
        }else{
            cout << "cap reached" << endl;
        }
    } else{
        if(abs(timestepX) < min_h_vel){
            timestepX = 0;
        } else{
            if (timestepX < 0){
                timestepX += h_accel;
            } else if (timestepX > 0){
                timestepX -= h_accel;
            }
        }
        jumpFrame = 0;
        isJumping = false;
    }
        
    player.move(timestepX*time, timestepY*time);
    window.clear();
    window.draw(player);
    for(int i = 0; i < N; i++){
        window.draw(platforms[i]);
    }
    window.display();
    }
    return 0;
}
