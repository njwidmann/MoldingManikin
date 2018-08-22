//used for serial output
const String comma = ",";
const String colon = ":";
const String hashtag = "#";
const String squiggly = "~";
const String semicolon = ";";

KalmanSmooth kalman_filter_depth(Q,R,0);
KalmanSmooth kalman_filter_depth2(Q,R/10,0);

void sendDepth(float depth) {
  
  int depth_adjusted = round(kalman_filter_depth2.smooth(kalman_filter_depth.smooth(75.0/15.0*depth)));
  
  long currentTime = millis();
  Serial.println(hashtag + currentTime + colon + depth_adjusted + squiggly);
  
}

