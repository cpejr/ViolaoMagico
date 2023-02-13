#include <SD.h>
#include <SPI.h>
#include <Arduino.h>
#include <vector>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ILI9341.h>

#include "SDCard.hpp"
#include "MyStepper.hpp"
#include "Engine.hpp"
#include "EnginesSet.hpp"

using namespace std;

TaskHandle_t Task_Buttons;

vector<string> musicNames;
int musicNumber;
int j;

// Variáveis do display TFT
#define DC 32
#define CS 33
Adafruit_ILI9341 tft = Adafruit_ILI9341(CS, DC);

int delayButtons = 150;
#define buttonUp 3
#define buttonSelect 16
#define buttonDown 17

int exitLoop = 0;

int GLOBAL_SPEED = 90;
const int GLOBAL_STEPS = 200;
const int GLOBAL_TARGET = 30;

int buttonUpState;
int buttonSelectState;
int buttonDownState;
int *enginePos;

// bool haltReadingButtons = false;

void readingButtons(void *parameter)
{
  for (;;)
  {
    buttonUpState = digitalRead(buttonUp);
    buttonSelectState = digitalRead(buttonSelect);
    buttonDownState = digitalRead(buttonDown);
  }
}

EnginesSet guitar;

SDCard sdCard;
// SDCard* xPtr = &x;

int targetScreen = 1;

void settings(int *targetScreen)
{
  tft.fillScreen(ILI9341_BLACK);
  int goBack = 0;
  int posSettings = 2;
  while (goBack == 0)
  {
    if (posSettings == 2)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Menu:");
      tft.setTextSize(2);
      tft.println("");
      tft.write(16);
      tft.println("Musicas");
      tft.println(" Afinar");
      tft.println(" Resetar motores");
    }
    if (posSettings == 3)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Menu:");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Musicas");
      tft.write(16);
      tft.println("Afinar");
      tft.println(" Resetar motores");
    }
    if (posSettings == 4)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Menu:");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Musicas");
      tft.println(" Afinar");
      tft.write(16);
      tft.println("Resetar motores");
    }

    if (buttonUpState == 0 && posSettings != 2)
    {
      posSettings--;
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      delay(delayButtons);
    }
    if (buttonDownState == 0 && posSettings != 4)
    {
      posSettings++;
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      delay(delayButtons);
    }
    if (buttonSelectState == 0)
    {
      goBack = 1;
      delay(delayButtons);
    }
  }
  *targetScreen = posSettings;
}

