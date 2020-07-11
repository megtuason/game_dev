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

sf::RectangleShape deCollide(sf::RectangleShape a, sf::RectangleShape b, float gap)
{
    float aMinX = a.getPosition().x - a.getSize().x/2;
    float bMinX = b.getPosition().x - b.getSize().x/2;
    float aMaxX = aMinX + a.getSize().x;
    float bMaxX = bMinX + b.getSize().x;
    
    float aMinY = a.getPosition().y - a.getSize().y/2;
    float bMinY = b.getPosition().y - b.getSize().y/2;
    float aMaxY = aMinY + a.getSize().y;
    float bMaxY = bMinY + b.getSize().y;

    //all in absolute value
    float bcol = aMaxY - bMinY;
    float tcol = bMaxY - aMinY;
    float rcol = aMaxX - bMinX;
    float lcol = bMaxX - aMinX;
    
    if(bcol < rcol && bcol < lcol && bcol < tcol)
    {
        a.move(0.0f, -bcol-gap);
    }
    if(tcol < rcol && tcol < lcol && tcol < bcol)
    {
        a.move(0.0f, tcol+gap);
    }
    if(rcol < bcol && rcol < tcol && rcol < lcol)
    {
        a.move(-rcol-gap, 0.0f);
    }
    if(lcol < bcol && lcol < tcol && lcol < rcol)
    {
        a.move(lcol+gap, 0.0f);
    }
    
    return a;
}

bool onTop(sf::RectangleShape a, sf::RectangleShape b)
{
    float bMinY = b.getPosition().y - b.getSize().y/2;
    float aMinY = a.getPosition().y - a.getSize().y/2;
    float aMaxY = aMinY + a.getSize().y;
    
    if(aMaxY <= bMinY)
        return true;
    else
        return false;
}

bool onBottom(sf::RectangleShape a, sf::RectangleShape b)
{
    float bMinY = b.getPosition().y - b.getSize().y/2;
    float bMaxY = bMinY + b.getSize().y;
    float aMinY = a.getPosition().y - a.getSize().y/2;
    
    if(aMinY >= bMaxY)
        return true;
    else
        return false;
}

