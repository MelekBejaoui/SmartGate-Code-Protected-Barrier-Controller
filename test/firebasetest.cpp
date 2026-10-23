
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// =====================================================
// Wi-Fi
// =====================================================

const char* ssid = "TOPNET_9890";
const char* password = "cjuuie2aej";

// =====================================================
// Firebase credentials
// =====================================================

#define API_KEY "AIzaSyCKjqECLdnCVAt4iTMAdLiEia3TYJzND30"
#define DATABASE_URL "https://esp32-test-ff8bd-default-rtdb.firebaseio.com/"
#define USER_EMAIL "melekbejaoui2005@gmail.com"
#define USER_PASSWORD "Melek2005"

#define led 2

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool ledState = false;

void setup() {
  Serial.begin(115200);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  // ---------------------------------------------------
  // Connect to Wi-Fi
  // ---------------------------------------------------

  Serial.println();
  Serial.print("Connecting to Wi-Fi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("Wi-Fi connected!");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // ---------------------------------------------------
  // Firebase configuration
  // ---------------------------------------------------

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("Firebase initialized!");
}

void loop() {
  if (Firebase.ready()) {
    if (Firebase.RTDB.getBool(&fbdo, "/led")) {
      ledState = fbdo.boolData();

      digitalWrite(led, ledState ? HIGH : LOW);

      Serial.print("LED state: ");
      Serial.println(ledState ? "ON" : "OFF");
    } else {
      Serial.print("Firebase error: ");
      Serial.println(fbdo.errorReason());
    }
  }

  delay(150);
}
