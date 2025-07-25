import sensor, time
from machine import UART
import pyb

scanned_qrs = []
count = 0
flag = 0
red_led = pyb.LED(1)
green_led = pyb.LED(2)
blue_led = pyb.LED(3)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA) # can be QVGA on M7...
sensor.skip_frames(30)
sensor.set_auto_gain(False) # must turn this off to prevent image washout...
clock = time.clock()
uart = UART(1,115200)

led_start_time = time.ticks_ms()
blue_led.on()

while(True):
    clock.tick()
    current_time = time.ticks_ms()
    if time.ticks_diff(current_time, led_start_time) > 5000:
        blue_led.off()
    img = sensor.snapshot()
    img.lens_corr(1.8) # strength of 1.8 is good for the 2.8mm lens.
    for code in img.find_qrcodes():
        if code.payload() not in scanned_qrs:
            red_led.on()
            led_start_time = time.ticks_ms()
            count += 1
            scanned_qrs.append(code.payload())
            print("发现新二维码:",code.payload())
            uart.write(code.payload())
            if count == 1 and flag == 1:
                green_led.on()
                last_clear_time = time.ticks_ms()
                scanned_qrs.clear()
                time.sleep(10)
                green_led.off()
            if count == 1 and flag == 1:
                uart.write(code.payload())
            if count >= 24:
                green_led.on()
                count = 0
                flag = 1
                last_clear_time = time.ticks_ms()
                scanned_qrs.clear()
                print("Over!")
                time.sleep(10)
                green_led.off()
    if time.ticks_diff(current_time, led_start_time) > 1000:
        red_led.off()

