#include "EnginesSet.hpp"

EnginesSet::EnginesSet()
{
    sairloop = 0;
    buttonUpState = digitalRead(buttonUp);
    buttonSelectState = digitalRead(buttonSelect);
    buttonDownState = digitalRead(buttonDown);
}

void EnginesSet::insertMotor(char cord, int _step, int _dir)
{
    Engine *engineConstrutor = new Engine(cord, _step, _dir);

    this->engines.push_back(engineConstrutor);
}

// void EnginesSet::boot(); // Lógica de fim de curso

void EnginesSet::addToenginesToPlay(std::string cords)
{
    for (auto &cord : cords)
    {
        for (auto engine : this->engines)
        {
            if (engine->getCord() == cord)
            {
                if (buttonUpState == 0 || buttonSelectState == 0 || buttonDownState == 0)
                {
                    sairloop = 1;
                }
                this->enginesToPlay.push_back(engine);
            }
        }
    }
}

void EnginesSet::playMany(int times = 1)
{
    Engine *anyEngine = this->enginesToPlay[0];

    for (int i = 0; i < times; i++)
    {
        if (anyEngine->getTarget() > 0)
        {
            while (anyEngine->getPosition() < anyEngine->getTarget())
            {
                for (auto engine : this->enginesToPlay)
                {
                    if (buttonUpState == 0 || buttonSelectState == 0 || buttonDownState == 0)
                    {
                        sairloop = 1;
                    }
                    engine->runToTarget();
                    delay(3);
                }
            }
        }
        else
        {
            while (anyEngine->getPosition() > anyEngine->getTarget())
            {
                for (auto engine : this->enginesToPlay)
                {
                    if (buttonUpState == 0 || buttonSelectState == 0 || buttonDownState == 0)
                    {
                        sairloop = 1;
                    }
                    engine->runToTarget();
                    delay(3);
                }
            }
        }

        for (auto engine : this->enginesToPlay)
        {
            engine->reverseTarget();
        }
    }

    this->enginesToPlay.clear();
}

void EnginesSet::parseFile(std::string stream)
{
    char cordaChar;
    std::string subStream = "";
    int forControl = 0;
    int strControl = 0;
    for (forControl; forControl < stream.size(); forControl++)
    {
        cordaChar = stream[forControl];
        if (cordaChar == ' ')
        {
            if (stream[forControl - 1] == 'd')
            {
                runThrough(DOWN);
            }
            else if (stream[forControl - 1] == 's')
            {
                runThrough(UP);
            }
            else
            {
                for (strControl; strControl < forControl; strControl++)
                {
                    subStream = subStream + stream[strControl];
                }
                this->addToenginesToPlay(subStream);
                Serial.println(subStream.c_str());
                this->playMany();
                subStream = "";
            }
            delay(500);
            strControl = forControl + 1;
        }
    }
}

void EnginesSet::runThrough(Direction direction)
{

    if (direction == UP)
    {
        for (auto it = this->engines.begin(); it != this->engines.end(); ++it)
        {
            this->enginesToPlay.push_back(*it);
            delay(100);
            this->playMany();
        }
    }
    else
    {
        for (auto it = this->engines.rbegin(); it != this->engines.rend(); ++it)
        {
            this->enginesToPlay.push_back(*it);
            delay(100);
            this->playMany();
        }
    }
}

void EnginesSet::tune(int tunePosition)
{
    switch (tunePosition)
    {
    case 0:
    {
        parseFile("E ");
        break;
    }
    case 1:
    {
        parseFile("A ");
        break;
    }
    case 2:
    {
        parseFile("D ");
        break;
    }
    case 3:
    {
        parseFile("B ");
        break;
    }
    case 4:
    {
        parseFile("G ");
        break;
    }
    case 5:
    {
        parseFile("e ");
        break;
    }
    }
}

void EnginesSet::playOneStep(int cordReset, int signal)
{
    this->engines[cordReset]->oneStep(signal);
}

void EnginesSet::getEnginePos(SDCard x)
{
    int *pos;
    char cord;
    std::vector<int> enginePos;
    for (int i = 0; i < engines.size(); i++)
    {
        cord = engines[i]->getCord();
        pos = engines[i]->getPosPtr();
        enginePos.push_back(*pos);
    }
    x.writeInFile(enginePos);
    x.printPosition("/final_position.txt");
    enginePos.clear();
}

// void EnginesSet::displayCord() {
//   char cord;
//   for (int i = 0; i < engines.size(); i++) {
//     cord = engines[i]->getCord();
//     Serial.println(cord);
//   }
// }

// int EnginesSet::getSize() {
//   return this-> engines.size();
// }
