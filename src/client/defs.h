/*
 * Copyright (c) 2016-2022 Vera Visions LLC.
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

#include "text.h"
#include "textmenu.h"
#include "efx.h"
#include "font.h"
#include "fade.h"
#include "cmd.h"
#include "util.h"
#include "NSView.h"
#include "crosshair.h"

#define PRINTFLAG(x) if (cvar("net_showUpdates")) \
	print(sprintf("%f %s read update %s\n", time, classname, #x));

#define READENTITY_BYTE(field, changedflag) {\
	if (flChanged & changedflag) {\
		field = readbyte();\
		PRINTFLAG(changedflag); \
	}\
}

#define READENTITY_SHORT(field, changedflag) {\
	if (flChanged & changedflag) {\
		field = readshort();\
		PRINTFLAG(changedflag); \
	}\
}

#define READENTITY_FLOAT(field, changedflag) {\
	if (flChanged & changedflag) {\
		field = readfloat();\
		PRINTFLAG(changedflag); \
	}\
}

#define READENTITY_INT(field, changedflag) {\
	if (flChanged & changedflag) {\
		field = readint();\
		PRINTFLAG(changedflag); \
	}\
}

#define READENTITY_STRING(field, changedflag) {\
	if (flChanged & changedflag) {\
		field = readstring();\
		PRINTFLAG(changedflag); \
	}\
}

#define READENTITY_COORD(field, changedflag) {\
	if (flChanged & changedflag) {\
		field = readcoord();\
		PRINTFLAG(changedflag); \
	}\
}

#define READENTITY_ANGLE(field, changedflag) {\
	if (flChanged & changedflag) {\
		field = readshort() / (32767 / 360);\
		PRINTFLAG(changedflag); \
	}\
}

#define READENTITY_ENTITY(field, changedflag) {\
	if (flChanged & changedflag) {\
		field = findfloat(world, ::entnum, readentitynum());\
		PRINTFLAG(changedflag); \
	}\
}

#define READENTITY_ENTNUM(field, changedflag) {\
	if (flChanged & changedflag) {\
		field = readentitynum();\
		PRINTFLAG(changedflag); \
	}\
}

/* undocumented printcall types */
#define PRINT_LOW		0
#define PRINT_MEDIUM	1
#define PRINT_HIGH		2
#define PRINT_CHAT		3

const float MASK_GLOWS = 16;
var bool g_focus;
bool Util_IsFocused(void);

var int g_numplayerslots;
int Util_GetMaxPlayers(void);

/* fonts */
font_s FONT_16;
font_s FONT_20;
font_s FONT_CON;

//var string g_shellchrome;
var float g_shellchromeshader;
var float g_shellchromeshader_cull;

/* clientside cvars */
var float autocvar_in_zoomSensitivity = 1.0f;
var int autocvar_pm_stairSmoothing = TRUE;
var int autocvar_pm_thirdPerson = FALSE;
var int autocvar_cg_viewmodelFlip = FALSE;
var vector autocvar_con_color = [255,150,0];
var vector autocvar_vgui_color = [255,170,0];
var vector autocvar_cg_viewmodelOffset = [0,0,0];
var int autocvar_cg_viewmodelPass = 0;
var float autocvar_cg_viewmodelFov = 90.0f;
var float autocvar_cg_viewmodelScale = 1.0f;
var float autocvar_cg_hudAspect = 0.0f;
var bool autocvar_pm_crouchToggle = false;

/* particle descriptors */
var float PART_DUSTMOTE;
var float PART_BURNING;

/* misc globals */
vector video_mins;
vector video_res;
vector mouse_pos;
int g_iIntermission;

/* this actually belongs in builtins.h since its an undocumented global */
float clframetime;

string(string modelname, int frame, float frametime) spriteframe = #0;

void CSQC_UpdateSeat(void);

/** Like drawstring() but aligns text to the right from the specified screen coordinates. */
void
drawstring_r(vector p, string t, vector s, vector c, float a, float f)
{
	p[0] -= stringwidth(t, TRUE, s);
	drawstring(p, t, s, c, a, f);
}

void GameMessage_Setup(string, int);
void View_SetMuzzleflash(int);

void Event_Callback(float mtime, __inout float btime);
void View_AddEvent(void(void) pCallback, float flTime);
void View_PlayAnimation(int);
void View_PlayAnimation(int);
void Event_ProcessModel(float, int, string);
void ClientGame_ModelEvent(float, int, string);

void View_EnableViewmodel(void);
void View_DisableViewmodel(void);

