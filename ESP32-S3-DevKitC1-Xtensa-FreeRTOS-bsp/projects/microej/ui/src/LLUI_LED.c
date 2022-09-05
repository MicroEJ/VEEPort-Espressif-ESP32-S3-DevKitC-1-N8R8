/*
 * C
 *
 * Copyright 2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdio.h>
#include "LED_DRIVER_WS2812.h"
#include "LLUI_LED_impl.h"

/*============================================================================*/
/* LOCAL CONSTANTS */

/** @brief Define the maximum number of LEDs supported by the hardware platform.
 * Here we have an addressable RGB LED which is treated as 3 different LEDs.
 * This value must be a multiple of 3 (we have 3 colors on an RGB led package.
 */
#define ESP32S_S3_DEV_KIT_C1_MAX_LEDS (3)

/** @brief Define the GPIO that is used to control the addressable LED */
#define LED_CTRL_GPIO (48)

/** @brief Set the RMT peripheral channel. */
#define LED_RMT_CHANNEL (0)

/** @brief position definition of RED LED in RGB package */
#define LLUI_LED_RED_POSITION (0)

/** @brief position definition of GREEN LED in RGB package */
#define LLUI_LED_GREEN_POSITION (1)

/** @brief position definition of BLUE LED in RGB package */
#define LLUI_LED_BLUE_POSITION (2)

/*============================================================================*/
/* LOCAL TYPEDEFINITIONS */

/*============================================================================*/
/* EXPORTED TYPEDEFINITIONS */

/*============================================================================*/
/* LOCAL VARIABLES */

/** @brief led strip type */
static led_strip_t *pStrip_a;

/** @brief set intensity for leds [0..255] */
static uint32_t led_intensity[ESP32S_S3_DEV_KIT_C1_MAX_LEDS];

/*============================================================================*/
/* EXPORTED VARIABLES */

/*============================================================================*/
/* LOCAL FUNCTIONS */

/*============================================================================*/
/* EXPORTED FUNCTIONS */

/*============================================================================*/
/**
 * @brief Initializes the LEDs.
 */
int32_t LLUI_LED_IMPL_initialize(void)
{
	int8_t i; /**< local counter */
	int32_t led_count = ESP32S_S3_DEV_KIT_C1_MAX_LEDS;

	/* First, we reset intensity for all defined leds */
	for (i = 0; i < ESP32S_S3_DEV_KIT_C1_MAX_LEDS; i++){
		led_intensity[i] = 0;
	}

	/* LED strip initialization with the GPIO and pixels number*/
	pStrip_a = led_strip_init(LED_RMT_CHANNEL, LED_CTRL_GPIO, 1);
	if (NULL == pStrip_a){
		led_count = 0;
	}
	else{
		/* Set all LED off to clear all pixels */
		if (ESP_OK != pStrip_a->clear(pStrip_a, 50)){
			led_count = 0;
		}
	}

	return led_count;
}
/*============================================================================*/

/*============================================================================*/
/**
 * @brief Get the intensity of the LED.
 */
int32_t LLUI_LED_IMPL_getIntensity(int32_t ledID)
{
	/* range checking for the input param (led ID) */
	if ((ledID < 0) || (ledID >= ESP32S_S3_DEV_KIT_C1_MAX_LEDS)){
		return LLUI_LED_MIN_INTENSITY;
	}

	/* we return the stored led intensity */
	return (led_intensity[ledID]);
}
/*============================================================================*/

/*============================================================================*/
/**
 * @brief Set the intensity of the LED.
 */
void LLUI_LED_IMPL_setIntensity(int32_t ledID, int32_t intensity)
{
	uint8_t led_phy_pos; /**< physical led index (led package) in the strip */
	uint8_t led_rgb_pos; /**< led selector on the physical led case (R=0. G=1, B=2) */
	esp_err_t status; /**< local status returned by the led strip functions */

	/* range checking for the input param (led ID) */
	if ((ledID < 0) || (ledID >= ESP32S_S3_DEV_KIT_C1_MAX_LEDS)){
		return;
	}

	/* We do not need to scale the 'intensity' parameter as it has the
	 * same input range as RGB LED intensity. We only check for range limits.
	 */
	 
	/* min. range checking for the intensity input parameter */
	if (intensity < LLUI_LED_MIN_INTENSITY){
		intensity = LLUI_LED_MIN_INTENSITY;
	}

	/* max. range checking for the intensity input parameter */
	if (intensity > LLUI_LED_MAX_INTENSITY){
		intensity = LLUI_LED_MAX_INTENSITY;
	}

	/* We extract the physical position of the RGB led within the strip based
	 * on the ledID received. We also extract the physical led within the RGB
	 * case, knowing that on a physical RGB led case we have 3 leds: R (=0),
	 * G (=1) and B (=2).
	 */
	led_phy_pos = ledID / 3;
	led_rgb_pos = ledID % 3;
	
	/* Based on the led position extracted, set the new intensity of the led. */
	switch (led_rgb_pos){
		case LLUI_LED_RED_POSITION:
			status = pStrip_a->set_pixel(pStrip_a, led_phy_pos, intensity, led_intensity[ledID + 1], led_intensity[ledID + 2]);
			break;
		case LLUI_LED_GREEN_POSITION:
			status = pStrip_a->set_pixel(pStrip_a, led_phy_pos, led_intensity[ledID - 1], intensity, led_intensity[ledID + 1]);
			break;
		case LLUI_LED_BLUE_POSITION:
			status = pStrip_a->set_pixel(pStrip_a, led_phy_pos, led_intensity[ledID - 2], led_intensity[ledID - 1], intensity);
			break;
		default:
			status = ESP_ERR_INVALID_ARG;
	}

	/* we set the intensity of the led and store it only if the operation is 
	 * successful.
	 */
	if (ESP_OK == status){
		/* Refresh the strip to send data */
		status = pStrip_a->refresh(pStrip_a, 100);
		if (ESP_OK == status) {
			led_intensity[ledID] = intensity;
		}
	}

	return;
}
/*============================================================================*/
