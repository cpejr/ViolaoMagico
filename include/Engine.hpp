#ifndef ENGINE
#define ENGINE

#include "MyStepper.hpp"

class Engine
{
public:
    MyStepper *stepperConstructor;

    // construtor do motor
    Engine(char pGuitarString, int pStep, int pDir);

    char getGuitarString();

    int getTarget();

    void reverseTarget();

    // utilizado para rodar até o posição final
    void runToTarget();

    int getPosition();

    int *getPosPtr();

    // utilizado para resetar o motor
    void oneStep(int signal_);

private:
    char mGuitarString; // corda
    int mTarget;        // posição final
    int mPosition;      // posição atual
    int *mPosPtr;       // ponteiro para gravar a posição
    int GLOBAL_SPEED;
    int GLOBAL_STEPS;
    int GLOBAL_TARGET;
};

#endif