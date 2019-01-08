# MoldingManikin
Arduino code for a prototype CPR training manikin that simulates dampening on recoil to make compressions feel more realistic. Uses a solenoid coil wrapped around the manikin spring (shown in picture below) to adjust the spring force throughout the recoil. A motor driver controls the voltage (using PWM) through the coil to adjust magnetic force. See [here](Molding_Manikin_Demo/Molding_Manikin_Demo_Video.mp4) for a video demo. You can see the chest recoil looks more like a nerf ball reinflating than a spring shooting back up.

![Picture of the inside of the manikin](Molding_Manikin_Demo/Molding_Manikin_Photo2.jpg)

## Mechanical Setup
Uses an accelerometer at the base of the chest plate (shown below) along with a series of kalman filters (for smoothing) to estimate depth.

![Picture of the accelerometer](Molding%20Manikin%20Demo/IMG_20180822_151436.jpg)

## Arduino Code
* [MoldingManikin.ino](MoldingManikin.ino) - The main arduino code file. Contains setup() and loop() functions. Handles setup and manages other .ino files below. Also accepts and handles input for these settings:
  * Damping magnitude (Recoil Time) - Set inside [CPRDummy android app](https://github.com/njwidmann/CPRDummy) set to "Damping Test" mode.
  * Molding (residual damping) - Set using a potentiometer (optional)
* [bluetooth_connection.ino](bluetooth_connection.ino) - Sends depth and ventilation data over bluetooth to [CPRDummy android app](https://github.com/njwidmann/CPRDummy).
* [magnet_control.ino](magnet_control.ino) - Sets direction and magnitude of magnet force
* [accelerometer.ino](accelerometer.ino) - Handles accelerometer input and converting to usable depth (z) data for calculations. Uses a stack of 2 kalman filters for smoothing.
* [ventilations.ino](ventilations.ino) - Reads ventilation velocity sensor. Uses a kalman filter to smooth raw data.
* [KalmanSmooth](KalmanSmooth.cpp) - My library for handling real-time smoothing of sensor data using a kalman filter.
