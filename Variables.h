#define motor1 12 // definition of D12 pin as motor1 (pitch)
#define motor2 8 // definition of D8 pin as motor2 (roll)

#define period1 5000 // us
#define period2 15 // ms

// the rotation of a motor is determined by 2 parameters
typedef struct  {
  unsigned int time_high;
  int nImpulse;
} MotorParameters;

