#ifndef MYSTEPPER
#define MYSTEPPER

#include <Arduino.h>

class MyStepper
{
public:
    // construtor do driver
    MyStepper(int pRevolution, int pStep, int pDir);

    // retorna a direção de rotação
    int getDir();

    // atualiza a velocidade de rotação
    void setSpeed(int pSpeedRPM);

    // funcionamento do driver em cada passo
    void step(int pSteps);

private:
    int mDIR;        // pino do drive
    int mSTEP;       // pino do drive
    int mSpeedRPM;   // velocidade de rotação
    int mRevolution; // quantidade de passos necessários para dar uma volta completa
};

#endif