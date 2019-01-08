const int ventilation_pin = A7;

float p0 = 0;

KalmanSmooth kalman_filter_p(Q, R, 5);

void setupVentilations() {
  calibrateP0();
}

void calibrateP0() {
  for(int i = 0; i < initialCalibrationSampleSize; i++) {
    p0+=analogRead(ventilation_pin);
  }
  p0 = p0 / (initialCalibrationSampleSize);
}

int readVentilationVelocity() {
  return kalman_filter_p.smooth(analogRead(ventilation_pin) - p0);
}

