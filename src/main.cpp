// #include <SD.h>
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

int delayBotoes = 150;
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

EnginesSet violao;

SDCard x;
// SDCard* xPtr = &x;

int telaDestino = 1;

void configuracoes(int *telaDestino)
{
  tft.fillScreen(ILI9341_BLACK);
  int retornar = 0;
  int posicaoConf = 2;
  while (retornar == 0)
  {
    if (posicaoConf == 2)
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
    if (posicaoConf == 3)
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
    if (posicaoConf == 4)
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

    if (buttonUpState == 0 && posicaoConf != 2)
    {
      posicaoConf--;
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      delay(delayBotoes);
    }
    if (buttonDownState == 0 && posicaoConf != 4)
    {
      posicaoConf++;
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      delay(delayBotoes);
    }
    if (buttonSelectState == 0)
    {
      retornar = 1;
      delay(delayBotoes);
    }
  }
  *telaDestino = posicaoConf;
}

void menu(int *telaDestino)
{
  // Declarando as variáveis locais
  tft.fillScreen(ILI9341_BLACK);
  int voltarCursor = 0;
  int musicaAtual = 0;
  int retornar = 0;
  int posicaoDoMenu = 1;
  int screenNumber = musicNumber / 10;
  int lastMusicScreenNumber = musicNumber % 10;
  int presentScreen = 0;
  if (lastMusicScreenNumber != 0)
  {
    screenNumber += 1;
  }
  int posicaoDoMenuMax = musicNumber + screenNumber;
  while (retornar == 0)
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
          if (posicaoDoMenu == j)
          {
            tft.write(16);
            tft.print(j);
            tft.print("-");
            tft.println(musicNames[j - 1].c_str());
            musicaAtual = j;
          }
          else
          {
            tft.print(" ");
            tft.print(j);
            tft.print("-");
            tft.println(musicNames[j - 1].c_str());
          }
        }
        if (posicaoDoMenu == posicaoDoMenuMax)
        {
          tft.write(16);
          tft.print("Voltar ");
          voltarCursor = 1;
        }
        else
        {
          tft.print(" Voltar ");
          voltarCursor = 0;
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
          if (posicaoDoMenu == j)
          {
            tft.write(16);
            tft.print(j);
            tft.print("-");
            tft.println(musicNames[j - 1].c_str());
            musicaAtual = j;
          }
          else
          {
            tft.print(" ");
            tft.print(j);
            tft.print("-");
            tft.println(musicNames[j - 1].c_str());
          }
        }
        if (posicaoDoMenu % 11 == 0)
        {
          tft.write(16);
          tft.print("Voltar ");
          voltarCursor = 1;
        }
        else
        {
          tft.print(" Voltar ");
          voltarCursor = 0;
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
        if ((posicaoDoMenu - presentScreen) == j)
        {
          tft.write(16);
          tft.print(j);
          tft.print("-");
          tft.println(musicNames[j - 1].c_str());
          musicaAtual = j;
        }
        else
        {
          tft.print(" ");
          tft.print(j);
          tft.print("-");
          tft.println(musicNames[j - 1].c_str());
        }
      }
      if (posicaoDoMenu % 11 == 0)
      {
        tft.write(16);
        tft.print("Voltar ");
        voltarCursor = 1;
      }
      else
      {
        tft.print(" Voltar ");
        voltarCursor = 0;
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
        if ((posicaoDoMenu - presentScreen) == j)
        {
          tft.write(16);
          tft.print(j);
          tft.print("-");
          tft.println(musicNames[j - 1].c_str());
          musicaAtual = j;
        }
        else
        {
          tft.print(" ");
          tft.print(j);
          tft.print("-");
          tft.println(musicNames[j - 1].c_str());
        }
      }
      if (posicaoDoMenu == posicaoDoMenuMax)
      {
        tft.write(16);
        tft.print("Voltar ");
        voltarCursor = 1;
      }
      else
      {
        tft.print(" Voltar ");
        voltarCursor = 0;
      }
    }
    delay(10);

    if (buttonUpState == 0 && posicaoDoMenu != 1)
    {
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      posicaoDoMenu--;
      if (posicaoDoMenu % 11 == 0)
      {
        presentScreen--;
        tft.fillScreen(ILI9341_BLACK);
      }
      delay(delayBotoes);
    }
    if (buttonDownState == 0 && posicaoDoMenu != posicaoDoMenuMax)
    {
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      if (posicaoDoMenu % 11 == 0)
      {
        presentScreen++;
        tft.fillScreen(ILI9341_BLACK);
      }
      posicaoDoMenu++;
      delay(delayBotoes);
    }
    if (buttonSelectState == 0)
    {
      if (voltarCursor == 0)
      {
        musica(musicaAtual);
        delay(delayBotoes);
      }
      else
      {
        *telaDestino = 1;
        retornar = 1;
        delay(delayBotoes);
      }
    }
  }
}

