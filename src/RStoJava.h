# line 148 "RStoJava.cweb"
#ifndef RSTOJAVA_H
#define RSTOJAVA_H

#if !defined(_R_)
#include "S.h"
#endif

#include <jni.h>

# line 294 "RStoJava.cweb"
USER_OBJECT_
RS_ConstructorJavaCall(USER_OBJECT_ className, USER_OBJECT_ args,
                         USER_OBJECT_ argNames, USER_OBJECT_ returnValueName,
                           USER_OBJECT_ signatures, USER_OBJECT_ convert);

# line 365 "RStoJava.cweb"
USER_OBJECT_
RS_OmegahatExpression(USER_OBJECT_ expressionString, 
                       USER_OBJECT_ userArgs, USER_OBJECT_ userArgNames,
                         USER_OBJECT_ returnValueName, 
                          USER_OBJECT_ signatures, USER_OBJECT_ convert);

# line 409 "RStoJava.cweb"
USER_OBJECT_ convertAnswer(JNIEnv *env, USER_OBJECT_ returnValueName, jobject val);


# line 480 "RStoJava.cweb"
jobjectArray
argsToArray(JNIEnv *env, USER_OBJECT_ userArgs, USER_OBJECT_ signatures);

# line 516 "RStoJava.cweb"
jobjectArray
argNamesToArray(JNIEnv *env, USER_OBJECT_ userArgNames, int addExtraValue);

# line 565 "RStoJava.cweb"
jobjectArray JavaObjectArray(JNIEnv *env, int length);

# line 590 "RStoJava.cweb"
jobjectArray JavaStringArray(JNIEnv *env, int length);

# line 622 "RStoJava.cweb"
JNIEnv *SgetJNIEnv();

# line 659 "RStoJava.cweb"
extern void *s_to_java_basic(JNIEnv *env, USER_OBJECT_ obj, char *sig, char **actual_sig_ptr);


# line 738 "RStoJava.cweb"
void *convertToJava(JNIEnv *env, USER_OBJECT_ obj, jboolean isArray, java_type whichType , long n) ;

# line 987 "RStoJava.cweb"
char * getJavaSignature(USER_OBJECT_ obj, jboolean *isReference);

# line 1050 "RStoJava.cweb"
USER_OBJECT_ javaToUserString(JNIEnv *env, jobject val);

# line 1074 "RStoJava.cweb"
jclass ManagerFindClass(JNIEnv *env, char *className);


# line 159 "RStoJava.cweb"
#ifdef __cplusplus
extern "C" {
#endif
  
# line 191 "RStoJava.cweb"
USER_OBJECT_
RS_genericJavaCall(USER_OBJECT_ qualifier, USER_OBJECT_ methodName,
                   USER_OBJECT_ userArgs,
                    USER_OBJECT_ userArgNames, USER_OBJECT_ returnValueName,
                     USER_OBJECT_ signatures, USER_OBJECT_ convert);



# line 1101 "RStoJava.cweb"
void RS_JAVA(isJVMInitialized)();

# line 163 "RStoJava.cweb"
#ifdef __cplusplus
}
#endif


# line 178 "RStoJava.cweb"
#define NAMED_JAVA_REFERENCE_SIG      "N"
#define ANONYMOUS_JAVA_REFERENCE_SIG  "A"
#define BASE_JAVA_REFERENCE_SIG       "R"

#define NAMED_R_REFERENCE_SIG      "W"
#define ANONYMOUS_R_REFERENCE_SIG  "X"

# line 187 "RStoJava.cweb"
#define VMENV (*env)->




# line 172 "RStoJava.cweb"
#endif


