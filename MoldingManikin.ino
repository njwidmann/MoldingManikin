#include <KalmanSmooth.h>

const int print_rate = 5; //print every
const int residual_monitor_pin = A2;

int j = 0;

float Q = 0.001;
float R = 0.1;

float residual = 0;
float damping = 0;
float MAX_DAMPING = 255;
float MIN_DAMPING = -255;
float RECOIL_TIME = 750000; //microsec
long recoil_start_time = 0;

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  setupAccelerometer();
  setupMagnetControl();
}

int updateResidual() {
  residual = MIN_DAMPING;
  /*residual = analogRead(residual_monitor_pin) / 1020.0 * (MAX_DAMPING * 2) - MAX_DAMPING;
  if(residual > MAX_DAMPING) residual = 255;
  if(residual < -MAX_DAMPING) residual = -255;
  */
  return residual;
  
}

void loop() {
  // put your main code here, to run repeatedly:
  updateAccelerometer();
  float z = getZ2();

  if(z > 11) {
    damping = MAX_DAMPING;
    recoil_start_time = micros();
  } else {
    updateResidual();
    float delta_t = micros() - recoil_start_time;
    damping = MAX_DAMPING - ((delta_t / RECOIL_TIME) * (MAX_DAMPING - residual));
    if(damping > MAX_DAMPING) { 
      damping = MAX_DAMPING; 
    } else if (damping < residual) {
      damping = residual;
    }
  }
  
  int pwm = round((damping/abs(damping))*255.0*pow(abs(damping)/MAX_DAMPING,3.0/11.0));
  
  if(pwm > 255) pwm = 255;
  if(pwm > 0) {
    setMagnetDown(pwm);
  } else {
    setMagnetUp(abs(pwm));
  }
  
  j++;
  if(j == print_rate) {
    sendDepth(z);
    //Serial.print(pwm/255.0*2);
    //Serial.print(",");
    //Serial.print(damping / MAX_DAMPING * 10);
    //Serial.print(",");
    //Serial.println(getZ2());
  
    j=0;
  }

  


  
}
