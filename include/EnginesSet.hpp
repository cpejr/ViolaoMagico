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
public:
    enum mDirection
    {
        UP = 0,
        DOWN = 1
    };

    EnginesSet();

    void insertMotor(char pCord, int pStep, int pDir);

    // void boot(); // Lógica de fim de curso

    // adiciona os motores que irão girar de acordo com a corda presente na música
    void addToenginesToPlay(std::string pCords);

    // roda o motor antes adicionado
    void playMany(int pTimes);

    // toca a música
    void parseFile(std::string pStream);

    // faz subida ou descida em todos os motores
    void runThrough(mDirection pDirection);

    void tune(int pTunePosition);

    // reseta o motor selecionado
    void playOneStep(int pCordReset, int pSignal);

    void getEnginePos(SDCard pSd);

    // void displayCord();

    // int getSize();
private:
    std::vector<Engine *> mEngines;
    std::vector<Engine *> mEnginesToPlay;
    int mExitLoop;
    int mButtonUpState;
    int mButtonSelectState;
    int mButtonDownState;
};

#endif