# line 9 "RReferenceCall.cweb"
#include "RReferenceCall.h"
#include "RtoJava.h"
#include "RStoJava.h"
#include "RForeignReference.h"

#define VMENV (*env)->

# line 53 "RReferenceCall.cweb"
 enum {IDENTIFIER=0, METHOD_NAME, RETURN_TYPE, SIGNATURE, THIS, ARGUMENTS, NUM_SLOTS};

# line 133 "RReferenceCall.cweb"
 static SEXP defaultMethodHandler = NULL;


# line 76 "RReferenceCall.cweb"
JNIEXPORT jobject JNICALL Java_org_omegahat_R_Java_RForeignReference_eval
  (JNIEnv *env, jobject rref, jstring jid, jobjectArray args, jstring jmethodName, jstring jreturnClass, jobjectArray  jsignature)
{
 extern int R_PPStackTop, R_EvalDepth;
 SEXP details, userAns;
 int numArgs = 0;
 char *sig;
 jobject ans = NULL_JAVA_OBJECT;
 int tmpStackTop;

  PROTECT(details = NEW_LIST((int) NUM_SLOTS));

    RS_JAVA(addStringValue)(env, jid,          details, IDENTIFIER);
    RS_JAVA(addStringValue)(env, jmethodName,  details, METHOD_NAME);
    RS_JAVA(addStringValue)(env, jreturnClass, details, RETURN_TYPE);
    RS_JAVA(addStringValues)(env, jsignature,   details, SIGNATURE);

 
     /* Convert the arguments. */
   if(args != NULL_JAVA_OBJECT)
     numArgs = VMENV GetArrayLength(env, args);
 
   if(numArgs > 0) {
     USER_OBJECT_ remoteArgs;
     jobject el;
     int i;
     remoteArgs = NEW_LIST(numArgs) ;
     VECTOR(details)[ARGUMENTS] = remoteArgs;       
     for(i = 0; i < numArgs ; i++) {
        el = VMENV GetObjectArrayElement(env, args, i);
        VECTOR(VECTOR(details)[ARGUMENTS])[i] = s_from_java_basic(env, el, NULL, TRUE, TRUE);
     }
   }
  
   /* Convert the `this' object to a reference. */
  tmpStackTop = R_PPStackTop;
  PROTECT(userAns = RJava(invokeRReferenceMethod)(details));
   if(GET_LENGTH(userAns) == 2) {
      /* An error. */
     const char *msg = strdup(CHAR_DEREF(STRING(VECTOR(userAns)[1])[0]));
     UNPROTECT(2); /* details */
     R_PPStackTop  = tmpStackTop;
     throw_Java_exception(msg, NULL,  env);
   } else {
       /* Convert result. */
    if(GET_LENGTH(VECTOR(userAns)[0]) > 0) {
     ans = s_to_java_basic(env, VECTOR(userAns)[0], NULL, &sig);
    }
   }

  UNPROTECT(2); /* details, userAns */
  return(ans);
}    


# line 226 "RReferenceCall.cweb"
int
RS_JAVA(addStringValue)(JNIEnv *env,  jstring str, USER_OBJECT_ details, int which)
{
  const char *tmp;
  jboolean isCopy;
    VECTOR(details)[which]  = NEW_CHARACTER(1);
    if(str == NULL_JAVA_OBJECT) {
       return(which);
    }
  
    tmp = VMENV GetStringUTFChars(env, str, &isCopy);
    STRING(VECTOR(details)[which])[0] = mkChar(tmp);
    if(isCopy == JNI_TRUE) {
         VMENV ReleaseStringUTFChars(env, str, tmp);
    }

  return(which);
}

# line 254 "RReferenceCall.cweb"
int
RS_JAVA(addStringValues)(JNIEnv *env, jobjectArray strArray, USER_OBJECT_ details, int slot)
{
 int n, i;
 jboolean isCopy;
 const char *tmp;
 jobject el;
 USER_OBJECT_ els;

  n = VMENV GetArrayLength(env, strArray);
    
  els = LIST_POINTER(details)[slot] = NEW_CHARACTER(n);
  for(i = 0; i < n ; i++) {
    el = VMENV GetObjectArrayElement(env, strArray, i);     
    tmp = VMENV GetStringUTFChars(env, el, &isCopy);
    CHARACTER_DATA(els)[i] = COPY_TO_USER_STRING(tmp);
    if(isCopy == JNI_TRUE) {
         VMENV ReleaseStringUTFChars(env, el, tmp);
    }    
  }

 return(slot);
}    


