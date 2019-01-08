//used for serial output
const String comma = ",";
const String colon = ":";
const String hashtag = "#";
const String squiggly = "~";
const String semicolon = ";";

float Q = 0.001;
float R = 0.1;

KalmanSmooth kalman_filter_depth(Q,R,0);
KalmanSmooth kalman_filter_depth2(Q,R/10,0);

void sendDepth(float depth) {
  
  int depth_adjusted = round(kalman_filter_depth2.smooth(kalman_filter_depth.smooth(75.0/19.0*depth)));
  
  long currentTime = millis();
  Serial.println(hashtag + currentTime + colon + depth_adjusted + squiggly);
  
}

void sendDepthAndVentPressure(float depth, int vent_pressure) {
  
  int depth_adjusted = round(kalman_filter_depth2.smooth(kalman_filter_depth.smooth(75.0/19.0*depth)));
  
  long currentTime = millis();
  Serial.println(hashtag + currentTime + colon + depth_adjusted + comma + vent_pressure + squiggly);
  
}
