#ifndef ENGINESSET
#define ENGINESSET

#include "Engine.hpp"
#include "SDCard.hpp"
#include <vector>
#include <string>
#include <Arduino.h>

#define buttonUp 3
#define buttonSelect 16
#define buttonDown 17

class EnginesSet
{
private:
    std::vector<Engine *> engines;
    std::vector<Engine *> enginesToPlay;
    int sairloop;
    int buttonUpState;
    int buttonSelectState;
    int buttonDownState;
    enum Direction
    {
        UP = 0,
        DOWN = 1
    };

public:
    EnginesSet();

    void insertMotor(char cord, int _step, int _dir);

    // void boot(); // Lógica de fim de curso

    // adiciona os motores que irão girar de acordo com a corda presente na música
    void addToenginesToPlay(std::string cords);

    // roda o motor antes adicionado
    void playMany(int times);

    // toca a música
    void parseFile(std::string stream);

    // faz subida ou descida em todos os motores
    void runThrough(Direction direction);

    void tune(int tunePosition);

    // reseta o motor selecionado
    void playOneStep(int cordReset, int signal);

    void getEnginePos(SDCard x);

    // void displayCord();

    // int getSize();
};

#endif