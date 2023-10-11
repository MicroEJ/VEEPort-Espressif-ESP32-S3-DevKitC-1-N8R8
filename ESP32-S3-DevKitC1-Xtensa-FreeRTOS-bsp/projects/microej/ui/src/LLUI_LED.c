/*
 * C
 *
 * Copyright 2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdio.h>
#include "led_strip.h"
#include "LLUI_LED_impl.h"

/** @brief Define the maximum number of LEDs supported by the hardware platform. */
#define ESP32S_S3_DEV_KIT_MAX_NUMBER_OF_LEDS (3)

/** @brief Define the GPIO that is used to control the addressable LED. */
#define ESP32S_S3_DEV_KIT_LED_CTRL_GPIO (48)

/** @brief 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution). */
#define LED_STRIP_RMT_RES_HZ  (10 * 1000 * 1000)

/** @brief LED Strip object handle. */
static led_strip_handle_t led_strip;

/** @brief Intensity for LEDs [0..255]. */
static int32_t led_intensity[ESP32S_S3_DEV_KIT_MAX_NUMBER_OF_LEDS];

int32_t LLUI_LED_IMPL_initialize(void)
{
    // LED strip general initialization, according to your led board design
    led_strip_config_t strip_config = {
        .strip_gpio_num = ESP32S_S3_DEV_KIT_LED_CTRL_GPIO, // The GPIO that connected to the LED strip's data line
        .max_leds = ESP32S_S3_DEV_KIT_MAX_NUMBER_OF_LEDS,  // The number of LEDs in the strip
        .led_pixel_format = LED_PIXEL_FORMAT_GRB,          // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812,                     // LED strip model
        .flags.invert_out = false,                         // whether to invert the output signal
    };

    // LED strip backend configuration: RMT
    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,        // different clock source can lead to different power consumption
        .resolution_hz = LED_STRIP_RMT_RES_HZ, // RMT counter clock frequency
        .flags.with_dma = true,                // DMA feature is available on ESP target like ESP32-S3
    };

    // Create LED strip object with RMT backend
    if (ESP_OK == led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip)) {
        return ESP32S_S3_DEV_KIT_MAX_NUMBER_OF_LEDS;
    } else {
        return 0;
    }
}

int32_t LLUI_LED_IMPL_getIntensity(int32_t ledID)
{
    if ((ledID < 0) || (ledID >= ESP32S_S3_DEV_KIT_MAX_NUMBER_OF_LEDS)) {
        return LLUI_LED_MIN_INTENSITY;
    }

    return led_intensity[ledID];
}

void LLUI_LED_IMPL_setIntensity(int32_t ledID, int32_t intensity)
{
    if ((ledID < 0) || (ledID >= ESP32S_S3_DEV_KIT_MAX_NUMBER_OF_LEDS)) {
        return;
    }

    if (intensity < LLUI_LED_MIN_INTENSITY) {
        intensity = LLUI_LED_MIN_INTENSITY;
    }

    if (intensity > LLUI_LED_MAX_INTENSITY) {
        intensity = LLUI_LED_MAX_INTENSITY;
    }

    led_intensity[ledID] = intensity;

    led_strip_set_pixel(led_strip, 0, led_intensity[0], led_intensity[1], led_intensity[2]);
    led_strip_refresh(led_strip);
}
