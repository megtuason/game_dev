#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SFML/Audio.hpp>

#define FPS 60
#define winW 800
#define winH 600

using namespace std;

int main( void )
{
    sf::Clock clock;
    float time;
    
    sf::Music music;
    sf::SoundBuffer buffer;
    sf::Sound sound;
    string soundFile;
    
    sf::CircleShape circListener;
    sf::CircleShape circSound;
    float radius = 20.0f;
    
    circListener.setPosition(winW/2,winH/2);
    circListener.setRadius(radius);
    circListener.setFillColor(sf::Color::Green);
    
    circSound.setPosition(winW/2,winH/2 - 50);
    circSound.setRadius(radius);
    circSound.setFillColor(sf::Color::Blue);
    
    sf::Listener::setPosition(winW/2 + radius, 0, winH/2 + radius);
    sound.setPosition(winW/2 + radius, 0, winH/2 - 50 + radius);
    sound.setMinDistance(radius*4);
    sound.setAttenuation(5.0f);
    
    float speed = 100.0f; //pixels per second
    bool isPlaying = false;
    bool isKeyLisUpPressed = false;
    bool isKeyLisLeftPressed = false;
    bool isKeyLisDownPressed = false;
    bool isKeyLisRightPressed = false;
    bool isKeyCirUpPressed = false;
    bool isKeyCirLeftPressed = false;
    bool isKeyCirDownPressed = false;
    bool isKeyCirRightPressed = false;
    
    sf::Keyboard::Key keyPlay = sf::Keyboard::Space;
    sf::Keyboard::Key keyLisUp = sf::Keyboard::W;
    sf::Keyboard::Key keyLisLeft = sf::Keyboard::A;
    sf::Keyboard::Key keyLisDown = sf::Keyboard::S;
    sf::Keyboard::Key keyLisRight = sf::Keyboard::D;
    sf::Keyboard::Key keyCirUp = sf::Keyboard::I;
    sf::Keyboard::Key keyCirLeft = sf::Keyboard::J;
    sf::Keyboard::Key keyCirDown = sf::Keyboard::K;
    sf::Keyboard::Key keyCirRight = sf::Keyboard::L;
    sf::Keyboard::Key keyHelp = sf::Keyboard::H;
    
    cout << "Input sound file: ";
    getline(cin, soundFile);
    
    sf::RenderWindow window( sf::VideoMode( winW, winH ), "SFML works!" );
    window.setFramerateLimit( FPS );
    
    cout << "Help: " << endl;
    cout << "Play Sound: Space" << endl;
    cout << "Listener Up: W" << endl;
    cout << "Listener Right: A" << endl;
    cout << "Listener Down: S" << endl;
    cout << "Listener Left: D" << endl;
    cout << "Sound Up: I" << endl;
    cout << "Sound Left: J" << endl;
    cout << "Sound Down: K" << endl;
    cout << "Sound Right: L" << endl;
    cout << "Call Help: H" << endl << endl;
    
    if( !buffer.loadFromFile("/Users/Meg/Desktop/slylve/slylve/boing.wav") )
        cout << "Unable to load sound file" << endl;
    sound.setBuffer(buffer);
    sound.setLoop(true);

    while( window.isOpen() )
    {
        sf::Event event;
        while( window.pollEvent( event ) )
        {
            switch( event.type )
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if( event.key.code == sf::Keyboard::Escape )
                {
                    window.close();
                }
                else if( event.key.code == keyPlay ) //Music
                {
                    isPlaying = !isPlaying;
                }
                else if (event.key.code == keyLisUp)
                {
                    isKeyLisUpPressed = true;
                }
                else if (event.key.code == keyLisLeft)
                {
                    isKeyLisLeftPressed = true;
                }
                else if (event.key.code == keyLisDown)
                {
                    isKeyLisDownPressed = true;
                }
                else if (event.key.code == keyLisRight)
                {
                    isKeyLisRightPressed = true;
                }
                else if (event.key.code == keyCirUp)
                {
                    isKeyCirUpPressed = true;
                }
                else if (event.key.code == keyCirLeft)
                {
                    isKeyCirLeftPressed = true;
                }
                else if (event.key.code == keyCirDown)
                {
                    isKeyCirDownPressed = true;
                }
                else if (event.key.code == keyCirRight)
                {
                    isKeyCirRightPressed = true;
                }
                break;
            case sf::Event::KeyReleased:
                if( event.key.code == keyHelp )
                {
                    cout << "Help: " << endl;
                    cout << "Play Sound: Space" << endl;
                    cout << "Listener Up: W" << endl;
                    cout << "Listener Right: A" << endl;
                    cout << "Listener Down: S" << endl;
                    cout << "Listener Left: D" << endl;
                    cout << "Sound Up: I" << endl;
                    cout << "Sound Left: J" << endl;
                    cout << "Sound Down: K" << endl;
                    cout << "Sound Right: L" << endl;
                    cout << "Call Help: H" << endl << endl;
                }
                else if (event.key.code == keyLisUp)
                {
                    isKeyLisUpPressed = false;
                }
                else if (event.key.code == keyLisLeft)
                {
                    isKeyLisLeftPressed = false;
                }
                else if (event.key.code == keyLisDown)
                {
                    isKeyLisDownPressed = false;
                }
                else if (event.key.code == keyLisRight)
                {
                    isKeyLisRightPressed = false;
                }
                else if (event.key.code == keyCirUp)
                {
                    isKeyCirUpPressed = false;
                }
                else if (event.key.code == keyCirLeft)
                {
                    isKeyCirLeftPressed = false;
                }
                else if (event.key.code == keyCirDown)
                {
                    isKeyCirDownPressed = false;
                }
                else if (event.key.code == keyCirRight)
                {
                    isKeyCirRightPressed = false;
                }
                break;
            }
        }
        
        time = clock.restart().asSeconds();
        
        //play or pause for Sound
        if(isPlaying && sound.getStatus() != sf::SoundSource::Playing)
        {
            sound.play();
        }
        else if(!isPlaying)
        {
            sound.pause();
        }
        
        //Listener WASD
        if(isKeyLisUpPressed && circListener.getPosition().y > 0.0f)
        {
            if(isKeyLisLeftPressed && circListener.getPosition().x > 0.0f)
                circListener.move(-speed*time*0.7071, -speed*time*0.7071);
            else if(isKeyLisRightPressed && (circListener.getPosition().x + 2*circListener.getRadius()) < winW)
                circListener.move(speed*time*0.7071, -speed*time*0.7071);
            else
                circListener.move(0.0f, -speed*time);
        }
        else if(isKeyLisDownPressed && (circListener.getPosition().y + 2*circListener.getRadius()) < winH)
        {
            if(isKeyLisLeftPressed && circListener.getPosition().x > 0.0f)
                circListener.move(-speed*time*0.7071, speed*time*0.7071);
            else if(isKeyLisRightPressed && (circListener.getPosition().x + 2*circListener.getRadius()) < winW)
                circListener.move(speed*time*0.7071, speed*time*0.7071);
            else
                circListener.move(0.0f, speed*time);
        }
        else if(isKeyLisLeftPressed && circListener.getPosition().x > 0.0f)
        {
            if(isKeyLisUpPressed && circListener.getPosition().y > 0.0f)
                circListener.move(-speed*time*0.7071, -speed*time*0.7071);
            else if(isKeyLisDownPressed && (circListener.getPosition().y + 2*circListener.getRadius()) < winH)
                circListener.move(-speed*time*0.7071, speed*time*0.7071);
            else
                circListener.move(-speed*time, 0.0f);
        }
        else if(isKeyLisRightPressed && (circListener.getPosition().x + 2*circListener.getRadius()) < winW)
        {
            if(isKeyLisUpPressed && circListener.getPosition().y > 0.0f)
                circListener.move(speed*time*0.7071, -speed*time*0.7071);
            else if(isKeyLisDownPressed && (circListener.getPosition().y + 2*circListener.getRadius()) < winH)
                circListener.move(speed*time*0.7071, speed*time*0.7071);
            else
                circListener.move(speed*time, 0.0f);
        }
        
        //Sound IJKL
        if(isKeyCirUpPressed && circSound.getPosition().y > 0.0f)
        {
            if(isKeyCirLeftPressed && circSound.getPosition().x > 0.0f)
                circSound.move(-speed*time*0.7071, -speed*time*0.7071);
            else if(isKeyCirRightPressed && (circSound.getPosition().x + 2*circSound.getRadius()) < winW)
                circSound.move(speed*time*0.7071, -speed*time*0.7071);
            else
                circSound.move(0.0f, -speed*time);
        }
        else if(isKeyCirDownPressed && (circSound.getPosition().y + 2*circSound.getRadius()) < winH)
        {
            if(isKeyCirLeftPressed && circSound.getPosition().x > 0.0f)
                circSound.move(-speed*time*0.7071, speed*time*0.7071);
            else if(isKeyCirRightPressed && (circSound.getPosition().x + 2*circSound.getRadius()) < winW)
                circSound.move(speed*time*0.7071, speed*time*0.7071);
            else
                circSound.move(0.0f, speed*time);
        }
        else if(isKeyCirLeftPressed && circSound.getPosition().x > 0.0f)
        {
            if(isKeyCirUpPressed && circSound.getPosition().y > 0.0f)
                circSound.move(-speed*time*0.7071, -speed*time*0.7071);
            else if(isKeyCirDownPressed && (circSound.getPosition().y + 2*circSound.getRadius()) < winH)
                circSound.move(-speed*time*0.7071, speed*time*0.7071);
            else
                circSound.move(-speed*time, 0.0f);
        }
        else if(isKeyCirRightPressed && (circSound.getPosition().x + 2*circSound.getRadius()) < winW)
        {
            if(isKeyCirUpPressed && circSound.getPosition().y > 0.0f)
                circSound.move(speed*time*0.7071, -speed*time*0.7071);
            else if(isKeyCirDownPressed && (circSound.getPosition().y + 2*circSound.getRadius()) < winH)
                circSound.move(speed*time*0.7071, speed*time*0.7071);
            else
                circSound.move(speed*time, 0.0f);
        }
        
        sf::Listener::setPosition(circListener.getPosition().x + radius, 0, circListener.getPosition().y + radius);
        sound.setPosition(circSound.getPosition().x + radius, 0, circSound.getPosition().y + radius);
        
        window.clear();
        window.draw(circListener);
        window.draw(circSound);
        window.display();
    }

    sound.stop();
    return 0;
}
