#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <windows.h>

const int screenWidth = 1280;
const int screenHeight = 720;

const sf::Vector2i zeroFloorCoordinates{
        sf::Vector2i(screenWidth / 2, screenHeight - 70) }; // coordinates relative to the middle of 300x170 elevator
const sf::Vector2i firstFloorCoordinates{ sf::Vector2i(screenWidth / 2, screenHeight - (3 * 70)) };
const sf::Vector2i secondFloorCoordinates{ sf::Vector2i(screenWidth / 2, screenHeight - (5 * 70)) };
const sf::Vector2i thirdFloorCoordinates{ sf::Vector2i(screenWidth / 2, screenHeight - (7 * 70)) };
const sf::Vector2i fourthFloorCoordinates{ sf::Vector2i(screenWidth / 2, screenHeight - (9 * 70)) };

const sf::Vector2f sizeOfElevator(300.0f, 140.0f);
const int FPS = 120;

enum Direction {
    left,
    right,
    up,
    down,
};

enum SittingSpot {
    firstSeat = 0,
    secondSeat = 1,
    thirdSeat = 2,
    fourthSeat = 3,
    fifthSeat = 4,
    sixthSeat = 5,
    seventhSeat = 6,
    eightSeat = 7,
    notSitting = 8,
};

struct Seat {
    float position{};
    bool taken = false;
};

struct passengerRoute {
    int startLevel{};
    int endLevel{};

    friend bool operator== (const passengerRoute& r1, const passengerRoute& r2);
};

bool operator== (const passengerRoute& r1, const passengerRoute& r2)
{
    return (r1.startLevel == r2.startLevel &&
        r1.endLevel == r2.endLevel);
}

class Button {
private:
    int m_value{};
    sf::RectangleShape m_shape{};
    sf::Text m_text{};
    sf::Vector2f m_textPositionVector{};

    sf::Color m_backgroundColor = sf::Color::Magenta;

public:
    Button(int x, int y, int width, int height, sf::Font* font, int value = 0) {
        m_value = value;

        m_shape.setSize(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
        m_shape.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
        m_shape.setFillColor(m_backgroundColor);

        m_text.setFont(*font);
        m_text.setString(std::to_string(m_value));
        m_text.setFillColor(sf::Color::Black);
        m_text.setCharacterSize(25);

        m_textPositionVector = sf::Vector2f((m_shape.getGlobalBounds().getPosition().x) + (m_shape.getGlobalBounds().getSize().x / 2.f) -
            (m_text.getGlobalBounds().getSize().x / 2.f),
            (m_shape.getGlobalBounds().getPosition().y) + (m_shape.getGlobalBounds().getSize().y / 2.f) -
            (m_text.getGlobalBounds().getSize().y));           //first expression sets it in top left corner of button, second moves it to the middle, overshoot by half of the dimensions of text itself, and last expression corrects it

        m_text.setPosition(m_textPositionVector);
    }

    bool clicked(bool event, sf::Vector2i mousePosition) {
        if (static_cast<float>(mousePosition.x) <= m_shape.getPosition().x + m_shape.getSize().x &&
            static_cast<float>(mousePosition.x) >= m_shape.getPosition().x &&
            static_cast<float>(mousePosition.y) <= m_shape.getPosition().y + m_shape.getSize().y &&
            static_cast<float>(mousePosition.y) >= m_shape.getPosition().y &&
            event) {
            return true;
        }
        return false;
    }

    void render(sf::RenderTarget* target) {
        target->draw(m_shape);
        target->draw(m_text);
    }

    int getValue() const { return m_value; }
};

class Passenger {
private:
    passengerRoute m_route{};
    int m_orderNumber{};
    int m_mass = 70;
    sf::Vector2f m_position{};
    sf::Sprite m_sprite{};
    bool sitting = false;
    SittingSpot m_seat = notSitting;

public:
    Passenger(const sf::Texture* texture, int startLevel, int endLevel, int orderNumber) {
        m_route.startLevel = startLevel;
        m_route.endLevel = endLevel;

        m_orderNumber = orderNumber;

        m_sprite.setTexture(*texture);
        m_sprite.setScale(0.25f, 0.25f);

        switch (startLevel) {
        case 0:
            m_position = sf::Vector2f(
                (screenWidth / 2) - (sizeOfElevator.x / 2) - (40 * (orderNumber + 2)), // 40 is width of one passenger
                static_cast<float>(zeroFloorCoordinates.y - 10));
            m_sprite.setPosition(m_position);
            break;
        case 1:
            m_position = sf::Vector2f((screenWidth / 2) + (sizeOfElevator.x / 2) + (40 * orderNumber),
                static_cast<float>(firstFloorCoordinates.y - 10));
            m_sprite.setPosition(m_position);
            break;
        case 2:
            m_position = sf::Vector2f((screenWidth / 2) - (sizeOfElevator.x / 2) - (40 * (orderNumber + 2)),
                static_cast<float>(secondFloorCoordinates.y - 10));
            m_sprite.setPosition(m_position);
            break;
        case 3:
            m_position = sf::Vector2f((screenWidth / 2) + (sizeOfElevator.x / 2) + (40 * orderNumber),
                static_cast<float>(thirdFloorCoordinates.y - 10));
            m_sprite.setPosition(m_position);
            break;
        case 4:
            m_position = sf::Vector2f((screenWidth / 2) - (sizeOfElevator.x / 2) - (40 * (orderNumber + 2)),
                static_cast<float>(fourthFloorCoordinates.y - 10));
            m_sprite.setPosition(m_position);
            break;
        default:
            break;
        }
    }

