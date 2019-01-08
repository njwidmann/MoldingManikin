#include <KalmanSmooth.h>

const int zInput = A5;
const int reference = A1;
 
// Take multiple samples to reduce noise
const int initialCalibrationSampleSize = 100;
const int sampleSize = 10;
float z0 = 0;
float z_raw = 0;
float z_smoothed = 0;
float z_smoothed2 = 0;
float ref0 = 0;
float Q_z = 0.001;
float R_z = 0.1;

KalmanSmooth kalman_filter_z(Q_z,R_z,1);
KalmanSmooth kalman_filter_z2(Q_z,R_z,1);
KalmanSmooth kalman_filter_ref(Q_z,R_z,1);
KalmanSmooth kalman_filter_ref2(Q_z,R_z,1);

void setupAccelerometer() 
{
  analogReference(EXTERNAL);
  //calibrate z0
  for(int i = 0; i < initialCalibrationSampleSize; i++) {
    z0+=ReadAxis(zInput);
    ref0+=ReadAxis(reference);
  }
  z0 = z0 / (initialCalibrationSampleSize);
  ref0 = ref0 / (initialCalibrationSampleSize);
  
}

void updateAccelerometer() 
{
  z_raw = ReadAxis(zInput);
  
  z_raw = z_raw - z0;

  z_smoothed = kalman_filter_z.smooth(z_raw);
  
  z_smoothed2 = kalman_filter_z2.smooth(z_smoothed);

  //z_smoothed2 -= kalman_filter_ref2.smooth(kalman_filter_ref.smooth(ReadAxis(reference) - ref0));
 
}

float getZ_raw() {return z_raw;}
float getZ() {return z_smoothed;}
float getZ2() {return z_smoothed2;}

/** 
 *  Read "sampleSize" samples and report the average
 */
int ReadAxis(int axisPin)
{
  long reading = 0;
  analogRead(axisPin);
  //analogRead(reference);
  for (int i = 0; i < sampleSize; i++)
  {
    //reading += (analogRead(axisPin) - analogRead(reference));
    reading += (analogRead(axisPin));
  }
  return reading/sampleSize - 512;
}