void musica(int musica)
{
  // Declarando as variáveis locais
  tft.fillScreen(ILI9341_BLACK);
  string batida1 = x.readFile(1, '1');
  string batida2 = x.readFile(1, '2');
  string batida3 = x.readFile(1, '3');
  int nBatidas = 3;
  if (batida2.length() == 0 && batida3.length() == 0)
  {
    nBatidas = 1;
  }
  if (batida2.length() != 0 && batida3.length() == 0)
  {
    nBatidas = 2;
  }

  int retornar = 0;
  int retornarMusica = 0;
  int posicaoDoMenu = 1;
  while (retornarMusica == 0)
  {
    while (retornar == 0)
    {
      if (posicaoDoMenu == 1)
      {
        tft.setCursor(0, 0);
        tft.setTextSize(3);
        tft.print(musicNames[musica - 1].c_str());
        tft.println(":");
        tft.setTextSize(2);
        tft.println("");
        tft.println("-Algumas Informacoes");
        tft.setCursor(0, 200);
        tft.write(16);
        tft.println("Tocar");
        tft.println(" Voltar");
      }
      if (posicaoDoMenu == 2)
      {
        tft.setCursor(0, 0);
        tft.setTextSize(3);
        tft.print(musicNames[musica - 1].c_str());
        tft.println(":");
        tft.setTextSize(2);
        tft.println("");
        tft.println("-Algumas Informacoes");
        tft.setCursor(0, 200);
        tft.println(" Tocar");
        tft.write(16);
        tft.println("Voltar");
      }

      if (buttonUpState == 0 && posicaoDoMenu != 1)
      {
        posicaoDoMenu--;
        tft.fillRect(0, 200, 10, 40, ILI9341_BLACK);
        delay(delayBotoes);
      }
      if (buttonDownState == 0 && posicaoDoMenu != 2)
      {
        posicaoDoMenu++;
        tft.fillRect(0, 200, 10, 40, ILI9341_BLACK);
        delay(delayBotoes);
      }
      if (buttonSelectState == 0)
      {
        retornar = 1;
        tft.fillScreen(ILI9341_BLACK);
        delay(delayBotoes);
      }
    }
    if (posicaoDoMenu == 1)
    {
      batidas(batida1, batida2, batida3, nBatidas);
      retornar = 0;
    }
    if (posicaoDoMenu == 2)
    {
      retornarMusica = 1;
    }
  }
}

