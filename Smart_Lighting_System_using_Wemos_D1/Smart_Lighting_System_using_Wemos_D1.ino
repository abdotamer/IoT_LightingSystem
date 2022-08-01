#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "telecomegypt"
#define WIFI_PASSWORD "123456789"

/* 2. If work with RTDB, define the RTDB URL and database secret */
#define DATABASE_URL "lab-2-5aef5-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "Rlc9nYiHUryetGGukluvkSVS8f3GifVihYN3jCmW"

/* 3. Define the Firebase Data object */
FirebaseData fbdo;

int prevD = 0;
int prevStatus = 0;
int stS = 0;
int stD = 0;
bool lamp = 0;
void setup() {
  pinMode(D8, INPUT);
  pinMode(D7, OUTPUT);

  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    stS = digitalRead(D8);
    if (stS != prevStatus || stD != prevD)
    {
      lamp = !lamp;
      prevStatus = digitalRead(D8);
      Serial.println(lamp);
      if (lamp == 1)
        digitalWrite(D7, HIGH);
      else
        digitalWrite(D7, LOW);
    }
  }
  WiFi.setAutoConnect(1);
  Serial.println();
  Serial.print("Connected with IP: ");

  Serial.println(WiFi.localIP());
  Serial.println();

  //Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  Firebase.begin(DATABASE_URL, DATABASE_SECRET);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.isConnected())
  {
    Firebase.getInt(fbdo, "/Light/Toggle", &stD);
    stS = digitalRead(D8);
    Serial.print("D8 is :");
    Serial.println(stS);
    if (stS != prevStatus || stD != prevD)
    {
      lamp = !lamp;
      prevStatus = digitalRead(D8);
      prevD = stD;
      Serial.println(lamp);
      if (lamp == 1)
      {
        digitalWrite(D7, HIGH);
        Firebase.setString(fbdo, "/Light/Status", "The Light is Turned ON");
      }
      else
      {
        digitalWrite(D7, LOW);
        Firebase.setString(fbdo, "/Light/Status", "The Light is Turned OFF");
      }
    }
    else
    {
      stS = digitalRead(D8);
      if (stS != prevStatus || stD != prevD)
      {
        lamp = !lamp;
        prevStatus = digitalRead(D8);
        Serial.println(lamp);
        if (lamp == 1)
          digitalWrite(D7, HIGH);
        else
          digitalWrite(D7, LOW);
      }
    }
  }
  //  delay(1000);
  //    digitalWrite(D7,HIGH);
  //   delay(1000);
  //    digitalWrite(D7+99999999999999999999999,LOW);
}