    bool isSitting() const { return sitting; }

    void setInicator() {
        m_sprite.setColor(sf::Color::Blue);
    }

    void setSitting(int x) {
        if (x == notSitting) {
            sitting = false;
        }
        else {
            sitting = true;
        }
        m_seat = static_cast<SittingSpot>(x);
    }

    int getSeat() const { return m_seat; }

    void setPos(sf::Vector2f pos) {
        m_position = pos;
        m_sprite.setPosition(m_position);
    }

    void move(Direction direction) {
        switch (direction) {
        case right:
            m_position.x += 1;
            break;
        case left:
            m_position.x -= 1;
            break;
        default:
            break;
        }
        m_sprite.setPosition(m_position);
    }

    void render(sf::RenderTarget* target) const {
        target->draw(this->getSprite());
    }

    passengerRoute getRoute() const { return m_route; }

    sf::Sprite getSprite() const { return m_sprite; }

    int getStartLevel() const { return m_route.startLevel; }

    int getEndLevel() const { return m_route.endLevel; }

    int getOrderNumber() const { return m_orderNumber; }

    int getMass() const { return m_mass; }

    sf::Vector2f getPosition() const { return m_position; }
};

class Floor {
private:
    std::vector<Passenger> m_queue{};
    std::vector<Passenger> m_deleteQueue{};
    std::vector<Button> m_otherFloorsButtons{};
    sf::RectangleShape m_shape{};
    int m_id{};

public:
    Floor(int id, sf::Font* font) {
        m_id = id;

        for (int i = 0; i < 5; i++) {
            if (m_id == i) {
                continue;
            }
            Button button((100 + (m_id % 2) * 900 + i * 40), screenHeight - m_id * 140 - 100, 30, 30, font, i);
            m_otherFloorsButtons.push_back(button);
        }

        m_shape.setSize(sf::Vector2f(485.5f, 6.f));                // dimensions of line being "floor"
        m_shape.setFillColor(sf::Color::Black);

        switch (m_id) {
        case 0:
            m_shape.setPosition(                                 // horizontal distance of elevator x is vertical dimension of floor
                sf::Vector2f((sizeOfElevator.x + 485.5f), static_cast<float>(zeroFloorCoordinates.y) + 68));
            break;
        case 1:
            m_shape.setPosition(
                sf::Vector2f((static_cast<float>(m_id % 2) * 790.f), static_cast<float>(firstFloorCoordinates.y) + 68));
            break;
        case 2:
            m_shape.setPosition(
                sf::Vector2f((static_cast<float>(m_id % 2) * 790.f), static_cast<float>(secondFloorCoordinates.y) + 68));
            break;
        case 3:
            m_shape.setPosition(
                sf::Vector2f((static_cast<float>(m_id % 2) * 790.f), static_cast<float>(thirdFloorCoordinates.y) + 68));
            break;
        case 4:
            m_shape.setPosition(
                sf::Vector2f((static_cast<float>(m_id % 2) * 790.f), static_cast<float>(fourthFloorCoordinates.y) + 68));
            break;
        }
    }

