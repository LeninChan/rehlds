/*
*
*    This program is free software; you can redistribute it and/or modify it
*    under the terms of the GNU General Public License as published by the
*    Free Software Foundation; either version 2 of the License, or (at
*    your option) any later version.
*
*    This program is distributed in the hope that it will be useful, but
*    WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software Foundation,
*    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*    In addition, as a special exception, the author gives permission to
*    link the code of this program with the Half-Life Game Engine ("HL
*    Engine") and Modified Game Libraries ("MODs") developed by Valve,
*    L.L.C ("Valve").  You must obey the GNU General Public License in all
*    respects for all of the code used other than the HL Engine and MODs
*    from Valve.  If you modify this file, you may extend this exception
*    to your version of the file, but you are not obligated to do so.  If
*    you do not wish to do so, delete this exception statement from your
*    version.
*
*/

#include "precompiled.h"

/* <a5976> ../engine/sv_main.c:96 */
typedef struct full_packet_entities_s
{
	int num_entities;
	entity_state_t entities[MAX_PACKET_ENTITIES];
} full_packet_entities_t;

int sv_lastnum;

extra_baselines_t g_sv_instance_baselines;

delta_t *g_pplayerdelta;
delta_t *g_pentitydelta;
delta_t *g_pcustomentitydelta;
delta_t *g_pclientdelta;
delta_t *g_pweapondelta;
#ifdef REHLDS_OPT_PEDANTIC
delta_t *g_pusercmddelta;
#endif

int hashstrings_collisions;

char *pr_strings;
qboolean scr_skipupdate;
float scr_centertime_off;
float g_LastScreenUpdateTime;

globalvars_t gGlobalVariables;
server_static_t g_psvs;
server_t g_psv;

rehlds_server_t g_rehlds_sv;

decalname_t sv_decalnames[512];
int sv_decalnamecount;

UserMsg *sv_gpNewUserMsgs;
UserMsg *sv_gpUserMsgs;

playermove_t g_svmove;

qboolean g_bOutOfDateRestart;


delta_info_t *g_sv_delta;
delta_t *g_peventdelta;

//int num_servers;

int gPacketSuppressed;

char localinfo[MAX_INFO_STRING * 128];
char localmodels[512][5];

ipfilter_t ipfilters[32768];
int numipfilters;
userfilter_t userfilters[32768];
int numuserfilters;


//cvar_t sv_language;

//cvar_t laddermode;

int sv_playermodel;

//int player_datacounts[32];
char outputbuf[MAX_ROUTEABLE_PACKET];

redirect_t sv_redirected;
netadr_t sv_redirectto;

// TODO: make one global var with mods enum.
int g_bCS_CZ_Flags_Initialized;
int g_bIsCZero;
int g_bIsCZeroRitual;
int g_bIsTerrorStrike;
int g_bIsTFC;
int g_bIsHL1;
int g_bIsCStrike;

qboolean allow_cheats;

/* 
 * Globals initialization 
 */
#ifndef HOOK_ENGINE

char *gNullString = "";
int SV_UPDATE_BACKUP = 8;
int SV_UPDATE_MASK = 7;
int giNextUserMsg = 64;

cvar_t sv_lan = { "sv_lan", "0", 0, 0.0f, NULL };
cvar_t sv_lan_rate = { "sv_lan_rate", "20000.0", 0, 0.0f, NULL };
cvar_t sv_aim = { "sv_aim", "1", FCVAR_SERVER | FCVAR_ARCHIVE , 0.0f, NULL };

cvar_t sv_skycolor_r = { "sv_skycolor_r", "0", 0, 0.0f, NULL };
cvar_t sv_skycolor_g = { "sv_skycolor_g", "0", 0, 0.0f, NULL };
cvar_t sv_skycolor_b = { "sv_skycolor_b", "0", 0, 0.0f, NULL };
cvar_t sv_skyvec_x = { "sv_skyvec_x", "0", 0, 0.0f, NULL };
cvar_t sv_skyvec_y = { "sv_skyvec_y", "0", 0, 0.0f, NULL };
cvar_t sv_skyvec_z = { "sv_skyvec_z", "0", 0, 0.0f, NULL };

cvar_t sv_spectatormaxspeed = { "sv_spectatormaxspeed", "500", 0, 0.0f, NULL };
cvar_t sv_airaccelerate = { "sv_airaccelerate", "10", FCVAR_SERVER, 0.0f, NULL };
cvar_t sv_wateraccelerate = { "sv_wateraccelerate", "10", FCVAR_SERVER, 0.0f, NULL };
cvar_t sv_waterfriction = { "sv_waterfriction", "1", FCVAR_SERVER, 0.0f, NULL };
cvar_t sv_zmax = { "sv_zmax", "4096", FCVAR_SPONLY, 0.0f, NULL };
cvar_t sv_wateramp = { "sv_wateramp", "0", 0, 0.0f, NULL };

cvar_t sv_skyname = { "sv_skyname", "desert", 0, 0.0f, NULL };
cvar_t mapcyclefile = { "mapcyclefile", "mapcycle.txt", 0, 0.0f, NULL };
cvar_t motdfile = { "motdfile", "motd.txt", 0, 0.0f, NULL };
cvar_t servercfgfile = { "servercfgfile", "server.cfg", 0, 0.0f, NULL };
cvar_t lservercfgfile = { "lservercfgfile", "listenserver.cfg", 0, 0.0f, NULL };
cvar_t logsdir = { "logsdir", "logs", 0, 0.0f, NULL };
cvar_t bannedcfgfile = { "bannedcfgfile", "banned.cfg", 0, 0.0f, NULL };

int g_userid = 1;

cvar_t rcon_password = { "rcon_password", "", 0, 0.0f, NULL };
cvar_t sv_enableoldqueries = { "sv_enableoldqueries", "0", 0, 0.0f, NULL };

cvar_t sv_instancedbaseline = { "sv_instancedbaseline", "1", 0, 0.0f, NULL };
cvar_t sv_contact = { "sv_contact", "", FCVAR_SERVER, 0.0f, NULL };
cvar_t sv_maxupdaterate = { "sv_maxupdaterate", "30.0", 0, 0.0f, NULL };
cvar_t sv_minupdaterate = { "sv_minupdaterate", "10.0", 0, 0.0f, NULL };
cvar_t sv_filterban = { "sv_filterban", "1", 0, 0.0f, NULL };
cvar_t sv_minrate = { "sv_minrate", "0", FCVAR_SERVER, 0.0f, NULL };
cvar_t sv_maxrate = { "sv_maxrate", "0", FCVAR_SERVER, 0.0f, NULL };
cvar_t sv_logrelay = { "sv_logrelay", "0", 0, 0.0f, NULL };

cvar_t violence_hblood = { "violence_hblood", "1", 0, 0.0f, NULL };
cvar_t violence_ablood = { "violence_ablood", "1", 0, 0.0f, NULL };
cvar_t violence_hgibs = { "violence_hgibs", "1", 0, 0.0f, NULL };
cvar_t violence_agibs = { "violence_agibs", "1", 0, 0.0f, NULL };
cvar_t sv_newunit = { "sv_newunit", "0", 0, 0.0f, NULL };

cvar_t sv_clienttrace = { "sv_clienttrace", "1", FCVAR_SERVER, 0.0f, NULL };
cvar_t sv_timeout = { "sv_timeout", "60", 0, 0.0f, NULL };
cvar_t sv_failuretime = { "sv_failuretime", "0.5", 0, 0.0f, NULL };
cvar_t sv_cheats = { "sv_cheats", "0", FCVAR_SERVER, 0.0f, NULL };
cvar_t sv_password = { "sv_password", "", FCVAR_SERVER | FCVAR_PROTECTED, 0.0f, NULL };
cvar_t sv_proxies = { "sv_proxies", "1", FCVAR_SERVER, 0.0f, NULL };
cvar_t sv_outofdatetime = { "sv_outofdatetime", "1800", 0, 0.0f, NULL };
cvar_t mapchangecfgfile = { "mapchangecfgfile", "", 0, 0.0f, NULL };

cvar_t sv_allow_download = { "sv_allowdownload", "1", 0, 0.0f, NULL };
cvar_t sv_send_logos = { "sv_send_logos", "1", 0, 0.0f, NULL };
cvar_t sv_send_resources = { "sv_send_resources", "1", 0, 0.0f, NULL };
cvar_t sv_log_singleplayer = { "sv_log_singleplayer", "0", 0, 0.0f, NULL };
cvar_t sv_logsecret = { "sv_logsecret", "0", 0, 0.0f, NULL };
cvar_t sv_log_onefile = { "sv_log_onefile", "0", 0, 0.0f, NULL };
cvar_t sv_logbans = { "sv_logbans", "0", 0, 0.0f, NULL };
cvar_t sv_allow_upload = { "sv_allowupload", "1", FCVAR_SERVER, 0.0f, NULL };
cvar_t sv_max_upload = { "sv_uploadmax", "0.5", FCVAR_SERVER, 0.0f, NULL };
cvar_t hpk_maxsize = { "hpk_maxsize", "4", FCVAR_ARCHIVE, 0.0f, NULL };
cvar_t sv_visiblemaxplayers = { "sv_visiblemaxplayers", "-1", 0, 0.0f, NULL };

cvar_t max_queries_sec = { "max_queries_sec", "3.0", FCVAR_SERVER | FCVAR_PROTECTED, 0.0f, NULL };
cvar_t max_queries_sec_global = { "max_queries_sec_global", "30", FCVAR_SERVER | FCVAR_PROTECTED, 0.0f, NULL };
cvar_t max_queries_window = { "max_queries_window", "60", FCVAR_SERVER | FCVAR_PROTECTED, 0.0f, NULL };
cvar_t sv_logblocks = { "sv_logblocks", "0", FCVAR_SERVER, 0.0f, NULL };
cvar_t sv_downloadurl = { "sv_downloadurl", "", FCVAR_PROTECTED, 0.0f, NULL };
cvar_t sv_allow_dlfile = { "sv_allow_dlfile", "1", 0, 0.0f, NULL };
#ifdef REHLDS_FIXES
cvar_t sv_version = { "sv_version", "", FCVAR_SERVER, 0.0f, NULL };
#else
cvar_t sv_version = {"sv_version", "", 0, 0.0f, NULL};
#endif

cvar_t sv_rcon_minfailures = { "sv_rcon_minfailures", "5", 0, 0.0f, NULL };
cvar_t sv_rcon_maxfailures = { "sv_rcon_maxfailures", "10", 0, 0.0f, NULL };
cvar_t sv_rcon_minfailuretime = { "sv_rcon_minfailuretime", "30", 0, 0.0f, NULL };
cvar_t sv_rcon_banpenalty = { "sv_rcon_banpenalty", "0", 0, 0.0f, NULL };

cvar_t scr_downloading = { "scr_downloading", "0", 0, 0.0f, NULL };

#else //HOOK_ENGINE

char *gNullString;
int SV_UPDATE_BACKUP;
int SV_UPDATE_MASK;
int giNextUserMsg;

cvar_t sv_lan;
cvar_t sv_lan_rate;
cvar_t sv_aim;

cvar_t sv_skycolor_r;
cvar_t sv_skycolor_g;
cvar_t sv_skycolor_b;
cvar_t sv_skyvec_x;
cvar_t sv_skyvec_y;
cvar_t sv_skyvec_z;

cvar_t sv_spectatormaxspeed;
cvar_t sv_airaccelerate;
cvar_t sv_wateraccelerate;
cvar_t sv_waterfriction;
cvar_t sv_zmax;
cvar_t sv_wateramp;

cvar_t sv_skyname;
cvar_t mapcyclefile;
cvar_t motdfile;
cvar_t servercfgfile;
cvar_t lservercfgfile;
cvar_t logsdir;
cvar_t bannedcfgfile;

int g_userid;

cvar_t rcon_password;
cvar_t sv_enableoldqueries;

cvar_t sv_instancedbaseline;
cvar_t sv_contact;
cvar_t sv_maxupdaterate;
cvar_t sv_minupdaterate;
cvar_t sv_filterban;
cvar_t sv_minrate;
cvar_t sv_maxrate;
cvar_t sv_logrelay;

cvar_t violence_hblood;
cvar_t violence_ablood;
cvar_t violence_hgibs;
cvar_t violence_agibs;
cvar_t sv_newunit;

cvar_t sv_clienttrace;
cvar_t sv_timeout;
cvar_t sv_failuretime;
cvar_t sv_cheats;
cvar_t sv_password;
cvar_t sv_proxies;
cvar_t sv_outofdatetime;
cvar_t mapchangecfgfile;

cvar_t sv_allow_download;
cvar_t sv_send_logos;
cvar_t sv_send_resources;
cvar_t sv_log_singleplayer;
cvar_t sv_logsecret;
cvar_t sv_log_onefile;
cvar_t sv_logbans;
cvar_t sv_allow_upload;
cvar_t sv_max_upload;
cvar_t hpk_maxsize;
cvar_t sv_visiblemaxplayers;

cvar_t max_queries_sec;
cvar_t max_queries_sec_global;
cvar_t max_queries_window;
cvar_t sv_logblocks;
cvar_t sv_downloadurl;
cvar_t sv_allow_dlfile;
cvar_t sv_version;

cvar_t sv_rcon_minfailures;
cvar_t sv_rcon_maxfailures;
cvar_t sv_rcon_minfailuretime;
cvar_t sv_rcon_banpenalty;

cvar_t scr_downloading;

#endif //HOOK_ENGINE


/* <a6492> ../engine/sv_main.c:113 */
delta_t *SV_LookupDelta(char *name)
{
	delta_info_t *p = g_sv_delta;

	while (p)
	{
		if (!Q_stricmp(name, p->name))
		{
			return p->delta;
		}
		p = p->next;
	}

	Sys_Error(__FUNCTION__ ": Couldn't find delta for %s\n", name);

	return NULL;
}

/* <a64e0> ../engine/sv_main.c:280 */
NOXREF void SV_DownloadingModules(void)
{
	NOXREFCHECK;

	char message[] = "Downloading Security Module from Speakeasy.net ...\n";
	Con_Printf(message);

#ifndef SWDS
	// TODO: Possibly some client-side code here
#endif // SWDS
}

/* <a654c> ../engine/sv_main.c:310 */
void SV_GatherStatistics(void)
{
	int players = 0;

	if (g_psvs.next_cleartime < realtime)
	{
		SV_CountPlayers(&players);
		g_psvs.next_cleartime = realtime + 3600.0;
		Q_memset(&g_psvs.stats, 0, sizeof(g_psvs.stats));
		g_psvs.stats.maxusers = players;
		g_psvs.stats.minusers = players;
		return;
	}

	if (g_psvs.next_sampletime > realtime)
		return;

	SV_CountPlayers(&players);

	g_psvs.stats.num_samples++;
	g_psvs.next_sampletime = realtime + 60.0;
	g_psvs.stats.cumulative_occupancy += (players * 100.0 / g_psvs.maxclients);

	if (g_psvs.stats.num_samples >= 1)
		g_psvs.stats.occupancy = g_psvs.stats.cumulative_occupancy / g_psvs.stats.num_samples;

	if (g_psvs.stats.minusers > players)
		g_psvs.stats.minusers = players;
	else if (g_psvs.stats.maxusers < players)
		g_psvs.stats.maxusers = players;

	if ((g_psvs.maxclients - 1) <= players)
		g_psvs.stats.at_capacity++;
	if (players <= 1)
		g_psvs.stats.at_empty++;

	if (g_psvs.stats.num_samples >= 1)
	{
		g_psvs.stats.capacity_percent = g_psvs.stats.at_capacity * 100.0 / g_psvs.stats.num_samples;
		g_psvs.stats.empty_percent = g_psvs.stats.at_empty * 100.0 / g_psvs.stats.num_samples;
	}

	int c = 0;
	float v = 0.0f;
	client_t *cl = g_psvs.clients;

	for (int i = g_psvs.maxclients; i > 0; i--, cl++)
	{
		if (cl->active && !cl->fakeclient)
		{
			c++;
			v += cl->latency;
		}
	}

	if (c)
		v = v / c;

	g_psvs.stats.cumulative_latency += v;
	if (g_psvs.stats.num_samples >= 1)
		g_psvs.stats.average_latency = g_psvs.stats.cumulative_latency / g_psvs.stats.num_samples;
	if (g_psvs.stats.num_sessions >= 1)
		g_psvs.stats.average_session_len = g_psvs.stats.cumulative_sessiontime / g_psvs.stats.num_sessions;
}

/* <a6628> ../engine/sv_main.c:432 */
void SV_DeallocateDynamicData(void)
{
	if (g_moved_edict)
		Mem_Free(g_moved_edict);
	if (g_moved_from)
		Mem_Free(g_moved_from);
	g_moved_edict = NULL;
	g_moved_from = NULL;
}

/* <a6644> ../engine/sv_main.c:450 */
void SV_ReallocateDynamicData(void)
{
	if (!g_psv.max_edicts)
	{
		Con_DPrintf(__FUNCTION__ " with sv.max_edicts == 0\n");
		return;
	}

	int nSize = g_psv.max_edicts;

	if (g_moved_edict)
	{
		Con_Printf("Reallocate on moved_edict\n");
#ifdef REHLDS_FIXES
		Mem_Free(g_moved_edict);
#endif
	}
	g_moved_edict = (edict_t **)Mem_ZeroMalloc(sizeof(edict_t *) * nSize);

	if (g_moved_from)
	{
		Con_Printf("Reallocate on moved_from\n");
#ifdef REHLDS_FIXES
		Mem_Free(g_moved_from);
#endif
	}
	g_moved_from = (vec3_t *)Mem_ZeroMalloc(sizeof(vec3_t) * nSize);
}

/* <a667f> ../engine/sv_main.c:485 */
void SV_AllocClientFrames(void)
{
	client_t *cl = g_psvs.clients;

	for (int i = 0; i < g_psvs.maxclientslimit; i++, cl++)
	{
		if (cl->frames)
		{
			Con_DPrintf("Allocating over frame pointer?\n");
#ifdef REHLDS_FIXES
			Mem_Free(cl->frames);
#endif
		}
		cl->frames = (client_frame_t *)Mem_ZeroMalloc(sizeof(client_frame_t) * SV_UPDATE_BACKUP);
	}
}

/* <a5dbe> ../engine/sv_main.c:500 */
qboolean SV_IsPlayerIndex(int index)
{
	return (index >= 1 && index <= g_psvs.maxclients);
}

qboolean __declspec(naked) SV_IsPlayerIndex_wrapped(int index)
{
	// Original SV_IsPlayerIndex in swds.dll doesn't modify ecx nor edx.
	// During the compilation of original swds.dll compiler was assuming that these registers wouldn't be modified during call to SV_IsPlayerIndex().
	// This is not true for code produced by msvc2013 (which uses ecx even in Release config). That's why we need a wrapper here that preserves ecx and edx before call to reversed SV_IsPlayerIndex().
	__asm
	{
		mov eax, dword ptr[esp + 4];
		push ecx;
		push edx;
		push eax;
		call SV_IsPlayerIndex;
		add esp, 4;
		pop edx;
		pop ecx;
		retn;
	}
}

/* <a6717> ../engine/sv_main.c:514 */
void SV_ClearPacketEntities(client_frame_t *frame)
{
	if (frame)
	{
		if (frame->entities.entities)
			Mem_Free(frame->entities.entities);
		frame->entities.entities = NULL;
		frame->entities.num_entities = 0;
	}
}

/* <a677e> ../engine/sv_main.c:532 */
void SV_AllocPacketEntities(client_frame_t *frame, int numents)
{
	if (frame)
	{
#ifdef REHLDS_OPT_PEDANTIC
		frame->entities.num_entities = numents;

		// only alloc for max possible numents
		if (!frame->entities.entities)
			frame->entities.entities = (entity_state_t *)Mem_ZeroMalloc(sizeof(entity_state_t) * MAX_PACKET_ENTITIES);
#else // REHLDS_OPT_PEDANTIC
		if (frame->entities.entities)
			SV_ClearPacketEntities(frame);

		int allocatedslots = numents;
		if (numents < 1)
			allocatedslots = 1;

		frame->entities.num_entities = numents;
		frame->entities.entities = (entity_state_t *)Mem_ZeroMalloc(sizeof(entity_state_t) * allocatedslots);
#endif // REHLDS_OPT_PEDANTIC
	}
}

/* <a6817> ../engine/sv_main.c:561 */
void SV_ClearFrames(client_frame_t **frames)
{
	client_frame_t *pframe = *frames;

	if (pframe)
	{
		for (int i = 0; i < SV_UPDATE_BACKUP; i++, pframe++)
		{
			SV_ClearPacketEntities(pframe);
			pframe->senttime = 0.0;
			pframe->ping_time = -1.0f;
		}

		Mem_Free(*frames);
		*frames = NULL;
	}
}

/* <a5e8f> ../engine/sv_main.c:593 */
void SV_Serverinfo_f(void)
{
	if (Cmd_Argc() == 1)
	{
		Con_Printf("Server info settings:\n");
		Info_Print(Info_Serverinfo());
		return;
	}
	if (Cmd_Argc() != 3)
	{
		Con_Printf("usage: serverinfo [ <key> <value> ]\n");
		return;
	}
	if (Cmd_Argv(1)[0] == '*')
	{
		Con_Printf("Star variables cannot be changed.\n");
		return;
	}

#ifdef REHLDS_FIXES
	Info_SetValueForKey(Info_Serverinfo(), Cmd_Argv(1), Cmd_Argv(2), MAX_INFO_STRING);	// Do it right
#else // REHLDS_FIXES
	Info_SetValueForKey(Info_Serverinfo(), Cmd_Argv(1), Cmd_Argv(2), 512);
#endif // REHLDS_FIXES

	cvar_t *var = Cvar_FindVar(Cmd_Argv(1));
	if (var)
	{
		Z_Free(var->string);
		var->string = CopyString((char *)Cmd_Argv(2));
		var->value = Q_atof(var->string);
	}

	SV_BroadcastCommand("fullserverinfo \"%s\"\n", Info_Serverinfo());
}

/* <a5c4a> ../engine/sv_main.c:638 */
void SV_Localinfo_f(void)
{
	if (Cmd_Argc() == 1)
	{
		Con_Printf("Local info settings:\n");
		Info_Print(localinfo);
		return;
	}
	if (Cmd_Argc() != 3)
	{
		Con_Printf("usage: localinfo [ <key> <value> ]\n");
		return;
	}
	if (Cmd_Argv(1)[0] == '*')
	{
		Con_Printf("Star variables cannot be changed.\n");
		return;
	}

	Info_SetValueForKey(localinfo, Cmd_Argv(1), Cmd_Argv(2), MAX_INFO_STRING * 128);
}

/* <a5c19> ../engine/sv_main.c:672 */
void SV_User_f(void)
{
	if (!g_psv.active)
	{
		Con_Printf("Can't 'user', not running a server\n");
		return;
	}
	if (Cmd_Argc() != 2)
	{
		Con_Printf("Usage: user <username / userid>\n");
		return;
	}

	int uid = Q_atoi(Cmd_Argv(1));
	client_t *cl = g_psvs.clients;

	for (int i = 0; i < g_psvs.maxclients; i++, cl++)
	{
		if (cl->active || cl->spawned || cl->connected)
		{
			if (!cl->fakeclient && cl->name[0])
			{
				if (cl->userid == uid || !Q_strcmp(cl->name, Cmd_Argv(1)))
				{
					Info_Print(cl->userinfo);
					return;
				}
			}
		}
	}

	Con_Printf("User not in server.\n");
}

/* <a6026> ../engine/sv_main.c:721 */
void SV_Users_f(void)
{
	if (!g_psv.active)
	{
		Con_Printf("Can't 'users', not running a server\n");
		return;
	}

	Con_Printf("userid : uniqueid : name\n");
	Con_Printf("------ : ---------: ----\n");

	int c = 0;
	client_t *cl = g_psvs.clients;

	for (int i = 0; i < g_psvs.maxclients; i++, cl++)
	{
		if (cl->active || cl->spawned || cl->connected)
		{
			if (!cl->fakeclient && cl->name[0])
			{
				Con_Printf("%6i : %s : %s\n", cl->userid, SV_GetClientIDString(cl), cl->name);
				c++;
			}
		}
	}

	Con_Printf("%i users\n", c);
}

/* <a651b> ../engine/sv_main.c:762 */
void SV_CountPlayers(int *clients)
{
	int count = 0;
	
	client_s *cl = g_psvs.clients;
	for (int i = 0; i < g_psvs.maxclients; i++, cl++)
	{
		if (cl->active | cl->spawned | cl->connected)
			count++;
	}

	*clients = count;
}

/* <a68a4> ../engine/sv_main.c:786 */
void SV_CountProxies(int *proxies)
{
	*proxies = 0;
	client_s *cl = g_psvs.clients;

	for (int i = 0; i < g_psvs.maxclients; i++, cl++)
	{
		if (cl->active || cl->spawned || cl->connected)
		{
			if (cl->proxy)
				(*proxies)++;
		}
	}
}

/* <a6905> ../engine/sv_main.c:810 */
void SV_FindModelNumbers(void)
{
	sv_playermodel = -1;

	for (int i = 0; i < HL_MODEL_MAX; i++)
	{
		if (!g_psv.model_precache[i])
			break;

		//use case-sensitive names to increase performance
#ifdef REHLDS_FIXES
		if (!Q_stricmp(g_psv.model_precache[i], "models/player.mdl"))
			sv_playermodel = i;
#else
		if (!Q_stricmp(g_psv.model_precache[i], "models/player.mdl"))
			sv_playermodel = i;
#endif
	}
}

/* <a693e> ../engine/sv_main.c:841 */
void SV_StartParticle(const vec_t *org, const vec_t *dir, int color, int count)
{
	if (g_psv.datagram.cursize + 16 <= g_psv.datagram.maxsize)
	{
		MSG_WriteByte(&g_psv.datagram, svc_particle);
		MSG_WriteCoord(&g_psv.datagram, org[0]);
		MSG_WriteCoord(&g_psv.datagram, org[1]);
		MSG_WriteCoord(&g_psv.datagram, org[2]);

		for (int i = 0; i < 3; i++)
		{
			MSG_WriteChar(&g_psv.datagram, clamp((int)(dir[i] * 16.0f), -128, 127));
		}

		MSG_WriteByte(&g_psv.datagram, count);
		MSG_WriteByte(&g_psv.datagram, color);
	}
}

void SV_StartSound(int recipients, edict_t *entity, int channel, const char *sample, int volume, float attenuation, int fFlags, int pitch)
{
	g_RehldsHookchains.m_SV_StartSound.callChain(SV_StartSound_internal, recipients, entity, channel, sample, volume, attenuation, fFlags, pitch);
}

/* <a6df2> ../engine/sv_main.c:887 */
void EXT_FUNC SV_StartSound_internal(int recipients, edict_t *entity, int channel, const char *sample, int volume, float attenuation, int fFlags, int pitch)
{
	vec3_t origin;

	for (int i = 0; i < 3; i++)
	{
		origin[i] = (entity->v.maxs[i] + entity->v.mins[i]) * 0.5f + entity->v.origin[i];
	}

	if (!SV_BuildSoundMsg(entity, channel, sample, volume, attenuation, fFlags, pitch, origin, &g_psv.multicast))
	{
		return;
	}

	int flags = 0;
	if (recipients == 1)
	{
		flags = MSG_FL_ONE;
	}

	qboolean sendPAS = channel != CHAN_STATIC && !(fFlags & SND_FL_STOP);
	if (sendPAS)
	{
		SV_Multicast(entity, origin, flags | MSG_FL_PAS, FALSE);
	}
	else
	{
		if (Host_IsSinglePlayerGame())
			SV_Multicast(entity, origin, flags | MSG_FL_BROADCAST, FALSE);
		else
			SV_Multicast(entity, origin, flags | MSG_FL_BROADCAST, TRUE);
	}
}

/* <a614c> ../engine/sv_main.c:942 */
qboolean SV_BuildSoundMsg(edict_t *entity, int channel, const char *sample, int volume, float attenuation, int fFlags, int pitch, const float *origin, sizebuf_t *buffer)
{
	int sound_num;
	int field_mask;

	if (volume < 0 || volume > 255)
	{
		Con_Printf(__FUNCTION__ ": volume = %i", volume);
		volume = (volume < 0) ? 0 : 255;
	}
	if (attenuation < 0.0f || attenuation > 4.0f)
	{
		Con_Printf(__FUNCTION__ ": attenuation = %f", attenuation);
		attenuation = (attenuation < 0.0f) ? 0.0f : 4.0f;
	}
	if (channel < 0 || channel > 7)
	{
		Con_Printf(__FUNCTION__ ": channel = %i", channel);
		channel = (channel < 0) ? CHAN_AUTO : CHAN_NETWORKVOICE_BASE;
	}
	if (pitch < 0 || pitch > 255)
	{
		Con_Printf(__FUNCTION__ ": pitch = %i", pitch);
		pitch = (pitch < 0) ? 0 : 255;
	}

	field_mask = fFlags;

	if (*sample == '!')
	{
		field_mask |= SND_FL_SENTENCE;
		sound_num = Q_atoi(sample + 1);
		if (sound_num >= CVOXFILESENTENCEMAX)
		{
			Con_Printf(__FUNCTION__ ": invalid sentence number: %s", sample + 1);
			return FALSE;
		}
	}
	else if (*sample == '#')
	{
		field_mask |= SND_FL_SENTENCE;
		sound_num = Q_atoi(sample + 1) + CVOXFILESENTENCEMAX;
	}
	else
	{
		sound_num = SV_LookupSoundIndex(sample);
		if (!sound_num || !g_psv.sound_precache[sound_num])
		{
			Con_Printf(__FUNCTION__ ": %s not precached (%d)\n", sample, sound_num);
			return FALSE;
		}
	}

	int ent = NUM_FOR_EDICT(entity);

	if (volume != DEFAULT_SOUND_PACKET_VOLUME)
		field_mask |= SND_FL_VOLUME;
	if (attenuation != DEFAULT_SOUND_PACKET_ATTENUATION)
		field_mask |= SND_FL_ATTENUATION;
	if (pitch != DEFAULT_SOUND_PACKET_PITCH)
		field_mask |= SND_FL_PITCH;
	if (sound_num > 255)
		field_mask |= SND_FL_LARGE_INDEX;

	MSG_WriteByte(buffer, svc_sound);
	MSG_StartBitWriting(buffer);
	MSG_WriteBits(field_mask, 9);
	if (field_mask & SND_FL_VOLUME)
		MSG_WriteBits(volume, 8);
	if (field_mask & SND_FL_ATTENUATION)
		MSG_WriteBits((uint32)(attenuation * 64.0f), 8);
	MSG_WriteBits(channel, 3);
	MSG_WriteBits(ent, 11);
	MSG_WriteBits(sound_num, (field_mask & SND_FL_LARGE_INDEX) ? 16 : 8);
	MSG_WriteBitVec3Coord(origin);
	if (field_mask & SND_FL_PITCH)
		MSG_WriteBits(pitch, 8);
	MSG_EndBitWriting(buffer);

	return TRUE;
}

