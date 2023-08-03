#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <windows.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

const sf::Vector2i firstFloorCoordinates = sf::Vector2i(SCREEN_WIDTH / 2,
                                                        SCREEN_HEIGHT - 70); // coordinates relative to the middle of 300x170 elevator
const sf::Vector2i secondFloorCoordinates = sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT - (3 * 70));
const sf::Vector2i thirdFloorCoordinates = sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT - (5 * 70));
const sf::Vector2i fourthFloorCoordinates = sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT - (7 * 70));
const sf::Vector2i fifthFloorCoordinates = sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT - (9 * 70));

class Button {
private:
    unsigned int m_value;
    sf::RectangleShape m_shape;
    sf::Font *m_font;
    sf::Text m_text;
    sf::Vector2f m_textPositionVector;

    sf::Color m_idleColor = sf::Color::Magenta;
    sf::Color m_pushedColor = sf::Color::Cyan;

public:
    Button(float x, float y, float width, float height, sf::Font *font, int value = 0) {
        this->m_value = value;

        this->m_shape.setSize(sf::Vector2f(width, height));
        this->m_shape.setPosition(sf::Vector2f(x, y));
        this->m_shape.setFillColor(m_idleColor);

        this->m_text.setFont(*font);
        this->m_text.setString(std::to_string(m_value));
        this->m_text.setFillColor(sf::Color::Black);
        this->m_text.setCharacterSize(25);

        m_textPositionVector = sf::Vector2f((m_shape.getGlobalBounds().getPosition().x) + (m_shape.getGlobalBounds().getSize().x / 2.f) -
                                            (m_text.getGlobalBounds().getSize().x / 2.f),
                                            (m_shape.getGlobalBounds().getPosition().y) + (m_shape.getGlobalBounds().getSize().y / 2.f) -
                                            (m_text.getGlobalBounds().getSize().y));

        this->m_text.setPosition(m_textPositionVector);
    }

    void render(sf::RenderTarget *target) {
        target->draw(this->m_shape);
        target->draw(this->m_text);
    }

    unsigned int getValue() const { return m_value; }

    bool clicked(bool evnt, sf::Vector2i mousePosition) {
        if (static_cast<float>(mousePosition.x) <= this->m_shape.getPosition().x + m_shape.getSize().x &&
            static_cast<float>(mousePosition.x) >= this->m_shape.getPosition().x &&
            static_cast<float>(mousePosition.y) <= this->m_shape.getPosition().y + m_shape.getSize().y &&
            static_cast<float>(mousePosition.y) >= this->m_shape.getPosition().y &&
            evnt) {
            return true;
        }
        return false;
    }
};

class Passenger {
private:
    int m_startLevel;
    int m_endLevel;
    int m_mass = 70;
    sf::Vector2f m_position;
    sf::Sprite m_sprite;

public:
    Passenger(const sf::Texture *texture, int startLevel, int endLevel) {
        m_startLevel = 4 - startLevel;                                         // TODO work out this to not hard code
        m_endLevel = endLevel;

        m_sprite.setTexture(*texture);
        m_sprite.setScale(0.25f, 0.25f);

        switch (startLevel) {
            case 4:                                                                //TODO change this order
                this->m_sprite.setPosition(sf::Vector2f(300.f, static_cast<float>(firstFloorCoordinates.y - 10)));
                break;
            case 3:
                this->m_sprite.setPosition(sf::Vector2f(900.f, static_cast<float>(secondFloorCoordinates.y - 10)));
                break;
            case 2:
                this->m_sprite.setPosition(sf::Vector2f(300.f, static_cast<float>(thirdFloorCoordinates.y - 10)));
                break;
            case 1:
                this->m_sprite.setPosition(sf::Vector2f(900.f, static_cast<float>(fourthFloorCoordinates.y - 10)));
                break;
            case 0:
                this->m_sprite.setPosition(sf::Vector2f(300.f, static_cast<float>(fifthFloorCoordinates.y - 10)));
                break;
            default:
                break;
        }
    }

    int getStartLevel() const { return m_startLevel; }

