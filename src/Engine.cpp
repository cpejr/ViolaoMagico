#include "Engine.hpp"

Engine::Engine(char pGuitarString, int pStep, int pDir) : mGuitarString(pGuitarString)
{
    mSpeedRPM = 90;
    GLOBAL_STEPS = 200;
    GLOBAL_TARGET = 15;
    this->stepperConstructor = new MyStepper(GLOBAL_STEPS, pStep, pDir);
    this->stepperConstructor->setSpeed(mSpeedRPM);

    this->mTarget = GLOBAL_TARGET;

    this->mPosition = 0;

    mPosPtr = &mPosition;
}

char Engine::getGuitarString()
{
    return this->mGuitarString;
}

int Engine::getTarget()
{
    return this->mTarget;
}

void Engine::reverseTarget()
{
    this->mTarget *= (-1);
}

void Engine::runToTarget()
{
    if (this->mTarget > 0)
    {
        this->stepperConstructor->step(1);
        this->mPosition++;
    }
    else
    {
        this->stepperConstructor->step(-1);
        this->mPosition--;
    }
}

int Engine::getPosition()
{
    return this->mPosition;
}

int *Engine::getPosPtr()
{
    return this->mPosPtr;
}

void Engine::oneStep(int signal_)
{
    this->stepperConstructor->step(signal_ * 1);
    this->mPosition = 0;
}

void Engine::setSpeed(int pSdSpeed)
{
    this->mSpeedRPM = pSdSpeed;
}

void Engine::setDelaySpeed(int pSdSpeed)
{

    this->mDelaySpeed = pSdSpeed;
}

int Engine::getDelaySpeed()
{
    return this->mDelaySpeed;
}
