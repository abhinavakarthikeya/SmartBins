#include<Wire.h>
#include<Servo.h>
int moisture_sensor=A0;
int gas_sensor=A1;
int trigg=13;
int echo=12;
int trigg2=7;
int echo2=6;
int lid=9;
int base=5;
int distance;
int duration;
int distance2;
char Status[2]="e";
Servo servo_lid;
Servo servo_base;


void setup() {
  // put your setup code here, to run once:
  pinMode(moisture_sensor,INPUT);
  pinMode(gas_sensor,INPUT);
  pinMode(echo,INPUT);
  pinMode(trigg,OUTPUT);
  servo_lid.attach(lid);
  servo_base.attach(base);
  Serial.begin(9600);
  Wire.begin();
}




void loop() {
  // put your main code here, to run repeatedly:

//  measuring distance
  digitalWrite(trigg, LOW);
  delayMicroseconds(2);   
  digitalWrite(trigg, HIGH);     // send waves for 10 us
  delayMicroseconds(10);
  duration = pulseIn(echo, HIGH); // receive reflected waves
  distance = (duration*0.034/2);   // convert to distance
  delay(1000);


    //  if distance is greater than 30cm
  if(distance<30){
    delay(1000);

    //    if status is not full
  digitalWrite(trigg2, LOW);
  delayMicroseconds(2);   
  digitalWrite(trigg2, HIGH);     // send waves for 10 us
  delayMicroseconds(10);
  duration = pulseIn(echo2, HIGH); // receive reflected waves
  distance2 = (duration*0.034/2);   // convert to distance
  if(distance2<30){
     Status[1]="f";
  }
  else{
    Status[1]="e";
  }
  delay(1000);
    if(Status!="f"){
      servo_lid.write(90);
      delay(2000);
      servo_lid.write(0);
      

        // if the watse is wet waste
        if(analogRead(moisture_sensor)>512){
              Serial.write("m",1);
              delay(10000);
              Serial.readBytes(Status,1);

              //if the wet bin is full
              if(Status=="f")
              {
                Wire.beginTransmission(2);
                Wire.write(8);
                Wire.endTransmission(2);
              }

              //if the wet bin is more than half filled
              else if (Status=="h"){
                Wire.beginTransmission(2);
                Wire.write(7);
                Wire.endTransmission(2);
                servo_base.write(90);
                delay(5000);
                servo_base.write(0);
              }


              //if the wet bin is less than half filled
              else if (Status=="e"){
                Wire.beginTransmission(2);
                Wire.write(6);
                Wire.endTransmission(2);
                servo_base.write(90);
                delay(5000);
                servo_base.write(0);
              }
        
      }


      //if detected by gas sensor
      else if(analogRead(gas_sensor)>512){
            
              Serial.write("g",1);
              Serial.readBytes(Status,1);
              
              if(Status=="f")
              {
                Wire.beginTransmission(2);
                Wire.write(5);
                Wire.endTransmission(2);
              }
              
              else if (Status=="h"){
                Wire.beginTransmission(2);
                Wire.write(4);
                Wire.endTransmission(2);
                servo_base.write(90);
                delay(5000);
                servo_base.write(0);
              }
              
              else if (Status=="e"){
                Wire.beginTransmission(2);
                Wire.write(3);
                Wire.endTransmission(2);
                servo_base.write(90);
                delay(5000);
                servo_base.write(0);
              }
      }

      
         //not detected by any sensor
         else{
              Serial.write("u",1);
              Serial.readBytes(Status,1);
              
              if(Status=="f")
              {
                Wire.beginTransmission(2);
                Wire.write(2);
                Wire.endTransmission(2);
              }
              
              else if (Status=="h"){
                Wire.beginTransmission(2);
                Wire.write(1);
                Wire.endTransmission(2);
                servo_base.write(90);
                delay(5000);
                servo_base.write(0);
              }

              else if (Status=="e"){
                Wire.beginTransmission(2);
                Wire.write(0);
                Wire.endTransmission(2);
                servo_base.write(90);
                delay(5000);
                servo_base.write(0);
              }
        
      }


      delay(5000);

      
    }


    else{
        Wire.write(9);
    }
  }
}
