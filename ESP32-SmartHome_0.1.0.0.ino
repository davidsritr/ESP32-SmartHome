#include <WiFi.h>
#include <DallasTemperature.h>
#include <OneWire.h>

WiFiServer server(80);

WiFiClient client;
char settemp = 25;
String  i;
const int pinCidlaDS = 4;
const int rele_18 = 18;
const int rele_19 = 19;
const int rele_21 = 21;
boolean topeni = false;
float temp_offset = 2;

String ClientRequest;
String myresultat;
String ReadIncomingRequest(){
  while(client.available()) {
ClientRequest = (client.readStringUntil('\r'));
 if ((ClientRequest.indexOf("HTTP/1.1")>0)&&(ClientRequest.indexOf("/favicon.ico")<0)){
myresultat = ClientRequest;
}
}
return myresultat;
}

// ­ instance oneWireDS z knihovny OneWire
OneWire oneWireDS(pinCidlaDS);
// ­ instance senzoryDS z knihovny DallasTemperature
DallasTemperature sensors(&oneWireDS);

double teplota_0;
double teplota_1;
double teplota_2;

void setup()
{
  Serial.begin(9600);
  
   delay(3000);
   Serial.println("START");
    WiFi.softAP("SmartHome","");
    Serial.println("Vytvoreno");
    Serial.println("IP SmartHome je:");
    Serial.println((WiFi.softAPIP()));
  sensors.begin();
  server.begin();
  pinMode(rele_18, OUTPUT);
  pinMode(rele_19, OUTPUT);  
  pinMode(rele_21, OUTPUT);  
}
void loop(void)
{
      client = server.available();
    if (!client) { return; }
    while(!client.available()){  delay(1); }
    i = (ReadIncomingRequest());
    sensors.requestTemperatures();
    
 teplota_0 = sensors.getTempCByIndex(0);
 teplota_1 = sensors.getTempCByIndex(1);
 teplota_2 = sensors.getTempCByIndex(2);
    
  if(topeni) { //topeni je zapnuto
      if(teplota_1 > settemp) {
        if((teplota_1 - settemp) > temp_offset) {
          topeni =  LOW;
          digitalWrite(rele_18, LOW);
        }
      }
    }
      else {
        if(teplota_1 < settemp) {
          if((settemp - teplota_1) > temp_offset) {
            topeni = HIGH;
            digitalWrite(rele_18, HIGH);
        }
      }
    }

    if (i.indexOf("Rele-on") > 0) {
      settemp += 1;
     } 
     if (i.indexOf("Rele-off") > 0) {
      settemp -= 1;
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
      client.println("<title>SmartHome ESP32</title>");
    client.println("</head>");
    client.println("<style>");
      client.println("body {background-color: powderblue;}");
      client.println("h1 {text-align: center;}");
    client.println("</style>");
    client.println("<body>");
      client.println("<h1 style=""color:#000000"">");
      client.println("SmartHome ESP32");
      client.println("</h1>");
      client.println("<span style=""color:#000000;font-size:14px"">");
      client.println("Topeni:");
      client.println("</span>");
      client.println("<span style=""color:#000000;font-size:14px"">");
      client.println(topeni);
      client.println("</span>");
      client.println("<br>");
      client.println("<span style=""color:#000000;font-size:14px"">");
      client.println("Teplota venku:");
      client.println("</span>");
      client.println("<span style=""color:#ff0000;font-size:14px"">");
      client.println(teplota_0);
      client.println("</span>");
      client.println("<span style=""color:#000000;font-size:14px"">");
      client.println("C");
      client.println("</span>");
      client.println("<br>");
      client.println("<span style=""color:#000000;font-size:14px"">");
      client.println("Teplota v predsini:");
      client.println("</span>");
      client.println("<span style=""color:#ff0000;font-size:14px"">");
      client.println(teplota_1);
      client.println("</span>");
      client.println("<span style=""color:#000000;font-size:14px"">");
      client.println("C");
      client.println("</span>");
      client.println("<br>");
      client.println("<span style=""color:#000000;font-size:14px"">");
      client.println("Teplota v kuchyni:");
      client.println("</span>");
      client.println("<span style=""color:#ff0000;font-size:14px"">");
      client.println(teplota_2);
      client.println("</span>");
      client.println("<span style=""color:#000000;font-size:14px"">");
      client.println("C");
      client.println("</span>");
      client.println("<br>");
      client.println("<span style=""color:#000000;font-size:14px"">");
      client.println("Teplota v pokoji:");
      client.println("</span>");
      client.println("<span style=""color:#ff0000;font-size:14px"">");
      client.println(teplota_1);
      client.println("</span>");
      client.println("<span style=""color:#000000;font-size:14px"">");
      client.println("C");
      client.println("</span>");
      client.println("<br>");
      client.println("Nastavena teplota: ");
      client.println("</span>");
      client.println("<span style=""color:#ff0000;font-size:14px"">");
      client.println(settemp);
      client.println("</span>");
      client.println("<br>");
      client.println("<a href=");
      client.println("Rele-on");
      client.println("><button>");
      client.println("Zvysit teplotu");
      client.println("</button></a>");
      client.println("<a href=");
      client.println("Rele-off");
      client.println("><button>");
      client.println("Snizit teplotu");
      client.println("</button></a>");
      client.println("<a href=");
      client.println("192.168.4.1");
      client.println("><button>");
      client.println("Ulozit");
      client.println("</button></a>");
      client.println("<br>");
    client.println("</body>");
    client.println("</html>");
    client.stop();
     delay(1);

}
