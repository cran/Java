# line 32 "RReferenceCall.cweb"
#ifndef RREFERENCECALL_H
#define RREFERENCECALL_H


# line 45 "RReferenceCall.cweb"
#include "RForeignReference.h"
#include "Rdefines.h"
#include "Rinternals.h"
#include "ErrorHandling.h"


# line 37 "RReferenceCall.cweb"
#define RJava(x) RJava_##x

# line 137 "RReferenceCall.cweb"
USER_OBJECT_ RJava(defaultHandlerFunction)();

# line 179 "RReferenceCall.cweb"
SEXP RJava(invokeRReferenceMethod)(SEXP opArgs);

# line 215 "RReferenceCall.cweb"
int RS_JAVA(addStringValue)(JNIEnv *env,  jstring str, USER_OBJECT_ details, int which);
int RS_JAVA(addStringValues)(JNIEnv *env, jobjectArray strArray, USER_OBJECT_ details, int slot);

# line 281 "RReferenceCall.cweb"
SEXP RJava(createCall)(SEXP fun, SEXP opArgs);

# line 315 "RReferenceCall.cweb"
USER_OBJECT_
getRReference(jobject ref, JNIEnv *env);

# line 362 "RReferenceCall.cweb"
/*
  This  calls the Java method getName() on the specified
  ForeignReference to retrieve the identifier for the
  R/S reference represented by that object.
 */   
jstring getJavaReference(jobject reference,  JNIEnv *env);


# line 40 "RReferenceCall.cweb"
#endif