/* <a622a> ../engine/sv_main.c:1073 */
int SV_HashString(const char *string, int iBounds)
{
	unsigned int hash = 0;
	const char *cc = string;

	while (*cc)
	{
		hash = tolower(*cc) + 2 * hash;
		++cc;
	}
	return hash % iBounds;
}

/* <a6ad2> ../engine/sv_main.c:1087 */
int EXT_FUNC SV_LookupSoundIndex(const char *sample)
{
	int index;

	if (!g_psv.sound_precache_hashedlookup_built)
	{
		if (g_psv.state == ss_loading)
		{
			for (index = 1; index < HL_SOUND_MAX && g_psv.sound_precache[index]; index++) // TODO: why from 1?
			{
				if (!Q_stricmp(sample, g_psv.sound_precache[index]))
					return index;
			}
			return 0;
		}
		SV_BuildHashedSoundLookupTable();
	}

	int starting_index = SV_HashString(sample, 1023);
	index = starting_index;
	while (g_psv.sound_precache_hashedlookup[index])
	{
		if (!Q_stricmp(sample, g_psv.sound_precache[g_psv.sound_precache_hashedlookup[index]]))
			return g_psv.sound_precache_hashedlookup[index];

		index++;
		if (index >= HL_SOUND_HASHLOOKUP_SIZE)
			index = 0;
		if (index == starting_index)
			return 0;
	}
	return 0;
}

/* <a6a3b> ../engine/sv_main.c:1136 */
void SV_BuildHashedSoundLookupTable(void)
{
	Q_memset(g_psv.sound_precache_hashedlookup, 0, sizeof(g_psv.sound_precache_hashedlookup));

	for (int sound_num = 0; sound_num < HL_SOUND_MAX; sound_num++)
	{
		if (!g_psv.sound_precache[sound_num])
			break;

		SV_AddSampleToHashedLookupTable(g_psv.sound_precache[sound_num], sound_num);
	}

	g_psv.sound_precache_hashedlookup_built = TRUE;
}

/* <a6bd9> ../engine/sv_main.c:1151 */
void SV_AddSampleToHashedLookupTable(const char *pszSample, int iSampleIndex)
{
	int starting_index = SV_HashString(pszSample, HL_SOUND_HASHLOOKUP_SIZE);
	int index = starting_index;
	while (g_psv.sound_precache_hashedlookup[index])
	{
		index++;
		hashstrings_collisions++;

		if (index >= HL_SOUND_HASHLOOKUP_SIZE)
			index = 0;

		if (index == starting_index)
			Sys_Error(__FUNCTION__ ": NO FREE SLOTS IN SOUND LOOKUP TABLE");
	}

	g_psv.sound_precache_hashedlookup[index] = iSampleIndex;
}

/* <a6c7c> ../engine/sv_main.c:1180 */
qboolean SV_ValidClientMulticast(client_t *client, int soundLeaf, int to)
{
	if (Host_IsSinglePlayerGame() || client->proxy)
	{
		return TRUE;
	}

	int destination = to & ~(MSG_FL_ONE);
	if (destination == MSG_FL_BROADCAST)
	{
		return TRUE;
	}

	unsigned char* mask;
	if (destination == MSG_FL_PVS)
	{
		mask = CM_LeafPVS(soundLeaf);
	}
	else
	{
		if (destination == MSG_FL_PAS)
		{
			mask = CM_LeafPAS(soundLeaf);
		}
		else
		{
			Con_Printf("MULTICAST: Error %d!\n", to);
			return FALSE;
		}
	}

	if (!mask)
	{
		return TRUE;
	}

	int bitNumber = SV_PointLeafnum(client->edict->v.origin);
	if (mask[(bitNumber - 1) >> 3] & (1 << ((bitNumber - 1) & 7)))
	{
		return TRUE;
	}

	return FALSE;
}

/* <a6d27> ../engine/sv_main.c:1240 */
void SV_Multicast(edict_t *ent, vec_t *origin, int to, qboolean reliable)
{
	client_t *save = host_client;
	int leafnum = SV_PointLeafnum(origin);
	if (ent && !(host_client && host_client->edict == ent))
	{
		for (int i = 0; i < g_psvs.maxclients; i++)
		{
			if (g_psvs.clients[i].edict == ent)
			{
				host_client = &g_psvs.clients[i];
				break;
			}
		}
	}

	for (int i = 0; i < g_psvs.maxclients; i++)
	{
		client_t *client = &g_psvs.clients[i];
		if (!client->active)
			continue;

		if ((to & MSG_FL_ONE) && client == host_client)
			continue;

		if (ent && ent->v.groupinfo != 0 && client->edict->v.groupinfo != 0)
		{
			if (g_groupop)
			{
				if (g_groupop == GROUP_OP_NAND && (ent->v.groupinfo & client->edict->v.groupinfo))
					continue;
			}
			else
			{
				if (!(ent->v.groupinfo & client->edict->v.groupinfo))
					continue;
			}
		}

		if (SV_ValidClientMulticast(client, leafnum, to))
		{
			sizebuf_t *pBuffer = &client->netchan.message;
			if (!reliable)
				pBuffer = &client->datagram;

#ifdef REHLDS_FIXES
			if (pBuffer->cursize + g_psv.multicast.cursize <= pBuffer->maxsize)	// TODO: Should it be <= ? I think so.
#else // REHLDS_FIXES
			if (pBuffer->cursize + g_psv.multicast.cursize < pBuffer->maxsize)
#endif // REHLDS_FIXES
				SZ_Write(pBuffer, g_psv.multicast.data, g_psv.multicast.cursize);
		}
		else
		{
			gPacketSuppressed += g_psv.multicast.cursize;
		}
	}

	SZ_Clear(&g_psv.multicast);
	host_client = save;
}

/* <a6f4f> ../engine/sv_main.c:1328 */
void EXT_FUNC SV_WriteMovevarsToClient(sizebuf_t *message)
{
	MSG_WriteByte(message, svc_newmovevars);
	MSG_WriteFloat(message, movevars.gravity);
	MSG_WriteFloat(message, movevars.stopspeed);
	MSG_WriteFloat(message, movevars.maxspeed);
	MSG_WriteFloat(message, movevars.spectatormaxspeed);
	MSG_WriteFloat(message, movevars.accelerate);
	MSG_WriteFloat(message, movevars.airaccelerate);
	MSG_WriteFloat(message, movevars.wateraccelerate);
	MSG_WriteFloat(message, movevars.friction);
	MSG_WriteFloat(message, movevars.edgefriction);
	MSG_WriteFloat(message, movevars.waterfriction);
	MSG_WriteFloat(message, movevars.entgravity);
	MSG_WriteFloat(message, movevars.bounce);
	MSG_WriteFloat(message, movevars.stepsize);
	MSG_WriteFloat(message, movevars.maxvelocity);
	MSG_WriteFloat(message, movevars.zmax);
	MSG_WriteFloat(message, movevars.waveHeight);
	MSG_WriteByte(message, movevars.footsteps != 0);
	MSG_WriteFloat(message, movevars.rollangle);
	MSG_WriteFloat(message, movevars.rollspeed);
	MSG_WriteFloat(message, movevars.skycolor_r);
	MSG_WriteFloat(message, movevars.skycolor_g);
	MSG_WriteFloat(message, movevars.skycolor_b);
	MSG_WriteFloat(message, movevars.skyvec_x);
	MSG_WriteFloat(message, movevars.skyvec_y);
	MSG_WriteFloat(message, movevars.skyvec_z);
	MSG_WriteString(message, movevars.skyName);
}

/* <a6f79> ../engine/sv_main.c:1365 */
void EXT_FUNC SV_WriteDeltaDescriptionsToClient(sizebuf_t *msg)
{
	int i, c;

	delta_description_t nulldesc;
	Q_memset(&nulldesc, 0, sizeof(nulldesc));

	for (delta_info_t *p = g_sv_delta; p != NULL; p = p->next)
	{
		MSG_WriteByte(msg, svc_deltadescription);
		MSG_WriteString(msg, p->name);
		MSG_StartBitWriting(msg);

		c = p->delta->fieldCount;

		MSG_WriteBits(c, 16);

		for (i = 0; i < c; i++)
		{
			DELTA_WriteDelta((byte *)&nulldesc, (byte *)&p->delta->pdd[i], TRUE, (delta_t *)&g_MetaDelta, NULL);
		}

		MSG_EndBitWriting(msg);
	}
}

/* <a7002> ../engine/sv_main.c:1407 */
void EXT_FUNC SV_SetMoveVars(void)
{
	movevars.entgravity			= 1.0f;
	movevars.gravity			= sv_gravity.value;
	movevars.stopspeed			= sv_stopspeed.value;
	movevars.maxspeed			= sv_maxspeed.value;
	movevars.spectatormaxspeed	= sv_spectatormaxspeed.value;
	movevars.accelerate			= sv_accelerate.value;
	movevars.airaccelerate		= sv_airaccelerate.value;
	movevars.wateraccelerate	= sv_wateraccelerate.value;
	movevars.friction			= sv_friction.value;
	movevars.edgefriction		= sv_edgefriction.value;
	movevars.waterfriction		= sv_waterfriction.value;
	movevars.bounce				= sv_bounce.value;
	movevars.stepsize			= sv_stepsize.value;
	movevars.maxvelocity		= sv_maxvelocity.value;
	movevars.zmax				= sv_zmax.value;
	movevars.waveHeight			= sv_wateramp.value;
	movevars.footsteps			= sv_footsteps.value;
	movevars.rollangle			= sv_rollangle.value;
	movevars.rollspeed			= sv_rollspeed.value;
	movevars.skycolor_r			= sv_skycolor_r.value;
	movevars.skycolor_g			= sv_skycolor_g.value;
	movevars.skycolor_b			= sv_skycolor_b.value;
	movevars.skyvec_x			= sv_skyvec_x.value;
	movevars.skyvec_y			= sv_skyvec_y.value;
	movevars.skyvec_z			= sv_skyvec_z.value;

	Q_strncpy(movevars.skyName, sv_skyname.string, sizeof(movevars.skyName) - 1);
	movevars.skyName[sizeof(movevars.skyName) - 1] = 0;
}

/* <a7018> ../engine/sv_main.c:1446 */
void SV_QueryMovevarsChanged(void)
{
	if (movevars.entgravity				!= 1.0f
		|| sv_maxspeed.value			!= movevars.maxspeed
		|| sv_gravity.value				!= movevars.gravity
		|| sv_stopspeed.value			!= movevars.stopspeed
		|| sv_spectatormaxspeed.value	!= movevars.spectatormaxspeed
		|| sv_accelerate.value			!= movevars.accelerate
		|| sv_airaccelerate.value		!= movevars.airaccelerate
		|| sv_wateraccelerate.value		!= movevars.wateraccelerate
		|| sv_friction.value			!= movevars.friction
		|| sv_edgefriction.value		!= movevars.edgefriction
		|| sv_waterfriction.value		!= movevars.waterfriction
		|| sv_bounce.value				!= movevars.bounce
		|| sv_stepsize.value			!= movevars.stepsize
		|| sv_maxvelocity.value			!= movevars.maxvelocity
		|| sv_zmax.value				!= movevars.zmax
		|| sv_wateramp.value			!= movevars.waveHeight
		|| sv_footsteps.value			!= movevars.footsteps
		|| sv_rollangle.value			!= movevars.rollangle
		|| sv_rollspeed.value			!= movevars.rollspeed
		|| sv_skycolor_r.value			!= movevars.skycolor_r
		|| sv_skycolor_g.value			!= movevars.skycolor_g
		|| sv_skycolor_b.value			!= movevars.skycolor_b
		|| sv_skyvec_x.value			!= movevars.skyvec_x
		|| sv_skyvec_y.value			!= movevars.skyvec_y
		|| sv_skyvec_z.value			!= movevars.skyvec_z
		|| Q_strcmp(sv_skyname.string, movevars.skyName))
	{
		SV_SetMoveVars();

		client_t *cl = g_psvs.clients;
		for (int i = 0; i < g_psvs.maxclients; i++, cl++)
		{
			if (!cl->fakeclient && (cl->active || cl->spawned || cl->connected))
				SV_WriteMovevarsToClient(&cl->netchan.message);
		}
	}
}

void EXT_FUNC SV_SendServerinfo_mod(sizebuf_t *msg, IGameClient* cl)
{
	SV_SendServerinfo_internal(msg, cl->GetClient());
}

void SV_SendServerinfo(sizebuf_t *msg, client_t *client)
{
	g_RehldsHookchains.m_SV_SendServerinfo.callChain(SV_SendServerinfo_mod, msg, GetRehldsApiClient(client));
}

/* <a7050> ../engine/sv_main.c:1499 */
void SV_SendServerinfo_internal(sizebuf_t *msg, client_t *client)
{
	char message[2048];

	if (developer.value != 0.0f || g_psvs.maxclients > 1)
	{
		MSG_WriteByte(msg, svc_print);
		Q_snprintf(message, ARRAYSIZE(message), "%c\nBUILD %d SERVER (%i CRC)\nServer # %i\n", 2, build_number(), 0, g_psvs.spawncount);
		MSG_WriteString(msg, message);
	}

	MSG_WriteByte(msg, svc_serverinfo);
	MSG_WriteLong(msg, PROTOCOL_VERSION);
	MSG_WriteLong(msg, g_psvs.spawncount);

	int playernum = NUM_FOR_EDICT(client->edict) - 1;
	int mungebuffer = g_psv.worldmapCRC;

	COM_Munge3((byte *)&mungebuffer, sizeof(mungebuffer), (-1 - playernum) & 0xFF);
	MSG_WriteLong(msg, mungebuffer);

	MSG_WriteBuf(msg, sizeof(g_psv.clientdllmd5), g_psv.clientdllmd5);
	MSG_WriteByte(msg, g_psvs.maxclients);
	MSG_WriteByte(msg, playernum);

	if (coop.value == 0.0f && deathmatch.value != 0.0f)
		MSG_WriteByte(msg, 1);
	else
		MSG_WriteByte(msg, 0);

	COM_FileBase(com_gamedir, message);
	MSG_WriteString(msg, message);
	MSG_WriteString(msg, Cvar_VariableString("hostname"));
	MSG_WriteString(msg, g_psv.modelname);

	int len = 0;
	unsigned char *mapcyclelist = COM_LoadFileForMe(mapcyclefile.string, &len);
#ifdef REHLDS_FIXES
	if (mapcyclelist && len)
	{
		MSG_WriteString(msg, (const char *)mapcyclelist);
	}
	else
	{
		MSG_WriteString(msg, "mapcycle failure");
	}
	// FIXED: Mem leak.
	if (mapcyclelist)
	{
		COM_FreeFile(mapcyclelist);
	}
#else // REHLDS_FIXES
	if (mapcyclelist && len)
	{
		MSG_WriteString(msg, (const char *)mapcyclelist);
		COM_FreeFile(mapcyclelist);
	}
	else
	{
		MSG_WriteString(msg, "mapcycle failure");
	}
#endif // REHLDS_FIXES
	MSG_WriteByte(msg, 0);

	MSG_WriteByte(msg, svc_sendextrainfo);
	MSG_WriteString(msg, com_clientfallback);
	MSG_WriteByte(msg, allow_cheats);

	SV_WriteDeltaDescriptionsToClient(msg);
	SV_SetMoveVars();
	SV_WriteMovevarsToClient(msg);

	MSG_WriteByte(msg, svc_cdtrack);
	MSG_WriteByte(msg, gGlobalVariables.cdAudioTrack);
	MSG_WriteByte(msg, gGlobalVariables.cdAudioTrack);
	MSG_WriteByte(msg, svc_setview);
	MSG_WriteShort(msg, playernum + 1);

	client->spawned = FALSE;
	client->connected = TRUE;
	client->fully_connected = FALSE;
}

/* <a70eb> ../engine/sv_main.c:1600 */
void SV_SendResources(sizebuf_t *msg)
{
	unsigned char nullbuffer[32];
	Q_memset(nullbuffer, 0, sizeof(nullbuffer));

	MSG_WriteByte(msg, svc_resourcerequest);
	MSG_WriteLong(msg, g_psvs.spawncount);
	MSG_WriteLong(msg, 0);

	if (sv_downloadurl.string && sv_downloadurl.string[0] != 0 && Q_strlen(sv_downloadurl.string) < 129)
	{
		MSG_WriteByte(msg, svc_resourcelocation);
		MSG_WriteString(msg, sv_downloadurl.string);
	}

	MSG_WriteByte(msg, svc_resourcelist);
	MSG_StartBitWriting(msg);
	MSG_WriteBits(g_psv.num_resources, 12);

	resource_t *r = g_psv.resourcelist;
	for (int i = 0; i < g_psv.num_resources; i++, r++)
	{
		MSG_WriteBits(r->type, t_generic);
		MSG_WriteBitString(r->szFileName);
		MSG_WriteBits(r->nIndex, 12);
		MSG_WriteBits(r->nDownloadSize, 24);
		MSG_WriteBits(r->ucFlags & (RES_WASMISSING | RES_FATALIFMISSING), 3);

		if (r->ucFlags & RES_CUSTOM)
		{
			MSG_WriteBitData(r->rgucMD5_hash, sizeof(r->rgucMD5_hash));
		}

		if (!Q_memcmp(r->rguc_reserved, nullbuffer, sizeof(r->rguc_reserved)))
		{
			MSG_WriteBits(0, 1);
		}
		else
		{
			MSG_WriteBits(1, 1);
			MSG_WriteBitData(r->rguc_reserved, sizeof(r->rguc_reserved));
		}
	}

	SV_SendConsistencyList(msg);
	MSG_EndBitWriting(msg);
}

/* <a717e> ../engine/sv_main.c:1659 */
void SV_WriteClientdataToMessage(client_t *client, sizebuf_t *msg)
{
	edict_t *ent = client->edict;
	client_frame_t *frame = &client->frames[SV_UPDATE_MASK & client->netchan.outgoing_sequence];
	int bits = (SV_UPDATE_MASK & host_client->delta_sequence);

	frame->senttime = realtime;
	frame->ping_time = -1.0f;

	if (client->chokecount)
	{
		MSG_WriteByte(msg, svc_choke);
		client->chokecount = 0;
	}

	if (ent->v.fixangle)
	{
		if (ent->v.fixangle == 2)
		{
			MSG_WriteByte(msg, svc_addangle);
			MSG_WriteHiresAngle(msg, ent->v.avelocity[1]);
			ent->v.avelocity[1] = 0;
		}
		else
		{
			MSG_WriteByte(msg, svc_setangle);
			MSG_WriteHiresAngle(msg, ent->v.angles[0]);
			MSG_WriteHiresAngle(msg, ent->v.angles[1]);
			MSG_WriteHiresAngle(msg, ent->v.angles[2]);
		}
		ent->v.fixangle = 0;
	}

	Q_memset(&frame->clientdata, 0, sizeof(frame->clientdata));
	gEntityInterface.pfnUpdateClientData(ent, host_client->lw != 0, &frame->clientdata);

	MSG_WriteByte(msg, svc_clientdata);

	if (client->proxy)
		return;

	MSG_StartBitWriting(msg);

	clientdata_t baseline;
	Q_memset(&baseline, 0, sizeof(baseline));

	clientdata_t *from = &baseline;
	clientdata_t *to = &frame->clientdata;

	if (host_client->delta_sequence == -1)
	{
		MSG_WriteBits(0, 1);
	}
	else
	{
		MSG_WriteBits(1, 1);
		MSG_WriteBits(host_client->delta_sequence, 8);
		from = &host_client->frames[bits].clientdata;
	}

	DELTA_WriteDelta((byte *)from, (byte *)to, TRUE, (delta_t *)g_pclientdelta, NULL);

	if (host_client->lw && gEntityInterface.pfnGetWeaponData(host_client->edict, frame->weapondata))
	{
		weapon_data_t wbaseline;
		Q_memset(&wbaseline, 0, sizeof(wbaseline));

		weapon_data_t *fdata = NULL;
		weapon_data_t *tdata = frame->weapondata;

		for (int i = 0; i < 64; i++, tdata++)
		{
			if (host_client->delta_sequence == -1)
				fdata = &wbaseline;
			else
				fdata = &host_client->frames[bits].weapondata[i];

			if (DELTA_CheckDelta((byte *)fdata, (byte *)tdata, g_pweapondelta))
			{
				MSG_WriteBits(1, 1);
				MSG_WriteBits(i, 6);

#if defined (REHLDS_OPT_PEDANTIC) || defined (REHLDS_FIXES)
				// all calculations are already done
				_DELTA_WriteDelta((byte *)fdata, (byte *)tdata, TRUE, g_pweapondelta, NULL, TRUE);
#else
				DELTA_WriteDelta((byte *)fdata, (byte *)tdata, TRUE, g_pweapondelta, NULL);
#endif
			}
		}
	}

	MSG_WriteBits(0, 1);
	MSG_EndBitWriting(msg);
}

/* <a889e> ../engine/sv_main.c:1789 */
void SV_WriteSpawn(sizebuf_t *msg)
{
	int i = 0;
	client_t *client = g_psvs.clients;

#ifdef REHLDS_FIXES
	// do it before PutInServer to allow mods send messages from forward
	SZ_Clear( &host_client->netchan.message );
	SZ_Clear( &host_client->datagram );
#endif // REHLDS_FIXES

	if (g_psv.loadgame)
	{
		if (host_client->proxy)
		{
			Con_Printf("ERROR! Spectator mode doesn't work with saved game.\n");
			return;
		}
		g_psv.paused = FALSE;
	}
	else
	{
		g_psv.state = ss_loading;
		ReleaseEntityDLLFields(sv_player);

		Q_memset(&sv_player->v, 0, sizeof(sv_player->v));
		InitEntityDLLFields(sv_player);

		sv_player->v.colormap = NUM_FOR_EDICT(sv_player);
		sv_player->v.netname = host_client->name - pr_strings;

		if (host_client->proxy)
			sv_player->v.flags |= FL_PROXY;

		gGlobalVariables.time = g_psv.time;
		gEntityInterface.pfnClientPutInServer(sv_player);
		g_psv.state = ss_active;
	}

#ifndef REHLDS_FIXES
	SZ_Clear(&host_client->netchan.message);
	SZ_Clear(&host_client->datagram);
#endif // REHLDS_FIXES

	MSG_WriteByte(msg, svc_time);
	MSG_WriteFloat(msg, g_psv.time);

	host_client->sendinfo = TRUE;

	for (i = 0; i < g_psvs.maxclients; i++, client++)
	{
		if (client == host_client || client->active || client->connected || client->spawned)
			SV_FullClientUpdate(client, msg);
	}

	for (i = 0; i < ARRAYSIZE( g_psv.lightstyles ); i++)
	{
		MSG_WriteByte(msg, svc_lightstyle);
		MSG_WriteByte(msg, i);
		MSG_WriteString(msg, g_psv.lightstyles[i]);
	}

	if (!host_client->proxy)
	{
		MSG_WriteByte(msg, svc_setangle);
		MSG_WriteHiresAngle(msg, sv_player->v.v_angle[0]);
		MSG_WriteHiresAngle(msg, sv_player->v.v_angle[1]);
		MSG_WriteHiresAngle(msg, 0.0f);
		SV_WriteClientdataToMessage(host_client, msg);
#ifndef SWDS
		if (g_psv.loadgame)
		{
			// TODO: add client code
		}
#endif // SWDS
	}

	MSG_WriteByte(msg, svc_signonnum);
	MSG_WriteByte(msg, 1);

	host_client->connecttime		= 0.0;
	host_client->ignorecmdtime		= 0.0;
	host_client->cmdtime			= 0.0;
	host_client->active				= TRUE;
	host_client->spawned			= TRUE;
	host_client->connected			= TRUE;
	host_client->fully_connected	= FALSE;

	NotifyDedicatedServerUI("UpdatePlayers");
}

/* <a7277> ../engine/sv_main.c:1920 */
void EXT_FUNC SV_SendUserReg(sizebuf_t *msg)
{
	for (UserMsg *pMsg = sv_gpNewUserMsgs; pMsg; pMsg = pMsg->next)
	{
		MSG_WriteByte(msg, svc_newusermsg);
		MSG_WriteByte(msg, pMsg->iMsg);
		MSG_WriteByte(msg, pMsg->iSize);
		MSG_WriteLong(msg, *(int *)&pMsg->szName[0]);
		MSG_WriteLong(msg, *(int *)&pMsg->szName[4]);
		MSG_WriteLong(msg, *(int *)&pMsg->szName[8]);
		MSG_WriteLong(msg, *(int *)&pMsg->szName[12]);
	}
}

/* <a87be> ../engine/sv_main.c:1953 */
void SV_New_f(void)
{
	int i;
	client_t *client;
	unsigned char data[NET_MAX_PAYLOAD];
	sizebuf_t msg;
	edict_t *ent;
	char szRejectReason[128];
	char szAddress[256];
	char szName[64];

	Q_memset(&msg, 0, sizeof(msg));
	msg.buffername = "New Connection";
	msg.data = data;
	msg.maxsize = sizeof(data);
	msg.cursize = 0;
	msg.flags = SIZEBUF_CHECK_OVERFLOW;

	// Not valid on the client
	if (cmd_source == src_command)
	{
		return;
	}

	if (!host_client->active && host_client->spawned)
	{
		return;
	}

	ent = host_client->edict;

	host_client->connected = TRUE;
	host_client->connection_started = realtime;
	host_client->m_sendrescount = 0;

	SZ_Clear(&host_client->netchan.message);
	SZ_Clear(&host_client->datagram);

	Netchan_Clear(&host_client->netchan);

	SV_SendServerinfo(&msg, host_client);

	if (sv_gpUserMsgs)
	{
		UserMsg *pTemp = sv_gpNewUserMsgs;
		sv_gpNewUserMsgs = sv_gpUserMsgs;
		SV_SendUserReg(&msg);
		sv_gpNewUserMsgs = pTemp;
	}
	host_client->hasusrmsgs = TRUE;

	// TODO: set userinfo to be sent?
	//host_client->sendinfo = true;

	// If the client was connected, tell the game dll to disconnect him/her.
	if ((host_client->active || host_client->spawned) && ent)
	{
		gEntityInterface.pfnClientDisconnect(ent);
	}

	Q_snprintf(szName, sizeof(szName), host_client->name);
	Q_snprintf(szAddress, sizeof(szAddress), NET_AdrToString(host_client->netchan.remote_address));
	Q_snprintf(szRejectReason, sizeof(szRejectReason), "Connection rejected by game\n");

	// Allow the game dll to reject this client.
	if (!gEntityInterface.pfnClientConnect(ent, szName, szAddress, szRejectReason))
	{
		// Reject the connection and drop the client.
		MSG_WriteByte(&host_client->netchan.message, svc_stufftext);
		MSG_WriteString(&host_client->netchan.message, va("echo %s\n", szRejectReason));
		SV_DropClient(host_client, FALSE, "Server refused connection because:  %s", szRejectReason);
		return;
	}

	MSG_WriteByte(&msg, svc_stufftext);
	MSG_WriteString(&msg, va("fullserverinfo \"%s\"\n", Info_Serverinfo()));
	for (i = 0, client = g_psvs.clients; i < g_psvs.maxclients; i++, client++)
	{
		if (client == host_client || client->active || client->connected || client->spawned)
			SV_FullClientUpdate(client, &msg);
	}
	Netchan_CreateFragments(TRUE, &host_client->netchan, &msg);
	Netchan_FragSend(&host_client->netchan);
}

/* <a7132> ../engine/sv_main.c:2057 */
void SV_SendRes_f(void)
{
	unsigned char data[NET_MAX_PAYLOAD];
	sizebuf_t msg;

	Q_memset(&msg, 0, sizeof(msg));
	msg.buffername = "SendResources";
	msg.data = data;
	msg.maxsize = sizeof(data);
	msg.cursize = 0;
	msg.flags = SIZEBUF_CHECK_OVERFLOW;

	if (cmd_source != src_command && (!host_client->spawned || host_client->active))
	{
		if (g_psvs.maxclients <= 1 || host_client->m_sendrescount <= 1)
		{
			host_client->m_sendrescount++;
			SV_SendResources(&msg);
			Netchan_CreateFragments(true, &host_client->netchan, &msg);
			Netchan_FragSend(&host_client->netchan);
		}
	}
}

/* <a8922> ../engine/sv_main.c:2096 */
void SV_Spawn_f(void)
{
	unsigned char data[NET_MAX_PAYLOAD];
	sizebuf_t msg;

	Q_memset(&msg, 0, sizeof(msg));
	msg.buffername = "Spawning";
	msg.data = data;
	msg.maxsize = sizeof(data);
	msg.cursize = 0;
	msg.flags = SIZEBUF_CHECK_OVERFLOW;

	if (Cmd_Argc() != 3)
	{
		Con_Printf("spawn is not valid\n");
		return;
	}

	host_client->crcValue = Q_atoi(Cmd_Argv(2));

	COM_UnMunge2((unsigned char *)&host_client->crcValue, 4, (-1 - g_psvs.spawncount) & 0xFF);

	if (cmd_source == src_command)
	{
		Con_Printf("spawn is not valid from the console\n");
		return;
	}

	if (g_pcls.demoplayback || Q_atoi(Cmd_Argv(1)) == g_psvs.spawncount)
	{
		SZ_Write(&msg, g_psv.signon.data, g_psv.signon.cursize);
		SV_WriteSpawn(&msg);
		SV_WriteVoiceCodec(&msg);
		Netchan_CreateFragments(1, &host_client->netchan, &msg);
		Netchan_FragSend(&host_client->netchan);
	}
	else
	{
		SV_New_f();
	}
}

/* <a72d8> ../engine/sv_main.c:2148 */
void SV_CheckUpdateRate(double *rate)
{
	if (*rate == 0.0)
	{
		*rate = 0.05;
		return;
	}

	if (sv_maxupdaterate.value <= 0.001f && sv_maxupdaterate.value != 0.0f)
		Cvar_Set("sv_maxupdaterate", "30.0");
	if (sv_minupdaterate.value <= 0.001f && sv_minupdaterate.value != 0.0f)
		Cvar_Set("sv_minupdaterate", "1.0");

	if (sv_maxupdaterate.value != 0.0f)
	{
		if (*rate < 1.0 / sv_maxupdaterate.value)
			*rate = 1.0 / sv_maxupdaterate.value;
	}
	if (sv_minupdaterate.value != 0.0f)
	{
		if (*rate > 1.0 / sv_minupdaterate.value)
			*rate = 1.0 / sv_minupdaterate.value;
	}
}

