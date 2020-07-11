//
//  main.cpp
//  homework6
//
//  Created by Meg Tuason on 23/10/2019.
//  Copyright © 2019 Meg Tuason. All rights reserved.
//
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#define W_HEIGHT 1000
#define W_WIDTH 1000

using namespace std;

int main(int argc, const char * argv[]) {
    sf::RenderWindow window(sf::VideoMode(W_WIDTH, W_HEIGHT), "hoemwork 6");
    sf::Clock clock;
    
    float radius = 20.0f;
    sf::CircleShape circle(radius);
    circle.setFillColor(sf::Color::Red);
    circle.setOrigin(radius, radius);
    
    sf::RectangleShape rect(sf::Vector2f(radius, radius));
    rect.setFillColor(sf::Color::White);
    
    sf::SoundBuffer buffer;
    if( !buffer.loadFromFile( "/Users/Meg/Desktop/gamedev/homeworks/homework6/homework6/boing.wav" ) ){ 
        cout << "Failed to load sound file" << endl;
        return -1;
    }
    sf::Sound sound; 
    sound.setBuffer( buffer );
    sound.setLoop(true);


    sf::Music music;
    if( !music.openFromFile( "/Users/Meg/Desktop/gamedev/homeworks/homework6/homework6/hi.wav" ) ) {
        cout << "Failed to load music file" << endl;
        return -1;
    }
    float mpitch = 1.0;
    float mvolume = 100;
    float spitch = 1.0;
    float svolume = 100;

    cout << "Press 'H' for help" << endl;
    
    bool incMusPitch = false;
    bool decMusPitch = false;
    bool incSouPitch = false;
    bool decSouPitch = false;
    bool incMusVol = false;
    bool decMusVol = false;
    bool incSouVol = false;
    bool decSouVol = false;
    
    float rx = 500;
    float ry = 500;
    float cx = 300;
    float cy = 300;
    
    float speed = 100.0f;
    float time;
    bool KeyUp = false;
    bool KeyDown = false;
    bool KeyLeft= false;
    bool KeyRight = false;
    
    rect.setPosition(rx,ry);
    circle.setPosition(cx,cy);
    
    sf::Listener::setPosition(rx + radius, 0, ry + radius);
    sound.setPosition(cx + radius, 0, cy + radius);
    sound.setMinDistance(radius*4);
    sound.setAttenuation(5.0f);
    
    
    
    while(window.isOpen())
    {         
        sf::Event event;
        while( window.pollEvent( event ) ){
            switch( event.type ){
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch( event.key.code ) {
                        case sf::Keyboard::H:
                            cout << "'Q' to play/pause music" << endl;
                            cout << "'W' to restart music" << endl;
                            cout << "'E' to increase MUSIC pitch" << endl;
                            cout << "'R' to decrease MUSIC pitch" << endl;
                            cout << "'T' to increase MUSIC volume" << endl;
                            cout << "'Y' to decrease MUSIC volume" << endl;
                            cout << "'Z' to play/pause sound" << endl;
                            cout << "'X' to restart sound" << endl;
                            cout << "'C' to increase SOUND pitch" << endl;
                            cout << "'V' to decrease SOUND pitch" << endl;
                            cout << "'B' to increase SOUND volume" << endl;
                            cout << "'N' to decrease SOUND volume" << endl;
                            break;
                        case sf::Keyboard::Q:
                            if( music.getStatus() == sf::SoundSource::Playing){
                                music.pause();
                            } else{
                                music.play();
                            }
                            break;
                        case sf::Keyboard::W:
                            if( music.getStatus() == sf::SoundSource::Playing){
                                music.stop();
                                music.play();
                            } else{
                                music.stop();
                            }
                            break;
                        case sf::Keyboard::E:
                            if( music.getStatus() == sf::SoundSource::Playing){
                                incMusPitch = true;
                            }
                            break;
                        case sf::Keyboard::R:
                            if( music.getStatus() == sf::SoundSource::Playing ){
                                decMusPitch = true;
                            }
                            break;
                        case sf::Keyboard::T:
                            if (music.getStatus() == sf::SoundSource::Playing){
                                incMusVol = true;
                            }
                            break;
                        case sf::Keyboard::Y:
                            if(music.getStatus() == sf::SoundSource::Playing){
                                decMusVol = true;
                            }
                            break;
                        case sf::Keyboard::Z:
                            if( sound.getStatus() == sf::SoundSource::Playing ){
                                sound.pause();
                            } else{
                                sound.play();
                            }
                            break;
                        case sf::Keyboard::X:
                            if( sound.getStatus() == sf::SoundSource::Playing ){
                                sound.stop();
                                sound.play();
                            } else{
                                sound.stop();
                            }
                            break;
                        case sf::Keyboard::C:
                            if( sound.getStatus() == sf::SoundSource::Playing ){
                                incSouPitch = true;
                            }
                            break;
                        case sf::Keyboard::V:
                            if( sound.getStatus() == sf::SoundSource::Playing ){
                                decSouPitch = true;
                            }
                            break;
                        case sf::Keyboard::B:
                            if (sound.getStatus() == sf::SoundSource::Playing){
                                incSouVol = true;
                            }
                            break;
                        case sf::Keyboard::N:
                            if (sound.getStatus() == sf::SoundSource::Playing){
                                decSouVol = true;
                            }
                            break;
                        case sf::Keyboard::Up:
                            KeyUp = true;
                            break;
                        case sf::Keyboard::Left:
                            KeyLeft = true;
                            break;
                        case sf::Keyboard::Down:
                            KeyDown = true;
                            break;
                        case sf::Keyboard::Right:
                            KeyRight = true;
                            break;
                        case sf::Keyboard::Escape:
                            window.close();
                    }
                    break;
                case sf::Event::KeyReleased:
                    switch( event.key.code ) {
                        case sf::Keyboard::E:
                            incMusPitch = false;
                            break;
                        case sf::Keyboard::R:
                            decMusPitch = false;
                            break;
                        case sf::Keyboard::T:
                            incMusVol = false;
                            break;
                        case sf::Keyboard::Y:
                            decMusVol = false;
                            break;
                        case sf::Keyboard::C:
                            incSouPitch = false;
                            break;
                        case sf::Keyboard::V:
                            decSouPitch = false;
                            break;
                        case sf::Keyboard::B:
                            incSouVol = false;
                            break;
                        case sf::Keyboard::N:
                            decSouVol = false;
                            break;
                        case sf::Keyboard::Up:
                            KeyUp = false;
                            break;
                        case sf::Keyboard::Left:
                            KeyLeft = false;
                            break;
                        case sf::Keyboard::Down:
                            KeyDown = false;
                            break;
                        case sf::Keyboard::Right:
                            KeyRight = false;
                            break;
                    }
            }
            if (incMusPitch){
                mpitch += 0.1;
                music.setPitch(mpitch);
            }
            if (decMusPitch){
                if (mpitch-0.1 >= 0.1){
                    mpitch -= 0.1;
                }
                music.setPitch(mpitch);
            }
            if (incMusVol){
                if (mvolume+2.5 <= 100){
                    mvolume += 2.5;
                }
                music.setVolume(mvolume);
            }
            if (decMusVol){
                if(mvolume-2.5 >= 0){
                    mvolume -= 2.5;
                }
                music.setVolume(mvolume);
            }
            if (incSouPitch){
                spitch += 0.1;
                sound.setPitch(spitch);
            }
            if (decSouPitch){
                if (spitch-0.1 >= 0.1){
                    spitch -= 0.1;
                }
                sound.setPitch(spitch);
            }
            if (incSouVol){
                if (svolume+2.5 <= 100){
                    svolume += 2.5;
                }
                sound.setVolume(svolume);
            }
            if (decSouVol){
                if(svolume-2.5 >= 0){
                    svolume -= 2.5;
                }
                sound.setVolume(svolume);
            }
            
            time = clock.restart().asSeconds();
            if(KeyUp && circle.getPosition().y > 0.0f)
            {
                if(KeyLeft && circle.getPosition().x > 0.0f)
                    circle.move(-speed*time*0.7071, -speed*time*0.7071);
                else if(KeyRight && (circle.getPosition().x + 2*circle.getRadius()) < W_WIDTH)
                    circle.move(speed*time*0.7071, -speed*time*0.7071);
                else
                    circle.move(0.0f, -speed*time);
            }
            else if(KeyDown && (circle.getPosition().y + 2*circle.getRadius()) < W_HEIGHT)
            {
                if(KeyLeft && circle.getPosition().x > 0.0f)
                    circle.move(-speed*time*0.7071, speed*time*0.7071);
                else if(KeyRight && (circle.getPosition().x + 2*circle.getRadius()) < W_WIDTH)
                    circle.move(speed*time*0.7071, speed*time*0.7071);
                else
                    circle.move(0.0f, speed*time);
            }
            else if(KeyLeft && circle.getPosition().x > 0.0f)
            {
                if(KeyUp && circle.getPosition().y > 0.0f)
                    circle.move(-speed*time*0.7071, -speed*time*0.7071);
                else if(KeyDown && (circle.getPosition().y + 2*circle.getRadius()) < W_HEIGHT)
                    circle.move(-speed*time*0.7071, speed*time*0.7071);
                else
                    circle.move(-speed*time, 0.0f);
            }
            else if(KeyRight && (circle.getPosition().x + 2*circle.getRadius()) < W_WIDTH)
            {
                if(KeyUp && circle.getPosition().y > 0.0f)
                    circle.move(speed*time*0.7071, -speed*time*0.7071);
                else if(KeyDown && (circle.getPosition().y + 2*circle.getRadius()) < W_HEIGHT)
                    circle.move(speed*time*0.7071, speed*time*0.7071);
                else
                    circle.move(speed*time, 0.0f);
            }
            sound.setPosition(circle.getPosition().x + radius, 0, circle.getPosition().y + radius);
            
            window.clear();
            window.draw(circle);
            window.draw(rect);
            window.display();
        }
    }
}
