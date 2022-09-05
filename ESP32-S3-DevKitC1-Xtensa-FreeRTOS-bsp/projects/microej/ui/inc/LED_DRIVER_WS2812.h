/*
 * C
 *
 * Copyright 2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief Driver implementation using RMT module, for the WS2812 LED strip.
 */

#include "esp_err.h"

/*============================================================================*/
/* EXPORTED CONSTANTS */

/**
 * @brief Default configuration for LED strip
 *
 */
#define LED_STRIP_DEFAULT_CONFIG(number, dev_hdl) \
    {                                             \
        .max_leds = number,                       \
        .dev = dev_hdl,                           \
    }

/*============================================================================*/
/* EXPORTED TYPEDEFINITIONS */

/**
* @brief LED Strip Type
*
*/
typedef struct led_strip_s led_strip_t;

/**
* @brief LED Strip Device Type
*
*/
typedef void *led_strip_dev_t;

/**
* @brief Declare of LED Strip Type
*
*/
struct led_strip_s {
    /**
    * @brief Set RGB for a specific pixel
    * @details
    * @param strip: LED strip
    * @param index: index of pixel to set
    * @param red: red part of color
    * @param green: green part of color
    * @param blue: blue part of color
    * @return
    *      - ESP_OK: Set RGB for a specific pixel successfully
    *      - ESP_ERR_INVALID_ARG: Set RGB for a specific pixel failed because of invalid parameters
    *      - ESP_FAIL: Set RGB for a specific pixel failed because other error occurred
    * @author
    * @date
    * @version 1
    */
    esp_err_t (*set_pixel)(led_strip_t *strip, uint32_t index, uint32_t red, uint32_t green, uint32_t blue);

    /**
    * @brief Refresh memory colors to LEDs
    * @details
    * @param strip: LED strip
    * @param timeout_ms: timeout value for refreshing task
    * @return
    *      - ESP_OK: Refresh successfully
    *      - ESP_ERR_TIMEOUT: Refresh failed because of timeout
    *      - ESP_FAIL: Refresh failed because some other error occurred
    * @note:
    *      After updating the LED colors in the memory, a following invocation of this API is needed to flush colors to strip.
    * @author
    * @date
    * @version 1
    */
    esp_err_t (*refresh)(led_strip_t *strip, uint32_t timeout_ms);

    /**
    * @brief Clear LED strip (turn off all LEDs)
    * @details
    * @param strip: LED strip
    * @param timeout_ms: timeout value for clearing task
    * @return
    *      - ESP_OK: Clear LEDs successfully
    *      - ESP_ERR_TIMEOUT: Clear LEDs failed because of timeout
    *      - ESP_FAIL: Clear LEDs failed because some other error occurred
    * @author
    * @date
    * @version 1
    */
    esp_err_t (*clear)(led_strip_t *strip, uint32_t timeout_ms);

    /**
    * @brief Free LED strip resources
    * @details
    * @param strip: LED strip
    * @return
    *      - ESP_OK: Free resources successfully
    *      - ESP_FAIL: Free resources failed because error occurred
    * @author
    * @date
    * @version 1
    */
    esp_err_t (*del)(led_strip_t *strip);
};

/**
* @brief LED Strip Configuration Type
*
*/
typedef struct {
    uint32_t max_leds;   /**< Maximum LEDs in a single strip */
    led_strip_dev_t dev; /**< LED strip device (e.g. RMT channel, PWM channel, etc) */
} led_strip_config_t;


/*============================================================================*/
/* EXPORTED VARIABLES */


/*============================================================================*/
/* EXPORTED FUNCTIONS */

/**
 * @brief Install a new ws2812 driver (based on RMT peripheral)
 * @details
 * @param config: LED strip configuration
 * @return
 *      LED strip instance or NULL
 * @author
 * @date
 * @version 1
 */
led_strip_t *led_strip_new_rmt_ws2812(const led_strip_config_t *config);

/**
 * @brief Init the RMT peripheral and LED strip configuration.
 * @param[in] channel: RMT peripheral channel number.
 * @param[in] gpio: GPIO number for the RMT data output.
 * @param[in] led_num: number of addressable LEDs.
 * @return
 *      LED strip instance or NULL
 * @author
 * @date
 * @version 1
 */
led_strip_t * led_strip_init(uint8_t channel, uint8_t gpio, uint16_t led_num);

/**
 * @brief Denit the RMT peripheral.
 * @details
 * @param[in] strip: LED strip
 * @return
 *     - ESP_OK
 *     - ESP_FAIL
 * @author
 * @date
 * @version 1
 */
esp_err_t led_strip_denit(led_strip_t *strip);

#ifdef __cplusplus
}
#endif