/* <a7302> ../engine/sv_main.c:2189 */
void EXT_FUNC SV_RejectConnection(netadr_t *adr, char *fmt, ...)
{
	va_list argptr;
	char text[1024];
	va_start(argptr, fmt);
	Q_vsnprintf(text, sizeof(text), fmt, argptr);
	va_end(argptr);

	SZ_Clear(&net_message);
	MSG_WriteLong(&net_message, -1);
	MSG_WriteByte(&net_message, '9');
	MSG_WriteString(&net_message, text);
	NET_SendPacket(NS_SERVER, net_message.cursize, net_message.data, *adr);
	SZ_Clear(&net_message);
}

/* <a735c> ../engine/sv_main.c:2213 */
void SV_RejectConnectionForPassword(netadr_t *adr)
{
	SZ_Clear(&net_message);
	MSG_WriteLong(&net_message, -1);
	MSG_WriteByte(&net_message, '8');
	MSG_WriteString(&net_message, "BADPASSWORD");
	NET_SendPacket(NS_SERVER, net_message.cursize, net_message.data, *adr);
	SZ_Clear(&net_message);
}

/* <a5bce> ../engine/sv_main.c:2230 */
int SV_GetFragmentSize(void *state)
{
	int size = 1024;
	client_t *cl = (client_t *)state;

	if (cl->active && cl->spawned && cl->connected && cl->fully_connected)
	{
		size = 256;
		const char *val = Info_ValueForKey(cl->userinfo, "cl_dlmax");
		if (val[0] != 0)
		{
			size = Q_atoi( val );
			size = clamp(size, 256, 1024);
		}
	}

	return size;
}

/* <ab01b> ../engine/sv_main.c:2266 */
qboolean EXT_FUNC SV_FilterUser(USERID_t *userid)
{
	int j = numuserfilters;
	for (int i = numuserfilters - 1; i >= 0; i--)
	{
		userfilter_t *filter = &userfilters[i];
		if (filter->banEndTime == 0.0f || filter->banEndTime > realtime)
		{
			if (SV_CompareUserID(userid, &filter->userid))
				return (qboolean)sv_filterban.value;
		}
		else
		{
			if (i + 1 < j)
				Q_memmove(filter, &filter[1], sizeof(userfilter_t) * (j - i + 1));

			numuserfilters = --j;
		}
	}

	return sv_filterban.value == 0.0f ? TRUE : FALSE;
}

int SV_CheckProtocol(netadr_t *adr, int nProtocol)
{
	return g_RehldsHookchains.m_SV_CheckProtocol.callChain(SV_CheckProtocol_internal, adr, nProtocol);
}

/* <a7396> ../engine/sv_main.c:2302 */
int EXT_FUNC SV_CheckProtocol_internal(netadr_t *adr, int nProtocol)
{
	if (adr == NULL)
	{
		Sys_Error(__FUNCTION__ ":  Null address\n");
		return FALSE;
	}

	if (nProtocol == PROTOCOL_VERSION)
	{
		return TRUE;
	}

	if (nProtocol < PROTOCOL_VERSION)
	{
		SV_RejectConnection(
			adr,
			"This server is using a newer protocol ( %i ) than your client ( %i ).  You should check for updates to your client.\n",
			PROTOCOL_VERSION,
			nProtocol);
	}
	else
	{
		char *contact = "(no email address specified)";
		if (sv_contact.string[0] != 0)
			contact = sv_contact.string;

		SV_RejectConnection(
			adr,
			"This server is using an older protocol ( %i ) than your client ( %i ).  If you believe this server is outdated, you can contact the server administrator at %s.\n",
			PROTOCOL_VERSION,
			nProtocol,
			contact);
	}

	return FALSE;
}

/* <a5a15> ../engine/sv_main.c:2335 */
typedef struct challenge_s
{
	netadr_t adr;
	int challenge;
	int time;
} challenge_t;

challenge_t g_rg_sv_challenges[MAX_CHALLENGES];
#ifdef REHLDS_OPT_PEDANTIC
int g_oldest_challenge = 0;
#endif

bool EXT_FUNC SV_CheckChallenge_api(const netadr_t &adr, int nChallengeValue) {
	netadr_t localAdr = adr;
	return SV_CheckChallenge(&localAdr, nChallengeValue) != 0;
}

/* <a5c55> ../engine/sv_main.c:2351 */
int SV_CheckChallenge(netadr_t *adr, int nChallengeValue)
{
	if (!adr)
		Sys_Error(__FUNCTION__ ":  Null address\n");

	if (NET_IsLocalAddress(*adr))
		return 1;
	
	for (int i = 0; i < MAX_CHALLENGES; i++)
	{
		if (NET_CompareBaseAdr(net_from, g_rg_sv_challenges[i].adr))
		{
			if (nChallengeValue != g_rg_sv_challenges[i].challenge)
			{
				SV_RejectConnection(adr, "Bad challenge.\n");
				return 0;
			}
			return 1;
		}
	}
	SV_RejectConnection(adr, "No challenge for your address.\n");
	return 0;
}

int SV_CheckIPRestrictions(netadr_t *adr, int nAuthProtocol)
{
	return g_RehldsHookchains.m_SV_CheckIPRestrictions.callChain(SV_CheckIPRestrictions_internal, adr, nAuthProtocol);
}

/* <a743d> ../engine/sv_main.c:2393 */
int EXT_FUNC SV_CheckIPRestrictions_internal(netadr_t *adr, int nAuthProtocol)
{
	if (sv_lan.value || nAuthProtocol != 3)
	{
		if (nAuthProtocol == 2)
			return 1;

		return (sv_lan.value && (NET_CompareClassBAdr(*adr, net_local_adr) || NET_IsReservedAdr(*adr)));
	}
	return 1;
}

/* <a748f> ../engine/sv_main.c:2427 */
int SV_CheckIPConnectionReuse(netadr_t *adr)
{
	int count = 0;

	client_t *cl = g_psvs.clients;
	for (int i = 0; i < g_psvs.maxclients; i++, cl++)
	{
		if (cl->connected && !cl->fully_connected && NET_CompareBaseAdr(cl->netchan.remote_address, *adr))
		{
			count++;
		}
	}

	if (count > 5)
	{
		Log_Printf("Too many connect packets from %s\n", NET_AdrToString(*adr));
		return 0;
	}

	return 1;
}

int SV_FinishCertificateCheck(netadr_t *adr, int nAuthProtocol, char *szRawCertificate, char *userinfo)
{
	return g_RehldsHookchains.m_SV_FinishCertificateCheck.callChain(SV_FinishCertificateCheck_internal, adr, nAuthProtocol, szRawCertificate, userinfo);
}

/* <a74f4> ../engine/sv_main.c:2461 */
int EXT_FUNC SV_FinishCertificateCheck_internal(netadr_t *adr, int nAuthProtocol, char *szRawCertificate, char *userinfo)
{
	if (nAuthProtocol != 2)
	{
		if (Q_stricmp(szRawCertificate, "steam"))
		{
			SV_RejectConnection(adr, "Expecting STEAM authentication USERID ticket!\n");
			return 0;
		}

		return 1;
	}

	if (Q_strlen(szRawCertificate) != 32)
	{
		SV_RejectConnection(adr, "Invalid CD Key.\n");
		return 0;
	}

	if (adr->type == NA_LOOPBACK)
	{
		return 1;
	}

	const char *val = Info_ValueForKey(userinfo, "*hltv");

	if (val[0] == 0 || Q_atoi(val) != 1)
	{
		SV_RejectConnection(adr, "Invalid CD Key.\n");
		return 0;
	}

	return 1;
}

int SV_CheckKeyInfo(netadr_t *adr, char *protinfo, short unsigned int *port, int *pAuthProtocol, char *pszRaw, char *cdkey)
{
	return g_RehldsHookchains.m_SV_CheckKeyInfo.callChain(SV_CheckKeyInfo_internal, adr, protinfo, port, pAuthProtocol, pszRaw, cdkey);
}

/* <a7584> ../engine/sv_main.c:2527 */
int EXT_FUNC SV_CheckKeyInfo_internal(netadr_t *adr, char *protinfo, short unsigned int *port, int *pAuthProtocol, char *pszRaw, char *cdkey)
{
	const char *s = Info_ValueForKey(protinfo, "prot");
	int nAuthProtocol = Q_atoi(s);

	if (nAuthProtocol <= 0 || nAuthProtocol > 4)
	{
		SV_RejectConnection(adr, "Invalid connection.\n");
		return 0;
	}

	s = Info_ValueForKey(protinfo, "raw");

	if (s[0] == 0 || (nAuthProtocol == 2 && Q_strlen(s) != 32))
	{
		SV_RejectConnection(adr, "Invalid authentication certificate length.\n");
		return 0;
	}

	Q_strcpy(pszRaw, s);

	if (nAuthProtocol != 2)
	{
		s = Info_ValueForKey(protinfo, "cdkey");

		if (Q_strlen(s) != 32)
		{
			SV_RejectConnection(adr, "Invalid hashed CD key.\n");
			return 0;
		}
	}

	Q_snprintf(cdkey, 64, "%s", s);
	*pAuthProtocol = nAuthProtocol;
	*port = Q_atoi("27005");

	return 1;
}

/* <aadf7> ../engine/sv_main.c:2590 */
int SV_CheckForDuplicateSteamID(client_t *client)
{
	if (sv_lan.value != 0.0f)
		return -1;

	for (int i = 0; i < g_psvs.maxclients; i++)
	{
		client_t *cl = &g_psvs.clients[i];

		if (!cl->connected || cl->fakeclient || cl == client)
			continue;

		if (cl->network_userid.idtype != AUTH_IDTYPE_STEAM && cl->network_userid.idtype != AUTH_IDTYPE_VALVE)
			continue;

		if (SV_CompareUserID(&client->network_userid, &cl->network_userid))
			return i;
	}

	return -1;
}

/* <a761b> ../engine/sv_main.c:2639 */
int SV_CheckForDuplicateNames(char *userinfo, qboolean bIsReconnecting, int nExcludeSlot)
{
	const char *val;
	int i;
	client_t *client;
	int dupc = 0;
	char rawname[MAX_NAME];
	char newname[MAX_NAME];
	int changed = FALSE;

	val = Info_ValueForKey(userinfo, "name");
	Q_strncpy(rawname, val, MAX_NAME - 1);

	while (true)
	{
		for (i = 0, client = g_psvs.clients; i < g_psvs.maxclients; i++, client++)
		{
			if (client->connected && !(i == nExcludeSlot && bIsReconnecting) && !Q_stricmp(client->name, val))
				break;
		}

		// no duplicates for current name
		if (i == g_psvs.maxclients)
			return changed;

		Q_snprintf(newname, sizeof(newname), "(%d)%-0.*s", ++dupc, 28, rawname);
#ifdef REHLDS_FIXES
		// Fix possibly incorrectly cut UTF8 chars
		if (!Q_UnicodeValidate(newname))
		{
			Q_UnicodeRepair(newname);
		}
#endif // REHLDS_FIXES
		Info_SetValueForKey(userinfo, "name", newname, MAX_INFO_STRING);
		val = Info_ValueForKey(userinfo, "name");
		changed = TRUE;
	}

	return changed;
}

/* <a76d2> ../engine/sv_main.c:2710 */
int SV_CheckUserInfo(netadr_t *adr, char *userinfo, qboolean bIsReconnecting, int nReconnectSlot, char *name)
{
	const char *s;
	char *pChar;
	char newname[MAX_NAME];
	int proxies;
	int i;

	if (!NET_IsLocalAddress(*adr))
	{
		if (sv_password.string[0] != 0 && Q_stricmp(sv_password.string, "none") && Q_strcmp(sv_password.string, Info_ValueForKey(userinfo, "password")))
		{
			Con_Printf("%s:  password failed\n", NET_AdrToString(*adr));
			SV_RejectConnectionForPassword(adr);

			return 0;
		}
	}

	i = strlen(userinfo);
	if (i <= 4 || strstr(userinfo, "\\\\") || userinfo[i - 1] == '\\')
	{
		SV_RejectConnection(adr, "Unknown HLTV client type.\n");

		return 0;
	}

	Info_RemoveKey(userinfo, "password");

	s = Info_ValueForKey(userinfo, "name");
	Q_strncpy(newname, s, sizeof(newname) - 1);
	newname[sizeof(newname) - 1] = '\0';

	for (pChar = newname; *pChar; pChar++)
	{
		if (*pChar == '%' || *pChar == '&')
			*pChar = ' ';
	}

#ifdef REHLDS_FIXES
	Q_strcpy(name, newname);
	Q_StripUnprintableAndSpace(name);
#else // REHLDS_FIXES
	TrimSpace(newname, name);
#endif // REHLDS_FIXES

	if (!Q_UnicodeValidate(name))
	{
		Q_UnicodeRepair(name);
	}

	for (pChar = newname; *pChar == '#'; pChar++)
	{
		*pChar = ' ';
	}

	if (name[0] == 0 || !Q_stricmp(name, "console") || Q_strstr(name, "..") != NULL)
	{
		Info_SetValueForKey(userinfo, "name", "unnamed", MAX_INFO_STRING);
	}
	else
	{
		Info_SetValueForKey(userinfo, "name", name, MAX_INFO_STRING);
	}

	if (SV_CheckForDuplicateNames(userinfo, bIsReconnecting, nReconnectSlot))
	{
		Q_strncpy(name, Info_ValueForKey(userinfo, "name"), MAX_NAME - 1);
		name[MAX_NAME - 1] = 0;
	}

	s = Info_ValueForKey(userinfo, "*hltv");

	if (!s[0])
		return 1;

	switch (Q_atoi(s))
	{
	case 0:
		return 1;

	case 1:
		SV_CountProxies(&proxies);
		if (proxies >= sv_proxies.value && !bIsReconnecting)
		{
			SV_RejectConnection(adr, "Proxy slots are full.\n");
			return 0;
		}
		return 1;

	case 3:
		SV_RejectConnection(adr, "Please connect to HLTV master proxy.\n");
		return 0;

	default:
		SV_RejectConnection(adr, "Unknown HLTV client type.\n");
		return 0;
	}
}

/* <a77de> ../engine/sv_main.c:2822 */
int SV_FindEmptySlot(netadr_t *adr, int *pslot, client_t ** ppClient)
{
	if (g_psvs.maxclients > 0)
	{
		int slot;
		client_t *client = g_psvs.clients;

		for (slot = 0; slot < g_psvs.maxclients; slot++, client++)
		{
			if (!client->active && !client->spawned && !client->connected)
				break;
		}

		if (slot < g_psvs.maxclients)
		{
			*pslot = slot;
			*ppClient = client;
			return 1;
		}
	}

	SV_RejectConnection(adr, "Server is full.\n");
	return 0;
}

void SV_ConnectClient(void)
{
	g_RehldsHookchains.m_SV_ConnectClient.callChain(SV_ConnectClient_internal);
}

/* <ab6f0> ../engine/sv_main.c:2859 */
void EXT_FUNC SV_ConnectClient_internal(void)
{
	client_t *client;
	netadr_t adr;
	int nClientSlot;
#ifdef REHLDS_FIXES
	char userinfo[MAX_INFO_STRING];
#else
	char userinfo[1024];
#endif
	char protinfo[1024];
	char cdkey[64];
	const char *s;
	char name[32];
	char szRawCertificate[512];
	int nAuthProtocol;
	short unsigned int port;
	qboolean reconnect;
	qboolean bIsSecure;

	client = NULL;
	Q_memcpy(&adr, &net_from, sizeof(adr));
	nAuthProtocol = -1;
	reconnect = FALSE;
	port = Q_atoi("27005");
	if (Cmd_Argc() < 5)
	{
		SV_RejectConnection(&adr, "Insufficient connection info\n");
		return;
	}

	if (!SV_CheckProtocol(&adr, Q_atoi(Cmd_Argv(1))))
		return;

	if (!SV_CheckChallenge(&adr, Q_atoi(Cmd_Argv(2))))
		return;

	Q_memset(szRawCertificate, 0, sizeof(szRawCertificate));
	s = Cmd_Argv(3);
	if (!Info_IsValid(s))
	{
		SV_RejectConnection(&adr, "Invalid protinfo in connect command\n");
		return;
	}

	Q_strncpy(protinfo, s, sizeof(protinfo) - 1);
	protinfo[sizeof(protinfo) - 1] = 0;
	if (!SV_CheckKeyInfo(&adr, protinfo, &port, &nAuthProtocol, szRawCertificate, cdkey))
		return;

	if (!SV_CheckIPRestrictions(&adr, nAuthProtocol))
	{
		SV_RejectConnection(&adr, "LAN servers are restricted to local clients (class C).\n");
		return;
	}

	s = Cmd_Argv(4);
#ifdef REHLDS_FIXES
	// truncate to 255 before sanity checks
	if (Q_strlen(s) > MAX_INFO_STRING - 1 || !Info_IsValid(s))
#else
	if (Q_strlen(s) > 256 || !Info_IsValid(s))
#endif
	{
		SV_RejectConnection(&adr, "Invalid userinfo in connect command\n");
		return;
	}
	Q_strncpy(userinfo, s, sizeof(userinfo) - 1);
	userinfo[sizeof(userinfo) - 1] = 0;
	if (Master_IsLanGame() || nAuthProtocol == 2 || nAuthProtocol == 3)
	{
		for (nClientSlot = 0; nClientSlot < g_psvs.maxclients; nClientSlot++)
		{
			client = &g_psvs.clients[nClientSlot];
			if (NET_CompareAdr(adr, client->netchan.remote_address))
			{
				reconnect = TRUE;
				break;
			}
		}
	}
	
	if (!SV_CheckUserInfo(&adr, userinfo, reconnect, nClientSlot, name))
		return;
	
	if (!SV_FinishCertificateCheck(&adr, nAuthProtocol, szRawCertificate, userinfo))
		return;
	
	if (reconnect)
	{
		Steam_NotifyClientDisconnect(client);
		if ((client->active || client->spawned) && client->edict)
			gEntityInterface.pfnClientDisconnect(client->edict);
	
		Con_Printf("%s:reconnect\n", NET_AdrToString(adr));
	}
	else
	{
		if (!SV_FindEmptySlot(&adr, &nClientSlot, &client))
			return;
	}

	if (!SV_CheckIPConnectionReuse(&adr))
		return;

	host_client = client;
	client->userid = g_userid++;
	if (nAuthProtocol == 3)
	{
		char szSteamAuthBuf[1024];
		int len = net_message.cursize - msg_readcount;
		if (net_message.cursize - msg_readcount <= 0 || len >= sizeof(szSteamAuthBuf))
		{
			SV_RejectConnection(&adr, "STEAM certificate length error! %i/%i\n", net_message.cursize - msg_readcount, sizeof(szSteamAuthBuf));
			return;
		}
		Q_memcpy(szSteamAuthBuf, &net_message.data[msg_readcount], len);
		client->network_userid.clientip = *(uint32 *)&adr.ip[0];
		if (adr.type == NA_LOOPBACK)
		{
			if (sv_lan.value <= 0.0f)
				client->network_userid.clientip = *(uint32 *)&adr.ip[0];
			else
				client->network_userid.clientip = 0x7F000001; //127.0.0.1
		}

		client->netchan.remote_address.port = adr.port ? adr.port : port;
		if (!Steam_NotifyClientConnect(client, szSteamAuthBuf, len))
		{
			if (sv_lan.value == 0.0f)
			{
				SV_RejectConnection(&adr, "STEAM validation rejected\n");
				return;
			}
			host_client->network_userid.idtype = AUTH_IDTYPE_STEAM;
			host_client->network_userid.m_SteamID = 0;
		}
	}
	else
	{
		if (nAuthProtocol != 2)
		{
			SV_RejectConnection(&adr, "Invalid authentication type\n");
			return;
		}

		const char* val = Info_ValueForKey(userinfo, "*hltv");
		if (!Q_strlen(val))
		{
			SV_RejectConnection(&adr, "Invalid validation type\n");
			return;
		}
		if (Q_atoi(val) != 1)
		{
			SV_RejectConnection(&adr, "Invalid validation type\n");
			return;
		}
		host_client->network_userid.idtype = AUTH_IDTYPE_LOCAL;
		host_client->network_userid.m_SteamID = 0;
		host_client->network_userid.clientip = *(uint32 *)&adr.ip[0];
		Steam_NotifyBotConnect(client);
	}

	SV_ClearResourceLists(host_client);
	SV_ClearFrames(&host_client->frames);

	host_client->frames = (client_frame_t *)Mem_ZeroMalloc(sizeof(client_frame_t) * SV_UPDATE_BACKUP);
	host_client->resourcesneeded.pPrev = &host_client->resourcesneeded;
	host_client->resourcesneeded.pNext = &host_client->resourcesneeded;
	host_client->resourcesonhand.pPrev = &host_client->resourcesonhand;
	host_client->resourcesonhand.pNext = &host_client->resourcesonhand;
	host_client->edict = EDICT_NUM(nClientSlot + 1);
	
	if (g_modfuncs.m_pfnConnectClient)
		g_modfuncs.m_pfnConnectClient(nClientSlot);

	Netchan_Setup(NS_SERVER, &host_client->netchan,	adr, client - g_psvs.clients, client, SV_GetFragmentSize);
	host_client->next_messageinterval = 5.0;
	host_client->next_messagetime = realtime + 0.05;
	host_client->delta_sequence = -1;
	Q_memset(&host_client->lastcmd, 0, sizeof(usercmd_t));
	host_client->nextping = -1.0;
	if (host_client->netchan.remote_address.type == NA_LOOPBACK)
	{
		Con_DPrintf("Local connection.\n");
	}
	else
	{
		Con_DPrintf("Client %s connected\nAdr: %s\n", name, NET_AdrToString(host_client->netchan.remote_address));
	}
	Q_strncpy(host_client->hashedcdkey, cdkey, 32);

	host_client->hashedcdkey[32] = 0;
	host_client->active = FALSE;
	host_client->spawned = FALSE;
	host_client->connected = TRUE;
	host_client->uploading = FALSE;
	host_client->fully_connected = FALSE;

	bIsSecure = Steam_GSBSecure();
	Netchan_OutOfBandPrint(NS_SERVER, adr, "%c %i \"%s\" %i %i", 66, host_client->userid, NET_AdrToString(host_client->netchan.remote_address), bIsSecure, build_number());
	Log_Printf("\"%s<%i><%s><>\" connected, address \"%s\"\n", name, host_client->userid, SV_GetClientIDString(host_client), NET_AdrToString(host_client->netchan.remote_address));
	Q_strncpy(host_client->userinfo, userinfo, MAX_INFO_STRING);
	host_client->userinfo[MAX_INFO_STRING - 1] = 0;

	SV_ExtractFromUserinfo(host_client);
	Info_SetValueForStarKey(host_client->userinfo, "*sid", va("%lld", host_client->network_userid.m_SteamID), MAX_INFO_STRING);

	host_client->datagram.flags = 1;
	host_client->datagram.data = (byte *)host_client->datagram_buf;
	host_client->datagram.maxsize = sizeof(host_client->datagram_buf);
	host_client->datagram.buffername = host_client->name;
	host_client->sendinfo_time = 0.0f;

	//Rehlds Security
	Rehlds_Security_ClientConnected(host_client - g_psvs.clients);

	g_RehldsHookchains.m_ClientConnected.callChain(NULL, GetRehldsApiClient(host_client));
}

/* <a78b5> ../engine/sv_main.c:3179 */
void SVC_Ping(void)
{
	char data[6] = "\xff\xff\xff\xffj";
	NET_SendPacket(NS_SERVER, sizeof(data), data, net_from);
}

int SV_GetChallenge(const netadr_t& adr)
{
	int i;
#ifndef REHLDS_OPT_PEDANTIC
	int oldest = 0;
	int oldestTime = INT_MAX;
#endif

	for (i = 0; i < MAX_CHALLENGES; i++)
	{
		if (NET_CompareBaseAdr(adr, g_rg_sv_challenges[i].adr))
			break;
#ifndef REHLDS_OPT_PEDANTIC
		if (g_rg_sv_challenges[i].time < oldestTime)
		{
			oldest = i;
			oldestTime = g_rg_sv_challenges[i].time;
		}
#endif
	}

	if (i == MAX_CHALLENGES)
	{
#ifdef REHLDS_OPT_PEDANTIC
		// oldest challenge is always next after last generated
		i = g_oldest_challenge++;

		if (g_oldest_challenge >= MAX_CHALLENGES)
			g_oldest_challenge = 0;
#else
		i = oldest;
#endif
		// generate new challenge number
#ifdef REHLDS_FIXES
		g_rg_sv_challenges[i].challenge = (RandomLong(0, 0x7fff) << 16) | (RandomLong(0, 0xffff));
#else // REHLDS_FIXES
		g_rg_sv_challenges[i].challenge = (RandomLong(0, 36863) << 16) | (RandomLong(0, 65535));
#endif // REHLDS_FIXES
		g_rg_sv_challenges[i].adr = adr;
		g_rg_sv_challenges[i].time = (int)realtime;
	}

	return g_rg_sv_challenges[i].challenge;
}

/* <a78d3> ../engine/sv_main.c:3208 */
void SVC_GetChallenge(void)
{
	char data[1024];
	qboolean steam = (Cmd_Argc() == 2 && !Q_stricmp(Cmd_Argv(1), "steam"));
	int challenge = SV_GetChallenge(net_from);

	if (steam)
		Q_snprintf(data, sizeof(data), "\xFF\xFF\xFF\xFF%c00000000 %u 3 %I64i %d\n", S2C_CHALLENGE, challenge, Steam_GSGetSteamID(), Steam_GSBSecure());
	else
	{
		Con_DPrintf("Server requiring authentication\n");
		Q_snprintf(data, sizeof(data), "\xFF\xFF\xFF\xFF%c00000000 %u 2\n", S2C_CHALLENGE, challenge);
	}

	// Give 3-rd party plugins a chance to modify challenge response
	g_RehldsHookchains.m_SVC_GetChallenge_mod.callChain(NULL, data, challenge);
	NET_SendPacket(NS_SERVER, Q_strlen(data) + 1, data, net_from);
}

/* <a794c> ../engine/sv_main.c:3292 */
void SVC_ServiceChallenge(void)
{
	char data[128];
	const char *type;
	int challenge;

	if (Cmd_Argc() != 2)
		return;

	type = Cmd_Argv(1);
	if (!type)
		return;

	if (!type[0] || Q_stricmp(type, "rcon"))
		return;

	challenge = SV_GetChallenge(net_from);

	Q_snprintf(data, sizeof(data), "%c%c%c%cchallenge %s %u\n", 255, 255, 255, 255, type, challenge);

	NET_SendPacket(NS_SERVER, Q_strlen(data) + 1, data, net_from);
}

/* <a7981> ../engine/sv_main.c:3346 */
void SV_ResetModInfo(void)
{
	FileHandle_t hLibListFile;
	unsigned int nFileSize;
	char *pszInputStream;
	int nBytesRead;
	char *pStreamPos;
	char szDllListFile[260];
	char szValue[256];
	char szKey[64];

	Q_memset(&gmodinfo, 0, sizeof(modinfo_t));
	gmodinfo.version = 1;
	gmodinfo.svonly = TRUE;
	gmodinfo.num_edicts = MAX_EDICTS;

	Q_snprintf(szDllListFile, sizeof(szDllListFile), "%s", "liblist.gam");
	hLibListFile = FS_Open(szDllListFile, "rb");
	if (!hLibListFile)
		return;

	nFileSize = FS_Size(hLibListFile);
	if (!nFileSize || (signed int)nFileSize > 256 * 1024)
	{
		Sys_Error("Game listing file size is bogus [%s: size %i]", "liblist.gam", nFileSize);
	}

	pszInputStream = (char *)Mem_Malloc(nFileSize + 1);
	if (!pszInputStream)
	{
		Sys_Error("Could not allocate space for game listing file of %i bytes", nFileSize + 1);
	}

	nBytesRead = FS_Read(pszInputStream, nFileSize, 1, hLibListFile);
	if (nBytesRead != nFileSize)
	{
		Sys_Error("Error reading in game listing file, expected %i bytes, read %i", nFileSize, nBytesRead);
	}

	pszInputStream[nFileSize] = 0;
	pStreamPos = pszInputStream;
	com_ignorecolons = TRUE;
	while (1)
	{
		pStreamPos = COM_Parse(pStreamPos);
		if (com_token[0] == 0)
			break;
		Q_strncpy(szKey, com_token, sizeof(szKey) - 1);
		szKey[sizeof(szKey) - 1] = 0;
		pStreamPos = COM_Parse(pStreamPos);
		Q_strncpy(szValue, com_token, sizeof(szValue) - 1);
		szValue[sizeof(szValue) - 1] = 0;

		if (Q_stricmp(szKey, "gamedll"))
			DLL_SetModKey(&gmodinfo, szKey, szValue);
	}

	com_ignorecolons = FALSE;
	Mem_Free(pszInputStream);
	FS_Close(hLibListFile);
}

/* <a5cfb> ../engine/sv_main.c:3431 */
int SV_GetFakeClientCount(void)
{
	int i;
	int fakeclients = 0;

	for (i = 0; i < g_psvs.maxclients; i++)
	{
		client_t *client = &g_psvs.clients[i];

		if (client->fakeclient)
			fakeclients++;
	}
	return fakeclients;
}

/* <a7a53> ../engine/sv_main.c:3445 */
NOXREF qboolean SV_GetModInfo(char *pszInfo, char *pszDL, int *version, int *size, qboolean *svonly, qboolean *cldll, char *pszHLVersion)
{
	if (gmodinfo.bIsMod)
	{
		Q_strcpy(pszInfo, gmodinfo.szInfo);
		Q_strcpy(pszDL, gmodinfo.szDL);
		Q_strcpy(pszHLVersion, gmodinfo.szHLVersion);

		*version = gmodinfo.version;
		*size = gmodinfo.size;
		*svonly = gmodinfo.svonly;
		*cldll = gmodinfo.cldll;
	}
	else
	{
		Q_strcpy(pszInfo, "");
		Q_strcpy(pszDL, "");
		Q_strcpy(pszHLVersion, "");

		*version = 1;
		*size = 0;
		*svonly = TRUE;
		*cldll = FALSE;
	}
	return gmodinfo.bIsMod;
}

/* <a5cdb> ../engine/sv_main.c:3479 */
NOXREF qboolean RequireValidChallenge(netadr_t* /*adr*/)
{
	return sv_enableoldqueries.value == 0.0f;
}

/* <a7b26> ../engine/sv_main.c:3490 */
NOXREF qboolean ValidInfoChallenge(netadr_t *adr, const char *nugget)
{
	if (nugget && g_psv.active && g_psvs.maxclients > 1)
	{
		if (RequireValidChallenge(adr))
			return Q_stricmp(nugget, "Source Engine Query") == 0;
		return TRUE;
	}
	return FALSE;
}

