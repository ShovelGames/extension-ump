#if defined(DM_PLATFORM_IOS) || defined(DM_PLATFORM_ANDROID)

#pragma once
#include <dmsdk/sdk.h>

namespace ext_ump {

	enum MessageEvent
	{
		EVENT_INITIALIZE_COMPLETE = 		1,
		EVENT_RESET_COMPLETE = 				2,
		EVENT_COMPLETE = 					3,
		EVENT_ERROR = 						4,
		EVENT_PRIVACY_OPTIONS_COMPLETE = 	5,
		EVENT_PRIVACY_OPTIONS_ERROR = 		6
	};

	struct CallbackData
	{
		MessageEvent event;
	};

	void SetLuaCallback(lua_State* L, int pos);
	void InitializeCallback();
	void FinalizeCallback();
	void UpdateCallback();
	void AddToQueueCallback(MessageEvent event);

}

#endif