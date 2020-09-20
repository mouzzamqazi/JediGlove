/* JediGlove - A new way of seeing Blindness
      by Mouzzam Husain (19234423)
      MSc in Biomedical Engineering
      (Thesis Project)
 */
#include<Wire.h> // Wire library- used for I2C communication


int ADXL345 = 0x53; //I2C address of ADXL345

float X_g, Y_g, Z_g; // outputs

const int trigPin = 11; // Trigger pin of Ultrasonic sensor
const int echoPin = 13; // Echo pin of ultrasonic sensor

const int motor1 = 4;   // Signal pin of vibration motor 1
const int motor2 = 9;   // Signal pin of vibration motor 2
const int motor3 = 7;   // Signal pin of vibration motor 3
const int motor4 = 8;   // Signal pin of vibration motor 4
const int motor5 = 3;   // Signal pin of vibration motor 5


long duration;
int distance;



void setup() {
pinMode(trigPin, OUTPUT); // output of ultrasonic sensor
pinMode(echoPin, INPUT);  // input to ultrasonic sensor

pinMode(motor1, OUTPUT); // output of motor 1
pinMode(motor2, OUTPUT); // output of motor 2
pinMode(motor3, OUTPUT); // output of motor 3
pinMode(motor4, OUTPUT); // output of motor 4
pinMode(motor5, OUTPUT); // output of motor 5

Serial.begin(9600); // Initiate serial communication for 
                      //printing the results on serial monitor
                      
Wire.begin();        // initiate the wire library
                      // set ADXL345 in measuring mode
                      
Wire.beginTransmission(ADXL345);//Start communicating with the device
Wire.write(0x2D);              // access to power control register-0x2D
Wire.write(8);                 // (oooo 1ooo) ninary bit D3 high for measuring enable
Wire.endTransmission();
delay(10);
}

void loop() {  
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = duration*0.034/2; // distance calculation
Serial.print("Distance: ");
Serial.println(distance);
Wire.beginTransmission(ADXL345); // read accelerometer data
Wire.write(0x32);                //start with register 0x32
Wire.endTransmission(false);
Wire.requestFrom(ADXL345, 6, true); // read 6 resgisters total, each axis value is stored in 2 register
X_g = (Wire.read()|Wire.read() << 8); // X-axis value
X_g = X_g/256;
Y_g = (Wire.read()|Wire.read() << 8); //For a range of +2g to -2g, 
                                     //we need to divide the new values by 256 
                                     //according to the datasheet
Y_g = Y_g/256;                     // Y-axis value
Z_g = (Wire.read()|Wire.read() << 8); // Z-axis value
Z_g = Z_g/256;
Serial.print("Xa= ");
Serial.print(X_g);
Serial.print(" Ya= ");
Serial.println(Y_g);
Serial.print(" Za= ");
Serial.println(Z_g);
 

if(Z_g > 0.40 && Z_g <= 0.94){  // conditional statement for positioning of hand in working state
if(distance >=351 && distance <=400){
  digitalWrite(motor5, HIGH); // Little finger motor will vibrate
}
else{
digitalWrite(motor5, LOW);
}
if(distance >=251 && distance <=350){
digitalWrite(motor4, HIGH); // Ring finger motor will vibrate
}
else{
digitalWrite(motor4, LOW);
}
if(distance >=151 && distance <=250){
digitalWrite(motor3, HIGH); // Middle finger motor will vibrate
}
else{
digitalWrite(motor3, LOW);
}
if(distance >=71 && distance <=150){
digitalWrite(motor2, HIGH); // Index finger motor will vibrate
}
else{
digitalWrite(motor2, LOW);
}
if(distance<=70){
digitalWrite(motor1, HIGH); // Thumb finger motor will vibrate
}
else{
digitalWrite(motor1, LOW);
}
}
else if(Z_g > -0.08 && Z_g <= 0.30){ // condition for position of hand in rest state
digitalWrite(motor1, LOW);
digitalWrite(motor2, LOW);
digitalWrite(motor3, LOW);
digitalWrite(motor4, LOW);
digitalWrite(motor5, LOW);
}
else{                              // if neither of above are true
digitalWrite(motor1, LOW);
digitalWrite(motor2, LOW);
digitalWrite(motor3, LOW);
digitalWrite(motor4, LOW);
digitalWrite(motor5, LOW);
}
delay(500);
}
