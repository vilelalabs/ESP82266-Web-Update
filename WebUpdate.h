/*
  WebUpdate.h

  ESP WiFi Web Update
  @author:    Henrique Vilela
  @version:   1.0
  @date:      2022-04-02

  @description:
    This code makes possible to update ESP8266 using a web server.
  @observations:
    To upload through terminal you can use: curl -F "image=@firmware.bin"
    "host"/update
*/

#ifndef __WEBUPDATE__H__
#define __WEBUPDATE__H__

#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

#include "acesso.h"

#ifndef STASSID
#define STASSID MINHA_REDE
#define STAPSK SENHA_REDE
#endif

const char* host = "esp8266-webupdate";
const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

/*
*  @usage: 
*   put this function in setup()
*  @params:
*   IP octets where the device will be found on local network
*   separated by commas.  
*   example: WebUpdateSetup(19,168,1,199);
*/

void WebUpdateSetup(uint8_t first_octet,
                    uint8_t second_octet,
                    uint8_t third_octet,
                    uint8_t fourth_octet) {
  Serial.begin(115200);

  // 
  IPAddress ip(first_octet, second_octet, third_octet, fourth_octet);
  IPAddress gateway(first_octet, second_octet, third_octet, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress DNS_1(8, 8, 8, 8);
  IPAddress DNS_2(8, 8, 4, 4);

  Serial.println();
  Serial.println("Booting Sketch...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.config(ip, gateway, subnet, DNS_1, DNS_2);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println("WiFi failed, retrying.");
  }

  MDNS.begin(host);

  httpUpdater.setup(&httpServer);

  Serial.println("\nConnected to " + WiFi.SSID() + "\nIP address: " +
                 WiFi.localIP().toString());

  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
  Serial.printf(
      "HTTPUpdateServer ready! Open http://%s.local/update in your browser\n",
      host);

  // Root page on the web server
  httpServer.on("/", [first_octet, second_octet, third_octet, fourth_octet]() {
    httpServer.send(200, "text/plain",
                    "Connected to ESP8266 at IP: " + (String)first_octet +
                        "." + (String)second_octet + "." + (String)third_octet +
                        "." + (String)fourth_octet);
  });
}

/*
*  @usage:
*   Put this function in loop()
*/
void WebUpdateLoop(void) {
  httpServer.handleClient();
  MDNS.update();
}

#endif  //!__WEBUPDATE__H__