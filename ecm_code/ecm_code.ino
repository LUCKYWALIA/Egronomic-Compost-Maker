#include <SoftwareSerial.h>
#include <ModbusMaster.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define MAX485_DE_RE_PIN 7
#define NPK_RX_PIN 10
#define NPK_TX_PIN 11

#define DHT_PIN 2
#define DHT_TYPE DHT22

#define LCD_NPK_ADDR 0x27
#define LCD_T_H_ADDR 0x26

#define NPK_SLAVE_ID 1
#define NPK_START_REGISTER 30
#define NPK_REGISTER_COUNT 3

SoftwareSerial rs485(NPK_RX_PIN, NPK_TX_PIN);
ModbusMaster node;
DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcdNPK(LCD_NPK_ADDR, 16, 2);
LiquidCrystal_I2C lcdTH(LCD_T_H_ADDR, 16, 2);

uint16_t N = 0;
uint16_t P = 0;
uint16_t K = 0;
float humidity = 0.0;
float temperature = 0.0;
unsigned long previousMillis = 0;
const long interval = 2000;

void preTransmission();
void postTransmission();
void readNPK();
void readDHT();
void displayData();
void initializeLCD(LiquidCrystal_I2C &lcd, const char* title);

void setup() {
  Serial.begin(9600);

  pinMode(MAX485_DE_RE_PIN, OUTPUT);
  digitalWrite(MAX485_DE_RE_PIN, LOW);
  rs485.begin(4800);

  node.begin(NPK_SLAVE_ID, rs485);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  dht.begin();

  Wire.begin();
  initializeLCD(lcdNPK, "Soil NPK");
  initializeLCD(lcdTH, "Temp/Humidity");

  lcdNPK.print("Sensor Monitor");
  lcdNPK.setCursor(0, 1);
  lcdNPK.print("NPK Ready...");

  lcdTH.print("Sensor Monitor");
  lcdTH.setCursor(0, 1);
  lcdTH.print("T/H Ready...");
  delay(2000);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    readNPK();
    readDHT();
    displayData();
  }
}

void preTransmission() {
  digitalWrite(MAX485_DE_RE_PIN, HIGH);
}

void postTransmission() {
  digitalWrite(MAX485_DE_RE_PIN, LOW);
}

void readNPK() {
  N = 0;
  P = 0;
  K = 0;

  uint8_t result;

  result = node.readHoldingRegisters(NPK_START_REGISTER, NPK_REGISTER_COUNT);

  if (result == node.ku8MBSuccess) {
    N = node.getResponseBuffer(0);
    P = node.getResponseBuffer(1);
    K = node.getResponseBuffer(2);

    Serial.print("NPK Read Success: N=");
    Serial.print(N);
    Serial.print(", P=");
    Serial.print(P);
    Serial.print(", K=");
    Serial.println(K);

  } else {
    Serial.print("NPK Read Error: ");
    Serial.println(result, HEX);
    lcdNPK.clear();
    lcdNPK.print("NPK Error: 0x");
    lcdNPK.print(result, HEX);
  }
}

void readDHT() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("DHT22 Failed to read from sensor!");
  } else {
    Serial.print("T/H Read Success: Temp=");
    Serial.print(temperature);
    Serial.print("C, Hum=");
    Serial.print(humidity);
    Serial.println("%");
  }
}

void initializeLCD(LiquidCrystal_I2C &lcd, const char* title) {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print(title);
}

void displayData() {
  lcdNPK.clear();
  lcdNPK.setCursor(0, 0);
  lcdNPK.print("N:");
  lcdNPK.print(N);
  lcdNPK.print(" P:");
  lcdNPK.print(P);

  lcdNPK.setCursor(0, 1);
  lcdNPK.print("K:");
  lcdNPK.print(K);
  lcdNPK.print(" mg/kg");


  lcdTH.clear();
  lcdTH.setCursor(0, 0);
  if (!isnan(temperature)) {
    lcdTH.print("Temp: ");
    lcdTH.print(temperature, 1);
    lcdTH.print((char)223);
    lcdTH.print("C");
  } else {
    lcdTH.print("Temp: Error!");
  }

  lcdTH.setCursor(0, 1);
  if (!isnan(humidity)) {
    lcdTH.print("Hum : ");
    lcdTH.print(humidity, 1);
    lcdTH.print("%");
  } else {
    lcdTH.print("Hum : Error!");
  }
}