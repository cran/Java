
# line 1121 "RStoJava.cweb"
#if defined(_R_)
#include "RtoJava.h"
#else
#include "StoJava.h"
#endif
#include "RStoJava.h"
#include "JNIUtils.h"
#include "ManagerInterfaceUtils.h"

#include "Converters.h"
#include "ErrorHandling.h"


# line 974 "RStoJava.cweb"
const int FOREIGN_REFERENCE_ID_SLOT = 1;
const int FOREIGN_REFERENCE_CLASSNAME_SLOT = 2;
const int FOREIGN_REFERENCE_CLASS_SLOT = 3;



# line 669 "RStoJava.cweb"
void *
s_to_java_basic(JNIEnv *env, USER_OBJECT_ obj, char *sig, char **actual_sig_ptr)
{
  /* this code is intended only for basic objects; it ignores classes,
     expecting the calling S or C code to have determined that it's
     proper to just convert the basic data in the object */
  jboolean isArray, isBuiltIn; 
  long n; 
  void *value; 
  enum java_types which;
  jclass klass;
  
  jboolean isReference = JNI_FALSE;

  if(sig== (char*)NULL || sig[0] == (char) NULL) {
    sig = getJavaSignature(obj, &isReference);
#if LDEBUG    
printf("Retrieved signature %s\n",sig);  fflush(stdout);
#endif    
    if(!sig) {
      PROBLEM "No default Java type for S class ???"
        ERROR;
      return NULL_JAVA_OBJECT;
    }
  }


#if LDEBUG  
printf("[s_to_java_basic] (sig) %s \n", sig);fflush(stdout);
#endif  

  which = which_java_type(sig, &isArray, &isBuiltIn);

#if LDEBUG  
printf("[s_to_java_basic] (type) %d (reference) %d, built-in type %s,  array %s\n", which, isReference, isBuiltIn ? "yes" : "no",isArray ? "yes" : "no");fflush(stdout);
#endif
  
  n = GET_LENGTH(obj);

  if(!isBuiltIn) {
    klass = ManagerFindClass(env, sig);
    get_Java_exception("ignoring exception", env);
  }


  if(klass != NULL_JAVA_OBJECT && ToJavaConverters) {
    int done = 0;
      value = userLevelToJavaConversion(env, obj, klass, which, sig, n, isArray, &done);
     if(done)
       return(value);
  }

#ifdef LDEBUG  
fprintf(stderr, "No user level conversion\n"); fflush(stderr);
#endif
  
  if(!isArray && n > 1 && isReference == FALSE) {
    PROBLEM "Length %ld > 1 but requested signature (\"%s\") implies a non-array",
                    n, sig
     ERROR;
    n = 1;
  }


  value = (void*)convertToJava(env, obj, isArray, which, n);

  if(actual_sig_ptr)
    *actual_sig_ptr = sig;

  return(value);
}

