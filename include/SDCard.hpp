#ifndef SDCARD
#define SDCARD

#include <vector>
#include <SD.h>
#include <string>

using namespace std;

class SDCard
{
public:
    SDCard();

    vector<string> getList();

    void Makelist();

    void printPosition(std::string pFileName);

    string readFile(int pNumFile, char pNumStroke);

    void writeInFile(vector<int> pEnginePos);

private:
    vector<string> mFilesNames;
    int GLOBAL_SPEED;
};

#endif