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

/**
 * @brief A classe EnginesSet corresponde a um conjunto de motores.
 * @details EngineSet armazena e gerencia o funcionamento de todos os motores do violão.
 *
 */
class EnginesSet
{
public:
    /**
     * @brief Enum para direções de subida/descida
     *
     */
    enum mDirection
    {
        UP = 0,
        DOWN = 1
    };

    /**
     * @brief Construtor padrao que apenas inicializa a classe
     *
     */
    EnginesSet();
    /**
     * @brief Função responsável por inserir um motor no vetor engines.
     *
     * @param pGuitarString Corda que o motor é responsável
     * @param pStep Pinos do motor
     * @param pDir Pinos do motor
     */
    void insertMotor(char pGuitarString, int pStep, int pDir);

    // void boot(); // Lógica de fim de curso

    /**
     * @brief  Função responsável para adicionar quais motores serão tocados no vetor enginestoplay a cada batida.
     * @param pGuitarString Qual cordas serao tocadas
     */
    void addToenginesToPlay(std::string pGuitarString);

    /**
     * @brief Função responsável por tocar cada corda de acordo com os que estao no vetor enginestoplay
     * @param pTimes Quantas vezes a funcao vai ser chamada para tocar os motores
     */
    void playMany(int pTimes);

    /**
     * @brief  Funçao responsável por ler um arquivo de texto
     */
    void parseFile(std::string pStream);

    /**
     * @brief Percorre as cordas do violão fazendo uma descida ou subida.
     * @param direction direção a percorrer (para cima ou para baixo)
     */
    void runThrough(mDirection pDirection);

    /**
     * @brief Recebe um número inteiro que corresponde a uma corda e por meio de um switch case chama a função parseFile para tocar a corda desejada.
     * Usada para afinar o violão.
     * @param pTunePosition Identifica qual corda deve ser afinada.
     */
    void tune(int pTunePosition);

    /**
     * @brief Reseta o motor selecionado
     *
     * @param pGuitarReset Referencia qual motor deve ser reiniciada
     * @param pSignal Indica se o motor deve girar no sentido horário ou anti horário.
     */
    void playOneStep(int pGuitarReset, int pSignal);

    /**
     * @brief Essa função registra a posição atual de cada motor e escreve esse valor de posição em um arquivo de texto.
     *
     * @param pSd Objeto do tipo SDCard.
     */
    void getEnginePos(SDCard pSd);

    /**
     * @brief configura velocidade a cada motor
     * 
     * @param pSd Objeto do tipo SDCard.
     */
    void setEngineSpeed(SDCard pSd); 

    /**
     * @brief determina o número de subdivisões de um compasso
     * 
     * @param pSd Objeto do tipo SDCard.
     */
    void setSubdivision(SDCard pSd);

    /**
     * @brief determina e retorna o delay entre um pulso e outro
     * 
     * @return int
     */
    int setDelay();

private:
    /**
     * @brief  Vetor que armazena todos os motores declarados
     */
    std::vector<Engine *> mEngines;
    /**
     * @brief Vetor que armazena todos os motores a serem tocados a cada batida
     */
    std::vector<Engine *> mEnginesToPlay;
    int mExitLoop;
    /**
     * @brief Estado do botão "up"
     *
     */
    int mButtonUpState;
    /**
     * @brief Estado do botão "select"
     */
    int mButtonSelectState;
    /**
     * @brief Estado do botão "down"
     *
     */
    int mButtonDownState;

    /**
     * @brief BPM do violão
     * 
     */
    int mBPMSpeed; 

    /**
     * @brief Número de subdivisões de um compasso
     * 
     */
    int mSubdivision;
};

#endif