# line 747 "RStoJava.cweb"
void*
convertToJava(JNIEnv *env, USER_OBJECT_ obj, jboolean isArray, java_type whichType, long n) 
{
 void* value = NULL_JAVA_OBJECT; 
 long i;

#if LDEBUG 
printf("convertToJava %d %s %d\n", whichType, (isArray == JNI_TRUE?"yes" :"no"), (int) n);fflush(stdout);
#endif
 
 switch(whichType) {
  case JAVA_DOUBLE: {
    double *in;
    jdouble *out;
    obj = AS_NUMERIC(obj); 
    in = NUMERIC_POINTER(obj);
    if(sizeof(*in) != sizeof(*out)) {
      out = (jdouble *) Calloc(n, jdouble);
      for(i=0; i<n; i++)
         out[i] = in[i];
    }
    else 
      out = in;
    value = toJavaType(env, whichType, n, (void *)out, isArray);
    break;
  }
  case JAVA_FLOAT: {
    double *in; 
    jfloat *out;
    obj = AS_NUMERIC(obj); 
    in = NUMERIC_POINTER(obj);
      out = (jfloat *) Calloc(n, jfloat);
      for(i=0; i<n; i++)
        out[i] = in[i];
    value = toJavaType(env, whichType, n, (void *)out, isArray);
    free(out);
    break;
  }
  case JAVA_BOOLEAN: {
    int *in; jboolean *out;
    obj = AS_LOGICAL(obj); 
    in = LOGICAL_DATA(obj);
    if(sizeof(*in) != sizeof(*out)) {
      out = (jboolean *) Calloc(n, jfloat);
      for(i=0; i<n; i++)
         out[i] = in[i];
    }
    else 
      out = (jboolean *)in;
    value = toJavaType(env, whichType, n, (void *)out, isArray);
    break;
  }
  case JAVA_STRING: {
    jstring str;
    /*
       In S4, we can do the following. But R currently has the
       character vector not easily accessible as a char ** array.
       So we do things locally here.
       char **out;       
       out = & CHAR_DEREF(CHARACTER_DATA(obj)[0]);
       value = toJavaType(env, whichType, n, (void *)obj, isArray);
     */

     if(n > 1) {
       value = JavaStringArray(env, n);
       for(i=0; i < n; i++) {
/*
  fprintf(stderr, "Converting to a string array of length %d\n", (int)i);        fflush(stderr);
*/
         str = VMENV NewStringUTF(env, CHAR_DEREF(STRING_ELT(obj, i)));
         VMENV SetObjectArrayElement(env, value, i, str);
         VMENV DeleteLocalRef(env, str);
       }
      } else {
         value = VMENV NewStringUTF(env, CHAR_DEREF(STRING_ELT(obj, 0)));
      }
    break;
  }
  case JAVA_INT: {
    int *in; jint *out;
    obj = AS_INTEGER(obj); 
    in = INTEGER_POINTER(obj);
    if(sizeof(*in) != sizeof(*out)) {
      out = (jint *) Calloc(n, jint);
      for(i=0; i<n; i++) out[i] = in[i];
    }
    else 
      out = (jint *)in;
    value = toJavaType(env, whichType, n, (void *)out, isArray);
    break;
  }
  case JAVA_LONG: {
    int *in; jlong *out;
    obj = AS_INTEGER(obj);
    in = INTEGER_POINTER(obj);
    if(sizeof(*in) != sizeof(*out)) {
      out = (jlong *) Calloc(n, jlong);
      for(i=0; i<n; i++) out[i] = in[i];
    }
    else out = (jlong *)in;
    value = toJavaType(env, whichType, n, (void *)out, isArray);
    break;
  }
  case JAVA_SHORT: {
    int *in; jshort *out;
    obj = AS_INTEGER(obj); in = INTEGER_POINTER(obj);
    if(sizeof(*in) != sizeof(*out)) {
      out = (jshort *) Calloc(n, jshort);
      for(i=0; i<n; i++) out[i] = in[i];
    }
    else out = (jshort *)in;
    value = toJavaType(env, whichType, n, (void *)out, isArray);
    break;
  }
  case JAVA_BYTE: {
#if 0
XXXXX
    char *in; jbyte *out;
    obj = AS_RAW(obj); 
    in = RAW_POINTER(obj);
    if(sizeof(*in) != sizeof(*out)) { /* this is not possible */
      out = (jbyte *) Calloc(n, jbyte);
      for(i=0; i<n; i++) out[i] = in[i];
    }
    else out = (jbyte *)in;
    value = toJavaType(env, whichType, n, (void *)out, isArray);
#endif
    break;
  }
  case JAVA_OBJECT: {
    /* this is a little different.  The strategy is to treat this as
       an uninterpreted recursive object, so we avoid using any S
       coercion methods -- these should be applied outside of the
       basic conversion code. */
#if 0        
    USER_OBJECT_ *in; 
    void **out;
    if(GET_MODE(obj) != VECSXP) /* must not use IS_RECURSIVE here */
      obj = AS_VECTOR(obj);
    in = RECURSIVE_DATA(obj);
    out = (void **) Calloc(n, void *);
    for(i=0; i<n; i++)
      out[i] = s_to_java_basic(env, in[i], NULL, NULL);
    value = toJavaType(env, whichType, n, (void *)out, isArray);
#endif

/* Now we assume this is a foreignReference. */
 }
 break;

  case JAVA_NAMED_REFERENCE:
  case JAVA_ANONYMOUS_REFERENCE:
     value = userReferenceToJava(env, obj, whichType);
   break;

  case RS_NAMED_REFERENCE:
  case RS_ANONYMOUS_REFERENCE:
#if LDEBUG    
fprintf(stderr, "Creating Java object from R reference %d\n", whichType);fflush(stderr);
#endif 
     value = NULL_JAVA_OBJECT;
     value = userReferenceToJava(env, obj, whichType);
   break;

  case JAVA_GENERIC_REFERENCE:
{
   int i;
    jstring jid, jclassName, tmp;
    jobjectArray jtypes = NULL;

      jid = VMENV NewStringUTF(env, CHAR_DEREF(STRING_ELT(VECTOR_ELT(obj, FOREIGN_REFERENCE_ID_SLOT), 0)));
      jclassName = VMENV NewStringUTF(env, CHAR_DEREF(STRING_ELT(VECTOR_ELT(obj, FOREIGN_REFERENCE_CLASSNAME_SLOT), 0)));   

      n = GET_LENGTH(VECTOR_ELT(obj, FOREIGN_REFERENCE_CLASS_SLOT));
      jtypes = JavaStringArray(env, n);
      for(i=0;i<n;i++) {
        tmp = VMENV NewStringUTF(env, CHAR_DEREF(STRING_ELT(VECTOR_ELT(obj, FOREIGN_REFERENCE_CLASS_SLOT), i)));
        VMENV SetObjectArrayElement(env, jtypes, i, tmp);
        VMENV DeleteLocalRef(env, tmp);

        tmp = VMENV GetObjectArrayElement(env, jtypes, i);
{
    const char *ptr;
    jboolean isCopy;
        ptr = VMENV GetStringUTFChars(env, tmp, &isCopy);
#if LDEBUG
printf("+%d) %s\n",i,ptr );
#endif
        if(isCopy==JNI_TRUE)
          VMENV ReleaseStringUTFChars(env, tmp, ptr);

        VMENV DeleteLocalRef(env, tmp);    
}             
       }
      value = RS(createForeignReference)(env, jid, jclassName, jtypes);
}   
    break;    

  default:
   { char *sig = "";
    PROBLEM "\"%s\" is not a basic type with a known conversion from S to Java",
      sig ERROR;
    value = NULL_JAVA_OBJECT;
   }
  }

 return(value);
}

