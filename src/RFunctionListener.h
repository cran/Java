
# line 11 "RFunctionListener.cweb"
#ifndef RFUNCTION_LISTENER_H
#define RFUNCTION_LISTENER_H
#include <jni.h>

#include "Rinternals.h"
#include "Rdefines.h"

#include "CtoJava.h"
#include "Converters.h"

# line 27 "RFunctionListener.cweb"
JNIEXPORT void JNICALL Java_org_omegahat_R_Java_RManualFunctionActionListener_actionPerformed
  (JNIEnv *, jobject, jobject);

# line 71 "RFunctionListener.cweb"
USER_OBJECT_ getREventCommand(jobject ev, JNIEnv *env);

# line 103 "RFunctionListener.cweb"
/*
   A converter for R objects to an ActionListener interface
   in the form of the class
 */   
jobject
RFunctionConverter(USER_OBJECT_ obj, jclass targetClass, java_type type, char *signature, jboolean isArray, int arrayLen, JNIEnv *env,  RSToJavaConverter *converter);

# line 22 "RFunctionListener.cweb"
#endif


