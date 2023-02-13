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
    // Serial.println(this->mSpeedRPM);
}

void MyStepper::step(int pSteps)
{
    float frequency = ((this->mSpeedRPM / 60) * this->mRevolution);
    int iFreq = frequency;

    if (pSteps > 0)
    {
        digitalWrite(this->mDIR, HIGH);
        delay(10);
        for (int i = 0; i < pSteps; i++)
        {

            digitalWrite(this->mSTEP, LOW);
            delay(10);
            digitalWrite(this->mSTEP, HIGH);
            delay(10);
        }
    }
    if (pSteps < 0)
    {

        digitalWrite(this->mDIR, LOW);
        delay(10);
        for (int i = 0; i > pSteps; i--)
        {
            digitalWrite(this->mSTEP, LOW);
            delay(10);
            digitalWrite(this->mSTEP, HIGH);
            delay(10);
        }
    }
}
