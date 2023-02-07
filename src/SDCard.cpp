#include "SDCard.hpp"

SDCard::SDCard()
{
    GLOBAL_SPEED = 90;
    SD.begin(5);

    if (!SD.exists("musicas"))
    {

        // Cria o diretório de músicas
        SD.mkdir("/musicas");

        // Essa parte não é essencial. Ela cria um arquivo
        // só pra ler e testar
        File teste = SD.open("/musicas/Teste.txt", FILE_WRITE);

        if (teste)
        {
            teste.println("V<60>");
            teste.println("S<1>");
            teste.println("1<AE  A E  DG Be  D B d s >");
            teste.close();
        }
    }

    Makelist();
}

vector<string> SDCard::getList()
{
    return this->mFilesNames;
}

void SDCard::Makelist()
{
    File root = SD.open("/musicas");
    while (true)
    {
        File entry = root.openNextFile();
        if (!entry)
        {
            break;
        }
        this->mFilesNames.push_back(entry.name());
    }
}

void SDCard::printPosition(std::string pFileName)
{
    const char *c = pFileName.c_str();
    File textFile = SD.open(c);
    if (textFile)
    {
        Serial.print("Conteudo do arquivo de posição: \n");
        while (textFile.available())
        {
            Serial.write(textFile.read());
        }
        textFile.close();
    }
    else
    {
        Serial.println("error opening file");
    }
}

string SDCard::readFile(int pNumFile, char pNumStroke)
{
    File text = SD.open("/musicas");
    string verify = "";
    string file = "";
    int goBack = 0;
    for (int i = 0; i < pNumFile; i++)
    {
        File textFile = text.openNextFile();
        if (i == pNumFile - 1)
        {
            if (textFile)
            {
                while (textFile.available())
                {
                    char ch = textFile.read();
                    if (ch == pNumStroke)
                    {
                        ch = textFile.read();
                        if (ch == '<')
                        {
                            while (goBack != 1)
                            {
                                ch = textFile.read();
                                if (ch == '>')
                                {
                                    goBack = 1;
                                }
                                else
                                {
                                    file += ch;
                                }
                            }
                        }
                    }
                    else if (ch == 'V')
                    {
                        string num = "";
                        int vel;
                        ch = textFile.read();
                        if (ch == '<') {
                        for (int i = 0; i < 3; i++)
                        {
                            ch = textFile.read();
                            num += ch;
                        }
                        vel = stoi(num);
                        SD_SPEED = vel;

                        }
                        else if (ch == '>') {
                        break;
                        }
                    }
                    else if (ch == 'S')
                    {
                        string sub = "";
                        int numSubdivision;
                        ch = textFile.read();
                        if (ch == '<')
                        {
                            for (int i = 0; ch != '>'; i++)
                            {
                                ch = textFile.read();
                                sub += ch;
                            }
                        }
                        numSubdivision = stoi(sub);
                        SD_SUBDIVISION = numSubdivision;
                    }
                    else
                    {
                        verify = "";
                    }
                }
                return file;
                textFile.close();
            }
        }
    }
}

void SDCard::writeInFile(vector<int> pEnginePos)
{

    // const char * fileNamePtr = fileName.c_str();

    File textFile = SD.open("/final_position.txt", FILE_WRITE);

    if (textFile)
    {
        Serial.print("\n Gravando a posição dos motores... \n");
        for (auto &i : pEnginePos)
        {
            textFile.println(i);
        }
        // textFile.println(enginePos);
        textFile.close();
    }
}