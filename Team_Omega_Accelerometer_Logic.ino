#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define LED 2
Adafruit_MPU6050 mpu;
double x_rot[5], y_rot[5], z_rot[5];
int xSize, ySize,zSize;
double range = 1;

void setup() {

  pinMode(LED,OUTPUT);
  xSize = 0;
  ySize = 0;
  zSize = 0;

  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  Serial.print("Hello World!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  double x = g.gyro.x,
         y = g.gyro.y,
         z = g.gyro.z;

  // LOGIC:
  // Reads the past 5 gyroscope readings and compare the average to the current reading
  // If there is less than 5 reading, then continue reading values
  // If there is 5 reading, find the average and then blink if criteria is met

  // Assuming xSize, ySize, and zSize are the same size
  if (xSize < 5){
    updateGyroscopeStat(x,y,z);
  }
  else { // Once the data has been collected
    double xAvg = getAverage('x'),
           yAvg = getAverage('y'),
           zAvg = getAverage('z');

    // Checks if a large shift is detected in movement
    bool xShift = (xAvg - x) > range,
         yShift = (yAvg - y) > range,
         zShift = (zAvg - z) > range;

    // Blink if shift is detected and clear array
    if (xShift || yShift || zShift){
      blink();
      clearArray('a');
    }

  }
}

void blink(){
  delay(500);
  digitalWrite(LED,HIGH);
  delay(500);
  digitalWrite(LED,LOW);
}

// Upload the current reading to the array
void updateGyroscopeStat(double x,double y, double z){
  x_rot[xSize] = x;
  y_rot[ySize] = y;
  z_rot[zSize] = z;

  xSize++;
  ySize++;
  zSize++;
}

// Set all the values to 0 and then reset the count to 0
void clearArray(char op){
  switch (op){
    case 'x':
      for (int i = 0; i < xSize; i++){
        x_rot[i] = 0;
      }
      xSize = 0;
    break;
    case 'y':
      for (int i = 0; i < xSize; i++){
        y_rot[i] = 0;
      }
      ySize = 0;
    break;
    case 'z':
      for (int i = 0; i < xSize; i++){
        z_rot[i] = 0;
      }
      zSize = 0;
    break;
    default:
      clearArray('x');
      clearArray('y');
      clearArray('z');
    break;
  }
}

// Find the average value of the array
double getAverage(char op){
  double sum = 0;
  double tempArr[5];

  switch (op){
    case 'x':
      for ( int i = 0; i < xSize; i++){
        sum += x_rot[i];
      }
    break;
    case 'y':
      for ( int i = 0; i < xSize; i++){
        sum += y_rot[i];
      }
    break;
    case 'z':
    for ( int i = 0; i < xSize; i++){
      sum += z_rot[i];
    }
    break;
  }

  return sum/xSize;
}