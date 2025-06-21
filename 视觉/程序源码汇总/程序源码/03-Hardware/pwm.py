from machine import Timer, PWM
import time

# RED=27, GREEN=26, BLUE=29
LED_PIN = 26

tim = Timer(Timer.TIMER0, Timer.CHANNEL0, mode=Timer.MODE_PWM)
pwm = PWM(tim, freq=500000, duty=50, pin=LED_PIN)

duty=0
add = True
while True:
    if add:
        duty += 5
    else:
        duty -= 5
    if duty > 100:
        duty = 100
        add = False
    elif duty < 0:
        duty = 0
        add = True
    time.sleep(0.05)
    pwm.duty(duty)