/* <a7b79> ../engine/sv_main.c:3510 */
NOXREF int GetChallengeNr(netadr_t *adr)
{
	int oldest = 0;
	int oldestTime = 0x7FFFFFFFu;
	int i;

	for (i = 0; i < MAX_CHALLENGES; i++)
	{
		if (NET_CompareBaseAdr(*adr, g_rg_sv_challenges[i].adr))
			break;

		if (g_rg_sv_challenges[i].time < oldestTime)
		{
			oldestTime = g_rg_sv_challenges[i].time;
			oldest = i;
		}
	}
	if (i == MAX_CHALLENGES)
	{
		g_rg_sv_challenges[oldest].challenge = RandomLong(0, 65535) | (RandomLong(0, 36863) << 16);
		g_rg_sv_challenges[oldest].adr = net_from;
		g_rg_sv_challenges[oldest].time = realtime;
		i = oldest;
	}
	return g_rg_sv_challenges[i].challenge;
}

/* <a7bd5> ../engine/sv_main.c:3546 */
NOXREF qboolean CheckChallengeNr(netadr_t *adr, int nChallengeValue)
{
	int i;
	if (nChallengeValue == -1 || adr == NULL)
		return FALSE;

	for (i = 0; i < MAX_CHALLENGES; i++)
	{
		if (NET_CompareBaseAdr(*adr, g_rg_sv_challenges[i].adr))
		{
			if (g_rg_sv_challenges[i].challenge == nChallengeValue)
			{
				if (g_rg_sv_challenges[i].time + 3600.0f >= realtime)
					return TRUE;
			}
			return FALSE;
		}
	}
	return FALSE;
}

/* <a7c3a> ../engine/sv_main.c:3589 */
NOXREF void ReplyServerChallenge(netadr_t *adr)
{
	char buffer[16];
	sizebuf_t buf;

	buf.buffername = "SVC_Challenge";
	buf.data = (byte *)buffer;
	buf.maxsize = sizeof(buffer);
	buf.cursize = 0;
	buf.flags = SIZEBUF_ALLOW_OVERFLOW;

	MSG_WriteLong(&buf, 0xffffffff);
	MSG_WriteByte(&buf, 65);
	MSG_WriteLong(&buf, GetChallengeNr(adr));
	NET_SendPacket(NS_SERVER, buf.cursize, (char *)buf.data, *adr);
}

/* <a7c9c> ../engine/sv_main.c:3615 */
NOXREF qboolean ValidChallenge(netadr_t *adr, int challengeNr)
{
	if (!g_psv.active)
		return FALSE;

	if (g_psvs.maxclients <= 1)
		return FALSE;

	if (!RequireValidChallenge(adr) || CheckChallengeNr(adr, challengeNr))
		return TRUE;

	ReplyServerChallenge(adr);
	return FALSE;
}

/* <a7d60> ../engine/sv_main.c:3645 */
NOXREF void SVC_InfoString(void)
{
	int i;
	int count = 0;
	int proxy = 0;
	sizebuf_t buf;
	unsigned char data[MAX_ROUTEABLE_PACKET];
	char address[256];
	char gd[260];
	char info[2048];
	int iHasPW = 0;
	char szOS[2];

#ifdef _WIN32
	if (noip && noipx)
		return;
#else
	if (noip)
		return;
#endif // _WIN32

	if (!g_psv.active)
		return;

	buf.buffername = "SVC_InfoString";
	buf.data = data;
	buf.maxsize = sizeof(data);
	buf.cursize = 0;
	buf.flags = SIZEBUF_ALLOW_OVERFLOW;

	for (i = 0; i < g_psvs.maxclients; i++)
	{
		client_t *client = &g_psvs.clients[i];
		if (client->active || client->spawned || client->connected)
		{
			if (client->proxy)
				proxy++;
			else
				count++;
		}
	}

	address[0] = 0;

#ifdef _WIN32
	if (noip && !noipx)
		Q_strncpy(address, NET_AdrToString(net_local_ipx_adr), 255);
	else
		Q_strncpy(address, NET_AdrToString(net_local_adr), 255);

	Q_strcpy(szOS, "w");
#else
	Q_strncpy(address, NET_AdrToString(net_local_adr), 255);
	Q_strcpy(szOS, "l");

#endif // _WIN32

	address[255] = 0;

	info[0] = 0;

    if (*sv_password.string)
		iHasPW = Q_stricmp(sv_password.string, "none") != 0;

	Info_SetValueForKey(info, "protocol", va("%i", PROTOCOL_VERSION), sizeof(info));
	Info_SetValueForKey(info, "address", address, sizeof(info));
	Info_SetValueForKey(info, "players", va("%i", count), sizeof(info));
	Info_SetValueForKey(info, "proxytarget", va("%i", proxy), sizeof(info));
	Info_SetValueForKey(info, "lan", va("%i", Master_IsLanGame() != FALSE), sizeof(info));

	int maxplayers = (int)sv_visiblemaxplayers.value;
	if (maxplayers < 0)
		maxplayers = g_psvs.maxclients;

	Info_SetValueForKey(info, "max", va("%i", maxplayers), sizeof(info));
	Info_SetValueForKey(info, "bots", va("%i", SV_GetFakeClientCount()), sizeof(info));

	COM_FileBase(com_gamedir, gd);
	Info_SetValueForKey(info, "gamedir", gd, sizeof(info));

#ifdef REHLDS_FIXES
	if (gEntityInterface.pfnGetGameDescription)
		Info_SetValueForKey(info, "description", gEntityInterface.pfnGetGameDescription(), ARRAYSIZE(info));
#else
	Info_SetValueForKey(info, "description", gEntityInterface.pfnGetGameDescription(), ARRAYSIZE(info));
#endif // REHLDS_FIXES
	Info_SetValueForKey(info, "hostname", Cvar_VariableString("hostname"), ARRAYSIZE(info));
	Info_SetValueForKey(info, "map", g_psv.name, ARRAYSIZE(info));

	const char *type;
	if (g_pcls.state)
		type = "l";
	else
		type = "d";
	Info_SetValueForKey(info, "type", type, sizeof(info));
	Info_SetValueForKey(info, "password", va("%i", iHasPW), sizeof(info));
	Info_SetValueForKey(info, "os", szOS, sizeof(info));
	Info_SetValueForKey(info, "secure", Steam_GSBSecure() ? "0" : "1", sizeof(info));

	if (gmodinfo.bIsMod)
	{
		Info_SetValueForKey(info, "mod", va("%i", 1), sizeof(info));
		Info_SetValueForKey(info, "modversion", va("%i", gmodinfo.version), sizeof(info));
		Info_SetValueForKey(info, "svonly", va("%i", gmodinfo.svonly), sizeof(info));
		Info_SetValueForKey(info, "cldll", va("%i", gmodinfo.cldll), sizeof(info));
	}

	MSG_WriteLong(&buf, 0xffffffff);
	MSG_WriteString(&buf, "infostringresponse");
	MSG_WriteString(&buf, info);
	NET_SendPacket(NS_SERVER, buf.cursize, (char *)buf.data, net_from);
}

/* <a7e8d> ../engine/sv_main.c:3783 */
NOXREF void SVC_Info(qboolean bDetailed)
{
	int i;
	int count = 0;
	sizebuf_t buf;
	unsigned char data[MAX_ROUTEABLE_PACKET];
	char szModURL_Info[512];
	char szModURL_DL[512];
	int mod_version;
	int mod_size;
	char gd[260];
	qboolean cldll;
	qboolean svonly;
	char szHLVersion[32];

	if (!g_psv.active)
		return;

	buf.buffername = "SVC_Info";
	buf.data = data;
	buf.maxsize = sizeof(data);
	buf.cursize = 0;
	buf.flags = SIZEBUF_ALLOW_OVERFLOW;

	for (i = 0; i < g_psvs.maxclients; i++)
	{
		client_t *client = &g_psvs.clients[i];
		if (client->active || client->spawned || client->connected)
			count++;
	}

	MSG_WriteLong(&buf, 0xffffffff);
	MSG_WriteByte(&buf, bDetailed ? 109 : 67);

	if (noip)
	{
#ifdef _WIN32
		if (!noipx)
			MSG_WriteString(&buf, NET_AdrToString(net_local_ipx_adr));
		else
#endif // _WIN32

		MSG_WriteString(&buf, "LOOPBACK");
	}
	else
		MSG_WriteString(&buf, NET_AdrToString(net_local_adr));

	MSG_WriteString(&buf, Cvar_VariableString("hostname"));
	MSG_WriteString(&buf, g_psv.name);
	COM_FileBase(com_gamedir, gd);
	MSG_WriteString(&buf, gd);

#ifdef REHLDS_FIXES
	if (gEntityInterface.pfnGetGameDescription)
		MSG_WriteString(&buf, gEntityInterface.pfnGetGameDescription());
	else
		MSG_WriteString(&buf, "");
#else
	MSG_WriteString(&buf, gEntityInterface.pfnGetGameDescription());
#endif // REHLDS_FIXES

	MSG_WriteByte(&buf, count);
	int maxplayers = (int)sv_visiblemaxplayers.value;
	if (maxplayers < 0)
		maxplayers = g_psvs.maxclients;

	MSG_WriteByte(&buf, maxplayers);
	MSG_WriteByte(&buf, PROTOCOL_VERSION);

	if (bDetailed)
	{
		MSG_WriteByte(&buf, g_pcls.state != ca_dedicated ? 108 : 100);

#ifdef _WIN32
		MSG_WriteByte(&buf, 119);
#else
		MSG_WriteByte(&buf, 108);
#endif // _WIN32

		if (Q_strlen(sv_password.string) > 0 && Q_stricmp(sv_password.string, "none"))
			MSG_WriteByte(&buf, 1);
		else
			MSG_WriteByte(&buf, 0);

		if (SV_GetModInfo(szModURL_Info, szModURL_DL, &mod_version, &mod_size, &svonly, &cldll, szHLVersion))
		{
			MSG_WriteByte(&buf, 1);
			MSG_WriteString(&buf, szModURL_Info);
			MSG_WriteString(&buf, szModURL_DL);
			MSG_WriteString(&buf, "");
			MSG_WriteLong(&buf, mod_version);
			MSG_WriteLong(&buf, mod_size);
			MSG_WriteByte(&buf, svonly != FALSE);
			MSG_WriteByte(&buf, cldll != FALSE);
		}
		else
			MSG_WriteByte(&buf, 0);

		MSG_WriteByte(&buf, Steam_GSBSecure() != FALSE);
		MSG_WriteByte(&buf, SV_GetFakeClientCount());
	}
	NET_SendPacket(NS_SERVER, buf.cursize, (char *)buf.data, net_from);
}

/* <a5a58> ../engine/sv_main.c:3940 */
typedef struct entcount_s
{
	int entdata;
	int playerdata;
} entcount_t;

//entcount_t ent_datacounts[32];

/* <a802d> ../engine/sv_main.c:3954 */
NOXREF void SVC_PlayerInfo(void)
{
	int i;
	int count = 0;
	client_t *client;
	sizebuf_t buf;
	unsigned char data[2048];

	if (!g_psv.active)
		return;

	if (g_psvs.maxclients <= 1)
		return;

	buf.buffername = "SVC_PlayerInfo";
	buf.data = data;
	buf.maxsize = sizeof(data);
	buf.cursize = 0;
	buf.flags = SIZEBUF_ALLOW_OVERFLOW;

	MSG_WriteLong(&buf, 0xffffffff);
	MSG_WriteByte(&buf, 68);

	for (i = 0; i < g_psvs.maxclients; i++)
	{
		client = &g_psvs.clients[i];
		if (client->active)
			count++;
	}

	MSG_WriteByte(&buf, count);

	count = 0;
	for (i = 0; i < g_psvs.maxclients; i++)
	{
		client = &g_psvs.clients[i];

		if (client->active)
		{
			MSG_WriteByte(&buf, ++count);
			MSG_WriteString(&buf, client->name);
			MSG_WriteLong(&buf, client->edict->v.frags);
			MSG_WriteFloat(&buf, (float)(realtime - client->netchan.connect_time));
		}
	}
	NET_SendPacket(NS_SERVER, buf.cursize, (char *)buf.data, net_from);
}

/* <a80a6> ../engine/sv_main.c:4017 */
NOXREF void SVC_RuleInfo(void)
{
	int nNumRules;
	cvar_t *var;
	sizebuf_t buf;
	unsigned char data[8192];

	if (!g_psv.active)
		return;

	if (g_psvs.maxclients <= 1)
		return;

	buf.buffername = "SVC_RuleInfo";
	buf.data = data;
	buf.maxsize = sizeof(data);
	buf.cursize = 0;
	buf.flags = SIZEBUF_ALLOW_OVERFLOW;

	nNumRules = Cvar_CountServerVariables();
	if (!nNumRules)
		return;

	MSG_WriteLong(&buf, 0xffffffff);
	MSG_WriteByte(&buf, 69);
	MSG_WriteShort(&buf, nNumRules);

	var = cvar_vars;
	while (var != NULL)
	{
		if (var->flags & FCVAR_SERVER)
		{
			MSG_WriteString(&buf, var->name);
			if (var->flags & FCVAR_PROTECTED)
			{
				if (Q_strlen(var->string) > 0 && Q_stricmp(var->string, "none"))
					MSG_WriteString(&buf, "1");
				else
					MSG_WriteString(&buf, "0");
			}
			else
				MSG_WriteString(&buf, var->string);
		}
		var = var->next;
	}
	NET_SendPacket(NS_SERVER, buf.cursize, (char *)buf.data, net_from);
}

/* <a8112> ../engine/sv_main.c:4083 */
int SVC_GameDllQuery(const char *s)
{
	int len;
	unsigned char data[4096];
	int valid;

	if (!g_psv.active || g_psvs.maxclients <= 1)
		return 0;

	Q_memset(data, 0, sizeof(data));
	len = 2044 - sizeof(data);
	valid = gEntityInterface.pfnConnectionlessPacket(&net_from, s, (char *) &data[4], &len);
	if (len && len <= 2044)
	{
		*(uint32 *)data = 0xFFFFFFFF; //connectionless packet
		NET_SendPacket(NS_SERVER, len + 4, data, net_from);
	}
	return valid;
}

/* <a817b> ../engine/sv_main.c:4133 */
void SV_FlushRedirect(void)
{
	unsigned char *data;
	sizebuf_t buf;

	if (sv_redirected == RD_PACKET)
	{
		int allocLen = Q_strlen(outputbuf) + 10;
		allocLen &= 0xFFFFFFFC;
		data = (unsigned char *)alloca(allocLen);

		buf.buffername = "Redirected Text";
		buf.data = data;
		buf.maxsize = Q_strlen(outputbuf) + 7;
		buf.cursize = 0;
		buf.flags = SIZEBUF_ALLOW_OVERFLOW;

		MSG_WriteLong(&buf, -1);
		MSG_WriteByte(&buf, 0x6Cu);
		MSG_WriteString(&buf, outputbuf);
		MSG_WriteByte(&buf, 0);
		NET_SendPacket(NS_SERVER, buf.cursize, buf.data, sv_redirectto);
		outputbuf[0] = 0;
	}
	else
	{
		if (sv_redirected == RD_CLIENT)
		{
			MSG_WriteByte(&host_client->netchan.message, svc_print);
			MSG_WriteString(&host_client->netchan.message, outputbuf);
		}
		outputbuf[0] = 0;
	}
}

/* <a81bf> ../engine/sv_main.c:4184 */
void SV_EndRedirect(void)
{
	SV_FlushRedirect();
	sv_redirected = RD_NONE;
}

/* <a81f5> ../engine/sv_main.c:4170 */
void SV_BeginRedirect(redirect_t rd, netadr_t *addr)
{
	Q_memcpy(&sv_redirectto, addr, sizeof(sv_redirectto));
	sv_redirected = rd;
	outputbuf[0] = 0;
}

#define MAX_RCON_FAILURES_STORAGE 32
#define MAX_RCON_FAILURES 20

/* <a5a8c> ../engine/sv_main.c:4214 */
typedef struct rcon_failure_s
{
	qboolean active;
	qboolean shouldreject;
	netadr_t adr;
	int num_failures;
	float last_update;
	float failure_times[MAX_RCON_FAILURES];
} rcon_failure_t;

rcon_failure_t g_rgRconFailures[MAX_RCON_FAILURES_STORAGE];

/* <a62b4> ../engine/sv_main.c:4229 */
void SV_ResetRcon_f(void)
{
	Q_memset(g_rgRconFailures, 0, sizeof(g_rgRconFailures));
}

/* <a822b> ../engine/sv_main.c:4238 */
void SV_AddFailedRcon(netadr_t *adr)
{
	int i;
	int best = 0;
	float best_update = -99999.0f;
	float time;
	qboolean found = FALSE;
	rcon_failure_t *r;
	int failed;

	if (sv_rcon_minfailures.value < 1.0f)
	{
		Cvar_SetValue("sv_rcon_minfailures", 1.0);
	}
	else if (sv_rcon_minfailures.value > 20.0f)
	{
		Cvar_SetValue("sv_rcon_minfailures", 20.0);
	}
	if (sv_rcon_maxfailures.value < 1.0f)
	{
		Cvar_SetValue("sv_rcon_maxfailures", 1.0);
	}
	else if (sv_rcon_maxfailures.value > 20.0f)
	{
		Cvar_SetValue("sv_rcon_maxfailures", 20.0);
	}
	if (sv_rcon_maxfailures.value < sv_rcon_minfailures.value)
	{
		float temp = sv_rcon_maxfailures.value;
		Cvar_SetValue("sv_rcon_maxfailures", sv_rcon_minfailures.value);
		Cvar_SetValue("sv_rcon_minfailures", temp);
	}
	if (sv_rcon_minfailuretime.value < 1.0f)
	{
		Cvar_SetValue("sv_rcon_minfailuretime", 1.0);
	}

	for (i = 0; i < MAX_RCON_FAILURES_STORAGE; i++)
	{
		r = &g_rgRconFailures[i];
		if (!r->active)
		{
			break;
		}
		if (NET_CompareAdr(r->adr, *adr))
		{
			found = TRUE;
			break;
		}
		time = (float)(realtime - r->last_update);
		if (time >= best_update)
		{
			best = i;
			best_update = time;
		}
	}

	// If no match found, take the oldest entry for usage
	if (i >= MAX_RCON_FAILURES_STORAGE)
	{
		r = &g_rgRconFailures[best];
	}

	// Prepare new or stale entry
	if (!found)
	{
		r->shouldreject = FALSE;
		r->num_failures = 0;
		Q_memcpy(&r->adr, adr, sizeof(netadr_t));
	}
	else if (r->shouldreject)
	{
		return;
	}

	r->active = TRUE;
	r->last_update = (float)realtime;

	if (r->num_failures >= sv_rcon_maxfailures.value)
	{
#ifdef REHLDS_FIXES
		// FIXED: Accessing beyond array
		for (i = 0; i < sv_rcon_maxfailures.value - 1; i++)
		{
			r->failure_times[i] = r->failure_times[i + 1];
		}
		r->num_failures = sv_rcon_maxfailures.value - 1;
#else // REHLDS_FIXES
		for (i = 0; i < sv_rcon_maxfailures.value; i++)
		{
			r->failure_times[i] = r->failure_times[i + 1];
		}
		r->num_failures--;
#endif // REHLDS_FIXES
	}

	r->failure_times[r->num_failures] = (float)realtime;
	r->num_failures++;

	failed = 0;
	for (i = 0; i < r->num_failures; i++)
	{
		if (realtime - r->failure_times[i] <= sv_rcon_minfailuretime.value)
			failed++;
	}

	if (failed >= sv_rcon_minfailures.value)
	{
		Con_Printf("User %s will be banned for rcon hacking\n", NET_AdrToString(*adr));
		r->shouldreject = TRUE;
	}
}

/* <a61f6> ../engine/sv_main.c:4364 */
qboolean SV_CheckRconFailure(netadr_t *adr)
{
	for (int i = 0; i < 32; i++)
	{
		rcon_failure_t *r = &g_rgRconFailures[i];
		if (NET_CompareAdr(*adr, r->adr))
		{
			if (r->shouldreject)
				return TRUE;
		}
	}

	return FALSE;
}

/* <a82c8> ../engine/sv_main.c:4400 */
int SV_Rcon_Validate(void)
{
	if (Cmd_Argc() < 3 || Q_strlen(rcon_password.string) == 0)
		return 1;

	if (sv_rcon_banpenalty.value < 0.0f)
		Cvar_SetValue("sv_rcon_banpenalty", 0.0);

	if (SV_CheckRconFailure(&net_from))
	{
		Con_Printf("Banning %s for rcon hacking attempts\n", NET_AdrToString(net_from));
		Cbuf_AddText(va("addip %i %s\n", (int)sv_rcon_banpenalty.value, NET_BaseAdrToString(net_from)));
		return 3;
	}

	if (!SV_CheckChallenge(&net_from, Q_atoi(Cmd_Argv(1))))
		return 2;
	
	if (Q_strcmp(Cmd_Argv(2), rcon_password.string))
	{
		SV_AddFailedRcon(&net_from);
		return 1;
	}
	return 0;
}

/* <a846e> ../engine/sv_main.c:4452 */
void SV_Rcon(netadr_t *net_from_)
{
	char remaining[512];
	char rcon_buff[1024];

	int invalid = SV_Rcon_Validate();
	int len = net_message.cursize - Q_strlen("rcon");
	if (len <= 0 || len >= sizeof(remaining))
		return;

	Q_memcpy(remaining, &net_message.data[Q_strlen("rcon")], len);
	remaining[len] = 0;
	if (invalid)
	{
		Con_Printf("Bad Rcon from %s:\n%s\n", NET_AdrToString(*net_from_), remaining);
		Log_Printf("Bad Rcon: \"%s\" from \"%s\"\n", remaining, NET_AdrToString(*net_from_));
	}
	else
	{
		Con_Printf("Rcon from %s:\n%s\n", NET_AdrToString(*net_from_), remaining);
		Log_Printf("Rcon: \"%s\" from \"%s\"\n", remaining, NET_AdrToString(*net_from_));
	}

	SV_BeginRedirect(RD_PACKET, net_from_);
	if (invalid)
	{
		if (invalid == 2)
			Con_Printf("Bad rcon_password.\n");
		else if (strlen(rcon_password.string) == 0)
			Con_Printf("Bad rcon_password.\nNo password set for this server.\n");
		else
			Con_Printf("Bad rcon_password.\n");
		
		SV_EndRedirect();
		return;
	}
	char *data = COM_Parse(COM_Parse(COM_Parse(remaining)));
	if (!data)
	{
		Con_Printf("Empty rcon\n");

#ifdef REHLDS_FIXES
		//missing SV_EndRedirect()
		SV_EndRedirect();
#endif // REHLDS_FIXES
		return;
	}

	Q_strncpy(rcon_buff, data, sizeof(rcon_buff) - 1);
	rcon_buff[sizeof(rcon_buff) - 1] = 0;
	Cmd_ExecuteString(rcon_buff, src_command);
	SV_EndRedirect();
}

/* <ab901> ../engine/sv_main.c:4564 */
void SV_ConnectionlessPacket(void)
{
	char *args;
	const char *c;

	MSG_BeginReading();
	MSG_ReadLong();
	args = MSG_ReadStringLine();
	Cmd_TokenizeString(args);
	c = Cmd_Argv(0);

	if (!Q_strcmp(c, "ping") || (c[0] == A2A_PING && (c[1] == 0 || c[1] == '\n')))
	{
		SVC_Ping();
	}
	else if (c[0] == A2A_ACK && (c[1] == 0 || c[1] == '\n'))
	{
		Con_Printf("A2A_ACK from %s\n", NET_AdrToString(net_from));
	}
	else if (c[0] == A2A_GETCHALLENGE || c[0] == A2S_INFO || c[0] == A2S_PLAYER || c[0] == A2S_RULES ||
		c[0] == S2A_LOGSTRING || c[0] == M2S_REQUESTRESTART || c[0] == M2A_CHALLENGE)
		return;

	else if (!Q_stricmp(c, "log"))
	{
		if (sv_logrelay.value != 0.0f && Q_strlen(args) > 4)
		{
			const char *s = &args[Q_strlen("log ")];
			if (s && s[0])
			{
				Con_Printf("%s\n", s);
			}
		}
	}
	else if (!Q_strcmp(c, "getchallenge"))
	{
		SVC_GetChallenge();
	}
	else if (!Q_stricmp(c, "challenge"))
	{
		SVC_ServiceChallenge();
	}
	else if (!Q_strcmp(c, "connect"))
	{
		SV_ConnectClient();
	}
	else if (!Q_strcmp(c, "pstat"))
	{
		if (g_modfuncs.m_pfnPlayerStatus)
			g_modfuncs.m_pfnPlayerStatus(net_message.data, net_message.cursize);
	}
	else if (!Q_strcmp(c, "rcon"))
	{
		SV_Rcon(&net_from);
	}
	else
		SVC_GameDllQuery(args);
}

/* <a8500> ../engine/sv_main.c:4656 */
void SV_CheckRate(client_t *cl)
{
	if (sv_maxrate.value > 0.0f)
	{
		if (cl->netchan.rate > sv_maxrate.value)
		{
			if (sv_maxrate.value > MAX_RATE)
				cl->netchan.rate = MAX_RATE;
			else
				cl->netchan.rate = sv_maxrate.value;
		}
	}
	if (sv_minrate.value > 0.0f)
	{
		if (cl->netchan.rate < sv_minrate.value)
		{
			if (sv_minrate.value < MIN_RATE)
				cl->netchan.rate = MIN_RATE;
			else
				cl->netchan.rate = sv_minrate.value;
		}
	}
}

/* <a8538> ../engine/sv_main.c:4675 */
void SV_ProcessFile(client_t *cl, char *filename)
{
	customization_t *pList;
	resource_t *resource;
	unsigned char md5[16];
	qboolean bFound;

	if (filename[0] != '!')
	{
		Con_Printf("Ignoring non-customization file upload of %s\n", filename);
		return;
	}

	COM_HexConvert(filename + 4, 32, md5);
	resource = cl->resourcesneeded.pNext;
	bFound = FALSE;
	while (resource != &cl->resourcesneeded)
	{
		if (!Q_memcmp(resource->rgucMD5_hash, md5, 16))
		{
			bFound = TRUE;
			break;
		}

		resource = resource->pNext;
	}

	if (!bFound)
	{
		Con_Printf("SV_ProcessFile:  Unrequested decal\n");
		return;
	}

	if (resource->nDownloadSize != cl->netchan.tempbuffersize)
	{
		Con_Printf("SV_ProcessFile:  Downloaded %i bytes for purported %i byte file\n", cl->netchan.tempbuffersize, resource->nDownloadSize);
		return;
	}

	HPAK_AddLump(TRUE, "custom.hpk", resource, cl->netchan.tempbuffer, NULL);
	resource->ucFlags &= ~RES_WASMISSING;
	SV_MoveToOnHandList(resource);
	pList = cl->customdata.pNext;
	while (pList) {
		if (!Q_memcmp(pList->resource.rgucMD5_hash, resource->rgucMD5_hash, 16))
		{
			Con_DPrintf("Duplicate resource received and ignored.\n");
			return;
		}
		pList = pList->pNext;
	}

	if (!COM_CreateCustomization(&cl->customdata, resource, -1, 7, NULL, NULL))
		Con_Printf("Error parsing custom decal from %s\n", cl->name);
}

/* <a85d5> ../engine/sv_main.c:4760 */
qboolean SV_FilterPacket(void)
{
	for (int i = numipfilters - 1; i >= 0; i--)
	{
		ipfilter_t* curFilter = &ipfilters[i];
		if (curFilter->compare.u32 == 0xFFFFFFFF || curFilter->banEndTime == 0.0f || curFilter->banEndTime > realtime)
		{
			if ((*(uint32*)net_from.ip & curFilter->mask) == curFilter->compare.u32)
				return (int)sv_filterban.value;
		}
		else
		{
			if (i < numipfilters - 1)
				memmove(curFilter, &curFilter[1], sizeof(ipfilter_t) * (numipfilters - i - 1));

			--numipfilters;
		}
	}
	return sv_filterban.value == 0.0f;
}

/* <a861b> ../engine/sv_main.c:4796 */
void SV_SendBan(void)
{
	char szMessage[64];
	_snprintf(szMessage, sizeof(szMessage), "You have been banned from this server.\n");

	SZ_Clear(&net_message);

	MSG_WriteLong(&net_message, -1);
	MSG_WriteByte(&net_message, 108);
	MSG_WriteString(&net_message, szMessage);
	NET_SendPacket(NS_SERVER, net_message.cursize, net_message.data, net_from);

	SZ_Clear(&net_message);
}

bool EXT_FUNC NET_GetPacketPreprocessor(uint8* data, unsigned int len, const netadr_t& srcAddr)
{
	return true;
}

/* <ab9af> ../engine/sv_main.c:4818 */
void SV_ReadPackets(void)
{
	while (NET_GetPacket(NS_SERVER))
	{
		if (SV_FilterPacket())
		{
			SV_SendBan();
			continue;
		}

		bool pass = g_RehldsHookchains.m_PreprocessPacket.callChain(NET_GetPacketPreprocessor, net_message.data, net_message.cursize, net_from);
		if (!pass)
			continue;

		if (*(uint32 *)net_message.data == 0xFFFFFFFF)
		{
			// Connectionless packet
			if (CheckIP(net_from))
			{
				Steam_HandleIncomingPacket(net_message.data, net_message.cursize, ntohl(*(u_long *)&net_from.ip[0]), htons(net_from.port));
				SV_ConnectionlessPacket();
			}
			else if (sv_logblocks.value != 0.0f)
			{
				Log_Printf("Traffic from %s was blocked for exceeding rate limits\n", NET_AdrToString(net_from));
			}
			continue;
		}

		for (int i = 0 ; i < g_psvs.maxclients; i++)
		{
			client_t *cl = &g_psvs.clients[i];
			if (!cl->connected && !cl->active && !cl->spawned)
			{
				continue;
			}

			if (NET_CompareAdr(net_from, cl->netchan.remote_address) != TRUE)
			{
				continue;
			}

			if (Netchan_Process(&cl->netchan))
			{
				if (g_psvs.maxclients == 1 || !cl->active || !cl->spawned || !cl->fully_connected)
				{
					cl->send_message = TRUE;
				}

				SV_ExecuteClientMessage(cl);
				gGlobalVariables.frametime = host_frametime;
			}

			if (Netchan_IncomingReady(&cl->netchan))
			{
				if (Netchan_CopyNormalFragments(&cl->netchan))
				{
					MSG_BeginReading();
					SV_ExecuteClientMessage(cl);
				}
				if (Netchan_CopyFileFragments(&cl->netchan))
				{
					host_client = cl;
					SV_ProcessFile(cl, cl->netchan.incomingfilename);
				}
			}
		}
	}
}

