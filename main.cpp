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

struct Seat {
    float position;
    bool taken = false;
};

struct passengerRoute : public _GUID {
    int startLevel;
    int endLevel;
};

class Button {
private:
    int m_value{};
    sf::RectangleShape m_shape{};
    sf::Text m_text;
    sf::Vector2f m_textPositionVector;

    sf::Color m_idleColor = sf::Color::Magenta;
    sf::Color m_pushedColor = sf::Color::Cyan;

public:
    Button(int x, int y, int width, int height, sf::Font *font, int id, int value = 0) {
        m_value = value;

        m_shape.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
        m_shape.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
        m_shape.setFillColor(m_idleColor);

        m_text.setFont(*font);
        m_text.setString(std::to_string(m_value));
        m_text.setFillColor(sf::Color::Black);
        m_text.setCharacterSize(25);

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

    void render(sf::RenderTarget *target) {
        target->draw(this->m_shape);
        target->draw(this->m_text);
    }

    int getValue() const { return m_value; }
};

class Passenger {
private:
    passengerRoute m_route{};
    int m_orderNumber;
    int m_mass = 70;
    sf::Vector2f m_position;
    sf::Sprite m_sprite;
    bool sitting = false;
    int m_seat = 1000;

public:
    Passenger(const sf::Texture *texture, int startLevel, int endLevel, int orderNumber) {
        m_route.startLevel = startLevel;
        m_route.endLevel = endLevel;

        m_orderNumber = orderNumber;

        m_sprite.setTexture(*texture);
        m_sprite.setScale(0.25f, 0.25f);

        switch (startLevel) {
            case 0:
                m_sprite.setPosition(
                        sf::Vector2f((400.f - 40.f * static_cast<float>(orderNumber)), static_cast<float>(zeroFloorCoordinates.y - 10)));
                break;
            case 1:
                m_sprite.setPosition(
                        sf::Vector2f((800.f + 40.f * static_cast<float>(orderNumber)), static_cast<float>(firstFloorCoordinates.y - 10)));
                break;
            case 2:
                m_sprite.setPosition(
                        sf::Vector2f((400.f - 40.f * static_cast<float>(orderNumber)), static_cast<float>(secondFloorCoordinates.y - 10)));
                break;
            case 3:
                m_sprite.setPosition(
                        sf::Vector2f((800.f + 40.f * static_cast<float>(orderNumber)), static_cast<float>(thirdFloorCoordinates.y - 10)));
                break;
            case 4:
                m_sprite.setPosition(
                        sf::Vector2f((400.f - 40.f * static_cast<float>(orderNumber)), static_cast<float>(fourthFloorCoordinates.y - 10)));
                break;
            default:
                break;
        }
    }

    bool isSitting() const {
        return sitting;
    }

    void setSitting(int x) {
        if (x == 1000) {
            sitting = false;
        } else {
            sitting = true;
        }
        m_seat = x;
    }

    int getSeat() const {
        return m_seat;
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

    void render(sf::RenderTarget *target) const {
        target->draw(this->getSprite());
    }

    passengerRoute getRoute() {
        return m_route;
    }

    sf::Sprite getSprite() const { return m_sprite; }

    int getStartLevel() const { return m_route.startLevel; }

    int getEndLevel() const { return m_route.endLevel; }

    int getOrderNumber() const { return m_orderNumber; }

    int getMass() const { return m_mass; }

    sf::Vector2f getPosition() {
        return m_position;
    }
};

class Floor {
private:
    std::vector<Passenger> m_queue;
    std::vector<Passenger> m_trash;
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
            Button button((100 + (m_id % 2) * 900 + i * 40), SCREEN_HEIGHT - m_id * 140 - 100, 30, 30, font, m_id, i);
            m_otherFloorsButtons.push_back(button);
        }

