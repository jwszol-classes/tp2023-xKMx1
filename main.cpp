#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <windows.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

const sf::Vector2i zeroFloorCoordinates = sf::Vector2i(SCREEN_WIDTH / 2,
    SCREEN_HEIGHT - 70); // coordinates relative to the middle of 300x170 elevator
const sf::Vector2i firstFloorCoordinates = sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT - (3 * 70));
const sf::Vector2i secondFloorCoordinates = sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT - (5 * 70));
const sf::Vector2i thirdFloorCoordinates = sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT - (7 * 70));
const sf::Vector2i fourthFloorCoordinates = sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT - (9 * 70));

class Button {
private:
    int m_value;
    sf::RectangleShape m_shape;
    sf::Text m_text;
    sf::Vector2f m_textPositionVector;

    sf::Color m_idleColor = sf::Color::Magenta;
    sf::Color m_pushedColor = sf::Color::Cyan;

public:
    Button(int x, int y, int width, int height, sf::Font* font, int id, int value = 0) {
        this->m_value = value;

        this->m_shape.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
        this->m_shape.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
        this->m_shape.setFillColor(m_idleColor);
        if (id == 3) {
            this->m_shape.setFillColor(m_pushedColor);
        }

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

    void render(sf::RenderTarget* target) {
        target->draw(this->m_shape);
        target->draw(this->m_text);
    }

    int getValue() const { return m_value; }
};

class Passenger {
private:
    int m_startLevel;
    int m_endLevel;
    int m_orderNumber;
    int m_mass = 70;
    sf::Vector2f m_position;
    sf::Sprite m_sprite;

public:
    Passenger(const sf::Texture* texture, int startLevel, int endLevel, int orderNumber) {
        m_startLevel = 4 - startLevel;                                         // TODO work out this to not hard code
        m_endLevel = endLevel;
        m_orderNumber = orderNumber;

        m_sprite.setTexture(*texture);
        m_sprite.setScale(0.25f, 0.25f);

        switch (startLevel) {
        case 0:
            this->m_sprite.setPosition(
                sf::Vector2f((400.f - 40.f * static_cast<float>(orderNumber)), static_cast<float>(zeroFloorCoordinates.y - 10)));
            break;
        case 1:
            this->m_sprite.setPosition(
                sf::Vector2f((800.f + 40.f * static_cast<float>(orderNumber)), static_cast<float>(firstFloorCoordinates.y - 10)));
            break;
        case 2:
            this->m_sprite.setPosition(
                sf::Vector2f((400.f - 40.f * static_cast<float>(orderNumber)), static_cast<float>(secondFloorCoordinates.y - 10)));
            break;
        case 3:
            this->m_sprite.setPosition(
                sf::Vector2f((800.f + 40.f * static_cast<float>(orderNumber)), static_cast<float>(thirdFloorCoordinates.y - 10)));
            break;
        case 4:
            this->m_sprite.setPosition(
                sf::Vector2f((400.f - 40.f * static_cast<float>(orderNumber)), static_cast<float>(fourthFloorCoordinates.y - 10)));
            break;
        default:
            break;
        }
    }

    void setPos(sf::Vector2f pos) {
        this->m_position = pos;

        m_sprite.setPosition(m_position);
    }

    void move(int direction) {
        switch (direction) {
        case 1:
            this->m_position.x += 1;
            break;
        case -1:
            this->m_position.x -= 1;
            break;
        default:
            break;
        }
        m_sprite.setPosition(m_position);
    }

    void render(sf::RenderTarget* target) {
        target->draw(this->getSprite());
    }

    sf::Sprite getSprite() const { return m_sprite; }

    int getStartLevel() const { return m_startLevel; }

    int getEndLevel() const { return m_endLevel; }

    int getOrderNumber() const { return m_orderNumber; }

    int getMass() const { return m_mass; }

    sf::Vector2f getPosition() {
        return m_position;
    }
};

class Floor {
private:
    std::vector<Passenger> m_queue;
    std::vector<Button> m_otherFloorsButtons;
    sf::RectangleShape m_shape;
    int m_id;

public:
    Floor(int id, sf::Font* font) {
        m_id = id;

        for (int i = 0; i < 5; i++) {
            if (m_id == i) {
                continue;
            }
            Button button((100 + (m_id % 2) * 900 + i * 40), SCREEN_HEIGHT - m_id * 140 - 90, 30, 30, font, m_id, i);
            m_otherFloorsButtons.push_back(button);
        }

        this->m_shape.setSize(sf::Vector2f(485.5f, 6.f));
        this->m_shape.setFillColor(sf::Color::Green);

        switch (m_id) {
        case 0:
            this->m_shape.setPosition(
                sf::Vector2f((static_cast<float>(m_id % 2) * 794.f), static_cast<float>(zeroFloorCoordinates.y) + 70));
            break;
        case 1:
            this->m_shape.setPosition(
                sf::Vector2f((static_cast<float>(m_id % 2) * 794.f), static_cast<float>(firstFloorCoordinates.y) + 70));
            break;
        case 2:
            this->m_shape.setPosition(
                sf::Vector2f((static_cast<float>(m_id % 2) * 794.f), static_cast<float>(secondFloorCoordinates.y) + 70));
            break;
        case 3:
            this->m_shape.setPosition(
                sf::Vector2f((static_cast<float>(m_id % 2) * 794.f), static_cast<float>(thirdFloorCoordinates.y) + 70));
            this->m_shape.setFillColor(sf::Color::Black);
            break;
        case 4:
            this->m_shape.setPosition(
                sf::Vector2f((static_cast<float>(m_id % 2) * 794.f), static_cast<float>(fourthFloorCoordinates.y) + 70));
            break;
        }
    }

