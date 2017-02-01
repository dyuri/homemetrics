#include <Adafruit_TCS34725.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

// web server related
TCPClient webClient;
TCPServer webServer = TCPServer(80);
char myIpAddress[24];

// loop counter, measurements in every LOOPCOUNT*LOOPLENGTH msec
int cLoop = 0;
const int LOOPCOUNT = 50;
const int LOOPLENGTH = 100;

// led controller pin
int tcsLed = D2;

// data
uint16_t r, g, b, c, colorTemp, lux;
float temp, pressure, hum;

void setup() {
  // temp+hum+press sensor
  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  } else {
    Serial.println("BME280 initialized.");
  }
  
  // color sensor
  if (!tcs.begin()) {
    Serial.println("No TCS34725 found, check wiring!");
    while (1);
  } else {
    Serial.println("Color sensor initialized.");
  }

  // webserver
  Particle.variable("ipAddress", myIpAddress, STRING);
  IPAddress myIp = WiFi.localIP();
  sprintf(myIpAddress, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
  Serial.print("Webserver IP: ");
  Serial.println(myIpAddress);

  webServer.begin();

  pinMode(tcsLed, OUTPUT);
  digitalWrite(tcsLed, LOW);
}

void loop() {
  // TODO turn on led on button
  // digitalWrite(tcsLed, HIGH);
  // digitalWrite(tcsLed, LOW);

  if (cLoop % LOOPCOUNT == 0) {
    cLoop = 0;
    
    temp = bme.readTemperature();
    pressure = bme.readPressure() / 100.0F;
    hum = bme.readHumidity();

    tcs.getRawData(&r, &g, &b, &c);
    colorTemp = tcs.calculateColorTemperature(r, g, b);
    lux = tcs.calculateLux(r, g, b);

    printDataSerial();
  }

  cLoop++;

  // web serve
  if (webClient.connected() && webClient.available()) {
    serveWebpage();
  } else {
    webClient = webServer.available();
  }

  delay(LOOPLENGTH);
}

void printDataSerial() {
  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(hum);
  Serial.println(" %");
  Serial.println();

  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println();
  
  Serial.println();
}

void serveWebpage() {
  webClient.println("HTTP/1.1 200 OK");
  webClient.println("Connection: close");
  webClient.println();
  
  webClient.print("Temperature: ");
  webClient.print(temp);
  webClient.println(" *C");
  
  webClient.print("Humidity: ");
  webClient.print(hum);
  webClient.println(" %");

  webClient.print("Air Pressure: ");
  webClient.print(pressure);
  webClient.println(" hPa");
  
  webClient.print("Light: ");
  webClient.print(lux);
  webClient.println(" lux");

  webClient.print("Color Red: ");
  webClient.println(r);
  webClient.print("Color Green: ");
  webClient.println(g);
  webClient.print("Color Blue: ");
  webClient.println(b);

  
  webClient.flush();
  webClient.stop();
}
