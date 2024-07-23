#if defined(DM_PLATFORM_ANDROID)

#include <jni.h>
/* Header for class com_defold_ump_Form */

#ifndef COM_DEFOLD_UMP_FORM_H
#define COM_DEFOLD_UMP_FORM_H
#ifdef __cplusplus
extern "C" {
#endif
	/*
	* Class:		com_defold_ump_Form
	* Method:		umpAddToQueue_first_arg
	* Signature:	(ILjava/lang/String;I)V
	*/
	JNIEXPORT void JNICALL Java_com_defold_ump_Form_umpAddToQueue(JNIEnv *, jclass, jint);

#ifdef __cplusplus
}
#endif
#endif

#endif