    void listenForButtons(bool event, sf::Vector2i mousePos, const sf::Texture* texture) {
        for (int i = 0; i < 4; i++) {
            if (m_otherFloorsButtons[i].clicked(event, mousePos)) {
                Passenger newPassenger(texture, m_id, m_otherFloorsButtons[i].getValue(), static_cast<int>(m_queue.size()));
                m_queue.push_back(newPassenger);
            }
        }
    }

    Passenger sendPassengerToElevator() {
        Passenger passenger(m_queue.back().getSprite().getTexture(), m_queue.back().getStartLevel(), m_queue.back().getEndLevel(),
            m_queue.back().getOrderNumber());
        passenger.setPos(m_queue.back().getSprite().getPosition());

        m_queue.pop_back();
        return passenger;
    }

    void acceptPassengerToFloor(const Passenger& passenger) {
        m_deleteQueue.push_back(passenger);
    }

    void getRidOfPassenger(bool& freeze) {
        int count = 0;
        for (Passenger& i : m_deleteQueue) {

            if (i.getEndLevel() % 2 == 0) {
                i.move(left);
            }
            else {
                i.move(right);
            }

            if (i.getPosition().x > screenWidth || i.getPosition().x < -70) {
                if (!m_deleteQueue.empty()) {
                    //                    m_deleteQueue.pop_back();
                    m_deleteQueue.erase(m_deleteQueue.begin() + count);
                }
            }

            bool freezeFlag = false;

            if (i.getPosition().x >= (screenWidth / 2 - (sizeOfElevator.x / 2) - 40) && // left side of the elevator minus passenger width
                i.getPosition().x <= (screenWidth / 2 + (sizeOfElevator.x / 2))) {      // right side of the elevator
                freezeFlag = true;
            }

            if (freezeFlag) {
                freeze = true;
            }
            else {
                freeze = false;
            }

            count++;
        }
    }

    void render(sf::RenderTarget* target) {
        target->draw(m_shape);
        for (int i = 0; i < 4; i++) {
            m_otherFloorsButtons[i].render(target);
        }
        for (const auto& i : m_queue) {
            target->draw(i.getSprite());
        }
        for (const auto& i : m_deleteQueue) {
            target->draw(i.getSprite());
        }
    }

    void returnPassengers(std::vector<passengerRoute>& gameQueue) {
        for (auto& i : m_queue) {
            bool pushFlag = true;
            for (auto& j : gameQueue) {
                if (j.startLevel == i.getStartLevel() && j.endLevel == i.getEndLevel()) {
                    pushFlag = false;
                }
            }
            if (pushFlag) {
                gameQueue.push_back(i.getRoute());
            }
        }
    }

    int getFloorSize() const { return m_queue.size(); }

    int getFloorValue() const { return m_id; }

