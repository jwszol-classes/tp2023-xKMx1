#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#define WIDTH 1280
#define HEIGHT 720

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
        sf::Vector2f sizeOfRectangle(300.0f, 200.0f);
        mRectangle.setOrigin(sizeOfRectangle.x/2, sizeOfRectangle.y/2);
        mRectangle.setPosition(WIDTH/2, HEIGHT/2);
        mRectangle.setOutlineColor(sf::Color::Black);
        mRectangle.setSize(sizeOfRectangle);
        mRectangle.setFillColor(sf::Color::White);
        mRectangle.setOutlineThickness(4.3f);
        
        //lines next to elevator(walls)
        sf::Vector2f sizeOfLine(2000.0f, 5.0f);
        mLine.setSize(sizeOfLine);
        mLine.setOrigin(sizeOfLine.x/2, sizeOfLine.y/2);
        mLine.setPosition(WIDTH/2 + sizeOfRectangle.x/2, 100.0f);
        mLine.setOutlineColor(sf::Color::Black);
        mLine.setFillColor(sf::Color::Black);
        mLine.setOutlineThickness(2);
        mLine.rotate(90);

        mLine2.setSize(sizeOfLine);
        mLine2.setOrigin(sizeOfLine.x/2, sizeOfLine.y/2);
        mLine2.setPosition(WIDTH/2 - sizeOfRectangle.x/2, 100.0f);
        mLine2.setOutlineColor(sf::Color::Black);
        mLine2.setFillColor(sf::Color::Black);
        mLine2.setOutlineThickness(2);
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
    sf::Vector2f m_position;
    sf::Sprite mSprite;
public:
    Passenger(const sf::Texture* texture) {
        m_position = sf::Vector2f(100.0f, 100.0f);
        mSprite.setTexture(*texture);
        mSprite.setScale(0.25f, 0.25f);
        mSprite.setPosition(m_position);
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

    void move(){
        m_position.x += 5;
        mSprite.setPosition(m_position);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Elevator Simulation");
    sf::Clock frameClock;

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