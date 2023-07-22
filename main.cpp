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

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 700), "Elevator Simulation");

    sf::Texture texture;
    if (!texture.loadFromFile("img/person_sprite.png"))
    {
        std::cout << "Error " << std::endl;
    }
    texture.setSmooth(true);

    Passenger first_passenger(&texture);

    window.setFramerateLimit(60);
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event))
        {
            switch (event.type) {
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