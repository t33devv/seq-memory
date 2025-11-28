#include <Arduino.h>

const int BTN_PINS[] = {2, 3, 4, 5};
const int LED_PINS[] = {10, 11, 12, 13};
const int NUM_PINS = 4;
const int BUZZER = 8;
const int MAX_SEQ_LEN = 9;
int sequenceArr[MAX_SEQ_LEN];
int seqLen = 0;

void turnOffAllLEDs();
void startNewGame();
void addRandomStep();
void showSequence();
bool isPlayerRight();
int waitForButtonPress();
void gameOverAnimation();
void winningAnimation();

void setup()
{
  Serial.begin(9600);
  for (int i = 0; i < NUM_PINS; i++)
  {
    pinMode(BTN_PINS[i], INPUT_PULLUP);
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }
  pinMode(8, OUTPUT);

  randomSeed(analogRead(A0));
}

void loop()
{
  showSequence();
  turnOffAllLEDs();
  bool correct = isPlayerRight();
  turnOffAllLEDs();
  delay(200);
  if (correct)
  {
    if (seqLen < MAX_SEQ_LEN)
    {
      addRandomStep();
    }
    else
    {
      winningAnimation();
    }
  }
  else
  {
    gameOverAnimation();
    startNewGame();
  }
}

void turnOffAllLEDs()
{
  for (int i = 0; i < NUM_PINS; i++)
  {
    digitalWrite(LED_PINS[i], LOW);
  }
}

void startNewGame()
{
  seqLen = 0;
  turnOffAllLEDs();
  addRandomStep();
}

void addRandomStep()
{
  if (seqLen >= MAX_SEQ_LEN)
    return;
  int newStep = random(0, NUM_PINS);
  sequenceArr[seqLen] = newStep;
  seqLen++;
  Serial.print("New step: ");
  Serial.println(newStep);
}

void showSequence()
{
  turnOffAllLEDs();
  for (int i = 0; i < seqLen; i++)
  {
    delay(150);
    digitalWrite(LED_PINS[sequenceArr[i]], HIGH);
    switch (sequenceArr[i])
    {
    case 0:
      tone(BUZZER, 1000);
      break;
    case 1:
      tone(BUZZER, 1300);
      break;
    case 2:
      tone(BUZZER, 1600);
      break;
    case 3:
      tone(BUZZER, 1900);
      break;
    }
    delay(150);
    digitalWrite(LED_PINS[sequenceArr[i]], LOW);
    noTone(BUZZER);
    delay(200);
  }
  turnOffAllLEDs();
}

bool isPlayerRight()
{
  for (int i = 0; i < seqLen; i++)
  {
    if (waitForButtonPress() != sequenceArr[i])
    {
      return false;
    }
  }
  return true;
}

int waitForButtonPress()
{
  turnOffAllLEDs();

  unsigned long releaseStart = millis();
  while (millis() - releaseStart < 200)
  {
    bool allReleased = true;
    for (int i = 0; i < NUM_PINS; i++)
    {
      if (digitalRead(BTN_PINS[i]) == HIGH)
      {
        allReleased = false;
        break;
      }
    }
    if (allReleased)
      break;
    delay(10);
  }

  int pressedIndex = -1;
  while (pressedIndex == -1)
  {
    for (int i = 0; i < NUM_PINS; i++)
    {
      if (digitalRead(BTN_PINS[i]) == HIGH)
      {
        delay(50);
        if (digitalRead(BTN_PINS[i]) == HIGH)
        {
          pressedIndex = i;
          switch (pressedIndex)
          {
          case 0:
            tone(BUZZER, 1000);
            break;
          case 1:
            tone(BUZZER, 1300);
            break;
          case 2:
            tone(BUZZER, 1600);
            break;
          case 3:
            tone(BUZZER, 1900);
            break;
          }
          break;
        }
      }
    }
    delay(5);
  }

  while (digitalRead(BTN_PINS[pressedIndex]) == HIGH)
  {
    for (int i = 0; i < NUM_PINS; i++)
    {
      digitalWrite(LED_PINS[i], i == pressedIndex ? HIGH : LOW);
    }
    delay(10);
  }

  turnOffAllLEDs();
  noTone(BUZZER);
  delay(50);
  return pressedIndex;
}

void gameOverAnimation()
{
  for (int k = 0; k < 4; k++)
  {
    for (int i = 0; i < NUM_PINS; i++)
    {
      digitalWrite(LED_PINS[i], HIGH);
      tone(BUZZER, 2000);
    }
    delay(100);
    turnOffAllLEDs();
    noTone(BUZZER);
    delay(150);
  }
  turnOffAllLEDs();
  delay(300);
}

void winningAnimation()
{
  for (int k = 0; k < 3; k++)
  {
    for (int i = 0; i < NUM_PINS; i++)
    {
      digitalWrite(LED_PINS[i], HIGH);
      tone(BUZZER, (i + 3) * 350);
      delay(150);
      turnOffAllLEDs();
      noTone(BUZZER);
    }
  }
  delay(300);
  startNewGame();
}