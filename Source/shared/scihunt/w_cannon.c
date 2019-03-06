/***
*
*	Copyright (c) 2016-2019 Marco 'eukara' Hladik. All rights reserved.
*
*	See the file LICENSE attached with the sources for usage details.
*
****/

enum
{
	CANNON_FIREBOTH,
	CANNON_FIRELEFT,
	CANNON_FIRERIGHT,
	CANNON_RELOAD,
	CANNON_DEPLOY,
	CANNON_PUTAWAY,
	CANNON_IDLE1,
	CANNON_IDLE2
};

void w_cannon_precache(void)
{
	precache_model("models/v_cannon.mdl");
	precache_model("models/p_cannon.mdl");
	precache_sound("cannon/cin.wav");
	precache_sound("cannon/close.wav");
	precache_sound("cannon/cout.wav");
	precache_sound("cannon/fire.wav");
	precache_sound("cannon/open.wav");
}
string w_cannon_vmodel(void)
{
	return "models/v_cannon.mdl";
}
string w_cannon_pmodel(void)
{
	return "models/p_cannon.mdl";
}
string w_cannon_deathmsg(void)
{
	return "%s killed %s with handcannon.";
}

void w_cannon_reload(void)
{
	player pl = (player)self;
	if (pl.w_attack_next > 0) {
		return;
	}
#ifdef CSQC
	if (pl.a_ammo1 >= 2) {
		return;
	}
	if (!pl.a_ammo2) {
		return;
	}

	Weapons_ViewAnimation(CANNON_RELOAD);
#else
	if (pl.cannon_mag >= 2) {
		return;
	}
	if (!pl.ammo_buckshot) {
		return;
	}

	Weapons_ReloadWeapon(pl, player::cannon_mag, player::ammo_buckshot, 2);
	Weapons_UpdateAmmo(pl, pl.cannon_mag, pl.ammo_buckshot, __NULL__);

#endif

	pl.w_attack_next = 3.0f;
	pl.w_idle_next = 3.0f;
}

void w_cannon_pickup(void)
{
#ifdef SSQC
	player pl = (player)self;
	pl.cannon_mag = bound(0, pl.cannon_mag + 2, 2);
#endif
}

void w_cannon_draw(void)
{
#ifdef SSQC
	player pl = (player)self;
	Weapons_ViewAnimation(CANNON_DEPLOY);
	Weapons_UpdateAmmo(pl, pl.cannon_mag, pl.ammo_buckshot, __NULL__);
#endif
}

void w_cannon_holster(void)
{
#ifdef SSQC
	Weapons_ViewAnimation(CANNON_PUTAWAY);
#endif
}
void w_cannon_primary(void)
{
	player pl = (player)self;

	if (pl.w_attack_next > 0.0) {
		return;
	}

#ifdef CSQC
	if (pl.a_ammo1 != 2) {
		w_cannon_reload();
		return;
	}

	Weapons_ViewAnimation(CANNON_FIREBOTH);
	Weapons_ViewPunchAngle([-5,0,0]);
#else
	if (pl.cannon_mag != 2) {
		w_cannon_reload();
		return;
	}

	TraceAttack_FireBullets(20, pl.origin + pl.view_ofs, 5, [0.08716,0.04362]);
	pl.cannon_mag -= 2;
	Weapons_PlaySound(pl, CHAN_WEAPON, "cannon/fire.wav", 1, ATTN_NORM);
	Weapons_UpdateAmmo(pl, pl.cannon_mag, pl.ammo_buckshot, __NULL__);
#endif

	pl.w_attack_next = 1.5f;
	pl.w_idle_next = 2.5f;
}
void w_cannon_secondary(void)
{
	player pl = (player)self;

	if (pl.w_attack_next > 0.0) {
		return;
	}

#ifdef CSQC
	if (!pl.a_ammo1) {
		w_cannon_reload();
		return;
	}

	if (pl.a_ammo1 == 2) {
		Weapons_ViewAnimation(CANNON_FIRELEFT);
	} else {
		Weapons_ViewAnimation(CANNON_FIRERIGHT);
	}

	Weapons_ViewPunchAngle([-5,0,0]);
#else
	if (!pl.cannon_mag) {
		w_cannon_reload();
		return;
	}

	TraceAttack_FireBullets(10, pl.origin + pl.view_ofs, 5, [0.08716,0.04362]);
	pl.cannon_mag--;
	Weapons_PlaySound(pl, CHAN_WEAPON, "cannon/fire.wav", 1, ATTN_NORM);
	Weapons_UpdateAmmo(pl, pl.cannon_mag, pl.ammo_buckshot, __NULL__);
#endif

	pl.w_attack_next = 1.5f;
	pl.w_idle_next = 2.5f;
}
void w_cannon_release(void)
{
	player pl = (player)self;
	if (pl.w_idle_next > 0.0) {
		return;
	}

	int r = floor(random(0,2));
	switch (r) {
	case 0:
		Weapons_ViewAnimation(CANNON_IDLE1);
		break;
	case 1:
		Weapons_ViewAnimation(CANNON_IDLE2);
		break;
	}

	pl.w_idle_next = 8.0f;
}
void w_cannon_crosshair(void)
{
#ifdef CSQC
	static vector cross_pos;
	cross_pos = (video_res / 2) + [-12,-12];
	drawsubpic(cross_pos, [24,24], "sprites/crosshairs.spr_0.tga", [48/128,24/128], [0.1875, 0.1875], [1,1,1], 1, DRAWFLAG_NORMAL);
	HUD_DrawAmmo1();
	HUD_DrawAmmo2();
#endif
}

void w_cannon_hudpic(int s, vector pos)
{
#ifdef CSQC
	if (s) {
		drawsubpic(pos, [170,45], "sprites/w_cannon.spr_0.tga", [0,48/256], [170/256,45/256], g_hud_color, 1, DRAWFLAG_ADDITIVE);
	} else {
		drawsubpic(pos, [170,45], "sprites/w_cannon.spr_0.tga", [0,0], [170/256,45/256], g_hud_color, 1, DRAWFLAG_ADDITIVE);
	}
#endif
}

weapon_t w_cannon =
{
	ITEM_CANNON,
	2,
	3,
	w_cannon_draw,
	w_cannon_holster,
	w_cannon_primary,
	w_cannon_secondary,
	w_cannon_reload,
	w_cannon_release,
	w_cannon_crosshair,
	w_cannon_precache,
	w_cannon_pickup,
	w_cannon_vmodel,
	__NULL__,
	w_cannon_pmodel,
	w_cannon_deathmsg,
	w_cannon_hudpic
};
