/***
*
*	Copyright (c) 2016-2019 Marco 'eukara' Hladik. All rights reserved.
*
*	See the file LICENSE attached with the sources for usage details.
*
****/

/*
===============================================================================

	Cubemap Entity Development Functions

===============================================================================
*/

int g_iCubeProcess;
entity g_eCubeCycle;
vector g_vecCubePos;

void CMap_Check(void);
void CMap_Shoot(void);

class env_cubemap:CBaseEntity {
	int m_iSize;

	void() env_cubemap;
	virtual void(string, string) SpawnKey;
};

void env_cubemap::SpawnKey(string strField, string strKey)
{
	switch (strField) {
		case "scale":
			m_iSize = stoi(strKey);
			if (m_iSize <= 16) {
				m_iSize = 16;
			} else if (m_iSize <= 32) {
				m_iSize = 32;
			} else if (m_iSize <= 64) {
				m_iSize = 64;
			} else if (m_iSize <= 128) {
				m_iSize = 128;
			} else {
				m_iSize = 256;
			}
			break;
		default:
			CBaseEntity::SpawnKey(strField, strKey);
	}
}

void env_cubemap::env_cubemap(void)
{
	m_iSize = 32;
	Init();
}

/*
=================
CMap_Check

	Checks whether or not a specific cubemap file was dumped, goes to the next
=================
*/
void CMap_Shoot(void)
{
	string strReflectcube;
	if ( self.owner ) {
		env_cubemap tmp = (env_cubemap) self.owner;
		print( "^3Cubemap processing...\n" );
		g_vecCubePos = tmp.origin;
		strReflectcube = sprintf(
									"env/%s_%d_%d_%d.dds",
									mapname,
									g_vecCubePos[0],
									g_vecCubePos[1],
									g_vecCubePos[2]);
		localcmd(sprintf(
							"screenshot_cubemap %s %i\n",
							strReflectcube,
							tmp.m_iSize));
		self.think = CMap_Check;
		self.nextthink = time;
	} else {
		print( "^2Cubemaps done...\n" );
		localcmd( "mod_findcubemaps\nvid_reload\n" );
		g_iCubeProcess = FALSE;
		remove( self );
	}
}

/*
=================
CMap_Check

	Checks whether or not a specific cubemap file was dumped, goes to the next
=================
*/
void CMap_Check(void)
{
	if (whichpack(sprintf(
							"textures/env/%s_%d_%d_%d.dds",
							mapname, 
							g_vecCubePos[0], 
							g_vecCubePos[1], 
							g_vecCubePos[2]))) {
		self.owner = find( self.owner, classname, "env_cubemap" );
		self.think = CMap_Shoot;
	}
	self.nextthink = time;
}

/*
=================
CMap_Build

	Called upon 'buildcubemaps'
=================
*/
void CMap_Build(void)
{
	if (g_iCubeProcess == TRUE) {
		return;
	}

	print("^4Building cubemaps...\n");
	g_eCubeCycle = spawn();
	g_eCubeCycle.owner = find(world, classname, "env_cubemap");

	if (g_eCubeCycle.owner) {
		g_eCubeCycle.think = CMap_Shoot;
		g_eCubeCycle.nextthink = time;
		g_iCubeProcess = TRUE;
	}
}