/* <aba20> ../engine/sv_main.c:4842 */
//NOBODY int ntohl(void);
//{
//}

/* <aba34> ../engine/sv_main.c:4842 */
//NOBODY int htons(void);
//{
//}

/* <a8656> ../engine/sv_main.c:4914 */
void SV_CheckTimeouts(void)
{
	int i;
	client_t *cl;
	float droptime;

	droptime = realtime - sv_timeout.value;

	for (i = 0, cl = g_psvs.clients; i < g_psvs.maxclients; i++, cl++)
	{
		if (cl->fakeclient)
			continue;
		if (!cl->connected && !cl->active && !cl->spawned)
			continue;
		if (cl->netchan.last_received < droptime)
		{
			SV_BroadcastPrintf("%s timed out\n", cl->name);
			SV_DropClient(cl, FALSE, "Timed out");
		}
	}
}

/* <a5d59> ../engine/sv_main.c:4943 */
int SV_CalcPing(client_t *cl)
{
	float ping;
	int i;
	int count;
	int back;
	client_frame_t *frame;
	int idx;

	if (cl->fakeclient)
	{
		return 0;
	}

	if (SV_UPDATE_BACKUP <= 31)
	{
		back = SV_UPDATE_BACKUP / 2;
		if (back <= 0)
		{
			return 0;
		}
	}
	else
	{
		back = 16;
	}

	ping = 0.0f;
	count = 0;
	for (i = 0; i < back; i++)
	{
		idx = cl->netchan.incoming_acknowledged + ~i;
		frame = &cl->frames[SV_UPDATE_MASK & idx];

		if (frame->ping_time > 0.0f)
		{
			ping += frame->ping_time;
			count++;
		}
	}

	if (count)
	{
		ping /= count;
		if (ping > 0.0f)
		{
			return ping * 1000.0f;
		}
	}
	return 0;
}

void EXT_FUNC SV_WriteFullClientUpdate_internal(IGameClient *client, char *info, size_t maxlen, sizebuf_t *sb, IGameClient *receiver)
{
	client_t* cl = client->GetClient();
	MD5Context_t ctx;
	unsigned char digest[16];

	MD5Init(&ctx);
	MD5Update(&ctx, (unsigned char*)cl->hashedcdkey, sizeof(cl->hashedcdkey));
	MD5Final(digest, &ctx);

	MSG_WriteByte(sb, svc_updateuserinfo);
	MSG_WriteByte(sb, cl - g_psvs.clients);
	MSG_WriteLong(sb, cl->userid);
	MSG_WriteString(sb, info);
	MSG_WriteBuf(sb, sizeof(digest), digest);
}

/* <a874a> ../engine/sv_main.c:4991 */
void SV_FullClientUpdate(client_t *cl, sizebuf_t *sb)
{
	char info[MAX_INFO_STRING];

	Q_strncpy(info, cl->userinfo, sizeof(info) - 1);
	info[sizeof(info) - 1] = 0;
	Info_RemovePrefixedKeys(info, '_');

	g_RehldsHookchains.m_SV_WriteFullClientUpdate.callChain(SV_WriteFullClientUpdate_internal, GetRehldsApiClient(cl), info, MAX_INFO_STRING, sb, GetRehldsApiClient((sb == &g_psv.reliable_datagram) ? NULL : host_client));
}

void EXT_FUNC SV_EmitEvents_api(IGameClient *cl, packet_entities_t *pack, sizebuf_t *ms)
{
	SV_EmitEvents_internal(cl->GetClient(), pack, ms);
}

void SV_EmitEvents(client_t *cl, packet_entities_t *pack, sizebuf_t *ms)
{
	g_RehldsHookchains.m_SV_EmitEvents.callChain(SV_EmitEvents_api, GetRehldsApiClient(cl), pack, ms);
}

/* <a8995> ../engine/sv_main.c:5027 */
void SV_EmitEvents_internal(client_t *cl, packet_entities_t *pack, sizebuf_t *msg)
{
	int i;
	int ev;
	event_state_t *es;
	event_info_t *info;
	entity_state_t *state;
	int ev_count = 0;
	int etofind;
	int c;
	event_args_t nullargs;

	Q_memset(&nullargs, 0, sizeof(event_args_t));

	es = &cl->events;

	for (ev = 0; ev < MAX_EVENT_QUEUE; ev++)
	{
		info = &es->ei[ev];

		if (info->index != 0)
		{
			ev_count++;
		}
	}

	if (ev_count == 0)
	{
		return;
	}

	if (ev_count > 31)
	{
		ev_count = 31;
	}

	for (ev = 0; ev < MAX_EVENT_QUEUE; ev++)
	{
		info = &es->ei[ev];

		if (info->index == 0)
		{
			continue;
		}

		etofind = info->entity_index;

		for (i = 0; i < pack->num_entities; i++)
		{
			state = &pack->entities[i];

			if (state->number == etofind)
			{
				break;
			}
		}

		if (i < pack->num_entities)
		{
			info->packet_index = i;
			info->args.ducking = 0;

			if (!(info->args.flags & FEVENT_ORIGIN))
			{
				info->args.origin[0] = 0.0f;
				info->args.origin[1] = 0.0f;
				info->args.origin[2] = 0.0f;
			}
			if (!(info->args.flags & FEVENT_ANGLES))
			{
				info->args.angles[0] = 0.0f;
				info->args.angles[1] = 0.0f;
				info->args.angles[2] = 0.0f;
			}
		}
		else
		{
			info->args.entindex = etofind;
			info->packet_index = pack->num_entities;
		}
	}

	MSG_WriteByte(msg, svc_event);
	MSG_StartBitWriting(msg);
	MSG_WriteBits(ev_count, 5);

	c = 0;

	for (ev = 0; ev < MAX_EVENT_QUEUE; ev++)
	{
		info = &es->ei[ev];

		if (info->index == 0)
		{
			info->packet_index = -1;
			info->entity_index = -1;
			continue;
		}

		if (c < ev_count)
		{
			MSG_WriteBits(info->index, 10);

			if (info->packet_index != -1)
			{
				MSG_WriteBits(1, 1);
				MSG_WriteBits(info->packet_index, 11);
				if (Q_memcmp(&nullargs, &info->args, sizeof(event_args_t)))
				{
					MSG_WriteBits(1, 1);
					DELTA_WriteDelta((byte *)&nullargs, (byte *)&info->args, TRUE, g_peventdelta, NULL);
				}
				else
				{
					MSG_WriteBits(0, 1);
				}
			}
			else
			{
				MSG_WriteBits(0, 1);
			}

			if (info->fire_time == 0.0f)
			{
				MSG_WriteBits(0, 1);
			}
			else
			{
				MSG_WriteBits(1, 1);
				MSG_WriteBits(info->fire_time * 100.0f, 16);
			}
		}

		info->index = 0;
		info->packet_index = -1;
		info->entity_index = -1;

		c++;
	}
	MSG_EndBitWriting(msg);
}

int fatbytes;
unsigned char fatpvs[1024];
int fatpasbytes;
unsigned char fatpas[1024];

/* <a8a7c> ../engine/sv_main.c:5196 */
void SV_AddToFatPVS(vec_t *org, mnode_t *node)
{
	while (node->contents >= 0)
	{
		mplane_t *plane = node->plane;
		float d = plane->normal[2] * org[2] + plane->normal[1] * org[1] + plane->normal[0] * org[0] - plane->dist;
		if (d <= 8.0f)
		{
			if (d >= -8.0f)
			{
				SV_AddToFatPVS(org, node->children[0]);
				node = node->children[1];
			}
			else
			{
				node = node->children[1];
			}
		}
		else
		{
			node = node->children[0];
		}
	}
	if (node->contents != CONTENTS_SOLID)
	{
		unsigned char *pvs = Mod_LeafPVS((mleaf_t *)node, g_psv.worldmodel);
		for (int i = 0; i < fatbytes; i++)
			fatpvs[i] |= pvs[i];
	}
}

/* <a8af2> ../engine/sv_main.c:5239 */
unsigned char* EXT_FUNC SV_FatPVS(float *org)
{
	fatbytes = (g_psv.worldmodel->numleafs + 31) >> 3;
	Q_memset(fatpvs, 0, fatbytes);
	SV_AddToFatPVS(org, g_psv.worldmodel->nodes);
	return fatpvs;
}

/* <a8b20> ../engine/sv_main.c:5249 */
void SV_AddToFatPAS(vec_t *org, mnode_t *node)
{
	int i;
	unsigned char *pas;
	mplane_t *plane;
	float d;

	while (node->contents >= 0)
	{
		plane = node->plane;
		d = org[0] * plane->normal[0] +
			org[1] * plane->normal[1] +
			org[2] * plane->normal[2] - plane->dist;

		if (d > 8.0f)
		{
			node = node->children[0];
		}
		else if (d < -8.0f)
		{
			node = node->children[1];
		}
		else
		{
			SV_AddToFatPAS(org, node->children[0]);
			node = node->children[1];
		}
	}

	if (node->contents == CONTENTS_SOLID)
	{
		return;
	}

	int leafnum = (mleaf_t *)node - g_psv.worldmodel->leafs;
	pas = CM_LeafPAS(leafnum);

	for (i = 0; i < fatpasbytes; ++i)
	{
		fatpas[i] |= pas[i];
	}
}

/* <a8bac> ../engine/sv_main.c:5295 */
unsigned char* EXT_FUNC SV_FatPAS(float *org)
{
	fatpasbytes = (g_psv.worldmodel->numleafs + 31) >> 3;
	Q_memset(fatpas, 0, fatpasbytes);
	SV_AddToFatPAS(org, g_psv.worldmodel->nodes);
	return fatpas;
}

/* <a6c4c> ../engine/sv_main.c:5304 */
int SV_PointLeafnum(vec_t *p)
{
	mleaf_t *mleaf = Mod_PointInLeaf(p, g_psv.worldmodel);
	return mleaf ? (mleaf - g_psv.worldmodel->leafs) : 0;
}

/* <a5e36> ../engine/sv_main.c:5313 */
void TRACE_DELTA(char *fmt, ...)
{
}

deltacallback_t g_svdeltacallback;

/* <a5dde> ../engine/sv_main.c:5349 */
void SV_SetCallback(int num, qboolean remove, qboolean custom, int *numbase, qboolean full, int offset)
{
	g_svdeltacallback.num = num;
	g_svdeltacallback.remove = remove;
	g_svdeltacallback.custom = custom;
	g_svdeltacallback.numbase = numbase;
	g_svdeltacallback.full = full;
	g_svdeltacallback.newbl = FALSE;
	g_svdeltacallback.newblindex = 0;
	g_svdeltacallback.offset = offset;
}

/* <a5e53> ../engine/sv_main.c:5361 */
void SV_SetNewInfo(int newblindex)
{
	g_svdeltacallback.newbl = TRUE;
	g_svdeltacallback.newblindex = newblindex;
}

/* <a8c81> ../engine/sv_main.c:5367 */
void SV_WriteDeltaHeader(int num, qboolean remove, qboolean custom, int *numbase, qboolean newbl, int newblindex, qboolean full, int offset)
{
	int delta;

	delta = num - *numbase;
	if (full)
	{
		if (delta == 1)
		{
			MSG_WriteBits(1, 1);
		}
		else
		{
			MSG_WriteBits(0, 1);
		}
	}
	else
	{
		MSG_WriteBits((remove != 0) ? 1 : 0, 1);
	}

	if (!full || delta != 1)
	{
		if (delta <= 0 || delta > 63)
		{
			MSG_WriteBits(1u, 1);
			MSG_WriteBits(num, 11);
		}
		else
		{
			MSG_WriteBits(0, 1);
			MSG_WriteBits(delta, 6);
		}
	}

	*numbase = num;
	if (!remove)
	{
		MSG_WriteBits(custom != 0, 1);
		if (g_psv.instance_baselines->number)
		{
			if (newbl)
			{
				MSG_WriteBits(1u, 1);
				MSG_WriteBits(newblindex, 6);
			}
			else
			{
				MSG_WriteBits(0, 1);
			}
		}
		if (full && !newbl)
		{
			if (offset)
			{
				MSG_WriteBits(1u, 1);
				MSG_WriteBits(offset, 6);
			}
			else
			{
				MSG_WriteBits(0, 1);
			}
		}
	}
}

/* <a8d24> ../engine/sv_main.c:5445 */
void SV_InvokeCallback(void)
{
	SV_WriteDeltaHeader(
		g_svdeltacallback.num,
		g_svdeltacallback.remove,
		g_svdeltacallback.custom,
		g_svdeltacallback.numbase,
		g_svdeltacallback.newbl,
		g_svdeltacallback.newblindex,
		g_svdeltacallback.full,
		g_svdeltacallback.offset
	);
}

/* <a8d3a> ../engine/sv_main.c:5467 */
int SV_FindBestBaseline(int index, entity_state_t ** baseline, entity_state_t *to, int num, qboolean custom)
{
	int bestbitnumber;
	delta_t* delta;

	if (custom)
	{
		delta = g_pcustomentitydelta;
	}
	else
	{
		if (SV_IsPlayerIndex(num))
			delta = g_pplayerdelta;
		else
			delta = g_pentitydelta;
	}

	bestbitnumber = DELTA_TestDelta((byte *)*baseline, (byte *)&to[index], delta);
	bestbitnumber -= 6;

	int i = 0;
	int bitnumber = 0;
	int bestfound = index;

	for (i = index - 1; bestbitnumber > 0 && i >= 0 && (index - i) <= 64; i--)
	{
		if (to[index].entityType == to[i].entityType)
		{
			bitnumber = DELTA_TestDelta((byte *)&to[i], (byte *)&to[index], delta);

			if (bitnumber < bestbitnumber)
			{
				bestbitnumber = bitnumber;
				bestfound = i;
			}
		}
	}

	if (index != bestfound)
		*baseline = &to[bestfound];

	return index - bestfound;
}

/* <a8e01> ../engine/sv_main.c:5525 */
int SV_CreatePacketEntities(sv_delta_t type, client_t *client, packet_entities_t *to, sizebuf_t *msg)
{
	packet_entities_t *from;
	int oldindex;
	int newindex;
	int oldnum;
	int newnum;
	int oldmax;
	int numbase;

	// fix for https://github.com/dreamstalker/rehlds/issues/24
#ifdef REHLDS_FIXES
	int baselineToIdx = -1; //index of the baseline in to->entities[]
	uint64 toBaselinesForceMask[MAX_PACKET_ENTITIES];
#endif

	numbase = 0;
	if (type == sv_packet_delta)
	{
		client_frame_t *fromframe = &client->frames[SV_UPDATE_MASK & client->delta_sequence];
		from = &fromframe->entities;
		_mm_prefetch((const char*)&from->entities[0], _MM_HINT_T0);
		_mm_prefetch(((const char*)&from->entities[0]) + 64, _MM_HINT_T0);
		oldmax = from->num_entities;
		MSG_WriteByte(msg, svc_deltapacketentities);
		MSG_WriteShort(msg, to->num_entities);
		MSG_WriteByte(msg, client->delta_sequence);
	}
	else
	{
		oldmax = 0;
		from = NULL;
		MSG_WriteByte(msg, svc_packetentities);
		MSG_WriteShort(msg, to->num_entities);
	}

	newnum = 0; //index in to->entities
	oldnum = 0; //index in from->entities
	MSG_StartBitWriting(msg);
	while (1)
	{
		if (newnum < to->num_entities)
		{
			newindex = to->entities[newnum].number;
		}
		else
		{
			if (oldnum >= oldmax)
				break;

			if (newnum < to->num_entities)
				newindex = to->entities[newnum].number;
			else
				newindex = 9999;
		}

		if (oldnum < oldmax)
			oldindex = from->entities[oldnum].number;
		else
			oldindex = 9999;

		if (newindex == oldindex)
		{
			entity_state_t *baseline_ = &to->entities[newnum];
			qboolean custom = baseline_->entityType & 0x2 ? TRUE : FALSE;
			SV_SetCallback(newindex, FALSE, custom, &numbase, FALSE, 0);
			DELTA_WriteDelta((uint8 *)&from->entities[oldnum], (uint8 *)baseline_, FALSE, custom ? g_pcustomentitydelta : (SV_IsPlayerIndex(newindex) ? g_pplayerdelta : g_pentitydelta), &SV_InvokeCallback);
			++oldnum;
			_mm_prefetch((const char*)&from->entities[oldnum], _MM_HINT_T0);
			_mm_prefetch(((const char*)&from->entities[oldnum]) + 64, _MM_HINT_T0);
			++newnum;
			continue;
		}

		if (newindex >= oldindex)
		{
			if (newindex > oldindex)
			{
				SV_WriteDeltaHeader(oldindex, TRUE, FALSE, &numbase, FALSE, 0, FALSE, 0);
				++oldnum;
				_mm_prefetch((const char*)&from->entities[oldnum], _MM_HINT_T0);
				_mm_prefetch(((const char*)&from->entities[oldnum]) + 64, _MM_HINT_T0);
			}
			continue;
		}

		edict_t *ent = EDICT_NUM(newindex);
		qboolean custom = to->entities[newnum].entityType & 0x2 ? TRUE : FALSE;
		SV_SetCallback(
			newindex,
			FALSE,
			custom,
			&numbase,
			from == 0,
			0);

		entity_state_t *baseline_ = &g_psv.baselines[newindex];
		if (sv_instancedbaseline.value != 0.0f && g_psv.instance_baselines->number != 0 && newindex > sv_lastnum)
		{
			for (int i = 0; i < g_psv.instance_baselines->number; i++)
			{
				if (g_psv.instance_baselines->classname[i] == ent->v.classname)
				{
					SV_SetNewInfo(i);
					baseline_ = &g_psv.instance_baselines->baseline[i];
					break;
				}
			}
		}
		else
		{
			if (!from)
			{
				int offset = SV_FindBestBaseline(newnum, &baseline_, to->entities, newindex, custom);
				_mm_prefetch((const char*)baseline_, _MM_HINT_T0);
				_mm_prefetch(((const char*)baseline_) + 64, _MM_HINT_T0);
				if (offset)
					SV_SetCallback(newindex, 0, custom, &numbase, 1, offset);

				// fix for https://github.com/dreamstalker/rehlds/issues/24
#ifdef REHLDS_FIXES
				if (offset) 
					baselineToIdx = newnum - offset;
#endif
			}
		}


		delta_t* delta = custom ? g_pcustomentitydelta : (SV_IsPlayerIndex(newindex) ? g_pplayerdelta : g_pentitydelta);

		// fix for https://github.com/dreamstalker/rehlds/issues/24
#ifdef REHLDS_FIXES
		DELTA_WriteDeltaForceMask(
			(uint8 *)baseline_,
			(uint8 *)&to->entities[newnum],
			TRUE,
			delta,
			&SV_InvokeCallback,
			baselineToIdx != -1 ? &toBaselinesForceMask[baselineToIdx] : NULL
		);
		baselineToIdx = -1;

		uint64 origMask = DELTAJit_GetOriginalMask(delta);
		uint64 usedMask = DELTAJit_GetMaskU64(delta);
		uint64 diffMask = origMask ^ usedMask;

		//Remember changed fields that was marked in original mask, but unmarked by the conditional encoder
		toBaselinesForceMask[newnum] = diffMask & origMask;
		
		
#else //REHLDS_FIXES
		DELTA_WriteDelta(
			(uint8 *)baseline_,
			(uint8 *)&to->entities[newnum],
			TRUE,
			delta,
			&SV_InvokeCallback
			);
#endif //REHLDS_FIXES

		++newnum;
		
	}

	MSG_WriteBits(0, 16);
	MSG_EndBitWriting(msg);
	return msg->cursize;
}

/* <a906b> ../engine/sv_main.c:5685 */
void SV_EmitPacketEntities(client_t *client, packet_entities_t *to, sizebuf_t *msg)
{
	SV_CreatePacketEntities(client->delta_sequence == -1 ? sv_packet_nodelta : sv_packet_delta, client, to, msg);
}

/* <a90cc> ../engine/sv_main.c:5708 */
qboolean SV_ShouldUpdatePing(client_t *client)
{
	if (client->proxy)
	{
		if (realtime < client->nextping)
			return 0;

		client->nextping = realtime + 2.0;
		return 1;
	}

	//useless call
	//SV_CalcPing(client);

	return client->lastcmd.buttons & 0x8000;
}

/* <a9109> ../engine/sv_main.c:5734 */
NOXREF qboolean SV_HasEventsInQueue(client_t *client)
{
	int i;
	event_state_t *es;
	event_info_t *ei;

	es = &client->events;

	for (i = 0; i < MAX_EVENT_QUEUE; i++)
	{
		ei = &es->ei[i];

		if (ei->index)
			return TRUE;
	}
	return FALSE;
}

/* <a91c0> ../engine/sv_main.c:5756 */
void SV_GetNetInfo(client_t *client, int *ping, int *packet_loss)
{
	static uint16 lastping[32];
	static uint8 lastloss[32];

	int i = client - g_psvs.clients;
	if (realtime >= client->nextping)
	{
		client->nextping = realtime + 2.0;
		lastping[i] = SV_CalcPing(client);
		lastloss[i] = client->packet_loss;
	}

	*ping = lastping[i];
	*packet_loss = lastloss[i];
}

/* <a92a5> ../engine/sv_main.c:5775 */
int EXT_FUNC SV_CheckVisibility(edict_t *entity, unsigned char *pset)
{
	int leaf;

	if (!pset)
		return 1;

	if (entity->headnode < 0)
	{
		for (int i = 0; i < entity->num_leafs; i++)
		{
			leaf = entity->leafnums[i];
			if (pset[leaf >> 3] & (1 << (leaf & 7)))
				return 1;
		}
		return 0;
	}
	else
	{
		for (int i = 0; i < 48; i++)
		{
			leaf = entity->leafnums[i];
			if (leaf == -1)
				break;

			if (pset[leaf >> 3] & (1 << (leaf & 7)))
				return 1;
		}

		if (CM_HeadnodeVisible(&g_psv.worldmodel->nodes[entity->headnode], pset, &leaf))
		{
			entity->leafnums[entity->num_leafs] = leaf;
			entity->num_leafs = (entity->num_leafs + 1) % 48;
			return 2;
		}

		return 0;
	}
}

/* <a9324> ../engine/sv_main.c:5844 */
void SV_EmitPings(client_t *client, sizebuf_t *msg)
{
	int ping;
	int packet_loss;

	MSG_WriteByte(msg, svc_pings);
	MSG_StartBitWriting(msg);
	for (int i = 0; i < g_psvs.maxclients; i++)
	{
		client_t *cl = &g_psvs.clients[i];
		if (!cl->active)
			continue;

		SV_GetNetInfo(cl, &ping, &packet_loss);
		MSG_WriteBits(1, 1);
		MSG_WriteBits(i, 5);
		MSG_WriteBits(ping, 12);
		MSG_WriteBits(packet_loss, 7);
	}

	MSG_WriteBits(0, 1);
	MSG_EndBitWriting(msg);
}

/* <a947b> ../engine/sv_main.c:5878 */
void SV_WriteEntitiesToClient(client_t *client, sizebuf_t *msg)
{
	
	client_frame_t *frame = &client->frames[SV_UPDATE_MASK & client->netchan.outgoing_sequence];

	unsigned char *pvs = NULL;
	unsigned char *pas = NULL;
	gEntityInterface.pfnSetupVisibility((edict_t*)client->pViewEntity, client->edict, &pvs, &pas);
	unsigned char *pSet = pvs;

	packet_entities_t *pack = &frame->entities;

	// for REHLDS_OPT_PEDANTIC: Allocate the MAX_PACKET_ENTITIES ents in the frame's storage 
	// This allows us to avoid intermediate 'fullpack' storage
#ifdef REHLDS_OPT_PEDANTIC
	SV_AllocPacketEntities(frame, MAX_PACKET_ENTITIES);
	packet_entities_t *curPack = &frame->entities;
	curPack->num_entities = 0;
#else
	SV_ClearPacketEntities(frame);
	full_packet_entities_t fullpack;
	fullpack.num_entities = 0;
	full_packet_entities_t* curPack = &fullpack;
#endif // REHLDS_OPT_PEDANTIC
	
	qboolean sendping = SV_ShouldUpdatePing(client);
	int flags = client->lw != 0;

	int e;
	for (e = 1; e <= g_psvs.maxclients; e++)
	{
		client_t *cl = &g_psvs.clients[e - 1];
		if( ( !cl->active && !cl->spawned ) || cl->proxy )
			continue;

		qboolean add = gEntityInterface.pfnAddToFullPack(&curPack->entities[curPack->num_entities], e, &g_psv.edicts[e], host_client->edict, flags, TRUE, pSet);
		if (add)
			++curPack->num_entities;
	}

	for (; e < g_psv.num_edicts; e++)
	{
		if (curPack->num_entities >= MAX_PACKET_ENTITIES)
		{
			Con_DPrintf("Too many entities in visible packet list.\n");
			break;
		}

		edict_t* ent = &g_psv.edicts[e];

#ifdef REHLDS_OPT_PEDANTIC
		//Part of gamedll's code is moved here to decrease amount of calls to AddToFullPack()
		//We don't even try to transmit entities without model as well as invisible entities
		if (ent->v.modelindex && !(ent->v.effects & EF_NODRAW)) {
			qboolean add = gEntityInterface.pfnAddToFullPack(&curPack->entities[curPack->num_entities], e, &g_psv.edicts[e], host_client->edict, flags, FALSE, pSet);
			if (add)
				++curPack->num_entities;
		}
#else
		qboolean add = gEntityInterface.pfnAddToFullPack(&curPack->entities[curPack->num_entities], e, &g_psv.edicts[e], host_client->edict, flags, FALSE, pSet);
		if (add)
			++curPack->num_entities;
#endif //REHLDS_OPT_PEDANTIC

	}

	//for REHLDS_FIXES: Entities are already in the frame's storage, no need to copy them
#ifndef REHLDS_OPT_PEDANTIC
	SV_AllocPacketEntities(frame, fullpack.num_entities);
	if (pack->num_entities)
		Q_memcpy(pack->entities, fullpack.entities, sizeof(entity_state_t) * pack->num_entities);
#endif

	SV_EmitPacketEntities(client, pack, msg);
	SV_EmitEvents(client, pack, msg);
	if (sendping)
		SV_EmitPings(client, msg);
}

/* <a968b> ../engine/sv_main.c:5981 */
void SV_CleanupEnts(void)
{
	for (int e = 1; e < g_psv.num_edicts; e++)
	{
		edict_t *ent = &g_psv.edicts[e];
		ent->v.effects &= ~(EF_NOINTERP | EF_MUZZLEFLASH);
	}
}

/* <a96d8> ../engine/sv_main.c:5999 */
qboolean SV_SendClientDatagram(client_t *client)
{
	unsigned char buf[MAX_DATAGRAM];
	sizebuf_t msg;

	msg.buffername = "Client Datagram";
	msg.data = buf;
	msg.maxsize = sizeof(buf);
	msg.cursize = 0;
	msg.flags = SIZEBUF_ALLOW_OVERFLOW;

	MSG_WriteByte(&msg, svc_time);
	MSG_WriteFloat(&msg, g_psv.time);

	SV_WriteClientdataToMessage(client, &msg);
	SV_WriteEntitiesToClient(client, &msg);

	if (client->datagram.flags & SIZEBUF_OVERFLOWED)
	{
		Con_Printf("WARNING: datagram overflowed for %s\n", client->name);
	}
	else
	{
#ifdef REHLDS_FIXES
		if (msg.cursize + client->datagram.cursize > msg.maxsize)
			Con_DPrintf("Warning: Ignoring unreliable datagram for %s, would overflow on msg\n", client->name);
		else
			SZ_Write(&msg, client->datagram.data, client->datagram.cursize);
#else
		SZ_Write(&msg, client->datagram.data, client->datagram.cursize);
#endif
	}

	SZ_Clear(&client->datagram);

	if (msg.flags & SIZEBUF_OVERFLOWED)
	{
		Con_Printf("WARNING: msg overflowed for %s\n", client->name);
		SZ_Clear(&msg);
	}

	Netchan_Transmit(&client->netchan, msg.cursize, buf);

	return TRUE;
}

/* <a981c> ../engine/sv_main.c:6062 */
void SV_UpdateToReliableMessages(void)
{
	int i;
	client_t *client;

	// Prepare setinfo changes and send new user messages
	for (i = 0; i < g_psvs.maxclients; i++)
	{
		client = &g_psvs.clients[i];

		if (!client->edict)
			continue;

		host_client = client;

#ifdef REHLDS_FIXES
		// skip update in this frame if would overflow
		if (client->sendinfo && client->sendinfo_time <= realtime && ( 1 + 1 + 4 + ( int )strlen( client->userinfo ) + 1 + 16 + g_psv.reliable_datagram.cursize <= g_psv.reliable_datagram.maxsize ) )
#else // REHLDS_FIXES
		if (client->sendinfo && client->sendinfo_time <= realtime)
#endif // REHLDS_FIXES
		{
			client->sendinfo = FALSE;
			client->sendinfo_time = realtime + 1.0;
			SV_ExtractFromUserinfo(client);
			SV_FullClientUpdate(client, &g_psv.reliable_datagram);
		}

		if (!client->fakeclient && (client->active || client->connected))
		{
			if (sv_gpNewUserMsgs != NULL)
			{
				SV_SendUserReg(&client->netchan.message);
			}
		}
	}

	// Link new user messages to sent chain
	if (sv_gpNewUserMsgs != NULL)
	{
		UserMsg *pMsg = sv_gpUserMsgs;
		if (pMsg != NULL)
		{
			while (pMsg->next)
			{
				pMsg = pMsg->next;
			}
			pMsg->next = sv_gpNewUserMsgs;
		}
		else
		{
			sv_gpUserMsgs = sv_gpNewUserMsgs;
		}
		sv_gpNewUserMsgs = NULL;
	}

	if (g_psv.datagram.flags & SIZEBUF_OVERFLOWED)
	{
		Con_DPrintf("sv.datagram overflowed!\n");
		SZ_Clear(&g_psv.datagram);
	}
	if (g_psv.spectator.flags & SIZEBUF_OVERFLOWED)
	{
		Con_DPrintf("sv.spectator overflowed!\n");
		SZ_Clear(&g_psv.spectator);
	}

	//Fix for the "server failed to transmit file 'AY&SY..." bug
	//https://github.com/dreamstalker/rehlds/issues/38
#ifdef REHLDS_FIXES
	bool svReliableCompressed = false;
#endif

	// Send broadcast data
	for (i = 0; i < g_psvs.maxclients; i++)
	{
		client = &g_psvs.clients[i];

		if (!client->fakeclient && client->active)
		{
			//Fix for the "server failed to transmit file 'AY&SY..." bug
			//https://github.com/dreamstalker/rehlds/issues/38
#ifdef REHLDS_FIXES
			if (!svReliableCompressed && g_psv.reliable_datagram.cursize + client->netchan.message.cursize < client->netchan.message.maxsize)
			{
				SZ_Write(&client->netchan.message, g_psv.reliable_datagram.data, g_psv.reliable_datagram.cursize);
			}
			else
			{
				Netchan_CreateFragments(TRUE, &client->netchan, &g_psv.reliable_datagram);
				svReliableCompressed = true;
			}
#else
			if (g_psv.reliable_datagram.cursize + client->netchan.message.cursize < client->netchan.message.maxsize)
			{
				SZ_Write(&client->netchan.message, g_psv.reliable_datagram.data, g_psv.reliable_datagram.cursize);
			}
			else
			{
				Netchan_CreateFragments(TRUE, &client->netchan, &g_psv.reliable_datagram);
			}
#endif

			if (g_psv.datagram.cursize + client->datagram.cursize < client->datagram.maxsize)
			{
				SZ_Write(&client->datagram, g_psv.datagram.data, g_psv.datagram.cursize);
			}
			else
			{
				Con_DPrintf("Warning:  Ignoring unreliable datagram for %s, would overflow\n", client->name);
			}

			if (client->proxy)
			{
				if (g_psv.spectator.cursize + client->datagram.cursize < client->datagram.maxsize)
				{
					SZ_Write(&client->datagram, g_psv.spectator.data, g_psv.spectator.cursize);
				}
#ifdef REHLDS_FIXES
				else
				{
					Con_DPrintf("Warning:  Ignoring spectator datagram for %s, would overflow\n", client->name);
				}
#endif
			}
		}
	}

	SZ_Clear(&g_psv.reliable_datagram);
	SZ_Clear(&g_psv.datagram);
	SZ_Clear(&g_psv.spectator);
}