int posicaoAfinar = 0;
void task_tune(void *parameter) // Tem que estar nessa posição, depois de setEngine e antes de afinar
{
  violao.tune(posicaoAfinar);
  vTaskDelete(NULL);
}
void afinar(int *telaDestino)
{
  tft.fillScreen(ILI9341_BLACK);
  int retornar = 0;
  posicaoAfinar = 0;
  while (retornar == 0)
  {
    if (posicaoAfinar == 0)
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
    if (posicaoAfinar == 1)
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
    if (posicaoAfinar == 2)
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
    if (posicaoAfinar == 3)
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
    if (posicaoAfinar == 4)
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
    if (posicaoAfinar == 5)
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
    if (posicaoAfinar == 6)
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

    if (buttonUpState == 0 && posicaoAfinar != 0)
    {
      posicaoAfinar--;
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      delay(delayBotoes);
    }
    if (buttonDownState == 0 && posicaoAfinar != 6)
    {
      posicaoAfinar++;
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      delay(delayBotoes);
    }
    if (buttonSelectState == 0)
    {
      if (posicaoAfinar != 6)
      {
        xTaskCreatePinnedToCore(task_tune, "task_tune", 1000, NULL, 1, NULL, 0);
      }
      else
      {
        tft.fillScreen(ILI9341_BLACK);
        retornar = 1;
        *telaDestino = 1;
      }
      delay(delayBotoes);
    }
    violao.getEnginePos(x);
  }
}

void resetarMotores(int *telaDestino)
{
  tft.fillScreen(ILI9341_BLACK);
  int retornar = 0;
  int posicaoResetar = 0;
  while (retornar == 0)
  {
    if (posicaoResetar == 0)
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
    if (posicaoResetar == 1)
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
    if (posicaoResetar == 2)
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
    if (posicaoResetar == 3)
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
    if (posicaoResetar == 4)
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
    if (posicaoResetar == 5)
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
    if (posicaoResetar == 6)
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

    if (buttonUpState == 0 && posicaoResetar != 0)
    {
      posicaoResetar--;
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      delay(delayBotoes);
    }
    if (buttonDownState == 0 && posicaoResetar != 6)
    {
      posicaoResetar++;
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      delay(delayBotoes);
    }
    if (buttonSelectState == 0)
    {
      if (posicaoResetar != 6)
      {
        ajustandoMotor(&posicaoResetar);
      }
      else
      {
        tft.fillScreen(ILI9341_BLACK);
        retornar = 1;
        *telaDestino = 1;
      }
      delay(delayBotoes);
    }
  }
}

void ajustandoMotor(int *posicaoResetar)
{
  int corda = *posicaoResetar;
  string cordas = "EADGBe";
  char cordaChar = cordas[corda];
  tft.fillScreen(ILI9341_BLACK);
  int retornar = 0;
  int posicaoAjuste = 0;
  while (retornar == 0)
  {
    if (posicaoAjuste == 0)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.print("Corda ");
      tft.print(cordaChar);
      tft.println(":");
      tft.setTextSize(2);
      tft.println("");
      tft.write(16);
      tft.println("Subir");
      tft.println(" Descer");
      tft.println(" Pronto");
    }
    if (posicaoAjuste == 1)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.print("Corda ");
      tft.print(cordaChar);
      tft.println(":");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Subir");
      tft.write(16);
      tft.println("Descer");
      tft.println(" Pronto");
    }
    if (posicaoAjuste == 2)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.print("Corda ");
      tft.print(cordaChar);
      tft.println(":");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Subir");
      tft.println(" Descer");
      tft.write(16);
      tft.println("Pronto");
    }

    if (buttonUpState == 0 && posicaoAjuste != 0)
    {
      posicaoAjuste--;
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      delay(delayBotoes);
    }
    if (buttonDownState == 0 && posicaoAjuste != 2)
    {
      posicaoAjuste++;
      tft.fillRect(0, 24, 10, 225, ILI9341_BLACK);
      delay(delayBotoes);
    }
    if (buttonSelectState == 0)
    {
      if (posicaoAjuste == 0)
      {
        violao.playOneStep(corda, 1);
        violao.getEnginePos(x);
      }
      if (posicaoAjuste == 1)
      {
        violao.playOneStep(corda, -1);
        violao.getEnginePos(x);
      }
      if (posicaoAjuste == 2)
      {
        tft.fillScreen(ILI9341_BLACK);
        retornar = 1;
        *posicaoResetar = 1;
        delay(delayBotoes);
      }
    }
  }
}