        m_shape.setSize(sf::Vector2f(485.5f, 6.f));
        m_shape.setFillColor(sf::Color::Black);

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
                break;
            case 4:
                this->m_shape.setPosition(
                        sf::Vector2f((static_cast<float>(m_id % 2) * 794.f), static_cast<float>(fourthFloorCoordinates.y) + 70));
                break;
        }
    }

    void listenForButtons(bool evnt, sf::Vector2i mousePos, const sf::Texture *texture) {
        for (int i = 0; i < 4; i++) {
            if (m_otherFloorsButtons[i].clicked(evnt, mousePos)) {
                Passenger newPassenger(texture, m_id, m_otherFloorsButtons[i].getValue(), static_cast<int>(m_queue.size()));
                m_queue.push_back(newPassenger);
            }
        }
    }

    Passenger sendPassengerToElevator(sf::RenderTarget *target) {
        Passenger passenger(m_queue.front().getSprite().getTexture(), m_queue.front().getStartLevel(), m_queue.front().getEndLevel(),
                            m_queue.front().getOrderNumber());
        passenger.setPos(m_queue.front().getSprite().getPosition());
        target->draw(passenger.getSprite());
        m_queue.erase(m_queue.begin());                                     //TODO move passenger to front of the queue
        //TODO check sending by reference
        return passenger;
    }

    void acceptPassenger(const Passenger &passenger) {
        m_trash.push_back(passenger);
    }

    void getRidOfPassenger(bool &freeze) {
        int count = 0;
        for (Passenger &i: m_trash) {
            if (i.getEndLevel() % 2 == 0) {
                i.move(-1);
            } else {
                i.move(1);
            }

            if (i.getPosition().x > SCREEN_WIDTH || i.getPosition().x < -70) {
//                m_trash.pop_back();
                m_trash.erase(m_trash.begin() + count);
            }

            if (i.getPosition().x >= 440 && i.getPosition().x <= 790) {
                freeze = true;
            } else {
                freeze = false;
            }
            count++;
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
        for (const auto &i: m_trash) {
            target->draw(i.getSprite());
        }
    }

    void returnPassengers(std::vector<passengerRoute> &gameQueue) {
        for (auto &i: m_queue) {
            if (gameQueue.empty()) {
                gameQueue.push_back(i.getRoute());
            }
            bool flag = true;
            for (auto &j: gameQueue) {
                if (j.startLevel == i.getStartLevel() && j.endLevel == i.getEndLevel()) {
                    flag = false;
                }
            }
            if (flag) {
                gameQueue.push_back(i.getRoute());
            }
        }
    }

    int getFloorValue() const { return m_id; }

    bool isFloorEmpty() {
        if (m_queue.empty()) return true;
        else return false;
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
    bool m_isFrozen;
    int m_direction;                            // 0 for down, 1 for up
    Seat Seats[8] = {490, false, 523, false, 556, false, 589, false, 621, false, 654, false, 687, false, 720, false};

public:
    Elevator() {
        // rectangle(elevator)
        m_rectangle.setOrigin(m_sizeOfRectangle.x / 2, m_sizeOfRectangle.y / 2);
        m_position = sf::Vector2f(zeroFloorCoordinates);
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
        m_isFrozen = false;
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

    void moveElevator(int floor) {                 //TODO cos sie nie popuje po dotarciu na pietro
        checkCurrentLevel();

        float increment;
        if (floor > m_currentLevel && !m_isFrozen) {
            m_direction = 1;
            increment = -1.f;
        } else if (floor < m_currentLevel && !m_isFrozen) {
            m_direction = 0;
            increment = 1.f;
        } else
            increment = 0.f;

        switch (floor) {
            case 0:
                if (m_position.y != static_cast<float>(zeroFloorCoordinates.y)) {
                    m_position.y += increment;
                    m_rectangle.setPosition(m_position);
                    break;
                } else
                    break;
            case 1:
                if (m_position.y != static_cast<float>(firstFloorCoordinates.y)) {
                    m_position.y += increment;
                    m_rectangle.setPosition(m_position);
                    break;
                } else
                    break;
            case 2:
                if (m_position.y != static_cast<float>(secondFloorCoordinates.y)) {
                    m_position.y += increment;
                    m_rectangle.setPosition(m_position);
                    break;
                } else
                    break;
            case 3:
                if (m_position.y != static_cast<float>(thirdFloorCoordinates.y)) {
                    m_position.y += increment;
                    m_rectangle.setPosition(m_position);
                    break;
                } else
                    break;
            case 4:
                if (m_position.y != static_cast<float>(fourthFloorCoordinates.y)) {
                    m_position.y += increment;
                    m_rectangle.setPosition(m_position);
                    break;
                } else
                    break;
            default:
                break;
        }

        for (auto &i: m_passengers_list) {
            i.setPos(sf::Vector2f(i.getPosition().x, m_position.y - 10));
        }
    }

    void takeSeat(Passenger &passenger) {
        if (!passenger.isSitting()) {
            bool flag = false;
            for (int j = 0; j < 8; j++) {
                if ((passenger.getPosition().x == Seats[j].position) && !Seats[j].taken) {
                    Seats[j].taken = true;
                    passenger.setSitting(j);
                    flag = false;
                    break;
                } else {
                    flag = true;
                }
            }
            if (flag) {
                m_isFrozen = true;
                if (passenger.getStartLevel() % 2 == 1) {
                    passenger.move(-1);
                } else {
                    passenger.move(1);
                }
            } else {
                m_isFrozen = false;
            }
        }
    }

    void runElevator(std::vector<int> &queue) {
        for (auto &i: m_passengers_list) {
            takeSeat(i);
        }
        if (!m_passengers_list.empty()) {
            if (m_currentLevel != queue.front()) {
                moveElevator(queue.front());
            } else {
                queue.erase(queue.begin());
            }
        } else {
            if (!queue.empty()) {
                moveElevator(queue.front());
                if (m_currentLevel == queue.front()) {
                    queue.erase(queue.begin());
                }
            } else {
                moveElevator(0);
            }
        }
    }

    Passenger sendPassengerToFloor(sf::RenderTarget *target, std::vector<passengerRoute> &queue) {
        Passenger passenger(m_passengers_list.back().getSprite().getTexture(), m_passengers_list.back().getStartLevel(),
                            m_passengers_list.back().getEndLevel(),
                            m_passengers_list.back().getOrderNumber());
        passenger.setPos(m_passengers_list.back().getSprite().getPosition());
        target->draw(passenger.getSprite());
        m_passengers_list.pop_back();
        queue.erase(queue.begin());

        return passenger;
    }

    void acceptPassenger(const Passenger &passenger) {
        m_passengers_list.push_back(passenger);
        m_totalPassengerMass = passenger.getMass();
    }

    void deliverPassneger(std::vector<Floor> &floors, sf::RenderTarget *target, std::vector<passengerRoute> &queue) {
        for (Passenger &i: m_passengers_list) {
            if (i.getEndLevel() == m_currentLevel) {
                floors[m_currentLevel].acceptPassenger(this->sendPassengerToFloor(target, queue));
                Seats[i.getSeat()].taken = false;
                i.setSitting(1000);
            }
        }
    }

    sf::RectangleShape get_rectangle() { return m_rectangle; }

    sf::RectangleShape get_line() { return m_line; }

    sf::RectangleShape get_line2() { return m_line2; }

    int getCurrentLevel() const { return m_currentLevel; }

    bool &setElevatorFreeze() {
        return m_isFrozen;
    }

    void render(sf::RenderTarget *target) {
        target->draw(this->m_rectangle);
        for (const auto &i: m_passengers_list) {
            target->draw(i.getSprite());
        }
    }

    bool isElevatorFull() {
        if (m_passengers_list.size() == 8) return true;
        else return false;
    }

    void elevatorLogic(std::vector<int> &elevatorQueue, const std::vector<passengerRoute> &passengerQueue) const {
        for (auto &i: passengerQueue) {
            if (!((std::find(elevatorQueue.begin(), elevatorQueue.end(), i.startLevel) != elevatorQueue.end()) &&
                  (std::find(elevatorQueue.begin(), elevatorQueue.end(), i.endLevel) != elevatorQueue.end()))) {
                if (m_direction == 1) {
                    if (i.endLevel > m_currentLevel) {
                        elevatorQueue.insert(elevatorQueue.begin(), i.endLevel);
                    }
                    if (i.startLevel > m_currentLevel) {
                        elevatorQueue.insert(elevatorQueue.begin(), i.startLevel);
                    }
                }
                if (m_direction == 0) {
                    if (i.startLevel < m_currentLevel) {
                        elevatorQueue.insert(elevatorQueue.begin(), i.startLevel);
                    }
                    if (i.endLevel < m_currentLevel) {
                        elevatorQueue.insert(elevatorQueue.begin(), i.endLevel);
                    }
                }
            }
//            if (i.startLevel > m_currentLevel && m_direction == 1 &&
//                !(std::find(elevatorQueue.begin(), elevatorQueue.end(), i.startLevel) != elevatorQueue.end())) {
//                elevatorQueue.push_back(i.startLevel);
//            }
//            if (i.endLevel > m_currentLevel && m_direction == 1 &&
//                !(std::find(elevatorQueue.begin(), elevatorQueue.end(), i.endLevel) != elevatorQueue.end())) {
//                elevatorQueue.push_back(i.endLevel);
//            }
//            if (i.startLevel < m_currentLevel && m_direction == 0 &&
//                !(std::find(elevatorQueue.begin(), elevatorQueue.end(), i.startLevel) != elevatorQueue.end())) {
//                elevatorQueue.push_back(i.startLevel);
//            }
//            if (i.endLevel < m_currentLevel && m_direction == 0 &&
//                !(std::find(elevatorQueue.begin(), elevatorQueue.end(), i.endLevel) != elevatorQueue.end())) {
//                elevatorQueue.push_back(i.endLevel);
//            }
//            if (i.startLevel < m_currentLevel && m_direction == 1 &&
//                !(std::find(elevatorQueue.begin(), elevatorQueue.end(), i.startLevel) != elevatorQueue.end())) {
//                elevatorQueue.push_back(i.startLevel);
//            }
//            if (i.endLevel < m_currentLevel && m_direction == 1 &&
//                !(std::find(elevatorQueue.begin(), elevatorQueue.end(), i.endLevel) != elevatorQueue.end())) {
//                elevatorQueue.push_back(i.endLevel);
//            }
//            if (i.startLevel > m_currentLevel && m_direction == 0 &&
//                !(std::find(elevatorQueue.begin(), elevatorQueue.end(), i.startLevel) != elevatorQueue.end())) {
//                elevatorQueue.push_back(i.startLevel);
//            }
//            if (i.endLevel > m_currentLevel && m_direction == 0 &&
//                !(std::find(elevatorQueue.begin(), elevatorQueue.end(), i.endLevel) != elevatorQueue.end())) {
//                elevatorQueue.push_back(i.endLevel);
//            }
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Elevator Simulation");

    std::vector<int> queueForElevator;
    std::vector<passengerRoute> passengerButtonQueue;

    sf::Font font;
    if (!font.loadFromFile("font/Akira Expanded Demo.otf")) {
        std::cout << "Font error " << std::endl;
    }

    sf::Texture texture;
    if (!texture.loadFromFile("img/person_sprite.png")) {
        std::cout << "Texture error " << std::endl;
    }
    texture.setSmooth(true);

    Elevator elevator;

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


        for (int i = 0; i < 5; i++) {                  //for every floor
            floors[i].listenForButtons(buttonSwitch, sf::Mouse::getPosition(window), &texture);
            if (elevator.getCurrentLevel() == floors[i].getFloorValue() && !floors[i].isFloorEmpty() && !elevator.isElevatorFull()) {
                elevator.acceptPassenger(floors[i].sendPassengerToElevator(&window));
            }
            elevator.deliverPassneger(floors, &window, passengerButtonQueue);
            floors[i].returnPassengers(passengerButtonQueue);
            floors[i].getRidOfPassenger(elevator.setElevatorFreeze());
        }                                                                     //TODO dodaæ funckcje reserve dla vektora windy ¿eby nie resizeowaæ

        elevator.elevatorLogic(queueForElevator, passengerButtonQueue);      //TODO do konstruktorow dodaæ lisy inicjowania
        elevator.runElevator(queueForElevator);

        for (auto &i: queueForElevator) {                                  //TODO usun¹æ this->y
            std::cout << i << ' ';
        }
        std::cout << '\n';
        for (auto &j: passengerButtonQueue) {
            std::cout << '(' << j.startLevel << " | " << j.endLevel << ") ";
        }
        std::cout << '\n';
        std::cout << "--------------" << '\n';

        window.clear(sf::Color(255, 255, 255));
        elevator.render(&window);
        for (int i = 0; i < 5; i++) {
            floors[i].render(&window);
        }
        window.draw(elevator.get_line());
        window.draw(elevator.get_line2());

        buttonSwitch = false;
        window.display();
    }
    return 0;
}