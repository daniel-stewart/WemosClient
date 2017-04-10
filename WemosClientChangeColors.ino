#include <ESP8266WiFi.h>

const char* ssid = "****";
const char* password = "****";

const char* host = "XXX.XXX.XXX.XXX"; // IP address of server

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  delay(5000);
  if (value > 4)
    return;

  Serial.print("Connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort))
  {
    Serial.println("Connection failed");
    return;
  }

  // Now create the URI for request
  String url = "/LED?external=";
  switch (value) {
    case 4:
      url += "off";
      break;
    case 1:
      url += "red";
      break;
    case 2:
      url += "green";
      break;
    case 3:
      url += "yellow";
      break;
    case 0:
      url += "orange";
      break;
    default:
      return;
  }
  value++;
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0)
  {
    if (millis() - timeout > 5000)
    {
      Serial.println(">>> Client timeout!");
      client.stop();
      return;
    }
  }

  while (client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("Closing connection");
}