void menu(int *targetScreen)
{
  // Declarando as variáveis locais
  tft.fillScreen(ILI9341_BLACK);
  int goBackCursor = 0;
  int selectedMusic = 0;
  int goBack = 0;
  int menuPos = 1;
  int screenNumber = musicNumber / 10;
  int lastMusicScreenNumber = musicNumber % 10;
  int presentScreen = 0;
  if (lastMusicScreenNumber != 0)
  {
    screenNumber += 1;
  }
  int menuPosMax = musicNumber + screenNumber;
  while (goBack == 0)
  {
    if (presentScreen == 0)
    {
      if (musicNumber < 10)
      {
        tft.setCursor(0, 0);
        tft.setTextSize(3);
        tft.println("Musicas:");
        tft.setTextSize(2);
        tft.println("");
        for (j = 1; j < musicNumber + 1; j++)
        {
          if (menuPos == j)
          {
            tft.write(16);
            tft.print(j);
            tft.print("-");
            tft.println(musicNames[j - 1].c_str());
            selectedMusic = j;
          }
          else
          {
            tft.print(" ");
            tft.print(j);
            tft.print("-");
            tft.println(musicNames[j - 1].c_str());
          }
        }
        if (menuPos == menuPosMax)
        {
          tft.write(16);
          tft.print("Voltar ");
          goBackCursor = 1;
        }
        else
        {
          tft.print(" Voltar ");
          goBackCursor = 0;
        }
      }
      if (musicNumber > 9)
      {
        tft.setCursor(0, 0);
        tft.setTextSize(3);
        tft.println("Musicas:");
        tft.setTextSize(2);
        tft.println("");
        for (j = 1; j < 11; j++)
        {
          if (menuPos == j)
          {
            tft.write(16);
            tft.print(j);
            tft.print("-");
            tft.println(musicNames[j - 1].c_str());
            selectedMusic = j;
          }
          else
          {
            tft.print(" ");
            tft.print(j);
            tft.print("-");
            tft.println(musicNames[j - 1].c_str());
          }
        }
        if (menuPos % 11 == 0)
        {
          tft.write(16);
          tft.print("Voltar ");
          goBackCursor = 1;
        }
        else
        {
          tft.print(" Voltar ");
          goBackCursor = 0;
        }
      }
      delay(10);
    }
    if (presentScreen > 0 && presentScreen < screenNumber - 1)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Musicas:");
      tft.setTextSize(2);
      tft.println("");
      for (j = (10 * presentScreen) + 1; j < (10 * (presentScreen + 1)) + 1; j++)
      {
        if ((menuPos - presentScreen) == j)
        {
          tft.write(16);
          tft.print(j);
          tft.print("-");
          tft.println(musicNames[j - 1].c_str());
          selectedMusic = j;
        }
        else
        {
          tft.print(" ");
          tft.print(j);
          tft.print("-");
          tft.println(musicNames[j - 1].c_str());
        }
      }
      if (menuPos % 11 == 0)
      {
        tft.write(16);
        tft.print("Voltar ");
        goBackCursor = 1;
      }
      else
      {
        tft.print(" Voltar ");
        goBackCursor = 0;
      }
    }
    if (presentScreen == screenNumber - 1 && screenNumber != 1)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Musicas:");
      tft.setTextSize(2);
      tft.println("");
      for (j = (10 * presentScreen) + 1; j < musicNumber + 1; j++)
      {
        if ((menuPos - presentScreen) == j)
        {
          tft.write(16);
          tft.print(j);
          tft.print("-");
          tft.println(musicNames[j - 1].c_str());
          selectedMusic = j;
        }
        else
        {
          tft.print(" ");
          tft.print(j);
          tft.print("-");
          tft.println(musicNames[j - 1].c_str());
        }
      }
      if (menuPos == menuPosMax)
      {
        tft.write(16);
        tft.print("Voltar ");
        goBackCursor = 1;
      }
      else
      {
        tft.print(" Voltar ");
        goBackCursor = 0;
      }
    }
    delay(10);

    if (buttonUpState == 0 && menuPos != 1)
    {
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      menuPos--;
      if (menuPos % 11 == 0)
      {
        presentScreen--;
        tft.fillScreen(ILI9341_BLACK);
      }
      delay(delayButtons);
    }
    if (buttonDownState == 0 && menuPos != menuPosMax)
    {
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      if (menuPos % 11 == 0)
      {
        presentScreen++;
        tft.fillScreen(ILI9341_BLACK);
      }
      menuPos++;
      delay(delayButtons);
    }
    if (buttonSelectState == 0)
    {
      if (goBackCursor == 0)
      {
        music(selectedMusic);
        delay(delayButtons);
      }
      else
      {
        *targetScreen = 1;
        goBack = 1;
        delay(delayButtons);
      }
    }
  }
}

