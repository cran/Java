# line 225 "ConverterExamples.cweb"
#include "Converters.h"
#include "Rdefines.h"
#include "RtoJava.h"
#include "JNIUtils.h"
#include "CtoJava.h" /* MUST_GET_METHOD_ID  */

#include "RReferenceCall.h"
# line 110 "ConverterExamples.cweb"
typedef enum {ConverterFunction, MatchFunction} ConverterOpType;

# line 114 "ConverterExamples.cweb"
USER_OBJECT_
RS_JAVA(callRConverterFunction)(USER_OBJECT_ fun, jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter, ConverterOpType opType);

# line 194 "ConverterExamples.cweb"
USER_OBJECT_
RS_JAVA(RfunctionConverter)(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter);

# line 54 "ConverterExamples.cweb"
USER_OBJECT_
RS_JAVA(RealVariableConverter)(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter)
{
 jmethodID ValuesID;
 jobject array;
 USER_OBJECT_ ans;
   ValuesID = VMENV GetMethodID(env, type, "getValues", "()[D");

   array = VMENV CallObjectMethod(env, obj, ValuesID);
   ans = s_from_java_basic(env, array, "[D", TRUE, FALSE);

  return(ans);
}



# line 85 "ConverterExamples.cweb"
jboolean
RS_JAVA(functionConverterMatch)(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter)
{
  USER_OBJECT_ ans;
  USER_OBJECT_ fun;
  USER_OBJECT_ functions;
  int ok;
  
  
    /* Get the matching function from the user data. */
    /* Have to handle a closure that does both conversion and matching! */  
  functions = (USER_OBJECT_)  (converter->userData);
  fun = LIST_POINTER(functions)[MatchFunction];

  ans = RS_JAVA(callRConverterFunction)(fun, obj, type, env, converter, MatchFunction);
  ok = LOGICAL_DATA(ans)[0];
  
 return(ok ? JNI_TRUE : JNI_FALSE);
}

# line 170 "ConverterExamples.cweb"
USER_OBJECT_
RS_JAVA(setFunctionConverter)(USER_OBJECT_ funs, USER_OBJECT_ description, USER_OBJECT_ fromJava)
{
 int index;
 USER_OBJECT_ ans;
 char *desc = NULL;
 R_PreserveObject(funs);

 if(GET_LENGTH(description))
   desc = CHAR_DEREF(CHARACTER_DATA(description)[0]);
 if(desc && desc[0])
    desc = strdup(desc);

 addFromJavaConverterInfo(RS_JAVA(functionConverterMatch),
                          RS_JAVA(RfunctionConverter), 1,
                           funs, desc, &index);
  PROTECT(ans = NEW_INTEGER(1));
   INTEGER_DATA(ans)[0] = index;
  UNPROTECT(1);
  return(ans);
}    

# line 209 "ConverterExamples.cweb"
USER_OBJECT_
RS_JAVA(RfunctionConverter)(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter)
{
  USER_OBJECT_ fun;
  USER_OBJECT_ functions;
  USER_OBJECT_ ans;
  
  functions = (USER_OBJECT_)  (converter->userData);
  fun = LIST_POINTER(functions)[ConverterFunction];
  ans = RS_JAVA(callRConverterFunction)(fun, obj, type, env, converter, ConverterFunction);

  return(ans);
}

# line 128 "ConverterExamples.cweb"
USER_OBJECT_
RS_JAVA(callRConverterFunction)(USER_OBJECT_ fun, jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter, ConverterOpType opType)
{
  USER_OBJECT_ args;
  USER_OBJECT_ rclassName;
  USER_OBJECT_ call;
  USER_OBJECT_ ans;

  jboolean isCopy;
  jstring tmp;
  const char *klassName;
    /* Now create the R arguments to the matching function. */  
  PROTECT(args = NEW_LIST(2));
  VECTOR(args)[0] = anonymousAssign(env, obj, type);
  PROTECT(rclassName = NEW_CHARACTER(1));
    
     tmp = get_object_class_name(env, obj);
     klassName = VMENV GetStringUTFChars(env, tmp, &isCopy);
     CHARACTER_DATA(rclassName)[0] = COPY_TO_USER_STRING(klassName);    
     if(isCopy) {
       VMENV ReleaseStringUTFChars(env, tmp, klassName);
     }
  
    VECTOR(args)[1] = rclassName;
  
   PROTECT(call = RJava(createCall)(fun, args));
    ans = eval(call, R_GlobalEnv);

  UNPROTECT(3);

  return(ans);
}    


