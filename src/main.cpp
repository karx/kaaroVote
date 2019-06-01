#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "ineedwifi";
const char *password = "tovotein91";

unsigned long check_wifi = 3000;

int touch_sensor_value[4];
const int VALUE_THRESHOLD = 25;
const int TOUCH_SENSITIVITY = 2;
const int ALL_TIME_INDICATOR_PIN = 2;

void lightUpLEDForTouchedIndex(int touchedIndex);
void lightDownLEDForTouchedIndex(int touchedIndex);

void httpHitForTouchedIndex(int touchedIndex);
bool assureTouchOfIndex(int index);
void refreshTouchValues();
int getTouchedIndex();
int getPinFromIndex(int index);
int getLEDPinFromIndex(int index);

void setup()
{

  Serial.begin(9600);
  touch_sensor_value[0] = 0;
  touch_sensor_value[1] = 100;
  touch_sensor_value[2] = 100;
  touch_sensor_value[3] = 100;
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(1, OUTPUT);
  delay(1000);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(ALL_TIME_INDICATOR_PIN, HIGH);
    delay(200);
    digitalWrite(ALL_TIME_INDICATOR_PIN, LOW);
    delay(200);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
}

void loop()
{

  if ((WiFi.status() == WL_CONNECTED))
  { //Check the current connection status
    refreshTouchValues();
    int touchedIndex = getTouchedIndex();
    assureTouchOfIndex(touchedIndex);
    if (touchedIndex >= 0 && assureTouchOfIndex(touchedIndex))
    {
      Serial.println('hit');
      lightUpLEDForTouchedIndex(touchedIndex);
      httpHitForTouchedIndex(touchedIndex);
      lightDownLEDForTouchedIndex(touchedIndex);
    }
  }
  else if (millis() > check_wifi)
  {
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    check_wifi = millis() + 3000;
    while (WiFi.status() != WL_CONNECTED)
    {
      digitalWrite(ALL_TIME_INDICATOR_PIN, HIGH);
      delay(200);
      digitalWrite(ALL_TIME_INDICATOR_PIN, LOW);
      delay(200);
      Serial.println("Connecting to WiFi..");
    }
  }
}

void lightUpLEDForTouchedIndex(int touchedIndex)
{
  int LED_PIN_NUMBER = getLEDPinFromIndex(touchedIndex);

  digitalWrite(LED_PIN_NUMBER, HIGH);
  digitalWrite(ALL_TIME_INDICATOR_PIN, HIGH);
}
void lightDownLEDForTouchedIndex(int touchedIndex)
{
  int LED_PIN_NUMBER = getLEDPinFromIndex(touchedIndex);

  digitalWrite(LED_PIN_NUMBER, LOW);
  digitalWrite(ALL_TIME_INDICATOR_PIN, LOW);
}

void httpHitForTouchedIndex(int touchedIndex)
{
  char endpoint[] = "https://us-central1-pubpul.cloudfunctions.net/q1opt1";
  endpoint[51] = touchedIndex + '0' + 1;
  Serial.println(endpoint);
  HTTPClient http;
  http.begin(endpoint);      //Specify the URL
  int httpCode = http.GET(); //Make the request

  if (httpCode > 0)
  { //Check for the returning code

    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);
  }

  else
  {
    Serial.println("Error on HTTP request");
  }

  http.end(); //Free the resources
}

bool assureTouchOfIndex(int index)
{
  int pinToAssure = getPinFromIndex(index);
  for (int i = 0; i < TOUCH_SENSITIVITY; i++)
  {
    int touch_sensor_value = touchRead(pinToAssure);
    if (touch_sensor_value > VALUE_THRESHOLD)
    {
      return false;
    }
  }
  return true;
}

void refreshTouchValues()
{
  int i;
  for (i = 0; i < 3; i++)
  {
    touch_sensor_value[i] = touchRead(getPinFromIndex(i));
  }

  for (i = 0; i < 4; i++)
  {
    Serial.print("\t ");

    Serial.print(" : ");
    Serial.print(touch_sensor_value[i]);
  }

  Serial.println();
}

int getTouchedIndex()
{
  int i;
  for (i = 0; i < 4; i++)
  {
    if (touch_sensor_value[i] < VALUE_THRESHOLD)
      return i;
  }
  return -1;
}

int getPinFromIndex(int index)
{
  switch (index)
  {
  case 0:
    return T0;
    break;
  case 1:
    return T4;
    break;
  case 2:
    return T3;
    break;
  case 3:
    return T5;
    break;

  default:
    return -1;
    break;
  }
}

int getLEDPinFromIndex(int index)
{
  switch (index)
  {
  case 0:
    return 3;
    break;
  case 1:
    return 23;
    break;
  case 2:
    return 22;
    break;
  case 3:
    return 1;
    break;

  default:
    return -1;
    break;
  }
}