int lastBatida = 0;
string definedBatida = "";
void task_batida(void *parameter)
{
  violao.parseFile(definedBatida);
  lastBatida = 0;
  vTaskDelete(NULL);
}

void batidas(string batida1, string batida2, string batida3, int nBatidas)
{
  tft.fillScreen(ILI9341_BLACK);
  ;
  int retornoTocando = 0;
  int posicaoTocando = 1;
  while (retornoTocando == 0)
  {
    if (posicaoTocando == 1)
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
      definedBatida = batida1;
      if (lastBatida == 0)
      {
        lastBatida = 1;
        xTaskCreatePinnedToCore(task_batida, "task_batida", 1000, NULL, 1, NULL, 0);
      }
    }
    if (posicaoTocando == 2)
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
      definedBatida = batida2;
      if (lastBatida == 0)
      {
        lastBatida = 1;
        xTaskCreatePinnedToCore(task_batida, "task_batida", 1000, NULL, 1, NULL, 0);
      }
    }
    if (posicaoTocando == 3)
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
      definedBatida = batida3;
      if (lastBatida == 0)
      {
        lastBatida = 1;
        xTaskCreatePinnedToCore(task_batida, "task_batida", 1000, NULL, 1, NULL, 0);
      }
    }

    violao.getEnginePos(x);

    if (buttonUpState == 0)
    {
      tft.fillRect(200, 0, 30, 30, ILI9341_BLACK);
      if (nBatidas == 3)
      {
        if (posicaoTocando != 1)
        {
          posicaoTocando--;
        }
        else
        {
          posicaoTocando = 3;
        }
      }
      if (nBatidas == 2)
      {
        if (posicaoTocando != 1)
        {
          posicaoTocando--;
        }
        else
        {
          posicaoTocando = 2;
        }
      }
      if (nBatidas == 1)
      {
        if (posicaoTocando != 1)
        {
          posicaoTocando--;
        }
        else
        {
          posicaoTocando = 1;
        }
      }
      delay(delayBotoes);
    }
    if (buttonDownState == 0)
    {
      tft.fillRect(200, 0, 30, 30, ILI9341_BLACK);
      if (nBatidas == 3)
      {
        if (posicaoTocando != 3)
        {
          posicaoTocando++;
        }
        else
        {
          posicaoTocando = 1;
        }
      }
      if (nBatidas == 2)
      {
        if (posicaoTocando != 2)
        {
          posicaoTocando++;
        }
        else
        {
          posicaoTocando = 1;
        }
      }
      if (nBatidas == 1)
      {
        if (posicaoTocando != 1)
        {
          posicaoTocando++;
        }
        else
        {
          posicaoTocando = 1;
        }
      }
      delay(delayBotoes);
    }
    if (buttonSelectState == 0)
    {
      retornoTocando = 1;
      delay(delayBotoes);
    }
  }
  tft.fillScreen(ILI9341_BLACK);
  ;
}

void setup()
{

  Serial.begin(9600);

  musicNames = x.getList();
  musicNumber = musicNames.size();

  violao.insertMotor('E', 13, 12);
  violao.insertMotor('A', 14, 27);
  violao.insertMotor('D', 26, 25);
  violao.insertMotor('G', 21, 22);
  violao.insertMotor('B', 15, 2);
  violao.insertMotor('e', 4, 1);

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
  switch (telaDestino)
  {
  case 1:
  {
    configuracoes(&telaDestino);
    break;
  }
  case 2:
  {
    menu(&telaDestino);
    break;
  }
  case 3:
  {
    afinar(&telaDestino);
    break;
  }
  case 4:
  {
    resetarMotores(&telaDestino);
    break;
  }
  }
}