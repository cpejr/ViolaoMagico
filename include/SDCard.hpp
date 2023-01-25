#ifndef SDCARD
#define SDCARD

#include <vector>
#include <SD.h>
#include <string>

using namespace std;

class SDCard
{
private:
    vector<string> filesNames;
    int GLOBAL_SPEED;

public:
    SDCard();

    vector<string> getList();

    void Makelist();

    void printPosition(std::string fileName);

    string readFile(int numFile, char numBatida);

    void writeInFile(vector<int> enginePos);
};

#endif