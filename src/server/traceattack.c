/*
 * Copyright (c) 2016-2020 Marco Hladik <marco@icculus.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/* cast a single bullet shot */
void
TraceAttack_FireSingle(vector vecPos, vector vAngle, int iDamage, int iWeapon)
{
	string tex_name;
	vector range;
	float surf;

	range = (vAngle * 8196);

	traceline(vecPos, vecPos + range, MOVE_LAGGED | MOVE_HITMODEL, self);

	if (trace_fraction >= 1.0f)
		return;

	if (trace_ent.takedamage == DAMAGE_YES) {
#ifdef CSTRIKE
		/* modify the damage based on the location */
		switch (trace_surface_id) {
		case BODY_HEAD:
			/* the helmet is one power house */
			if (trace_ent.items & ITEM_HELMET) {
				iDamage = 0;
				sound(self, CHAN_ITEM, "weapons/ric_metal-2.wav", 1, ATTN_IDLE);
				trace_ent.items &= ~ITEM_HELMET;
				return;
			} else {
				iDamage *= 4;
			}
			break;
		case BODY_STOMACH:
			iDamage *= 0.9;
			break;
		case BODY_LEGLEFT:
		case BODY_LEGRIGHT:
			iDamage *= 0.4;
			break;
		}
#else
		/* only headshots count in HL */
		if (trace_surface_id == BODY_HEAD)
			iDamage *= 3;
#endif
		Damage_Apply(trace_ent, self, iDamage, iWeapon, DMG_BULLET);
	}

	if (trace_ent.iBleeds == TRUE) {
		FX_Blood(trace_endpos, [1,0,0]);
		return;
	}

	switch (serverkeyfloat("*bspversion")) {
	case BSPVER_HL:
		surf = getsurfacenearpoint(trace_ent, trace_endpos);
		tex_name = Materials_FixName(getsurfacetexture(trace_ent, surf));

		/* our hashtable is the key to all this */
		switch ((float)hash_get(hashMaterials, tex_name)) {
		case MATID_ALIEN:
			FX_Impact(IMPACT_ALIEN, trace_endpos, trace_plane_normal);
			break;
		case MATID_COMPUTER:
			FX_Impact(IMPACT_COMPUTER, trace_endpos, trace_plane_normal);
			break;
		case MATID_CONCRETE:
			FX_Impact(IMPACT_CONCRETE, trace_endpos, trace_plane_normal);
			break;
		case MATID_DIRT:
			FX_Impact(IMPACT_DIRT, trace_endpos, trace_plane_normal);
			break;
		case MATID_BLOODYFLESH:
		case MATID_FLESH:
			FX_Impact(IMPACT_FLESH, trace_endpos, trace_plane_normal);
			break;
		case MATID_FOLIAGE:
			FX_Impact(IMPACT_FOLIAGE, trace_endpos, trace_plane_normal);
			break;
		case MATID_GLASS:
			FX_Impact(IMPACT_GLASS, trace_endpos, trace_plane_normal);
			break;
		case MATID_GRATE:
			FX_Impact(IMPACT_GRATE, trace_endpos, trace_plane_normal);
			break;
		case MATID_METAL:
			FX_Impact(IMPACT_METAL, trace_endpos, trace_plane_normal);
			break;
		case MATID_SLOSH:
			FX_Impact(IMPACT_SLOSH, trace_endpos, trace_plane_normal);
			break;
		case MATID_SNOW:
			FX_Impact(IMPACT_SNOW, trace_endpos, trace_plane_normal);
			break;
		case MATID_TILE:
			FX_Impact(IMPACT_TILE, trace_endpos, trace_plane_normal);
			break;
		case MATID_VENT:
			FX_Impact(IMPACT_VENT, trace_endpos, trace_plane_normal);
			break;
		case MATID_WOOD:
			FX_Impact(IMPACT_WOOD, trace_endpos, trace_plane_normal);
			break;
		default:
			FX_Impact(IMPACT_DEFAULT, trace_endpos, trace_plane_normal);
			break;
		}
		break;
	case BSPVER_Q3: /* Q3 */
	case BSPVER_RTCW: /* RtCW */
	case BSPVER_RBSP: /* RFVBSP */
		switch (trace_surfaceflagsi) {
		case SURF_ALIEN:
			FX_Impact(IMPACT_ALIEN, trace_endpos, trace_plane_normal);
			break;
		case SURF_COMPUTER:
			FX_Impact(IMPACT_COMPUTER, trace_endpos, trace_plane_normal);
			break;
		case SURF_CONCRETE:
			FX_Impact(IMPACT_CONCRETE, trace_endpos, trace_plane_normal);
			break;
		case SURF_DIRT:
			FX_Impact(IMPACT_DIRT, trace_endpos, trace_plane_normal);
			break;
		case SURF_BLOODYFLESH:
			FX_Impact(IMPACT_FLESH, trace_endpos, trace_plane_normal);
			break;
		case SURF_FOLIAGE:
			FX_Impact(IMPACT_FOLIAGE, trace_endpos, trace_plane_normal);
			break;
		case SURF_GLASS:
			FX_Impact(IMPACT_GLASS, trace_endpos, trace_plane_normal);
			break;
		case SURF_GRATE:
			FX_Impact(IMPACT_GRATE, trace_endpos, trace_plane_normal);
			break;
		case SURF_METAL:
			FX_Impact(IMPACT_METAL, trace_endpos, trace_plane_normal);
			break;
		case SURF_SLOSH:
			FX_Impact(IMPACT_SLOSH, trace_endpos, trace_plane_normal);
			break;
		case SURF_SNOW:
			FX_Impact(IMPACT_SNOW, trace_endpos, trace_plane_normal);
			break;
		case SURF_TILE:
			FX_Impact(IMPACT_TILE, trace_endpos, trace_plane_normal);
			break;
		case SURF_VENT:
			FX_Impact(IMPACT_VENT, trace_endpos, trace_plane_normal);
			break;
		case SURF_WOOD:
			FX_Impact(IMPACT_WOOD, trace_endpos, trace_plane_normal);
			break;
		default:
			FX_Impact(IMPACT_DEFAULT, trace_endpos, trace_plane_normal);
			break;
		}
		break;
	default:
		FX_Impact(IMPACT_DEFAULT, trace_endpos, trace_plane_normal);
	}

#ifdef BULLETPENETRATION
	if (iTotalPenetrations > 0) {
		iTotalPenetrations -= 1;
		TraceAttack_FireSingle(trace_endpos + (v_forward * 2), vAngle, iDamage / 2, iWeapon);
	}
#endif
}

/* fire a given amount of shots */
void
TraceAttack_FireBullets(int iShots, vector vecPos, int iDamage, vector vecSpread, int iWeapon)
{
	vector vDir;
	makevectors(self.v_angle);

	while (iShots > 0) {
		vDir = aim(self, 100000);
#ifndef BULLETPATTERNS
		vDir += random(-1,1) * vecSpread[0] * v_right;
		vDir += random(-1,1) * vecSpread[1] * v_up;
#else
		player pl = (player)self;

		/* weapons have already applied their multiplier... so attempt this */
		int multiplier = pl.cs_shotmultiplier - iShots;
		float frand = (multiplier / 6);

		/* shoddy attempt at spray patterns */
		if (frand < 1)
			frand = frand;
		else if (frand <= 2)
			frand = 2 - (frand * 1.5);

		vDir += frand * vecSpread[0] * v_right;
		vDir += (vecSpread[1] * v_up) * 2;
#endif
		TraceAttack_FireSingle(vecPos, vDir, iDamage, iWeapon);
		iShots--;
	}
}

#ifdef BULLETPENETRATION
void
TraceAttack_SetPenetrationPower(int power)
{
	iTotalPenetrations = power;
}
#endif
