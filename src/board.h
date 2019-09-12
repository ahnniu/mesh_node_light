/* board.h - Board-specific hooks */

/*
 * Copyright (c) 2017 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if defined(CONFIG_BOARD_BBC_MICROBIT)
void board_output_number(bt_mesh_output_action_t action, u32_t number);

void board_prov_complete(void);

void board_init(void);

void light_on(void);

void light_off(void);

void blink_a_led(void);
#else
static inline void board_output_number(bt_mesh_output_action_t action,
				       u32_t number)
{
}

static inline void board_prov_complete(void)
{
}

static inline void board_init(void)
{
}

static inline void light_on(void)
{

}

static inline void light_off(void)
{

}
#endif
