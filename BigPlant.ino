//THINGSPEAK
#define IP "184.106.153.149"                        // IP address of thingspeak.com
String GET = "GET /update?key=W48RXBQH6XJOLHK4";    // replace with your channel key

//ESP-01
#include <SoftwareSerial.h>
SoftwareSerial esp8266Module( 3, 2 ); // RX, TX
#define DEBUG 0
String network = "MOVISTAR_A334";                            // your access point SSID
String password = "navidad2015";                  // your wifi Access Point password

//DHT-22
#include <DHT.h>
#define DHTPIN 7
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//PINS
const int redLed = 12;
const int greenLed = 11;
const int blueLed = 10;
const int moistPin = A0;
const int relay = A3;
const int lightPin = A6;

//DATA VALUES
int soil, light, cont = 0;
double airTemp, airHum, hi;

//CONTROL PARAMETERS
#define WET_SOIL 85
#define DRY_SOIL 66
#define PUMP_TIME 10
#define DELAY 300000

void setup() {
  dht.begin();

  Serial.begin(9600);
  esp8266Module.begin(11520);

  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(moistPin, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(lightPin, INPUT);
  ledInit();
}

void loop() {
  if (cont < 100) {
    readSensors();
    displayValues();
    leds();
    delay(500);
    cont++;
  } else {
    readSensors();
    displayValues();

    if (light <= 30 && soil <= DRY_SOIL) {
      water(PUMP_TIME);
      readSensors();
    }

    //writeThingSpeak();

    leds();
    delay(DELAY);
  }


}

void writeThingSpeak() {
  delay(50);
  digitalWrite(redLed, HIGH);
  digitalWrite(blueLed, HIGH);
  digitalWrite(greenLed, LOW);
  updateTemp(String(soil), String(airTemp), String(light), String(airHum), String(hi));
  leds();

  delay(20000);
}

void readSensors() {
  airTemp = dht.readTemperature();
  airHum = dht.readHumidity();
  soil = map(analogRead(moistPin), 570, 270, 0, 100);
  light = map(analogRead(lightPin), 0, 1023, 0, 100);
  hi = dht.computeHeatIndex(airTemp, airHum, false);
}

void displayValues() {
  Serial.println("-------------------------");
  Serial.print(airTemp);
  Serial.println("ºC");
  Serial.print(hi);
  Serial.println("ºC");
  Serial.print(airHum);
  Serial.println("%");
  Serial.print(light);
  Serial.println(" light");
  Serial.print(soil);
  Serial.println("%");
  Serial.println("-------------------------");
}

void water(int s) {
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, HIGH);
  digitalWrite(blueLed, HIGH);
  digitalWrite(relay, HIGH);
  delay(s * 1000);
  digitalWrite(relay, LOW);
  leds();
}

void leds() {
  if (soil <= DRY_SOIL) {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(blueLed, LOW);
  }
  if (soil > DRY_SOIL && soil <= WET_SOIL) {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, LOW);
    digitalWrite(blueLed, HIGH);
  }
  if (soil > WET_SOIL) {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(blueLed, LOW);
  }
}

void ledInit() {
  digitalWrite(redLed, HIGH);
  delay(150);
  digitalWrite(redLed, LOW);
  digitalWrite(blueLed, HIGH);
  delay(150);
  digitalWrite(blueLed, LOW);
  digitalWrite(greenLed, HIGH);
  delay(150);
  digitalWrite(greenLed, LOW);
}

void setupEsp8266()
{
  Serial.println("Setting up ESP . . .");

  Serial.println("Reseting esp8266");

  esp8266Module.flush();
  esp8266Module.println(F("AT+RST"));
  delay(7000);
  if (esp8266Module.find("OK"))
  {

    Serial.println("Found OK");
    Serial.println("Changing espmode");

    esp8266Module.flush();
    changingMode();
    delay(5000);
    esp8266Module.flush();
    connectToWiFi();
  }
  else
  {
    Serial.println("OK not found");
  }
}

//-------------------------------------------------------------------
// Following function sets esp8266 to station mode
//-------------------------------------------------------------------
bool changingMode()
{
  Serial.println("Changing mode . . .");
  esp8266Module.println(F("AT+CWMODE=1"));
  if (esp8266Module.find("OK"))
  {
    Serial.println("Mode changed");
    return true;
  }
  else if (esp8266Module.find("NO CHANGE")) {
    Serial.println("Already in mode 1");
    return true;
  }
  else
  {
    Serial.println("Error while changing mode");
    return false;
  }
}

//-------------------------------------------------------------------
// Following function connects esp8266 to wifi access point
//-------------------------------------------------------------------
bool connectToWiFi()
{
  Serial.println("Connecting to WiFi . . .");
  Serial.println("inside connectToWiFi");
  String cmd = F("AT+CWJAP=\"");
  cmd += network;
  cmd += F("\",\"");
  cmd += password;
  cmd += F("\"");
  esp8266Module.println(cmd);
  delay(15000);

  if (esp8266Module.find("OK"))
  {
    Serial.println("Connected to Access Point");
    return true;
  }
  else
  {
    Serial.println("Could not connect to Access Point");
    return false;
  }
}

//-------------------------------------------------------------------
// Following function sends sensor data to thingspeak.com
//-------------------------------------------------------------------
void updateTemp(String voltage1, String voltage2, String voltage3, String voltage4, String voltage5)
{
  digitalWrite(redLed, HIGH);
  digitalWrite(blueLed, HIGH);
  digitalWrite(greenLed, LOW);

  Serial.println("Updating values . . .");
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  esp8266Module.println(cmd);
  delay(5000);
  if (esp8266Module.find("Error")) {
    Serial.println("ERROR while SENDING");
    return;
  }
  cmd = GET + "&field1=" + voltage1 + "&field2=" + voltage2 + "&field3=" + voltage3 + "&field4=" + voltage4 + "&field5=" + voltage5 + "\r\n";
  esp8266Module.print("AT+CIPSEND=");
  esp8266Module.println(cmd.length());
  delay(15000);
  if (esp8266Module.find(">"))
  {
    esp8266Module.print(cmd);
    Serial.println("Data sent");
  } else
  {
    esp8266Module.println("AT+CIPCLOSE");
    Serial.println("Connection closed");
  }
}