USER_OBJECT_
foo(USER_OBJECT_ obj)
{
   int i,n;
printf("Creating foreign reference %s\n",
           CHAR_DEREF(STRING_ELT(VECTOR_ELT(obj, FOREIGN_REFERENCE_ID_SLOT), 0)));fflush(stdout);

  n = GET_LENGTH(VECTOR_ELT(obj, FOREIGN_REFERENCE_CLASS_SLOT));
 for(i=0;i < n; i++) {
   printf("%d) %s\n", i, CHAR_DEREF(STRING_ELT(VECTOR_ELT(obj, FOREIGN_REFERENCE_CLASS_SLOT), i)));     fflush(stdout);   
 }
  
 return(obj);
}  


# line 1002 "RStoJava.cweb"
char *
getJavaSignature(USER_OBJECT_ obj, jboolean *isReference)
{
 char *sig = NULL;

 int n = GET_LENGTH(obj);
 
    /* pick the signature suitable to the mode of the data */
    switch(TYPEOF(obj)) {
    case REALSXP:  /* No single in R. */
         sig = (n > 1 ? "[D" : "D"); 
         break;
    case LGLSXP: 
         sig = n > 1 ? "[Z" : "Z"; 
         break; /* Java boolean[] */
    case INTSXP: /* pick the corresponding Java integer size, usually int
                 (32 bits) on a 32-bit machine */
      if(sizeof(jint) == sizeof(long))
        sig = n > 1 ? "[I" : "I";
      else if(sizeof(jlong) == sizeof(long))
        sig = n > 1 ? "[J" : "J";
      else if(sizeof(jshort) == sizeof(long)) /* What ?? */
        sig = n > 1 ? "[S" : "S";
      break;
    case STRSXP: 
        sig = n > 1 ? "[Ljava.lang.String;" : "Ljava.lang.String;"; 
        break;
    case VECSXP: 
        /* Check whether this can be considered 
           as a reference in any way (incuding a list of them).
         */
      if(( sig = javaReferenceSignature(obj, TRUE)) == NULL)  {
        /* treat recursive objects as arrays of Object; naturally this
           isn't likely to work well other than for simple lists, unless
           the Java side somehow knows how to interpret the particular
           class of objects. */
      sig = n > 1 ? "[Ljava.lang.Object;" : "Ljava.lang.Object;";
     } else
        *isReference = TRUE;

      break;
    case NILSXP: /* return a java null */
      sig = NULL;
    default:
          printf("Problems: unrecognized user-level object of mode %d\n", TYPEOF(obj)); fflush(stdout);
    }

 return(sig);
}



# line 643 "RStoJava.cweb"
USER_OBJECT_
s_create_default_vm()
{
char *NULL_STRING = (char*)NULL;
 const char *msg;
 USER_OBJECT_ ans = NEW_LOGICAL(1);

  msg = interface_start_VM(NULL_STRING, NULL_STRING, NULL_STRING,
                           NULL_STRING, (char **)NULL, 0);
  if(msg)
    PROBLEM "Couldn't start default Java VM: %s", msg
      ERROR;

 LOGICAL_POINTER(ans)[0] = 1;

 return(ans);
}




# line 105 "RStoJava.cweb"
/* The order is important to allow the CHAR in the 
   conversion switch statement be well-defined.
 */
