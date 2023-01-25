#include "Engine.hpp"

Engine::Engine(char _cord, int _step, int _dir) : CORD(_cord)
{
    GLOBAL_SPEED = 90;
    GLOBAL_STEPS = 200;
    GLOBAL_TARGET = 30;
    this->stepperConstructor = new MyStepper(GLOBAL_STEPS, _step, _dir);
    this->stepperConstructor->setSpeed(GLOBAL_SPEED);

    this->target = GLOBAL_TARGET;

    this->position = 0;

    posPtr = &position;
}

char Engine::getCord()
{
    return this->CORD;
}

int Engine::getTarget()
{
    return this->target;
}

void Engine::reverseTarget()
{
    this->target *= (-1);
}

void Engine::runToTarget()
{
    if (this->target > 0)
    {
        this->stepperConstructor->step(1);
        this->position++;
    }
    else
    {
        this->stepperConstructor->step(-1);
        this->position--;
    }

    // enginePos = getPosPtr();
}

int Engine::getPosition()
{
    return this->position;
}

int *Engine::getPosPtr()
{
    return this->posPtr;
}

void Engine::oneStep(int signal_)
{
    this->stepperConstructor->step(signal_ * 1);
    this->position = 0;
}