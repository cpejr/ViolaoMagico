#ifndef ENGINE
#define ENGINE

#include "MyStepper.hpp"

class Engine
{
private:
    char CORD;    // corda
    int target;   // posição final
    int position; // posição atual
    int *posPtr;  // ponteiro para gravar a posição
    int GLOBAL_SPEED;
    int GLOBAL_STEPS;
    int GLOBAL_TARGET;

public:
    MyStepper *stepperConstructor;

    // construtor do motor
    Engine(char _cord, int _step, int _dir);

    char getCord();

    int getTarget();

    void reverseTarget();

    // utilizado para rodar até o posição final
    void runToTarget();

    int getPosition();

    int *getPosPtr();

    // utilizado para resetar o motor
    void oneStep(int signal_);
};

#endif