# line 135 "RStoJava.cweb"
#if !defined(_R_)
#if defined(CHAR)
#undef CHAR
#endif

#define CHAR(x) (x) 
#define NULL_USER_OBJECT S_void
#endif




# line 207 "RStoJava.cweb"
USER_OBJECT_
RS_genericJavaCall(USER_OBJECT_ qualifier, USER_OBJECT_ methodName,
                   USER_OBJECT_ userArgs,
                    USER_OBJECT_ userArgNames, USER_OBJECT_ returnValueName,
                     USER_OBJECT_ signatures, USER_OBJECT_ convert)
{
 JNIEnv *env = getJNIEnv();

  jobject jqualifier;
  jobjectArray args;
  jobjectArray argNames;
  jstring JreturnValueName = NULL_JAVA_OBJECT;
  jstring jmethodName = NULL_JAVA_OBJECT;
  jobject val;
  USER_OBJECT_ user_ans;
  char *returnName;
  JavaVM* vm;
 
  if(env == NULL) {
    PROBLEM "There is no Java virtual machine available. Start it with a call to the R function .JavaInit()"
    ERROR;
  }
 
  vm = getJavaVM();
  (*vm)->AttachCurrentThread(vm, (void**) &env, NULL);
 

 
#ifdef LDEBUG 
{
    extern int  R_Visible;
    fprintf(stderr, "Visible flag %d\n", R_Visible);fflush(stderr);
}
#endif
 
#if LDEBUG
printf("Qualifier\n");
#endif 
   if(GET_LENGTH(qualifier) > 0)
     jqualifier = s_to_java_basic(env, qualifier, NULL, NULL);
   else
     jqualifier = NULL_JAVA_OBJECT;

#if LDEBUG
 printf("Arguments\n");
#endif 
   args = argsToArray(env, userArgs, signatures);

   argNames = argNamesToArray(env, userArgNames, FALSE);
 

   returnName  = (GET_LENGTH(returnValueName) > 0) ?
                             CHAR_DEREF(STRING_ELT(returnValueName,0)) : "";

  JreturnValueName = VMENV NewStringUTF(env, returnName);
  jmethodName = VMENV NewStringUTF(env, CHAR_DEREF(STRING_ELT(methodName, 0)));

  val = callGenericOmegaMethod(env, jqualifier, jmethodName,
                                 args, argNames, JreturnValueName, LOGICAL_DATA(convert)[0]);


 
  CATCH_EXCEPTION;

#ifdef LDEBUG
if(val != NULL_JAVA_OBJECT) {
     jboolean isCopy;
  fprintf(stderr, "Return value class name %s\n", (char*)getClassName(env, VMENV GetObjectClass(env,val), &isCopy));fflush(stderr);
}
#endif

  user_ans =  convertAnswer(env, returnValueName, val);
  CATCH_EXCEPTION;

#ifdef LDEBUG
{
    extern int  R_Visible;
    fprintf(stderr, "Visible flag %d\n", R_Visible);fflush(stderr);
}
#endif

 return(user_ans);
}



# line 445 "RStoJava.cweb"
USER_OBJECT_
convertAnswer(JNIEnv *env, USER_OBJECT_ returnValueName, jobject val) 
{
 USER_OBJECT_ user_ans = NULL_USER_OBJECT;
  if(val == NULL_JAVA_OBJECT)
      return(user_ans);

#ifdef LDEBUG  
fprintf(stderr,"convertAnswer %d\n", (GET_LENGTH(returnValueName) == 0));fflush(stderr);
#endif       
 
  if(GET_LENGTH(returnValueName) == 0) {
         /* convert the returned value. */

    if(val != NULL && val != NULL_JAVA_OBJECT) {
      const char *dataType;
      jboolean isCopy;
      jstring className;
#ifdef LDEBUG  
fprintf(stderr,"No return name. Getting class name\n");fflush(stderr);
#endif       
        className = get_object_class_name(env, val);

        dataType = VMENV GetStringUTFChars(env, className, &isCopy);
        user_ans = s_from_java_basic(env, val, dataType, TRUE, TRUE);

        if(isCopy == JNI_TRUE) {
          VMENV ReleaseStringUTFChars(env, className, dataType);
        }
    } 
  } else {
     user_ans = s_from_java_basic(env, val, NULL, TRUE, TRUE);
  }

 return(user_ans);
}



