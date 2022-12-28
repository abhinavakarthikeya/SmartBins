#include <ESP8266WiFi.h>
#include<ThingSpeak.h>
#include<WiFiClient.h>
#include<Servo.h>

WiFiClient client; 

const int httpPort = 80;

Servo myservo;

const char* host = "maker.ifttt.com";


int servopin=A0;

int trigg_m=D0;
int echo_m=D1;

int trigg_g=D2;
int echo_g=D3;

int trigg_u=D4;
int echo_u=D5;

char Status[1];
char bin[1];
const char* ssid = "abhi";
const char* password = "abhi01234";

int duration;


int initial_d_m;
int initial_d_g;
int initial_d_u;

int distance_m;
int distance_g;
int distance_u;

int percentage;



void setup() {

  // put your setup code here, to run once:
  WiFi.begin(ssid,password);
  Serial.begin(9600);

  myservo.attach(servopin);
  digitalWrite(trigg_m, LOW);
  delayMicroseconds(2);   
  digitalWrite(trigg_m, HIGH);     // send waves for 10 us
  delayMicroseconds(10);
  duration = pulseIn(echo_m, HIGH); // receive reflected waves
  initial_d_m = (duration*0.034/2);   // convert to 

  digitalWrite(trigg_g, LOW);
  delayMicroseconds(2);   
  digitalWrite(trigg_g, HIGH);     // send waves for 10 us
  delayMicroseconds(10);
  duration = pulseIn(echo_g, HIGH); // receive reflected waves
  initial_d_g = (duration*0.034/2);   // convert to 

  digitalWrite(trigg_u, LOW);
  delayMicroseconds(2);   
  digitalWrite(trigg_u, HIGH);     // send waves for 10 us
  delayMicroseconds(10);
  duration = pulseIn(echo_u, HIGH); // receive reflected waves
  initial_d_u= (duration*0.034/2);   // convert to 
  
}



void loop() {
  
  Serial.readBytes(bin,1);

  
//  if stected waste has moisture content
  if(bin=="m"){
    percentage=moisture_sensor();
    if(percentage<=0.4){
      Serial.write("e",1);
      myservo.write(0);
      delay(10000);
      
    }
    else if(0.4<percentage<=0.85){
      Serial.write("h",1);
      myservo.write(0);
      delay(10000);
    }
    else{
        
      if (!client.connect(host, httpPort)) {  
          Serial.println("connection failed");  
            return;}

            
     String url = "/trigger/BIN_1/with/key/m7dMFf-5rWz8jfbbD1nx13g9qJYXAfOGonmhZWP2w7H"; 
     Serial.print("Requesting URL: ");
     Serial.println(url);
     client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" +"Connection: close\r\n\r\n");  
     Serial.write("f",1);
     delay(10000);
    }  
   } 


//   if detected waste has gas content
  else if(bin=="g"){
    percentage=gas_sensor();
    if(percentage<=0.4){
      Serial.write("e",1);
      myservo.write(90);
      delay(10000);
      myservo.write(0);
    }
    else if(0.4<percentage<=0.85){
      Serial.write("h",1);
      myservo.write(90);
      delay(10000);
      myservo.write(0);
    }
    else{
      Serial.write("f",1);
      if (!client.connect(host, httpPort)) {  
          Serial.println("connection failed");  
            return;}

            
     String url = "/trigger/BIN_2/with/key/m7dMFf-5rWz8jfbbD1nx13g9qJYXAfOGonmhZWP2w7H"; 
     Serial.print("Requesting URL: ");
     Serial.println(url);
     client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" +"Connection: close\r\n\r\n");  
     Serial.write("f",1);
     delay(10000);
     myservo.write(0);
    }  
   } 

//   if waste is undetected
   else{
    percentage=undetected();
    if(percentage<=0.4){
      Serial.write("e",1);
      myservo.write(180);
      delay(10000);
      myservo.write(0);
    }
    else if(0.4<percentage<=0.85){
      Serial.write("h",1);
      myservo.write(180);
      delay(10000);
      myservo.write(0);
    }
    else{
      Serial.write("f",1);
      if (!client.connect(host, httpPort)) {  
          Serial.println("connection failed");  
            return;}

            
     String url = "/trigger/BIN_3/with/key/m7dMFf-5rWz8jfbbD1nx13g9qJYXAfOGonmhZWP2w7H"; 
     Serial.print("Requesting URL: ");
     Serial.println(url);
     client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" +"Connection: close\r\n\r\n");  
     Serial.write("f",1);
     delay(10000);
    }
   }
}




//detecting the percentage upto which the bin filled
//moisture sensor
int moisture_sensor(){
    delay(1000);
    digitalWrite(trigg_m, LOW);
    delayMicroseconds(2);   
    digitalWrite(trigg_m, HIGH);     // send waves for 10 us
    delayMicroseconds(10);
    duration = pulseIn(echo_m, HIGH); // receive reflected waves
    distance_m = (duration*0.034/2); 
    percentage=(distance_m/initial_d_m);
    ThingSpeak.writeField(1454630,1,percentage,"74IJ9X1BEJFKAOWB");
    return percentage;
}



//gas sensor
int gas_sensor(){
    delay(1000);
    digitalWrite(trigg_g, LOW);
    delayMicroseconds(2);   
    digitalWrite(trigg_g, HIGH);     // send waves for 10 us
    delayMicroseconds(10);
    duration = pulseIn(echo_g, HIGH); // receive reflected waves
    distance_g = (duration*0.034/2); 
    percentage=(distance_g/initial_d_g);
    ThingSpeak.writeField(1454630,2,percentage,"74IJ9X1BEJFKAOWB");
    return percentage;
}



//undetected
int undetected(){
    delay(1000);
    digitalWrite(trigg_u, LOW);
    delayMicroseconds(2);   
    digitalWrite(trigg_u, HIGH);     // send waves for 10 us
    delayMicroseconds(10);
    duration = pulseIn(echo_u, HIGH); // receive reflected waves
    distance_u = (duration*0.034/2); 
    percentage=(distance_u/initial_d_u);
    ThingSpeak.writeField(1454630,3,percentage,"74IJ9X1BEJFKAOWB");
    return percentage;
}
