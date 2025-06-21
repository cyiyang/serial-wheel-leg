# Hello World Example
#
# Welcome to the CanMV IDE! Click on the green run arrow button below to run the script!

import sensor, image, time, lcd

lcd.init()                          # Init lcd display
#lcd.clear(lcd.BLUE)                  # Clear lcd screen.
info = 'Hello Yahboom'
loading = image.Image(size=(lcd.width(), lcd.height()))
loading.draw_rectangle((0, 0, lcd.width(), lcd.height()), fill=True, color=(0, 81, 137))
loading.draw_string(int(lcd.width()//2 - len(info) * 5), (lcd.height())//4, info, color=(255, 255, 255), scale=2, mono_space=0)
lcd.display(loading)
sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)   # Set frame size to QVGA (320x240)
sensor.skip_frames(time = 500)      # Wait for settings take effect.
clock = time.clock()                # Create a clock object to track the FPS.

while(True):
    clock.tick()                    # Update the FPS clock.
    try:
        img = sensor.snapshot()         # Take a picture and return the image.
        fps = clock.fps()
        img.draw_string(0, 0, "%2.1ffps" %(fps), color=(0, 60, 150), scale=2.0)
        lcd.display(img)                # Display image on lcd.
        print(fps)              # Note: CanMV Cam runs about half as fast when connected
                                    # to the IDE. The FPS should increase once disconnected.
    except:
        pass
