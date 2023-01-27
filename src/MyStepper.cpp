#include "MyStepper.hpp"

MyStepper::MyStepper(int pRevolution, int pStep, int pDir) : mRevolution(pRevolution), mSTEP(pStep), mDIR(pDir)
{
    pinMode(pDir, OUTPUT);
    pinMode(pStep, OUTPUT);
}

int MyStepper::getDir()
{
    return this->mDIR;
}

void MyStepper::setSpeed(int pSpeedRPM)
{
    this->mSpeedRPM = pSpeedRPM;
}

void MyStepper::step(int pSteps)
{
    float frequency = ((this->mSpeedRPM / 60) * this->mRevolution);
    int iFreq = frequency;

    if (pSteps > 0)
    {
        float timeMs = (pSteps / frequency) * 1000;
        digitalWrite(this->mDIR, HIGH);
        tone(this->mSTEP, iFreq, timeMs + 1);
        // delay(tempoMilis + 1);
    }
    if (pSteps < 0)
    {
        float timeMs = -(pSteps / frequency) * 1000;
        digitalWrite(this->mDIR, LOW);
        tone(this->mSTEP, iFreq, timeMs + 1);
        // delay(tempoMilis + 1);
    }
}