#include "MyStepper.hpp"

MyStepper::MyStepper(int _revolution, int _step, int _dir) : revolution(_revolution), STEP(_step), DIR(_dir)
{
    pinMode(_dir, OUTPUT);
    pinMode(_step, OUTPUT);
}

int MyStepper::getDir()
{
    return this->DIR;
}

void MyStepper::setSpeed(int _speedRPM)
{
    this->speedRPM = _speedRPM;
}

void MyStepper::step(int _steps)
{
    float frequencia = ((this->speedRPM / 60) * this->revolution);
    int freq = frequencia;

    if (_steps > 0)
    {
        float tempoMilis = (_steps / frequencia) * 1000;
        digitalWrite(this->DIR, HIGH);
        tone(this->STEP, freq, tempoMilis + 1);
        // delay(tempoMilis + 1);
    }
    if (_steps < 0)
    {
        float tempoMilis = -(_steps / frequencia) * 1000;
        digitalWrite(this->DIR, LOW);
        tone(this->STEP, freq, tempoMilis + 1);
        // delay(tempoMilis + 1);
    }
}