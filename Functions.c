#include <Arduino.h>
#include "Variables.h"

MotorParameters MP1, MP2;
unsigned int time_high_diff; // difference between the two time_high

// ---- Rotation of the motors ----
void rotateMotors(MotorParameters MP1, MotorParameters MP2){

	unsigned int time_low1 = period1 - MP1.time_high; 
  unsigned int time_low2 = period1 - MP2.time_high; 
	int i = 0; // variable to count the impulses
	
	// If the number of impulses to be given in input to the second motor is greater
  if (MP1.nImpulse < MP2.nImpulse) {

    while (i < MP1.nImpulse) {
      digitalWrite(motor1, HIGH);
      digitalWrite(motor2, HIGH);
      
      // If the time high to be given in input to the second motor is greater than or equal to the one of the first
      if (MP1.time_high <= MP2.time_high) {
        delayMicroseconds(MP1.time_high);
        digitalWrite(motor1, LOW);
        time_high_diff = MP2.time_high - MP1.time_high;
        delayMicroseconds(time_high_diff);
        digitalWrite(motor2, LOW);
        delayMicroseconds(time_low2);
        delay(period2);
        i++;
      }
      
      // If the time high to be given in input to the first motor is greater
      else {
        delayMicroseconds(MP2.time_high);
        digitalWrite(motor2, LOW);
        time_high_diff = MP1.time_high - MP2.time_high;
        delayMicroseconds(time_high_diff);
        digitalWrite(motor1, LOW);
        delayMicroseconds(time_low1);
        delay(period2);
        i++;
      }
    }

    while (i >= MP1.nImpulse && i < MP2.nImpulse) {
      digitalWrite(motor2, HIGH);
      delayMicroseconds(MP2.time_high);
      digitalWrite(motor2, LOW);
      delay(period2);
      delayMicroseconds(time_low2);
      i++;
    }

  }

 // If the number of impulses to be given in input to the first motor is greater
 if (MP2.nImpulse < MP1.nImpulse) {

    while (i < MP2.nImpulse) {
      digitalWrite(motor1, HIGH);
      digitalWrite(motor2, HIGH);

      // If the time high to be given in input to the second motor is greater than or equal to the one of the first
      if (MP1.time_high <= MP2.time_high) {
        delayMicroseconds(MP1.time_high);
        digitalWrite(motor1, LOW);
        time_high_diff = MP2.time_high - MP1.time_high;
        delayMicroseconds(time_high_diff);
        digitalWrite(motor2, LOW);
        delayMicroseconds(time_low2);
        delay(period2);
        i++;
      }

      // If the time high to be given in input to the first motor is greater
      else {
        delayMicroseconds(MP2.time_high);
        digitalWrite(motor2, LOW);
        time_high_diff = MP1.time_high - MP2.time_high;
        delayMicroseconds(time_high_diff);
        digitalWrite(motor1, LOW);
        delayMicroseconds(time_low1);
        delay(period2);
        i++;
      }
    }

    while (i >= MP2.nImpulse && i < MP1.nImpulse) {
      digitalWrite(motor1, HIGH);
      delayMicroseconds(MP1.time_high);
      digitalWrite(motor1, LOW);
      delay(period2);
      delayMicroseconds(time_low1);
      i++;
    }

  }

  // If the number of impulses to be input to the first motor is equal to that of the second
  if (MP2.nImpulse == MP1.nImpulse) {

    while (i < MP2.nImpulse) { 
      digitalWrite(motor1, HIGH);
      digitalWrite(motor2, HIGH);

      // If the time high to be given in input to the second motor is greater than or equal to the one of the first
      if (MP1.time_high <= MP2.time_high) {
        delayMicroseconds(MP1.time_high);
        digitalWrite(motor1, LOW);
        time_high_diff = MP2.time_high - MP1.time_high;
        delayMicroseconds(time_high_diff);
        digitalWrite(motor2, LOW);
        delayMicroseconds(time_low2);
        delay(period2);
        i++;
      }

      // If the time high to be given in input to the first motor is greater
      else {
        delayMicroseconds(MP2.time_high);
        digitalWrite(motor2, LOW);
        time_high_diff = MP1.time_high - MP2.time_high;
        delayMicroseconds(time_high_diff); 
        digitalWrite(motor1, LOW);
        delayMicroseconds(time_low1);
        delay(period2);
        i++;
      }
    }
  }
}

// ---- Bring the motors at 90° ----
void position90Deg(){
  MotorParameters MP;
	MP.time_high = 1500; // time high for 90°
	MP.nImpulse = 9; // number of pulses to rotate for at least 90°
	rotateMotors(MP, MP);
}

// ---- Calculate the parameters for the rotation of the motors ----
MotorParameters rotationParameters(int theta0, int theta){
  MotorParameters MP;
	MP.time_high = ceil(1400-((70/9)*theta));
	MP.nImpulse = ceil((0.12*abs(theta0-theta)/60)/0.02);

  return MP;
}

// ---- Not to move the motors ----
MotorParameters stoppedMotor() {
  MotorParameters MP;
	MP.time_high = 0;
	MP.nImpulse = 0;

  return MP;
}