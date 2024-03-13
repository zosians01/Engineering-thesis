#include "Adafruit_VL53L0X.h"
#include "Adafruit_ADXL345_U.h"
#include "Wire.h"
#define czujnik1_adres 0x30
#define czujnik2_adres 0x31
#define czujnik3_adres 0x32
int ADXL345 = 0x53;
int sensor1, sensor2, sensor3;
#define SilnikLewy 4
#define SilnikPrawy 5
#define czujnik1_xshut 2
#define czujnik2_xshut 3
#define czujnik3_xshut 4
Adafruit_VL53L0X czujnik1 = Adafruit_VL53L0X();
Adafruit_VL53L0X czujnik2 = Adafruit_VL53L0X();
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
float X_out, Y_out, Z_out;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
unsigned long startMillis;
unsigned long startMillis2;
unsigned long currentMillis;
const unsigned long period = 250;
void setup() {
Serial.begin(9600);
Wire.begin();
Wire.setClock(100000);
digitalWrite(SilnikLewy, LOW);
digitalWrite(SilnikPrawy, LOW);
pinMode(czujnik1_xshut, OUTPUT);
pinMode(czujnik2_xshut, OUTPUT);
Serial.println("inicjalizacja pinow XSHUT");
digitalWrite(czujnik1_xshut, LOW);
digitalWrite(czujnik2_xshut, LOW);
Serial.println("Rozpoczynanie pomiaru...");
setID();
startMillis = millis();
startMillis2 = millis();
if (!accel.begin()) {
Serial.println("Nie odnalezniono ADXL345");
while (1);
}
}
void setID() {
digitalWrite(czujnik1_xshut, LOW);
digitalWrite(czujnik2_xshut, LOW);
delay(10);
digitalWrite(czujnik1_xshut, HIGH);
digitalWrite(czujnik2_xshut, HIGH);
delay(10);
digitalWrite(czujnik1_xshut, LOW);
digitalWrite(czujnik2_xshut, LOW);
digitalWrite(czujnik1_xshut, HIGH);
// inicjalizacja czujnika1
if (!czujnik1.begin(czujnik1_adres)) {
Serial.println(F("Nie mozna zinicjalizowac czujnika lewego"));
while (1);
}
delay(10);
digitalWrite(czujnik2_xshut, HIGH);
delay(10);
//inicjalizacja czujnika2
if (!czujnik2.begin(czujnik2_adres)) {
Serial.println(F("Nie mozna zinicjalizowac czujnika prawego"));
while (1);
}
digitalWrite(czujnik3_xshut, HIGH);
delay(10);
}
void read_sensors() {
czujnik1.rangingTest(&measure1, false);
czujnik2.rangingTest(&measure2, false);
Serial.print(measure1.RangeMilliMeter);
Serial.print("\t");
Serial.println(measure2.RangeMilliMeter);
digitalWrite(SilnikLewy, LOW);
digitalWrite(SilnikPrawy, LOW);
czujnik1.rangingTest(&measure1, false);
Serial.println(measure1.RangeMilliMeter);
czujnik2.rangingTest(&measure2, false);
Serial.print("1: ");
sensor1 = measure1.RangeMilliMeter;
Serial.print(sensor1);
Serial.print("mm");
Serial.println();
delay(100);
if (sensor1 < 70) {
currentMillis = millis();
if (currentMillis - startMillis >= period) {
digitalWrite(SilnikLewy, HIGH);
startMillis = currentMillis;
}
Serial.print(" ");
Serial.print("High");
delay(100);
digitalWrite(SilnikLewy, LOW);
} else {
Serial.print(" ");
Serial.print("Low");
}
Serial.println();
Serial.print("2: ");
sensor2 = measure2.RangeMilliMeter;
Serial.println(measure2.RangeMilliMeter);
Serial.print(sensor2);
Serial.print("mm");
if (sensor2 < 70) {
currentMillis = millis();
if (currentMillis - startMillis2 >= period) {
digitalWrite(SilnikPrawy, HIGH);
startMillis2 = currentMillis;
}
Serial.println();
Serial.println("High");
delay(100);
digitalWrite(SilnikPrawy, LOW);
} else {
Serial.print(" ");
Serial.println("Low");
}
Serial.print(" ");
Serial.println();
}
void read_position(void) {
sensors_event_t event;
accel.getEvent(&event);
Serial.print("X: ");
Serial.println(event.acceleration.x);
delay(100);
Serial.print("Y: ");
Serial.println(event.acceleration.y);
delay(100);

Serial.print("Z: ");
Serial.println(event.acceleration.z);
delay(100);
// Wyłączanie pomairu w przypadku spoczynku psa
if (event.acceleration.x < 0) {
digitalWrite(czujnik1_xshut, LOW);
digitalWrite(czujnik2_xshut, LOW);
digitalWrite(SilnikLewy, LOW);
digitalWrite(SilnikPrawy, LOW);
} 
else if (event.acceleration.x >= 2.20 && event.acceleration.x <=2.40) 
{
digitalWrite(czujnik1_xshut, LOW);
digitalWrite(czujnik2_xshut, LOW);
digitalWrite(SilnikLewy, LOW);
digitalWrite(SilnikPrawy, LOW);
}
delay(500);
}
void loop() {
read_position();
read_sensors();
delay(100);
}
