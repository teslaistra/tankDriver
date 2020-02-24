#include <Arduino.h>
#include <L298N.h>
#include "MegunoLink.h"
#include "CommandHandler.h"

#define EN 9
#define IN1 3
#define IN2 4
#define IN3 5
#define IN4 6

#define TURN_LEFT 4
#define TURN_RIGHT 3
#define STOP 2
#define FORWARD 1
#define BACKWARD 0
 
CommandHandler<> SerialCommandHandler;

L298N motorLeft(EN, IN1, IN2);
L298N motorRight(EN, IN4, IN3);
int way = STOP;
int time = 0;
uint32_t setTime;

void Cmd_Move(CommandParameter &Parameters)
{
  way = Parameters.NextParameterAsInteger(way);
  time = Parameters.NextParameterAsInteger(time);
  setTime = millis();
}

void Cmd_Unknown()
{
  Serial.println(F("I don't understand"));
}

void setup() {
  SerialCommandHandler.AddCommand(F("Move"), Cmd_Move);
  SerialCommandHandler.SetDefaultHandler(Cmd_Unknown);

  Serial.begin(9600);
  motorLeft.setSpeed(255);
  motorRight.setSpeed(255);
}

void loop() {
  SerialCommandHandler.Process();

  uint32_t uNow = millis();
  if (way != STOP) {
    if (uNow < setTime + (uint32_t)time ) {
      switch (way) {
        case STOP:
          motorLeft.stop();
          motorRight.stop();
          break;
        case FORWARD:
          motorLeft.forward();
          motorRight.forward();
          break;
        case BACKWARD:
          motorLeft.backward();
          motorRight.backward();
          break;
        case TURN_RIGHT:
          motorLeft.forward();
          motorRight.backward();
          break;
        case TURN_LEFT:
          motorLeft.backward();
          motorRight.forward();
          break;
        default:
          break;
      }
    } else {
      way = STOP;
      motorRight.stop();
      motorLeft.stop();
    }
  }
}