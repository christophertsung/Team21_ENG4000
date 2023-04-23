#include <Arduino.h>
#include <Wire.h>
#include "HX711.h"

// Define FSR (grip sensor) pin:
#define fsrpin A0
#define analogload A2

// HX711 (load sensor) circuit wiring
const int LOADCELL_DOUT_PIN = 4;
const int LOADCELL_SCK_PIN = 5;
HX711 scale;

// GY-521 (Accelerometer variables)
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t accelerometer1_x, accelerometer1_y, accelerometer1_z; // variables for accelerometer1 raw data
int16_t gyro1_x, gyro1_y, gyro1_z; // variables for gyro1 raw data
int16_t accelerometer2_x, accelerometer2_y, accelerometer2_z; // variables for accelerometer2 raw data
int16_t gyro2_x, gyro2_y, gyro2_z; // variables for gyro2 raw data
int16_t accelerometer3_x, accelerometer3_y, accelerometer3_z; // variables for accelerometer3 raw data
int16_t gyro3_x, gyro3_y, gyro3_z; // variables for gyro3 raw data

//Define variable to store other sensor readings:
int gripReading; // Variable to store grip analog reading output value
long loadReading; // Variable to store load analog reading output value
int analogLoad;
// Function for selecting TCA9548A MUX bus
void TCA9548A(uint8_t bus)
{
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}

void setup() {
  // Begin serial communication at a baud rate of 9600:
  Serial.begin(57600);
  // Initialize scale
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  //Set scale calibration factor
  scale.set_scale();
  // Tare scale
  // scale.tare(); // Reset the scale to 0



  // Initialize accelerometer
  TCA9548A(2);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  TCA9548A(3);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  TCA9548A(4);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

}

void loop() {

  // Grip force analog reading
  gripReading = analogRead(fsrpin);

  // Force sensor reading
  loadReading = scale.get_units();

  // analog amplifier
  analogLoad = analogRead(analogload);

  // Accelerometer1 readings
  TCA9548A(2);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers

  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  accelerometer1_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer1_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer1_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  gyro1_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro1_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro1_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)

    // Accelerometer2 readings
  TCA9548A(3);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  accelerometer2_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer2_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer2_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  gyro2_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro2_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro2_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)

    // Accelerometer3 readings
  TCA9548A(4);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  accelerometer3_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer3_y = Wire.read()<<8 | Wire.read(); // registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer3_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  gyro3_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro3_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro3_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)

  Serial.print(gripReading); 
  Serial.print(","); 
  Serial.print(loadReading); 
  Serial.print(","); 
  Serial.print(accelerometer1_x); 
  Serial.print(","); 
  Serial.print(accelerometer1_y); 
  Serial.print(","); 
  Serial.print(accelerometer1_z); 
  Serial.print(","); 
  Serial.print(gyro1_x); 
  Serial.print(","); 
  Serial.print(gyro1_y); 
  Serial.print(","); 
  Serial.print(gyro1_z); 
  Serial.print(","); 
  Serial.print(accelerometer2_x); 
  Serial.print(","); 
  Serial.print(accelerometer2_y); 
  Serial.print(","); 
  Serial.print(accelerometer2_z); 
  Serial.print(","); 
  Serial.print(gyro2_x); 
  Serial.print(","); 
  Serial.print(gyro2_y); 
  Serial.print(","); 
  Serial.print(gyro2_z); 
  Serial.print(","); 
  Serial.print(accelerometer3_x); 
  Serial.print(","); 
  Serial.print(accelerometer3_y); 
  Serial.print(","); 
  Serial.print(accelerometer3_z); 
  Serial.print(","); 
  Serial.print(gyro3_x); 
  Serial.print(","); 
  Serial.print(gyro3_y); 
  Serial.print(","); 
  Serial.print(gyro3_z); 
  Serial.println();

  delay(20); //Delay 20 ms -- Equivaent to 50Hz
}
