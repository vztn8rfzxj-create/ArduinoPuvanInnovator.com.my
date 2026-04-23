#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_TEMPLATE_ID "TMPL6iId1UnPT"
#define BLYNK_TEMPLATE_NAME "Fire alert"
#define BLYNK_AUTH_TOKEN "1XQIhaBwYa8adrYb1wCxnzNT69MuJiqT"


// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Blynk credentials
char auth[] = "YOUR_BLYNK_AUTH_TOKEN";

// Fire sensor pin
const int fireSensorPin = 34;

// Telegram Bot
const char* botToken = "7476740140";
const char* chatID = "";

const char* telegramHost = "api.telegram.org";
const int httpsPort = 443;

WiFiClientSecure client;

bool fireDetected = false;

void setup() {
  Serial.begin(115200);
  pinMode(fireSensorPin, INPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  client.setInsecure();  // Don't check certificate
  Blynk.begin(auth, ssid, password);
}

void loop() {
  Blynk.run();

  int fireValue = digitalRead(fireSensorPin);
  Serial.print("Fire Sensor Value: ");
  Serial.println(fireValue);

  if (fireValue == LOW && !fireDetected) {
    fireDetected = true;
    Serial.println("🔥 FIRE DETECTED!");
    Blynk.logEvent("fire_alert", "🔥 Fire detected!");
    sendTelegramMessage("🔥 Fire Detected! Take Action Immediately!");
  } else if (fireValue == HIGH && fireDetected) {
    fireDetected = false;
    Serial.println("✅ Fire Cleared");
    sendTelegramMessage("✅ Fire condition cleared.");
  }

  delay(2000);
}

void sendTelegramMessage(String message) {
  if (!client.connect(telegramHost, httpsPort)) {
    Serial.println("Connection to Telegram failed");
    return;
  }

  String url = "/bot" + String(botToken) + "/sendMessage?chat_id=" + chatID + "&text=" + urlEncode(message);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + telegramHost + "\r\n" +
               "Connection: close\r\n\r\n");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") break;
  }

  Serial.println("Telegram message sent (raw HTTPS).");
  client.stop();
}

String urlEncode(const String &text) {
  String encoded = "";
  char c;
  char code0, code1;
  for (int i = 0; i < text.length(); i++) {
    c = text.charAt(i);
    if (isalnum(c)) {
      encoded += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) code1 += 7;
      code0 = ((c >> 4) & 0xf) + '0';
      if (((c >> 4) & 0xf) > 9) code0 += 7;
      encoded += '%';
      encoded += code0;
      encoded += code1;
    }
  }
  return encoded;
}
