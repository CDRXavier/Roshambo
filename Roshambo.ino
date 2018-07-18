#include <Arduboy.h>
Arduboy arduboy;

bool nextRound = false;
int8_t choice = -1;
int8_t computer = -1;
bool released = true;
bool guessing = true;

//to allocate enough room to accomdate the longest possible line to print
String dispString[5] = {"                     ","                     ","                     ","                     ","                     "};
int8_t dispLength = 0;
bool drawn;
void dispPush(String string) {
  if (dispLength > 3) {
    for (uint8_t i = 1; i < 5; i++)
      dispString[i - 1] = dispString[i];
    dispLength--;
  }
  dispLength++;
  dispString[dispLength] = string;
}
void dispPrint() {
  consoleWindow();
  for (uint8_t d = 0; d <= dispLength; d++) {
    arduboy.setCursor(1, 12 + d * 10);
    arduboy.print(dispString[d]);
  }
  arduboy.display();
}
void dispClear() {
  for (uint8_t c = 0; c <= dispLength; c++) {
    dispString[c] = "                     ";
  }
  dispLength = 0;
}
void consoleWindow() {
  arduboy.setCursor(2, 2);
  arduboy.print(F("Roshambo.exe"));
  arduboy.drawRect(0,0,128,64,WHITE);
  arduboy.drawLine(0,10,128,10,WHITE);
  arduboy.drawLine(106,1,106,9,WHITE);
  arduboy.drawLine(108,7,115,7,WHITE);
  arduboy.drawLine(117,1,117,9,WHITE);
  arduboy.drawLine(118,1,126,9,WHITE);
  arduboy.drawLine(118,9,126,1,WHITE);
}
void dispUpdate(String string) {
  dispString[dispLength] = string;
}
void setup() {
  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.setFrameRate(30);
  arduboy.initRandomSeed();
  arduboy.clear();
  arduboy.setTextSize(1);
  arduboy.display();
  drawn = true;
}
void loop() {
  if (!arduboy.nextFrame()) return;
  arduboy.clear();
  if (guessing) {
    if (arduboy.pressed(UP_BUTTON) && released) {
      released = false;
      choice++;
    }
    if (arduboy.pressed(DOWN_BUTTON) && released) {
      released = false;
      choice--;
    }
    if (choice > 2) choice = 0;
    if (choice < 0) choice = 2;
    if (!drawn) {
      drawn = true;
      switch (choice) {
        case 0: dispPush(F("Your turn: rock")); break;
        case 1: dispPush(F("Your turn: paper")); break;
        case 2: dispPush(F("Your turn: scissor")); break;
      }
    } else {
      switch (choice) {
        case 0: dispUpdate(F("Your turn: rock")); break;
        case 1: dispUpdate(F("Your turn: paper")); break;
        case 2: dispUpdate(F("Your turn: scissor")); break;
      }
    }
    if (arduboy.pressed(A_BUTTON) && released) {
      released = guessing = false;
      computer = random(0, 3);
      switch (computer) {
        case 0: dispPush(F("Arduboy plays rock"));
          dispPrint();
          delay(200);
          if (choice == 1) dispPush(F("win"));
          if (choice == 0) dispPush(F("draw"));
          if (choice == 2) dispPush(F("lose")); break;
        case 1: dispPush(F("Arduboy plays paper"));
          dispPrint();
          delay(200);
          if (choice == 2) dispPush(F("win"));
          if (choice == 1) dispPush(F("draw"));
          if (choice == 0) dispPush(F("lose")); break;
        case 2: dispPush(F("Arduboy plays scissor"));
          dispPrint();
          delay(200);
          if (choice == 0) dispPush(F("win"));
          if (choice == 2) dispPush(F("draw"));
          if (choice == 1) dispPush(F("lose")); break;
      }
      drawn = false;
    }
  } else {
    if ((arduboy.pressed(UP_BUTTON) || arduboy.pressed(DOWN_BUTTON)) && released) {
      released = false;
      nextRound = !nextRound;
    }
    if (!drawn) {
      drawn = true;
      dispPush(F("Play again? Y/N"));
      dispPush(nextRound ? F("Y") : F("N"));
    } else dispUpdate(nextRound ? F("Y") : F("N"));

    if (arduboy.pressed(A_BUTTON) && released) {
      released = false;
      if (nextRound) {
        computer = choice = 0;
        guessing = true;
        drawn = false;
      } else {
        arduboy.clear();
        consoleWindow();
        arduboy.display();
        exit(0);
      }
    }
  }
  dispPrint();
  if (arduboy.notPressed(A_BUTTON | B_BUTTON | UP_BUTTON | DOWN_BUTTON)) released = true;
}
