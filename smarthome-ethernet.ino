#include <Ethernet2.h>
#include <DHT.h>
#include <UbidotsEthernet.h>
#include <SPI.h>
#include <Servo.h>
#include <Adafruit_Sensor.h>

#define TOKEN "YyodYC5Kx6UX1t9XK9VKFwULlbx40B"
#define ID "5850e40f76254265cfe73650"
#define ID1 "5850e3d77625426643c3cd98"
#define ID2 "5850e43176254265f83bc0c2"
#define ID3 "586e033376254216ca493253"
#define ID4 "586e02eb76254216d6c0a935"
#define ID5 "586e403576254216cf3c33d6"

#define DHTTYPE DHT11
#define DHTPIN 2
int pin = 3;
int led = 8;
int alarm1 = 5;
const int buzzer = 7;
int light = 1;
DHT dht(DHTPIN, DHTTYPE);
Ubidots client (TOKEN);
Servo myservo;

byte mac[] = { 0x90, 0xA2, 0xDA, 0x10, 0x7B, 0x43 };
IPAddress ip(192, 168, 1, 105);

void setup() {
  Serial.begin(9600);
  pinMode (pin, INPUT);
  pinMode (led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(alarm1, OUTPUT);
  myservo.attach(4);
    if (Ethernet.begin(mac) == 0)
    {
      Serial.println("Failed to configure Ethernet using DHCP");
      Ethernet.begin(mac, ip);
    }
    delay(1000);
    Serial.println("Initialization complete");
}
void loop() {
    int control = client.getValue(ID4);
    if (control == 1) myservo.write(90);             
    else myservo.write(0); 
    delay(1000);
    int light = client.getValue(ID5);
    if (light == 1) digitalWrite(led,HIGH);
    else digitalWrite(led,LOW);
    delay(1000);
    float hum = dht.readHumidity();
    float temp = dht.readTemperature();
    int door = digitalRead (pin);
    float co = analogRead(A0);
    if (co >= 1000)
    {
      tone(buzzer, 1000); // Send 1KHz sound signal...
      digitalWrite(alarm1,HIGH);
      delay(1000);        // ...for 1 sec
      noTone(buzzer);     // Stop sound...
      digitalWrite(alarm1,LOW);
      delay(1000);        // ...for 1sec 
    }
    client.add(ID, hum);
    client.add(ID1, temp);
    client.add(ID2, co);
    client.add(ID3, door);
    client.sendAll();
    Serial.println ("Humidity: "+String(hum)+"%, Temperature: "+String(temp)+"°C, CO level: "+String(co)+" ppm\n");
}   
