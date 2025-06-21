import sensor, image, time, lcd
from machine import Timer
from modules import ybserial
from robot_Lib import Robot

ser = ybserial()
bot = Robot(ser)
bot.set_beep(50)
bot.set_car_motion(0, 0, 0)

lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 100)


def parse_payload(payload):
    global car_count
    if payload == "turn left":
        print("car turn left")
        car_count = 20
        bot.set_car_motion(0, 0, 3)
    elif payload == "turn right":
        print("car turn right")
        car_count = 20
        bot.set_car_motion(0, 0, -3)
    if payload == "spin left":
        print("car spin left")
        car_count = 40
        bot.set_car_motion(0, 0, 3)
    elif payload == "spin right":
        print("car spin right")
        car_count = 40
        bot.set_car_motion(0, 0, -3)
    elif payload == "go ahead":
        print("car go ahead")
        car_count = 20
        bot.set_car_motion(0.5, 0, 0)
    elif payload == "fall back":
        print("car fall back")
        car_count = 20
        bot.set_car_motion(-0.5, 0, 0)

car_count = 0
car_state = 0
def on_timer(timer):
    global car_count
    if car_count > 0:
        car_count = car_count - 1


timer = Timer(Timer.TIMER0, Timer.CHANNEL0,
            mode=Timer.MODE_PERIODIC, period=50,
            unit=Timer.UNIT_MS, callback=on_timer, arg=None)


try:
    clock = time.clock()
    while(True):
        clock.tick()
        img = sensor.snapshot()
        fps = clock.fps()
        if car_state == 0:
            for code in img.find_qrcodes():
                img.draw_rectangle(code.rect(), color = 127, thickness=3)
                img.draw_string(code.x(), code.y()-25, code.payload(), color=(200, 0, 50), scale=2.0)
                print(code)
                car_state = 1
                parse_payload(code.payload())
        img.draw_string(0, 0, "%2.1ffps" %(fps), color=(0, 60, 128), scale=2.0)
        lcd.display(img)
        #print(fps)

        if car_count == 0:
            if car_state > 0:
                bot.set_car_motion(0, 0, 0)
                car_state = 0
        elif car_count < 0:
            car_count = 0
except Exception as e:
    timer.deinit()
    del timer
    print("Exit or Abort:", e)


