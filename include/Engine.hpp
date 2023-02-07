#ifndef ENGINE
#define ENGINE

#include "MyStepper.hpp"

/**
 * @brief A classe Engine corresponde a único um motor.
 *
 * @details Os atributos do motor são o seu identificador (mGuitarString) , a sua posição (mPosition) e o seu alvo (mTarget).
 * Em um motor de passo, é interessante salvar a posição atual do motor tendo em vista que ele próprio não consegue fazer isso quando ligado.
 */
class Engine
{
public:
    /**
     * @brief Ponteiro de acesso direto às funções do objeto Stepper
     */
    MyStepper *stepperConstructor;

    /**
     * @brief  Construtor do motor.
     * @details Inicializa as variaveis privadas referentes aos pinos dos motores e a corda do violão.
     * @param pGuitarString Corda do violão
     * @param pStep, pDir pinos do motor
     */
    Engine(char pGuitarString, int pStep, int pDir);

    /**
     * @brief  Getter que retorna a corda do violão que o motor é responsável
     */
    char getGuitarString();

    /**
     * @brief  Getter que retorna os passos que o motor precisa andar
     * para tocar uma corda
     */
    int getTarget();

    /**
     * @brief  Função que coloca reverte o target, para o motor fazer o movimento de subida e descida na corda
     */
    void reverseTarget();

    /**
     * @brief  Função que faz o motor fazer o movimento  até o alvo de passos
     */
    void runToTarget();

    /**
     * @brief  Getter que retorna a posição do motor
     */
    int getPosition();

    /**
     * @brief  Getter que retorna o ponteiro referente a posição do motor
     */
    int *getPosPtr();

    /**
     * @brief Faz com que o motor ande um passo
     *
     * @param signal_ to be explained
     */
    void oneStep(int signal_);

    /**
     * @brief Utilizado para determinar o valor da velocidade
     * 
     * @param sdSpeed 
     */
    void setSpeed(int sdSpeed);

    /**
     * @brief Determina o BPM do violão
     * 
     * @param sdSpeed 
     */
    void setDelaySpeed(int sdSpeed);

    /**
     * @brief Retorna o valor do delay entre os pulsos
     * 
     * @return int 
     */
    int getDelaySpeed();

private:
    /**
     * @brief Corda do motor
     */
    char mGuitarString;

    /**
     * @brief Passos a serem alcançados pelo motor
     */
    int mTarget;

    /**
     * @brief  Posição do Motor
     */
    int mPosition;

    /**
     * @brief  Ponteiro de posição do motor
     */
    int *mPosPtr;
    int GLOBAL_SPEED;
    int GLOBAL_STEPS;
    int GLOBAL_TARGET;
};

#endif