# line 493 "RStoJava.cweb"
jobjectArray
argsToArray(JNIEnv *env, USER_OBJECT_ userArgs, USER_OBJECT_ signatures)
{
  char *actual_sig;
  long numSignatures = GET_LENGTH(signatures);
  long numArgs = GET_LENGTH(userArgs);
  int i;
  jobjectArray argArray;

   /* First, we traverse the list of arguments
      and convert them to Java objects.
      We gather these into an Object[] array.
    */ 
  argArray = JavaObjectArray(env, numArgs);
  for(i = 0; i < numArgs; i++) {
     jobject tmp;
      tmp = (jobject)s_to_java_basic(env, VECTOR_ELT(userArgs, i), 
                                     i < numSignatures ? CHAR(STRING_ELT(signatures, i)) : NULL,
                                             &actual_sig);
    /* now put this into an Object array. */
   VMENV SetObjectArrayElement(env, argArray, i, tmp);
  }

 return(argArray);
}

# line 531 "RStoJava.cweb"
jobjectArray
argNamesToArray(JNIEnv *env, USER_OBJECT_ userArgNames, int addExtraValue)
{
 jobjectArray argNames = NULL_JAVA_OBJECT;
 long numNames = GET_LENGTH(userArgNames);


  if(numNames > 0) {

#if 1
/* For the moment, we just write the code here rather than using
   the toJavaType(). This latter routine is now fixed and in the case
   of a JAVA_STRING array expects the pointer to the actual character() vector.
 */
 jobjectArray array;
 jclass klass = VMENV FindClass(env, "java/lang/String");
 int i;
 jstring str;
     array = VMENV NewObjectArray(env, numNames, klass, NULL);
     for(i = 0; i < numNames; i++) {
       str = VMENV NewStringUTF(env, CHAR_DEREF(STRING_ELT(userArgNames, i)));
       VMENV SetObjectArrayElement(env, array, i, str);
       VMENV DeleteLocalRef(env, str);
     }

   argNames = array;
#else
        /* Now works for R by passing in the top-level character object */
    argNames = 
      toJavaType(env, JAVA_STRING, numNames, 
                  (void*)userArgNames, (jboolean) JNI_TRUE);
#endif
  }

 return(argNames);
}


# line 575 "RStoJava.cweb"
jobjectArray
JavaObjectArray(JNIEnv *env, int length)
{
  /* Should be a pthread_once */
 static jclass clazz = NULL;
 jobjectArray arr;

 if(clazz == NULL) {
    clazz = VMENV FindClass(env, "java/lang/Object");
    clazz = VMENV NewGlobalRef(env, clazz);
 }


   arr = VMENV NewObjectArray(env, (jint) length, clazz, NULL);

return(arr);
}

# line 608 "RStoJava.cweb"
jobjectArray
JavaStringArray(JNIEnv *env, int length)
{
  /* Should be a pthread_once */
 static jclass clazz = NULL;
 jobjectArray arr;
 
 if(clazz == NULL) {
    clazz = VMENV FindClass(env, "java/lang/String");
    clazz = VMENV NewGlobalRef(env, clazz);
 }

 arr = VMENV NewObjectArray(env, (jint) length, clazz, NULL);

return(arr);
}


# line 325 "RStoJava.cweb"
USER_OBJECT_
RS_ConstructorJavaCall(USER_OBJECT_ className, USER_OBJECT_ userArgs,
                         USER_OBJECT_ userArgNames, USER_OBJECT_ returnValueName,
                           USER_OBJECT_ signatures, USER_OBJECT_ convert)
{

 JNIEnv *env = getJNIEnv();
 
  jstring JreturnValueName = NULL_JAVA_OBJECT;
  jobject val;
  USER_OBJECT_ user_ans;
  jstring JclassName;

  jobjectArray args;
  jobjectArray argNames;

  if(env == NULL) {
     PROBLEM "The JVM is not currently running. Use .JavaInit()" 
     ERROR;
  }  
    args = argsToArray(env, userArgs, signatures);
    argNames = argNamesToArray(env, userArgNames, FALSE);
    JclassName = VMENV NewStringUTF(env, CHAR_DEREF(STRING_ELT(className, 0)));


  if(GET_LENGTH(returnValueName) > 0) {
    JreturnValueName = VMENV NewStringUTF(env, CHAR_DEREF(STRING_ELT(returnValueName, 0)));
  } else 
    JreturnValueName = VMENV NewStringUTF(env, "");

  val = callConstructorMethod(env, JclassName, args, argNames,  JreturnValueName, (jboolean)LOGICAL_DATA(convert)[0]);


  CATCH_EXCEPTION;

  user_ans =  convertAnswer(env, returnValueName, val);

  CATCH_EXCEPTION;

 return(user_ans);
}


