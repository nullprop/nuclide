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

typedef enumflags
{
	SPECFL_ORIGIN,
	SPECFL_VELOCITY,
	SPECFL_TARGET,
	SPECFL_MODE,
	SPECFL_FLAGS
} NSClientSpectatorFlags_t;

typedef enum
{
	SPECMODE_FREE,
	SPECMODE_THIRDPERSON,
	SPECMODE_FIRSTPERSON,
	SPECMODE_OVERVIEW
} NSClientSpectatorMode_t;

enumflags
{
	SPECFLAG_BUTTON_RELEASED,
};

/** This entity class represents every spectator client.

These types of clients are not meant to interfere with the gameplay,
they are merely observers.

NSClientPlayer is a sub-class which has the ability to interact with games.

When clients connect via the `spectate` command, they will findthemselves
of type NSClientSpectator.
*/
class NSClientSpectator:NSClient
{
private:
	PREDICTED_FLOAT(spec_ent)
	PREDICTED_FLOAT(spec_flags)
	NSClientSpectatorMode_t spec_mode; NSClientSpectatorMode_t spec_mode_net;

	vector spec_org;

	int sequence;

public:
	void NSClientSpectator(void);

	virtual void ClientInput(void);
	
	virtual void InputNext(void);
	virtual void InputPrevious(void);
	virtual void InputMode(void);
	
	virtual void WarpToTarget(void);
	
	virtual void PreFrame(void);
	virtual void PostFrame(void);
	virtual void SpectatorTrackPlayer(void);
	
	virtual bool IsFakeSpectator(void);
	virtual bool IsRealSpectator(void);
	virtual bool IsDead(void);
	virtual bool IsPlayer(void);
	
	#ifdef SERVER
	virtual void Save(float);
	virtual void Restore(string,string);
	virtual void EvaluateEntity(void);
	virtual float SendEntity(entity,float);
	virtual void RunClientCommand(void);
	#else
	virtual void ClientInputFrame(void);
	virtual void ReceiveEntity(float,float);
	virtual float predraw(void);
	#endif
};

#ifdef CLIENT
void Spectator_ReadEntity(float new);
#endif