# line 151 "RReferenceCall.cweb"
USER_OBJECT_
RJava(defaultHandlerFunction)()
{
   if(defaultMethodHandler == NULL) {
       PROBLEM "the Java callback handler has not been registered!"
       ERROR;
    }
   return(defaultMethodHandler);
}

# line 163 "RReferenceCall.cweb"
USER_OBJECT_
RJava(setDefaultHandlerFunction)(USER_OBJECT_ handler)
{
 USER_OBJECT_ old = defaultMethodHandler;   
   if(old) {
     R_PreserveObject(handler);
   } else
     old = R_NilValue;
 
    defaultMethodHandler = handler;

 return(old);     
}    

# line 186 "RReferenceCall.cweb"
SEXP
RJava(invokeRReferenceMethod)(SEXP opArgs)
{
 SEXP fun;
  SEXP call;
  SEXP ans;

  fun = RJava(defaultHandlerFunction)();

   /* Check whether we have a function. If not, assume it is a list and
      that the first element is a function.
    */
  if(isFunction(fun) == FALSE) {
    if(IS_VECTOR(fun)) {
      fun = VECTOR(fun)[0];
     }
  }

 call = RJava(createCall)(fun, opArgs);
 
  PROTECT(call);
   ans = eval(call, R_GlobalEnv);

  UNPROTECT(1);
  return(ans);    
}    
     

# line 285 "RReferenceCall.cweb"
SEXP
RJava(createCall)(SEXP fun, SEXP opArgs)
{
 SEXP c, call;
 int i;
 long n;
 
   /* Generate the appropriate R object for invoking the function with these arguments. */
  n = GET_LENGTH(opArgs);
  if(n  > 0) {
    PROTECT(c = call = allocList(n));
    for (i = 0; i < n; i++) {
      CAR(c) = VECTOR(opArgs)[i];
      c = CDR(c);
    }

    call = LCONS(fun, call);
    UNPROTECT(1);
  } else  {
     call = allocVector(LANGSXP,1);
     CAR(call) = fun;
  }  
 
 return(call);
}




# line 322 "RReferenceCall.cweb"
USER_OBJECT_
getRReference(jobject ref, JNIEnv *env)
{
 jboolean isCopy;
 const char *tmp;
 USER_OBJECT_ name, e, ans;

 USER_OBJECT_ f = RJava_defaultHandlerFunction();
 USER_OBJECT_ getReferenceFun;
 
    /* Get the getReference() function and set this as the function
       to call.
     */
 getReferenceFun = LIST_POINTER(f)[4];


    /* Now call this function with the single argument, the
       reference name/identifier.
     */
 PROTECT(e = allocVector(LANGSXP, 2));   
 CAR(e) = getReferenceFun;

     /* Convert the java string identifying the reference to an R object. */
 tmp = VMENV GetStringUTFChars(env, (jstring) ref, &isCopy);
 PROTECT(name = NEW_CHARACTER(1));
 CHARACTER_DATA(name)[0] = COPY_TO_USER_STRING(tmp);
 if(isCopy)
   VMENV ReleaseStringUTFChars(env, (jstring) ref, tmp); 
 CAR(CDR(e)) = name;

      /* Now ready to call the getReference() function. */
 ans =  eval(e, R_GlobalEnv);

 return(ans);
}    



# line 374 "RReferenceCall.cweb"
jstring
getJavaReference(jobject reference,  JNIEnv *env)
{
 jclass klass = VMENV GetObjectClass(env, reference);
 jmethodID mid;
 jstring ans;
   mid = VMENV GetMethodID(env, klass, "getName", "()Ljava/lang/String;");

  ans = VMENV CallObjectMethod(env, reference, mid);

  return(ans);
}    