void music(int music)
{
  // Declarando as variáveis locais
  tft.fillScreen(ILI9341_BLACK);
  string firstStroke = sdCard.readFile(1, '1');
  string secondStroke = sdCard.readFile(1, '2');
  string thirdStroke = sdCard.readFile(1, '3');

  // funcao do SD que seta a velocidade SDCard.SD_SPEED
  guitar.setEngineSpeed(sdCard);
  // função do EnginesSet que seta o número de subdivisões SD_SUBDIVISION
  guitar.setSubdivision(sdCard);

  int nStrokes = 3;
  if (secondStroke.length() == 0 && thirdStroke.length() == 0)
  {
    nStrokes = 1;
  }
  if (secondStroke.length() != 0 && thirdStroke.length() == 0)
  {
    nStrokes = 2;
  }

  int goBack = 0;
  int goBackMusica = 0;
  int menuPos = 1;
  while (goBackMusica == 0)
  {
    while (goBack == 0)
    {
      if (menuPos == 1)
      {
        tft.setCursor(0, 0);
        tft.setTextSize(3);
        tft.print(musicNames[music - 1].c_str());
        tft.println(":");
        tft.setTextSize(2);
        tft.println("");
        tft.println("-Algumas Informacoes");
        tft.setCursor(0, 200);
        tft.write(16);
        tft.println("Tocar");
        tft.println(" Voltar");
      }
      if (menuPos == 2)
      {
        tft.setCursor(0, 0);
        tft.setTextSize(3);
        tft.print(musicNames[music - 1].c_str());
        tft.println(":");
        tft.setTextSize(2);
        tft.println("");
        tft.println("-Algumas Informacoes");
        tft.setCursor(0, 200);
        tft.println(" Tocar");
        tft.write(16);
        tft.println("Voltar");
      }

      if (buttonUpState == 0 && menuPos != 1)
      {
        menuPos--;
        tft.fillRect(0, 200, 10, 40, ILI9341_BLACK);
        delay(delayButtons);
      }
      if (buttonDownState == 0 && menuPos != 2)
      {
        menuPos++;
        tft.fillRect(0, 200, 10, 40, ILI9341_BLACK);
        delay(delayButtons);
      }
      if (buttonSelectState == 0)
      {
        goBack = 1;
        tft.fillScreen(ILI9341_BLACK);
        delay(delayButtons);
      }
    }
    if (menuPos == 1)
    {
      strokes(firstStroke, secondStroke, thirdStroke, nStrokes);
      goBack = 0;
    }
    if (menuPos == 2)
    {
      goBackMusica = 1;
    }
  }
}

int tunePos = 0;
void taskTune(void *parameter) // Tem que estar nessa posição, depois de setEngine e antes de afinar
{
  guitar.tune(tunePos);
  vTaskDelete(NULL);
}
void afinar(int *targetScreen)
{
  tft.fillScreen(ILI9341_BLACK);
  int goBack = 0;
  tunePos = 0;
  while (goBack == 0)
  {
    if (tunePos == 0)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Selecionar corda:");
      tft.setTextSize(2);
      tft.println("");
      tft.write(16);
      tft.println("Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (tunePos == 1)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Selecionar corda:");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Corda E");
      tft.write(16);
      tft.println("Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (tunePos == 2)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Selecionar corda:");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.write(16);
      tft.println("Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (tunePos == 3)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Selecionar corda:");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.write(16);
      tft.println("Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (tunePos == 4)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Selecionar corda:");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.write(16);
      tft.println("Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (tunePos == 5)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Selecionar corda:");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.write(16);
      tft.println("Corda e");
      tft.println(" Voltar");
    }
    if (tunePos == 6)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Selecionar corda:");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.write(16);
      tft.println("Voltar");
    }
    delay(10);

    if (buttonUpState == 0 && tunePos != 0)
    {
      tunePos--;
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      delay(delayButtons);
    }
    if (buttonDownState == 0 && tunePos != 6)
    {
      tunePos++;
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      delay(delayButtons);
    }
    if (buttonSelectState == 0)
    {
      if (tunePos != 6)
      {
        xTaskCreatePinnedToCore(taskTune, "taskTune", 1000, NULL, 1, NULL, 0);
      }
      else
      {
        tft.fillScreen(ILI9341_BLACK);
        goBack = 1;
        *targetScreen = 1;
      }
      delay(delayButtons);
    }
    guitar.getEnginePos(sdCard);
  }
}

