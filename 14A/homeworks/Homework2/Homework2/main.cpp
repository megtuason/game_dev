#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>

#define FPS 60
#define CSIZE 5

using namespace std;

int main( void )
{
    srand(time(NULL));
    sf::RenderWindow window( sf::VideoMode( 200, 200 ), "SFML works!" );
    sf::CircleShape shape( 100.f );
    sf::Color color[CSIZE];
    color[0] = sf::Color::Green;
    color[1] = sf::Color::Blue;
    color[2] = sf::Color::Red;
    color[3] = sf::Color::Yellow;
    color[4] = sf::Color::Cyan;
    int curr = 0;
    shape.setFillColor( color[curr] );
    sf::Clock clock;
    sf::Time timer;
    sf::Time elapsed;
    sf::Time iterationTime;
    window.setFramerateLimit( FPS );
    float ittime;
    float itmili;
    float itsecond;
    while( window.isOpen() )
    {
        ittime = iterationTime.asSeconds();
        itmili = modf(ittime, &itsecond);
        itmili = (round( itmili * 1000.0 ) / 1000.0) * 1000;
        cout << "Last iteration time: " << itsecond  << " seconds and " << itmili << " milliseconds" << endl;
        
        iterationTime = clock.restart();
        timer += iterationTime;
        sf::Event event;
        if (timer.asSeconds() >= 3){
            int oldcurr = curr;
            curr = rand() % 5;
            if (oldcurr == curr){
                if (curr == 4){
                    curr = 0;
                } else {
                    curr += 1;
                }
            }
            shape.setFillColor(color[curr]);
            timer = clock.restart();
        }
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
                    break;
            }
        }
        window.clear();
        window.draw(shape);
        window.display();
        
        elapsed += iterationTime;
        float elapsedtime = elapsed.asSeconds(), elmili, elsecond;
        elmili = modf(elapsedtime, &elsecond);
        elmili = (round( elmili * 1000.0 ) / 1000.0) * 1000;
        if (elmili != 0 || elsecond != 0){
            cout << "Total time elapsed: " << elsecond  << " seconds and " << elmili << " milliseconds" << endl;
        }
    }
    return 0;
}
