import sensor, lcd
import time

lcd.init()
sensor.reset()

sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.run(1)
sensor.skip_frames(10)


clock = time.clock()

while(True):
    clock.tick()
    img = sensor.snapshot()
    fps = clock.fps()
    img.draw_string(0, 0, "%2.1ffps" %(fps), color=(0, 0, 255), scale=2.0)
    lcd.display(img)
    print(fps)

