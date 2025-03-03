#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Pin mappings for ESP8266
#define D0 16
#define D1 5  // I2C Bus SCL (clock)
#define D2 4  // I2C Bus SDA (data)
#define D3 0
#define D4 2  // Same as "LED_BUILTIN", but inverted logic
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS)
#define D9 3  // RX0 (Serial console)
#define D10 1 // TX0 (Serial console)

// Wi-Fi credentials for connecting to the network
const char *ssid = "Group_11";
const char *password = "Password123!";

// Web server on port 80
ESP8266WebServer server(80);

// Volatile variables for encoder counter (modified in ISRs)
volatile long previousCounter = 0;
volatile long counter = 0;

// ISR for pin D1 (Encoder A)
ICACHE_RAM_ATTR void encoderA()
{
    if (digitalRead(D1) == digitalRead(D2))
    {
        counter++;
    }
    else
    {
        counter--;
    }
}

// ISR for pin D2 (Encoder B)
ICACHE_RAM_ATTR void encoderB()
{
    if (digitalRead(D1) == digitalRead(D2))
    {
        counter--;
    }
    else
    {
        counter++;
    }
}

void setup()
{
    Serial.begin(9600);

    // Connect to Wi-Fi network
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    Serial.println(WiFi.localIP());

    // Configure encoder pins with pull-up resistors
    pinMode(D1, INPUT_PULLUP);
    pinMode(D2, INPUT_PULLUP);

    // Attach interrupts for encoder pins
    attachInterrupt(digitalPinToInterrupt(D1), encoderA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(D2), encoderB, CHANGE);

    // Start the web server and set up the root route
    server.begin();
    server.on("/", handleRoot);
}

void loop()
{
    server.handleClient();

    // Check if the encoder counter has changed
    if (counter != previousCounter)
    {
        float angle = getAngle();
        Serial.print(angle, 2);
        Serial.println();
        previousCounter = counter;
    }
}

float getAngle()
{
    return counter * 360.0 / 2400.0;
}

// Send current angle to client
void handleRoot()
{
    float angle = getAngle();
    server.send(200, "text/plain", "\n" + String(angle));
}