/** Draws a non-filled rectangle with a specified outline. */
void drawrect(vector pos, vector sz, float thickness, vector rgb, float al, optional float dfl)
{
	/* top */
	drawfill(pos, [sz[0], thickness], rgb, al, dfl);
	/* bottom */
	drawfill(pos + [0, sz[1] - thickness], [sz[0], thickness], rgb, al, dfl);
	/* left */
	drawfill(pos + [0, thickness], [thickness, sz[1] - (thickness * 2)], rgb, al, dfl);
	/* right */
	drawfill(pos + [sz[0] - thickness, thickness], [thickness, sz[1] - (thickness * 2)], rgb, al, dfl);
}


/** Like drawpic, but instead of screen coords, it will take world coords.
Will project the 2D image relative to the active NSView that we're currently
rendering in (g_view). So it may only be called within certain contexts. */
void
drawpic3d(vector worldpos, string mat, vector sz, vector rgb, float alpha)
{
	static bool
	drawpic3d_visible(vector p1) {
		vector delta;
		float fov;
		vector p2 = g_view.GetCameraOrigin();
		vector ang = g_view.GetCameraAngle();

		makevectors(ang);
		delta = normalize (p1 - p2);
		fov = delta * v_forward;

		if (fov > (g_view.GetAFOV()/180)) {
			traceline(p2, p1, MOVE_WORLDONLY, self);
			if (trace_fraction == 1.0) {
				return (1);
			}
		}
		return (0);
	}

	if (drawpic3d_visible(worldpos) == true) {
		vector vecProj = project(worldpos) - (sz/2);
		drawpic(vecProj, mat, sz, rgb, alpha);
	}
}

/** Like precache_pic, but will precache sky/cube map images (_bk, _dn etc.) */
void
precache_cubemap(string path)
{
	precache_pic(strcat(path, "_bk"));
	precache_pic(strcat(path, "_dn"));
	precache_pic(strcat(path, "_ft"));
	precache_pic(strcat(path, "_lf"));
	precache_pic(strcat(path, "_rt"));
	precache_pic(strcat(path, "_up"));
}

struct
{
	/* viewmodel stuff */
	entity m_eViewModel;
	entity m_eMuzzleflash;
	int m_iVMBones;
	entity m_eViewModelL;
	entity m_eMuzzleflashL;
	int m_iVMBonesL;

	void(void) m_pEventCall;
	float m_flEventTime;
	float m_flEventFrame;
	float m_flEventMdl;
	int m_iEventWeapon;

	int m_iLastWeapon;
	int m_iOldWeapon;

	/* damage overlay */
	float m_flDamageAlpha;
	vector m_vecDamagePos;
	int m_iDamageFlags;

	/* +zoomin cmd */
	int m_iZoomed;
	float m_flZoomTime;

	/* player fields */
	entity m_ePlayer;
	vector m_vecPredictedOrigin;
	vector m_vecPredictedOriginOld;
	vector m_vecPredictedVelocity;
	float m_flPredictedFlags;
	
	/* camera fields */
	vector m_vecCameraOrigin;
	vector m_vecCameraAngle;
	float m_flCameraTime;

	int m_iHUDWeaponSelected;
	float m_flHUDWeaponSelectTime;

	int m_iScoresVisible;

	/* saturn controller */
	int m_iSaturnMenu;

	/* centerprint related */
	float m_flCenterprintAlpha;
	float m_flCenterprintTime;
	float m_iCenterprintLines;
	string m_strCenterprintBuffer[18];

	/* chat related */
	float m_flPrintTime;
	string m_strPrintBuffer[5];
	int m_iPrintLines;

	bool m_iInputAttack;
	int m_iInputAttack2;
	int m_iInputReload;
	int m_iInputUse;
	int m_iInputDuck;
	int m_iInputExtra1;
	int m_iInputExtra2;
	float m_flInputBlockTime;
	
	/* fading */
	float m_flFadeDuration;
	float m_flFadeHold;
	float m_flFadeMaxAlpha;
	float m_flFadeStyle;
	float m_flFadeAlpha;
	float m_flFadeTime;
	vector m_vecFadeColor;
	int m_iFadeActive;

	/* shake */
	float m_flShakeFreq;
	float m_flShakeDuration;
	float m_flShakeTime;
	float m_flShakeAmp;

	vector m_vecLag;

	/* vehicles */
	float m_flVehTransition;
	vector m_vecVehEntry;

	/* new XR helpers */
	bool m_bMoveForward;

	bool m_bInterfaceFocused;
} g_seats[4], *pSeat;

var vector g_vecMousePos;
