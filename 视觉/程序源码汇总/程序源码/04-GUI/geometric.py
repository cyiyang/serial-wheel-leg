import lcd, image

lcd.init()
img = image.Image(size=(lcd.width(), lcd.height()))
img.draw_rectangle((0, 0, lcd.width(), lcd.height()), fill=True, color=(0, 81, 137))

img.draw_string(50, 10, "Hello Yahboom", color=(255, 255, 255), scale=2)
img.draw_line(50, 50, 200, 50, color=(100, 0, 0), thickness=5)
img.draw_rectangle(50, 60, 150, 150, color=(0, 100, 0), thickness=2, fill=True)
img.draw_circle(125, 135, 50, color=(255, 255, 255), thickness=2, fill=False)

lcd.display(img)
