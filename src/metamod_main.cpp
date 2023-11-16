#include <stdio.h>
#include "metamod_main.hpp"
#include "iserver.h"

#include "paths/paths.hpp"
#include "managers/infomanager.hpp"

SH_DECL_HOOK3_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool, bool, bool);
SH_DECL_HOOK4_void(IServerGameClients, ClientActive, SH_NOATTRIB, 0, CPlayerSlot, bool, const char*, uint64);
SH_DECL_HOOK5_void(IServerGameClients, ClientDisconnect, SH_NOATTRIB, 0, CPlayerSlot, int, const char*, uint64, const char*);
SH_DECL_HOOK4_void(IServerGameClients, ClientPutInServer, SH_NOATTRIB, 0, CPlayerSlot, char const*, int, uint64);
SH_DECL_HOOK1_void(IServerGameClients, ClientSettingsChanged, SH_NOATTRIB, 0, CPlayerSlot);
SH_DECL_HOOK6_void(IServerGameClients, OnClientConnected, SH_NOATTRIB, 0, CPlayerSlot, const char*, uint64, const char*, const char*, bool);
SH_DECL_HOOK6(IServerGameClients, ClientConnect, SH_NOATTRIB, 0, bool, CPlayerSlot, const char*, uint64, const char*, bool, CBufferString*);
SH_DECL_HOOK2(IGameEventManager2, FireEvent, SH_NOATTRIB, 0, bool, IGameEvent*, bool);

SH_DECL_HOOK2_void(IServerGameClients, ClientCommand, SH_NOATTRIB, 0, CPlayerSlot, const CCommand&);

WCSSkills g_WCSSkills;
ISmmAPI* WCSSkills::ismm = nullptr;
IServerGameDLL* server = NULL;
IServerGameClients* gameclients = NULL;
IVEngineServer* engine = NULL;
IGameEventManager2* gameevents = NULL;
ICvar* icvar = NULL;

// Should only be called within the active game loop (i e map should be loaded and active)
// otherwise that'll be nullptr!
CGlobalVars* GetGameGlobals()
{
	INetworkGameServer* server = g_pNetworkServerService->GetIGameServer();

	if (!server)
		return nullptr;

	return g_pNetworkServerService->GetIGameServer()->GetGlobals();
}

#if 0
// Currently unavailable, requires hl2sdk work!
ConVar sample_cvar("sample_cvar", "42", 0);
#endif

CON_COMMAND_F(sample_command, "Sample command", FCVAR_NONE)
{
	//META_CONPRINTF("Sample command called by %d. Command: %s\n", context.GetPlayerSlot(), args.GetCommandString());
}

PLUGIN_EXPOSE(WCSSkills, g_WCSSkills);
bool WCSSkills::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
	this->ismm = ismm;
	try {
		Paths::init();
	}
	catch (Paths::WrongDataLocation& e) {
		return false;
	}
	events::EventManager::init();
	functions::Functions::init();
	try {
		managers::InfoManager::init();
	}
	catch (managers::InfoManagerRecreating& e) {
		//META_CONPRINTF("InfoManager re-creation!\n");
		return false;
	}
	
	PLUGIN_SAVEVARS();

	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer, INTERFACEVERSION_VENGINESERVER);
	GET_V_IFACE_CURRENT(GetEngineFactory, icvar, ICvar, CVAR_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetServerFactory, server, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL);
	GET_V_IFACE_ANY(GetServerFactory, gameclients, IServerGameClients, INTERFACEVERSION_SERVERGAMECLIENTS);
	GET_V_IFACE_ANY(GetEngineFactory, g_pNetworkServerService, INetworkServerService, NETWORKSERVERSERVICE_INTERFACE_VERSION);

	// Currently doesn't work from within mm side, use GetGameGlobals() in the mean time instead
	// gpGlobals = ismm->GetCGlobals();

	// Required to get the IMetamodListener events
	g_SMAPI->AddListener(this, this);

	SH_ADD_HOOK_MEMFUNC(IServerGameDLL, GameFrame, server, this, &WCSSkills::Hook_GameFrame, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientActive, gameclients, this, &WCSSkills::Hook_ClientActive, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientDisconnect, gameclients, this, &WCSSkills::Hook_ClientDisconnect, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientPutInServer, gameclients, this, &WCSSkills::Hook_ClientPutInServer, true);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientSettingsChanged, gameclients, this, &WCSSkills::Hook_ClientSettingsChanged, false);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, OnClientConnected, gameclients, this, &WCSSkills::Hook_OnClientConnected, false);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientConnect, gameclients, this, &WCSSkills::Hook_ClientConnect, false);
	SH_ADD_HOOK_MEMFUNC(IServerGameClients, ClientCommand, gameclients, this, &WCSSkills::Hook_ClientCommand, false);

	g_pCVar = icvar;
	ConVar_Register(FCVAR_RELEASE | FCVAR_CLIENT_CAN_EXECUTE | FCVAR_GAMEDLL);
	return true;
}

