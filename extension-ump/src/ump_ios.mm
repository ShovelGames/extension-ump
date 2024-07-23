#if defined(DM_PLATFORM_IOS)

#include "ump_private.h"
#include "ump_callback_private.h"

#include <atomic>

#include <UserMessagingPlatform/UserMessagingPlatform.h>

namespace ext_ump
{
	static UIViewController *uiViewController = nil;
	static NSString *testDeviceHashedId = @"";
	static std::atomic<bool> isUmpInitializeCalled;

	void completeForm() {
		bool expected = false;
		if (!isUmpInitializeCalled.compare_exchange_strong(expected, true)) {
			return;
		}

		sendEvent(EVENT_COMPLETE);
	}

	void sendEvent(MessageEvent msg) {
		AddToQueueCallback(msg);
	}

	void Initialize_Ump() {
		UIWindow* window = dmGraphics::GetNativeiOSUIWindow();
		uiViewController = window.rootViewController;
	}

	void Initialize(const char* deviceId) {
		testDeviceHashedId = [NSString stringWithUTF8String:deviceId];
		sendEvent(EVENT_INITIALIZE_COMPLETE);
	}

	void Reset() {
		[UMPConsentInformation.sharedInstance reset];
		sendEvent(EVENT_RESET_COMPLETE);
	}

	void ShowConsentForm() {
		UMPRequestParameters *parameters = [[UMPRequestParameters alloc] init];
		parameters.tagForUnderAgeOfConsent = NO;

		// Testing
		if (testDeviceHashedId.length > 0) {
			UMPDebugSettings *debugSettings = [[UMPDebugSettings alloc] init];
			debugSettings.testDeviceIdentifiers = @[testDeviceHashedId];
			debugSettings.geography = UMPDebugGeographyEEA;
			parameters.debugSettings = debugSettings;
		}

		[UMPConsentInformation.sharedInstance
		requestConsentInfoUpdateWithParameters:parameters
				completionHandler:^(NSError *_Nullable requestConsentError) {
			if (requestConsentError) {
				NSLog(@"UMP requestConsentError: %@", requestConsentError.localizedDescription);
				sendEvent(EVENT_ERROR);
				return;
			}

			[UMPConsentForm loadAndPresentIfRequiredFromViewController:uiViewController
				completionHandler:^(NSError *loadAndPresentError) {
				if (loadAndPresentError) {
					NSLog(@"UMP loadAndPresentError: %@", loadAndPresentError.localizedDescription);
					sendEvent(EVENT_ERROR);
					return;
				}

				// Consent has been gathered.
				if (UMPConsentInformation.sharedInstance.canRequestAds) {
					completeForm();
				}
			}];
		}];

		// Check if you can initialize the Google Mobile Ads SDK in parallel
		// while checking for new consent information. Consent obtained in
		// the previous session can be used to request ads.
		if (UMPConsentInformation.sharedInstance.canRequestAds) {
			completeForm();
		}
	}

	void ShowPrivacyOptionsForm() {
		// REQUIRED only
		if (IsPrivacyOptionsRequired()) {
			[UMPConsentForm presentPrivacyOptionsFormFromViewController:uiViewController
			completionHandler:^(NSError *_Nullable formError) {
				if (formError) {
					NSLog(@"UMP formError: %@", formError.localizedDescription);
					sendEvent(EVENT_PRIVACY_OPTIONS_ERROR);
					return;
				}

				sendEvent(EVENT_PRIVACY_OPTIONS_COMPLETE);
			}];
		}
	}

	bool IsPrivacyOptionsRequired() {
		return UMPConsentInformation.sharedInstance.privacyOptionsRequirementStatus ==
		UMPPrivacyOptionsRequirementStatusRequired;
	}
}

#endif