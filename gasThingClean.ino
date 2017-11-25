#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define gazSensor D0

//Router parameters
const char* ssid     = "PLANET_D9CM";
const char* password = "F72KGLT3";

int gasValue;

char msg[] = "Gas Detected";
char mqttClientID[] = "gasThing";

// IP address of the MQTT broker
char server[] = { "iot.eclipse.org" };//iot.eclipse.org
int port = 1883 ;
char topic[] = { "amphie/gasLeakDetection" };

void connectToInternet() {
  delay(10);
  
  // We start by connecting to a WiFi network
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid);
  
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
  would try to act as both a client and an access-point and could cause
  network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("WiFi connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
}

boolean getGas() {
  int previousGasValue=digitalRead(gazSensor);
  delay(1000);
  gasValue=digitalRead(gazSensor);
  if(gasValue == 0 && gasValue != previousGasValue) {
    return true;
  }
  else
    return false;
}

WiFiClient wifiClient;
PubSubClient pubSubClient(server, port, 0, wifiClient);

void publishSensorData() {
  // Connect MQTT Broker
  Serial.println("");
  Serial.println("[INFO] Connecting to MQTT Broker");
  if (pubSubClient.connect(mqttClientID)) {
    Serial.println("[INFO] Connection to MQTT Broker Successful");
  }
  else {
    Serial.println("[ERROR] Connection to MQTT Broker Failed");
  }
  // Publish to MQTT Topic
  if (pubSubClient.connected()) {
    Serial.println("[INFO] Publishing to MQTT Broker");
    pubSubClient.publish(topic, msg);
    Serial.print("[INFO] Publish '");
    Serial.print(msg);
    Serial.println("' to MQTT Broker Complete");
  }
  else {
    Serial.println("[ERROR] Publish to MQTT Broker Failed");
  }
  pubSubClient.disconnect();
  Serial.println("[INFO] Disconnecting from MQTT Broker Complete");
  Serial.println("________________________________________________________");
}

void setup() {
  Serial.begin(115200);
  connectToInternet();
//  ESP.wdtDisable();
//  ESP.wdtEnable(WDTO_8S);
}

void loop() {
//  ESP.wdtFeed();
  if(getGas()) {
    publishSensorData();
    
  }
}
