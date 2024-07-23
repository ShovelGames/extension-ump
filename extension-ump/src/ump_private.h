#if defined(DM_PLATFORM_IOS) || defined(DM_PLATFORM_ANDROID)

#pragma once

namespace ext_ump {

	const char* GetTestDeviceHashedId();

	void Initialize(const char* testDeviceHashedId);
	void Initialize_Ump();
	void Reset();
	void ShowConsentForm();
	void ShowPrivacyOptionsForm();
	bool IsPrivacyOptionsRequired();

}

#endif