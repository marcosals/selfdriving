#include "I2Cdev.h"
#include "MPU6050.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
#include <SoftwareSerial.h>

SoftwareSerial hc06(10, 11);
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int sendGyroData;

void setupBluetooth() {
  // initialize hc06
  hc06.begin(9600);
  // ping
  command("AT", 2000);
  // set pin
  command("AT+PIN1234", 2000);
  // set name
  command("AT+NAMEAduinoMarcos", 2000);
  // set baud rate
  command("AT+BAUD4", 2000);
}

void setup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif
    
  //Initialize Serial Monitor
  Serial.begin(9600);

  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  sendGyroData = 0;

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  accelgyro.setXGyroOffset(0);
  accelgyro.setYGyroOffset(35);
  accelgyro.setZGyroOffset(0);
  accelgyro.setXAccelOffset(0);
  accelgyro.setYAccelOffset(0);
  accelgyro.setZAccelOffset(0);
  
  setupBluetooth();  
}

void loop() {
  // read raw accel/gyro measurements from device
  //accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // these methods (and a few others) are also available
  //accelgyro.getAcceleration(&ax, &ay, &az);
  accelgyro.getRotation(&gx, &gy, &gz);

  sendGyroData++;

  if (sendGyroData > 200) {
    sendGyroData = 0;
    // display tab-separated accel/gyro x/y/z values
    
    hc06.print("ag|");
    /*
    hc06.print(ax); hc06.print("|");
    hc06.print(ay); hc06.print("|");
    hc06.print(az); hc06.print("|");
    hc06.print(gx); hc06.print("|");*/
    hc06.print(gy); /*hc06.print("|");
    hc06.print(gz);*/ hc06.print("~");
    /*
    Serial.print("ag|");
    Serial.print(ax); Serial.print("|");
    Serial.print(ay); Serial.print("|");
    Serial.print(az); Serial.print("|");
    Serial.print(gx); Serial.print("|");
    Serial.print(gy); Serial.print("|");
    Serial.print(gz); Serial.println("~");
    */
  }
}


String command(const char *toSend, unsigned long milliseconds) {
  String result;
  Serial.print("Sending: ");
  Serial.println(toSend);
  hc06.print(toSend);
  unsigned long startTime = millis();
  Serial.print(F("Received: "));
  while (millis() - startTime < milliseconds) {
    if (hc06.available()) {
      char c = hc06.read();
      Serial.write(c);
      result += c;  // append to the result string
    }
  }
  Serial.println();  // new line after timeout.
  return result;
}
