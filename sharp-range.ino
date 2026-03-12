/**
 * @file sharp-range.ino
 * @author Joshua Marshall (joshua.marshall@queensu.ca), Thomas Sears (thomas.sears@queensu.ca)
 * @brief Arduino program to read proximity data from a Sharp GP2Y0A21YK.
 * @version 2.1
 * @date 2022-12-21
 *
 * @copyright Copyright (c) 2022
 *
 */

// Arduino analog input pin to which the Sharp sensor is connected
const byte SHARP_PIN = A0;
const byte SHARP_PIN1 = A1;
const byte SHARP_PIN2 = A2;

// Variables to store the proximity measurement
int sharp_val_left = 0; // integer read from analog pin
int sharp_val_front = 0;
int sharp_val_right = 0;
double d_left = 0;
double d_front = 0;
double d_right = 0;
float sharp_range; // range measurement [cm]

void setup()
{
    // Open the serial port at 115200 bps
    // Serial.begin(115200);
    Serial.begin(9600);
}

void loop()
{
    // Read the sensor output (0-1023, which is 10 bits and fits inside an Arduino int-type)
    sharp_val_left = analogRead(SHARP_PIN);
    sharp_val_front = analogRead(SHARP_PIN1);
    sharp_val_right = analogRead(SHARP_PIN2);

    d_left = 141 + -0.72 * sharp_val_left + (0.000991)*pow(sharp_val_left,2);
    d_front = 141 + -0.72 * sharp_val_front + (0.000991)*pow(sharp_val_front,2);
    d_right = 141 + -0.72 * sharp_val_right + (0.000991)*pow(sharp_val_right,2);


    // Print all values
    Serial.print(d_left);
    Serial.print('\t');
    Serial.print(d_front);
    Serial.print('\t');
    Serial.print(d_right);
    Serial.print("\n");



    // Delay for a bit before reading the sensor again
    delay(500);
}
