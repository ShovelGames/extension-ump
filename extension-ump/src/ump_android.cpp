#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/dlib/android.h>
#include "ump_private.h"
#include "ump_callback_private.h"
#include "com_defold_ump_Form.h"


JNIEXPORT void JNICALL Java_com_defold_ump_Form_umpAddToQueue(JNIEnv * env, jclass cls, jint jmsg)
{
	ext_ump::AddToQueueCallback((ext_ump::MessageEvent)jmsg);
}

namespace ext_ump
{
	struct UMP
	{
		jobject		m_Form;

		jmethodID	m_Initialize;
		jmethodID	m_Reset;
		jmethodID	m_ShowConsentForm;
		jmethodID	m_ShowPrivacyOptionsForm;
		jmethodID	m_IsPrivacyOptionsRequired;
	};

	static UMP		g_ump;

	static void CallVoidMethod(jobject instance, jmethodID method)
	{
		dmAndroid::ThreadAttacher threadAttacher;
		JNIEnv* env = threadAttacher.GetEnv();

		env->CallVoidMethod(instance, method);
	}

	static void CallVoidMethodChar(jobject instance, jmethodID method, const char* cstr)
	{
		dmAndroid::ThreadAttacher threadAttacher;
		JNIEnv* env = threadAttacher.GetEnv();

		jstring jstr = env->NewStringUTF(cstr);
		env->CallVoidMethod(instance, method, jstr);
		env->DeleteLocalRef(jstr);
	}
	
	static bool CallBoolMethod(jobject instance, jmethodID method)
	{
		dmAndroid::ThreadAttacher threadAttacher;
		JNIEnv* env = threadAttacher.GetEnv();

		jboolean return_value = (jboolean)env->CallBooleanMethod(instance, method);
		return JNI_TRUE == return_value;
	}
	
	void Initialize_Ump()
	{
		dmAndroid::ThreadAttacher threadAttacher;
		JNIEnv* env = threadAttacher.GetEnv();
		jclass cls = dmAndroid::LoadClass(env, "com.defold.ump.Form");

		g_ump.m_Initialize = env->GetMethodID(cls, "initialize", "(Ljava/lang/String;)V");
		g_ump.m_Reset = env->GetMethodID(cls, "reset", "()V");
		g_ump.m_ShowConsentForm = env->GetMethodID(cls, "showConsentForm", "()V");
		g_ump.m_ShowPrivacyOptionsForm = env->GetMethodID(cls, "showPrivacyOptionsForm", "()V");
		g_ump.m_IsPrivacyOptionsRequired = env->GetMethodID(cls, "isPrivacyOptionsRequired", "()Z");

		jmethodID jni_constructor = env->GetMethodID(cls, "<init>", "(Landroid/app/Activity;)V");
		g_ump.m_Form = env->NewGlobalRef(env->NewObject(cls, jni_constructor, threadAttacher.GetActivity()->clazz));
	}

	void Initialize(const char* testDeviceHashedId)
	{
		CallVoidMethodChar(g_ump.m_Form, g_ump.m_Initialize, testDeviceHashedId);
	}

	void Reset()
	{
		CallVoidMethod(g_ump.m_Form, g_ump.m_Reset);
	}

	void ShowConsentForm()
	{
		CallVoidMethod(g_ump.m_Form, g_ump.m_ShowConsentForm);
	}

	void ShowPrivacyOptionsForm()
	{
		CallVoidMethod(g_ump.m_Form, g_ump.m_ShowPrivacyOptionsForm);
	}

	bool IsPrivacyOptionsRequired()
	{
		return CallBoolMethod(g_ump.m_Form, g_ump.m_IsPrivacyOptionsRequired);
	}
}

#endif