bool onSides(sf::RectangleShape a, sf::RectangleShape b)
{
    float aMinX = a.getPosition().x - a.getSize().x/2;
    float bMinX = b.getPosition().x - b.getSize().x/2;
    float aMaxX = aMinX + a.getSize().x;
    float bMaxX = bMinX + b.getSize().x;
    
    if(aMaxX <= bMinX || aMinX >= bMaxX)
        return true;
    else
        return false;
}
int main(int argc, const char * argv[]) {

    sf::RenderWindow window(sf::VideoMode(W_WIDTH, W_HEIGHT), "Platformer Controls");
    window.setFramerateLimit(FPS);
    sf::Clock clock;
    sf::Color color[CSIZE];
    color[0] = sf::Color::White;
    color[1] = sf::Color::Cyan;
    color[2] = sf::Color::Red;
    color[3] = sf::Color::Transparent;


    float time = 0.0;
    float time_v_safe = 0.0;
    float time_v_hold = 0.0;
    float speedX = 0.0;
    float speedY = 0.0;

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
    float cam_type;
    float cam_edges[4];
    float cam_drift;
    
    float base_h_accel;
    float air_h_accel;
    
    bool KeyUpReleased = false;
    bool isFalling = false;
    bool isJumping = false;
    bool isGrounded = false;
    
    string filename;
    string junk;

    cout << "Enter file name: ";
    cin >> filename;
    ifstream fileConfig("/Users/Meg/Desktop/gamedev/179.14B/platformer_controls/platformer_controls/" + filename );
    if (fileConfig.is_open())
    {
        fileConfig >> junk;
        fileConfig >> h_accel;
        h_accel /= FPS;
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
        max_h_vel /= FPS;
        fileConfig >> junk;
        fileConfig >> gravity;
        gravity /= FPS;
        fileConfig >> junk;
        fileConfig >> v_accel;
        v_accel /= FPS;
        fileConfig >> junk;
        fileConfig >> v_hold;
        fileConfig >> junk;
        fileConfig >> v_safe;
        fileConfig >> junk;
        fileConfig >> cut_v_vel;
        cut_v_vel /= FPS;
        fileConfig >> junk;
        fileConfig >> max_v_vel;
        max_v_vel /= FPS;
        fileConfig >> junk;
        fileConfig >> gap;
        fileConfig >> junk;
        fileConfig >> cam_type;
        fileConfig >> junk;
        fileConfig >> cam_edges[0];
        fileConfig >> cam_edges[1];
        fileConfig >> cam_edges[2];
        fileConfig >> cam_edges[3];
        fileConfig >> junk;
        fileConfig >> cam_drift;
        
        fileConfig.close();
    }
    else{
        cout << "Failed to load config /Users/Meg/Desktop/gamedev/179.14B/platformer_controls/platformer_controls/first.txt" << endl;
    }

    base_h_accel = h_accel;
    air_h_accel = h_accel*h_air;

    float Px; //player x pos
    float Py; //player y pos
    int p_width = 24; //player width
    int p_height = 32; //player height
    int N; //number of walls
    int Wx; //line x pos
    int Wy; //line y pos
    int Wwidth; //line width
    int Wheight; //line height

    sf::RectangleShape player;
    player.setSize(sf::Vector2f(p_width,p_height));
    sf::RectangleShape rect(sf::Vector2f(100.0f, 100.0f));
    
    ifstream fileSetup("/Users/Meg/Desktop/gamedev/179.14B/platformer_controls/platformer_controls/second.txt");
    if (fileSetup.is_open())
    {
        fileSetup >> Px;
        fileSetup >> Py;
        fileSetup >> N;
        fileSetup.close();
    }
    else{
        cout << "Failed to load config /Users/Meg/Desktop/gamedev/179.14B/platformer_controls/platformer_controls/second.txt" << endl;
    }

    sf::RectangleShape platforms[N];
    bool isColliding[N];
    bool isPlayerCollidingX = false;
    bool isPlayerCollidingY = false;

    ifstream fileSet("/Users/Meg/Desktop/gamedev/179.14B/platformer_controls/platformer_controls/second.txt");
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
            platforms[i].setPosition(Wx, Wy);
            platforms[i].setSize(sf::Vector2f(Wwidth, Wheight));
            platforms[i].setOrigin(platforms[i].getSize().x*0.5f, platforms[i].getSize().y*0.5f);
            platforms[i].setFillColor(color[0]);
            isColliding[i] = false;
        }
        fileSet.close();
    }
    else{
        cout << "Failed to load config /Users/Meg/Desktop/gamedev/179.14B/platformer_controls/platformer_controls/second.txt" << endl;
    }

    player.setOrigin(player.getSize().x*0.5f, player.getSize().y*0.5f);
    player.setPosition(Px,Py);
    player.setFillColor(color[1]);

    bool KeyUp = false;
    bool KeyDown = false;
    bool KeyLeft = false;
    bool KeyRight = false;
    
    float rectW = abs(cam_edges[0]-cam_edges[2]);
    float rectH = abs(cam_edges[1]-cam_edges[3]);
    
    sf::RectangleShape edges(sf::Vector2f(rectW, rectH));
    edges.setPosition(cam_edges[0], cam_edges[1]);
    edges.setOrigin(rectW*0.5f, rectH*0.5f);
    edges.setOutlineColor(color[2]);
    edges.setOutlineThickness(3);
    edges.setFillColor(color[3]);
    
    sf::View view;
    view.setSize(rectW,rectH);//size of camera to be shown
    view.setViewport(sf::FloatRect(0,0,1,1)); //whole window
