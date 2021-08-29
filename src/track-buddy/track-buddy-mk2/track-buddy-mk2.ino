// Track Buddy Firmware Mk2
// Author: Christian Merrill
// License: MIT 2.0
// Version: 0.1
// Tested on Arduino Pro Micro with BNO080 and IDE 1.8.15

#include <Wire.h>
#include <Joystick.h>
#include <SparkFun_BNO080_Arduino_Library.h>

#define tareButton 8
#define pauseButton 10
// #define pauseButton 9

int currentTareState = 0;
int currentPauseState;
int lastTareState = 0;
int lastPauseState = LOW;
int lastPauseCHGState = LOW;
int xAxis_ = 0;
int yAxis_ = 0;
int rzAxis_ = 0;

float roll = 0;
float yaw = 0;
float pitch = 0;
float xOffset = 0;
float yOffset = 0;
float rzOffset = 0;

bool paused = false;

unsigned long lastPauseDebounceTime = 0;

const bool debug = false;  // enables serial printing if True (outputs degrees not ints)
const bool initAutoSendState = false;  // used to automatically send joystick state
const int smoothing = 1023; // used to map roll/pitch/yaw to an integer value 
const int debounce = 1;  // delay in ms for debouncing
const int sendRate = 1; // send rate in ms for the BNO080 IMU;
const int pauseDebounceTime = 50;



// Initalize the Joystick Data Structure
//The Joystick is defined in the following setup:
//Joystick(Joystick HID ID, Joystick Type, Button Count, Hat Switch Count, Include X, Include Y, Include Z, Include Rx, Include Ry, Include Rz, Include Rudder, Include Throttle, Include Accelerator, Include Brake, Include Steering
//Joystick HID ID: A Hex value identifier for HID Device Recognition (default: 0x03). DO NOT USE 0x01 or 0x02
//Joystick type: Define the type of joystick from the types supported. Types: DEFAULT Joystick (0x04 or JOYSTICK_TYPE_JOYSTICK), Gamepad (0x05 or JOYSTICK_TYPE_GAMEPAD), Multi-Axis Controller (0x08 or JOYSTICK_TYPE_MULTI_AXIS)
//Button Count: Number of Buttons shown to HID system (default: 32)
//Hat Switch Count: Number of Hat Switches, max 2. (default:2)
//Include X Axis: Determines whether the X axis is avalible for used by the HID system, defined as a bool value (default:true)
//Include Y Axis: Determines whether the Y axis is avalible for used by the HID system, defined as a bool value (default:true)
//Include Z Axis: Determines whether the Z axis is avalible for used by the HID system, defined as a bool value (default:true)
//Include Rx Axis: Determines whether the X Rotational axis is avalible for used by the HID system, defined as a bool value (default:true)
//Include Ry Axis: Determines whether the Y Rotational axis is avalible for used by the HID system, defined as a bool value (default:true)
//Include Rz Axis: Determines whether the Z Rotational axis is avalible for used by the HID system, defined as a bool value (default:true)
//Include Rudder: Determines whether a Rudder axis is avalible for used by the HID system, defined as a bool value (default:true)
//Include Throttle: Determines whether a Throttle axis is avalible for used by the HID system, defined as a bool value (default:true)
//Include Accelerator: Determines whether an Accelerator axis is avalible for used by the HID system, defined as a bool value (default:true)
//Include Brake: Determines whether a Brake axis is avalible for used by the HID system, defined as a bool value (default:true)
//Include Steering: Determines whether a Steering axis is avalible for used by the HID system, defined as a bool value (default:true)

Joystick_ Joystick(0x16, JOYSTICK_TYPE_JOYSTICK, 0, 0, true, true, false, false, false, true, false, false, false, false, false);  //Make sure that hex value is unique, update as needed.

BNO080 BNO080_;
// Constants for Program


void setup()
{
  if (debug == true) {
    Serial.begin(115200);
    Serial.println();
    Serial.println("Track-buddy Mk2 v0.1. Debug mode enabled.");
  }

  Wire.begin();
  pinMode(tareButton, INPUT_PULLUP);
  pinMode(pauseButton, INPUT_PULLUP);
  
  Joystick.begin();
  
  if (BNO080_.begin() == false) {
     Serial.println(F("BNO080 not detected. Check connection and try again."));
     while (1);
   }
  
  Wire.setClock(400000);

  BNO080_.enableRotationVector(sendRate); //Send data update every 30ms
  
  if (debug == true) {
    Serial.println(F("BNO080 detected and rotation vector has been enabled"));
    Serial.println(F("Output in form of degrees roll, pitch, yaw"));
  }
}

void loop()
{

  if (BNO080_.dataAvailable() == true)
  {
    
    if (paused != true){
      // Get raw values from sensor, example taken from Spark Fun library
      roll = (BNO080_.getRoll()) * 180.0 / PI; // Convert roll to degrees
      pitch = (BNO080_.getPitch()) * 180.0 / PI; // Convert pitch to degrees
      yaw = (BNO080_.getYaw()) * 180.0 / PI; // Convert yaw / heading to degrees

      // Apply offset to roll/pitch/yaw
      roll = roll - xOffset;
      pitch = pitch - yOffset;
      yaw = yaw - rzOffset;
    
      // Check for tare button push
      currentTareState = !digitalRead(tareButton);

      // debouncing not necessary for tare button
      if (currentTareState != lastTareState) {
        // set current axis values as offset from internal measures of current values
        xOffset = (BNO080_.getRoll()) * 180.0 / PI;
        yOffset = (BNO080_.getPitch()) * 180.0 / PI;
        rzOffset = (BNO080_.getYaw()) * 180.0 / PI;
    
    }
    }
    
   

    // Convert roll/pitch/yaw into integers for sending to joystick axis values
    xAxis_ = map(roll, -180, 180, 0, smoothing);
    yAxis_ = map(pitch, -180, 180, 0, smoothing);
    rzAxis_ = map(yaw, -180, 180, 0, smoothing);

    // Set axis values to joystick
    Joystick.setXAxis(xAxis_);
    Joystick.setYAxis(yAxis_);
    Joystick.setRzAxis(rzAxis_);
  
    
    
    currentPauseState = digitalRead(pauseButton);
    
    if (currentPauseState != lastPauseCHGState) {
      lastPauseDebounceTime = millis();
      lastPauseCHGState = currentPauseState;
    }

    if ((millis() - lastPauseDebounceTime) > pauseDebounceTime) {
      if (lastPauseState == HIGH && currentPauseState == LOW) {
        paused = !paused;
      }


      lastPauseState = currentPauseState;
    }

    
    
  if (debug == true){
      Serial.print(roll, 1);
      Serial.print(F(","));
      Serial.print(pitch, 1);
      Serial.print(F(","));
      Serial.print(yaw, 1);
      Serial.print(F(","));
      if (paused == true) {
        Serial.print("Paused");
      }
      else {
        Serial.print("Running");
      }
      Serial.println();
    }
    
    Joystick.sendState();
    delay(debounce); // Delay for debouncing
  } 
}
