/*
   Futura Academy: https://academy.futuranet.it/
   Webinar 1: Esp32 corso base
   Dario Ciceri
   Canale YouTube: https://www.youtube.com/channel/UCuPuHsNjWX7huiztYu9ROQA
   Profilo Instagram: https://www.instagram.com/_dario.ciceri_/
*/

#define LED 2

#include <WiFi.h>

const char* ssid = "webinar";
const char* password = "12345678";

WiFiServer server(80);

String header;

String ledState = "off";

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.println("Connected!");
  Serial.print("IP ADDRESS: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {

          if (currentLine.length() == 0) {

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /LED/on") >= 0) {
              Serial.println("LED is on");
              ledState = "on";
              digitalWrite(LED, HIGH);
            } else if (header.indexOf("GET /LED/off") >= 0) {
              Serial.println("LED is off");
              ledState = "off";
              digitalWrite(LED, LOW);
            }

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");

            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #00ff44; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #ff0000;}</style></head>");

            client.println("<body><h1>Futura Academy: Webinar Base ESP32</h1>");

            client.println("<p>LED state " + ledState + "</p>");

            if (ledState == "off") {
              client.println("<p><a href=\"/LED/on\"><button class=\"button\">TURN ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/LED/off\"><button class=\"button button2\">TURN OFF</button></a></p>");
            }

            client.println("</body></html>");

            client.println();

            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    header = "";

    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
