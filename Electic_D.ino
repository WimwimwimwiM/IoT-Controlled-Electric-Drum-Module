/*#define BLYNK_AUTH_TOKEN "JLxpwBmovEiKogt2vT5jLjeu2WqlFPkg"
#define WIFI_SSID "wimwim"
#define WIFI_PASS "12345678"
#define BLYNK_TEMPLATE_ID "TMPL6ksIe6aJG"
#define BLYNK_TEMPLATE_NAME "project"*/

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DFRobotDFPlayerMini.h>

HardwareSerial mySerial(2);

DFRobotDFPlayerMini Player;

int piezo_hat = 34;
int piezo_snare = 35;
int piezo_kick = 32;

int kick = 1;
int snare = 1;
int hi_hat = 1;
int set = 4;
int volume = 25; // ตั้งระดับเสียง (0-30)

unsigned long previousMillis = 0;  
const long interval = 100;         

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, 16, 17);
  // เชื่อมต่อ WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi Connected!");

  // เชื่อมต่อ Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

  while (!Player.begin(mySerial)) {
    delay(1000);
    Serial.println("DFPlayer Mini not detected");
  }
  Serial.println("DFPlayer Mini online.");
  Player.volume(volume);  

  analogReadResolution(12);
  analogSetWidth(12);

  pinMode(piezo_hat, INPUT);
  pinMode(piezo_snare, INPUT);
  pinMode(piezo_kick, INPUT);
}
BLYNK_WRITE(V0) {
  int menu_kick = param.asInt();
  switch (menu_kick) {
    case 0:
      Serial.println("Change to kick1!");
      kick = 1;
      break;
    case 1:
      Serial.println("Change to kick2!");
      kick = 2;
      break;
}
}
BLYNK_WRITE(V1) {
  int menu_snare = param.asInt();

  switch (menu_snare) {
    case 0:
      Serial.println("Change to snare1!");
      snare = 1;
      set = 4;
      break;
    case 1:
      Serial.println("Change to snare2!");
      snare = 2;
      set = 5;
      break;
}
}
BLYNK_WRITE(V2) {
  int menu_hat = param.asInt();

  switch (menu_hat) {
    case 0:
      Serial.println("Change to hat1!");
      hi_hat = 1;
      break;
    case 1:
      Serial.println("Change to hat2!");
      hi_hat = 2;
      break;
}
}
void loop() {
  int connect_wifi = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    connect_wifi = 1;
  }
  if (connect_wifi == 1) {
    Serial.println("WiFi Connected!");
    Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
  }

  Blynk.run();

  int value_snare = analogRead(piezo_snare);
  int value_kick = analogRead(piezo_kick);
  int value_hat = analogRead(piezo_hat);

  unsigned long currentMillis = millis();  
  if (value_snare > 350 && (currentMillis - previousMillis >= interval )) {
    previousMillis = currentMillis;
    Player.playFolder(1,snare);  
     delay(50);
  }

  if (value_kick > 400 && (currentMillis - previousMillis >= interval)) {
    previousMillis = currentMillis;
    Player.playFolder(2, kick);  

  }

  if (value_hat > 600 && (currentMillis - previousMillis >= interval )) {
    previousMillis = currentMillis;
    Player.playFolder(3,hi_hat); 
    delay(50);
  }
}