    bool isFloorEmpty() const {
        if (m_queue.empty()) return true;
        else return false;
    }
};

class Elevator {
private:
    int m_totalPassengerMass = 0;
    int m_currentLevel = 0;
    sf::Vector2f m_position{};
    sf::RectangleShape m_rectangle{};
    sf::RectangleShape m_line{};
    sf::RectangleShape m_line2{};
    sf::Vector2f m_sizeOfLine = sf::Vector2f(2000.0f, 5.0f);
    sf::Text m_currentMassOutput{};
    std::vector<int> m_order{};
    std::vector<Passenger> m_elevatorPassengersList{};
    bool m_isElevatorFrozen = false;
    Direction m_direction = up;
    Seat Seats[8] = { 490, false, 523, false, 556, false, 589, false, 621, false, 654, false, 687, false, 720, false };
    //array of Seat structs, its: seat x position, is it taken bool repeated 8 times

public:
    Elevator(sf::Font* font) {
        // rectangle(elevator)
        m_rectangle.setOrigin(sizeOfElevator.x / 2, sizeOfElevator.y / 2);
        m_position = sf::Vector2f(zeroFloorCoordinates);
        m_rectangle.setPosition(m_position);
        m_rectangle.setOutlineColor(sf::Color::Black);
        m_rectangle.setOutlineThickness(4.3f);
        m_rectangle.setFillColor(sf::Color::White);
        m_rectangle.setSize(sizeOfElevator);

        // lines next to elevator(walls)
        m_line.setSize(m_sizeOfLine);
        m_line.setOrigin(m_sizeOfLine.x / 2, m_sizeOfLine.y / 2);
        m_line.setPosition(static_cast<float>(screenWidth) / 2 + sizeOfElevator.x / 2, 100.0f);
        m_line.setOutlineColor(sf::Color::Black);
        m_line.setFillColor(sf::Color::Black);
        m_line.setOutlineThickness(2);
        m_line.rotate(90);

        m_line2.setSize(m_sizeOfLine);
        m_line2.setOrigin(m_sizeOfLine.x / 2, m_sizeOfLine.y / 2);
        m_line2.setPosition(static_cast<float>(screenWidth) / 2 - sizeOfElevator.x / 2, 100.0f);
        m_line2.setOutlineColor(sf::Color::Black);
        m_line2.setFillColor(sf::Color::Black);
        m_line2.setOutlineThickness(2);
        m_line2.rotate(90);

        // text used to display current elevator load
        m_currentMassOutput.setFont(*font);
        m_currentMassOutput.setString("Masa: " + std::to_string(m_totalPassengerMass));
        m_currentMassOutput.setFillColor(sf::Color::Black);
        m_currentMassOutput.setCharacterSize(30);
        m_currentMassOutput.setPosition(sf::Vector2f(800.f, 35.f));
    }

    void checkCurrentLevel() {
        if (m_position.y == static_cast<float>(zeroFloorCoordinates.y)) {
            m_currentLevel = 0;
        }
        else if (m_position.y == static_cast<float>(firstFloorCoordinates.y)) {
            m_currentLevel = 1;
        }
        else if (m_position.y == static_cast<float>(secondFloorCoordinates.y)) {
            m_currentLevel = 2;
        }
        else if (m_position.y == static_cast<float>(thirdFloorCoordinates.y)) {
            m_currentLevel = 3;
        }
        else if (m_position.y == static_cast<float>(fourthFloorCoordinates.y)) {
            m_currentLevel = 4;
        }
    }

    void determineDirection(float& increment, const int floor) {
        if (floor > m_currentLevel && !m_isElevatorFrozen) {
            m_direction = up;
            increment = -1.f;
        }
        else if (floor < m_currentLevel && !m_isElevatorFrozen) {
            m_direction = down;
            increment = 1.f;
        }
        else
            increment = 0.f;
    }

    void moveElevator(int floor) {                 //TODO cos sie nie popuje po dotarciu na pietro
        float increment;
        determineDirection(increment, floor);

        m_position.y += increment;
        m_rectangle.setPosition(m_position);

        for (auto& i : m_elevatorPassengersList) {                                //moves passengers with elevator up and down
            i.setPos(sf::Vector2f(i.getPosition().x, m_position.y - 10));
        }
    }

    void takeSeat(Passenger& passenger) {
        bool sittingFlag = false;
        for (int j = 0; j < 8; j++) {               //for every seat in elevator
            if (((passenger.getPosition().x == Seats[j].position) && !Seats[j].taken)) {
                Seats[j].taken = true;
                passenger.setSitting(j);
                std::cout << "T " << j << ' ' << passenger.getEndLevel() << '\n';
                sittingFlag = false;
                break;
            }
            else {
                sittingFlag = true;
            }
        }
        if (sittingFlag) {
            m_isElevatorFrozen = true;
            if (passenger.getStartLevel() % 2 == 1) {
                passenger.move(left);
            }
            else {
                passenger.move(right);
            }
        }
        else {
            m_isElevatorFrozen = false;
        }

    }

