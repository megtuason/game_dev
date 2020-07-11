//
//  main.cpp
//  bezier-curve
//
//  Created by Meg Tuason on 02/02/2020.
//  Copyright © 2020 Meg Tuason. All rights reserved.
//
#include <SFML/Graphics.hpp>
#include <iostream>

#define FPS 1000
#define CSIZE 4
#define W_HEIGHT 800
#define W_WIDTH 800

using namespace std;

sf::VertexArray createBezier(sf::Vertex p1, sf::Vertex control, sf::Vertex p2, int step){
    
    sf::VertexArray lineP1C(sf::LineStrip, step);
    sf::VertexArray lineP2C(sf::LineStrip, step);
	sf::VertexArray bezier(sf::LineStrip, step+1);

    lineP1C[0] = p1;
	lineP2C[0] = control;
    float stepX1 =  (control.position.x - p1.position.x)/step;
	float stepY1 =  (control.position.y - p1.position.y)/step;
	float stepX2 =  (p2.position.x - control.position.x)/step;
	float stepY2 =  (p2.position.y - control.position.y)/step;

    for (int i = 1; i < step; i++)
	{
		lineP1C[i] = lineP1C[i-1];
		lineP2C[i] = lineP2C[i-1];
		lineP1C[i].position.x += stepX1; 
		lineP1C[i].position.y += stepY1; 
		lineP2C[i].position.x += stepX2; 
		lineP2C[i].position.y += stepY2; 
	}
    
    bezier[0] = p1;
	bezier[step] = p2;
	
	for (int i = 1; i < step; i++)
	{
		bezier[i] = lineP1C[i];
		bezier[i].position.x += i*(lineP2C[i].position.x - lineP1C[i].position.x)/step;
		bezier[i].position.y += i*(lineP2C[i].position.y - lineP1C[i].position.y)/step;
	}

    return bezier;
}

int main(int argc, const char * argv[]) {
    
    sf::RenderWindow window(sf::VideoMode(W_WIDTH, W_HEIGHT), "Bezier Curve HW 1");
    window.setFramerateLimit(FPS);
    sf::Color color[CSIZE];
    color[0] = sf::Color::Blue;
    color[1] = sf::Color::Transparent;

    int step;
    cin >> step;
    int control;
    cin >> control;

    int beznum = (control-1)/2;
    sf::VertexArray bezier[beznum];
    sf::Vertex points[control];

    sf::CircleShape circles[control];
    float radius = 10.0f;
    sf::CircleShape circle(radius);
    circle.setOrigin(radius,radius);
    circle.setFillColor(color[1]);
    circle.setOutlineColor(color[0]);
    circle.setOutlineThickness(2);

    for (int i = 0,j = 0, k = 0; i < control; i++){
        cin >> points[i].position.x >> points[i].position.y;
        circle.setPosition(points[i].position);
        circles[i] = circle;
        k += 1;
        if(k == 3){
            bezier[j].setPrimitiveType(sf::LinesStrip);
            bezier[j].resize(step);
            bezier[j] = createBezier(points[i-2], points[i-1], points[i],step);
            j += 1;
            k = 1;
        }
    }

    bool isMouseLeftButtonPressed = false;
    bool inCircle = false;
    int pos;

    while(window.isOpen())
    {
        sf::Event event;
        while( window.pollEvent( event ) ){
            switch( event.type ){
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Left){
						isMouseLeftButtonPressed = true;
                    }
					break;
				case sf::Event::MouseButtonReleased:
					if (event.mouseButton.button == sf::Mouse::Left){
						isMouseLeftButtonPressed = false;
					}
					break;
            }
        }

        if(isMouseLeftButtonPressed){
            sf::Vector2i mPos = sf::Mouse::getPosition(window);
            if(inCircle){
                points[pos].position = sf::Vector2f(mPos);
                circles[pos].setPosition(sf::Vector2f(mPos));
            } else{
                for(int i = 0; i < control; i++){
                    if (circles[i].getGlobalBounds().contains(window.mapPixelToCoords(mPos))){
                        inCircle = true;
                        pos = i;
                    }
                }
            }
        }else{
            inCircle = false;
        }

        for (int i = 0,j = 0, k = 0; i < control; i++){
            k += 1;
            if(k == 3){
                bezier[j].setPrimitiveType(sf::LinesStrip);
                bezier[j].resize(step);
                bezier[j] = createBezier(points[i-2], points[i-1], points[i],step);
                j += 1;
                k = 1;
            }
        }

        window.clear();
        for(int i = 0; i < control; i++){
            window.draw(circles[i]);
        }
        for(int i = 0; i < beznum; i++){
            window.draw(bezier[i]);
        }
        window.display();
                          
    }
    return 0;
}
