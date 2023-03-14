#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

enum direction_t {
  DOWN,
  UP,
};

struct valueAndDir {
  int value;
  direction_t dir;
};

direction_t dir = UP;
int lastVal;

// const char *ssid = "thepromisedLAN";
// const char *password = "67192893961044349985";
// const char *url = "http://grow-active:8000/activity";
// const char *ssid = "Pixel 6";
// const char *password = "t6bzeq2knmyr6yc";
const char *ssid = "CityLAB-Guest";
const char *password = "Tempelhof";
const char *url = "https://werkverzeichnis.orijauch.de/activity";
// const char *url = "http://192.168.82.119:8000/activity";

const char IRG_Root_X1 [] PROGMEM = R"CERT()
  -----BEGIN CERTIFICATE-----
  MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
  TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
  cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
  WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
  ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
  MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
  h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
  0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
  A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
  T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
  B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
  B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
  KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
  OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
  jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
  qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
  rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
  HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
  hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
  ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
  3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
  NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
  ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
  TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
  jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
  oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
  4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
  mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
  emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
  -----END CERTIFICATE-----
)CERT";

X509List cert(IRG_Root_X1);

const size_t BUFLEN = 10;
struct valueAndDir buffer[BUFLEN];
size_t bufStart;
size_t bufEnd;

void resetCB() {
  for (size_t i = 0; i < BUFLEN; ++i) {
    buffer[i].value = -1;
    buffer[i].dir = UP;
  }
  bufStart = 0;
  bufEnd = 1;
}

void putCB(int item, direction_t dir) {
  buffer[bufStart].value = item;
  buffer[bufStart].dir = dir;
  ++bufStart %= BUFLEN;
}

valueAndDir getCB() {
  valueAndDir valAndDir = buffer[bufEnd];
  ++bufEnd %= BUFLEN;
  return valAndDir;
}

void httpPost(int minutesVal) {
  WiFiClientSecure client;

  if ((WiFi.status() == WL_CONNECTED)) {
    // client.setTrustAnchors(&cert);
    client.setInsecure();
    client.connect(url, 443);
    HTTPClient https;

    if (https.begin(client, url)) {
      https.addHeader("Content-Type", "application/json");
      String buffer = "{\"minutes\":" + String(minutesVal) + "}";
      int httpResponseCode = https.POST(buffer);
      if (httpResponseCode < 0) {
        Serial.printf(https.errorToString(httpResponseCode).c_str());
      } else {
        Serial.printf("HTTP response code: %d\n", httpResponseCode);
      }
    }
    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect.\n");
  }
}

int readValue() {
  int val = analogRead(A0);
  if (val <= 8) {return 0;}
  if (val <= 210) {return 5;}
  if (val <= 410) {return 10;}
  if (val <= 610) {return 15;}
  if (val <= 810) {return 20;}
  if (val <= 1023) {return 25;}
  return 30;
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nConnected to WiFi network with IP address:");
  Serial.println(WiFi.localIP());

  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("Waiting for NTP time sync:");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println();
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time:");
  Serial.print(asctime(&timeinfo));

  pinMode(A0, INPUT);
  resetCB();
  lastVal = -1;
}

void loop() {
  int value = readValue();
  if (value > lastVal && dir == DOWN) {
    dir = UP;
    resetCB();
  } else if (value < lastVal && dir == UP) {
    dir = DOWN;
    resetCB();
  }
  valueAndDir vad = getCB();
  putCB(value, dir);
  Serial.printf("%d,", value);
  if (value == vad.value && value == lastVal && dir == vad.dir) {
    Serial.println();
    httpPost(value);
    while (true) {
      delay(150);
      if (readValue() > value) {
        break;
      }
      if (value != 0 && readValue() == 0) {
        break;
      }
    }
  } else {
    delay(150);
  }
  lastVal = value;
}