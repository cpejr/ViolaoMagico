#ifndef SDCARD
#define SDCARD

#include <vector>
#include <SD.h>
#include <string>

using namespace std;

/**
 * @brief A classe SDCard usada para gerenciar um objeto do tipo SD da biblioteca de SD para Arduino.
 *
 */
class SDCard
{
public:
    /**
     * @brief Construtor padrão de um novo cartão SD.
     *
     */
    SDCard();

    /**
     * @brief Retorna o vetor com o nome de todos os arquivos no SD
     *
     * @return vector<string> vetor de strings com o nome de todos os arquivos armazenados no SD.
     */
    vector<string> getList();

    /**
     * @brief Preenche o vector filesNames com os nomes dos arquivos no cartão
     *
     */
    void Makelist();

    /**
     * @brief Printa a posição atual dos motores.
     *
     * @param pFileName string contendo o nome do arquivo onde está armazenado as posições dos motores.
     */
    void printPosition(std::string pFileName);

    /**
     * @brief  Recebe como parametro a posição de uma musica no cartão e o n da batida que deverá ser retornada.
     *
     * @param pNumFile
     * @param pNumStroke
     * @return string
     */
    string readFile(int pNumFile, char pNumStroke);

    /**
     * @brief Escreve a posição dos motores no arquivo de armazenar posição.
     *
     * @param pEnginePos
     */
    void writeInFile(vector<int> pEnginePos);

private:
    /**
     * @brief Vetor que conterá o nome dos arquivos no cartão SD.
     *
     */
    vector<string> mFilesNames;
    int GLOBAL_SPEED;
};

#endif