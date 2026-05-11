// Include the "Wire.h" library - it is a built-in Arduino library 
// allowing hardware I2C communication (pins A4 and A5).
#include <Wire.h> 

// Include the external Adafruit library to handle our specific thermometer.
// Thanks to it, we don't have to write hundreds of lines of complex I2C code.
#include <Adafruit_MLX90614.h> 

// Macros (preprocessor directives). Before the program compiles, the compiler
// will replace every word "pir" with "8", "diode" with "9", etc. This doesn't use RAM!
#define pir 8
#define diode 9
#define fan 3

// A floating-point variable (float). It stores our threshold temperature.
float threshold = 26.0;

// Create an "object" named 'mlx' based on the 'Adafruit_MLX90614' class.
// It's as if we created a virtual representation of our physical sensor.
Adafruit_MLX90614 mlx = Adafruit_MLX90614();


// The setup() function runs only ONCE, after a reset or powering on.
void setup() {
  // Start the serial port (communication with PC) at a baud rate of 9600.
  Serial.begin(9600);
  
  // Pin configuration: PIR is an input (we read current from it).
  pinMode(pir, INPUT);
  
  // Fan and diode are outputs (we send current to them).
  pinMode(fan, OUTPUT);
  pinMode(diode, OUTPUT);
  
  // Safety measure: on startup, make sure the fan and diode are definitely turned off.
  digitalWrite(diode, LOW);
  digitalWrite(fan, LOW);
  
  // Attempt to initialize the I2C sensor (the begin() method from the mlx class).
  // The exclamation mark (!) means negation. We read this as: "If the sensor did NOT start correctly..."
  if(!mlx.begin())
  {
    // ...turn off the diode (just in case).
    digitalWrite(diode, LOW);
    // ...send an error to the computer.
    Serial.println("Sensor not found, check wiring!");
    // ...and fall into an infinite loop. while(1) is a loop that never ends.
    // We do this to halt the program - without the sensor, it makes no sense for the system to run.
    while(1); 
  }
  
  // If the program reached here, it means if(!mlx.begin()) didn't stop it.
  Serial.println("Cooling system ready!");
}

// The loop() function runs endlessly, thousands of times a second.
void loop() 
{
  // Call our diode blinking function (indicates that the program is running).
  blink();
  
  // Conditional statement checking the motion sensor. Calls our hasMotion() function.
  if(hasMotion())
  {
    // If the hasMotion() function returned "true" (presence detected):
    Serial.println("Someone is here!");
    
    // Turn the diode on constantly (active mode).
    digitalWrite(diode, HIGH);
    
    // Get the temperature from the sensor and save it to the local variable 'currentTemp'.
    float currentTemp = getTemperature();
    
    // Send the nicely formatted temperature text to the computer.
    Serial.println("Object temperature: ");
    Serial.print(currentTemp);
    Serial.println("*C");
    
      // If the read temperature is greater than our threshold (26.0)...
      if(currentTemp > threshold)
      {
        // ...we call the slow fan startup function (PWM).
        turnOn();
        
        // A for loop that will execute exactly 10 times (from h=0 to h=9).
        for(int h = 0; h < 10; h++)
        {
          // Make sure the fan gets full power.
          digitalWrite(fan, HIGH);
          // Wait 1 second (1000 milliseconds). 
          // Since the loop runs 10 times, the fan will be on for exactly 10 seconds.
          delay(1000);
        }
        
        // After 10 seconds, we call the slow fan braking function.
        turnOff();
      }
      else
      {
        // If there is motion, but it is NOT hot - make sure the fan is stopped.
        digitalWrite(fan, LOW);
      }
    
    // Wait 1 second before the next read of the entire cycle.
    delay(1000);
  }
  // If the hasMotion() function returned "false" (no one is in front of the desk):
  else
  {
    Serial.println("No motion. Sleep mode");
    // Make sure the fan is turned off.
    digitalWrite(fan, LOW);
  }
}

// ---------------- HELPER FUNCTIONS ----------------

// A function returning a boolean value (true/false). Checks the state of the PIR pin.
bool hasMotion()
{
  // Read the voltage on pin 8. It can only be HIGH (current is present) or LOW (no current).
  int state = digitalRead(pir);
  
  // If current is present (the sensor detected human heat)...
  if(state == HIGH)
    return true; // Return true to the main program.
  else
    return false; // Otherwise, return false.
}

// A function to get the temperature. Returns a float.
float getTemperature()
{
  // Calls the readObjectTempC() method from the mlx object and immediately "spits out" its result.
  return mlx.readObjectTempC();
}

// A function for smooth motor startup using PWM (Pulse Width Modulation).
void turnOn()
{
  // i goes from 0 to 255. 255 is the maximum PWM signal duty cycle (100% power).
  for(int i = 0; i < 256; i++)
  {
    // We send pulsating current to pin 3. The higher 'i' is, the more power the motor gets.
    analogWrite(fan, i);
    // Wait 50ms for each step. The whole startup will take 256 * 50ms = 12.8 seconds!
    delay(50);
  }
  Serial.println("Fan ON");
}

// A function for smooth motor braking.
void turnOff()
{
  // j decreases from 255 down to 1 (we decrease the signal duty cycle).
  for(int j = 255; j > 0; j--)
  {
    analogWrite(fan, j);
    // Wait 50ms. Braking will also take almost 13 seconds.
    delay(50);
  }
  Serial.println("Fan OFF");
}

// A function that blinks the indicator diode.
void blink()
{
  digitalWrite(diode, HIGH); // Turn the diode on
  delay(500);                // Wait half a second
  digitalWrite(diode, LOW);  // Turn the diode off
  delay(500);                // Wait half a second
}