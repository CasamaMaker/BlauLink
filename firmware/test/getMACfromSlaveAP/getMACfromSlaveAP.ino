#include "WiFi.h"

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Iniciant l'escaneig de xarxes...");
  int n = WiFi.scanNetworks();
  Serial.println("Escaneig complet.");
  if (n == 0) {
    Serial.println("No s'han trobat xarxes.");
  } else {
    for (int i = 0; i < n; ++i) {
      // Comprovem si l'SSID és "Slave1"
      if (WiFi.SSID(i) == "Slave_1") {
        Serial.print("Xarxa trobada: ");
        Serial.println(WiFi.SSID(i));
        Serial.print("MAC de l'AP: ");
        Serial.println(WiFi.BSSIDstr(i));
        break; // Sortim del bucle si hem trobat la xarxa
      }
    }
  }
}

void loop() {
  // Aquí pots implementar qualsevol altra funcionalitat que necessitis
}