void resetEngines(int *targetScreen)
{
  tft.fillScreen(ILI9341_BLACK);
  int goBack = 0;
  int resetPos = 0;
  while (goBack == 0)
  {
    if (resetPos == 0)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Selecionar corda:");
      tft.setTextSize(2);
      tft.println("");
      tft.write(16);
      tft.println("Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (resetPos == 1)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Selecionar corda:");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Corda E");
      tft.write(16);
      tft.println("Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (resetPos == 2)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Selecionar corda:");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.write(16);
      tft.println("Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (resetPos == 3)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Selecionar corda:");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.write(16);
      tft.println("Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (resetPos == 4)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Selecionar corda:");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.write(16);
      tft.println("Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (resetPos == 5)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Selecionar corda:");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.write(16);
      tft.println("Corda e");
      tft.println(" Voltar");
    }
    if (resetPos == 6)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Selecionar corda:");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.write(16);
      tft.println("Voltar");
    }
    delay(10);

    if (buttonUpState == 0 && resetPos != 0)
    {
      resetPos--;
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      delay(delayButtons);
    }
    if (buttonDownState == 0 && resetPos != 6)
    {
      resetPos++;
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      delay(delayButtons);
    }
    if (buttonSelectState == 0)
    {
      if (resetPos != 6)
      {
        adjustEngine(&resetPos);
      }
      else
      {
        tft.fillScreen(ILI9341_BLACK);
        goBack = 1;
        *targetScreen = 1;
      }
      delay(delayButtons);
    }
  }
}

void adjustEngine(int *resetPos)
{
  int guitarString = *resetPos;
  string guitarStrings = "EADGBe";
  char guitarStringChr = guitarStrings[guitarString];
  tft.fillScreen(ILI9341_BLACK);
  int goBack = 0;
  int adjustPos = 0;
  while (goBack == 0)
  {
    if (adjustPos == 0)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.print("Corda ");
      tft.print(guitarStringChr);
      tft.println(":");
      tft.setTextSize(2);
      tft.println("");
      tft.write(16);
      tft.println("Subir");
      tft.println(" Descer");
      tft.println(" Pronto");
    }
    if (adjustPos == 1)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.print("Corda ");
      tft.print(guitarStringChr);
      tft.println(":");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Subir");
      tft.write(16);
      tft.println("Descer");
      tft.println(" Pronto");
    }
    if (adjustPos == 2)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.print("Corda ");
      tft.print(guitarStringChr);
      tft.println(":");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Subir");
      tft.println(" Descer");
      tft.write(16);
      tft.println("Pronto");
    }

    if (buttonUpState == 0 && adjustPos != 0)
    {
      adjustPos--;
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      delay(delayButtons);
    }
    if (buttonDownState == 0 && adjustPos != 2)
    {
      adjustPos++;
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      delay(delayButtons);
    }
    if (buttonSelectState == 0)
    {
      if (adjustPos == 0)
      {
        guitar.playOneStep(guitarString, 1);
        guitar.getEnginePos(sdCard);
      }
      if (adjustPos == 1)
      {
        guitar.playOneStep(guitarString, -1);
        guitar.getEnginePos(sdCard);
      }
      if (adjustPos == 2)
      {
        tft.fillScreen(ILI9341_BLACK);
        goBack = 1;
        *resetPos = 1;
        delay(delayButtons);
      }
    }
  }
}

int lastStroke = 0;
string defStroke = "";
void taskStroke(void *parameter)
{
  guitar.parseFile(defStroke);
  lastStroke = 0;
  vTaskDelete(NULL);
}

