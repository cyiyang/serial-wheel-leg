from machine import Timer
import time

def on_timer(timer):
    print("This is on_timer callback")


timer = Timer(Timer.TIMER0, Timer.CHANNEL0,
            mode=Timer.MODE_PERIODIC, period=100,
            unit=Timer.UNIT_MS, callback=on_timer, arg=None)

last_time = time.ticks_ms()
try:
    while True:
        if time.ticks_ms() - last_time >= 200:
            last_time = time.ticks_ms()
            print("This is main loop")
except:
    timer.deinit()
    del timer
