/**
 * @file motor-angular-rate.ino
 * @author Joshua Marshall (joshua.marshall@queensu.ca)
 * @brief Arduino program to estimate motor speed from encoder.
 * @version 2.1
 * @date 2022-12-09
 *
 * @copyright Copyright (c) 2021-2022
 *
 */

// Wheel PWM pin (must be a PWM pin)
int EA = 6;
int EB = 10;

// Wheel direction digital pins for right wheel
int I1 = 7;
int I2 = 5;

// Wheel direction digital pins for left wheel
int I3 = 12;
int I4 = 11;

// Motor PWM command variable [0-255]
byte u_r = 0;
byte u_l = 0;

// left wheel encoder digital pins
const byte SIGNAL_A = 3;
const byte SIGNAL_B = 4;

// right wheel encoder digital pins
const byte SIGNAL_AR = 9;
const byte SIGNAL_BR = 8;

// Encoder ticks per (motor) revolution (TPR)
const int TPR = 3000;

// Wheel radius [m]
const double RHO = 0.0625;

// Counter to keep track of encoder ticks [integer]
volatile long encoder_ticks = 0;
volatile long encoder_ticksr = 0;

// Variable to store estimated angular rate of left wheel [rad/s]
double omega_L = 0.0;
double omega_R = 0.0;

//variables to store the velocity
double v_right = 0.0;
double v_left = 0.0;

//vhat and what
double v_hat = 0.0;
double w_hat = 0.0;

// Sampling interval for measurements in milliseconds
const int T = 1000;

// Counters for milliseconds during interval
long t_now = 0;
long t_last = 0;

//set the P controller constants
int kP = 150;
//for the PI controller
int kI = 200;
int v_d = 220;
int w_d = 0;
double v_rd = 0.0;
double v_ld = 0.0;
double l = 0.2775;

double diff_l= 0.0;
double diff_r = 0.0;
double diff_prev1 = 0.0;
double diff_prev2 = 0.0;

// This function is called when SIGNAL_A goes HIGH when clockwise
void decodeEncoderTicks()
{
    // For left wheel
    if (digitalRead(SIGNAL_B) == LOW)
    {
        // SIGNAL_A leads SIGNAL_B, so count one way
        encoder_ticks--;
    }
    else
    {
        // SIGNAL_B leads SIGNAL_A, so count the other way
        encoder_ticks++;
    }

    // For right wheel
    if (digitalRead(SIGNAL_BR) == LOW)
    {
        encoder_ticksr--;
    }
    else
    {
        encoder_ticksr++;
    }

}

void setup()
{
    // Open the serial port at 9600 bps
    Serial.begin(9600);

    // Set the pin modes for the motor driver
    pinMode(EA, OUTPUT);
    pinMode(I1, OUTPUT);
    pinMode(I2, OUTPUT);

    pinMode(EB, OUTPUT);
    pinMode(I3, OUTPUT);
    pinMode(I4, OUTPUT);

    // Set the pin modes for the encoders right side
    pinMode(SIGNAL_A, INPUT);
    pinMode(SIGNAL_B, INPUT);

    // left side pin modes
    pinMode(SIGNAL_AR, INPUT);
    pinMode(SIGNAL_BR, INPUT);

    // Every time the pin goes high, this is a pulse
    attachInterrupt( digitalPinToInterrupt(SIGNAL_A), decodeEncoderTicks, RISING);

    // for other wheel
    attachInterrupt( digitalPinToInterrupt(SIGNAL_AR), decodeEncoderTicks, RISING);

    // Print a message
    Serial.print("Program initialized.");
    Serial.print("\n");
}

void loop()
{
    // Get the elapsed time [ms]
    t_now = millis();

    if (t_now - t_last >= T)
    {
        // Estimate the rotational speed [rad/s]
        omega_R = 2.0 * PI * ((double)encoder_ticks / (double)TPR) * 1000.0 / (double)(t_now - t_last);

        //for the left wheel
        omega_L = 2.0 * PI * ((double)encoder_ticksr / (double)TPR) * 1000.0 / (double)(t_now - t_last);

        //v estimate for right [m/s]
        v_right = (0.0625 * omega_R);

        //v estimate for left [m/s]
        v_left = (0.0625 * omega_L);

        //Traslational speed
        v_hat = 0.5 * (v_right + v_left);

        //Turning rate
        w_hat = (1/0.2775) * (v_right - v_left);
        
        // Desired right velocity
        v_rd = v_d + w_d;

        // Desired left velocity
        v_ld = v_d - w_d;

        //for the integral portion
        diff_l = (v_ld - v_left);
        diff_r = (v_rd - v_right);

        if (-255 < u_r < 255 and -255 < u_l < 255) {
            diff_prev1 += diff_l;
            diff_prev2 += diff_r;
        }

        //PI controller equations 
        u_r = kP * (v_rd - v_right) + kI * diff_prev1;
        u_l = kP * (v_ld - v_left) + kI * diff_prev2;

        // Keeping u within the assignable limits
        if (u_r > 255) {
            u_r = 255;
        }
        else if (u_r < -255) {
            u_r = -255;
        }

        if (u_l > 255) {
            u_l = 255;
        }
        else if (u_l < -255) {
            u_l = -255;
        }

                // // Print some stuff to the serial monitor
                // Serial.print("W_RIGHT: ");
                // Serial.print(omega_L);
                // Serial.print(" rad/s");
                // Serial.print("\t");
                // Serial.print("V_RIGHT: ");
                // Serial.print(v_right);
                // Serial.print("m/s");

                // Serial.print("\t");

                // //print for the left motor?
                // Serial.print("W_LEFT: ");
                // Serial.print(omega_LR);
                // Serial.print(" rad/s");
                // Serial.print("\t");
                // Serial.print("V_LEFT: ");
                // Serial.print(v_left);
                // Serial.print("m/s");
        
        Serial.print("\n");

        // Record the current time [ms]
        t_last = t_now;

        // Reset the encoder ticks counter
        encoder_ticks = 0;

        //reset the other wheel
        encoder_ticksr = 0;
    }

    //Serial.println("reached!");
    // Set the wheel motor PWM command [0-255]

    // Select a direction right
    digitalWrite(I1, HIGH);
    digitalWrite(I2, LOW);

    // select a direction left
    digitalWrite(I3, LOW);
    digitalWrite(I4, HIGH);

    // PWM command to the motor driver
    analogWrite(EA, u_l);
    analogWrite(EB, u_r);
}
