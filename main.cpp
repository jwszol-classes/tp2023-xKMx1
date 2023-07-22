#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

class Elevator {
private:
    int mPassengerMass;
    int mCurrentLevel;
    sf::RectangleShape mRectangle;
    sf::RectangleShape mLine;
    sf::RectangleShape mLine2;


public:
    Elevator() {
        //rectangle
        sf::Vector2f sizeOfRectangle(100.0f, 100.0f);
        mRectangle.setPosition(300.0f, 100.0f);
        mRectangle.setOutlineColor(sf::Color::Black);
        mRectangle.setSize(sizeOfRectangle);
        mRectangle.setFillColor(sf::Color::White);
        mRectangle.setOutlineThickness(10);
        //lines next to elevator(walls)
        sf::Vector2f sizeOfLine(400.0f, 0.0f);
        mLine.setSize(sizeOfLine);
        mLine.setPosition(450.0f, 100.0f);
        mLine.setOutlineColor(sf::Color::Black);
        mLine.setFillColor(sf::Color::Black);
        mLine.setOutlineThickness(10);
        mLine.rotate(90);

        mLine2.setSize(sizeOfLine);
        mLine2.setPosition(250.0f, 100.0f);
        mLine2.setOutlineColor(sf::Color::Black);
        mLine2.setFillColor(sf::Color::Black);
        mLine2.setOutlineThickness(10);
        mLine2.rotate(90);
        
        
    }
    sf::RectangleShape get_rectangle() {
        return mRectangle;
        

    }
    sf::RectangleShape get_line() {
        return mLine;
    }
    sf::RectangleShape get_line2() {
        return mLine2;
    }
    
};

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
    Elevator main_elevator;
    


    
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
        window.draw(main_elevator.get_rectangle());
        window.draw(main_elevator.get_line());
        window.draw(main_elevator.get_line2());

        window.display();
    }
    return 0;
}