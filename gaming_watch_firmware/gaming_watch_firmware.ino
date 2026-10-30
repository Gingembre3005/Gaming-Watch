#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//screen stuff
#define  screenWidth 128
#define  screenHeight 64
#define  oledReset 1
#define  oledAddress 0x3c

int selection = 0;
int resetmenu = 1;
int gameMode = 0;


//Pong variables
int ballX = 64;
int ballY = 32; 

int ballSpeedX = 1;
int ballSpeedY = 1;

float playerY = 25;
float botY = 25;

Adafruit_SSD1306 screen(
  screenWidth,
  screenHeight,
  &Wire,
  oledReset
);

int interpret(int GPIO){ //apparently touchRead doesn't return 0 or 1 so we have to convert
int value = touchRead(GPIO);

if (value > 50){

  return 1;
}else{
  return 0;
}

}

void DrawMenu(int VSelect){
screen.clearDisplay();
screen.setTextColor(SSD1306_WHITE);
screen.setTextSize(1);

screen.setCursor(0,0);
screen.println("Menu");

if (VSelect == 1){
  screen.drawRect(0,17 ,35,12,SSD1306_WHITE);
};

screen.setCursor(0,20);
screen.println("  Pong");

screen.setCursor(0,35);
screen.println("  Made by: Gingembre");

screen.display();
};

void scrollmenu(){
  if(resetmenu == 1){
    if(interpret(1) == 1){
      selection += 1;
      resetmenu = 0;
    };
    if(interpret(2) == 1){
      selection -= 1;
      resetmenu = 0;
    };
  };
  if(resetmenu == 0 && interpret(1) == 0 && interpret(2) ==0){
    resetmenu =1;
  };
  if (selection > 1){
    selection = 1;

  };
  if (selection < 0){
    selection = 0;
  };
};

void MenuSelect(){

  if (selection == 1  interpret(3) == 1){
    gameMode = 1;
  };

};

void PongBot(){
  if (botY < ballY){
    botY += 0.5;
  }

  if (botY > ballY) {

    botY -= 0.5;
  }

  if (botY < 7.5){
    botY = 7.5;
  };

  if (botY > 56.5){
    botY = 56.5;
  };
};

void PongPlayer(){

  if (interpret(1) == 1) {
    playerY -= 2;
  };

  if (interpret(2) == 1){
    playerY += 2;
  };

  if (playerY < 7.5){
    playerY = 7.5;
  };

  if (playerY > 56.5){
    playerY = 56.5;
  };
};

void Pong(){
screen.clearDisplay();

PongPlayer();
PongBot();

ballX += ballSpeedX;
ballY += ballSpeedY;

if (ballY <= 0 || ballY >= 63){
 ballSpeedY = -ballSpeedY;
};

if (ballX <= 0 && (playerY-7.5)<ballY && ballY<(playerY+7.5) ){
  ballSpeedX = -ballSpeedX; 
}else if (ballX <= 0){
  screen.setCursor(0,20);
  screen.println("  You lost...");
};

if (ballX >= 125 && (botY-7.5)<ballY && ballY<(botY+7.5)){
  ballSpeedX = -ballSpeedX;

}else if (ballX >= 125){
  screen.setCursor(0,20);
  screen.println("  You won!!");
};

screen.fillRect(ballX, ballY, 3,3,SSD1306_WHITE);

screen.fillRect(4, playerY-7.5,4,15, SSD1306_WHITE);
screen.fillRect(120 , botY-7.5, 4,15, SSD1306_WHITE);

screen.display();
}



void setup(){
Wire.begin(7,8);
screen.begin(SSD1306_SWITCHCAPVCC, 0x3C);

DrawMenu(0);


}

void loop(){

if (gameMode == 0){
scrollmenu();
MenuSelect();
DrawMenu(selection);
};

if (gameMode == 1){
  Pong();
}



}


