#include <zephyr/device.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>

#define NUM_LEDS 60

static const struct device *strip;
static struct led_rgb pixels[NUM_LEDS];

static void boot_show_pattern() {

  memset(pixels, 0, sizeof(pixels));

  int rc = 0;
  int row = 0;
  int rowIdx = 0; 

  while (1) {
    if (row >= 10)
        row = 0;

    // Everything is kinda tealish?
    for (int i = 0; i < NUM_LEDS; i++) {
      pixels[i].r = 0;
      pixels[i].g = 3;
      pixels[i].b = 3;
    }

    // The row we're on is red
    rowIdx = row * 6;
    for(int i = rowIdx; i < rowIdx + 6; ++i)
    { 
      pixels[i].r = 4;
      pixels[i].g = 0;
      pixels[i].b = 0;
    }
     
    rc = led_strip_update_rgb(strip, pixels, NUM_LEDS);
    printk("led_strip_update_rgb() returned %d\n", rc);
    k_sleep(K_SECONDS(1));

    printk("Doing it again....\n");
    row++;
  }
}

void led_log_test_thread(void *p1, void *p2, void *p3) {
  k_sleep(K_SECONDS(2));
  printk("Initializing NeoPixels..3.\n");

  strip = DEVICE_DT_GET(DT_NODELABEL(led_strip));

  if (!device_is_ready(strip)) {
    printk("LED Strip device not ready!\n");
    k_sleep(K_SECONDS(1));
    return -ENODEV;
  }

  boot_show_pattern();

  return;
}

K_THREAD_DEFINE(led_log_id, 1024, led_log_test_thread, NULL, NULL, NULL, 10, 0,
                0);