/* <a9736> ../engine/sv_main.c:6184 */
void SV_SkipUpdates(void)
{
	for (int i = 0; i < g_psvs.maxclients; i++)
	{
		client_t *client = &g_psvs.clients[i];
		if (!client->active && !client->connected && !client->spawned)
			continue;

		if (!host_client->fakeclient) //TODO: should be client, not host_client; investigation needed
			client->skip_message = TRUE;
	}
}

/* <a98aa> ../engine/sv_main.c:6204 */
void SV_SendClientMessages(void)
{
	SV_UpdateToReliableMessages();

	for (int i = 0; i < g_psvs.maxclients; i++)
	{
		client_t *cl = &g_psvs.clients[i];
		host_client = cl;

		if ((!cl->active && !cl->connected && !cl->spawned) || cl->fakeclient)
			continue;

		if (cl->skip_message)
		{
			cl->skip_message = FALSE;
			continue;
		}

		if (host_limitlocal.value == 0.0f && cl->netchan.remote_address.type == NA_LOOPBACK)
			cl->send_message = TRUE;

		if (cl->active && cl->spawned && cl->fully_connected && host_frametime + realtime >= cl->next_messagetime)
			cl->send_message = TRUE;

		if (cl->netchan.message.flags & SIZEBUF_OVERFLOWED)
		{
			SZ_Clear(&cl->netchan.message);
			SZ_Clear(&cl->datagram);
			SV_BroadcastPrintf("%s overflowed\n", cl->name);
			Con_Printf("WARNING: reliable overflow for %s\n", cl->name);
			SV_DropClient(cl, FALSE, "Reliable channel overflowed");
			cl->send_message = TRUE;
			cl->netchan.cleartime = 0;
		}
		else if (cl->send_message)
		{
			if (sv_failuretime.value < realtime - cl->netchan.last_received)
				cl->send_message = FALSE;
		}
		
		if (cl->send_message)
		{
			if (!Netchan_CanPacket(&cl->netchan))
			{
				++cl->chokecount;
				continue;
			}

			host_client->send_message = FALSE;
			cl->next_messagetime = host_frametime + cl->next_messageinterval + realtime;
			if (cl->active && cl->spawned && cl->fully_connected)
				SV_SendClientDatagram(cl);
			else
				Netchan_Transmit(&cl->netchan, 0, NULL);
		}
	}
	SV_CleanupEnts();
}

/* <a976e> ../engine/sv_main.c:6307 */
void SV_ExtractFromUserinfo(client_t *cl)
{
	const char *val;
	int i;
	char newname[MAX_NAME];
	char rawname[MAX_NAME];

	char *userinfo = cl->userinfo;

	val = Info_ValueForKey(userinfo, "name");
	Q_strncpy(rawname, val, sizeof(rawname) - 1);
	rawname[sizeof(rawname) - 1] = 0;

	for (char *p = rawname; *p; p++)
	{
		if (*p == '%' || *p == '&')
			*p = ' ';
	}

#ifdef REHLDS_FIXES
	Q_strcpy(newname, rawname);
	Q_StripUnprintableAndSpace(newname);
#else // REHLDS_FIXES
	TrimSpace(rawname, newname);
#endif // REHLDS_FIXES

	if (!Q_UnicodeValidate(newname))
	{
		Q_UnicodeRepair(newname);
	}

	// Fix name to not start with '#', so it will not resemble userid
	for (char *p = rawname; *p == '#'; p++) *p = ' ';

	if (newname[0] == 0 || !Q_stricmp(newname, "console")
#ifdef REHLDS_FIXES
		|| Q_strstr(newname, "..") != NULL)
#else // REHLDS_FIXES
		)
#endif // REHLDS_FIXES
	{
		Info_SetValueForKey(userinfo, "name", "unnamed", MAX_INFO_STRING);
	}
	else if (Q_strcmp(val, newname))
	{
		Info_SetValueForKey(userinfo, "name", newname, MAX_INFO_STRING);
	}

	// Check for duplicate names
	SV_CheckForDuplicateNames(userinfo, TRUE, cl - g_psvs.clients);

	gEntityInterface.pfnClientUserInfoChanged(cl->edict, userinfo);

	val = Info_ValueForKey(userinfo, "name");
	Q_strncpy(cl->name, val, sizeof(cl->name) - 1);
	cl->name[sizeof(cl->name) - 1] = 0;

	ISteamGameServer_BUpdateUserData(cl->network_userid.m_SteamID, cl->name, 0);

	val = Info_ValueForKey(userinfo, "rate");
	if (val[0] != 0)
	{
		i = Q_atoi(val);
		cl->netchan.rate = clamp(i, MIN_RATE, MAX_RATE);
	}

	val = Info_ValueForKey(userinfo, "topcolor");
	if (val[0] != 0)
		cl->topcolor = Q_atoi(val);
	else
		Con_DPrintf("topcolor unchanged for %s\n", cl->name);

	val = Info_ValueForKey(userinfo, "bottomcolor");
	if (val[0] != 0)
		cl->bottomcolor = Q_atoi(val);
	else
		Con_DPrintf("bottomcolor unchanged for %s\n", cl->name);

	val = Info_ValueForKey(userinfo, "cl_updaterate");
	if (val[0] != 0)
	{
		i = Q_atoi(val);
		if (i >= 10)
			cl->next_messageinterval = 1.0 / i;
		else
			cl->next_messageinterval = 0.1;
	}

	val = Info_ValueForKey(userinfo, "cl_lw");
	cl->lw = val[0] != 0 ? Q_atoi(val) != 0 : 0;

	val = Info_ValueForKey(userinfo, "cl_lc");
	cl->lc = val[0] != 0 ? Q_atoi(val) != 0 : 0;

	val = Info_ValueForKey(userinfo, "*hltv");
	cl->proxy = val[0] != 0 ? Q_atoi(val) == 1 : 0;

	SV_CheckUpdateRate(&cl->next_messageinterval);
	SV_CheckRate(cl);
}

/* <a9900> ../engine/sv_main.c:6507 */
int SV_ModelIndex(const char *name)
{
	if (!name || !name[0])
		return 0;

#ifdef REHLDS_OPT_PEDANTIC
	auto node = g_rehlds_sv.modelsMap.get(name);
	if (node) {
		return node->val;
	}
#else
	for (int i = 0; i < HL_MODEL_MAX; i++)
	{
		if (!g_psv.model_precache[i])
			break;

		if (!Q_stricmp(g_psv.model_precache[i], name))
			return i;
	};
#endif

	Sys_Error("SV_ModelIndex: model %s not precached", name);
}

/* <a9992> ../engine/sv_main.c:6529 */
void SV_AddResource(resourcetype_t type, const char *name, int size, unsigned char flags, int index)
{
	resource_t *r;
	if (g_psv.num_resources >= 1280)
		Sys_Error("Too many resources on server.");

	r = &g_psv.resourcelist[g_psv.num_resources++];
	r->type = type;
	Q_strncpy(r->szFileName, name, sizeof(r->szFileName) - 1);
	r->szFileName[sizeof(r->szFileName) - 1] = 0;
	r->ucFlags = flags;
	r->nDownloadSize = size;
	r->nIndex = index;
}

/* <a99d9> ../engine/sv_main.c:6557 */
void SV_CreateGenericResources(void)
{
	char filename[MAX_PATH];
	char *buffer;
	char *data;

	COM_StripExtension(g_psv.modelname, filename);
	COM_DefaultExtension(filename, ".res");
	COM_FixSlashes(filename);

	buffer = (char *)COM_LoadFile(filename, 5, NULL);
	if (buffer == NULL)
		return;

	data = buffer;
	Con_DPrintf("Precaching from %s\n", filename);
	Con_DPrintf("----------------------------------\n");
	g_psv.num_generic_names = 0;

	while (1)
	{
		data = COM_Parse(data);
		if (Q_strlen(com_token) <= 0)
			break;

		if (Q_strstr(com_token, ".."))
			Con_Printf("Can't precache resource with invalid relative path %s\n", com_token);
		else if (Q_strstr(com_token, ":"))
			Con_Printf("Can't precache resource with absolute path %s\n", com_token);
		else if (Q_strstr(com_token, "\\"))
			Con_Printf("Can't precache resource with invalid relative path %s\n", com_token);
		else if (Q_strstr(com_token, ".cfg"))
			Con_Printf("Can't precache .cfg files:  %s\n", com_token);
		else if (Q_strstr(com_token, ".lst"))
			Con_Printf("Can't precache .lst files:  %s\n", com_token);
		else if (Q_strstr(com_token, ".exe"))
			Con_Printf("Can't precache .exe files:  %s\n", com_token);
		else if (Q_strstr(com_token, ".vbs"))
			Con_Printf("Can't precache .vbs files:  %s\n", com_token);
		else if (Q_strstr(com_token, ".com"))
			Con_Printf("Can't precache .com files:  %s\n", com_token);
		else if (Q_strstr(com_token, ".bat"))
			Con_Printf("Can't precache .bat files:  %s\n", com_token);
		else if (Q_strstr(com_token, ".dll"))
			Con_Printf("Can't precache .dll files:  %s\n", com_token);
		else
		{
			Q_strncpy(g_psv.generic_precache_names[g_psv.num_generic_names], com_token, sizeof(g_psv.generic_precache_names[g_psv.num_generic_names]) - 1);
			g_psv.generic_precache_names[g_psv.num_generic_names][sizeof(g_psv.generic_precache_names[g_psv.num_generic_names]) - 1] = 0;
			PF_precache_generic_I(g_psv.generic_precache_names[g_psv.num_generic_names]);
			Con_DPrintf("  %s\n", g_psv.generic_precache_names[g_psv.num_generic_names++]);
		}
	}
	Con_DPrintf("----------------------------------\n");
	COM_FreeFile(buffer);
}

/* <a9a24> ../engine/sv_main.c:6675 */
void SV_CreateResourceList(void)
{
	char const ** s;
	int ffirstsent = 0;
	int i;
	int nSize;
	event_t *ep;

	g_psv.num_resources = 0;
#ifdef REHLDS_CHECKS
	for (i = 1, s = &g_psv.generic_precache[1]; i < HL_GENERIC_MAX && *s != NULL; i++, s++)
#else // REHLDS_CHECKS
	for (i = 1, s = &g_psv.generic_precache[1]; *s != NULL; i++, s++)
#endif // REHLDS_CHECKS
	{
		if (g_psvs.maxclients > 1)
			nSize = FS_FileSize(*s);
		else
			nSize = 0;

		SV_AddResource(t_generic, *s, nSize, 1, i);
	}
#ifdef REHLDS_CHECKS
	for (i = 1, s = &g_psv.sound_precache[1]; i < HL_SOUND_MAX && *s != NULL; i++, s++)
#else // REHLDS_CHECKS
	for (i = 1, s = &g_psv.sound_precache[1]; *s != NULL; i++, s++)
#endif // REHLDS_CHECKS
	{
		if (**s == '!')
		{
			if (!ffirstsent)
			{
				ffirstsent = 1;
				SV_AddResource(t_sound, "!", 0, 1, i);
			}
		}
		else
		{
			nSize = 0;
			if (g_psvs.maxclients > 1)
				nSize = FS_FileSize(va("sound/%s", *s));
			SV_AddResource(t_sound, *s, nSize, 0, i);
		}
	}
#ifdef REHLDS_CHECKS
	for (i = 1, s = &g_psv.model_precache[1]; i < HL_MODEL_MAX && *s != NULL; i++, s++)
#else // REHLDS_CHECKS
	for (i = 1, s = &g_psv.model_precache[1]; *s != NULL; i++, s++)
#endif // REHLDS_CHECKS
	{
		if (g_psvs.maxclients > 1 && **s != '*')
			nSize = FS_FileSize(*s);
		else
			nSize = 0;

		SV_AddResource(t_model, *s, nSize, g_psv.model_precache_flags[i], i);
	}
	for (i = 0; i < sv_decalnamecount; i++)
		SV_AddResource(t_decal, sv_decalnames[i].name, Draw_DecalSize(i), 0, i);

	for (i = 1; i < HL_EVENT_MAX; i++)
	{
		ep = &g_psv.event_precache[i];
		if (!ep->filename)
			break;

		SV_AddResource(t_eventscript, (char *)ep->filename, ep->filesize, 1, i);
	}
}

/* <a9c59> ../engine/sv_main.c:6766 */
void SV_ClearCaches(void)
{
	int i;
	event_t *ep;
	for (i = 1; i < HL_EVENT_MAX; i++)
	{
		ep = &g_psv.event_precache[i];
		if (ep->filename == NULL)
			break;

		ep->filename = NULL;
		if (ep->pszScript)
			Mem_Free((void *)ep->pszScript);
		ep->pszScript = NULL;
	}
}

/* <a9c82> ../engine/sv_main.c:6790 */
// Sends customizations from all active players to the current player.
void SV_PropagateCustomizations(void)
{
	client_t *pHost;
	customization_t *pCust;
	resource_t *pResource;
	int i;

	// For each active player
	for (i = 0, pHost = g_psvs.clients; i < g_psvs.maxclients; i++, pHost++)
	{
		if (!pHost->active && !pHost->spawned || pHost->fakeclient)
			continue;

		// Send each customization to current player
		pCust = pHost->customdata.pNext;
		while (pCust != NULL)
		{
			if (pCust->bInUse)
			{
				pResource = &pCust->resource;
				MSG_WriteByte(&host_client->netchan.message, svc_customization);
				MSG_WriteByte(&host_client->netchan.message, i);
				MSG_WriteByte(&host_client->netchan.message, pResource->type);
				MSG_WriteString(&host_client->netchan.message, pResource->szFileName);
				MSG_WriteShort(&host_client->netchan.message, pResource->nIndex);
				MSG_WriteLong(&host_client->netchan.message, pResource->nDownloadSize);
				MSG_WriteByte(&host_client->netchan.message, pResource->ucFlags);
				if (pResource->ucFlags & RES_CUSTOM)
				{
					SZ_Write(&host_client->netchan.message, pResource->rgucMD5_hash, 16);
				}
			}

			pCust = pCust->pNext;
		}
	}
}

/* <a9cdb> ../engine/sv_main.c:6850 */
void SV_WriteVoiceCodec(sizebuf_t *pBuf)
{
	MSG_WriteByte(pBuf, svc_voiceinit);
	MSG_WriteString(pBuf, "");
	MSG_WriteByte(pBuf, 0);
}

/*
 * Interface between engine and gamedll has a flaw which can lead to inconsistent behavior when passing arguments of type vec3_t to gamedll
 * Consider function func(vec3_t v) defined in gamedll. vec3_t defined in gamedll as a class (not array), therefore it's expected that all vector components (12 bytes) will be written in the stack,
 * i.e. the function signature may be represented as func(float v_0, float v_1, float v_2).
 * In the engine, on the other hand, vec3_t is an array of vec_t (vec_t[3]). C/C++ compiler treats arguments of array type as pointers to array's first element, thus, on attempt to
 * invoke gamedll's func(vec3_t v) from engine, only pointer to first vector's element will be passed in stack, while gamedll expects all 3 vector elements.
 * This inconsistency in the interface between gamedll and engine leads to exposure of some data from stack of caller function to vector's elements in gamedll, which, in turn,
 * leads to inconsistent behavior (since stack data may contain pointers) across different systems
 *
 * This functions emulates swds.dll behavior, i.e. it sends the same garbage when invoking CreateBaseline as swds.dll does.
 * This is required since not emulating this behavior will break rehlds test demos
 */
void __invokeValvesBuggedCreateBaseline(void* func, int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec_t* pmins, vec_t* pmaxs)
{
	__asm {
		mov ecx, func
		push 0
		push 1
		push 0
		push 0
		push pmaxs
		push pmins
		push playermodelindex
		push entity
		push baseline
		push eindex
		push player
		call ecx
		add esp, 0x2C
	}
}

/* <a9cf9> ../engine/sv_main.c:6866 */
void SV_CreateBaseline(void)
{
	edict_t *svent;
	int entnum;
	qboolean player;
	qboolean custom;
	entity_state_t nullstate;
	delta_t *pDelta;

	g_psv.instance_baselines = &g_sv_instance_baselines;
	Q_memset(&nullstate, 0, sizeof(entity_state_t));
	SV_FindModelNumbers();

	for (entnum = 0; entnum < g_psv.num_edicts; entnum++)
	{
		svent = &g_psv.edicts[entnum];
		if (!svent->free)
		{
			if (g_psvs.maxclients >= entnum || svent->v.modelindex)
			{
				player = SV_IsPlayerIndex(entnum);
				g_psv.baselines[entnum].number = entnum;
				g_psv.baselines[entnum].entityType = 1;

				if (svent->v.flags & FL_CUSTOMENTITY)
					g_psv.baselines[entnum].entityType = 2;

				__invokeValvesBuggedCreateBaseline((void *)gEntityInterface.pfnCreateBaseline, player, entnum, &(g_psv.baselines[entnum]), svent, sv_playermodel, player_mins[0], player_maxs[0]);
				sv_lastnum = entnum;
			}
		}
	}
	gEntityInterface.pfnCreateInstancedBaselines();
	MSG_WriteByte(&g_psv.signon, svc_spawnbaseline);
	MSG_StartBitWriting(&g_psv.signon);
	for (entnum = 0; entnum < g_psv.num_edicts; entnum++)
	{
		svent = &g_psv.edicts[entnum];
		if (!svent->free && (g_psvs.maxclients >= entnum || svent->v.modelindex))
		{
			MSG_WriteBits(entnum, 11);
			MSG_WriteBits(g_psv.baselines[entnum].entityType, 2);
			custom = ~g_psv.baselines[entnum].entityType & 1;
			if (custom)
				pDelta = g_pcustomentitydelta;
			else
			{
				pDelta = g_pplayerdelta;
				if (!SV_IsPlayerIndex(entnum))
					pDelta = g_pentitydelta;
			}

			DELTA_WriteDelta((byte *)&nullstate, (byte *)&(g_psv.baselines[entnum]), TRUE, pDelta, NULL);
		}
	}

	MSG_WriteBits(0xFFFF, 16);
	MSG_WriteBits(g_psv.instance_baselines->number, 6);
	for (entnum = 0; entnum < g_psv.instance_baselines->number; entnum++)
		DELTA_WriteDelta((byte *)&nullstate, (byte *)&(g_psv.instance_baselines->baseline[entnum]), TRUE, g_pentitydelta, NULL);

	MSG_EndBitWriting(&g_psv.signon);
}

/* <a9da2> ../engine/sv_main.c:6969 */
void SV_BroadcastCommand(char *fmt, ...)
{
	va_list argptr;
	char string[1024];
	char data[128];
	sizebuf_t msg;

	if (!g_psv.active)
		return;

	va_start(argptr, fmt);
	msg.data = (byte *)data;
	msg.buffername = "Broadcast Command";
	msg.cursize = 0;
	msg.maxsize = sizeof(data);
	msg.flags = SIZEBUF_ALLOW_OVERFLOW;

	Q_vsnprintf(string, sizeof(string), fmt, argptr);
	va_end(argptr);

	MSG_WriteByte(&msg, svc_stufftext);
	MSG_WriteString(&msg, string);
	if (msg.flags & SIZEBUF_OVERFLOWED)
		Sys_Error("SV_BroadcastCommand:  Overflowed on %s, %i is max size\n", string, msg.maxsize);

	for (int i = 0; i < g_psvs.maxclients; ++i)
	{
		client_t *cl = &g_psvs.clients[i];
		if (cl->active || cl->connected || (cl->spawned && !cl->fakeclient))
		{
			SZ_Write(&cl->netchan.message, msg.data, msg.cursize);
		}
	}
}

/* <a9e84> ../engine/sv_main.c:7017 */
void SV_BuildReconnect(sizebuf_t *msg)
{
	MSG_WriteByte(msg, svc_stufftext);
	MSG_WriteString(msg, "reconnect\n");
}

/* <a9ea2> ../engine/sv_main.c:7032 */
NOXREF void SV_ReconnectAllClients(void)
{
	int i;
	char message[34];
	Q_snprintf(message, sizeof(message), "Server updating Security Module.\n");

	for (i = 0; i < g_psvs.maxclients; i++)
	{
		client_t *client = &g_psvs.clients[i];

		if ((client->active || client->connected) && !client->fakeclient)
		{
			Netchan_Clear(&client->netchan);

			MSG_WriteByte(&client->netchan.message, svc_print);
			MSG_WriteString(&client->netchan.message, message);

			MSG_WriteByte(&client->netchan.message, svc_stufftext);
			MSG_WriteString(&client->netchan.message, "retry\n");

			SV_DropClient(client, FALSE, message);
		}
	}

}

/* <a9eeb> ../engine/sv_main.c:7068 */
void SetCStrikeFlags(void)
{
	if (!g_bCS_CZ_Flags_Initialized)	// TODO: Convert these to enum
	{
		if (!Q_stricmp(com_gamedir, "valve"))
		{
			g_bIsHL1 = 1;
		}
		else if (!Q_stricmp(com_gamedir, "cstrike") || !Q_stricmp(com_gamedir, "cstrike_beta"))
		{
			g_bIsCStrike = 1;
		}
		else if (!Q_stricmp(com_gamedir, "czero"))
		{
			g_bIsCZero = 1;
		}
		else if (!Q_stricmp(com_gamedir, "czeror"))
		{
			g_bIsCZeroRitual = 1;
		}
		else if (!Q_stricmp(com_gamedir, "terror"))
		{
			g_bIsTerrorStrike = 1;
		}
		else if (!Q_stricmp(com_gamedir, "tfc"))
		{
			g_bIsTFC = 1;
		}

		g_bCS_CZ_Flags_Initialized = 1;
	}
}

/* <a9f01> ../engine/sv_main.c:7107 */
void SV_ActivateServer(int runPhysics)
{
	int i;
	unsigned char data[NET_MAX_PAYLOAD];
	sizebuf_t msg;
	client_t *cl;
	UserMsg *pTemp;
	char szCommand[256];

	Q_memset(&msg, 0, sizeof(sizebuf_t));
	msg.buffername = "Activate Server";
	msg.data = data;
	msg.maxsize = sizeof(data);
	msg.cursize = 0;
	msg.flags = SIZEBUF_CHECK_OVERFLOW;

	SetCStrikeFlags();
	Cvar_Set("sv_newunit", "0");

	ContinueLoadingProgressBar("Server", 8, 0.0f);
	gEntityInterface.pfnServerActivate(g_psv.edicts, g_psv.num_edicts, g_psvs.maxclients);
	Steam_Activate();
	ContinueLoadingProgressBar("Server", 9, 0.0f);
	SV_CreateGenericResources();
	g_psv.active = TRUE;
	g_psv.state = ss_active;
	ContinueLoadingProgressBar("Server", 10, 0.0f);

	if (!runPhysics)
	{
		host_frametime = 0.001;
		SV_Physics();
	}
	else
	{
		if (g_psvs.maxclients <= 1)
		{
			host_frametime = 0.1;
			SV_Physics();
			SV_Physics();
		}
		else
		{
			host_frametime = 0.8;
			for (i = 0; i < 16; i++)
				SV_Physics();
		}
	}
	SV_CreateBaseline();
	SV_CreateResourceList();
	g_psv.num_consistency = SV_TransferConsistencyInfo();
	for (i = 0, cl = g_psvs.clients; i < g_psvs.maxclients; cl++, i++)
	{
		if (!cl->fakeclient && (cl->active || cl->connected))
		{
			Netchan_Clear(&cl->netchan);
			if (g_psvs.maxclients > 1)
			{
				SV_BuildReconnect(&cl->netchan.message);
				Netchan_Transmit(&cl->netchan, 0, NULL);
			}
			else
				SV_SendServerinfo(&msg, cl);

			if (sv_gpUserMsgs)
			{
				pTemp = sv_gpNewUserMsgs;
				sv_gpNewUserMsgs = sv_gpUserMsgs;
				SV_SendUserReg(&msg);
				sv_gpNewUserMsgs = pTemp;
			}
			cl->hasusrmsgs = TRUE;
			Netchan_CreateFragments(TRUE, &cl->netchan, &msg);
			Netchan_FragSend(&cl->netchan);
			SZ_Clear(&msg);
		}
	}
	HPAK_FlushHostQueue();
	if (g_psvs.maxclients <= 1)
		Con_DPrintf("Game Started\n");
	else
		Con_DPrintf("%i player server started\n",g_psvs.maxclients);
	Log_Printf("Started map \"%s\" (CRC \"%i\")\n", g_psv.name, g_psv.worldmapCRC);

	if (mapchangecfgfile.string && *mapchangecfgfile.string)
	{
		AlertMessage(at_console, "Executing map change config file\n");
		Q_sprintf(szCommand, "exec %s\n", mapchangecfgfile.string);
		Cbuf_AddText(szCommand);
	}
}

