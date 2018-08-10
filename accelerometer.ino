#include <KalmanSmooth.h>

const int zInput = A0;
const int aInput = A1;
 
// Take multiple samples to reduce noise
const int initialCalibrationSampleSize = 100;
const int sampleSize = 10;
float z0 = 0;
float a0 = 0;
float z_smoothed = 0;
float v1_smoothed = 0;
float v2_smoothed = 0;
float v_smoothed = 0;
float a_smoothed = 0;
float Q_z = 0.001;
float R_z = 0.1;
float Q_v = 0.001;
float R_v = 0.1;
float Q_a = 0.001;
float R_a = 0.1;
int z_log_size = 5;
float z_log[5] = {0,0,0,0,0};

float v1 = 0;
float v2 = 0;
float v2_max = 1;
int v2_log_size = 10;
float v2_log[10];
int v2_i = 0;


KalmanSmooth kalman_filter_z(Q_z,R_z,1);
KalmanSmooth kalman_filter_v1(Q_v,R_v,0);
KalmanSmooth kalman_filter_v2(Q_v,R_v,0);
KalmanSmooth kalman_filter_v(Q_v,R_v,0);
KalmanSmooth kalman_filter_a(Q_a,R_a,2);

void setupAccelerometer() 
{
  analogReference(EXTERNAL);
  //calibrate z0 and a0
  for(int i = 0; i < initialCalibrationSampleSize; i++) {
    z0+=ReadAxis(zInput);
    a0+=ReadAxis(aInput);
  }
  z0 = z0 / (initialCalibrationSampleSize);
  a0 = a0 / (initialCalibrationSampleSize);

  for(int i = 0; i < v2_log_size; i++) {
    v2_log[i] = 0;
  }
  
}


void updateAccelerometer() 
{
  int z_raw = ReadAxis(zInput);
  
  int z = z_raw - z0;
  
  z_smoothed = kalman_filter_z.smooth(z);
  //z_smoothed = z;
  
  updateZLog(z_smoothed);

  int a_raw = ReadAxis(aInput);
  
  int a = a_raw - a0;

  float a_smoothed_prev = a_smoothed;
  
  a_smoothed = kalman_filter_a.smooth(a);
  //a_smoothed = a;

  v1 = findSlope(z_log);

  v1_smoothed = kalman_filter_v1.smooth(v1);

  v2 = v2 + (a_smoothed_prev + a_smoothed)/2;

  float v2_smoothed_temp = kalman_filter_v2.smooth(v2);

  if(z < 1) {
    updateV2Log(v2_smoothed_temp);
  }
  
  float v2_offset = findAvg(v2_log);

  v2_smoothed = v2_smoothed_temp - v2_offset;

  if(abs(v2_smoothed) > v2_max)
    v2_max = abs(v2_smoothed);
  
  v_smoothed = kalman_filter_v.smooth(v1_smoothed);// / 0.25 * abs(v2_smoothed));
  
}

float getZ() {return z_smoothed;}
float getV() {return v_smoothed;}
float getV1() {return v1_smoothed;}
float getV2() {return v2_smoothed;}
float getA() {return a_smoothed;}

/**
 * calculates trendline using this method
 * https://classroom.synonym.com/calculate-trendline-2709.html
 */
float findSlope(float data[]) {
  
  int n = sizeof(data);

  float a = 0;
  float b1 = 0;
  float b2 = 0;
  float c = 0;
  for(int i = 0; i < n; i++) {
    a += n*i*data[i];
    b1 += i;
    b2 += data[i];
    c += n*pow(i,2);
  }
  float b = b1*b2;
  float d = pow(b1,2);

  return (a-b)/(c-d);
}

float findAvg(float data[]) {
  float sum = 0;
  for(int i = 0; i < sizeof(data); i++) {
    sum += data[i];
  }
  return sum / sizeof(data);
}

void updateZLog(float z) {
  for(int i = 0; i < z_log_size-1; i++) {
    z_log[i] = float(z_log[i+1]); 
  }
  z_log[z_log_size-1] = z;
}

void updateV2Log(float v2) {
  v2_i++;
  if(v2_i == v2_log_size) {
    v2_i = 0;
  }
  v2_log[v2_i] = v2;
}
 
//
// Read "sampleSize" samples and report the average
//
int ReadAxis(int axisPin)
{
  long reading = 0;
  analogRead(axisPin);
  for (int i = 0; i < sampleSize; i++)
  {
    reading += analogRead(axisPin);
  }
  return reading/sampleSize - 512;
}


  
  

