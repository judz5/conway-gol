#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>

const int CELLSIZE = 10;

class Map {

public:
    const int WIDTH = 100;
    const int HEIGHT = 100;

    struct Pixel {
        bool state;
        sf::RectangleShape shape;

        Pixel(bool state) : state(state) {
            shape.setSize(sf::Vector2f(CELLSIZE, CELLSIZE));
            shape.setOutlineThickness(0.5);
            shape.setOutlineColor(sf::Color(201, 201, 201));

            if (state) {
                shape.setFillColor(sf::Color::Black);
            } else {
                shape.setFillColor(sf::Color::White);
            }
        }

        void setState(bool newState) {
            state = newState;
            if (state) {
                shape.setFillColor(sf::Color::Black);
            } else {
                shape.setFillColor(sf::Color::White);
            }
        }
    };

    std::vector<std::vector<Pixel>> map;

    Map() {
        map.resize(WIDTH, std::vector<Pixel>(HEIGHT, Pixel(false)));
        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {
                map[i][j].shape.setPosition(i * CELLSIZE, j * CELLSIZE);
            }
        }
    }

    Map &operator=(const Map &other) {
        if (this != &other) { // Avoid self-assignment
            map.resize(WIDTH, std::vector<Pixel>(HEIGHT, Pixel(false)));
            // Copy the map data from 'other' to 'this'
            for (int i = 0; i < WIDTH; i++) {
                for (int j = 0; j < HEIGHT; j++) {
                    map[i][j] = other.map[i][j];
                }
            }
        }
        return *this;
    }

    void clear() {
        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {
                map[i][j].setState(false);
            }
        }
    }

    Pixel &get(int row, int col) {
        return map[row][col];
    }

    void set(int row, int col) {
        map[row][col].setState(!map[row][col].state); // toggle
    }

    void draw(sf::RenderWindow &window) {
        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {
                window.draw(map[i][j].shape);
            }
        }
    }

    int getNeighbor(int row, int col) {
        int count = 0;
        for (int i = row - 1; i <= row + 1; i++) {
            for (int j = col - 1; j <= col + 1; j++) {
                if (i >= 0 && i < WIDTH && j >= 0 && j < HEIGHT && (i != row || j != col)) {
                    if (map[i][j].state == true) {
                        count++;
                    }
                }
            }
        }
        return count;
    }

    void randomize() {
        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {
                bool randomState = rand() % 2 == 0;
                map[i][j].setState(randomState);
            }
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Conways Game of Life");

    Map game;
    bool playing = false;
    int generation = 0;

    // bool mouseHeldDown = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                break;
            }
            if (event.type == sf::Event::MouseButtonPressed && !playing) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int x = event.mouseButton.x / CELLSIZE;
                    int y = event.mouseButton.y / CELLSIZE;
                    if (x >= 0 && x <= game.WIDTH && y >= 0 && y <= game.HEIGHT) {
                        game.set(x, y);
                        // mouseHeldDown = true;
                    }
                }
            }
            // if (event.type == sf::Event::MouseButtonReleased && !playing) {
            //     if (event.mouseButton.button == sf::Mouse::Left) {
            //         mouseHeldDown = false;
            //     }
            // }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    playing = !playing;
                }
                if (event.key.code == sf::Keyboard::C && !playing) {
                    game.clear();
                }
                if (event.key.code == sf::Keyboard::R && !playing) {
                    game.randomize();
                }
            }
        }

        // if (mouseHeldDown && !playing) {
        //     int x = sf::Mouse::getPosition(window).x / CELLSIZE;
        //     int y = sf::Mouse::getPosition(window).y / CELLSIZE;
        //     if (x >= 0 && x <= game.WIDTH && y >= 0 && y <= game.HEIGHT) {
        //         game.get(x, y).setState(true);
        //     }
        // }

        if (playing) {
            Map copy = game;

            for (int i = 0; i < game.WIDTH; i++) {
                for (int j = 0; j < game.HEIGHT; j++) {
                    int neighbors = game.getNeighbor(i, j);
                    if (game.get(i, j).state) {
                        if (neighbors < 2 || neighbors > 3) {
                            copy.get(i, j).setState(false);
                        } else if (neighbors == 2 || neighbors == 3) {
                            copy.get(i, j).setState(true);
                        }
                    } else {
                        if (neighbors == 3) {
                            copy.get(i, j).setState(true);
                        }
                    }
                }
            }

            game = copy;
            generation++;
        }

        window.clear();
        game.draw(window);
        window.display();

        sf::sleep(sf::milliseconds(100));
    }

    return 0;
}
