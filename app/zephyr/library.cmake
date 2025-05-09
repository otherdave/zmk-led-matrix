zephyr_library_sources_ifdef(CONFIG_WS2812_STRIP_SPI
    ${ZEPHYR_BASE}/drivers/led_strip/ws2812_spi.c
)
