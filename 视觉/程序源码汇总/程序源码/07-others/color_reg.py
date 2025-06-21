import sensor, image, time, lcd
from modules import ybserial, ybrgb

RGB = ybrgb()


lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 100)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
clock = time.clock()


def learning_color():
    print("Hold the object you want to track in front of the camera in the box.")
    print("MAKE SURE THE COLOR OF THE OBJECT YOU WANT TO TRACK IS FULLY ENCLOSED BY THE BOX!")

    BOX = 30
    r = [(320//2)-(BOX//2), (240//2)-(BOX//2), BOX, BOX]
    for i in range(50):
        img = sensor.snapshot()
        img.draw_rectangle(r)
        lcd.display(img)

    print("Learning thresholds...")
    threshold = [50, 50, 0, 0, 0, 0] # Middle L, A, B values.
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

    print("threshold:", threshold)
    while True:
        clock.tick()
        img = sensor.snapshot()
        fps = clock.fps()

        for blob in img.find_blobs([threshold], pixels_threshold=100, area_threshold=100, merge=True, margin=10):
            img.draw_rectangle(blob.rect())
            img.draw_cross(blob.cx(), blob.cy())

        img.draw_string(0, 0, "Learned threshold", color=(0, 60, 128), scale=2.0)
        img.draw_string(0, 50, "%s" % threshold, color=(0, 60, 128), scale=2.0)
        lcd.display(img)


#learning_color()


#红色 color red
threshold_red = [15, 50, 36, 73, 16, 49]
#绿色 color green
threshold_green = [25, 65, -47, -13, 16, 39]
#蓝色 color blue
threshold_blue = [24, 61, 17, 63, -78, -60]
#黄色 color yellow
threshold_yellow = [54, 79, -8, 24, 57, 69]

color_list = [threshold_red, threshold_green, threshold_blue, threshold_yellow]

last_color = 0
def print_color(c):
    global last_color
    color = ""
    #if c == last_color:
        #return color
    if c == 1:
        #bot.set_colorful_lamps(0xff, 255, 0, 0)
        RGB.set(255, 0, 0)
        print("This is color red")
        color = "red"
    elif c == 2:
        #bot.set_colorful_lamps(0xff, 0, 255, 0)
        RGB.set(0, 255, 0)
        print("This is color green")
        color = "green"
    elif c == 3:
        #bot.set_colorful_lamps(0xff, 0, 0, 255)
        RGB.set(0, 0, 255)
        print("This is color blue")
        color = "blue"
    elif c == 4:
        #bot.set_colorful_lamps(0xff, 255, 255, 0)
        RGB.set(255, 255, 0)
        print("This is color yellow")
        color = "yellow"
    last_color = c
    return color



print("Start Color Recognition...")
while(True):
    clock.tick()
    img = sensor.snapshot()
    fps = clock.fps()
    index = 0
    led = 0
    for t in color_list:
        index = index + 1
        for blob in img.find_blobs([t], pixels_threshold=100, area_threshold=100, merge=True, margin=10):
            img.draw_rectangle(blob.rect())
            img.draw_cross(blob.cx(), blob.cy())
            color = print_color(index)
            if len(color)>1:
                led = led + 1
                img.draw_string(int(blob.cx()-blob.w()/2), int(blob.cy()-blob.h()/2-20), color, scale=2.0)
    if led == 0:
        RGB.set(0, 0, 0)
    img.draw_string(0, 0, "%2.1ffps" %(fps), color=(0, 60, 128), scale=2.0)
    lcd.display(img)
    #print(fps)
