# line 21 "Reflectance.cweb"
#ifndef RS_JAVA_REFLECTANCE_H
#define RS_JAVA_REFLECTANCE_H

#include "Rinternals.h"
#include "Rdefines.h"
#include "Converters.h"


typedef struct {

 jmethodID getClass;
 jmethodID getParameters;
 jmethodID getExceptions;
 jmethodID getModifiers;
 
     /* Not used for constructor, but only for Method. */
 jmethodID getName; /* Available for */
 jmethodID getReturnType; 
    
} ReflectanceMethodIDs;    

# line 73 "Reflectance.cweb"
USER_OBJECT_ 
RS_JAVA(MethodConverter)(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter);

# line 205 "Reflectance.cweb"
const char *getClassName(JNIEnv *env, jclass klass, jboolean *isCopy);

# line 223 "Reflectance.cweb"
char * initReflectanceMethods(JNIEnv *env);


# line 44 "Reflectance.cweb"
#endif