bool WCSSkills::Unload(char* error, size_t maxlen)
{
	events::EventManager::close();
	functions::Functions::close();
	try {
		managers::InfoManager::close();
	}
	catch (managers::InfoManagerReclosing& e) {
		//META_CONPRINTF("InfoManager re-close-ing!\n");
		return false;
	}

	SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, GameFrame, server, this, &WCSSkills::Hook_GameFrame, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientActive, gameclients, this, &WCSSkills::Hook_ClientActive, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientDisconnect, gameclients, this, &WCSSkills::Hook_ClientDisconnect, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientPutInServer, gameclients, this, &WCSSkills::Hook_ClientPutInServer, true);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientSettingsChanged, gameclients, this, &WCSSkills::Hook_ClientSettingsChanged, false);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, OnClientConnected, gameclients, this, &WCSSkills::Hook_OnClientConnected, false);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientConnect, gameclients, this, &WCSSkills::Hook_ClientConnect, false);
	SH_REMOVE_HOOK_MEMFUNC(IServerGameClients, ClientCommand, gameclients, this, &WCSSkills::Hook_ClientCommand, false);

	this->ismm = nullptr;
	return true;
}

void WCSSkills::AllPluginsLoaded()
{
	/* This is where we'd do stuff that relies on the mod or other plugins
	 * being initialized (for example, cvars added and events registered).
	 */
}

ISmmAPI* WCSSkills::getISmmAPI(){
	return this->ismm;
}

void WCSSkills::Hook_ClientActive(CPlayerSlot slot, bool bLoadGame, const char* pszName, uint64 xuid)
{
	//META_CONPRINTF("Hook_ClientActive(%d, %d, \"%s\", %d)\n", slot, bLoadGame, pszName, xuid);
}

void WCSSkills::Hook_ClientCommand(CPlayerSlot slot, const CCommand& args)
{
	//META_CONPRINTF("Hook_ClientCommand(%d, \"%s\")\n", slot, args.GetCommandString());
}

void WCSSkills::Hook_ClientSettingsChanged(CPlayerSlot slot)
{
	//META_CONPRINTF("Hook_ClientSettingsChanged(%d)\n", slot);
}

void WCSSkills::Hook_OnClientConnected(CPlayerSlot slot, const char* pszName, uint64 xuid, const char* pszNetworkID, const char* pszAddress, bool bFakePlayer)
{
	//META_CONPRINTF("Hook_OnClientConnected(%d, \"%s\", %d, \"%s\", \"%s\", %d)\n", slot, pszName, xuid, pszNetworkID, pszAddress, bFakePlayer);
}

bool WCSSkills::Hook_ClientConnect(CPlayerSlot slot, const char* pszName, uint64 xuid, const char* pszNetworkID, bool unk1, CBufferString* pRejectReason)
{
	//META_CONPRINTF("Hook_ClientConnect(%d, \"%s\", %d, \"%s\", %d, \"%s\")\n", slot, pszName, xuid, pszNetworkID, unk1, pRejectReason->ToGrowable()->Get());

	RETURN_META_VALUE(MRES_IGNORED, true);
}

void WCSSkills::Hook_ClientPutInServer(CPlayerSlot slot, char const* pszName, int type, uint64 xuid)
{
	//META_CONPRINTF("Hook_ClientPutInServer(%d, \"%s\", %d, %d, %d)\n", slot, pszName, type, xuid);
}

void WCSSkills::Hook_ClientDisconnect(CPlayerSlot slot, int reason, const char* pszName, uint64 xuid, const char* pszNetworkID)
{
	//META_CONPRINTF("Hook_ClientDisconnect(%d, %d, \"%s\", %d, \"%s\")\n", slot, reason, pszName, xuid, pszNetworkID);
}

void WCSSkills::Hook_GameFrame(bool simulating, bool bFirstTick, bool bLastTick)
{
	/**
	 * simulating:
	 * ***********
	 * true  | game is ticking
	 * false | game is not ticking
	 */
}

void WCSSkills::OnLevelInit(char const* pMapName,
	char const* pMapEntities,
	char const* pOldLevel,
	char const* pLandmarkName,
	bool loadGame,
	bool background)
{
	//META_CONPRINTF("OnLevelInit(%s)\n", pMapName);
}

void WCSSkills::OnLevelShutdown()
{
	//META_CONPRINTF("OnLevelShutdown()\n");
}

bool WCSSkills::Pause(char* error, size_t maxlen)
{
	return true;
}

bool WCSSkills::Unpause(char* error, size_t maxlen)
{
	return true;
}

// Constants
const char* WCSSkills:: GetLicense		(){ return "MIT"										;}
const char* WCSSkills:: GetVersion		(){ return "0.1.0.0"									;}
const char* WCSSkills:: GetDate			(){ return __DATE__										;}
const char* WCSSkills:: GetLogTag		(){ return "WCSS"										;}
const char* WCSSkills:: GetAuthor		(){ return "Articha"									;}
const char* WCSSkills:: GetDescription	(){ return "Warcraft mod based on skills"				;}
const char* WCSSkills:: GetName			(){ return "WCSSkills"									;}
const char* WCSSkills:: GetURL			(){ return "https://github.com/ArtichaTM/CS2_WCSSkills"	;}
