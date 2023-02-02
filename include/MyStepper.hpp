#ifndef MYSTEPPER
#define MYSTEPPER

#include <Arduino.h>

/**
 * @brief A classe MyStepper é utilizada para controlar os motores de passo.
 */
class MyStepper
{
public:
    // construtor do driver
    /**
     * @brief Construtor do objeto MyStepper.
     *
     * @param pRevolution int que representa a quantidade de passos necessários para o motor dar uma volta completa.
     * @param pStep Pino do driver.
     * @param pDir Pino do driver.
     */
    MyStepper(int pRevolution, int pStep, int pDir);

    // retorna a direção de rotação
    /**
     * @brief Retorna a direção de rotação do motor
     *
     * @return int que representa a direção do motor
     */
    int getDir();

    // atualiza a velocidade de rotação
    /**
     * @brief Atualiza a velocidade de rotação do motor.
     *
     * @param pSpeedRPM int que representa a nova velocidade
     */
    void setSpeed(int pSpeedRPM);

    // funcionamento do driver em cada passo
    /**
     * @brief Faz o motor girar em um número especifico de passos.
     * @details - `digitalWrite()` recebe como parâmetros o pino e seu valor (**HIGH** ou **LOW**), sendo que HIGH irá aumentar a tensão no pino e
     * LOW irá diminuir a tensão.
     * Primeiro é feito um cálculo para determinar a frequência do motor.
     * Depois é verificado se pSteps é positivo ou negativo para saber para qual lado o motor irá girar.
     * Caso seja positivo, a tensão é aumentada no pino de direção através da função `digitalWrite()` e caso seja negativo, a tensão é diminuída.
     * Depois a mesma função é utilizada aumentando e diminuindo a tensão no pino de passos, fazendo o motor girar.
     *
     * @param pSteps int que representa a quantidade de passos que o motor irá dar, podendo ser positivo ou negativo.
     */
    void step(int pSteps);

private:
    /**
     * @brief Pino do drive
     *
     */
    int mDIR; // pino do drive
    /**
     * @brief Pino do drive
     *
     */
    int mSTEP; // pino do drive
    /**
     * @brief Velocidade de rotação
     *
     */
    int mSpeedRPM; // velocidade de rotação
    /**
     * @brief Quantidade de passos para dar uma volta completa
     *
     */
    int mRevolution; // quantidade de passos necessários para dar uma volta completa
};

#endif