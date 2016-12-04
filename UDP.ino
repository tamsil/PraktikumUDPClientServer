#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "poltekbatam#45";
const char* password = "cdevfr$345";

WiFiUDP Udp;
unsigned int localUdpPort = 2000;   // UDP Port yang dibuka pada sisi Wemos
char incomingPacket[255];           // Variabel untuk menampung data yang diterima, dalam bentuk
byte array;
char sendPacket[10];                // Variabel untuk mengirim data

unsigned long previousMillis = 0;
const long interval = 1000;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
  Serial.begin(115200);
  Serial.println();
  // Koneksi ke WiFi
//  Serial.print("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  // Membuka akses UDP Port 
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(),
  localUdpPort);
}

void loop()
{
  int packetSize = Udp.parsePacket();
  // Jika ada data yang masuk
  if (packetSize)
  { 
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize,
    Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    // Convert byte array ke string
    String data(incomingPacket);
    // Cek data
    if (data.equals("On")) {
      digitalWrite(LED_BUILTIN, 0);
    }
    else if (data.equals("Off")) {
      digitalWrite(LED_BUILTIN, 1);
    }
  }

  unsigned long currentMillis = millis();
  // Kirim data tiap 1 second
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // Generate data random
    String tempVal = String(random(0, 100));
    // Convert string ke byte array
    tempVal.toCharArray(sendPacket, 10);
    // Kirim data ke IP dan Port Tujuan 
    Udp.beginPacket("10.14.3.128", 2000);
    Udp.write(sendPacket);
    Udp.endPacket();
  }
}
