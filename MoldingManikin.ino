#include <KalmanSmooth.h>

const int print_rate = 5; //print every
const int residual_monitor_pin = A1;

int j = 0;

float residual = 0;
float damping = 0;
float MAX_DAMPING = 255;
float MIN_DAMPING = -255;
float RECOIL_TIME = 750000; //microsec
long recoil_start_time = 0;
float RECOIL_START_DEPTH = 11;

void setup() {
  Serial.begin(9600);
  setupAccelerometer();
  setupMagnetControl();
}

int updateResidual() {
  residual = analogRead(residual_monitor_pin) / 1020.0 * (MAX_DAMPING * 2) - MAX_DAMPING;
  if(residual > MAX_DAMPING) residual = 255;
  if(residual < MIN_DAMPING) residual = -255;
  return residual;
}

void loop() {
  updateAccelerometer();
  float z = getZ2();

  if(z > RECOIL_START_DEPTH) {
    damping = MAX_DAMPING;
    recoil_start_time = micros();
  } else {
    updateResidual(); //get residual damping ("molding") from sliding potentiometer
    float delta_t = micros() - recoil_start_time;
    //vary damping force linearly over the span of RECOIL_TIME
    damping = MAX_DAMPING - ((delta_t / RECOIL_TIME) * (MAX_DAMPING - residual)); 
    if(damping > MAX_DAMPING) { 
      damping = MAX_DAMPING; 
    } else if (damping < residual) {
      damping = residual;
    }
  }

  //convert linear damping force magnitude to pwm signal (pwm has a x^(3/11) relationship with magnet force)
  int pwm = round((damping/abs(damping))*255.0*pow(abs(damping)/MAX_DAMPING,3.0/11.0));
  
  if(pwm > 255) pwm = 255;
  if(pwm > 0) {
    setMagnetDown(pwm);
  } else {
    setMagnetUp(abs(pwm));
  }
  
  j++;
  if(j == print_rate) {
    sendDepth(z); //send depth info over bluetooth
    j=0;
  }

  


  
}