//    view.setViewport(sf::FloatRect(0.25f, 0.25f, 0.5f, 0.5f)); //half of window dimensions
    view.setCenter(player.getPosition());//what the camera should focus on
    edges.setPosition(player.getPosition());
    
    float xMin = rectW*0.5f;
    float yMin = rectH*0.5f;
    float xMax =  W_WIDTH - rectW*0.5f;
    float yMax =  W_HEIGHT - rectH*0.5f;
    
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event))
        {
            switch(event.type)
            {
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
        
        time = clock.restart().asSeconds()*FPS;
        time_v_safe += time;
        
        //Horizontal Logic
        if(!isGrounded)
            h_accel = air_h_accel;
        else
            h_accel = base_h_accel;
        
        if(KeyLeft)
        {
            if(speedX > 0)
                speedX -= h_accel*h_opposite;
            else
                speedX -= h_accel;
        }
        else if(KeyRight)
        {
            if(speedX < 0)
                speedX += h_accel*h_opposite;
            else
                speedX += h_accel;
        }
        else
        {
            if(speedX < min_h_vel && speedX > -min_h_vel)
                speedX = 0.0;
            else
                speedX *= h_coeff;
        }
        
        if(speedX > max_h_vel)
            speedX = max_h_vel;
        else if(speedX < -max_h_vel)
            speedX = -max_h_vel;
        
        //Vertical Logic
        if(speedY > max_v_vel)
            speedY = max_v_vel;
        else
            speedY += gravity;
        
        if(KeyUp && (isGrounded || isJumping) && (time_v_hold) <= v_hold && !isFalling)
        {
            isJumping = true;
            KeyUpReleased = false;
            time_v_hold += time;
            speedY += v_accel;
        }
        else if(!KeyUpReleased)
        {
            KeyUpReleased = true;
            isFalling = true;
            isJumping = false;
            time_v_hold = 0;
            if(speedY < cut_v_vel)
                speedY = cut_v_vel;
        }
        
        //cout << speedY << endl;
        player.move(speedX*time, speedY*time);
        
        //Collision
        for (int i = 0; i < N; i++)
        {
            if(colCheck(player,platforms[i]))
            {
                player = deCollide(player, platforms[i], gap);
                if(onSides(player,platforms[i]))
                {
                    speedX = 0.0;
                }
                if(onTop(player,platforms[i]))
                {
                    time_v_safe = 0.0;
                    time_v_hold = 0;
                    speedY = 0.0;
                    isGrounded = true;
                    isPlayerCollidingY = true;
                    isFalling = false;
                    isJumping = false;
                }
                else if(onBottom(player, platforms[i]))
                {
                    time_v_safe = 0.0;
                    time_v_hold = 0;
                    speedY = 0.0;
                    isGrounded = false;
                    isPlayerCollidingY = false;
                    isFalling = true;
                    isJumping = false;
                }
            }
        }
        
        if((time_v_safe) >= v_safe && !isPlayerCollidingY)
        {
            time_v_safe = 0.0;
            isGrounded = false;
        }
        isPlayerCollidingY = false;
        
        if(cam_type == 0 || cam_type == 1){
            view.setCenter(player.getPosition());//what the camera should focus on
            edges.setPosition(player.getPosition());
        }
        if(cam_type == 1){
            sf::Vector2f pos = edges.getPosition();

            if(pos.x < xMin){ //left
                pos.x = xMin;
            }
            if(pos.x > xMax){ //right
                pos.x = xMax;
            }
            if(pos.y < yMin){ //top
                pos.y = yMin;
            }
            if(pos.y > yMax){ //bottom
                pos.y = yMax;
            }
            edges.setPosition(pos);
            view.setCenter(pos);
        } else if(cam_type == 2){
            sf::Vector2f pos = player.getPosition();
            sf::Vector2f borderpos = edges.getPosition();
            xMin = (borderpos.x - borderpos.x/2) + (p_width*0.5f);
            yMin = (borderpos.y - borderpos.y/2) + (p_height*0.5f);
            xMax = (xMin+rectW) - (p_width*0.5f);
            yMax = (yMin+rectH) - (p_height*0.5f);
            if(pos.x < xMin){ //left
                edges.setPosition(borderpos.x-(abs(xMin-pos.x)), borderpos.y);
            }
            if(pos.x > xMax){ //right
                edges.setPosition(borderpos.x+abs(xMax-pos.x), borderpos.y);
            }
            if(pos.y < yMin){ //top
                edges.setPosition(borderpos.x, borderpos.y+abs(yMin-pos.y));
            }
            if(pos.y > yMax){ //bottom
                edges.setPosition(borderpos.x, borderpos.y-abs(yMin-pos.y));
                
            }
            view.setCenter(edges.getPosition());
        }

        window.clear();
        window.setView(view);

        //default view
        //window.setView(window.getDefaultView() );
        window.draw(player);
        for(int i = 0; i < N; i++){
            window.draw(platforms[i]);
            isColliding[i] = false;
        }
        window.draw(edges);
        window.display();
        
        }
        

        
                
        return 0;
    }
