#include <SFML/Graphics.hpp>
#include <iostream>

#define FPS 60
#define CSIZE 3

using namespace std;

int main( void )
{
    sf::RenderWindow window( sf::VideoMode( 200, 200 ), "SFML works!" );
    sf::CircleShape shape( 100.f );
	sf::Color color[CSIZE];
	color[0] = sf::Color::Green;
	color[1] = sf::Color::Blue;
	color[2] = sf::Color::Red;
	int curr = 0;
    shape.setFillColor( color[curr] );
	sf::Clock clock;
	window.setFramerateLimit( FPS );

    while( window.isOpen() )
    {
		sf::Time elapsed = clock.restart();
        sf::Event event;
		cout << "iteration\n";
        while( window.pollEvent( event ) )
        {
            switch( event.type )
			{
			case sf::Event::Closed:
                window.close();
				break;
			case sf::Event::MouseButtonPressed:
				if( event.mouseButton.button == sf::Mouse::Left )
				{
					curr = (curr + 1) % CSIZE;
					shape.setFillColor( color[curr] );
				}
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
		
		cout << elapsed.asSeconds() << "\n";
    }

    return 0;
}