# line 381 "RStoJava.cweb"
USER_OBJECT_
RS_OmegahatExpression(USER_OBJECT_ expressionString, 
                       USER_OBJECT_ userArgs, USER_OBJECT_ userArgNames,
                         USER_OBJECT_ returnValueName,
                          USER_OBJECT_ signatures,
                           USER_OBJECT_ convert)
{
 JNIEnv *env = getJNIEnv();

 jstring JreturnValueName = NULL_JAVA_OBJECT;
 jstring expression;
 jobject val;
 USER_OBJECT_ user_ans = NULL_USER_OBJECT;

 jobjectArray args = argsToArray(env, userArgs, signatures);
 jobjectArray argNames = argNamesToArray(env, userArgNames, FALSE);

 if(GET_LENGTH(returnValueName) > 0) {
    JreturnValueName = VMENV NewStringUTF(env, CHAR_DEREF(STRING_ELT(returnValueName, 0)));
 }


   expression = VMENV NewStringUTF(env,  CHAR_DEREF(STRING_ELT(expressionString, 0)));
   val = evalOmegaExpression(env, expression, args, argNames, JreturnValueName, (jboolean) LOGICAL_DATA(convert)[0]);

  user_ans =  convertAnswer(env, returnValueName, val); 

return(user_ans);
}



# line 632 "RStoJava.cweb"
JNIEnv *SgetJNIEnv()
{
 if(!exists_Java_vm())  
   s_create_default_vm();

 return(getJNIEnv());
}



# line 1059 "RStoJava.cweb"
USER_OBJECT_
javaToUserString(JNIEnv *env, jobject val)
{
 const char *inString;
 USER_OBJECT_ ans;
 jboolean isCopy;
   inString = VMENV GetStringUTFChars(env, (jstring) val, &isCopy);
   PROTECT(ans = NEW_CHARACTER(1));
   SET_STRING_ELT(ans, 0, COPY_TO_USER_STRING((char *)inString));
   UNPROTECT(1); 

   if(isCopy)
     VMENV ReleaseStringUTFChars(env, (jstring) val, inString);


return(ans);
}