/* <a9fdc> ../engine/sv_main.c:7245 */
void SV_ServerShutdown(void)
{
	Steam_NotifyOfLevelChange();
	gGlobalVariables.time = g_psv.time;

	if (g_psvs.dll_initialized)
	{
		if (g_psv.active)
			gEntityInterface.pfnServerDeactivate();
	}
}
/* <a9ff2> ../engine/sv_main.c:7265 */
int SV_SpawnServer(qboolean bIsDemo, char *server, char *startspot)
{
	client_t *cl;
	edict_t *ent;
	int i;
	char *pszhost;
	char oldname[64];

	if (g_psv.active)
	{
		cl = g_psvs.clients;
		for (i = 0; i < g_psvs.maxclients; i++, cl++)
		{
			if (cl->active || cl->spawned || cl->connected)
			{
				ent = cl->edict;
				if (ent == NULL || ent->free)
					continue;

				if (ent->pvPrivateData)
					gEntityInterface.pfnClientDisconnect(ent);
				else
					Con_Printf("Skipping reconnect on %s, no pvPrivateData\n", cl->name);
			}
		}
	}
	if (g_bOutOfDateRestart)
	{
		g_bOutOfDateRestart = FALSE;
		Cmd_ExecuteString("quit\n", src_command);
	}

	Log_Open();
	Log_Printf("Loading map \"%s\"\n", server);
	Log_PrintServerVars();
	NET_Config((qboolean)(g_psvs.maxclients > 1));
	
	pszhost = Cvar_VariableString("hostname");
	if (pszhost && *pszhost == '\0')
	{
		if (gEntityInterface.pfnGetGameDescription != NULL)
			Cvar_Set("hostname", gEntityInterface.pfnGetGameDescription());
		else
			Cvar_Set("hostname", "Half-Life");
	}

	scr_centertime_off = 0.0f;
	if (startspot)
		Con_DPrintf("Spawn Server %s: [%s]\n", server, startspot);
	else
		Con_DPrintf("Spawn Server %s\n", server);

	g_LastScreenUpdateTime = 0.0f;
	g_psvs.spawncount = ++gHostSpawnCount;

	if (coop.value != 0.0f)
		Cvar_SetValue("deathmatch", 0.0f);

	current_skill = (int)(skill.value + 0.5f);
	if (current_skill < 0)
		current_skill = 0;
	else if (current_skill > 3)
		current_skill = 3;

	Cvar_SetValue("skill", current_skill);
	ContinueLoadingProgressBar("Server", 2, 0.0f);

	HPAK_CheckSize("custom");
	oldname[0] = 0;
	Q_strncpy(oldname, g_psv.name, sizeof(oldname) - 1);
	oldname[sizeof(oldname) - 1] = 0;
	Host_ClearMemory(FALSE);
	
	cl = g_psvs.clients;
	for (i = 0; i < g_psvs.maxclientslimit; i++, cl++)
		SV_ClearFrames(&cl->frames);

	SV_UPDATE_BACKUP = g_psvs.maxclients != 1 ? 64 : 8;
	SV_UPDATE_MASK = g_psvs.maxclients != 1 ? 63 : 7;

	SV_AllocClientFrames();
	Q_memset(&g_psv, 0, sizeof(server_t));

#ifdef REHLDS_OPT_PEDANTIC
	g_rehlds_sv.modelsMap.clear();
#endif

	Q_strncpy(g_psv.oldname, oldname, sizeof(oldname) - 1);
	g_psv.oldname[sizeof(oldname) - 1] = 0;
	Q_strncpy(g_psv.name, server, sizeof(g_psv.name) - 1);
	g_psv.name[sizeof(g_psv.name) - 1] = 0;

	if (startspot)
	{
		Q_strncpy(g_psv.startspot, startspot, sizeof(g_psv.startspot) - 1);
		g_psv.startspot[sizeof(g_psv.startspot) - 1] = 0;
	}
	else
		g_psv.startspot[0] = 0;

	pr_strings = gNullString;
	gGlobalVariables.pStringBase = gNullString;

	if (g_psvs.maxclients == 1)
		Cvar_SetValue("sv_clienttrace", 1.0);

	g_psv.max_edicts = COM_EntsForPlayerSlots(g_psvs.maxclients);

	SV_DeallocateDynamicData();
	SV_ReallocateDynamicData();

	gGlobalVariables.maxEntities = g_psv.max_edicts;

	g_psv.edicts = (edict_t *)Hunk_AllocName(sizeof(edict_t) * g_psv.max_edicts, "edicts");
	g_psv.baselines = (entity_state_s *)Hunk_AllocName(sizeof(entity_state_t) * g_psv.max_edicts, "baselines");

	g_psv.datagram.buffername = "Server Datagram";
	g_psv.datagram.data = g_psv.datagram_buf;
	g_psv.datagram.maxsize = sizeof(g_psv.datagram_buf);
	g_psv.datagram.cursize = 0;

	g_psv.reliable_datagram.buffername = "Server Reliable Datagram";
	g_psv.reliable_datagram.data = g_psv.reliable_datagram_buf;
	g_psv.reliable_datagram.maxsize = sizeof(g_psv.reliable_datagram_buf);
	g_psv.reliable_datagram.cursize = 0;

	g_psv.spectator.buffername = "Server Spectator Buffer";
	g_psv.spectator.data = g_psv.spectator_buf;
	g_psv.spectator.maxsize = sizeof(g_psv.spectator_buf);

	g_psv.multicast.buffername = "Server Multicast Buffer";
	g_psv.multicast.data = g_psv.multicast_buf;
	g_psv.multicast.maxsize = sizeof(g_psv.multicast_buf);

	g_psv.signon.buffername = "Server Signon Buffer";
	g_psv.signon.data = g_psv.signon_data;
	g_psv.signon.maxsize = sizeof(g_psv.signon_data);
	g_psv.signon.cursize = 0;
	
	g_psv.num_edicts = g_psvs.maxclients + 1;

	cl = g_psvs.clients;
	for (i = 1; i < g_psvs.maxclients; i++, cl++)
		cl->edict = &g_psv.edicts[i];

	gGlobalVariables.maxClients = g_psvs.maxclients;
	g_psv.time = 1.0f;
	g_psv.state = ss_loading;
	g_psv.paused = FALSE;
	gGlobalVariables.time = 1.0f;

	R_ForceCVars((qboolean)(g_psvs.maxclients > 1));
	ContinueLoadingProgressBar("Server", 3, 0.0f);

	Q_snprintf(g_psv.modelname, sizeof(g_psv.modelname), "maps/%s.bsp", server);
	g_psv.worldmodel = Mod_ForName(g_psv.modelname, FALSE, FALSE);

	if (!g_psv.worldmodel)
	{
		Con_Printf("Couldn't spawn server %s\n", g_psv.modelname);
		g_psv.active = FALSE;
		return 0;
	}

	Sequence_OnLevelLoad(server);
	ContinueLoadingProgressBar("Server", 4, 0.0);
	if (gmodinfo.clientDllCRC)
	{
		char szDllName[64];
		Q_snprintf(szDllName, sizeof(szDllName), "cl_dlls//client.dll");
		COM_FixSlashes(szDllName);
		if (!MD5_Hash_File(g_psv.clientdllmd5, szDllName, FALSE, FALSE, NULL))
		{
			Con_Printf("Couldn't CRC client side dll:  %s\n", szDllName);
			g_psv.active = FALSE;
			return 0;
		}
	}
	ContinueLoadingProgressBar("Server", 6, 0.0);

	if (g_psvs.maxclients <= 1)
		g_psv.worldmapCRC = 0;
	else
	{
		CRC32_Init(&g_psv.worldmapCRC);
		if (!CRC_MapFile(&g_psv.worldmapCRC, g_psv.modelname))
		{
			Con_Printf("Couldn't CRC server map:  %s\n", g_psv.modelname);
			g_psv.active = FALSE;
			return 0;
		}
		CM_CalcPAS(g_psv.worldmodel);
	}

	g_psv.models[1] = g_psv.worldmodel;
	SV_ClearWorld();
	g_psv.model_precache_flags[1] |= RES_FATALIFMISSING;
	g_psv.model_precache[1] = g_psv.modelname;

#ifdef REHLDS_OPT_PEDANTIC
	{
		int __itmp = 1;
		g_rehlds_sv.modelsMap.put(ED_NewString(g_psv.modelname), __itmp);
	}
#endif

	g_psv.sound_precache[0] = pr_strings;
	g_psv.model_precache[0] = pr_strings;
	g_psv.generic_precache[0] = pr_strings;

	for (i = 1; i < g_psv.worldmodel->numsubmodels; i++)
	{
		g_psv.model_precache[i + 1] = localmodels[i];
		g_psv.models[i + 1] = Mod_ForName(localmodels[i], FALSE, FALSE);
		g_psv.model_precache_flags[i + 1] |= RES_FATALIFMISSING;

#ifdef REHLDS_OPT_PEDANTIC
		{
			int __itmp = i + 1;
			g_rehlds_sv.modelsMap.put(g_psv.model_precache[i + 1], __itmp);
		}
#endif
	}

	Q_memset(&g_psv.edicts->v, 0, sizeof(entvars_t));

	g_psv.edicts->free = FALSE;
	g_psv.edicts->v.modelindex = 1;
	g_psv.edicts->v.model = (size_t)g_psv.worldmodel - (size_t)pr_strings;
	g_psv.edicts->v.solid = SOLID_BSP;
	g_psv.edicts->v.movetype = MOVETYPE_PUSH;

	if (coop.value == 0.0f)
		gGlobalVariables.deathmatch_ = deathmatch.value;
	else
		gGlobalVariables.coop_ = coop.value;

	gGlobalVariables.serverflags = g_psvs.serverflags;
	gGlobalVariables.mapname = (size_t)g_psv.name - (size_t)pr_strings;
	gGlobalVariables.startspot = (size_t)g_psv.startspot - (size_t)pr_strings;
	allow_cheats = sv_cheats.value;
	SV_SetMoveVars();

	return 1;
}

/* <aa169> ../engine/sv_main.c:7586 */
void SV_LoadEntities(void)
{
	ED_LoadFromFile(g_psv.worldmodel->entities);
}

/* <aa17f> ../engine/sv_main.c:7592 */
void SV_ClearEntities(void)
{
	int i;
	edict_t *pEdict;

	for (i = 0; i < g_psv.num_edicts; i++)
	{
		pEdict = &g_psv.edicts[i];

		if (!pEdict->free)
			ReleaseEntityDLLFields(pEdict);
	}
}
/* <aa1be> ../engine/sv_main.c:7620 */
int EXT_FUNC RegUserMsg(const char *pszName, int iSize)
{
	if (giNextUserMsg > 255 || !pszName || Q_strlen(pszName) > 11 || iSize > 192)
		return 0;

	UserMsg *pUserMsgs = sv_gpUserMsgs;
	while (pUserMsgs)
	{
		if (!Q_strcmp(pszName, pUserMsgs->szName))
			return pUserMsgs->iMsg;

		pUserMsgs = pUserMsgs->next;
	}

	UserMsg *pNewMsg = (UserMsg *)Mem_ZeroMalloc(sizeof(UserMsg));
	pNewMsg->iMsg = giNextUserMsg++;
	pNewMsg->iSize = iSize;
	Q_strcpy(pNewMsg->szName, pszName);
	pNewMsg->next = sv_gpNewUserMsgs;
	sv_gpNewUserMsgs = pNewMsg;
	
	return pNewMsg->iMsg;
}

/* <aa23b> ../engine/sv_main.c:7717 */
qboolean StringToFilter(const char *s, ipfilter_t *f)
{
	char num[128];
	unsigned char b[4] = { 0, 0, 0, 0 };
	unsigned char m[4] = { 0, 0, 0, 0 };

	const char* cc = s;
	int i = 0;
	while (1)
	{
		if (*cc < '0' || *cc > '9')
			break;

		int j = 0;
#ifdef REHLDS_FIXES
		//check num for overflow
		while (*cc >= '0' && *cc <= '9' && j < sizeof(num))
			num[j++] = *(cc++);

		if (j >= sizeof(num)) 
			break;
#else // REHLDS_FIXES
		while (*cc >= '0' && *cc <= '9')
			num[j++] = *(cc++);
#endif // REHLDS_FIXES

		num[j] = 0;
		b[i] = Q_atoi(num);
		if (b[i])
			m[i] = -1;

		if (*cc)
		{
			++cc;
			++i;
			if (i < 4)
				continue;
		}
		f->mask = *(uint32 *)m;
		f->compare.u32 = *(uint32 *)b;
		return TRUE;
	}
	Con_Printf("Bad filter address: %s\n", cc);
	return FALSE;
}

/* <aa365> ../engine/sv_main.c:7765 */
USERID_t *SV_StringToUserID(const char *str)
{
	static USERID_t id;
	Q_memset(&id, 0, sizeof(id));

#ifdef REHLDS_FIXES
	if (!str || Q_strlen(str) < 7) //also check the length of identifier
		return &id;
#else
	if (!str || !*str)
		return &id;
#endif

	char szTemp[128];
	const char *pszUserID = str + 6;
	if (Q_strnicmp(str, "STEAM_", 6))
	{
		Q_strncpy(szTemp, pszUserID, sizeof(szTemp) - 1);
		id.idtype = 2;
	}
	else
	{
		Q_strncpy(szTemp, pszUserID, sizeof(szTemp) - 1);
		id.idtype = 1;
	}
	szTemp[127] = 0;
	id.m_SteamID = Steam_StringToSteamID(szTemp);

	return &id;
}

/* <ab410> ../engine/sv_main.c:7799 */
void SV_BanId_f(void)
{
	char szreason[256];
	char idstring[64];

	if (Cmd_Argc() < 3 || Cmd_Argc() > 8)
	{
		Con_Printf("Usage:  banid <minutes> <uniqueid or #userid> { kick }\n");
		Con_Printf("Use 0 minutes for permanent\n");
		return;
	}

	float banTime = Q_atof(Cmd_Argv(1));
	if (banTime < 0.01f)
		banTime = 0.0f;

	Q_strncpy(idstring, Cmd_Argv(2), sizeof(idstring));
	idstring[63] = 0;

	qboolean bKick;
	if (Cmd_Argc() > 3 && !Q_stricmp(Cmd_Argv(Cmd_Argc() - 1), "kick"))
		bKick = TRUE;
	else
		bKick = FALSE;

	USERID_t *id = NULL;
	if (idstring[0] == '#')
	{
		int search;
		if (Q_strlen(idstring) == 1)
		{
			if (Cmd_Argc() < 3)
			{
				Con_Printf("Insufficient arguments to banid\n");
				return;
			}
			search = Q_atoi(Cmd_Argv(3));
		}
		else
			search = Q_atoi(&idstring[1]);

		for (int i = 0; i < g_psvs.maxclients; i++)
		{
			client_t *cl = &g_psvs.clients[i];
			if ((!cl->active && !cl->connected && !cl->spawned) || cl->fakeclient)
				continue;

			if (cl->userid == search)
			{
				id = &cl->network_userid;
				break;
			}
		}
		if (id == NULL)
		{
			Con_Printf(__FUNCTION__ ":  Couldn't find #userid %u\n", search);
			return;
		}
	}
	else
	{
		if (!Q_strnicmp(idstring, "STEAM_", 6) || !Q_strnicmp(idstring, "VALVE_", 6))
		{
			Q_snprintf(idstring, sizeof(idstring) - 1, "%s:%s:%s", Cmd_Argv(2), Cmd_Argv(4), Cmd_Argv(6));
			idstring[63] = 0;
		}

		for (int i = 0; i < g_psvs.maxclients; i++)
		{
			client_t *cl = &g_psvs.clients[i];
			if (!cl->active && !cl->connected && !cl->spawned || cl->fakeclient)
				continue;

			if (!Q_stricmp(SV_GetClientIDString(cl), idstring))
			{
				id = &cl->network_userid;
				break;
			}
		}
		if (id == NULL)
			id = SV_StringToUserID(idstring);
		
		if (id == NULL)
		{
			Con_Printf(__FUNCTION__ ":  Couldn't resolve uniqueid %s.\n", idstring);
			Con_Printf("Usage:  banid <minutes> <uniqueid or #userid> { kick }\n");
			Con_Printf("Use 0 minutes for permanent\n");
			return;
		}
	}

	int i = 0;
	for (i = 0; i < numuserfilters; i++)
	{
		if (SV_CompareUserID(&userfilters[i].userid, id))
			break;
	}

	if (i >= numuserfilters)
	{
		if (numuserfilters >= 32768)
		{
			Con_Printf(__FUNCTION__ ":  User filter list is full\n");
			return;
		}
		numuserfilters++;
	}

	userfilters[i].banTime = banTime;
	userfilters[i].banEndTime = (banTime == 0.0f) ? 0.0f : banTime * 60.0f + realtime;

	Q_memcpy(&userfilters[i].userid, id, sizeof(USERID_t));

	// give 3-rd party plugins a chance to serialize ID
	g_RehldsHookchains.m_SerializeSteamId.callChain(NULL, &userfilters[i].userid);

	if (banTime == 0.0f)
		Q_sprintf(szreason, "permanently");
	else
		Q_snprintf(szreason, sizeof(szreason), "for %.2f minutes", banTime);

	const char *pszCmdGiver;
	if (cmd_source == src_command)
	{
#ifndef SWDS
		pszCmdGiver = (g_pcls.state != ca_dedicated) ? cv_name.string : "Console";
#else
		pszCmdGiver = "Console";
#endif // SWDS
	}
	else
		pszCmdGiver = host_client->name;

	if (!bKick)
	{
		if (sv_logbans.value != 0.0f)
			Log_Printf("Ban: \"<><%s><>\" was banned \"%s\" by \"%s\"\n", SV_GetIDString(id), szreason, pszCmdGiver);

		return;
	}

	client_t *save = host_client;
	for (int i = 0; i < g_psvs.maxclients; i++)
	{
		client_t *cl = &g_psvs.clients[i];
		if (!cl->active && !cl->connected && !cl->spawned || cl->fakeclient)
			continue;

		if (SV_CompareUserID(&cl->network_userid, id))
		{
			host_client = cl;
			if (sv_logbans.value != 0.0f)
			{
				Log_Printf(
					"Ban: \"%s<%i><%s><>\" was kicked and banned \"%s\" by \"%s\"\n",
					host_client->name, host_client->userid, SV_GetClientIDString(host_client), szreason, pszCmdGiver
					);
			}
			SV_ClientPrintf("You have been kicked and banned %s by the server op.\n", szreason);
			SV_DropClient(host_client, FALSE, "Kicked and banned");
			break;
		}
	}
	host_client = save;
}