    int getEndLevel() const { return m_endLevel; }

    sf::Sprite getSprite() const { return m_sprite; }

    void move() {
        m_position.x += 5;
        m_sprite.setPosition(m_position);
    }
};

class Floor {
private:
    std::vector<Passenger> m_queue;
    std::vector<Button> m_otherFloorsButtons;
    sf::RectangleShape m_shape;
    int m_id;

public:
    Floor(int id, sf::Font *font) {
        m_id = id;

        for (int i = 0; i < 5; i++) {
            if (m_id == i) {
                continue;
            }
            Button button((100.f + (m_id % 2) * 900.f + i * 40), 50.f + static_cast<int>(m_id) * 125.f, 30.f, 30.f, font, i);
            m_otherFloorsButtons.push_back(button);
        }

        this->m_shape.setSize(sf::Vector2f(485.5f, 6.f));
        this->m_shape.setFillColor(sf::Color::Green);

        switch (m_id) {
            case 0:
                this->m_shape.setPosition(
                        sf::Vector2f(((m_id % 2) * 794.f), firstFloorCoordinates.y + 70));
                break;
            case 1:
                this->m_shape.setPosition(
                        sf::Vector2f(((m_id % 2) * 794.f), secondFloorCoordinates.y + 70));
                break;
            case 2:
                this->m_shape.setPosition(
                        sf::Vector2f(((m_id % 2) * 794.f), thirdFloorCoordinates.y + 70));
                break;
            case 3:
                this->m_shape.setPosition(
                        sf::Vector2f(((m_id % 2) * 794.f), fourthFloorCoordinates.y + 70));
                break;
            case 4:
                this->m_shape.setPosition(
                        sf::Vector2f(((m_id % 2) * 794.f), fifthFloorCoordinates.y + 70));
                break;
        }
    }

    int getID() const { return m_id; }

    void listenForButtons(bool evnt, sf::Vector2i mousePos, const sf::Texture *texture) {
        for (int i = 0; i < 4; i++) {
            if (m_otherFloorsButtons[i].clicked(evnt, mousePos)) {
                Passenger newPassenger(texture, m_id, m_otherFloorsButtons[i].getValue());
                std::cout << newPassenger.getStartLevel() << " | " << newPassenger.getEndLevel() << '\n';
                m_queue.push_back(newPassenger);
            }
        }
    }

    void render(sf::RenderTarget *target) {
        target->draw(this->m_shape);
        for (int i = 0; i < 4; i++) {
            m_otherFloorsButtons[i].render(target);
        }
        for (const auto &i: m_queue) {
            target->draw(i.getSprite());
        }
    }
};

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
    std::vector<int> order;
    int m_direction;

