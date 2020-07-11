#include <SFML/Graphics.hpp>

int main(){
    sf::RenderWindow window( sf::VideoMode( 640, 480 ), "Awesome Game");
    
    while( window.isOpen( ) ){
        sf::Event event;
        while(window.pollEvent(event)){
            switch( event.type ){
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }
        //update the game
        window.clear();
        //draw objs here
        window.display();
    }
}
