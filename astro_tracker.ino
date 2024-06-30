#include <AccelStepper.h>

// #define DEBUG
#define MotorInterfaceType 4

typedef enum {
  STOP,
  TRACK,
  REWIND
} OperationMode;

// STEPPER MOTOR CONSTANTS (28BYJ-48)
const int STEPS_PER_REVOLUTION = 2048;  // 2048 steps to make one full revolution

// MECHANICAL CONSTANTS
const double SIDEREAL_TIME_SEC = 86164.0905;  // time for a complete rotation of Earth
const double THREAD_PITCH_MM = 1.25;          // thread pitch is 1.25 mm
const double BASE_LENGTH_MM = 295.0;          // 29.5 cm from hinge to thread center

// CALCULATIONS
const double SIDEREAL_RAD_SEC = (2 * PI) / SIDEREAL_TIME_SEC;                      // one-second Earth rotation value expressed in radiant degrees
const double LINEAR_MOVEMENT_MM_MIN = (BASE_LENGTH_MM) * (SIDEREAL_RAD_SEC * 60);  // linear movement of the rod in millimeters in a minute
const double ROTATIONS_PER_MINUTE = LINEAR_MOVEMENT_MM_MIN / THREAD_PITCH_MM;      // computes how many revolutions the rod does in a minute
const double STEPS_PER_SEC = (ROTATIONS_PER_MINUTE * STEPS_PER_REVOLUTION) / 60;   // computes required steps in one second to match required rotation

// CHECKING MAX DURATION OF TRACKING OPERATIONS
const double USABLE_ROD_LENGTH_MM = 300;                                        // usable rod length from starting position to end
const double MAX_DURATION_MIN = USABLE_ROD_LENGTH_MM / LINEAR_MOVEMENT_MM_MIN;  // max amount of minutes of tracking time

AccelStepper motor(MotorInterfaceType, 2, 4, 3, 5);
OperationMode opMode = OperationMode::STOP;
int buttonState;
int resetButtonState;

OperationMode getOperationMode() {
  if (buttonState < 100) {
    return OperationMode::TRACK;
  } else if (buttonState > 900) {
    return OperationMode::REWIND;
  } else {
    return opMode;
  }
}

double getSpeed() {
  if (opMode == OperationMode::STOP)
    return 0;
  else if (opMode == OperationMode::TRACK)
    return -STEPS_PER_SEC;
  return motor.maxSpeed();
}

void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  motor.setMaxSpeed(700);

#ifdef DEBUG
  Serial.begin(9600);
#endif
}

void loop() {
  buttonState = analogRead(A0);
  resetButtonState = analogRead(A1);

#ifdef DEBUG
  Serial.print("buttonState = ");
  Serial.print(buttonState);
  Serial.print(" | ");
  Serial.print("resetButtonState = ");
  Serial.print(resetButtonState);
  Serial.print("\n");
#endif

  OperationMode newOpMode = getOperationMode();

  if (newOpMode != opMode) {
    opMode = newOpMode;
  }

  if (resetButtonState < 50) {
    opMode = OperationMode::STOP;
  }

  motor.setSpeed(getSpeed());
  motor.runSpeed();
}