public:
    Elevator() {
        // rectangle(elevator)
        m_rectangle.setOrigin(m_sizeOfRectangle.x / 2, m_sizeOfRectangle.y / 2);
        m_position = sf::Vector2f(fifthFloorCoordinates);
        m_rectangle.setPosition(m_position);
        m_rectangle.setOutlineColor(sf::Color::Black);
        m_rectangle.setSize(m_sizeOfRectangle);
        m_rectangle.setFillColor(sf::Color::White);
        m_rectangle.setOutlineThickness(4.3f);

        // lines next to elevator(walls)
        sf::Vector2f m_sizeOfLine(2000.0f, 5.0f);
        m_line.setSize(m_sizeOfLine);
        m_line.setOrigin(m_sizeOfLine.x / 2, m_sizeOfLine.y / 2);
        m_line.setPosition(SCREEN_WIDTH / 2 + m_sizeOfRectangle.x / 2, 100.0f);
        m_line.setOutlineColor(sf::Color::Black);
        m_line.setFillColor(sf::Color::Black);
        m_line.setOutlineThickness(2);
        m_line.rotate(90);

        m_line2.setSize(m_sizeOfLine);
        m_line2.setOrigin(m_sizeOfLine.x / 2, m_sizeOfLine.y / 2);
        m_line2.setPosition(SCREEN_WIDTH / 2 - m_sizeOfRectangle.x / 2, 100.0f);
        m_line2.setOutlineColor(sf::Color::Black);
        m_line2.setFillColor(sf::Color::Black);
        m_line2.setOutlineThickness(2);
        m_line2.rotate(90);

        m_currentLevel = 0;
        m_direction = 1; // 1 for up, -1 for down
    }

    sf::RectangleShape get_rectangle() { return m_rectangle; }

    sf::RectangleShape get_line() { return m_line; }

    sf::RectangleShape get_line2() { return m_line2; }

    int getCurrentLevel() const { return m_currentLevel; }

    void checkCurrentLevel() {
        if (m_position.y < firstFloorCoordinates.y &&
            m_position.y > secondFloorCoordinates.y)
            m_currentLevel = 0;
        if (m_position.y < secondFloorCoordinates.y &&
            m_position.y > thirdFloorCoordinates.y)
            m_currentLevel = 1;
        if (m_position.y < thirdFloorCoordinates.y &&
            m_position.y > fourthFloorCoordinates.y)
            m_currentLevel = 2;
        if (m_position.y < fourthFloorCoordinates.y &&
            m_position.y > fifthFloorCoordinates.y)
            m_currentLevel = 3;
        if (m_position.y < fifthFloorCoordinates.y && m_position.y > 0)
            m_currentLevel = 4;
    }

    void moveElevator(int floor) {
        this->checkCurrentLevel();

        int increment;
        if (floor > m_currentLevel)
            increment = -1;
        else
            increment = 1;

        switch (floor) {
            case 0:
                if (!(m_position.y == firstFloorCoordinates.y)) {
                    m_position.y += increment;
                    m_rectangle.setPosition(m_position);
                    break;
                } else
                    break;
            case 1:
                if (!(m_position.y == secondFloorCoordinates.y)) {
                    std::cout << m_position.y << " | " << firstFloorCoordinates.y << '\n';
                    m_position.y += increment;
                    m_rectangle.setPosition(m_position);
                    break;
                } else
                    break;
            case 2:
                if (!(m_position.y == thirdFloorCoordinates.y)) {
                    m_position.y += increment;
                    m_rectangle.setPosition(m_position);
                    break;
                } else
                    break;
            case 3:
                if (!(m_position.y == fourthFloorCoordinates.y)) {
                    m_position.y += increment;
                    m_rectangle.setPosition(m_position);
                    break;
                } else
                    break;
            case 4:
                if (!(m_position.y == fifthFloorCoordinates.y)) {
                    m_position.y += increment;
                    m_rectangle.setPosition(m_position);
                    break;
                } else
                    break;
            default:
                break;
        }
    }

    void elevatorLogic(Floor firstFloor, Floor secondFloor, Floor thirdFloor,
                       Floor fourthFloor, Floor fifthFloor) {}
};

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Elevator Simulation");

    std::vector<int> floorQueue;

    sf::Font font;
    if (!font.loadFromFile("font/Akira Expanded Demo.otf")) {
        std::cout << "Font error " << std::endl;
    }

    sf::Texture texture;
    if (!texture.loadFromFile("img/person_sprite.png")) {
        std::cout << "Texture error " << std::endl;
    }
    texture.setSmooth(true);

    Elevator main_elevator;

    std::vector<Floor> floors;
    for (int i = 0; i < 5; i++) {
        Floor floor(i, &font);
        floors.push_back(floor);
    }

    bool buttonSwitch = 0;
    window.setFramerateLimit(120);
    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        buttonSwitch = 1;
                    }
                    break;
            }
        }

        main_elevator.moveElevator(0);
        for (int i = 0; i < 5; i++) {
            floors[i].listenForButtons(buttonSwitch, sf::Mouse::getPosition(window), &texture);
        }

        window.clear(sf::Color(255, 255, 255));
        window.draw(main_elevator.get_rectangle());
        window.draw(main_elevator.get_line());
        window.draw(main_elevator.get_line2());
        for (int i = 0; i < 5; i++) {
            floors[i].render(&window);
        }

        buttonSwitch = 0;
        window.display();
    }
    return 0;
}