#ifndef MYSTEPPER
#define MYSTEPPER

#include <Arduino.h>

class MyStepper
{
private:
    int DIR;        // pino do drive
    int STEP;       // pino do drive
    int speedRPM;   // velocidade de rotação
    int revolution; // quantidade de passos necessários para dar uma volta completa
public:
    // construtor do driver
    MyStepper(int _revolution, int _step, int _dir);

    // retorna a direção de rotação
    int getDir();

    // atualiza a velocidade de rotação
    void setSpeed(int _speedRPM);

    // funcionamento do driver em cada passo
    void step(int _steps);
};

#endif