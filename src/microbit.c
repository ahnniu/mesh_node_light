/* microbit.c - BBC micro:bit specific hooks */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <drivers/gpio.h>

#include <display/mb_display.h>

#include <bluetooth/mesh.h>

#include "board.h"
#include "onoff_srv.h"

#define BUTTON_DEBOUNCE_DELAY_MS 250

static u32_t oob_number;
static struct device *gpio;
static int is_prov_completed = 0;

static u32_t last_time = 0, time = 0;

static void button_pressed(struct device *dev, struct gpio_callback *cb,
			   u32_t pins)
{
	struct mb_display *disp = mb_display_get();

	time = k_uptime_get_32();

	if(time < last_time + BUTTON_DEBOUNCE_DELAY_MS) {
		last_time = time;
		return;
	}

	last_time = time;

	if(!is_prov_completed)
		mb_display_print(disp, MB_DISPLAY_MODE_DEFAULT, K_MSEC(500),
				"%04u", oob_number);
	else
		k_work_submit(&btn_a.work);
}

static void configure_button(void)
{
	static struct gpio_callback button_cb;

	gpio = device_get_binding(DT_ALIAS_SW0_GPIOS_CONTROLLER);

	gpio_pin_configure(gpio, DT_ALIAS_SW0_GPIOS_PIN,
			   (GPIO_DIR_IN | GPIO_INT | GPIO_INT_EDGE |
			    GPIO_INT_ACTIVE_LOW));

	gpio_init_callback(&button_cb, button_pressed, BIT(DT_ALIAS_SW0_GPIOS_PIN));

	gpio_add_callback(gpio, &button_cb);

	gpio_pin_enable_callback(gpio, DT_ALIAS_SW0_GPIOS_PIN);
}

void board_output_number(bt_mesh_output_action_t action, u32_t number)
{
	struct mb_display *disp = mb_display_get();
	struct mb_image arrow = MB_IMAGE({ 0, 0, 1, 0, 0 },
					 { 0, 1, 0, 0, 0 },
					 { 1, 1, 1, 1, 1 },
					 { 0, 1, 0, 0, 0 },
					 { 0, 0, 1, 0, 0 });

	oob_number = number;

	// gpio_pin_enable_callback(gpio, DT_ALIAS_SW0_GPIOS_PIN);

	mb_display_image(disp, MB_DISPLAY_MODE_DEFAULT, K_FOREVER, &arrow, 1);
}

void board_prov_complete(void)
{
	struct mb_display *disp = mb_display_get();
	struct mb_image arrow = MB_IMAGE({ 0, 1, 0, 1, 0 },
					 { 0, 1, 0, 1, 0 },
					 { 0, 0, 0, 0, 0 },
					 { 1, 0, 0, 0, 1 },
					 { 0, 1, 1, 1, 0 });

	is_prov_completed = 1;
	// gpio_pin_disable_callback(gpio, DT_ALIAS_SW0_GPIOS_PIN);

	mb_display_image(disp, MB_DISPLAY_MODE_DEFAULT, K_SECONDS(10),
			 &arrow, 1);
}

void board_init(void)
{
	struct mb_display *disp = mb_display_get();
	static struct mb_image blink[] = {
		MB_IMAGE({ 1, 1, 1, 1, 1 },
			 { 1, 1, 1, 1, 1 },
			 { 1, 1, 1, 1, 1 },
			 { 1, 1, 1, 1, 1 },
			 { 1, 1, 1, 1, 1 }),
		MB_IMAGE({ 0, 0, 0, 0, 0 },
			 { 0, 0, 0, 0, 0 },
			 { 0, 0, 0, 0, 0 },
			 { 0, 0, 0, 0, 0 },
			 { 0, 0, 0, 0, 0 })
	};

	mb_display_image(disp, MB_DISPLAY_MODE_DEFAULT | MB_DISPLAY_FLAG_LOOP,
			 K_SECONDS(1), blink, ARRAY_SIZE(blink));

	configure_button();
}

void light_on(void)
{
	struct mb_display *disp = mb_display_get();
	struct mb_image arrow = 	MB_IMAGE({ 0, 0, 0, 0, 0 },
			 { 0, 0, 0, 0, 0 },
			 { 0, 0, 1, 0, 0 },
			 { 0, 0, 0, 0, 0 },
			 { 0, 0, 0, 0, 0 });

	mb_display_image(disp, MB_DISPLAY_MODE_DEFAULT, K_FOREVER,
			 &arrow, 1);
}

void light_off(void)
{
	struct mb_display *disp = mb_display_get();
	struct mb_image arrow = 	MB_IMAGE({ 0, 0, 0, 0, 0 },
			 { 0, 0, 0, 0, 0 },
			 { 0, 0, 0, 0, 0 },
			 { 0, 0, 0, 0, 0 },
			 { 0, 0, 0, 0, 0 });

	mb_display_image(disp, MB_DISPLAY_MODE_DEFAULT, K_FOREVER,
			 &arrow, 1);
}