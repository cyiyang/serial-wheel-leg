import lvgl as lv
import lvgl_helper as lv_h
import touchscreen as ts
import lcd
import time


lcd.init()
ts.init()
lv.init()

disp_buf1 = lv.disp_buf_t()
buf1_1 = bytearray(320*10)
lv.disp_buf_init(disp_buf1, buf1_1, None, len(buf1_1)//4)
disp_drv = lv.disp_drv_t()
lv.disp_drv_init(disp_drv)
disp_drv.buffer = disp_buf1
disp_drv.flush_cb = lv_h.flush
disp_drv.hor_res = 320
disp_drv.ver_res = 240
lv.disp_drv_register(disp_drv)


indev_drv = lv.indev_drv_t()
lv.indev_drv_init(indev_drv)
indev_drv.type = lv.INDEV_TYPE.POINTER
indev_drv.read_cb = lv_h.read
lv.indev_drv_register(indev_drv)

class Page_Buttons:
    def __init__(self, app, page):
        self.app = app
        self.page = page

        # counter button
        self.counter_btn = lv.btn(page)
        self.counter_btn.set_size(100,60)
        self.counter_btn.align(page, lv.ALIGN.CENTER, 0, 0)
        self.counter_label = lv.label(self.counter_btn)
        self.counter_label.set_text('Count')
        self.counter_btn.set_event_cb(self.on_counter_btn)
        self.counter = 0

    def on_counter_btn(self, obj, event):
        if event == lv.EVENT.CLICKED:
            self.counter += 1
            self.counter_label.set_text(str(self.counter))

class Page_Slider:
    def __init__(self, app, page):
        self.app = app
        self.page = page

        # slider
        self.slider = lv.slider(page)
        self.slider.align(page, lv.ALIGN.CENTER, 0, -10)
        self.slider_label = lv.label(page)
        self.slider_label.align(self.slider, lv.ALIGN.OUT_LEFT_MID, -10, 0)
        self.slider.set_event_cb(self.on_slider_changed)
        self.on_slider_changed(None)

    def on_slider_changed(self, obj=None, event=-1):
        self.slider_label.set_text(str(self.slider.get_value()))


class Anim(lv.anim_t):
    def __init__(self, obj, val, size, exec_cb, path_cb, time=500, playback = False, ready_cb=None):
        super().__init__()
        lv.anim_init(self)
        lv.anim_set_time(self, time, 0)
        lv.anim_set_values(self, val, val+size)
        if callable(exec_cb):
            lv.anim_set_custom_exec_cb(self, exec_cb)
        else:
            lv.anim_set_exec_cb(self, obj, exec_cb)
        lv.anim_set_path_cb(self, path_cb )
        if playback: lv.anim_set_playback(self, 0)
        if ready_cb: lv.anim_set_ready_cb(self, ready_cb)
        lv.anim_create(self)

class AnimatedChart(lv.chart):
    def __init__(self, parent, val, size):
        super().__init__(parent)
        self.val = val
        self.size = size
        self.max = 2000
        self.min = 500
        self.factor = 100
        self.anim_phase1()

    def anim_phase1(self):
        Anim(
            self,
            self.val,
            self.size,
            lambda a, val: self.set_range(0, val),
            lv.anim_path_ease_in,
            ready_cb=lambda a:self.anim_phase2(),
            time=(self.max * self.factor) // 100)

    def anim_phase2(self):
        Anim(
            self,
            self.val+self.size,
            -self.size,
            lambda a, val: self.set_range(0, val),
            lv.anim_path_ease_out,
            ready_cb=lambda a:self.anim_phase1(),
            time=(self.min * self.factor) // 100)


class Page_Chart():
    def __init__(self, app, page):
        self.app = app
        self.page = page
        self.chart = AnimatedChart(page, 100, 1000)
        self.chart.set_width(page.get_width() - 100)
        self.chart.set_height(page.get_height() - 30)
        self.chart.align(page, lv.ALIGN.CENTER, 0, 0)
        self.series1 = self.chart.add_series(lv.color_hex(0xFF0000))
        self.chart.set_type(self.chart.TYPE.POINT | self.chart.TYPE.LINE)
        self.chart.set_series_width(3)
        self.chart.set_range(0,100)
        self.chart.init_points(self.series1, 10)
        self.chart.set_points(self.series1, [10,20,30,20,10,40,50,80,95,80])
        self.chart.set_x_tick_texts('a\nb\nc\nd\ne', 2, lv.chart.AXIS.DRAW_LAST_TICK)
        self.chart.set_x_tick_length(10, 5)
        self.chart.set_y_tick_texts('1\n2\n3\n4\n5', 2, lv.chart.AXIS.DRAW_LAST_TICK)
        self.chart.set_y_tick_length(10, 5)
        self.chart.set_div_line_count(3, 3)
        self.chart.set_margin(30)

        self.slider = lv.slider(page)
        self.slider.align(self.chart, lv.ALIGN.OUT_RIGHT_TOP, 10, 0)
        self.slider.set_width(30)
        self.slider.set_height(self.chart.get_height())
        self.slider.set_range(10, 200)
        self.slider.set_value(self.chart.factor, 0)
        self.slider.set_event_cb(self.on_slider_changed)

    # Create a slider that controls the chart animation speed
    def on_slider_changed(self, obj=None, event=-1):
        self.chart.factor = self.slider.get_value()


class Screen_Main(lv.obj):
    def __init__(self, app, *args, **kwds):
        self.app = app
        super().__init__(*args, **kwds)

        self.tabview = lv.tabview(self)
        self.tabview.set_style(lv.tabview.STYLE.BG, lv.style_plain_color)
        self.page_buttons = Page_Buttons(self.app, self.tabview.add_tab('Button'))
        self.page_slider = Page_Slider(self.app, self.tabview.add_tab('Slider'))
        self.page_chart = Page_Chart(self.app, self.tabview.add_tab('Chart'))


screen_main = Screen_Main(lv.obj())
lv.scr_load(screen_main)

tim = time.ticks_ms()
while True:
    if time.ticks_ms()-tim > 5:
        tim = time.ticks_ms()
        lv.task_handler()
        lv.tick_inc(5)

