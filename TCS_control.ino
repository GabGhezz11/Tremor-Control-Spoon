#include "Functions.c"
#include <Wire.h>

// Global variables
const int MPU = 0x68; // MPU6050 I2C address (pag 45 datasheet)
float acc_X, acc_Y, acc_Z; // acceleration
float gyro_X, gyro_Y, gyro_Z, gyro_XOffset, gyro_YOffset, gyro_ZOffset, gyro_X_cal, gyro_Y_cal, gyro_Z_cal; // angular velocity before and after calibration and offset
float phi = 0; // angle x axis (ROLL)
float theta = 0; // angle y axis (PITCH)
float phi_acc, theta_acc, phi_prec, theta_prec; // variables for the implementation of the complementary filter
float square_sum1;
float rad_to_deg = 180 / 3.141592654; // to convert from rad to deg
float fs = 99.5; // sampling frequency
float T = 1.0 / fs; // sampling period

void setup() {

  pinMode(motor1,OUTPUT);
  pinMode(motor2,OUTPUT);

  Serial.begin(2000000); // serial initialization
  init_MPU(); // MPU6050 initialization
  
  position90Deg(); // motors at 90°
  
  calibration(); // sensor calibration

  // Interrupt Setup
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // set entire TCCR1B register to 0
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // prescaler 1024
  TCNT1  = 0; // Initialize counter value to 0
  OCR1A = 156; // set OCR1A Register to 156
  TIMSK1 |= (1 << OCIE1A); // Set interrupt on OCR1A

  interrupts(); // Enable Interrupt

}

void loop() {

}

ISR(TIMER1_COMPA_vect) { // ISR associated to timer 1 through COMPA register
  
  interrupts();
  
  readFrom_MPU(); // read from sensor

  noInterrupts(); // disable interrupts

  // Subtraction of the offset
  gyro_X_cal = gyro_X - gyro_XOffset;
  gyro_Y_cal = gyro_Y - gyro_YOffset;
  gyro_Z_cal = gyro_Z - gyro_ZOffset;

  //Complementary Filter
  phi_prec = phi;
  theta_prec = theta;
  square_sum1 = sqrt((acc_Y * acc_Y) + (acc_Z * acc_Z));
  phi_acc = atan2(acc_Y, acc_Z);
  theta_acc = atan2(-acc_X, square_sum1);

  // values of coefficient found from literature
  phi = 0.04 * phi_acc * rad_to_deg + 0.96 * (gyro_X_cal * T + phi_prec);
  theta = 0.04 * theta_acc * rad_to_deg + 0.96 * (gyro_Y_cal * T + theta_prec);

  // Rotation of the motors with limit of +/-30°
  if (theta >= 30 && phi >= 30) {
    // do nothing
  }
  else if (theta >= 30) {
    MP1 = stoppedMotor();
    MP2 = rotationParameters(90 + phi_prec, 90 + phi);
    // Servo commands
    rotateMotors(MP1, MP2);
  }
  else if (phi >= 30) {
    MP1 = rotationParameters(90 + theta_prec, 90 + theta);
    MP2 = stoppedMotor();
    // Servo commands
    rotateMotors(MP1, MP2);
  }
  else if (theta <= -30 && phi <= -30) {
    // do nothing
  }
  else if (theta <= -30) {
    MP1 = stoppedMotor();
    MP2 = rotationParameters(90 + phi_prec, 90 + phi);
    // Servo commands
    rotateMotors(MP1, MP2);
  }
  else if (phi <= -30) {
    MP1 = rotationParameters(90 + theta_prec, 90 + theta);
    MP2 = stoppedMotor();
    // Servo commands
    rotateMotors(MP1, MP2);
  }
  else {
    MP1 = rotationParameters(90 + theta_prec, 90 + theta);
    MP2 = rotationParameters(90 + phi_prec, 90 + phi);
    // Servo commands
    rotateMotors(MP1, MP2);
  }

  interrupts(); // enable interrupts
}

// ---- Sensor initialization ---- 
void init_MPU() {
  Wire.begin(); // communication inizialization
  Wire.setClock(400000); // I2C frequency 400 kHz

  // Reset
  Wire.beginTransmission(MPU); // beginning of communication with MPU6050 // MPU=0x68
  Wire.write(0x6B); // call to register 6B
  Wire.write(0b1000000); // reset (DEVICE_RESET)
  Wire.endTransmission(true); // end of transmission
  delay(100);
  Wire.beginTransmission(MPU); // beginning of communication with MPU6050 // MPU=0x68
  Wire.write(0x68); // call to register 68
  Wire.write(0b0000111); // reset of gyroscope, accelerometer and temperature sensor (GYRO_RESET, ACCEL_RESET, TEMP_RESET)
  Wire.endTransmission(true); // end of transmission
  delay(100);
  
  // Configuration full-scale range accelerometer
  Wire.beginTransmission(MPU);
  Wire.write(0x1C); // call to register ACCEL_CONFIG  (1C hex)
  Wire.write(0b00000000); // set range +/-2g
  
  Wire.endTransmission(true);
  // Configuration full-scale range gyroscope
  Wire.beginTransmission(MPU);
  Wire.write(0x1B); // call to register GYRO_CONFIG (1B hex)
  Wire.write(0b00010000); // Set range +/-500°/s

  Wire.endTransmission(true);
  delay(20);
}

// ---- Acquisition from axes X,Y,Z of MPU6050 ----
void readFrom_MPU() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // start reading from register 0x3B (ACCEL_XOUT_H) to get data from the accelerometers 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // reading of the 6 registers, since the data from 1 axis is placed on 2 registers (16 bytes) (ACCEL_XOUT_H, ACCEL_XIN_H)
  while (Wire.available() < 6);
  // sensitivity: 2^(16)/2^2
  acc_X = (Wire.read() << 8 | Wire.read()) / 16384.0; // acceleration X axis 
  acc_Y = (Wire.read() << 8 | Wire.read()) / 16384.0; // acceleration Y axis 
  acc_Z = (Wire.read() << 8 | Wire.read()) / 16384.0; // acceleration Z axis

  Wire.beginTransmission(MPU);
  Wire.write(0x43); // start reading from register 0x43 (GYRO_XOUT_H) to get data from the gyroscopes
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // reading of the 6 registers, since the data from 1 axis is placed on 2 registers (16 bytes) (GYRO_XOUT_H, GYRO_XIN_H)
  // sensitivity: 2^(16)/(2*500)
  while (Wire.available() < 6);
  gyro_X = (Wire.read() << 8 | Wire.read()) / 65.5; // angular velocity X axis
  gyro_Y = (Wire.read() << 8 | Wire.read()) / 65.5; // angular velocity Y axis
  gyro_Z = (Wire.read() << 8 | Wire.read()) / 65.5; // angular velocity Z axis

}

// ---- Gyroscopes calibration ----
void calibration(){
  for (int i = 0; i < 1000; i++) { //take 1000 measures
    readFrom_MPU(); // read from sensor

    gyro_XOffset += gyro_X;
    gyro_YOffset += gyro_Y;
    gyro_ZOffset += gyro_Z;
  }

  // computation of the mean
  gyro_XOffset /= 1000;
  gyro_YOffset /= 1000;
  gyro_ZOffset /= 1000;
}