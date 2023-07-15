#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

int main(){
    sf::RenderWindow window(sf::VideoMode(1200, 700), "Elevator Simulation");
    sf::Texture texture;

    window.setFramerateLimit(60);
    while(window.isOpen()){
        sf::Event event;

        while (window.pollEvent(event))
        {
            switch (event.type){
            case sf::Event::Closed:
                window.close();
                break;
            }   
        }

        window.clear(sf::Color(255,255,255));
        window.display();
    }
    return 0;
}