# line 148 "RFunctionListener.cweb"
#include "RManualFunctionActionListener.h"
#include "RFunctionListener.h"

#include "RReferenceCall.h" /* RJava_defaultHandlerFunction. */
#include "RSCommon.h"       /* CHAR_DEREF */

#include "RtoJava.h"
# line 32 "RFunctionListener.cweb"
JNIEXPORT void JNICALL
Java_org_omegahat_R_Java_RManualFunctionActionListener_actionPerformed(JNIEnv *env, jobject This, jobject ev)   
{
 jobject jref; 
 USER_OBJECT_ rref, e, arg;
      /* Get the name of the R reference from This object */
    jref = getJavaReference(This,  env);

      /* Resolve the R reference, getting its value, not the entire
         reference. 
       */
    rref = getRReference(jref, env);
 
      /* Now, turn the event into an argument by getting its actionCommand. */
   /* arg = getREventCommand(ev, env); */
      /* Or alternatively, stick the event argument into the anonymous
         database of the manager and pass the reference as an argument to the
         R function.
         When does it get released.
       */
    PROTECT(arg = anonymousAssign(env, ev, NULL_JAVA_OBJECT));

      /* Create a call to the function that is the R reference,
         passing it the action command as its only argument.
       */
    e = allocVector(LANGSXP, 2);
    PROTECT(e);
    CAR(e) = rref;
    CAR(CDR(e)) = arg;

      /* Evaluate the call to the R function.
         Ignore the return value.
       */
    eval(e, R_GlobalEnv);
    UNPROTECT(2);
}    

# line 77 "RFunctionListener.cweb"
USER_OBJECT_
getREventCommand(jobject ev, JNIEnv *env)
{
 USER_OBJECT_ ans;    
 jmethodID mid;
 const char *tmp;
 jboolean isCopy;
 jobject jstr;
  mid = VMENV GetMethodID(env, VMENV GetObjectClass(env, ev),"getActionCommand","()Ljava/lang/String;");

  if(mid == NULL)
     return(NULL_USER_OBJECT);

 jstr = VMENV CallObjectMethod(env, ev, mid);
 tmp = VMENV GetStringUTFChars(env, jstr, &isCopy);
 PROTECT(ans = NEW_CHARACTER(1));
 CHARACTER_DATA(ans)[0] = COPY_TO_USER_STRING(tmp);
 if(isCopy)
   VMENV ReleaseStringUTFChars(env, jstr, tmp);
 UNPROTECT(1);

return(ans); 
}    

# line 115 "RFunctionListener.cweb"
jobject
RFunctionConverter(USER_OBJECT_ obj, jclass targetClass, java_type type, char *signature, jboolean isArray, int arrayLen, JNIEnv *env,  RSToJavaConverter *converter)
{
  jclass klass;
  jobject arg;
  jobject val;
  jmethodID constructor;
  char *rrefName;

   PROTECT( obj = createRAnonymousReference(obj));
/*
   fprintf(stderr, "Converting R object to %s\n", signature);fflush(stderr);
*/   
    klass = VMENV FindClass(env, "org/omegahat/R/Java/RFunctionActionListener");
    rrefName =   CHAR_DEREF(CHARACTER_DATA(LIST_POINTER(obj)[1])[0]);

    arg = VMENV NewStringUTF(env, rrefName);
    constructor = VMENV GetMethodID(env, klass, "<init>", "(Ljava/lang/String;)V");

    val = VMENV NewObject(env, klass, constructor, arg);

  CATCH_EXCEPTION;
  
   UNPROTECT(1);
  return(val);
}    

 




