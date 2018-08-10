#include <KalmanSmooth.h>

const int sample_size = 1;
const int print_rate = 5; //print every
const int voltage_monitor_pin = A3;

int k = 0;
int j = 0;
float v_sum = 0;
float z_sum = 0;
float max_v = 0;
float max_z = 0;

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  setupAccelerometer();
  setupMagnetControl();
}


void loop() {
  // put your main code here, to run repeatedly:
  updateAccelerometer();
  float v = getV();
  float z = getZ();
  //Serial.println(velocity);

  if(v < max_v) {
    max_v = v;
  }

  if(z > max_z) {
    max_z = z;
  }

  k++;
  v_sum += v;
  z_sum += z;
  int pwm = 0;
  if(k >= sample_size) {
    k = 0;
    float v_avg = v_sum / sample_size;
    float z_avg = z_sum / sample_size;
    v_sum = 0;
    z_sum = 0;

    if(v_avg < 0 && z_avg > max_z * 0.15) {
      
      pwm = round(255 * (v_avg / -0.2));
      if(pwm > 255) pwm = 255;
  
      setMagnetDown(pwm);
      delay(1);
      
    } else {
      setMagnetOff();
    }
    
  }

  
  j++;
  if(j == print_rate) {
    Serial.print(getV());
    Serial.print(",");
    Serial.print(getZ());
    Serial.print(",");
    Serial.println(pwm / 255.0);
  
    j=0;
  }
  

  
}
