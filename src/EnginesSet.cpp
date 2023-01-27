#include "EnginesSet.hpp"

EnginesSet::EnginesSet()
{
    mExitLoop = 0;
    mButtonUpState = digitalRead(buttonUp);
    mButtonSelectState = digitalRead(buttonSelect);
    mButtonDownState = digitalRead(buttonDown);
}

void EnginesSet::insertMotor(char pCord, int pStep, int pDir)
{
    Engine *engineConstrutor = new Engine(pCord, pStep, pDir);

    this->mEngines.push_back(engineConstrutor);
}

// void EnginesSet::boot(); // Lógica de fim de curso

void EnginesSet::addToenginesToPlay(std::string pCords)
{
    for (auto &cord : pCords)
    {
        for (auto engine : this->mEngines)
        {
            if (engine->getCord() == cord)
            {
                if (mButtonUpState == 0 || mButtonSelectState == 0 || mButtonDownState == 0)
                {
                    mExitLoop = 1;
                }
                this->mEnginesToPlay.push_back(engine);
            }
        }
    }
}

void EnginesSet::playMany(int pTimes = 1)
{
    Engine *anyEngine = this->mEnginesToPlay[0];

    for (int i = 0; i < pTimes; i++)
    {
        if (anyEngine->getTarget() > 0)
        {
            while (anyEngine->getPosition() < anyEngine->getTarget())
            {
                for (auto engine : this->mEnginesToPlay)
                {
                    if (mButtonUpState == 0 || mButtonUpState == 0 || mButtonDownState == 0)
                    {
                        mExitLoop = 1;
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
                for (auto engine : this->mEnginesToPlay)
                {
                    if (mButtonUpState == 0 || mButtonUpState == 0 || mButtonDownState == 0)
                    {
                        mExitLoop = 1;
                    }
                    engine->runToTarget();
                    delay(3);
                }
            }
        }

        for (auto engine : this->mEnginesToPlay)
        {
            engine->reverseTarget();
        }
    }

    this->mEnginesToPlay.clear();
}

void EnginesSet::parseFile(std::string pStream)
{
    char cord;
    std::string subStream = "";
    int forControl = 0;
    int strControl = 0;
    for (forControl; forControl < pStream.size(); forControl++)
    {
        cord = pStream[forControl];
        if (cord == ' ')
        {
            if (pStream[forControl - 1] == 'd')
            {
                runThrough(DOWN);
            }
            else if (pStream[forControl - 1] == 's')
            {
                runThrough(UP);
            }
            else
            {
                for (strControl; strControl < forControl; strControl++)
                {
                    subStream = subStream + pStream[strControl];
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

void EnginesSet::runThrough(mDirection pDirection)
{

    if (pDirection == UP)
    {
        for (auto it = this->mEngines.begin(); it != this->mEngines.end(); ++it)
        {
            this->mEnginesToPlay.push_back(*it);
            delay(100);
            this->playMany();
        }
    }
    else
    {
        for (auto it = this->mEngines.rbegin(); it != this->mEngines.rend(); ++it)
        {
            this->mEnginesToPlay.push_back(*it);
            delay(100);
            this->playMany();
        }
    }
}

void EnginesSet::tune(int pTunePosition)
{
    switch (pTunePosition)
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

void EnginesSet::playOneStep(int pCordReset, int pSignal)
{
    this->mEngines[pCordReset]->oneStep(pSignal);
}

void EnginesSet::getEnginePos(SDCard pSd)
{
    int *pos;
    char cord;
    std::vector<int> enginePos;
    for (int i = 0; i < mEngines.size(); i++)
    {
        cord = mEngines[i]->getCord();
        pos = mEngines[i]->getPosPtr();
        enginePos.push_back(*pos);
    }
    pSd.writeInFile(enginePos);
    pSd.printPosition("/final_position.txt");
    enginePos.clear();
}