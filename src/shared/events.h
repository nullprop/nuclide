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

/* one-time network events */
enum
{
	EV_INTERMISSION,
	EV_MUSICTRACK,
	EV_MUSICLOOP,
	EV_WEAPON_DRAW,
	EV_WEAPON_PRIMARYATTACK,
	EV_WEAPON_SECONDARYATTACK,
	EV_WEAPON_RELOAD,
	EV_WEAPON_PICKUP,
	EV_DAMAGE,
	EV_ANGLE,
	EV_IMPACT,
	EV_GIBHUMAN,
	EV_BLOOD,
	EV_EXPLOSION,
	EV_SPARK,
	EV_SHAKE,
	EV_HUDHINT,
	EV_FADE,
	EV_TEXT,
	EV_MESSAGE,
	EV_SPRITE,
	EV_MODELGIB,
	EV_CAMERATRIGGER,
	EV_OLDREMOVEME,
	EV_OBITUARY, // new one
	EV_SPEAK,
	EV_SENTENCE,
	EV_CHAT,
	EV_CHAT_TEAM,
	EV_CHAT_VOX,
	EV_VIEWMODEL,
	EV_SEPARATOR
};
