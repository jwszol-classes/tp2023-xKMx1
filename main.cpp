#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

class Passenger {
private:
    int mStartLevel;
    int mEndLevel;
    int mMass = 70;
    sf::Sprite mSprite;
public:
    Passenger(const sf::Texture* texture) {
        mSprite.setTexture(*texture);
        mSprite.setScale(0.25f, 0.25f);
        mSprite.setPosition(100.0f, 100.0f);
    }

    int getStartLevel() {
        return mStartLevel;
    }

    int getEndLevel() {
        return mEndLevel;
    }

    sf::Sprite getSprite() {
        return mSprite;
    }
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

        window.clear(sf::Color(255, 255, 255));
        window.draw(first_passenger.getSprite());
        window.display();
    }
    return 0;
}