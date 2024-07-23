#define EXTENSION_NAME UMP
#define LIB_NAME "ump"
#define MODULE_NAME "ump"

#define DLIB_LOG_DOMAIN LIB_NAME
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_IOS) || defined(DM_PLATFORM_ANDROID)

#include "ump_private.h"
#include "ump_callback_private.h"

namespace ext_ump {

	static int Lua_Initialize(lua_State* L)
	{
		DM_LUA_STACK_CHECK(L, 0);
		
		#if defined(DM_DEBUG)
		const char* lua_testDeviceHashedId = luaL_checkstring(L, 1);
		Initialize(lua_testDeviceHashedId);
		#else
		Initialize("");
		#endif

		return 0;
	}

	static int Lua_SetCallback(lua_State* L)
	{
		DM_LUA_STACK_CHECK(L, 0);
		SetLuaCallback(L, 1);
		return 0;
	}

	static int Lua_Reset(lua_State* L)
	{
		DM_LUA_STACK_CHECK(L, 0);

		// Debug only
		#if defined(DM_DEBUG)
		Reset();
		#endif

		return 0;
	}

	static int Lua_ShowConsentForm(lua_State* L) 
	{
		DM_LUA_STACK_CHECK(L, 0);
		ShowConsentForm();
		return 0;
	}

	static int Lua_ShowPrivacyOptionsForm(lua_State* L) 
	{
		DM_LUA_STACK_CHECK(L, 0);
		ShowPrivacyOptionsForm();
		return 0;
	}

	static int Lua_IsPrivacyOptionsRequired(lua_State* L) 
	{
		DM_LUA_STACK_CHECK(L, 1);
		bool status = IsPrivacyOptionsRequired();
		lua_pushboolean(L, status);
		return 1;
	}

	static const luaL_reg Module_methods[] = 
	{
		// 呼ばれるメソッド名, 内部のメソッド名
		{"initialize", Lua_Initialize},
		{"set_callback", Lua_SetCallback},
		{"reset", Lua_Reset},
		{"show_consent_form", Lua_ShowConsentForm},
		{"show_privacy_options_form", Lua_ShowPrivacyOptionsForm},
		{"is_privacy_options_required", Lua_IsPrivacyOptionsRequired},
		{0, 0}
	};

	static void LuaInit(lua_State* L) 
	{
		DM_LUA_STACK_CHECK(L, 0);
		luaL_register(L, MODULE_NAME, Module_methods);

		struct { const char* name; int value; } constants[] = {
			{"EVENT_INITIALIZE_COMPLETE", EVENT_INITIALIZE_COMPLETE},
			{"EVENT_RESET_COMPLETE", EVENT_RESET_COMPLETE},
			{"EVENT_COMPLETE", EVENT_COMPLETE},
			{"EVENT_ERROR", EVENT_ERROR},
			{"EVENT_PRIVACY_OPTIONS_COMPLETE", EVENT_PRIVACY_OPTIONS_COMPLETE},
			{"EVENT_PRIVACY_OPTIONS_ERROR", EVENT_PRIVACY_OPTIONS_ERROR},
		};

		for (const auto& constant : constants)
		{
			lua_pushnumber(L, constant.value);
			lua_setfield(L, -2, constant.name);
		}
		
		lua_pop(L, 1);
	}

} // namespace
	
static dmExtension::Result AppInitializeUmp(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

static dmExtension::Result InitializeUmp(dmExtension::Params* params)
{
	ext_ump::LuaInit(params->m_L);

	ext_ump::Initialize_Ump();
	ext_ump::InitializeCallback();

	return dmExtension::RESULT_OK;
}

static dmExtension::Result UpdateUmp(dmExtension::Params* params)
{
	ext_ump::UpdateCallback();
	return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeUmp(dmExtension::Params* params)
{
	ext_ump::FinalizeCallback();
	return dmExtension::RESULT_OK;
}

#else // unsupported platforms


static dmExtension::Result AppInitializeUmp(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

static dmExtension::Result InitializeUmp(dmExtension::Params* params)
{
	dmLogInfo("Registered extension UMP (null)");
	return dmExtension::RESULT_OK;
}

static dmExtension::Result UpdateUmp(dmExtension::Params* params)
{
	return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeUmp(dmExtension::Params* params)
{
	return dmExtension::RESULT_OK;
}


#endif


static dmExtension::Result AppFinalizeUmp(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}


DM_DECLARE_EXTENSION(UMP, LIB_NAME, AppInitializeUmp, AppFinalizeUmp, InitializeUmp, UpdateUmp, 0, FinalizeUmp)