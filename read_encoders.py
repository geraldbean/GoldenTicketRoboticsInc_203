from RPi import GPIO
from time import sleep
import time 
GPIO.setwarnings(False)

chanA = 1
chanB = 2

chanC = 3
chanD = 4

GPIO.setmode(GPIO.BOARD)
GPIO.setup(chanA, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
GPIO.setup(chanB, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
GPIO.setup(15, GPIO.OUT)
GPIO.setup(11, GPIO.OUT)

GPIO.setup(chanC, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
GPIO.setup(chanD, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
GPIO.setup(16, GPIO.OUT)
GPIO.setup(10, GPIO.OUT)

counter = 0
counter2 = 0

ALastState = GPIO.input(chanA)
CLastState = GPIO.input(chanC)

p=GPIO.PWM(15,100)
q=GPIO.PWM(11,100)

n=GPIO.PWM(16,100)
u=GPIO.PWM(10,100)

p.start(0)
q.start(0)

n.start(0)
u.start(0)
    
p.ChangeDutyCycle(100)
n.ChangeDutyCycle(100)

start=time.time()
goal = 2

try:

        while True:
                CState = GPIO.input(chanC)
                DState = GPIO.input(chanD)
                
                if CState != CLastState:
                    if DState != CState:
                        counter2 += 1
                    else:
                        counter2 -= 1
                        
                CLastState = CState
            
                AState = GPIO.input(chanA)
                BState = GPIO.input(chanB)
                if AState != ALastState:
                        if BState != AState:
                                counter += 1
                        else:
                                counter -= 1
                        print(counter, counter2)
                ALastState = AState
                
                sleep(0.01)

                #Controlling motors               
finally:
        GPIO.cleanup()
