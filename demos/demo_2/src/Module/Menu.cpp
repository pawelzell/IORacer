//
// Created by maciej on 30.03.17.
//

#include "Menu.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

#include <GameData.h>


Menu::Menu(sf::RenderWindow *window, GameDataPtr gameData) :
        Module(window), gameData_(gameData), workshop_(std::make_shared<Workshop>(window_, gameData_)),
        game_(std::make_shared<Game>(window_, gameData_)) {

    sf::Vector2f menuButtonSize = sf::Vector2f(300, 80);
    // set buttons
    playButton_ = std::make_shared<Button>(
            sf::Vector2f(400, 150),
            menuButtonSize,
            "Play",
            [this]() {
                game_->run();
            }
    );

    workshopButton_ = std::make_shared<Button>(
            sf::Vector2f(400, 300),
            menuButtonSize,
            "Workshop",
            [this]() {
                workshop_->run();
            }
    );

    quitButton_ = std::make_shared<Button>(
            sf::Vector2f(400, 450),
            menuButtonSize,
            "Quit",
            [this]() {
                close_ = true;
            }
    );

    // every drawable object
    objects_.push_back(playButton_);
    objects_.push_back(workshopButton_);
    objects_.push_back(quitButton_);

    // buttons
    buttons_.push_back(playButton_);
    buttons_.push_back(workshopButton_);
    buttons_.push_back(quitButton_);
}


int Menu::run() {
    close_ = false;
    while (!close_) {
        bool mouseClick = false; // info for buttons

        // EVENT handling
        sf::Event event;
        while (window_->pollEvent(event)) {
            // "close_ requested" event: end while loop
            if (event.type == sf::Event::Closed) {
                close_ = true;
            }
            // handle mouse click
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                mouseClick = true;
            }
        }

        /// BUTTON things
        std::shared_ptr<Button> activeButton = nullptr;
        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(*window_));
        // select hovered button, change style
        for (auto &button : buttons_) {
            if (button->toggleHover(mousePos)) {
                activeButton = button;
            }
        }
        if (activeButton != nullptr && mouseClick) {
            activeButton->onClick();
        }

        // DRAWING
        window_->clear(sf::Color(60, 70, 80));
        for (auto &drawableObject : objects_) {
            drawableObject->draw(window_);
        }
        window_->display();
    }

    return 0;
}