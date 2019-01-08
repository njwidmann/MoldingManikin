#include <KalmanSmooth.h>

const int print_rate = 5; //send data every 5 iterations

const bool ADJUSTABLE_MOLDING = false;
const int residual_monitor_pin = A1;

int j = 0;

float MAX_DAMPING = 255;
float MIN_DAMPING = -255;
float damping = 0;
float recoil_time = 750; //microsec
long recoil_start_time = 0;
float RECOIL_START_DEPTH = 11;
float residual = MIN_DAMPING;


void setup() {
  Serial.begin(9600);
  setupAccelerometer();
  setupMagnetControl();
  setupVentilations();
}

/**
 * residual is the amount of molding of the manikin. If ADJUSTABLE_MOLDING is set to true, a potentiometer on residual_monitor_pin
 * can be used to set permanent magnetic resistance force opposing spring. 255 is max molding. -255 is min. During recoil, the magnet
 * force varies between 255 (MAX_DAMMPING) and residual (default is MIN_DAMPING=-255). At the beginning of recoil, magnet force opposes spring
 * force. At the end, it adds to spring force.
 */
int updateResidual() {
  if(ADJUSTABLE_MOLDING) {
    residual = analogRead(residual_monitor_pin) / 1020.0 * (MAX_DAMPING * 2) - MAX_DAMPING;
    if(residual > MAX_DAMPING) residual = 255;
    if(residual < MIN_DAMPING) residual = -255;
  } else {
    residual = MIN_DAMPING;    
  }
  return residual;
}

/**
 * Recoil time is set in the android app. Longer recoil time means more damping. Recoil time is the time it takes magnet force
 * to vary between MAX_DAMPING (255) and residual (Default is MIN_DAMPING = -255)
 */
void updateRecoilTime() {
  if (Serial.available() > 0) {
      int recv = Serial.read(); //check for updates from android over bluetooth

      recoil_time = 20*recv;
  }
  
}


void loop() {
  updateAccelerometer();
  float z = getZ2();

  if(z > RECOIL_START_DEPTH) {
    damping = MAX_DAMPING;
    recoil_start_time = millis();
  } else {
    updateResidual(); //get residual damping ("molding") from sliding potentiometer
    updateRecoilTime(); //get recoil time from android
    float delta_t = millis() - recoil_start_time;
    //vary damping force linearly over the span of RECOIL_TIME
    damping = MAX_DAMPING - ((delta_t / recoil_time) * (MAX_DAMPING - residual)); 
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
    sendDepthAndVentPressure(z, readVentilationVelocity()); //send depth info over bluetooth
    //Serial.println(readVentilationVelocity());
    j=0;
  }
  
}
