#include <sys/printk.h>

#include <settings/settings.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>

#include "board.h"
#include "main.h"
#include "onoff_srv.h"

/* Model Operation Codes */
#define BT_MESH_MODEL_OP_GEN_ONOFF_GET		BT_MESH_MODEL_OP_2(0x82, 0x01)
#define BT_MESH_MODEL_OP_GEN_ONOFF_SET		BT_MESH_MODEL_OP_2(0x82, 0x02)
#define BT_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK	BT_MESH_MODEL_OP_2(0x82, 0x03)
#define BT_MESH_MODEL_OP_GEN_ONOFF_STATUS	BT_MESH_MODEL_OP_2(0x82, 0x04)

static void gen_onoff_srv_pub_status(struct bt_mesh_model *model)
{
	struct net_buf_simple *msg = model->pub->msg;
	struct onoff_state *srv_state = model->user_data;
	u8_t state;
	int err;

	state = srv_state->current;
  printk("Publish state: %d\n", state);

	bt_mesh_model_msg_init(msg, BT_MESH_MODEL_OP_GEN_ONOFF_STATUS);
	net_buf_simple_add_u8(msg, state);
	err = bt_mesh_model_publish(model);
	if (err) {
		printk("bt_mesh_model_publish err %d\n", err);
		return;
	}
}

static void gen_onoff_srv_handler_get(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct net_buf_simple *buf)
{
	NET_BUF_SIMPLE_DEFINE(msg, 2 + 1 + 4);
	struct onoff_state *srv_state = model->user_data;
	u8_t state;

	printk("addr 0x%04x onoff 0x%02x\n",
	       bt_mesh_model_elem(model)->addr, srv_state->current);

	bt_mesh_model_msg_init(&msg, BT_MESH_MODEL_OP_GEN_ONOFF_STATUS);
	net_buf_simple_add_u8(&msg, srv_state->current);

	if (bt_mesh_model_send(model, ctx, &msg, NULL, NULL)) {
		printk("Unable to send On Off Status response\n");
	}
}


static void gen_onoff_srv_handler_set_unack(struct bt_mesh_model *model,
				struct bt_mesh_msg_ctx *ctx,
				struct net_buf_simple *buf)
{
	u8_t buflen = buf->len;
	struct onoff_state *srv_state = model->user_data;

	u8_t target_onoff_state = net_buf_simple_pull_u8(buf);
	u8_t tid = net_buf_simple_pull_u8(buf);

	printk("Previous state: %d, recived new state: %d\n",
		srv_state->current, target_onoff_state);

	if(target_onoff_state) {
		light_on();
	} else {
		light_off();
	}

	if(target_onoff_state != srv_state->current) {
		srv_state->current = target_onoff_state;
		gen_onoff_srv_pub_status(model);
	}
}

static void gen_onoff_srv_handler_set(struct bt_mesh_model *model,
			  struct bt_mesh_msg_ctx *ctx,
			  struct net_buf_simple *buf)
{
	printk("gen_onoff_srv_handler_set\n");

	gen_onoff_srv_handler_set_unack(model, ctx, buf);
	gen_onoff_srv_handler_get(model, ctx, buf);
}

const struct bt_mesh_model_op gen_onoff_srv_op[] = {
	{ BT_MESH_MODEL_OP_GEN_ONOFF_GET, 0, gen_onoff_srv_handler_get },
	{ BT_MESH_MODEL_OP_GEN_ONOFF_SET, 2, gen_onoff_srv_handler_set },
	{ BT_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK, 2, gen_onoff_srv_handler_set_unack },
	BT_MESH_MODEL_OP_END,
};

struct button btn_a = {
	.model = &root_models[2],
	.srv_state = &onoff_srv_state
};

struct onoff_state onoff_srv_state = {
	.current = 0
};

struct k_work btn_b_work;

static void button_a_pressed_worker(struct k_work *work)
{
	u8_t new_state;
	struct button *btn = CONTAINER_OF(work, struct button, work);

	if(primary_addr == BT_MESH_ADDR_UNASSIGNED)
		return;

	new_state = btn->srv_state->current ? 0 : 1;

	btn->srv_state->current = new_state;

	printk("Change the onoff state locally: new_state: %d\n", new_state);

	if(new_state) {
		light_on();
	} else {
		light_off();
	}
	gen_onoff_srv_pub_status(btn->model);

}

static void button_b_pressed_worker(struct k_work *work)
{
	if(primary_addr) {
		bt_mesh_proxy_identity_enable();
		blink_a_led();
	}
}

void onoff_srv_init()
{
  /* Initialize button worker task*/
	k_work_init(&btn_a.work, button_a_pressed_worker);
	k_work_init(&btn_b_work, button_b_pressed_worker);
}