#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

class Passenger{
    private:
        int startLevel;
        int endLevel;
        int mass = 70;

    public:

};

int main(){
    sf::RenderWindow window(sf::VideoMode(1200, 700), "Elevator Simulation");

    sf::Texture texture;
    if (!texture.loadFromFile("img/person_sprite.jpg", sf::IntRect(0, 0, 800, 2000)))
    {
        std::cout << "Error " << std::endl;
    }
    texture.setSmooth(true);

    sf::Sprite sprite;
    sprite.setTexture(texture);   
// sa

    window.setFramerateLimit(60);
    while(window.isOpen()){
        sf::Event event;
        //test
        while (window.pollEvent(event))
        {
            switch (event.type){
            case sf::Event::Closed:
                window.close();
                break;
            }   
        }

        window.clear(sf::Color(255,255,255));
        window.draw(sprite);
        window.display();
    }
    return 0;
}