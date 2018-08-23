const int PIN_a = 3;
const int PIN_b = 5;

const int DIRECTION_OFF = 0; 
const int DIRECTION_UP = 1;
const int DIRECTION_DOWN = 2;

void setupMagnetControl() {
  pinMode(PIN_a, OUTPUT);
  pinMode(PIN_b, OUTPUT);
}

void setMagnetDown(int pwm) {
  setMagnet(DIRECTION_DOWN, pwm);
}

void setMagnetUp(int pwm) {
  setMagnet(DIRECTION_UP, pwm);
}

void setMagnetOff() {
  setMagnet(DIRECTION_OFF, 0);
}

void setMagnet(int direction, int pwm) {
  if(direction == DIRECTION_DOWN) {
    analogWrite(PIN_a, pwm);
    digitalWrite(PIN_b, LOW);
  } else if(direction == DIRECTION_UP) {
    digitalWrite(PIN_a, LOW);
    analogWrite(PIN_b, pwm);
  } else {
    digitalWrite(PIN_a, LOW);
    digitalWrite(PIN_b, LOW);
  }
}

