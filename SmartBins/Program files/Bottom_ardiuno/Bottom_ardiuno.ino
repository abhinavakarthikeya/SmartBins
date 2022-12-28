#include<LiquidCrystal.h>
#include<Wire.h>
int en=11;
int rs=12;
int d4=5;
int d5=4;
int d6=3;
int d7=2;

LiquidCrystal lcd(12,11,5,4,3,2);

void setup(){

lcd.begin(16,2);
Wire.begin(2);
Serial.begin(9600);
}


void loop(){
lcd.setCursor(0,0);
lcd.print("Put Your waste here");
delay(10000);
Wire.onReceive(receiveEvent);
delay(1000);
}



void receiveEvent(int data){
  int i=Wire.read();
if(i!=9){
  lcd.print("Main Bin is not empty"); 
  delay(5000);
  lcd.setCursor(0,0);
  if(i==8||i==5||i==2){
    lcd.print("Bins are full");
  }
  else if(i==6||i==3||i==0){
    lcd.print("All the bins are empty");
  }
  else if(i==7){
    lcd.print("Bin 1 is half filled");
  }
  else if(i==4){
    lcd.print("Bin 2 is half filled");
  }
  else if(i==1){
    lcd.print("Bin 3 is half filled");
  }
}
else{
  lcd.print("Main bin is filled");
}

}
