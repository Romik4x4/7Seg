#include <OneWire.h>

// #include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

// OneWire oneWire(ONE_WIRE_BUS);

OneWire ds(ONE_WIRE_BUS);


// DallasTemperature sensors(&oneWire);
// DeviceAddress insideThermometer = { 0x28, 0x0C, 0x49, 0x7F, 0x05, 0x00, 0x00, 0x7E };

// const int LED = 6; // Test LED

byte addr[8] = { 0x28, 0x0C, 0x49, 0x7F, 0x05, 0x00, 0x00, 0x7E };
byte data[12];

#define LED 6

#define A A4
#define B A2
#define C 10
#define D 8
#define E 7
#define F A3
#define G 11

#define CA2 A0
#define CA1 13
#define CA3 A1

#define DP 9      // Точка

#define COOLER 5  // Вентилятор Не работает

const int segs[7] = { A, B, C, D, E, F, G };

const byte numbers[10] = {  
  0b1000000, 
  0b1111001, 
  0b0100100, 
  0b0110000, 
  0b0011001, 
  0b0010010,
  0b0000010, 
  0b1111000, 
  0b0000000, 
  0b0010000 };

long previousMillis = 0; 
long interval = 1000;
unsigned long currentMillis;
boolean run = false;

float temp;
int t1,t2,t3;

void setup() {    

  //  sensors.begin();
  //  sensors.setResolution(insideThermometer, 9);

  pinMode(LED, OUTPUT);  

  pinMode(13,OUTPUT);  // Digital 1
  pinMode(A0,OUTPUT);  // Digital 2  
  pinMode(A1,OUTPUT);  // Digital 3

  pinMode(A4,OUTPUT); // SEG A
  pinMode(A2,OUTPUT); // SEG B
  pinMode(10,OUTPUT); // SEG C
  pinMode(8,OUTPUT);  // SEG D
  pinMode(7,OUTPUT);  // SEG E
  pinMode(A3,OUTPUT); // SEG F
  pinMode(11,OUTPUT); // SEG G

  pinMode(DP,OUTPUT);  // DP

  digitalWrite(LED,HIGH);

}


void loop() {

  byte i;
  
  currentMillis = millis();

  // sensors.requestTemperatures();

  lightDigit1(numbers[1]); // temp%10]);
  delay(5);
  lightDigit2(numbers[2]); // int(temp/10)]);
  delay(5);
  lightDigit3(numbers[9]); // int(8)]);
  delay(5);

  // temp = sensors.getTempC(insideThermometer);

  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;

    if (run) {
     ds.reset();
     ds.select(addr);    
     ds.write(0xBE);  
     for ( i = 0; i < 9; i++) data[i] = ds.read();
     run = false;
     int HighByte, LowByte, TReading, Tc_100;
     LowByte = data[0];
     HighByte = data[1];
     TReading = (HighByte << 8) + LowByte; 
     Tc_100 = TReading/2;  
     
     byte MSB = data[1];
     byte LSB = data[0];

     float tempRead = ((MSB << 8) | LSB); //using two's compliment
     float TemperatureSum = tempRead / 16;
 
 /*
 
 thousands=value/1000;
 hundreds=(value%1000)/100;
 tens=(value%100)/10;
 ones=value%10;

 digdisp(thousands,0);
 digdisp(hundreds,1);
 digdisp(tens,2);
 digdisp(ones,3);
 cleardisplay();

 */
 
 
   }    
   
    if (!run) {
     ds.reset();
     ds.select(addr);
     ds.write(0x44,1);
     run = true;
    }    
    
    if (digitalRead(LED) == 1) { 
      digitalWrite(LED,LOW); 
    } 
    else { 
      digitalWrite(LED,HIGH); 
    }
  }
}

void lightDigit1(byte number) {
  digitalWrite(CA1,HIGH);
  digitalWrite(CA2,LOW);
  digitalWrite(CA3,LOW);  
  digitalWrite(DP, HIGH);
  lightSegments(number);
}

void lightDigit2(byte number) {
  digitalWrite(CA1,LOW);
  digitalWrite(CA2,HIGH);
  digitalWrite(CA3,LOW);    
  digitalWrite(DP, LOW);  
  lightSegments(number);
}

void lightDigit3(byte number) {
  digitalWrite(CA1,LOW);
  digitalWrite(CA2,LOW);
  digitalWrite(CA3,HIGH);
  digitalWrite(DP, HIGH);  
  lightSegments(number);
}

void lightSegments(byte number) {
  for (int i = 0; i < 7; i++) {
    int bit = bitRead(number, i);
    digitalWrite(segs[i], bit);
  }
}


