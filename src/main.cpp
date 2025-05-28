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
float targetZ = 90.0;

float errorX = 0.0;
float errorZ = 0.0;

float KpX = 1.0;
float KpZ = 1.0;
float KdX = 1.0;
float KdZ = 1.0;

float Px = 0.0;
float Pz = 0.0;
float Dx = 0.0;
float Dz = 0.0;


//TEST


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
long elapsed;
elapsed = millis();

//x
errorX = targetX-x;         //target is 90 degrees | MPU6050 flat
Px = errorX*KpX;
Dx = (((errorX) / t) * KdX);
servoX.write(Px);       //servo mounted vertically
if (Px <= 0); {
    servoX.write(Px+90);
}

//z
errorZ = targetZ-z;         
Pz = errorZ*KpZ;
Dx = (((errorZ) / t) * KdZ);
servoY.write(Pz);
if (Pz <= 0); {
    servoY.write(Pz);
}




//print
Serial.print(x);
Serial.print("   |   ");
Serial.print(y);
Serial.print("   |   ");
Serial.print(z);
Serial.print("   |   ");
Serial.print(Px);
Serial.print("   |   ");
Serial.print(Pz);
Serial.print("   |   ");
Serial.print(t/1000);
Serial.print("   |   ");
Serial.println();


    

delay(10);


}