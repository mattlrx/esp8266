#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>

#define PIN 5

const char WiFiAPPSK[] = "12345678";

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

WiFiServer server(80);
int r = 0;
int g = 0;
int b = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupWiFi();
  server.begin();
  strip.begin();
  strip.show();

}

void loop() {
  // put your main code here, to run repeatedly:

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
//  client.flush();

  // Match the request
  // request type (read/set) and value if set.
  if (req.indexOf("/greenplus") != -1){
    g = g+20;
    if (g > 255){
      g = 255;
    }
  }
  else if (req.indexOf("/redplus") != -1){
    r = r+20;
    if (r> 255){
      r = 255;
    }
  }
  else if (req.indexOf("/blueplus") != -1){
    b = b+20;
    if (b > 255){
      b = 255;
    }
  }
  else if (req.indexOf("/greenminus") != -1){
    g = g-20;
    if (g < 1){
      g = 0;
    }
  }
  else if (req.indexOf("/redminus") != -1){
    r = r-20;
    if (r < 1){
      r = 0;
    }
  }
  else if (req.indexOf("/blueminus") != -1){
    b = b-20;
    if (b <1){
      b = 0;
    }
  }
  else if (req.indexOf("/off") != -1){
    g = 0;
    r = 0;
    b = 0;
  }
  strip.setPixelColor(0, strip.Color(g, r, b));
  strip.show();


//  client.flush();
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
s = "<!DOCTYPE html><html>";
s += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
s += "<link rel=\"icon\" href=\"data:,\">";
s += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
s += ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
s += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
s += ".button2 {background-color: #77878A;}</style></head>";
s += "<body><h1>ESP8266 Web Server</h1>";
s += "<p>red: "+String(r)+", green: "+String(g)+", blue: "+String(b)+"</p>";
s += "<p><a href=\"/greenminus\"><button class=\"button\">-</button></a>GREEN<a href=\"/greenplus\"><button class=\"button\">+</button></a></p>";
s += "<p><a href=\"/redminus\"><button class=\"button\">-</button></a>  RED  <a href=\"/redplus\"><button class=\"button\">+</button></a></p>";
s += "<p><a href=\"/blueminus\"><button class=\"button\">-</button></a> BLUE <a href=\"/blueplus\"><button class=\"button\">+</button></a></p>";
//s += "<p><a href=\"/blueminus\"><button class=\"button\">BLUE</button></a></p>";
s += "<p><a href=\"/off\"><button class=\"button\">OFF</button></a></p>";
s += "</body></html>\n";


  // Send the response to the client
  Serial.println(s);
  client.println(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed

}

void setupWiFi()
{
  WiFi.mode(WIFI_AP);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "ESP8266 Thing " + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
}
