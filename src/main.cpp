#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>

Servo servoX;
Servo servoY;

const int MPU_addr=0x68;

int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

// remove wiring from bluetooth while uploading program 

int minVal=265;
int maxVal=402;

double x;
double y;
double z;

//PID
float targetX = 90.0;
float errorX = 0.0;
float KpX = 0.7;
float angleX = 0.0;

float targetZ = 90.0;
float errorZ = 0.0;
float KpZ = 1.0;
float angleZ = 0.0;

void setup(){

Wire.begin();
Wire.beginTransmission(MPU_addr);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);

servoX.attach(3);
servoY.attach(4);

Serial.begin(9600);

}

void loop(){

//accel
Wire.beginTransmission(MPU_addr);
Wire.write(0x3B);
Wire.endTransmission(false);
Wire.requestFrom(MPU_addr,14,true);

AcX=Wire.read()<<8|Wire.read();
AcY=Wire.read()<<8|Wire.read();
AcZ=Wire.read()<<8|Wire.read();

int xAng = map(AcX,minVal,maxVal,-90,90);
int yAng = map(AcY,minVal,maxVal,-90,90);
int zAng = map(AcZ,minVal,maxVal,-90,90);
 
x = RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
y = RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
z = RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);


//PID
long time;
time = millis();

errorX = targetX-x;         //target is 90 degrees | MPU6050 flat
angleX = errorX*KpX;
servoX.write(angleX);       //servo mounted vertically
if (angleX <= 0); {
    servoX.write(angleX+90);
}

errorZ = targetZ-z;         
angleZ = errorZ*KpZ;
servoY.write(angleZ);       
if (angleZ <= 0); {
    servoY.write(angleZ);
}


//print
Serial.print(x);
Serial.print("   |   ");
Serial.print(y);
Serial.print("   |   ");
Serial.print(z);
Serial.print("   |   ");
Serial.print(angleX);
Serial.print("   |   ");
Serial.print(time/1000);
Serial.print("   |   ");
Serial.println();


    

delay(10);


}