    bool fiveSecondTimer(int appFPS, bool resetFlag) {
        static double timer = 0;

        if (resetFlag) timer = 0;
        else timer++;

        if (timer == appFPS * 5) { // appFPS * 5 (5 sec)
            timer = 0;
            return true;
        }
        else return false;
    }

    void runElevator(std::vector<int>& queue) {
        checkCurrentLevel();

        for (auto& i : Seats) {
            std::cout << std::boolalpha << i.taken << ' ';
        }
        std::cout << '\n';

        for (auto& i : m_elevatorPassengersList) {
            if (!i.isSitting()) {
                takeSeat(i);
            }
        }

        bool timerInterruptFlag = false;
        if (!queue.empty()) {
            timerInterruptFlag = true;
            if (m_currentLevel != queue.front()) {
                moveElevator(queue.front());
            }
            else {
                queue.erase(queue.begin());
            }
        }
        else {
            if (fiveSecondTimer(FPS, timerInterruptFlag)) {  // if passenger is sitting on the opposite side of the elevator, timer starts instantly counting
                queue.push_back(0);
            }
        }
    }

    Passenger sendPassengerToFloor(std::vector<passengerRoute>& queue, int passengerSpot) {
        Passenger passenger(m_elevatorPassengersList.at(passengerSpot).getSprite().getTexture(), m_elevatorPassengersList.at(passengerSpot).getStartLevel(),
            m_elevatorPassengersList.at(passengerSpot).getEndLevel(),
            m_elevatorPassengersList.at(passengerSpot).getOrderNumber());
        passenger.setPos(m_elevatorPassengersList.at(passengerSpot).getSprite().getPosition());

        m_elevatorPassengersList.erase(m_elevatorPassengersList.begin() + passengerSpot);

        m_totalPassengerMass -= passenger.getMass();
        m_currentMassOutput.setString("Masa: " + std::to_string(m_totalPassengerMass));

        if (std::find(queue.begin(), queue.end(), passenger.getRoute()) != queue.end()) {
            queue.erase(std::find(queue.begin(), queue.end(), passenger.getRoute()));
        }

        return passenger;
    }

    void acceptPassengerToElevator(const Passenger& passenger) {
        m_elevatorPassengersList.push_back(passenger);
        m_totalPassengerMass += passenger.getMass();
        m_currentMassOutput.setString("Masa: " + std::to_string(m_totalPassengerMass));
    }

    void deliverPassenger(std::vector<Floor>& floors, std::vector<passengerRoute>& queue) {
        int counter{};
        for (int j = 0; j < m_elevatorPassengersList.size(); j++) {
            if (m_elevatorPassengersList[j].getEndLevel() == m_currentLevel) {
                Seats[m_elevatorPassengersList[j].getSeat()].taken = false;
                std::cout << "S " << m_elevatorPassengersList[j].getSeat() << ' ' << counter << '\n';
                m_elevatorPassengersList[j].setSitting(notSitting);
                floors[m_currentLevel].acceptPassengerToFloor(sendPassengerToFloor(queue, j));
                j--;
            }
        }
    }

    bool& returnElevatorFreeze() {
        return m_isElevatorFrozen;
    }

    void render(sf::RenderTarget* target) {
        target->draw(m_rectangle);
        for (const auto& i : m_elevatorPassengersList) {
            target->draw(i.getSprite());
        }
        target->draw(m_currentMassOutput);
    }

    bool isElevatorFull() {
        if (m_elevatorPassengersList.size() == 8) return true;
        else return false;
    }

