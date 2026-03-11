package com.defold.ump;

import android.app.Activity;
import android.util.Log;
import java.util.concurrent.atomic.AtomicBoolean;

import com.google.android.ump.ConsentInformation;
import com.google.android.ump.ConsentInformation.PrivacyOptionsRequirementStatus;
import com.google.android.ump.ConsentRequestParameters;
import com.google.android.ump.ConsentDebugSettings;
import com.google.android.ump.UserMessagingPlatform;
import com.google.android.ump.FormError;
import com.google.android.ump.ConsentForm;

public class Form {
	private static final String TAG = "UMP";

	public static native void umpAddToQueue(int msg);

	// duplicate of enums from admob_callback_private.h:
	// CONSTANTS:
	private static final int EVENT_INITIALIZE_COMPLETE = 1;
	private static final int EVENT_RESET_COMPLETE = 2;
	private static final int EVENT_COMPLETE = 3;
	private static final int EVENT_ERROR = 4;
	private static final int EVENT_PRIVACY_OPTIONS_COMPLETE = 5;
	private static final int EVENT_PRIVACY_OPTIONS_ERROR = 6;

	private Activity activity;
	private String testDeviceHashedId;
	private ConsentInformation consentInformation;
	private final AtomicBoolean isUmpInitializeCalled = new AtomicBoolean(false);

	private void completeForm() {
		if (isUmpInitializeCalled.getAndSet(true)) {
			return;
		}

		sendEvent(EVENT_COMPLETE);
	}

	private void sendEvent(int msg) {
		umpAddToQueue(msg);
	}

	public Form(Activity activity) {
		this.activity = activity;
	}

	public void initialize(final String testDeviceHashedId) {
		consentInformation = UserMessagingPlatform.getConsentInformation(activity);
		this.testDeviceHashedId = testDeviceHashedId;

		sendEvent(EVENT_INITIALIZE_COMPLETE);
	}

	public void reset() {
		consentInformation.reset();
		sendEvent(EVENT_RESET_COMPLETE);
	}

	public void showConsentForm() {
		if (consentInformation == null) {
			Log.w(TAG, "ConsentInformation is not initialized.");
			sendEvent(EVENT_ERROR);
			return;
		}

		// ToDo setTagForUnderAgeOfConsent
		ConsentRequestParameters params = new ConsentRequestParameters.Builder()
				.setTagForUnderAgeOfConsent(false)
				.build();

		// Testing
		if (this.testDeviceHashedId.length() > 0) {
			ConsentDebugSettings debugSettings = new ConsentDebugSettings.Builder(activity)
				.setDebugGeography(ConsentDebugSettings.DebugGeography.DEBUG_GEOGRAPHY_EEA)
				.addTestDeviceHashedId(this.testDeviceHashedId)
				.build();

				params = new ConsentRequestParameters
						.Builder()
						.setConsentDebugSettings(debugSettings)
						.setTagForUnderAgeOfConsent(false)
						.build();
		}

		consentInformation.requestConsentInfoUpdate(
				activity,
				params,
				(ConsentInformation.OnConsentInfoUpdateSuccessListener) () -> {
					UserMessagingPlatform.loadAndShowConsentFormIfRequired(
							activity,
							(ConsentForm.OnConsentFormDismissedListener) loadAndShowError -> {
								if (loadAndShowError != null) {
									// Consent gathering failed.
									Log.w(TAG, String.format("%s: %s",
										loadAndShowError.getErrorCode(),
										loadAndShowError.getMessage()));
									sendEvent(EVENT_ERROR);
								}

								// Consent has been gathered.
								if (consentInformation.canRequestAds()) {
									completeForm();
								}
							});
				},
				(ConsentInformation.OnConsentInfoUpdateFailureListener) requestConsentError -> {
					// Consent gathering failed.
					Log.w(TAG, String.format("%s: %s",
						requestConsentError.getErrorCode(),
						requestConsentError.getMessage()));
					sendEvent(EVENT_ERROR);
				}
		);

		// Check if you can initialize the Google Mobile Ads SDK in parallel
		// while checking for new consent information. Consent obtained in
		// the previous session can be used to request ads.
		if (consentInformation.canRequestAds()) {
			completeForm();
		}
	}

	public void showPrivacyOptionsForm() {
		// REQUIRED only
		if (isPrivacyOptionsRequired()) {
			activity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					UserMessagingPlatform.showPrivacyOptionsForm(
						activity,
						formError -> {
							if (formError != null) {
								Log.w(TAG, String.format("%s: %s",
										formError.getErrorCode(),
										formError.getMessage()));
								sendEvent(EVENT_PRIVACY_OPTIONS_ERROR);
							} else {
								sendEvent(EVENT_PRIVACY_OPTIONS_COMPLETE);
							}
						}
					);
				}
			});
		}
	}

	public boolean isPrivacyOptionsRequired() {
		return consentInformation.getPrivacyOptionsRequirementStatus() == PrivacyOptionsRequirementStatus.REQUIRED;
	}
}