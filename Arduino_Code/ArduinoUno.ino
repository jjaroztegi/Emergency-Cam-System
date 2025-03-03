#include <SoftwareSerial.h>

// SoftwareSerial on pins 0 (SRX) and 1 (STX), typically hardware serial pins
SoftwareSerial SUART(0, 1);

const int stepPin = 7; // Stepper motor step pin
const int dirPin = 6;  // Stepper motor direction pin

bool autoMode = false; // Auto mode flag
bool clockwise = true; // Rotation direction for auto mode

const int stepsPerRevolution = 400; // Steps for one full revolution
int stepDelay = 4000;               // Delay between steps in microseconds (speed control)

void setup()
{
  Serial.begin(9600); // Hardware serial for debugging
  SUART.begin(9600);  // Software serial

  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);

  digitalWrite(dirPin, clockwise ? HIGH : LOW); // Set initial direction
}

// Function to step the motor a specified number of steps in a given direction
void stepMotor(int steps, bool clockwise)
{
  digitalWrite(dirPin, clockwise ? HIGH : LOW);
  for (int i = 0; i < steps; i++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepDelay);
  }
}

void loop()
{
  if (SUART.available() > 0)
  {
    String input = SUART.readStringUntil('\n');
    Serial.print("Received command: ");
    Serial.println(input);

    if (input.startsWith("auto"))
    {
      autoMode = true;
      Serial.println("Auto mode enabled");
      // Continuously rotate 180 degrees back and forth until stopped
      while (autoMode)
      {
        stepMotor(stepsPerRevolution / 2, clockwise);
        Serial.println("Automatic rotation");
        clockwise = !clockwise; // Alternate direction

        if (SUART.available() > 0)
        {
          String command = SUART.readStringUntil('\n');
          Serial.print("Received command in auto mode: ");
          Serial.println(command);
          if (command.startsWith("stop"))
          {
            autoMode = false;
            Serial.println("Auto mode disabled");
          }
          else if (command.startsWith("vel"))
          {
            int pos = command.indexOf(" ");
            if (pos != -1)
            {
              stepDelay = command.substring(pos + 1).toInt();
              Serial.print("Set velocity: ");
              Serial.println(stepDelay);
            }
          }
        }
      }
    }
    else if (input.startsWith("angle"))
    {
      int pos = input.indexOf(" ");
      if (pos != -1)
      {
        int degrees = input.substring(pos + 1).toInt();
        Serial.print("Received angle: ");
        Serial.println(degrees);

        // Rotate based on angle; negative for counterclockwise
        bool clockwise = degrees >= 0;
        degrees = abs(degrees);
        float steps = (degrees / 360.0) * stepsPerRevolution;
        stepMotor(steps, clockwise);
      }
    }
    else if (input.startsWith("stop"))
    {
      autoMode = false;
      Serial.println("Auto mode disabled");
    }
    else if (input.startsWith("vel"))
    {
      int pos = input.indexOf(" ");
      if (pos != -1)
      {
        stepDelay = input.substring(pos + 1).toInt();
        Serial.print("Set velocity: ");
        Serial.println(stepDelay);
      }
    }
  }
}