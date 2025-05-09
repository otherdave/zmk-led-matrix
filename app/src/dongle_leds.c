#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(ZMK, CONFIG_ZMK_LOG_LEVEL);

#define STRIP_NODE DT_NODELABEL(neopixel)
#define NUM_LEDS 7 

static const struct device *strip;
static struct led_rgb pixels[NUM_LEDS];

static void boot_show_pattern() {
    printk("building array\n");
    k_sleep(K_MSEC(100));

    memset(pixels, 0, sizeof(pixels));

    /*
    for (int i = 0; i < NUM_LEDS; i++) {
        if (i % 3 == 0) {
            pixels[i].r = 100;
            pixels[i].g = 0;
            pixels[i].b = 0;
        } else if (i % 3 == 1) {
            pixels[i].r = 0;
            pixels[i].g = 100;
            pixels[i].b = 0;
        } else {
            pixels[i].r = 0;
            pixels[i].g = 0;
            pixels[i].b = 100;
        }
    }
        */

    pixels[0] = (struct led_rgb){ .r = 64, .g = 0, .b = 0 }; // red
    pixels[1] = (struct led_rgb){ .r = 0, .g = 64, .b = 0 }; // green
    pixels[2] = (struct led_rgb){ .r = 0, .g = 0, .b = 64 }; // blue

    pixels[3] = (struct led_rgb){ .r = 0, .g = 0, .b = 64 }; // blue
    pixels[4] = (struct led_rgb){ .r = 64, .g = 0, .b = 0 }; // red
    pixels[5] = (struct led_rgb){ .r = 0, .g = 64, .b = 0 }; // green

    int rc = led_strip_update_rgb(strip, pixels, NUM_LEDS);
    k_busy_wait(100);
    printk("led_strip_update_rgb() returned %d\n", rc);


    /*
    printk("Calling update\n");
    k_sleep(K_SECONDS(1));
    int rc = led_strip_update_rgb(strip, pixels, NUM_LEDS);
    printk("led_strip_update_rgb() returned %d\n", rc);
    */
}

void led_log_test_thread(void *p1, void *p2, void *p3) {
    k_sleep(K_SECONDS(5));
    printk("Initializing NeoPixels..3.\n");

    //strip = DEVICE_DT_GET(STRIP_NODE);
    strip = DEVICE_DT_GET(DT_NODELABEL(ws2812));
    printk("Got Strip\n");
    k_sleep(K_SECONDS(1));

   
    if (!device_is_ready(strip)) {
        printk("NeoPixel device not ready!\n");
        k_sleep(K_SECONDS(1));
        return -ENODEV;
    }


    printk("Showing pattern\n");
    k_sleep(K_SECONDS(1));
    boot_show_pattern();  

    printk("I'm out!\n");
    k_sleep(K_SECONDS(1));
    return 0;
}

K_THREAD_DEFINE(led_log_id, 1024, led_log_test_thread, NULL, NULL, NULL, 10, 0, 0);