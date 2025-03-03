#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>
#include <WiFiNINA.h>
#include <Stepper.h>
#include <SoftwareSerial.h>

// SoftwareSerial on pins 0 (SRX) and 1 (STX)
SoftwareSerial SUART(0, 1);

// Hotspot credentials
char ssid[] = "Group_11";
char pass[] = "Password123!";

int status = WL_IDLE_STATUS;
WiFiServer server(80);

// Stepper motor pins
const int stepPin = 7;
const int dirPin = 6;

const int stepsPerRevolution = 400;
int stepDelay = 4000;
bool clockwise = true;

// Analog pins for potentiometers
const int slidePotPin = A0;
const int slidePotPin1 = A1;

// Flags for handling asynchronous requests
bool rotationRequested = false;
bool autoModeRequested = false;
bool manualModeRequested = false;
bool sensor1Requested = false;
bool sensor2Requested = false;
bool codifRequested = false;
bool vel1Requested = false;
bool vel2Requested = false;

// Encoder setup
Encoder myEnc(2, 3);
long counter = 0;

void setup()
{
    Serial.begin(9600);
    SUART.begin(9600);

    pinMode(dirPin, OUTPUT);
    pinMode(stepPin, OUTPUT);

    digitalWrite(dirPin, clockwise ? HIGH : LOW);

    Serial.println("Access Point Web Server");

    if (WiFi.status() == WL_NO_MODULE)
    {
        Serial.println("Communication with WiFi module failed!");
        while (true);
    }

    // Create a Wi-Fi access point
    Serial.print("Creating access point named: ");
    Serial.println(ssid);
    status = WiFi.beginAP(ssid, pass);
    if (status != WL_AP_LISTENING)
    {
        Serial.println("Creating access point failed");
        while (true);
    }

    delay(10000);

    // Start the web server
    server.begin();
}

void loop()
{
    // Update encoder position
    counter = myEnc.read();

    handleRequests();
}

// Function to handle HTTP requests and set flags accordingly
void handleRequests()
{
    WiFiClient client = server.available();
    if (!client)
    {
        return;
    }

    while (!client.available())
    {
        delay(1);
    }

    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Check for specific commands in the request
    if (request.indexOf("/?angle=") != -1)
        rotationRequested = true;
    else if (request.indexOf("/?auto") != -1)
        autoModeRequested = true;
    else if (request.indexOf("/?manual") != -1)
        manualModeRequested = true;
    else if (request.indexOf("/?sensor1") != -1)
        sensor1Requested = true;
    else if (request.indexOf("/?sensor2") != -1)
        sensor2Requested = true;
    else if (request.indexOf("/?codif") != -1)
        codifRequested = true;
    else if (request.indexOf("/?vel1") != -1)
        vel1Requested = true;
    else if (request.indexOf("/?vel2") != -1)
        vel2Requested = true;

    // Handle flagged requests
    if (rotationRequested)
    {
        rotateStepperAsync(request);
        rotationRequested = false;
    }
    else if (autoModeRequested)
    {
        autoStepperAsync(true);
        autoModeRequested = false;
    }
    else if (manualModeRequested)
    {
        autoStepperAsync(false);
        manualModeRequested = false;
    }
    else if (sensor1Requested)
    {
        sendSensorValueAsync(slidePotPin);
        sensor1Requested = false;
    }
    else if (sensor2Requested)
    {
        sendSensorValueAsync(slidePotPin1);
        sensor2Requested = false;
    }
    else if (codifRequested)
    {
        sendCodifValueAsync();
        codifRequested = false;
    }
    else if (vel1Requested)
    {
        setVelocity(8000);
        vel1Requested = false;
    }
    else if (vel2Requested)
    {
        setVelocity(16000);
        vel2Requested = false;
    }
}

// Set velocity by sending command over SoftwareSerial
void setVelocity(int stepDelay)
{
    SUART.println("vel " + String(stepDelay));
}

// Rotate stepper motor based on angle from request
void rotateStepperAsync(String request)
{
    int angleIndex = request.indexOf("/?angle=") + 8;
    int angle = request.substring(angleIndex).toInt();
    SUART.println("angle " + String(angle));
}

// Enable or disable auto mode
void autoStepperAsync(bool isAutoMode)
{
    SUART.println(isAutoMode ? "auto" : "stop");
}

// Send sensor value to client
void sendSensorValueAsync(int pin)
{
    int sensorValue = analogRead(pin);
    int mappedValue = map(sensorValue, 0, 1023, 0, 30);

    WiFiClient client = server.available();
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println(String(mappedValue));
}

// Send encoder angle to client
void sendCodifValueAsync()
{
    float angle_codif = counter * 360.0 / 2400.0;

    WiFiClient client = server.available();
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println(String(angle_codif));
}