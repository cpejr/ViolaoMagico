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
            teste.println("X150");
            teste.println("1<AE A E AE E A AE d s >");
            teste.close();
        }
    }

    Makelist();
}

vector<string> SDCard::getList()
{
    return filesNames;
}

void SDCard::Makelist()
{
    File root = SD.open("/musicas");
    while (true)
    {
        File entry = root.openNextFile();
        if (!entry)
        {
            // no more files
            break;
        }
        filesNames.push_back(entry.name());
    }
}

void SDCard::printPosition(std::string fileName)
{
    const char *c = fileName.c_str();
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

string SDCard::readFile(int numFile, char numBatida)
{
    File text = SD.open("/musicas");
    string verifica = "";
    string arquivo = "";
    int retorno = 0;
    for (int i = 0; i < numFile; i++)
    {
        File textFile = text.openNextFile();
        if (i == numFile - 1)
        {
            if (textFile)
            {
                while (textFile.available())
                {
                    char ch = textFile.read();
                    if (ch == numBatida)
                    {
                        ch = textFile.read();
                        if (ch == '<')
                        {
                            while (retorno != 1)
                            {
                                ch = textFile.read();
                                if (ch == '>')
                                {
                                    retorno = 1;
                                }
                                else
                                {
                                    arquivo += ch;
                                }
                            }
                        }
                    }
                    else if (ch == 'X')
                    {
                        string num = "";
                        int vel;
                        for (int i = 0; i < 3; i++)
                        {
                            ch = textFile.read();
                            num += ch;
                        }
                        vel = stoi(num);
                        GLOBAL_SPEED = vel;
                    }
                    else
                    {
                        verifica = "";
                    }
                }
                return arquivo;
                textFile.close();
            }
        }
    }
}

void SDCard::writeInFile(vector<int> enginePos)
{

    // const char * fileNamePtr = fileName.c_str();

    File textFile = SD.open("/final_position.txt", FILE_WRITE);

    if (textFile)
    {
        Serial.print("\n Gravando a posição dos motores... \n");
        for (auto &i : enginePos)
        {
            textFile.println(i);
        }
        // textFile.println(enginePos);
        textFile.close();
    }
}