# line 1088 "RStoJava.cweb"
jclass
ManagerFindClass(JNIEnv *env, char *className)
{
  jclass klass;  
  jstring jstr;
  jmethodID findClassID;
  jobject mgr = getInterfaceManager();
    jstr = VMENV NewStringUTF(env, className);
    klass = VMENV GetObjectClass(env, mgr);
  findClassID = VMENV GetMethodID(env, klass, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");
  klass =  VMENV CallObjectMethod(env, mgr, findClassID, jstr);

 return(klass);
}    



# line 1110 "RStoJava.cweb"
void
RS_JAVA(isJVMInitialized)(long *wellThen)
{
 JNIEnv *env = getJNIEnv();
  *wellThen = (env != NULL);
}    




# line 1135 "RStoJava.cweb"
/*
        Copyright (c) 1998, 1999 The Omega Project for Statistical Computing.
         All rights reserved.

*/
/*
                             GNU GENERAL PUBLIC LICENSE

                                Version 2, June 1991

        Copyright (C) 1989, 1991 Free Software Foundation, Inc. 675 Mass
        Ave, Cambridge, MA 02139, USA. Everyone is permitted to copy and
        distribute verbatim copies of this license document, but changing it
        is not allowed.

                                      Preamble

        The licenses for most software are designed to take away your
        freedom to share and change it. By contrast, the GNU General Public
        License is intended to guarantee your freedom to share and change
        free software--to make sure the software is free for all its users.
        This General Public License applies to most of the Free Software
        Foundation's software and to any other program whose authors commit
        to using it. (Some other Free Software Foundation software is
        covered by the GNU Library General Public License instead.) You can
        apply it to your programs, too.

        When we speak of free software, we are referring to freedom, not
        price. Our General Public Licenses are designed to make sure that
        you have the freedom to distribute copies of free software (and
        charge for this service if you wish), that you receive source code
        or can get it if you want it, that you can change the software or
        use pieces of it in new free programs; and that you know you can do
        these things.

        To protect your rights, we need to make restrictions that forbid
        anyone to deny you these rights or to ask you to surrender the
        rights. These restrictions translate to certain responsibilities for
        you if you distribute copies of the software, or if you modify it.

        For example, if you distribute copies of such a program, whether
        gratis or for a fee, you must give the recipients all the rights
        that you have. You must make sure that they, too, receive or can get
        the source code. And you must show them these terms so they know
        their rights.

        We protect your rights with two steps: (1) copyright the software,
        and (2) offer you this license which gives you legal permission to
        copy, distribute and/or modify the software.

        Also, for each author's protection and ours, we want to make certain
        that everyone understands that there is no warranty for this free
        software. If the software is modified by someone else and passed on,
        we want its recipients to know that what they have is not the
        original, so that any problems introduced by others will not reflect
        on the original authors' reputations.

        Finally, any free program is threatened constantly by software
        patents. We wish to avoid the danger that redistributors of a free
        program will individually obtain patent licenses, in effect making
        the program proprietary. To prevent this, we have made it clear that
        any patent must be licensed for everyone's free use or not licensed
        at all.

        The precise terms and conditions for copying, distribution and
        modification follow.

                             GNU GENERAL PUBLIC LICENSE
          TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

        0. This License applies to any program or other work which contains
        a notice placed by the copyright holder saying it may be distributed
        under the terms of this General Public License. The "Program",
        below, refers to any such program or work, and a "work based on the
        Program" means either the Program or any derivative work under
        copyright law: that is to say, a work containing the Program or a
        portion of it, either verbatim or with modifications and/or
        translated into another language. (Hereinafter, translation is
        included without limitation in the term "modification".) Each
        licensee is addressed as "you".

        Activities other than copying, distribution and modification are not
        covered by this License; they are outside its scope. The act of
        running the Program is not restricted, and the output from the
        Program is covered only if its contents constitute a work based on
        the Program (independent of having been made by running the
        Program). Whether that is true depends on what the Program does.

        1. You may copy and distribute verbatim copies of the Program's
        source code as you receive it, in any medium, provided that you
        conspicuously and appropriately publish on each copy an appropriate
        copyright notice and disclaimer of warranty; keep intact all the
        notices that refer to this License and to the absence of any
        warranty; and give any other recipients of the Program a copy of
        this License along with the Program.

        You may charge a fee for the physical act of transferring a copy,
        and you may at your option offer warranty protection in exchange for
        a fee.

        2. You may modify your copy or copies of the Program or any portion
        of it, thus forming a work based on the Program, and copy and
        distribute such modifications or work under the terms of Section 1
        above, provided that you also meet all of these conditions:

        a) You must cause the modified files to carry prominent notices
        stating that you changed the files and the date of any change.

        b) You must cause any work that you distribute or publish, that in
        whole or in part contains or is derived from the Program or any part
        thereof, to be licensed as a whole at no charge to all third parties
        under the terms of this License.

        c) If the modified program normally reads commands interactively
        when run, you must cause it, when started running for such
        interactive use in the most ordinary way, to print or display an
        announcement including an appropriate copyright notice and a notice
        that there is no warranty (or else, saying that you provide a
        warranty) and that users may redistribute the program under these
        conditions, and telling the user how to view a copy of this License.
        (Exception: if the Program itself is interactive but does not
        normally print such an announcement, your work based on the Program
        is not required to print an announcement.)

        These requirements apply to the modified work as a whole. If
        identifiable sections of that work are not derived from the Program,
        and can be reasonably considered independent and separate works in
        themselves, then this License, and its terms, do not apply to those
        sections when you distribute them as separate works. But when you
        distribute the same sections as part of a whole which is a work
        based on the Program, the distribution of the whole must be on the
        terms of this License, whose permissions for other licensees extend
        to the entire whole, and thus to each and every part regardless of
        who wrote it.

        Thus, it is not the intent of this section to claim rights or
        contest your rights to work written entirely by you; rather, the
        intent is to exercise the right to control the distribution of
        derivative or collective works based on the Program.

        In addition, mere aggregation of another work not based on the
        Program with the Program (or with a work based on the Program) on a
        volume of a storage or distribution medium does not bring the other
        work under the scope of this License.

        3. You may copy and distribute the Program (or a work based on it,
        under Section 2) in object code or executable form under the terms
        of Sections 1 and 2 above provided that you also do one of the
        following:

        a) Accompany it with the complete corresponding machine-readable
        source code, which must be distributed under the terms of Sections 1
        and 2 above on a medium customarily used for software interchange;
        or,

        b) Accompany it with a written offer, valid for at least three
        years, to give any third party, for a charge no more than your cost
        of physically performing source distribution, a complete
        machine-readable copy of the corresponding source code, to be
        distributed under the terms of Sections 1 and 2 above on a medium
        customarily used for software interchange; or,

        c) Accompany it with the information you received as to the offer to
        distribute corresponding source code. (This alternative is allowed
        only for noncommercial distribution and only if you received the
        program in object code or executable form with such an offer, in
        accord with Subsection b above.)

        The source code for a work means the preferred form of the work for
        making modifications to it. For an executable work, complete source
        code means all the source code for all modules it contains, plus any
        associated interface definition files, plus the scripts used to
        control compilation and installation of the executable. However, as
        a special exception, the source code distributed need not include
        anything that is normally distributed (in either source or binary
        form) with the major components (compiler, kernel, and so on) of the
        operating system on which the executable runs, unless that component
        itself accompanies the executable.

        If distribution of executable or object code is made by offering
        access to copy from a designated place, then offering equivalent
        access to copy the source code from the same place counts as
        distribution of the source code, even though third parties are not
        compelled to copy the source along with the object code.

        4. You may not copy, modify, sublicense, or distribute the Program
        except as expressly provided under this License. Any attempt
        otherwise to copy, modify, sublicense or distribute the Program is
        void, and will automatically terminate your rights under this
        License. However, parties who have received copies, or rights, from
        you under this License will not have their licenses terminated so
        long as such parties remain in full compliance.

        5. You are not required to accept this License, since you have not
        signed it. However, nothing else grants you permission to modify or
        distribute the Program or its derivative works. These actions are
        prohibited by law if you do not accept this License. Therefore, by
        modifying or distributing the Program (or any work based on the
        Program), you indicate your acceptance of this License to do so, and
        all its terms and conditions for copying, distributing or modifying
        the Program or works based on it.

        6. Each time you redistribute the Program (or any work based on the
        Program), the recipient automatically receives a license from the
        original licensor to copy, distribute or modify the Program subject
        to these terms and conditions. You may not impose any further
        restrictions on the recipients' exercise of the rights granted
        herein. You are not responsible for enforcing compliance by third
        parties to this License.

        7. If, as a consequence of a court judgment or allegation of patent
        infringement or for any other reason (not limited to patent issues),
        conditions are imposed on you (whether by court order, agreement or
        otherwise) that contradict the conditions of this License, they do
        not excuse you from the conditions of this License. If you cannot
        distribute so as to satisfy simultaneously your obligations under
        this License and any other pertinent obligations, then as a
        consequence you may not distribute the Program at all. For example,
        if a patent license would not permit royalty-free redistribution of
        the Program by all those who receive copies directly or indirectly
        through you, then the only way you could satisfy both it and this
        License would be to refrain entirely from distribution of the
        Program.

        If any portion of this section is held invalid or unenforceable
        under any particular circumstance, the balance of the section is
        intended to apply and the section as a whole is intended to apply in
        other circumstances.

        It is not the purpose of this section to induce you to infringe any
        patents or other property right claims or to contest validity of any
        such claims; this section has the sole purpose of protecting the
        integrity of the free software distribution system, which is
        implemented by public license practices. Many people have made
        generous contributions to the wide range of software distributed
        through that system in reliance on consistent application of that
        system; it is up to the author/donor to decide if he or she is
        willing to distribute software through any other system and a
        licensee cannot impose that choice.

        This section is intended to make thoroughly clear what is believed
        to be a consequence of the rest of this License.

        8. If the distribution and/or use of the Program is restricted in
        certain countries either by patents or by copyrighted interfaces,
        the original copyright holder who places the Program under this
        License may add an explicit geographical distribution limitation
        excluding those countries, so that distribution is permitted only in
        or among countries not thus excluded. In such case, this License
        incorporates the limitation as if written in the body of this
        License.

        9. The Free Software Foundation may publish revised and/or new
        versions of the General Public License from time to time. Such new
        versions will be similar in spirit to the present version, but may
        differ in detail to address new problems or concerns.

        Each version is given a distinguishing version number. If the
        Program specifies a version number of this License which applies to
        it and "any later version", you have the option of following the
        terms and conditions either of that version or of any later version
        published by the Free Software Foundation. If the Program does not
        specify a version number of this License, you may choose any version
        ever published by the Free Software Foundation.

        10. If you wish to incorporate parts of the Program into other free
        programs whose distribution conditions are different, write to the
        author to ask for permission. For software which is copyrighted by
        the Free Software Foundation, write to the Free Software Foundation;
        we sometimes make exceptions for this. Our decision will be guided
        by the two goals of preserving the free status of all derivatives of
        our free software and of promoting the sharing and reuse of software
        generally.

                                    NO WARRANTY

        11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO
        WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.
        EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR
        OTHER PARTIES PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY
        KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,
        THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
        PARTICULAR PURPOSE. THE ENTIRE RISK AS TO THE QUALITY AND
        PERFORMANCE OF THE PROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE
        DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR
        CORRECTION.

        12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN
        WRITING WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY
        AND/OR REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU
        FOR DAMAGES, INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR
        CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR INABILITY TO USE THE
        PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF DATA OR DATA BEING
        RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD PARTIES OR A
        FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS), EVEN IF
        SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF
        SUCH DAMAGES.

                            END OF TERMS AND CONDITIONS
*/