void strokes(string firstStroke, string secondStroke, string thirdStroke, int nStrokes)
{
  tft.fillScreen(ILI9341_BLACK);
  ;
  int returnPlaying = 0;
  int playingPos = 1;
  while (returnPlaying == 0)
  {
    if (playingPos == 1)
    {
      tft.setCursor(80, 0);
      tft.setTextSize(3);
      tft.println("BATIDA 1");
      tft.setCursor(0, 100);
      tft.write(27);
      tft.setCursor(300, 100);
      tft.write(26);
      tft.setTextSize(2);
      tft.setCursor(100, 160);
      tft.write(16);
      tft.println("Voltar");
      defStroke = firstStroke;
      if (lastStroke == 0)
      {
        lastStroke = 1;
        xTaskCreatePinnedToCore(taskStroke, "taskStroke", 1000, NULL, 1, NULL, 0);
      }
    }
    if (playingPos == 2)
    {
      tft.setCursor(80, 0);
      tft.setTextSize(3);
      tft.println("BATIDA 2");
      tft.setCursor(0, 100);
      tft.write(27);
      tft.setCursor(300, 100);
      tft.write(26);
      tft.setTextSize(2);
      tft.setCursor(100, 160);
      tft.write(16);
      tft.println("Voltar");
      defStroke = secondStroke;
      if (lastStroke == 0)
      {
        lastStroke = 1;
        xTaskCreatePinnedToCore(taskStroke, "taskStroke", 1000, NULL, 1, NULL, 0);
      }
    }
    if (playingPos == 3)
    {
      tft.setCursor(0, 0);
      tft.setCursor(80, 0);
      tft.setTextSize(3);
      tft.println("BATIDA 3");
      tft.setCursor(0, 100);
      tft.write(27);
      tft.setCursor(300, 100);
      tft.write(26);
      tft.setTextSize(2);
      tft.setCursor(100, 160);
      tft.write(16);
      tft.println("Voltar");
      defStroke = thirdStroke;
      if (lastStroke == 0)
      {
        lastStroke = 1;
        xTaskCreatePinnedToCore(taskStroke, "taskStroke", 1000, NULL, 1, NULL, 0);
      }
    }

    guitar.getEnginePos(sdCard);

    if (buttonUpState == 0)
    {
      tft.fillRect(200, 0, 30, 30, ILI9341_BLACK);
      if (nStrokes == 3)
      {
        if (playingPos != 1)
        {
          playingPos--;
        }
        else
        {
          playingPos = 3;
        }
      }
      if (nStrokes == 2)
      {
        if (playingPos != 1)
        {
          playingPos--;
        }
        else
        {
          playingPos = 2;
        }
      }
      if (nStrokes == 1)
      {
        if (playingPos != 1)
        {
          playingPos--;
        }
        else
        {
          playingPos = 1;
        }
      }
      delay(delayButtons);
    }
    if (buttonDownState == 0)
    {
      tft.fillRect(200, 0, 30, 30, ILI9341_BLACK);
      if (nStrokes == 3)
      {
        if (playingPos != 3)
        {
          playingPos++;
        }
        else
        {
          playingPos = 1;
        }
      }
      if (nStrokes == 2)
      {
        if (playingPos != 2)
        {
          playingPos++;
        }
        else
        {
          playingPos = 1;
        }
      }
      if (nStrokes == 1)
      {
        if (playingPos != 1)
        {
          playingPos++;
        }
        else
        {
          playingPos = 1;
        }
      }
      delay(delayButtons);
    }
    if (buttonSelectState == 0)
    {
      returnPlaying = 1;
      delay(delayButtons);
    }
  }
  tft.fillScreen(ILI9341_BLACK);
  ;
}

void setup()
{

  Serial.begin(9600);

  musicNames = sdCard.getList();
  musicNumber = musicNames.size();

  guitar.insertMotor('E', 13, 12);
  guitar.insertMotor('A', 14, 27);
  guitar.insertMotor('D', 26, 25);
  guitar.insertMotor('G', 21, 22);
  guitar.insertMotor('B', 15, 2);
  guitar.insertMotor('e', 4, 1);

  // Teste para a inicialização do TFT
  tft.begin();
  tft.setRotation(1);

  // Inicializar a fonte com tamanho padrão
  tft.setTextSize(2);              // Tamanho do texto
  tft.setTextColor(ILI9341_WHITE); // Cor do texto

  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonSelect, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);

  // Limpa o display
  tft.fillScreen(ILI9341_BLACK);

  xTaskCreatePinnedToCore(
      readingButtons, /* Função da tarefa                        */
      "Task_Buttons", /* Nome da tarefa                          */
      1000,           /* Tamanho da tarefa na Stack              */
      NULL,           /* Parâmetro para a tarefa                 */
      1,              /* Prioridade                              */
      &Task_Buttons,  /* Handler para supervisionar a tarefa     */
      0               /* Núcleo do ESP32 no qual a tarefa rodará */
  );
}

void loop()
{
  switch (targetScreen)
  {
  case 1:
  {
    settings(&targetScreen);
    break;
  }
  case 2:
  {
    menu(&targetScreen);
    break;
  }
  case 3:
  {
    afinar(&targetScreen);
    break;
  }
  case 4:
  {
    resetEngines(&targetScreen);
    break;
  }
  }
}