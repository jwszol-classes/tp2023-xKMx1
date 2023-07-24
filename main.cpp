#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

const sf::Vector2f firstFloorCoordinates = sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 70);          // coordinates relative to the middle of 300x170 elevator
const sf::Vector2f secondFloorCoordinates = sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT - (3 * 70));
const sf::Vector2f thirdFloorCoordinates = sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT - (5 * 70));
const sf::Vector2f fourthFloorCoordinates = sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT - (7 * 70));
const sf::Vector2f fifthFloorCoordinates = sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT - (9 * 70));

class Elevator {
    private:
        int m_totalPassengerMass;
        int m_currentLevel;
        sf::Vector2f m_position;
        sf::RectangleShape m_rectangle;
        sf::RectangleShape m_line;
        sf::RectangleShape m_line2;
        sf::Vector2f m_sizeOfRectangle = sf::Vector2f(300.0f, 140.0f);
        sf::Vector2f m_m_sizeOfLine = sf::Vector2f(2000.0f, 5.0f);

    public:
        Elevator() {
            //rectangle(elevator)
            m_rectangle.setOrigin(m_sizeOfRectangle.x/2, m_sizeOfRectangle.y/2);
            m_position = sf::Vector2f(fifthFloorCoordinates);
            m_rectangle.setPosition(m_position);
            m_rectangle.setOutlineColor(sf::Color::Black);
            m_rectangle.setSize(m_sizeOfRectangle);
            m_rectangle.setFillColor(sf::Color::White);
            m_rectangle.setOutlineThickness(4.3f);

            //lines next to elevator(walls)
            sf::Vector2f m_sizeOfLine(2000.0f, 5.0f);
            m_line.setSize(m_sizeOfLine);
            m_line.setOrigin(m_sizeOfLine.x/2, m_sizeOfLine.y/2);
            m_line.setPosition(SCREEN_WIDTH/2 + m_sizeOfRectangle.x/2, 100.0f);
            m_line.setOutlineColor(sf::Color::Black);
            m_line.setFillColor(sf::Color::Black);
            m_line.setOutlineThickness(2);
            m_line.rotate(90);

            m_line2.setSize(m_sizeOfLine);
            m_line2.setOrigin(m_sizeOfLine.x/2, m_sizeOfLine.y/2);
            m_line2.setPosition(SCREEN_WIDTH/2 - m_sizeOfRectangle.x/2, 100.0f);
            m_line2.setOutlineColor(sf::Color::Black);
            m_line2.setFillColor(sf::Color::Black);
            m_line2.setOutlineThickness(2);
            m_line2.rotate(90);

            m_currentLevel = 0;
        }

        sf::RectangleShape get_rectangle() {
            return m_rectangle;
        }

        sf::RectangleShape get_line() {
            return m_line;
        }

        sf::RectangleShape get_line2() {
            return m_line2;
        }

        int getCurrentLevel(){
            return m_currentLevel;
        }

        void checkCurrentLevel(){
            if(m_position.y < SCREEN_HEIGHT                            &&  m_position.y > SCREEN_HEIGHT - m_sizeOfRectangle.y)      m_currentLevel = 0;
            if(m_position.y < SCREEN_HEIGHT - m_sizeOfRectangle.y      &&  m_position.y > SCREEN_HEIGHT - 2 * m_sizeOfRectangle.y)  m_currentLevel = 1;
            if(m_position.y < SCREEN_HEIGHT - 2 * m_sizeOfRectangle.y  &&  m_position.y > SCREEN_HEIGHT - 3 * m_sizeOfRectangle.y)  m_currentLevel = 2;
            if(m_position.y < SCREEN_HEIGHT - 3 * m_sizeOfRectangle.y  &&  m_position.y > SCREEN_HEIGHT - 4 * m_sizeOfRectangle.y)  m_currentLevel = 3;
            if(m_position.y < SCREEN_HEIGHT - 4 * m_sizeOfRectangle.y  &&  m_position.y > SCREEN_HEIGHT - 5 * m_sizeOfRectangle.y)  m_currentLevel = 4;
        }
        
        void moveElevator(int floor){
            this->checkCurrentLevel();

            int increment;
            if(floor > m_currentLevel) increment = -1;
            else increment = 1;

            switch(floor){
                case 0:
                    if(!(m_position.y == firstFloorCoordinates.y)){
                        m_position.y += increment;
                        m_rectangle.setPosition(m_position);
                        break;
                    }
                    else break;
                case 1:
                    if(!(m_position.y == secondFloorCoordinates.y)){
                        m_position.y += increment;
                        m_rectangle.setPosition(m_position);
                        break;
                    }
                    else break;
                case 2:
                    if(!(m_position.y == thirdFloorCoordinates.y)){
                        m_position.y += increment;
                        m_rectangle.setPosition(m_position);
                        break;
                    }
                    else break;
                case 3:
                    if(!(m_position.y == fourthFloorCoordinates.y)){
                        m_position.y += increment;
                        m_rectangle.setPosition(m_position);
                        break;
                    }
                    else break;
                case 4:
                    if(!(m_position.y == fifthFloorCoordinates.y)){
                        m_position.y += increment;
                        m_rectangle.setPosition(m_position);
                        break;
                    }
                    else break;
                default:
                    break;
            }
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
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Elevator Simulation");

    sf::Texture texture;
    if (!texture.loadFromFile("img/person_sprite.png"))
    {
        std::cout << "Error " << std::endl;
    }
    texture.setSmooth(true);

    Elevator main_elevator;
    
    window.setFramerateLimit(120);
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

        main_elevator.moveElevator(0);

        window.clear(sf::Color(255, 255, 255));
        window.draw(main_elevator.get_rectangle());
        window.draw(main_elevator.get_line());
        window.draw(main_elevator.get_line2());

        window.display();
    }
    return 0;
}