/*
 * circuits4you.com
 * 2016 November 25
 * Load Cell HX711 Module Interface with Arduino to measure weight in Kgs
 Arduino 
 pin 
 2 -> HX711 CLK
 3 -> DOUT
 5V -> VCC
 GND -> GND
 
 Most any pin on the Arduino Uno will be compatible with DOUT/CLK.
 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
*/
 
#include "HX711.h"  //You must have this library in your arduino library folder
 
#define DOUT  3
#define CLK  2
 
HX711 scale(DOUT, CLK);

//Change this calibration factor as per your load cell once it is found you many need to vary it in thousands
float calibration_factor = 20545; // Set around 20550 for engine testing 

// Setup integer to take a limited amount of data points
  int i = 0;

// Setup t0 to measure time
double t0;
 
//=============================================================================================
//                         SETUP
//=============================================================================================
void setup() {
  Serial.begin(9600);
  Serial.println("HX711 Calibration");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press a,s,d,f to increase calibration factor by 10,100,1000,10000 respectively");
  Serial.println("Press z,x,c,v to decrease calibration factor by 10,100,1000,10000 respectively");
  Serial.println("Press t for tare");
  Serial.println("----------------------------------------------------------------------------------");
  Serial.println("   THIS CODE WAS (written by some arduino genius and) EDITED BY SAMUELITO PERRO");
  Serial.println("    ----- Scale to measure thrust for Morningside College Rocket Club. -----");
  Serial.println("  Safety first, Ad Astra,                          AND DON'T YOU FUCKING PANIC!");
  Serial.println("----------------------------------------------------------------------------------");
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
 
  long zero_factor = scale.read_average(); //Get a baseline reading
//  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
//  Serial.println(zero_factor);

}
 
//=============================================================================================
//                         LOOP
//=============================================================================================
void loop() {
  
  // This defines t0 right at the beginning of the first measurement of the loop
    // this cancels out the small delay from the setup function.
  if (i == 0)
  {
    t0 = millis();
  }
  
  // This makes the loop run until i = x (i > 300 is about 29s). The Engine has to burn in that time.
  if (i > 280) //This can be adjusted to take longer or shorter measurements
  {
    // When in, calculate final time, output, and exit.
    double finalT = millis() - t0;
    Serial.println("Total time of data collection: ");
    Serial.print(finalT/1000, 3); //Result is in miliseconds, /1000 to find seconds
    Serial.println();
    Serial.println("EXIT");
    delay(100);
    exit(0);
  }
  
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
 
  Serial.print(scale.get_units()*9.8, 3);
  Serial.println();
//  Serial.print(i);
//  Serial.println();
 
  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 10;
    else if(temp == 's')
      calibration_factor += 100;  
    else if(temp == 'x')
      calibration_factor -= 100;  
    else if(temp == 'd')
      calibration_factor += 1000;  
    else if(temp == 'c')
      calibration_factor -= 1000;
    else if(temp == 'f')
      calibration_factor += 10000;  
    else if(temp == 'v')
      calibration_factor -= 10000;  
    else if(temp == 't')
      scale.tare();  //Reset the scale to zero
  }

  i++;
}
