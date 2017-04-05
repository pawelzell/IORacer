/**
 *  @file
 *  @ingroup car
 *  @author Jacek Łysiak <jaceklysiako.o@gmail.com>
 *  @date 3/31/17
 */

#include <iostream>

#include <Car.h>
#include <Entity.h>
#include <MathUtil.h>

#define SCALE 30.f

int Car::GetEntityType() {
    return CAR;
}

void Car::CreateBody(b2World *World, int positionX, int positionY) {
    b2BodyDef BodyDef;
    BodyDef.position = b2Vec2(positionX / SCALE, positionY / SCALE);
    BodyDef.type = b2_dynamicBody;
    body_ = World->CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
    b2FixtureDef FixtureDef;
    FixtureDef.density = 4.f;
    FixtureDef.friction = 1.7f;
    FixtureDef.restitution = 0.5;
    FixtureDef.shape = &Shape;
    b2Fixture* fixture = body_->CreateFixture(&FixtureDef);
    body_->SetUserData(this);
}

Car::Car() {

}

void Car::DebugPrint() {
    std::cout << "Car ";
    std::cout << body_->GetPosition().x << " "
              << body_->GetPosition().y << std::endl;
}

void Car::Initialize(b2World *world, int x, int y) {
    CreateBody(world, x, y);

    car_texture.loadFromFile("../resource/car.png");
    car_sprite.setTexture(car_texture);
    car_sprite.setOrigin(16.f, 16.f);
    car_sprite.setScale(0.5f,0.5f);
}

Car::~Car() {
    body_->GetWorld()->DestroyBody(body_);
}

void Car::Update(int state, float modifier) {
    updateFriction(modifier);
    updateDrive(state);
    updateTurn(state);
}

const sf::Drawable &Car::GetSprite() {
    car_sprite.setPosition(SCALE * body_->GetPosition().x,
                       SCALE * body_->GetPosition().y);
    car_sprite.setRotation(body_->GetAngle() * 180 / b2_pi);
    return car_sprite;
}

void Car::setCharacteristics(float maxForwardSpeed_, float maxBackwardSpeed_,
                             float maxDriveForce_) {
    maxForwardSpeed = maxForwardSpeed_;
    maxBackwardSpeed = maxBackwardSpeed_;
    maxDriveForce = maxDriveForce_;
}

b2Vec2 Car::getLateralVelocity() {
    b2Vec2 currentRightNormal = body_->GetWorldVector(b2Vec2(1, 0));
    return b2Dot(currentRightNormal, body_->GetLinearVelocity()) * currentRightNormal;
}

b2Vec2 Car::getForwardVelocity() {
    b2Vec2 currentForwardNormal = body_->GetWorldVector(b2Vec2(0, 1));
    return b2Dot(currentForwardNormal, body_->GetLinearVelocity()) * currentForwardNormal;
}

void Car::updateFriction(float modifier) {
    //lateral linear velocity
    float maxLateralImpulse = 2.5f;
    b2Vec2 impulse = body_->GetMass() * -getLateralVelocity();
    if (impulse.Length() > maxLateralImpulse)
        impulse *= maxLateralImpulse / impulse.Length();
    body_->ApplyLinearImpulse(impulse, body_->GetWorldCenter(), true);

    //angular velocity
    body_->ApplyAngularImpulse(0.1f * body_->GetInertia() *
                              -body_->GetAngularVelocity(), true);

    //forward linear velocity
    b2Vec2 currentForwardNormal = getForwardVelocity();
    float currentForwardSpeed = currentForwardNormal.Normalize();
    float dragForceMagnitude = -2 * currentForwardSpeed;
    body_->ApplyForce(modifier * dragForceMagnitude * currentForwardNormal,
                     body_->GetWorldCenter(), true);
}

void Car::updateDrive(int controlState) {

    //find desired speed
    float desiredSpeed = 0;
    switch (controlState & (UP | DOWN)) {
        case UP:
            desiredSpeed = maxForwardSpeed;
            break;
        case DOWN:
            desiredSpeed = maxBackwardSpeed;
            break;
        default:
            return;//do nothing
    }

    //find current speed in forward direction
    b2Vec2 currentForwardNormal = body_->GetWorldVector(b2Vec2(0, 1));
    float currentSpeed = b2Dot(getForwardVelocity(), currentForwardNormal);

    //apply necessary force
    float force = 0;
    if (desiredSpeed > currentSpeed)
        force = maxDriveForce;
    else if (desiredSpeed < currentSpeed)
        force = -maxDriveForce;
    else
        return;
    body_->ApplyForce(force * currentForwardNormal, body_->GetWorldCenter
            (), true);
}

void Car::updateTurn(int controlState) {
    float desiredTorque = 0;
    switch (controlState & (LEFT | RIGHT)) {
        case LEFT:
            desiredTorque = -15;
            break;
        case RIGHT:
            desiredTorque = +15;
            break;
        default:;//nothing
    }
    body_->ApplyTorque(desiredTorque, true);
}

const sf::Vector2f &Car::GetPosition() const {
    return car_sprite.getPosition();
}

float Car::GetRotation() {
    return MathUtil::RadianToDegree(body_->GetAngle());
}

float Car::GetSpeed() {
    return body_->GetLinearVelocity().Length();
}


