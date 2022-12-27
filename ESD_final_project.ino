//Libraries
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Servo.h>

#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
int LEDT = 2;
int LEDS = 3;
int LEDL = 4;
int LEDW = 5;


//variable and object for servo motor
Servo myservo;  // creating servo object to control a servo
int pos = 0;    // variable to store the servo position

//variables for infrared sensor
const int flamePin = 11;
int Flame = HIGH;

//variables to store wifi password and username
char ssid[] = "example";//(enter username of your wifi)
char pass[] = "testing_123";//(enter password of your wifi)

//variable for mq135 gas sensor
int sensorValue;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
//creating objects to access the functions present
//in mqtt library and wifi library

//Defining the port topic and brocker to publish data
const char broker[] = "broker.mqttdashboard.com";
int        port     = 1883;
const char topic_gas[]  = "Gas_sensor_data";
const char topic_temp[]  = "temp_sensor_data";
const char topic_hum[]  = "hum_sensor_data";
const char topic_flame[]  = "flame_sensor_data";
const char topic_sub[]  = "web_data";


//constants to keep track of the publishing of data
const long interval = 1000;
unsigned long previousMillis = 0;

int count = 0;


//Constants for temprature sensor
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);


//variables for temprature sensor
float hum;  //Stores humidity value
float temp; //Stores temperature value

//constants for infrared sensor
// lowest and highest sensor readings:
const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum

void setup() {
  pinMode(LEDT, OUTPUT);
  pinMode(LEDS, OUTPUT);
  pinMode(LEDL, OUTPUT);
  pinMode(LEDW, OUTPUT);
  pinMode(flamePin, INPUT);
  Serial.begin(9600);
  dht.begin();
  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED)
  {
    // failed, retry
    Serial.print(".");
    delay(5000);
  } // retry after 5 seconds to reconnect
  //once connected display the following
  Serial.println("You're connected to the network");
  Serial.println();
  //connecting to mqtt brocker to publish data
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
  mqttClient.subscribe(topic_sub);

}


void loop() {
  int messageSize = mqttClient.parseMessage();
  while (mqttClient.available()) {
    char z = (char)mqttClient.read();
    if (z == 'T')
    {
      digitalWrite(LEDT, HIGH);
      digitalWrite(LEDS, LOW);
      digitalWrite(LEDL, LOW);
      digitalWrite(LEDW, LOW);
    }
    if (z == 'S')
    {
      digitalWrite(LEDS, HIGH);
      digitalWrite(LEDT, LOW);
      digitalWrite(LEDL, LOW);
      digitalWrite(LEDW, LOW);
    }
    if (z == 'L')
    {
      digitalWrite(LEDL, HIGH);
      digitalWrite(LEDT, LOW);
      digitalWrite(LEDS, LOW);
      digitalWrite(LEDW, LOW);
    }
    if (z == 'W')
    {
      digitalWrite(LEDW, HIGH);
      digitalWrite(LEDT, LOW);
      digitalWrite(LEDS, LOW);
      digitalWrite(LEDL, LOW);
    }
  }
  //avoids disconnection from the broker
  mqttClient.poll();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;


    Serial.println();

    //code to send data to raspberry pi
    //for temp data
    mqttClient.beginMessage(topic_temp);
    mqttClient.publish(Temp_sensor());
    mqttClient.endMessage();

    //for hum data
    mqttClient.beginMessage(topic_hum);
    mqttClient.publish(Hum_sensor());
    mqttClient.endMessage();

    //for gas data
    mqttClient.beginMessage(topic_gas);
    mqttClient.publish(gas_sensor());
    mqttClient.endMessage();

    //    for flame detection
    mqttClient.beginMessage(topic_flame);
    if (flame_sensor)
      mqttClient.publish("Flame detected");
    mqttClient.endMessage();
  }
}

//gas sensor code
int gas_sensor()
{
  sensorValue = analogRead(0);   // reads from A0 pin
  return (sensorValue);
}

// code for temprature sensor
float Temp_sensor()
{
  //  Read data and store it to variables hum and temp
  temp = (float) dht.readTemperature();

  return (temp);

}

float Hum_sensor()
{
  hum = dht.readHumidity();
  return (hum);
}

bool flame_sensor()
{
  Flame = digitalRead(flamePin);
  if (Flame == LOW)
  {
    return true;
  }
  else
  {
    return false;
  }
}
void servo() {
  for (pos = 0; pos <= 180; pos += 1)
  {
    // in steps of 1 degree
    myservo.write(pos);
    delay(15);
  }
}
