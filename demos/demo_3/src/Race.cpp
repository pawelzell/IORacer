//
// Created by pawel on 30.03.17.
//

#include <Race.h>
#include <CheckPoint.h>
#include <CheckPointManager.h>


//#ifndef DEGTORAD
//#define DEGTORAD 0.0174532925199432957f
//#define RADTODEG 57.295779513082320876f
//#endif

void setclr(int *reg, int mask, int type) {
    if (type)
        *reg |= mask;
    else
        *reg &= ~mask;
}

void HandleKeyboard(sf::Event::KeyEvent Event, int *state, int type) {
    /// @TODO change to map
    switch (Event.code) {
        case sf::Keyboard::Key::Left:
            setclr(state, LEFT, type);
            break;
        case sf::Keyboard::Key::Up:
            setclr(state, UP, type);
            break;
        case sf::Keyboard::Key::Down:
            setclr(state, DOWN, type);
            break;
        case sf::Keyboard::Key::Right:
            setclr(state, RIGHT, type);
            break;
        default:
            break;
    }
}

void Race::Initialize() {
    // Prepare map
    map.LoadMap("map_0", "Mapa testowa");
    /// TODO add race builder
    auto checkPoint = new CheckPoint(&world, b2Vec2(8, 12), b2Vec2(9, 1));
    auto checkPoint2 = new CheckPoint(&world, b2Vec2(12, 17), b2Vec2(9, 1));
    std::vector<CheckPoint*> checkPoints =
            {checkPoint, checkPoint2};
    checkPointManager_ = new CheckPointManager(checkPoints);
    car.Initialize(&world, 300, 300);
    /// @TODO inject parameter rather than hardcode it
    car.setCharacteristics(40, -10, 50);
    map.AlignCameraViewSize(window);
    map.SetCameraViewPosition(car.GetPosition());
    world.SetContactListener(&contactListener);
    checkPointManager_->StartTimer();
}

void Race::Run() {
    // Initialize game
    Initialize();

    int carState = 0;

    int cnt = 0;

    while (window.isOpen()) {
        sf::Event Event;
        while (window.pollEvent(Event)) {
            // Close window : exit
            if (Event.type == sf::Event::Closed) {
                window.close();
            } else if (Event.type == sf::Event::KeyPressed) {
                if (Event.key.code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
                HandleKeyboard(Event.key, &carState, 1);
            } else if (Event.type == sf::Event::KeyReleased) {
                HandleKeyboard(Event.key, &carState, 0);
            }
        }

        /** Simulate the world */
        car.Update(carState, map.GetFrictionModifier(car.GetPosition()));
        world.Step(1 / 60.f, 8, 3);
        map.SetCameraViewPosition(car.GetPosition());
//        float zoom = 0.8f + car.GetSpeed()/70;
//        map.SetCameraViewZoom(zoom);
        window.clear(sf::Color::White);
        if (cnt == 120) {
            car.DebugPrint();
            printf("Elapsed time: %f\n", checkPointManager_->GetElapsedTime()
                    .asSeconds());
            cnt = 0;
        } else {
            cnt++;
        }
        //checkPoint->DebugPrint();
        map.RenderBottomLayer(window);
        window.setView(map.GetCameraView());
        checkPointManager_->DrawCheckPoints(&window); /// TEMPORARY
        window.draw(car.GetSprite());
//        map.SetCameraViewZoom(1.f / zoom);
//        window.setView(map.GetMinimapView());
//        window.draw(map.GetFrictionMap());
//        window.draw(car.GetSprite());
        window.setView(window.getDefaultView());
        window.display();
    }
}
