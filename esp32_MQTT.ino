#include <WiFi.h>
#include <PubSubClient.h>


// Set the network SSID/Password
const char* ssid = "steren_2_4G";
const char* password = "password";

// Add mosquitto MQTT Broker IP address
const char* mqtt_server = "192.168.1.6";//CUANDO NOS CONECTEMOS AL MODEM DE PRUEBA PARA QUE NO SALGA LA DEL TEC

//Define
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
float num = 0.0;
const int ledPin = 2;

const int State1=21;
const int State2=19;
const int sensor1=18;
const int sensor2=5;
const int sensor3=23;
const int sensor4=22;
int B1=0;
int B2=0;
int B3=0;
int B4=0;
bool state=0;
int t=0;

//Set-up configurations
void setup() {
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(ledPin, OUTPUT);

  //
  pinMode(State1,OUTPUT);
  pinMode(State2,OUTPUT);
  pinMode(sensor1,INPUT);
  pinMode(sensor2,INPUT);
  pinMode(sensor3,INPUT);
  pinMode(sensor4,INPUT);
}

//Connecting to WiFi network
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//Callback for MQTT received messages 
//Logica para encender Leds y leer estado de botones.
void callback(char* topic, byte* message, unsigned int length) {
  // If a message is received on the topic esp32/LED, you check if the message is either "on" or "off". 
  // Changes the output state according to the message

  B1=digitalRead(sensor1);
  B2=digitalRead(sensor2);
  B3=digitalRead(sensor3);
  B4=digitalRead(sensor4);

  if (state==0){
    digitalWrite(State1,HIGH);
    digitalWrite(State2,LOW);
  }
  
  else if (state==1){
    digitalWrite(State1,LOW);
    digitalWrite(State2,HIGH); 
  }

  if (B1==LOW or B2==LOW){
    state=0;
    t=0;
    Client.publish("esp32/B1", B1); 
  }

  else if (B3==LOW or B4==LOW){
    state=1;
    t=0;
    Client.publish("esp32/B1", B2);
  }
  t=t+100;
  if(t==5000){
    if (state==0){
      state=1;
      t=0;
    }
    else if (state==1){
      state=0;
      t=0;
    }
  }
delay(100);
Serial.println();
}

//Connecting to MQTT broker
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe to topics
      client.subscribe("esp32/S1");
      client.subscribe("esp32/S2");
      client.subscribe("esp32/S3");
      client.subscribe("esp32/S4");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    //Preparingn data to send
    num++;
    if (num>100) num=0;   
    
    // Convert the value to a char array
    char numString[8];
    dtostrf(num, 1, 2, numString);

    //Publish
    Serial.println("Sending message on topic: esp32/num");
    client.publish("esp32/num", numString);
    Serial.println("Sending message on topic: esp32/message");
    client.publish("esp32/message", "Hello World");

  }
}