    void elevatorLogic(std::vector<int>& elevatorQueue, std::vector<passengerRoute>& passengerQueue) {
        int count = 0;

        if (!passengerQueue.empty()) {
            for (auto& i : passengerQueue) {
                if (m_direction == up) {
                    if (i.startLevel > m_currentLevel) {
                        if (std::find(elevatorQueue.begin(), elevatorQueue.end(), i.startLevel) == elevatorQueue.end()) {
                            elevatorQueue.push_back(i.startLevel);
                        }
                    }
                    if (i.endLevel > m_currentLevel) {
                        if (std::find(elevatorQueue.begin(), elevatorQueue.end(), i.endLevel) == elevatorQueue.end()) {
                            elevatorQueue.push_back(i.endLevel);
                        }
                    }
                    if (elevatorQueue.empty()) {
                        m_direction = down;
                    }
                    std::sort(elevatorQueue.begin(), elevatorQueue.end());
                }
                if (m_direction == down) {
                    if (i.endLevel < m_currentLevel) {
                        if (std::find(elevatorQueue.begin(), elevatorQueue.end(), i.endLevel) == elevatorQueue.end()) {
                            elevatorQueue.push_back(i.endLevel);
                        }
                    }
                    if (i.startLevel < m_currentLevel) {
                        if (std::find(elevatorQueue.begin(), elevatorQueue.end(), i.startLevel) == elevatorQueue.end()) {
                            elevatorQueue.push_back(i.startLevel);
                        }
                    }
                    if (elevatorQueue.empty()) {
                        m_direction = up;
                    }
                    std::sort(elevatorQueue.begin(), elevatorQueue.end(), [](int a, int b) {
                        return a > b;
                        });
                }
            }

            count++;
        }

    }

    sf::RectangleShape get_line() const { return m_line; }

    sf::RectangleShape get_line2() const { return m_line2; }

    int getCurrentLevel() const { return m_currentLevel; }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Elevator Simulation");

    std::vector<int> queueForElevator;
    std::vector<passengerRoute> sharedPassengerQueue;

    sf::Font font;
    if (!font.loadFromFile("font/Akira Expanded Demo.otf")) {
        std::cout << "Font error " << std::endl;
    }

    sf::Texture texture;
    if (!texture.loadFromFile("img/person_sprite.png")) {
        std::cout << "Texture error " << std::endl;
    }
    texture.setSmooth(true);

    Elevator elevator(&font);

    std::vector<Floor> floors;
    for (int i = 0; i < 5; i++) {
        Floor floor(i, &font);
        floors.push_back(floor);
    }

    bool buttonSwitch = false;

    window.setFramerateLimit(FPS);
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

        elevator.checkCurrentLevel();

        elevator.elevatorLogic(queueForElevator, sharedPassengerQueue);
        elevator.runElevator(queueForElevator);

        for (int i = 0; i < 5; i++) {                  //for every floor
            floors[i].listenForButtons(buttonSwitch, sf::Mouse::getPosition(window), &texture);

            elevator.deliverPassenger(floors, sharedPassengerQueue);
            floors[i].returnPassengers(sharedPassengerQueue);

            if (elevator.getCurrentLevel() == floors[i].getFloorValue() && !floors[i].isFloorEmpty() && !elevator.isElevatorFull()) {
                elevator.acceptPassengerToElevator(floors[i].sendPassengerToElevator());
            }

            floors[i].getRidOfPassenger(elevator.returnElevatorFreeze());
        }
        buttonSwitch = false;


        // -----------------------------------------

        // for (auto& i : queueForElevator) {
        //     std::cout << i << ' ';
        // }
        // std::cout << '\n';
        // for (auto& j : sharedPassengerQueue) {
        //     std::cout << '(' << j.startLevel << " | " << j.endLevel << ") ";
        // }
        // std::cout << '\n';
        // std::cout << "--------------" << '\n';

        // -----------------------------------------

        window.clear(sf::Color(255, 255, 255));
        elevator.render(&window);
        for (int i = 0; i < 5; i++) {
            floors[i].render(&window);
        }
        window.draw(elevator.get_line());
        window.draw(elevator.get_line2());

        window.display();
    }
    return 0;
}