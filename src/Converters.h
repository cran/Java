# line 87 "Converters.cweb"
#ifndef CONVERTERS_JAVA_H
#define CONVERTERS_JAVA_H

#include <jni.h>
#include "Rinternals.h"
#include "CtoJava.h"


# line 109 "Converters.cweb"
typedef struct _RSFromJavaConverter RSFromJavaConverter;

typedef jboolean (*FromJavaConverterMatch)(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter);

typedef USER_OBJECT_ (*FromJavaConverter)(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter);

struct _RSFromJavaConverter {
   /* Determines whether this element is to be used for
      converting the specified object. If this returns
      false, it is skipped. Otherwise, it is used to
      perform the conversion.
    */
 FromJavaConverterMatch match;

   /* The method that performs the computation
      to convert the Java object to an R/S object.
    */

 FromJavaConverter converter;

   /* Handle arrays automaticall by matching on the first
      element and converting the array element wise using this
      elements converter.
      If this is false, then the match and converter routines
      can and must handle arrays differently.
    */
 jboolean autoArray;

    /*
      Available to the match and converter routines for storing
      local data.
     */
 void *userData;

 char *description;


     /* Next element in the linked list of converters. */
 RSFromJavaConverter *next;
};

extern RSFromJavaConverter *FromJavaConverters;




typedef struct _RSToJavaConverter RSToJavaConverter;

typedef jboolean (*ToJavaConverterMatch)(USER_OBJECT_ obj, jclass type, java_type, char *signature, jboolean isArray, int arrayLen, JNIEnv *,  RSToJavaConverter *);
typedef jobject (*ToJavaConverter)(USER_OBJECT_ obj, jclass type, java_type, char *signature, jboolean isArray, int arrayLen, JNIEnv *,  RSToJavaConverter *);

struct _RSToJavaConverter {

  ToJavaConverterMatch match;
  ToJavaConverter      converter;
  jboolean             autoArray;   
  void                *userData;

  char *description;  

  RSToJavaConverter *next;
};    

extern RSToJavaConverter *ToJavaConverters;



# line 96 "Converters.cweb"
#ifdef __cplusplus
extern "C" {
#endif
# line 177 "Converters.cweb"
USER_OBJECT_
userLevelJavaConversion(JNIEnv *env, jobject obj, const char *dataType, jboolean isArray, int arrayLength, int *ok);

# line 263 "Converters.cweb"
RSFromJavaConverter *removeFromJavaConverterByIndex(int which);
RSFromJavaConverter *removeFromJavaConverterByDescription(char *desc, int *which);

# line 396 "Converters.cweb"
jboolean 
SimpleExactClassMatch(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter);
jboolean
InstanceOfFromClassMatch(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter);
jboolean
AssignableFromClassMatch(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter);

typedef enum {InstanceOf=1, Equals, AssignableFrom} JavaConverterMatch;

# line 463 "Converters.cweb"
/*
 Routine that establishes the basic converters for
 Method and Constructor, the reference classes and
 also any that convert from R to Java.
*/   
void RS_JAVA(registerDefaultConverters)(JNIEnv *env);

# line 505 "Converters.cweb"
/*
  High-level mechanism for registering a converter for Java to R objects.
 */
RSFromJavaConverter *
addFromJavaConverterInfo(FromJavaConverterMatch match, FromJavaConverter converter, jboolean autoArray, void *userData, char *description, int *index);

# line 535 "Converters.cweb"
RSToJavaConverter * addToJavaConverterInfo(ToJavaConverterMatch match, ToJavaConverter converter, jboolean autoArray, void *userData, char *description);
int addToJavaConverter(RSToJavaConverter *cvt);
# line 577 "Converters.cweb"
int
getNumConverters();

# line 664 "Converters.cweb"
char *fromJavaConverterDescription(RSFromJavaConverter *cvt);

# line 710 "Converters.cweb"
/*
 Can be used by a converter that discovers it is not capable
 of converting a Java object to an R object.
 This calls the assign method in the Omega Interface Manager
 which stores the value in the anonymous reference database
 and returns an AnonymousReference object identifying it.
 */   
USER_OBJECT_ anonymousAssign(JNIEnv *env, jobject obj, jclass klass);

# line 755 "Converters.cweb"
const char *initAssignMethod(JNIEnv *env);
     
# line 772 "Converters.cweb"
/*
 Routine to traverse the list of converters from R to Java and check
 whether any of them applies to the object being converted and the target class.
 The first that does is applied and expected to create and return a Java object.
 */   
jobject userLevelToJavaConversion(JNIEnv *env, USER_OBJECT_ obj, jclass klass, java_type which, char *sig, int arrayLen, jboolean isArray, int *done);

# line 811 "Converters.cweb"
/*
 The basic matching for determining whether a converter applies
 for turning an R object to Java. This compares the name of the
 target class, given by the argument signature, and the name
 stored in the converter's userData field.
 */   
jboolean SimpleRTargetMatch(USER_OBJECT_ obj, jclass targetClass, java_type type, char *signature, jboolean isArray, int arrayLen, JNIEnv *env,  RSToJavaConverter *This);

# line 836 "Converters.cweb"
USER_OBJECT_ RS_JAVA(ReferenceConverter)(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter);

# line 848 "Converters.cweb"
jboolean 
RS_JAVA(ReferenceClassMatch)(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter);

# line 871 "Converters.cweb"
USER_OBJECT_ 
RS_JAVA(PropertyConverter)(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter);

# line 100 "Converters.cweb"
#ifdef __cplusplus
}
#endif

#endif

