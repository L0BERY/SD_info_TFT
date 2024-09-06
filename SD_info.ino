#include <SPI.h>
#include <SD.h>
#include "FS.h"
#include <TFT_eSPI.h>

#define CS_PIN 21

File NewFile;

TFT_eSPI tft = TFT_eSPI();

TFT_eSPI_Button button;

void setup() {
  Serial.begin(9600);
  //initSD();
  tft.init();
  tft.invertDisplay(0);
  tft.fillScreen(TFT_BLACK);

  DrawButt();
}

void loop() {
  uint16_t x = 0, y = 0;
  bool pressed = tft.getTouch(&x, &y);
  if(pressed && button.contains(x, y)){
    button.press(true);
  }
  else {
    button.press(false);
  }

  if(button.justReleased()) button.drawButton();
  if(button.justPressed()){
    button.drawButton(true);
    if(!initSD()){
      CardInfo();
      SD.end();
    }
  }

  delay(10);
}

void DrawButt(){
  button.initButton(&tft, 100, 200, 70, 40, TFT_WHITE, TFT_BLUE, TFT_WHITE, "Start", 1);
  button.drawButton();
}

int initSD(){
  if(!SD.begin(CS_PIN)){
    tft.setCursor(0, 20);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.fillScreen(TFT_BLACK);
    tft.println("SD card not connected!");
    return 1;
  }
  return 0;
}

void CardInfo(){
  tft.setCursor(0, 20);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK);
  tft.printf("Size SD card: %lluMB\n", SD.cardSize() / (1024 * 1024));
  tft.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  tft.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));

  tft.print("Type SD card: ");
  //tft.println(SD.cardType());
  switch (SD.cardType()){
  case CARD_MMC:
    tft.println("MMC");
    break;
  case CARD_SD:
    tft.println("SDSC");
    break;
  case CARD_SDHC:
    tft.println("SDHC");
    break;
  default:
    tft.println("Unknown");
  }
  tft.setCursor(0, 0);
}
