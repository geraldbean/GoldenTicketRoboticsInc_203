from RPi import GPIO
from time import sleep
import time 
GPIO.setwarnings(False)

clk = 7
dt = 13

GPIO.setmode(GPIO.BOARD)
GPIO.setup(clk, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
GPIO.setup(dt, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
GPIO.setup(15, GPIO.OUT)
GPIO.setup(11, GPIO.OUT)

counter = 0
clkLastState = GPIO.input(clk)

p=GPIO.PWM(15,100)
q=GPIO.PWM(11,100)

p.start(0)
q.start(0)

    
p.ChangeDutyCycle(100)

start=time.time()
goal = 2

try:

        while True:
                clkState = GPIO.input(clk)
                dtState = GPIO.input(dt)
                if clkState != clkLastState:
                        if dtState != clkState:
                                counter += 1
                        else:
                                counter -= 1
                        print(counter)
                clkLastState = clkState
                sleep(0.01)

                #Controlling motors               
finally:
        GPIO.cleanup()