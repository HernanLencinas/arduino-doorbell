#include <ESP8266WiFi.h>
#include <PubSubClient.h>

int BUTTON_PIN = D2;

const char* ssid = "";
const char* password = "";
const char* mqtt_server = "";
const char* mqttUser = "";
const char* mqttPassword = "";
const String mqttTopic = "/iot/doorbell";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
String clientId = "esp8266-";
String macaddr;
String publishTopic;

void setup_wifi() {
    delay(100);
    Serial.println("");
    Serial.print("Connecting to ");
    Serial.print(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  
} 

void reconnect() {
  while (!client.connected()) 
  {
    Serial.println("Attempting MQTT connection...");
    macaddr = String(WiFi.macAddress());
    macaddr.replace(":","");
    macaddr.toLowerCase();
    clientId += macaddr;
    publishTopic = macaddr;
    publishTopic += mqttTopic;
    
    if (client.connect("esp8622", mqttUser, mqttPassword)) {
      Serial.print("MQTT connected [");
      Serial.print(clientId);
      Serial.println("]");
      client.subscribe("iot");
    } else {
      Serial.print("MQTT failed, rc=");
      Serial.print(client.state());
      Serial.println("MQTT try again in 5 seconds");
      delay(5000);
    }
  }
} 

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(BUTTON_PIN,INPUT);

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  int status;
  if (now - lastMsg > 1000) {
     lastMsg = now;
     client.publish( publishTopic.c_str(), "ring");

     Serial.println(digitalRead(BUTTON_PIN));
     
     /*status=digitalRead(BUTTON_PIN);
     Serial.println( digitalRead(BUTTON_PIN) );
     /*if(status==HIGH ) {
       Serial.println(status);
       status==LOW;
      /*
        msg= msg+ "Presionado";
        char message[58];
        msg.toCharArray(message,58);
        Serial.println(message);
        client.publish("iot/doorbell", message);
        status==LOW;
     }
     */


     
  }
}
