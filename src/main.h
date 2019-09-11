#ifndef _MAIN_H
#define _MAIN_H

extern u16_t primary_addr;

extern struct bt_mesh_model root_models[];
extern struct bt_mesh_elem elements[];
extern const struct bt_mesh_comp comp;

extern struct bt_mesh_model *mod_onoff_cli[];

#endif