    void listenForButtons(bool evnt, sf::Vector2i mousePos, const sf::Texture* texture) {
        for (int i = 0; i < 4; i++) {
            if (m_otherFloorsButtons[i].clicked(evnt, mousePos)) {
                std::cout << "ID " << m_id << "\n";
                Passenger newPassenger(texture, m_id, m_otherFloorsButtons[i].getValue(), static_cast<int>(m_queue.size()));
                m_queue.push_back(newPassenger);
            }
        }
    }

    Passenger sendPassengerToElevator(sf::RenderTarget* target) {
        Passenger passenger(m_queue.back().getSprite().getTexture(), m_queue.back().getStartLevel(), m_queue.back().getEndLevel(),
            m_queue.back().getOrderNumber());
        passenger.setPos(m_queue.back().getSprite().getPosition());
        target->draw(passenger.getSprite());
        m_queue.pop_back();

        return passenger;
    }

    [[maybe_unused]] void getRidOfPassenger(const Passenger& passenger) {
    }

    void render(sf::RenderTarget* target) {
        target->draw(this->m_shape);
        for (int i = 0; i < 4; i++) {
            m_otherFloorsButtons[i].render(target);
        }
        for (const auto& i : m_queue) {
            target->draw(i.getSprite());
        }
    }

    int getFloorValue() const { return m_id; }

    bool isFloorEmpty() {
        if (this->m_queue.empty()) return true;
        else return false;
    }

