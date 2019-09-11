#ifndef _ONOFF_SRV_H
#define _ONOFF_SRV_H

struct onoff_state {
	u8_t current;
};

struct button {
	struct bt_mesh_model *model;
	struct onoff_state *srv_state;
	struct k_work work;
};

extern struct button btn_a;
extern struct onoff_state onoff_srv_state;

extern const struct bt_mesh_model_op gen_onoff_srv_op[];


extern void onoff_srv_init();



#endif