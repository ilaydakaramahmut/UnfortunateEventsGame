
#define IR_RECEIVE_PIN 2

#define BUTTON_UP 24
#define BUTTON_LEFT 8
#define BUTTON_RIGHT 90
#define BUTTON_DOWN 82
#define BUTTON_OK 28

#include <SPI.h>
#include <IRremote.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

int flag1 = 0;
int flag2 = 0;
int flag3 = 0;
int flag4 = 0;
int flag5 = 0;
int flag6 = 0;

int command;
int points = 0;

int xposition = random(1, 80);
int yposition = random(13, 45);

int xposition_gain = random(1, 80);
int yposition_gain = random(13, 45);

int xposition_lose = random(1, 80);
int yposition_lose = random(13, 45);

void setup() {
  IrReceiver.begin(IR_RECEIVE_PIN);
  Serial.begin(115200);
  display.begin();
  display.setContrast(60);
  display.clearDisplay();

}

void loop() {
  game_startup();//a line and our character
  points_display();//points
  game_setup(); //controlling your characters movements
  allies_enemies();//characters that make you gain or lose points
  gain_change(); //whether you gain or lose points

}

void game_startup() {

  display.drawLine(0, 10, 84, 10, BLACK); //drawLine(x1, y1, x2, y2, color),under the POINTS
  display.drawRect(xposition, yposition, 3, 3, BLACK); //our character that we can control
  display.display();

}

void game_setup() {
  if (IrReceiver.decode()) {
    IrReceiver.resume();
    command = IrReceiver.decodedIRData.command;

    switch (command) {
      case BUTTON_UP: {
          yposition = yposition - 1;
          character_refresh();
          break;
        }

      case BUTTON_RIGHT: {
          xposition = xposition + 1;
          character_refresh();
          break;
        }

      case BUTTON_LEFT: {
          xposition = xposition - 1;
          character_refresh();
          break;
        }

      case BUTTON_DOWN: {
          yposition = yposition + 1;
          character_refresh();
          break;
        }

      default: {
          // do nothing
        }

    }

  }

}

void gain_change() {

  if ((xposition == xposition_gain) && (yposition == yposition_gain)) {

    while (flag1 == 0) {
      points=points+random(1,10);
      points_refresh();//to see points more clearly
      points_display();
      delay(500);
      flag1++;
    }
  }



  if ((xposition == xposition_lose) && (yposition == yposition_lose)) {

    while (flag2 == 0) {
      points=points-random(1,10);
      points_refresh();//to see points more clearly
      points_display();
      delay(500);
      flag2++;
    }
  }

}

void allies_enemies() {

  do
  {

    display.fillRect(xposition_gain, yposition_gain, 3, 3, BLACK); //character to gain points
    flag3++;
  } while (flag3 == 0);

  do
  {
    display.fillRect(xposition_lose, yposition_lose, 3, 3, BLACK); //character to lose points
    flag4++;
  } while (flag4 == 0);

  if (flag1 != 0) {
    display.fillRect(xposition_gain, yposition_gain, 3, 3, WHITE);
    flag5 = 1;
  }

  if (flag2 != 0) {
    display.fillRect(xposition_lose, yposition_lose, 3, 3, WHITE);
    flag6 = 1;
  }

  if (flag5 == 1 && flag6 == 1) {
    win_lose();
    game_reset();
  }

}


void points_display() {
  points_refresh();
  display.setCursor(0, 0);
  display.print("POINTS:");
  display.print(points);
  display.display();

}

void game_reset() {
  flag1 = 0;
  flag2 = 0;
  flag3 = 0;
  flag4 = 0;
  flag5 = 0;
  flag6 = 0;

  points = 0;

  xposition = random(1, 80);
  yposition = random(13, 45);


  delay(500);

}


void points_refresh() {

  display.fillRect(0, 0, 84, 9, WHITE);
  display.display();

}

void character_refresh() {

  if (command == BUTTON_UP) {
    for (int i = 0; i < yposition; i++) {
      display.fillRect((xposition - 1), (yposition - i), 4, 6, WHITE);
      display.display();
    }
  }

  if (command == BUTTON_RIGHT) {
    for (int i = 0; i < xposition; i++) {
      display.fillRect((xposition - i), (yposition - 1), 4, 6, WHITE);
      display.display();
    }
  }

  if (command == BUTTON_LEFT) {
    for (int i = 0; i < xposition; i++) {
      display.fillRect((xposition + i), (yposition - 1), 4, 6, WHITE);
      display.display();
    }
  }

  if (command == BUTTON_DOWN) {
    for (int i = 0; i < yposition; i++) {
      display.fillRect((xposition - 1), (yposition - i), 4, 6, WHITE);
      display.display();
    }
  }

}

void win_lose(){
  points_refresh();
  display.setCursor(0, 0);

  if(points>0){
    display.print("WINNER");
    display.display();
    delay(500);
  }

  if(points<=0){
    display.print("LOSER");
    display.display();
    delay(500);
  }

  
}