/* <ab1ec> ../engine/sv_main.c:8040 */
void Host_Kick_f(void)
{
	const char *p;
	char idstring[64];
	int argsStartNum;

	qboolean isSteam = FALSE;
	qboolean found = FALSE;

	if (Cmd_Argc() <= 1)
	{
		Con_Printf("usage:  kick < name > | < # userid >\n");
		return;
	}

	if (cmd_source == src_command)
	{
		if (!g_psv.active)
		{
			Cmd_ForwardToServer();
			return;
		}
	}
	else
	{
		if (host_client->netchan.remote_address.type != NA_LOOPBACK)
		{
			SV_ClientPrintf("You can't 'kick' because you are not a server operator\n");
			return;
		}
	}

	client_t *save = host_client;
	p = Cmd_Argv(1);
	if (p && *p == '#')
	{
		int searchid;
		if (Cmd_Argc() <= 2 || p[1])
		{
			p++;
			argsStartNum = 2;
			searchid = Q_atoi(p);
		}
		else
		{
			searchid = Q_atoi(Cmd_Argv(2));
			p = Cmd_Argv(2);
			argsStartNum = 3;
		}

		Q_strncpy(idstring, p, 63);
		idstring[63] = 0;
		
		if (!Q_strnicmp(idstring, "STEAM_", 6) || !Q_strnicmp(idstring, "VALVE_", 6))
		{
			Q_snprintf(idstring, 63, "%s:%s:%s", p, Cmd_Argv(argsStartNum + 1), Cmd_Argv(argsStartNum + 3));

			argsStartNum += 4;
			idstring[63] = 0;
			isSteam = 1;
		}

		for (int i = 0; i < g_psvs.maxclients; i++)
		{
			host_client = &g_psvs.clients[i];
			if (!host_client->active && !host_client->connected)
				continue;

			if (searchid && host_client->userid == searchid)
			{
				found = TRUE;
				break;
			}

			if (Q_stricmp(SV_GetClientIDString(host_client), idstring) == 0)
			{
				found = TRUE;
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < g_psvs.maxclients; i++)
		{
			host_client = &g_psvs.clients[i];
			if (!host_client->active && !host_client->connected)
				continue;

			if (!Q_stricmp(host_client->name, Cmd_Argv(1)))
			{
				found = TRUE;
				break;
			}

		}
		argsStartNum = 2;
	}

	if (found)
	{
		const char *who;
		if (cmd_source == src_command)
		{
#ifndef SWDS
			who = (g_pcls.state != ca_dedicated) ? cv_name.string : "Console";
#else
			who = "Console";
#endif // SWDS
		}
		else
		{
			who = save->name;
		}

		if (host_client->netchan.remote_address.type == NA_LOOPBACK)
		{
			Con_Printf("The local player cannot be kicked!\n");
#ifdef REHLDS_FIXES
			host_client = save;
#endif // REHLDS_FIXES
			return;
		}

		if (Cmd_Argc() > argsStartNum)
		{
			unsigned int dataLen = 0;
			for (int i = 1; i < argsStartNum; i++)
			{
				dataLen += strlen(Cmd_Argv(i)) + 1;
			}

			if (isSteam)
				dataLen -= 4;

			p = Cmd_Args();
			if (dataLen <= strlen(p))
			{
				const char *message = dataLen + p;
				if (message)
				{
					SV_ClientPrintf("Kicked by %s: %s\n", who, message);
					Log_Printf(
						"Kick: \"%s<%i><%s><>\" was kicked by \"%s\" (message \"%s\")\n",
						host_client->name, host_client->userid, SV_GetClientIDString(host_client), who,	message
					);
					SV_DropClient(host_client, 0, va("Kicked :%s", message));
					host_client = save;
					return;
				}
			}
		}

		SV_ClientPrintf("Kicked by %s\n", who);
		Log_Printf("Kick: \"%s<%i><%s><>\" was kicked by \"%s\"\n", host_client->name, host_client->userid, SV_GetClientIDString(host_client), who);
		SV_DropClient(host_client, 0, "Kicked");
	}
	host_client = save;
}

/* <aafa6> ../engine/sv_main.c:8237 */
void SV_RemoveId_f(void)
{
	char idstring[64];

	if (Cmd_Argc() != 2 && Cmd_Argc() != 6)
	{
		Con_Printf("Usage:  removeid <uniqueid | #slotnumber>\n");
		return;
	}

	Q_strncpy(idstring, Cmd_Argv(1), sizeof(idstring) - 1);
	idstring[63] = 0;
	if (!idstring[0])
	{
		Con_Printf(__FUNCTION__ ":  Id string is empty!\n");
		return;
	}
	
	if (idstring[0] == '#')
	{
		int slot = Q_atoi(&idstring[1]);
		if (slot <= 0 || slot > numuserfilters)
		{
			Con_Printf(__FUNCTION__ ":  invalid slot #%i\n", slot);
			return;
		}
		slot--;

		USERID_t id;
		Q_memcpy(&id, &userfilters[slot].userid, sizeof(id));

		if (slot + 1 < numuserfilters)
			Q_memcpy(&userfilters[slot], &userfilters[slot + 1], (numuserfilters - (slot + 1)) * sizeof(userfilter_t));
		
		numuserfilters--;
		Con_Printf("UserID filter removed for %s, id %s\n", idstring, SV_GetIDString(&id));
	}
	else
	{
		if (!Q_strnicmp(idstring, "STEAM_", 6) || !Q_strnicmp(idstring, "VALVE_", 6))
		{
			_snprintf(idstring, 0x3Fu, "%s:%s:%s", Cmd_Argv(1), Cmd_Argv(3), Cmd_Argv(5));
			idstring[63] = 0;
		}
		
		for (int i = 0; i < numuserfilters; i++)
		{
			if (!Q_stricmp(SV_GetIDString(&userfilters[i].userid), idstring))
			{
				if (i + 1 < numuserfilters)
					Q_memcpy(&userfilters[i], &userfilters[i + 1], (numuserfilters - (i + 1)) * sizeof(userfilter_t));

				numuserfilters--;
				Con_Printf("UserID filter removed for %s\n", idstring);
				return;
			}
		}

		Con_Printf("removeid: couldn't find %s\n", idstring);
	}
}

/* <aaf4f> ../engine/sv_main.c:8320 */
void SV_WriteId_f(void)
{
	char name[MAX_PATH];
	_snprintf(name, MAX_PATH, "%s", bannedcfgfile.string);
	Con_Printf("Writing %s.\n", name);

	FILE *f = FS_Open(name, "wt");
	if (!f)
	{
		Con_Printf("Couldn't open %s\n", name);
		return;
	}

	for (int i = 0; i < numuserfilters; i++)
	{
		if (userfilters[i].banTime != 0.0f)
			continue;

		FS_FPrintf(f, "banid 0.0 %s\n", SV_GetIDString(&userfilters[i].userid));
	}

	FS_Close(f);
}

/* <aaf26> ../engine/sv_main.c:8358 */
void SV_ListId_f(void)
{
	if (numuserfilters <= 0)
	{
		Con_Printf("UserID filter list: empty\n");
		return;
	}
	
	Con_Printf("UserID filter list: %i entries\n", numuserfilters);
	for (int i = 0; i < numuserfilters; i++)
	{
		if (userfilters[i].banTime == 0.0f)
		{
			Con_Printf("%i %s : permanent\n", i, SV_GetIDString(&userfilters[i].userid));
		}
		else
		{
			Con_Printf("%i %s : %.3f min\n", i, SV_GetIDString(&userfilters[i].userid), userfilters[i].banTime);
		}
	}
}

/* <a5eab> ../engine/sv_main.c:8388 */
void SV_AddIP_f(void)
{
	if (Cmd_Argc() != 3)
	{
		Con_Printf("Usage: addip <minutes> <ipaddress>\nUse 0 minutes for permanent\n");
		return;
	}

	float banTime = Q_atof(Cmd_Argv(1));
	ipfilter_t tempFilter;
	if (!StringToFilter(Cmd_Argv(2), &tempFilter))
	{
		Con_Printf("Invalid IP address!\nUsage: addip <minutes> <ipaddress>\nUse 0 minutes for permanent\n");
		return;
	}

	int i = 0;
	for (; i < numipfilters; i++)
	{
		if (ipfilters[i].mask == tempFilter.mask && ipfilters[i].compare.u32 == tempFilter.compare.u32)
		{
			ipfilters[i].banTime = banTime;
			ipfilters[i].banEndTime = (banTime == 0.0f) ? 0.0f : banTime * 60.0f + realtime;
			return;
		}
	}

	if (numipfilters >= 32768)
	{
		Con_Printf("IP filter list is full\n");
		return;
	}
	
	++numipfilters;
	if (banTime < 0.0099999998f)
		banTime = 0.0f;

	ipfilters[i].banTime = banTime;
	ipfilters[i].compare = tempFilter.compare;
	ipfilters[i].banEndTime = (banTime == 0.0f) ? 0.0f : banTime * 60.0f + realtime;
	ipfilters[i].mask = tempFilter.mask;

	for (int i = 0; i < g_psvs.maxclients; i++)
	{
		host_client = &g_psvs.clients[i];
		if (!host_client->connected || !host_client->active || !host_client->spawned || host_client->fakeclient)
			continue;

		Q_memcpy(&net_from, &host_client->netchan.remote_address, sizeof(net_from));
		if (SV_FilterPacket())
		{
			SV_ClientPrintf("The server operator has added you to banned list\n");
			SV_DropClient(host_client, 0, "Added to banned list");;
		}
	}
}

/* <aa2c1> ../engine/sv_main.c:8470 */
void SV_RemoveIP_f(void)
{
	ipfilter_t f;

	if (!StringToFilter(Cmd_Argv(1), &f))
		return;

	for (int i = 0; i < numipfilters; i++)
	{
		if (ipfilters[i].mask == f.mask && ipfilters[i].compare.u32 == f.compare.u32)
		{
			if (i + 1 < numipfilters)
				Q_memcpy(&ipfilters[i], &ipfilters[i + 1], (numipfilters - (i + 1)) * sizeof(ipfilter_t));
			numipfilters--;
			ipfilters[numipfilters].banTime = 0.0f;
			ipfilters[numipfilters].banEndTime = 0.0f;
			ipfilters[numipfilters].compare.u32 = 0;
			ipfilters[numipfilters].mask = 0;
			Con_Printf("IP filter removed.\n");

			return;
		}
	}
	Con_Printf("removeip: couldn't find %s.\n", Cmd_Argv(1));
}

/* <a62ca> ../engine/sv_main.c:8507 */
void SV_ListIP_f(void)
{
	if (numipfilters <= 0)
	{
		Con_Printf("IP filter list: empty\n");
		return;
	}

	Con_Printf("IP filter list:\n");
	for (int i = 0; i < numipfilters; i++)
	{
		uint8* b = ipfilters[i].compare.octets;
		if (ipfilters[i].banTime == 0.0f) 
			Con_Printf("%3i.%3i.%3i.%3i : permanent\n", b[0], b[1], b[2], b[3]);
		else
			Con_Printf("%3i.%3i.%3i.%3i : %.3f min\n", b[0], b[1], b[2], b[3], ipfilters[i].banTime);
	}
}

/* <a6301> ../engine/sv_main.c:8535 */
void SV_WriteIP_f(void)
{
	char name[MAX_PATH];
	Q_snprintf(name, MAX_PATH, "listip.cfg");
	Con_Printf("Writing %s.\n", name);

	FILE *f = FS_Open(name, "wb");
	if (!f)
	{
		Con_Printf("Couldn't open %s\n", name);
		return;
	}

	for (int i = 0; i < numipfilters; i++)
	{
		if (ipfilters[i].banTime != 0.0f)
			continue;

		uint8 *b = ipfilters[i].compare.octets;
		FS_FPrintf(f, "addip 0.0 %i.%i.%i.%i\n", b[0], b[1], b[2], b[3]);
	}
	FS_Close(f);
}

/* <aaeb1> ../engine/sv_main.c:8569 */
void SV_KickPlayer(int nPlayerSlot, int nReason)
{
	if (nPlayerSlot < 0 || nPlayerSlot >= g_psvs.maxclients)
		return;

	client_t * client = &g_psvs.clients[nPlayerSlot];
	if (!client->connected || !g_psvs.isSecure)
		return;

	USERID_t id;
	Q_memcpy(&id, &client->network_userid, sizeof(id));

	Log_Printf("Secure: \"%s<%i><%s><>\" was detected cheating and dropped from the server.\n", client->name, client->userid, SV_GetIDString(&id), nReason);
	
	char rgchT[1024];
	rgchT[0] = svc_print;
	Q_sprintf(
		&rgchT[1],
		"\n********************************************\nYou have been automatically disconnected\nfrom this secure server because an illegal\ncheat was detected on your computer.\nRepeat violators may be permanently banned\nfrom all secure servers.\n\nFor help cleaning your system of cheats, visit:\nhttp://www.counter-strike.net/cheat.html\n********************************************\n\n"
	);
	Netchan_Transmit(&g_psvs.clients[nPlayerSlot].netchan, strlen(rgchT) + 1, (byte *)rgchT);

	Q_sprintf(rgchT, "%s was automatically disconnected\nfrom this secure server.\n", client->name);
	for (int i = 0; i < g_psvs.maxclients; i++)
	{
		if (!g_psvs.clients[i].active && !g_psvs.clients[i].spawned || g_psvs.clients[i].fakeclient)
			continue;

		MSG_WriteByte(&g_psvs.clients[i].netchan.message, svc_centerprint);
		MSG_WriteString(&g_psvs.clients[i].netchan.message, rgchT);
	}

	SV_DropClient(&g_psvs.clients[nPlayerSlot], FALSE, "Automatically dropped by cheat detector");
}

/* <aa3c7> ../engine/sv_main.c:8639 */
void SV_InactivateClients(void)
{
	int i;
	client_t *cl;
	for (i = 0, cl = g_psvs.clients; i < g_psvs.maxclients; i++, cl++)
	{
		if (!cl->active && !cl->connected && !cl->spawned)
			continue;

		if (cl->fakeclient)
			SV_DropClient(cl, FALSE, "Dropping fakeclient on level change");
		else
		{
			cl->active = FALSE;
			cl->connected = TRUE;
			cl->spawned = FALSE;
			cl->fully_connected = FALSE;

			SZ_Clear(&cl->netchan.message);
			SZ_Clear(&cl->datagram);

			COM_ClearCustomizationList(&cl->customdata, FALSE);
			Q_memset(cl->physinfo, 0, MAX_PHYSINFO_STRING);
		}
	}
}

/* <a5edd> ../engine/sv_main.c:8676 */
void SV_FailDownload(const char *filename)
{
	if (filename && *filename)
	{
		MSG_WriteByte(&host_client->netchan.message, svc_filetxferfailed);
		MSG_WriteString(&host_client->netchan.message, filename);
	}
}

/* <aa437> ../engine/sv_main.c:8686 */
//-----------------------------------------------------------------------------
// Finds a string in another string with a case insensitive test
//-----------------------------------------------------------------------------
const char *Q_stristr(const char *pStr, const char *pSearch)
{
	if (!pStr || !pSearch)
		return NULL;

	char const *pLetter = pStr;

	// Check the entire string
	while (*pLetter != 0)
	{
		// Skip over non-matches
		if (tolower(*pLetter) == tolower(*pSearch))
		{
			// Check for match
			char const* pMatch = pLetter + 1;
			char const* pTest = pSearch + 1;
			while (*pTest != 0)
			{
				// We've run off the end; don't bother.
				if (*pMatch == 0)
					return NULL;

				if (tolower(*pMatch) != tolower(*pTest))
					break;

				++pMatch;
				++pTest;
			}

			// Found a match!
			if (*pTest == 0)
				return pLetter;
		}

		++pLetter;
	}

	return NULL;
}

/* <aa4f4> ../engine/sv_main.c:8736 */
qboolean IsSafeFileToDownload(const char *filename)
{
	char *first;
	char *last;
	char lwrfilename[MAX_PATH];

	if (!filename)
		return FALSE;

#ifdef REHLDS_FIXES
	// FIXED: Allow to download customizations
	if (filename[0] == '!')
	{
		return TRUE;
	}
#endif // REHLDS_FIXES

	// Convert to lower case
	Q_strncpy(lwrfilename, filename, ARRAYSIZE(lwrfilename));
#ifdef REHLDS_CHECKS
	lwrfilename[ARRAYSIZE(lwrfilename) - 1] = 0;
#endif
	Q_strlwr(lwrfilename);

	first = Q_strchr(lwrfilename, '.');
	last = Q_strrchr(lwrfilename, '.');

	if (lwrfilename[0] == '/'
		|| Q_strstr(lwrfilename, "\\")
		|| Q_strstr(lwrfilename, ":")
		|| Q_strstr(lwrfilename, "..")
		|| Q_strstr(lwrfilename, "~")
		|| first != last
		|| !first
		|| Q_strlen(first) != 4
		|| Q_stristr(lwrfilename, ".cfg")
		|| Q_stristr(lwrfilename, ".lst")
		|| Q_stristr(lwrfilename, ".exe")
		|| Q_stristr(lwrfilename, ".vbs")
		|| Q_stristr(lwrfilename, ".com")
		|| Q_stristr(lwrfilename, ".bat")
		|| Q_stristr(lwrfilename, ".dll")
		|| Q_stristr(lwrfilename, ".ini")
		|| Q_stristr(lwrfilename, ".log")
		|| Q_stristr(lwrfilename, "halflife.wad")
		|| Q_stristr(lwrfilename, "pak0.pak")
		|| Q_stristr(lwrfilename, "xeno.wad")
		|| Q_stristr(lwrfilename, ".so")
		|| Q_stristr(lwrfilename, ".dylib")
		|| Q_stristr(lwrfilename, ".sys"))
	{
		return FALSE;
	}
	return TRUE;
}

/* <aa564> ../engine/sv_main.c:8835 */
void SV_BeginFileDownload_f(void)
{
	const char *name;
	char szModuleC[13] = "!ModuleC.dll";

	if (Cmd_Argc() < 2 || cmd_source == src_command)
	{
		return;
	}

	name = Cmd_Argv(1);
	if (!name || !name[0] || (!Q_strncmp(name, szModuleC, 12) && g_psvs.isSecure))
	{
		return;
	}

	if (!IsSafeFileToDownload(name) || sv_allow_download.value == 0.0f)
	{
		SV_FailDownload(name);
		return;
	}

	// Regular downloads
	if (name[0] != '!')
	{
		if (host_client->fully_connected ||
			sv_send_resources.value == 0.0f ||
			sv_downloadurl.string != NULL && sv_downloadurl.string[0] != 0 && Q_strlen(sv_downloadurl.string) <= 128 && sv_allow_dlfile.value == 0.0f ||
			Netchan_CreateFileFragments(TRUE, &host_client->netchan, name) == 0)
		{
			SV_FailDownload(name);
			return;
		}
		Netchan_FragSend(&host_client->netchan);
		return;
	}

	// Customizations
	if (Q_strlen(name) != 36 || Q_strnicmp(name, "!MD5", 4) != 0 || sv_send_logos.value == 0.0f)
	{
		SV_FailDownload(name);
		return;
	}

	unsigned char md5[16];
	resource_t custResource;
	unsigned char *pbuf = NULL;
	int size = 0;

	Q_memset(&custResource, 0, sizeof(custResource));
	COM_HexConvert(name + 4, 32, md5);
	if (HPAK_ResourceForHash("custom.hpk", md5, &custResource))
	{
		HPAK_GetDataPointer("custom.hpk", &custResource, &pbuf, &size);
#ifdef REHLDS_FIXES
		if (pbuf && size)
		{
			Netchan_CreateFileFragmentsFromBuffer(1, &host_client->netchan, name, pbuf, size);
			Netchan_FragSend(&host_client->netchan);
		}
		// Mem_Free pbuf even if size is zero
		if (pbuf)
		{
			Mem_Free((void *)pbuf);
		}
#else // REHLDS_FIXES
		if (pbuf && size)
		{
			Netchan_CreateFileFragmentsFromBuffer(1, &host_client->netchan, name, pbuf, size);
			Netchan_FragSend(&host_client->netchan);
			Mem_Free((void *)pbuf);
		}
#endif // REHLDS_FIXES
	}

#ifdef REHLDS_FIXES
	// TODO: Shouldn't we SV_FailDownload if hpak do not contain resource? Ok, let's do it
	if (pbuf == NULL || size == 0)
	{
		SV_FailDownload(name);
	}
#endif // REHLDS_FIXES
}

/* <aa65c> ../engine/sv_main.c:8942 */
void SV_SetMaxclients(void)
{
	int i;
	client_t *cl;

	for (i = 0, cl = g_psvs.clients; i < g_psvs.maxclientslimit; i++, cl++)
		SV_ClearFrames(&cl->frames);
	
	g_psvs.maxclients = 1;
	i = COM_CheckParm("-maxplayers");

	if (i)
		g_psvs.maxclients = Q_atoi(com_argv[i + 1]);
	else
	{
		if (g_bIsDedicatedServer)
			g_psvs.maxclients = 6;
	}

	g_pcls.state = (cactive_t)(g_bIsDedicatedServer == FALSE);

	if (g_psvs.maxclients > 32)
		g_psvs.maxclients = 32;
	if (g_psvs.maxclients < 1)
		g_psvs.maxclients = 6;

	if (g_bIsDedicatedServer)
		g_psvs.maxclientslimit = 32;
	else if(host_parms.memsize > 0x1000000)
		g_psvs.maxclientslimit = 4;

	SV_UPDATE_BACKUP = 8;
	SV_UPDATE_MASK = 7;

	if(g_psvs.maxclients != 1)
	{
		SV_UPDATE_BACKUP = 64;
		SV_UPDATE_MASK = 63;
	}

	g_psvs.clients = (client_t *)Hunk_AllocName(sizeof(client_t) * g_psvs.maxclientslimit, "clients");
	for (i = 0, cl = g_psvs.clients; i < g_psvs.maxclientslimit; i++, cl++)
	{
		Q_memset(cl, 0, sizeof(client_t));

		cl->resourcesneeded.pPrev = &cl->resourcesneeded;
		cl->resourcesneeded.pNext = &cl->resourcesneeded;
		cl->resourcesonhand.pPrev = &cl->resourcesonhand;
		cl->resourcesonhand.pNext = &cl->resourcesonhand;
	}
	if (g_psvs.maxclients >= 2)
		Cvar_SetValue("deathmatch", 1.0);
	else Cvar_SetValue("deathmatch", 0.0);
	SV_AllocClientFrames();

	if (g_psvs.maxclientslimit < g_psvs.maxclients)
		g_psvs.maxclients = g_psvs.maxclientslimit;

	Rehlds_Interfaces_InitClients();
}

/* <aa728> ../engine/sv_main.c:9033 */
void SV_HandleRconPacket(void)
{
	MSG_BeginReading();
	MSG_ReadLong();
	char* s = MSG_ReadStringLine();
	Cmd_TokenizeString(s);
	const char* c = Cmd_Argv(0);
	if (!Q_strcmp(c, "getchallenge"))
	{
		SVC_GetChallenge();
	} 
	else if (!Q_stricmp(c, "challenge"))
	{
		SVC_ServiceChallenge();
	}
	else if (!Q_strcmp(c, "rcon"))
	{
		SV_Rcon(&net_from);
	}
}

/* <aa7b0> ../engine/sv_main.c:9065 */
void SV_CheckCmdTimes(void)
{
	static double lastreset;

	if (Host_IsSinglePlayerGame())
		return;

	if (realtime - lastreset < 1.0)
		return;

	lastreset = realtime;
	for (int i = g_psvs.maxclients - 1; i >= 0; i--)
	{
		client_t* cl = &g_psvs.clients[i];
		if (!cl->connected || !cl->active)
			continue;

		if (cl->connecttime == 0.0)
			cl->connecttime = realtime;

		float dif = cl->connecttime + cl->cmdtime - realtime;
		if (dif > clockwindow.value)
		{
			cl->ignorecmdtime = clockwindow.value + realtime;
			cl->cmdtime = realtime - cl->connecttime;
		}

		if (dif < -clockwindow.value)
			cl->cmdtime = realtime - cl->connecttime;
	}
}

/* <aa80a> ../engine/sv_main.c:9122 */
void SV_CheckForRcon(void)
{
	if (g_psv.active || g_pcls.state != ca_dedicated || giActive == DLL_CLOSE || !host_initialized)
		return;

	while (NET_GetPacket(NS_SERVER))
	{
		if (SV_FilterPacket())
		{
			SV_SendBan();
		}
		else
		{
			if (*(uint32 *)net_message.data == 0xFFFFFFFF)
				SV_HandleRconPacket();
		}
	}
}

/* <aa8ac> ../engine/sv_main.c:9144 */
qboolean SV_IsSimulating(void)
{
	if (g_psv.paused)
		return FALSE;

	if (g_psvs.maxclients > 1)
		return TRUE;

	if (!key_dest && (g_pcls.state == ca_active || g_pcls.state == ca_dedicated))
		return TRUE;

	return FALSE;
}

/* <aa8fc> ../engine/sv_main.c:9156 */
void SV_CheckMapDifferences(void)
{
	static double lastcheck;

	if (realtime - lastcheck < 5.0)
		return;

	lastcheck = realtime;
	for (int i = 0; i < g_psvs.maxclients; i++)
	{
		client_t *cl = &g_psvs.clients[i];
		if (!cl->active || !cl->crcValue)
			continue;

		if (cl->netchan.remote_address.type == NA_LOOPBACK)
			continue;

		if (cl->crcValue != g_psv.worldmapCRC)
			cl->netchan.message.flags |= SIZEBUF_OVERFLOWED;
	}
}

/* <aba46> ../engine/sv_main.c:9191 */
void SV_Frame(void)
{
	if (!g_psv.active)
		return;

	gGlobalVariables.frametime = host_frametime;
	g_psv.oldtime = g_psv.time;
	SV_CheckCmdTimes();
	SV_ReadPackets();
	if (SV_IsSimulating())
	{
		SV_Physics();
		g_psv.time += host_frametime;
	}
	SV_QueryMovevarsChanged();
	SV_RequestMissingResourcesFromClients();
	SV_CheckTimeouts();
	SV_SendClientMessages();
	SV_CheckMapDifferences();
	SV_GatherStatistics();
	Steam_RunFrame();
}

/* <a81ca> ../engine/sv_main.c:9252 */
void SV_Drop_f(void)
{
	if (cmd_source == src_command)
	{
		Cmd_ForwardToServer();
	}
	else
	{
		SV_EndRedirect();
		SV_BroadcastPrintf("%s dropped\n", host_client->name);
		SV_DropClient(host_client, FALSE, "Client sent 'drop'");
	}
}

/* <a6270> ../engine/sv_main.c:9265 */
void SV_RegisterDelta(char *name, char *loadfile)
{
	delta_t *pdesc = NULL;
	if (!DELTA_Load(name, &pdesc, loadfile))
		Sys_Error("Error parsing %s!!!\n", loadfile);

	delta_info_t *p = (delta_info_t *)Mem_ZeroMalloc(sizeof(delta_info_t));
	p->loadfile = Mem_Strdup(loadfile);
	p->name = Mem_Strdup(name);
	p->delta = pdesc;
	p->next = g_sv_delta;
	g_sv_delta = p;

#if defined(REHLDS_OPT_PEDANTIC) || defined(REHLDS_FIXES)
	g_DeltaJitRegistry.CreateAndRegisterDeltaJIT(pdesc);
#endif
}

/* <aa966> ../engine/sv_main.c:9284 */
void SV_InitDeltas(void)
{
	Con_DPrintf("Initializing deltas\n");
	SV_RegisterDelta("clientdata_t", "delta.lst");
	SV_RegisterDelta("entity_state_t", "delta.lst");
	SV_RegisterDelta("entity_state_player_t", "delta.lst");
	SV_RegisterDelta("custom_entity_state_t", "delta.lst");
	SV_RegisterDelta("usercmd_t", "delta.lst");
	SV_RegisterDelta("weapon_data_t", "delta.lst");
	SV_RegisterDelta("event_t", "delta.lst");

	g_pplayerdelta = SV_LookupDelta("entity_state_player_t");
	if (!g_pplayerdelta)
		Sys_Error("No entity_state_player_t encoder on server!\n");

	g_pentitydelta = SV_LookupDelta("entity_state_t");
	if (!g_pentitydelta)
		Sys_Error("No entity_state_t encoder on server!\n");

	g_pcustomentitydelta = SV_LookupDelta("custom_entity_state_t");
	if (!g_pcustomentitydelta)
		Sys_Error("No custom_entity_state_t encoder on server!\n");

	g_pclientdelta = SV_LookupDelta("clientdata_t");
	if (!g_pclientdelta)
		Sys_Error("No clientdata_t encoder on server!\n");

	g_pweapondelta = SV_LookupDelta("weapon_data_t");
	if (!g_pweapondelta)
		Sys_Error("No weapon_data_t encoder on server!\n");

	g_peventdelta = SV_LookupDelta("event_t");
	if (!g_peventdelta)
		Sys_Error("No event_t encoder on server!\n");

#ifdef REHLDS_OPT_PEDANTIC
	g_pusercmddelta = SV_LookupDelta("usercmd_t");
	if (!g_pusercmddelta)
		Sys_Error("No usercmd_t encoder on server!\n");
#endif

#if defined(REHLDS_OPT_PEDANTIC) || defined(REHLDS_FIXES)
	g_DeltaJitRegistry.CreateAndRegisterDeltaJIT(&g_MetaDelta[0]);
#endif
}

/* <aac49> ../engine/sv_main.c:9339 */
void SV_InitEncoders(void)
{
	delta_t *pdesc;
	delta_info_t *p;
	for (p = g_sv_delta; p != NULL; p = p->next)
	{
		pdesc = p->delta;
		if (Q_strlen(pdesc->conditionalencodename) > 0)
			pdesc->conditionalencode = DELTA_LookupEncoder(pdesc->conditionalencodename);
	}
}

/* <aac82> ../engine/sv_main.c:9362 */
void SV_Init(void)
{
#ifdef HOOK_ENGINE

	Cmd_AddCommand("banid", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_BanId_f", (void *)SV_BanId_f));
	Cmd_AddCommand("removeid", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_RemoveId_f", (void *)SV_RemoveId_f));
	Cmd_AddCommand("listid", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_ListId_f", (void *)SV_ListId_f));
	Cmd_AddCommand("writeid", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_WriteId_f", (void *)SV_WriteId_f));
	Cmd_AddCommand("resetrcon", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_ResetRcon_f", (void *)SV_ResetRcon_f));
	Cmd_AddCommand("logaddress", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_SetLogAddress_f", (void *)SV_SetLogAddress_f));
	Cmd_AddCommand("logaddress_add", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_AddLogAddress_f", (void *)SV_AddLogAddress_f));
	Cmd_AddCommand("logaddress_del", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_DelLogAddress_f", (void *)SV_DelLogAddress_f));
	Cmd_AddCommand("log", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_ServerLog_f", (void *)SV_ServerLog_f));
	Cmd_AddCommand("serverinfo", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_Serverinfo_f", (void *)SV_Serverinfo_f));
	Cmd_AddCommand("localinfo", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_Localinfo_f", (void *)SV_Localinfo_f));
	Cmd_AddCommand("showinfo", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_ShowServerinfo_f", (void *)SV_ShowServerinfo_f));
	Cmd_AddCommand("user", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_User_f", (void *)SV_User_f));
	Cmd_AddCommand("users", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_Users_f", (void *)SV_Users_f));
	Cmd_AddCommand("dlfile", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_BeginFileDownload_f", (void *)SV_BeginFileDownload_f));
	Cmd_AddCommand("addip", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_AddIP_f", (void *)SV_AddIP_f));
	Cmd_AddCommand("removeip", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_RemoveIP_f", (void *)SV_RemoveIP_f));
	Cmd_AddCommand("listip", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_ListIP_f", (void *)SV_ListIP_f));
	Cmd_AddCommand("writeip", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_WriteIP_f", (void *)SV_WriteIP_f));
	Cmd_AddCommand("dropclient", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_Drop_f", (void *)SV_Drop_f));
	Cmd_AddCommand("spawn", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_Spawn_f", (void *)SV_Spawn_f));
	Cmd_AddCommand("new", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_New_f", (void *)SV_New_f));
	Cmd_AddCommand("sendres", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_SendRes_f", (void *)SV_SendRes_f));
	Cmd_AddCommand("sendents", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_SendEnts_f", (void *)SV_SendEnts_f));
	Cmd_AddCommand("fullupdate", (xcommand_t)GetOriginalFuncAddrOrDefault("SV_FullUpdate_f", (void *)SV_FullUpdate_f));

#else // HOOK_ENGINE

	Cmd_AddCommand("banid", SV_BanId_f);
	Cmd_AddCommand("removeid", SV_RemoveId_f);
	Cmd_AddCommand("listid", SV_ListId_f);
	Cmd_AddCommand("writeid", SV_WriteId_f);
	Cmd_AddCommand("resetrcon", SV_ResetRcon_f);
	Cmd_AddCommand("logaddress", SV_SetLogAddress_f);
	Cmd_AddCommand("logaddress_add", SV_AddLogAddress_f);
	Cmd_AddCommand("logaddress_del", SV_DelLogAddress_f);
	Cmd_AddCommand("log", SV_ServerLog_f);
	Cmd_AddCommand("serverinfo", SV_Serverinfo_f);
	Cmd_AddCommand("localinfo", SV_Localinfo_f);
	Cmd_AddCommand("showinfo", SV_ShowServerinfo_f);
	Cmd_AddCommand("user", SV_User_f);
	Cmd_AddCommand("users", SV_Users_f);
	Cmd_AddCommand("dlfile", SV_BeginFileDownload_f);
	Cmd_AddCommand("addip", SV_AddIP_f);
	Cmd_AddCommand("removeip", SV_RemoveIP_f);
	Cmd_AddCommand("listip", SV_ListIP_f);
	Cmd_AddCommand("writeip", SV_WriteIP_f);
	Cmd_AddCommand("dropclient", SV_Drop_f);
	Cmd_AddCommand("spawn", SV_Spawn_f);
	Cmd_AddCommand("new", SV_New_f);
	Cmd_AddCommand("sendres", SV_SendRes_f);
	Cmd_AddCommand("sendents", SV_SendEnts_f);
	Cmd_AddCommand("fullupdate", SV_FullUpdate_f);

#endif // HOOK_ENGINE

	Cvar_RegisterVariable(&sv_failuretime);
	Cvar_RegisterVariable(&sv_voiceenable);
	Cvar_RegisterVariable(&rcon_password);
	Cvar_RegisterVariable(&sv_enableoldqueries);
	Cvar_RegisterVariable(&mp_consistency);
	Cvar_RegisterVariable(&sv_instancedbaseline);
	Cvar_RegisterVariable(&sv_contact);
	Cvar_RegisterVariable(&sv_unlag);
	Cvar_RegisterVariable(&sv_maxunlag);
	Cvar_RegisterVariable(&sv_unlagpush);
	Cvar_RegisterVariable(&sv_unlagsamples);
	Cvar_RegisterVariable(&sv_filterban);
	Cvar_RegisterVariable(&sv_maxupdaterate);
	Cvar_RegisterVariable(&sv_minupdaterate);
	Cvar_RegisterVariable(&sv_logrelay);
	Cvar_RegisterVariable(&sv_lan);
	Cvar_DirectSet(&sv_lan, PF_IsDedicatedServer() ? "0" : "1");
	Cvar_RegisterVariable(&sv_lan_rate);
	Cvar_RegisterVariable(&sv_proxies);
	Cvar_RegisterVariable(&sv_outofdatetime);
	Cvar_RegisterVariable(&sv_visiblemaxplayers);
	Cvar_RegisterVariable(&sv_password);
	Cvar_RegisterVariable(&sv_aim);
	Cvar_RegisterVariable(&violence_hblood);
	Cvar_RegisterVariable(&violence_ablood);
	Cvar_RegisterVariable(&violence_hgibs);
	Cvar_RegisterVariable(&violence_agibs);
	Cvar_RegisterVariable(&sv_newunit);
	Cvar_RegisterVariable(&sv_gravity);
	Cvar_RegisterVariable(&sv_friction);
	Cvar_RegisterVariable(&sv_edgefriction);
	Cvar_RegisterVariable(&sv_stopspeed);
	Cvar_RegisterVariable(&sv_maxspeed);
	Cvar_RegisterVariable(&sv_footsteps);
	Cvar_RegisterVariable(&sv_accelerate);
	Cvar_RegisterVariable(&sv_stepsize);
	Cvar_RegisterVariable(&sv_bounce);
	Cvar_RegisterVariable(&sv_airaccelerate);
	Cvar_RegisterVariable(&sv_wateraccelerate);
	Cvar_RegisterVariable(&sv_waterfriction);
	Cvar_RegisterVariable(&sv_skycolor_r);
	Cvar_RegisterVariable(&sv_skycolor_g);
	Cvar_RegisterVariable(&sv_skycolor_b);
	Cvar_RegisterVariable(&sv_skyvec_x);
	Cvar_RegisterVariable(&sv_skyvec_y);
	Cvar_RegisterVariable(&sv_skyvec_z);
	Cvar_RegisterVariable(&sv_timeout);
	Cvar_RegisterVariable(&sv_clienttrace);
	Cvar_RegisterVariable(&sv_zmax);
	Cvar_RegisterVariable(&sv_wateramp);
	Cvar_RegisterVariable(&sv_skyname);
	Cvar_RegisterVariable(&sv_maxvelocity);
	Cvar_RegisterVariable(&sv_cheats);
	if (COM_CheckParm("-dev"))
		Cvar_SetValue("sv_cheats", 1.0);
	Cvar_RegisterVariable(&sv_spectatormaxspeed);
	Cvar_RegisterVariable(&sv_allow_download);
	Cvar_RegisterVariable(&sv_allow_upload);
	Cvar_RegisterVariable(&sv_max_upload);
	Cvar_RegisterVariable(&sv_send_logos);
	Cvar_RegisterVariable(&sv_send_resources);
	Cvar_RegisterVariable(&sv_logbans);
	Cvar_RegisterVariable(&hpk_maxsize);
	Cvar_RegisterVariable(&mapcyclefile);
	Cvar_RegisterVariable(&motdfile);
	Cvar_RegisterVariable(&servercfgfile);
	Cvar_RegisterVariable(&mapchangecfgfile);
	Cvar_RegisterVariable(&lservercfgfile);
	Cvar_RegisterVariable(&logsdir);
	Cvar_RegisterVariable(&bannedcfgfile);
	Cvar_RegisterVariable(&sv_rcon_minfailures);
	Cvar_RegisterVariable(&sv_rcon_maxfailures);
	Cvar_RegisterVariable(&sv_rcon_minfailuretime);
	Cvar_RegisterVariable(&sv_rcon_banpenalty);
	Cvar_RegisterVariable(&sv_minrate);
	Cvar_RegisterVariable(&sv_maxrate);
	Cvar_RegisterVariable(&max_queries_sec);
	Cvar_RegisterVariable(&max_queries_sec_global);
	Cvar_RegisterVariable(&max_queries_window);
	Cvar_RegisterVariable(&sv_logblocks);
	Cvar_RegisterVariable(&sv_downloadurl);
	Cvar_RegisterVariable(&sv_version);
	Cvar_RegisterVariable(&sv_allow_dlfile);

	for (int i = 0; i < 512; i++)
	{
		Q_snprintf(localmodels[i], 5u, "*%i", i);
	}

	g_psvs.isSecure = FALSE;

	for (int i = 0; i < g_psvs.maxclientslimit; i++)
	{
		client_t *cl = &g_psvs.clients[i];
		SV_ClearFrames(&cl->frames);
		Q_memset(cl, 0, sizeof(client_t));
		cl->resourcesonhand.pPrev = &cl->resourcesonhand;
		cl->resourcesonhand.pNext = &cl->resourcesonhand;
		cl->resourcesneeded.pPrev = &cl->resourcesneeded;
		cl->resourcesneeded.pNext = &cl->resourcesneeded;
	}

	PM_Init(&g_svmove);
	SV_AllocClientFrames();
	SV_InitDeltas();
}

/* <aad4b> ../engine/sv_main.c:9558 */
void SV_Shutdown(void)
{
	g_DeltaJitRegistry.Cleanup();
	delta_info_t *p = g_sv_delta;
	while (p)
	{
		delta_info_t *n = p->next;
		if (p->delta)
			DELTA_FreeDescription(&p->delta);

		Mem_Free(p->name);
		Mem_Free(p->loadfile);
		Mem_Free(p);
		p = n;
	}

	g_sv_delta = NULL;
}

qboolean SV_CompareUserID(USERID_t *id1, USERID_t *id2)
{
	return g_RehldsHookchains.m_SV_CompareUserID.callChain(SV_CompareUserID_internal, id1, id2);
}

/* <a5ef9> ../engine/sv_main.c:9585 */
qboolean EXT_FUNC SV_CompareUserID_internal(USERID_t *id1, USERID_t *id2)
{
	if (id1 == NULL || id2 == NULL)
		return FALSE;

	if (id1->idtype != id2->idtype)
		return FALSE;

	if (id1->idtype != AUTH_IDTYPE_STEAM && id1->idtype != AUTH_IDTYPE_VALVE)
		return FALSE;

	char szID1[64];
	char szID2[64];

	Q_strncpy(szID1, SV_GetIDString(id1), sizeof(szID1) - 1);
	szID1[sizeof(szID1) - 1] = 0;

	Q_strncpy(szID2, SV_GetIDString(id2), sizeof(szID2) - 1);
	szID2[sizeof(szID2) - 1] = 0;

	return Q_stricmp(szID1, szID2) ? FALSE : TRUE;
}

char* SV_GetIDString(USERID_t *id)
{
	return g_RehldsHookchains.m_SV_GetIDString.callChain(SV_GetIDString_internal, id);
}

/* <aad82> ../engine/sv_main.c:9625 */
char* EXT_FUNC SV_GetIDString_internal(USERID_t *id)
{
	static char idstr[64];

	idstr[0] = 0;

	if (!id)
	{
		return idstr;
	}

	switch (id->idtype)
	{
	case AUTH_IDTYPE_STEAM:
		if (sv_lan.value != 0.0f)
		{
			Q_strncpy(idstr, "STEAM_ID_LAN", ARRAYSIZE(idstr) - 1);
		}
		else if (!id->m_SteamID)
		{
			Q_strncpy(idstr, "STEAM_ID_PENDING", ARRAYSIZE(idstr) - 1);
		}
		else
		{
			TSteamGlobalUserID steam2ID = Steam_Steam3IDtoSteam2(id->m_SteamID);
			Q_snprintf(idstr, ARRAYSIZE(idstr) - 1, "STEAM_%u:%u:%u", steam2ID.m_SteamInstanceID, steam2ID.m_SteamLocalUserID.Split.High32bits, steam2ID.m_SteamLocalUserID.Split.Low32bits);
		}
		break;
	case AUTH_IDTYPE_VALVE:
		if (sv_lan.value != 0.0f)
		{
			Q_strncpy(idstr, "VALVE_ID_LAN", ARRAYSIZE(idstr) - 1);
		}
		else if (!id->m_SteamID)
		{
			Q_strncpy(idstr, "VALVE_ID_PENDING", ARRAYSIZE(idstr) - 1);
		}
		else
		{
			TSteamGlobalUserID steam2ID = Steam_Steam3IDtoSteam2(id->m_SteamID);
			Q_snprintf(idstr, ARRAYSIZE(idstr) - 1, "VALVE_%u:%u:%u", steam2ID.m_SteamInstanceID, steam2ID.m_SteamLocalUserID.Split.High32bits, steam2ID.m_SteamLocalUserID.Split.Low32bits);
		}
		break;
	case AUTH_IDTYPE_LOCAL:
		Q_strncpy(idstr, "HLTV", ARRAYSIZE(idstr) - 1);
		break;
	default:
		Q_strncpy(idstr, "UNKNOWN", ARRAYSIZE(idstr) - 1);
		break;
	}
	// Don't be paranoid
	//idstr[ARRAYSIZE(idstr) - 1] = 0;

	return idstr;
}

/* <a5f45> ../engine/sv_main.c:9697 */
char *SV_GetClientIDString(client_t *client)
{
	static char idstr[64];

	idstr[0] = 0;

	if (!client)
	{
		return idstr;
	}

	if (client->netchan.remote_address.type == NA_LOOPBACK && client->network_userid.idtype == AUTH_IDTYPE_VALVE)
	{
		Q_snprintf(idstr, ARRAYSIZE(idstr) - 1, "VALVE_ID_LOOPBACK");
	}
	else
	{
		USERID_t *id = &client->network_userid;
		Q_snprintf(idstr, ARRAYSIZE(idstr) - 1, "%s", SV_GetIDString(id));
		idstr[ARRAYSIZE(idstr) - 1] = 0;
	}

	return idstr;
}

/* <a5b9a> ../engine/sv_main.c:9719 */
typedef struct GameToAppIDMapItem_s
{
	unsigned int iAppID;
	const char *pGameDir;
} GameToAppIDMapItem_t;

GameToAppIDMapItem_t g_GameToAppIDMap[11] = {
	0x0A, "cstrike",
	0x14, "tfc",
	0x1E, "dod",
	0x28, "dmc",
	0x32, "gearbox",
	0x3C, "ricochet",
	0x46, "valve",
	0x50, "czero",
	0x64, "czeror",
	0x82, "bshift",
	0x96, "cstrike_beta",
};

/* <abae3> ../engine/sv_main.c:9748 */
int GetGameAppID(void)
{
	char arg[260];
	char gd[260];

	COM_ParseDirectoryFromCmd("-game", gd, "valve");
	COM_FileBase(gd, arg);
	for (int i = 0; i < ARRAYSIZE(g_GameToAppIDMap); i++)
	{
		if (!Q_stricmp(g_GameToAppIDMap[i].pGameDir, arg))
			return g_GameToAppIDMap[i].iAppID;
	}

	return 70;
}

/* <abb2e> ../engine/sv_main.c:9772 */
qboolean IsGameSubscribed(const char *gameName)
{
#ifdef _WIN32
	for (int i = 0; i < ARRAYSIZE(g_GameToAppIDMap); i++)
	{
		if (!Q_stricmp(g_GameToAppIDMap[i].pGameDir, gameName))
		{
			return ISteamApps_BIsSubscribedApp(g_GameToAppIDMap[i].iAppID);
		}
	}

	return ISteamApps_BIsSubscribedApp(70);
#else //_WIN32
	return 0;
#endif
}

/* <abb5b> ../engine/sv_main.c:9796 */
NOXREF qboolean BIsValveGame(void)
{
	for (int i = 0; i < ARRAYSIZE(g_GameToAppIDMap); i++)
	{
		if (!Q_stricmp(g_GameToAppIDMap[i].pGameDir, com_gamedir))
			return TRUE;
	}
	return FALSE;
}