    sf::RectangleShape getShape() {
        return m_shape;
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
    sf::Vector2f m_sizeOfLine = sf::Vector2f(2000.0f, 5.0f);
    std::vector<int> m_order;
    std::vector<Passenger> m_passengers_list;
    int m_direction;

public:
    Elevator() {
        // rectangle(elevator)
        m_rectangle.setOrigin(m_sizeOfRectangle.x / 2, m_sizeOfRectangle.y / 2);
        m_position = sf::Vector2f(fourthFloorCoordinates);
        m_rectangle.setPosition(m_position);
        m_rectangle.setOutlineColor(sf::Color::Black);
        m_rectangle.setSize(m_sizeOfRectangle);
        m_rectangle.setFillColor(sf::Color::White);
        m_rectangle.setOutlineThickness(4.3f);

        // lines next to elevator(walls)
        m_line.setSize(m_sizeOfLine);
        m_line.setOrigin(m_sizeOfLine.x / 2, m_sizeOfLine.y / 2);
        m_line.setPosition(static_cast<float>(SCREEN_WIDTH) / 2 + m_sizeOfRectangle.x / 2, 100.0f);
        m_line.setOutlineColor(sf::Color::Black);
        m_line.setFillColor(sf::Color::Black);
        m_line.setOutlineThickness(2);
        m_line.rotate(90);

        m_line2.setSize(m_sizeOfLine);
        m_line2.setOrigin(m_sizeOfLine.x / 2, m_sizeOfLine.y / 2);
        m_line2.setPosition(static_cast<float>(SCREEN_WIDTH) / 2 - m_sizeOfRectangle.x / 2, 100.0f);
        m_line2.setOutlineColor(sf::Color::Black);
        m_line2.setFillColor(sf::Color::Black);
        m_line2.setOutlineThickness(2);
        m_line2.rotate(90);

        m_currentLevel = 0;
        m_totalPassengerMass = 0;
        m_direction = 1; // 1 for up, -1 for down
    }

    void checkCurrentLevel() {
        if (m_position.y < static_cast<float>(zeroFloorCoordinates.y) + 2.f &&
            m_position.y > static_cast<float>(zeroFloorCoordinates.y) - 2.f)
            m_currentLevel = 0;
        else if (m_position.y <= static_cast<float>(firstFloorCoordinates.y) + 2.f &&
            m_position.y > static_cast<float>(firstFloorCoordinates.y) - 2.f)
            m_currentLevel = 1;
        else if (m_position.y <= static_cast<float>(secondFloorCoordinates.y) + 2.f &&
            m_position.y > static_cast<float>(secondFloorCoordinates.y) - 2.f)
            m_currentLevel = 2;
        else if (m_position.y <= static_cast<float>(thirdFloorCoordinates.y) + 2.f &&
            m_position.y > static_cast<float>(thirdFloorCoordinates.y) - 2.f)
            m_currentLevel = 3;
        else if (m_position.y <= static_cast<float>(fourthFloorCoordinates.y) + 2.f &&
            m_position.y > static_cast<float>(fourthFloorCoordinates.y) - 2.f)
            m_currentLevel = 4;
    }

    void moveElevator(int floor) {
        checkCurrentLevel();

        float increment;
        if (floor > m_currentLevel)
            increment = -1.f;
        else
            increment = 1.f;

        switch (floor) {
        case 0:
            if (m_position.y != static_cast<float>(zeroFloorCoordinates.y)) {
                m_position.y += increment;
                m_rectangle.setPosition(m_position);
                break;
            }
            else
                break;
        case 1:
            if (m_position.y != static_cast<float>(firstFloorCoordinates.y)) {
                m_position.y += increment;
                m_rectangle.setPosition(m_position);
                break;
            }
            else
                break;
        case 2:
            if (m_position.y != static_cast<float>(secondFloorCoordinates.y)) {
                m_position.y += increment;
                m_rectangle.setPosition(m_position);
                break;
            }
            else
                break;
        case 3:
            if (m_position.y != static_cast<float>(thirdFloorCoordinates.y)) {
                m_position.y += increment;
                m_rectangle.setPosition(m_position);
                break;
            }
            else
                break;
        case 4:
            if (m_position.y != static_cast<float>(fourthFloorCoordinates.y)) {
                m_position.y += increment;
                m_rectangle.setPosition(m_position);
                break;
            }
            else
                break;
        default:
            break;
        }
    }

    void runElevator(std::vector<int>& queue) {
        if (!m_passengers_list.empty()) {
            if (m_passengers_list.back().getPosition().x != 600) {
                std::cout << m_passengers_list.back().getPosition().x << '\n';
                if (m_passengers_list.back().getStartLevel() % 2 == 1) {
                    m_passengers_list.back().move(-1);
                }
                else {
                    m_passengers_list.back().move(1);
                }
            }
        }

        if (this->m_currentLevel != queue.back())
            this->moveElevator(queue.back());
        else {
            Sleep(100);
            queue.pop_back();
        }
    }

    void acceptPassenger(const Passenger& passenger) {
        m_passengers_list.push_back(passenger);
        m_totalPassengerMass = passenger.getMass();
    }

    Passenger deliverPassneger() {
        Passenger passenger = m_passengers_list.back();
        m_passengers_list.pop_back();

        return passenger;
    }

    sf::RectangleShape get_rectangle() { return m_rectangle; }

    sf::RectangleShape get_line() { return m_line; }

    sf::RectangleShape get_line2() { return m_line2; }

    int getCurrentLevel() const { return m_currentLevel; }

    void render(sf::RenderTarget* target) {
        target->draw(this->m_rectangle);
        for (const auto& i : m_passengers_list) {
            target->draw(i.getSprite());
        }
    }

    //    void elevatorLogic(Floor firstFloor, Floor secondFloor, Floor thirdFloor,
    //                       Floor fourthFloor, Floor fifthFloor) {}
};

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Elevator Simulation");

    std::vector<int> floorQueue;
    floorQueue.push_back(3);

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

    bool buttonSwitch = false;
    bool passengerMoving = false;

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
                    buttonSwitch = true;
                }
                break;
            }
        }

        main_elevator.runElevator(floorQueue);


        for (int i = 0; i < 5; i++) {
            floors[i].listenForButtons(buttonSwitch, sf::Mouse::getPosition(window), &texture);
            //            std::cout << main_elevator.getCurrentLevel() << " | " << floors[i].getFloorValue() << " | "
            //                      << floors[i].getShape().getPosition().y << " | "
            //                      << !floors[i].isFloorEmpty()
            //                      << "\n";
            if (main_elevator.getCurrentLevel() == floors[i].getFloorValue() && !floors[i].isFloorEmpty()) {
                std::cout << "A\n";
                main_elevator.acceptPassenger(floors[i].sendPassengerToElevator(&window));
            }
        }


        window.clear(sf::Color(255, 255, 255));
        //        window.draw(main_elevator.get_rectangle());
        main_elevator.render(&window);
        window.draw(main_elevator.get_line());
        window.draw(main_elevator.get_line2());
        for (int i = 0; i < 5; i++) {
            floors[i].render(&window);
        }

        buttonSwitch = false;
        window.display();
    }
    return 0;
}