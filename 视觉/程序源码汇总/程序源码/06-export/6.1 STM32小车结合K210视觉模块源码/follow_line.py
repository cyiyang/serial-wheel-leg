import sensor, image, time, lcd

from modules import ybserial
from robot_Lib import Robot
from simplePID import PID

#FollowLinePID = (22, 0, 2)
FollowLinePID = (15, 0, 2)
SCALE = 1000.0

PID_controller = PID(
    160,
    FollowLinePID[0] / 1.0 / (SCALE),
    FollowLinePID[1] / 1.0 / (SCALE),
    FollowLinePID[2] / 1.0 / (SCALE))

ser = ybserial()
bot = Robot(ser)
bot.set_beep(50)
bot.set_car_motion(0, 0, 0)

lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 100)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(True)
clock = time.clock()

print("Hold the object you want to track in front of the camera in the box.")
print("MAKE SURE THE COLOR OF THE OBJECT YOU WANT TO TRACK IS FULLY ENCLOSED BY THE BOX!")

# Capture the color thresholds for whatever was in the center of the image.
# 50x50 center of QVGA.
BOX = 30
r = [(320//2)-(BOX//2), (240//2)-(BOX//2), BOX, BOX]
for i in range(50):
    img = sensor.snapshot()
    img.draw_rectangle(r)
    lcd.display(img)

print("Learning thresholds...")
threshold = [BOX, BOX, 0, 0, 0, 0] # Middle L, A, B values.
for i in range(50):
    img = sensor.snapshot()
    hist = img.get_histogram(roi=r)
    lo = hist.get_percentile(0.01) # Get the CDF of the histogram at the 1% range (ADJUST AS NECESSARY)!
    hi = hist.get_percentile(0.99) # Get the CDF of the histogram at the 99% range (ADJUST AS NECESSARY)!
    # Average in percentile values.
    threshold[0] = (threshold[0] + lo.l_value()) // 2
    threshold[1] = (threshold[1] + hi.l_value()) // 2
    threshold[2] = (threshold[2] + lo.a_value()) // 2
    threshold[3] = (threshold[3] + hi.a_value()) // 2
    threshold[4] = (threshold[4] + lo.b_value()) // 2
    threshold[5] = (threshold[5] + hi.b_value()) // 2
    for blob in img.find_blobs([threshold], pixels_threshold=100, area_threshold=100, merge=True, margin=10):
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())
        img.draw_rectangle(r, color=(0,255,0))
    lcd.display(img)

print("Thresholds learned...")
print("Start Color Recognition...")

state = 0
while(True):
    clock.tick()
    img = sensor.snapshot()
    fps = clock.fps()
    data_in = 0
    index = 0
    for blob in img.find_blobs([threshold], pixels_threshold=100, area_threshold=100, merge=True, margin=10):
        #img.draw_rectangle(blob.rect())
        #img.draw_cross(blob.cx(), blob.cy())
        index = index + 1
        state = 1
        if index == 1:
            area_max = blob.w()*blob.h()
            area = blob
        else:
            temp_area = blob.w()*blob.h()
            if temp_area > area_max:
                area_max = temp_area
                area = blob
    if state == 1:
        print("area:", index, area.w(), area.h())
        value = PID_controller.incremental(area.cx())
        img.draw_rectangle(area.rect())
        img.draw_cross(area.cx(), area.cy())
        bot.set_car_motion(0.2, 0, value)
        state = 0

    img.draw_string(0, 0, "%2.1ffps" %(fps), color=(0, 60, 128), scale=2.0)
    lcd.display(img)
    #print("FPS:s", fps)

