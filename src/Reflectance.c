# line 267 "Reflectance.cweb"
#include "RtoJava.h"
#include "Reflectance.h"

#include "JNIUtils.h"      /* The class name method. */
#include "ErrorHandling.h" /* for get_java_exception in MUST_GET_METHOD_ID() */
# line 262 "Reflectance.cweb"
static ReflectanceMethodIDs MethodIDs, ConstructorIDs;
static jmethodID ModifierStringID = NULL;

# line 212 "Reflectance.cweb"
const char *
getClassName(JNIEnv *env, jclass klass, jboolean *isCopy)
{
 const char *tmp;
 jstring str = get_java_class_name(env, klass);
 tmp = VMENV GetStringUTFChars(env, str, isCopy);   
 return(tmp);
}

# line 78 "Reflectance.cweb"
USER_OBJECT_ 
RS_JAVA(MethodConverter)(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter)
{
 int i = 0, k, n;
 int numSlots;
 USER_OBJECT_ ans, names;
 const char *tmp;

 jboolean isCopy;
 jstring jval;
 jclass klass;
 jobject jsig, jobj;
 ReflectanceMethodIDs *mids;
 boolean isMethod;
 jint modifier;

 if(ModifierStringID == NULL)
  initReflectanceMethods(env);

  /* Determine whether we have a constructor or method
     and set the method identifiers and number of slots
     appropriately.
   */
 isMethod = VMENV IsSameObject(env, type, VMENV FindClass(env, "java/lang/reflect/Method")) == JNI_TRUE;

 if(isMethod) {
   mids = &MethodIDs;
   numSlots = 6;
 } else {
   mids = &ConstructorIDs;
   numSlots = 5; /* Drop out the */
 }
 
 
 PROTECT(ans = NEW_LIST(numSlots));
 PROTECT(names = NEW_CHARACTER(numSlots));
 
 SET_VECTOR_ELT(ans, i, NEW_CHARACTER(1));
 jval = VMENV CallObjectMethod(env, obj, mids->getName);
 tmp = VMENV GetStringUTFChars(env, jval, &isCopy);  
 SET_STRING_ELT(VECTOR_ELT(ans, i), 0, COPY_TO_USER_STRING(tmp));
 if(isCopy)
   VMENV ReleaseStringUTFChars(env, jval, tmp);
 SET_STRING_ELT(names, i, COPY_TO_USER_STRING("name"));
 i++;

 SET_VECTOR_ELT(ans, i, NEW_CHARACTER(1));
 klass = VMENV CallObjectMethod(env, obj, mids->getClass);
 tmp = getClassName(env, klass, &isCopy);
 SET_STRING_ELT(VECTOR_ELT(ans, i), 0, COPY_TO_USER_STRING(tmp));
 SET_STRING_ELT(names, i, COPY_TO_USER_STRING("Declaring class")); 
 i++;


 jsig = VMENV CallObjectMethod(env, obj, mids->getParameters);
 n = VMENV GetArrayLength(env, jsig);
 SET_VECTOR_ELT(ans, i, NEW_CHARACTER(n));
 for(k = 0; k < n ; k++) {
   jobj = VMENV GetObjectArrayElement(env, jsig, k);
   tmp = getClassName(env, jobj, &isCopy);
   SET_STRING_ELT(VECTOR_ELT(ans, i), k, COPY_TO_USER_STRING(tmp));
 }
 SET_STRING_ELT(names, i, COPY_TO_USER_STRING("Parameters"));    
 i++;

 
 SET_VECTOR_ELT(ans, i, NEW_INTEGER(1));
 modifier = VMENV CallIntMethod(env, obj, mids->getModifiers);
 INTEGER_DATA(VECTOR_ELT(ans, i))[0] = modifier;
 SET_STRING_ELT(names, i, COPY_TO_USER_STRING("Modifiers"));
 {
      /* Now get the string that represents the modifier value.
         Do this by calling the static method toString(int)
         in the java.lang.reflect.Modifier class.
         We assume we have initialized the ModifierStringID
         method id earlier when getting all the method ids
         for the reflectance classes.
       */
   USER_OBJECT_ tmpr;
   const char *modName;
   jstring jmodName;
    PROTECT(tmpr = NEW_CHARACTER(1));
    jmodName = VMENV CallStaticObjectMethod(env, (jclass)VMENV FindClass(env, "java/lang/reflect/Modifier"), ModifierStringID, modifier);

   if(jmodName != NULL_JAVA_OBJECT) {
      modName = VMENV GetStringUTFChars(env, jmodName, &isCopy);   
      SET_STRING_ELT(tmpr, 0, COPY_TO_USER_STRING(modName));
    }
    SET_NAMES(VECTOR_ELT(ans, i), tmpr);
    UNPROTECT(1);
 }
 i++;


 jsig = VMENV CallObjectMethod(env, obj, mids->getExceptions);
 n = VMENV GetArrayLength(env, jsig);
 SET_VECTOR_ELT(ans, i, NEW_CHARACTER(n));
 for(k = 0; k < n ; k++) {
   jobj = VMENV GetObjectArrayElement(env, jsig, k);
   tmp = getClassName(env, jobj, &isCopy);
   SET_STRING_ELT(VECTOR_ELT(ans, i), k, COPY_TO_USER_STRING(tmp));
 }
 SET_STRING_ELT(names, i, COPY_TO_USER_STRING("Exceptions"));    
 i++;

 

 if(isMethod) {
   SET_VECTOR_ELT(ans, i, NEW_CHARACTER(1));
   klass = VMENV CallObjectMethod(env, obj, mids->getReturnType);
   tmp = getClassName(env, klass, &isCopy);
   SET_VECTOR_ELT(VECTOR_ELT(ans, i), 0, COPY_TO_USER_STRING(tmp));
   SET_STRING_ELT(names, i, COPY_TO_USER_STRING("Return type"));    
   i++;
 }
 
 SET_NAMES(ans, names); 
 
 /* Now set the class to be "JavaMethod" */

 UNPROTECT(2);

 return(ans);
}

# line 228 "Reflectance.cweb"
char *
initReflectanceMethods(JNIEnv *env)
{
 jclass MethodClass;
  MethodClass = VMENV FindClass(env, "java/lang/reflect/Method");

  MUST_GET_METHOD_ID(MethodIDs.getModifiers,  MethodClass, NULL, "getModifiers","()I", env);
  MUST_GET_METHOD_ID(MethodIDs.getReturnType, MethodClass, NULL, "getReturnType","()Ljava/lang/Class;", env);
  MUST_GET_METHOD_ID(MethodIDs.getParameters, MethodClass, NULL, "getParameterTypes","()[Ljava/lang/Class;", env);
  MUST_GET_METHOD_ID(MethodIDs.getClass,      MethodClass, NULL, "getDeclaringClass","()Ljava/lang/Class;", env);
  MUST_GET_METHOD_ID(MethodIDs.getName,       MethodClass, NULL, "getName","()Ljava/lang/String;", env);
  MUST_GET_METHOD_ID(MethodIDs.getExceptions, MethodClass, NULL, "getExceptionTypes","()[Ljava/lang/Class;", env);



  MethodClass = VMENV FindClass(env, "java/lang/reflect/Constructor");

  MUST_GET_METHOD_ID(ConstructorIDs.getModifiers,  MethodClass, NULL, "getModifiers","()I", env);

  MUST_GET_METHOD_ID(ConstructorIDs.getParameters, MethodClass, NULL, "getParameterTypes","()[Ljava/lang/Class;", env);
  MUST_GET_METHOD_ID(ConstructorIDs.getClass,      MethodClass, NULL, "getDeclaringClass","()Ljava/lang/Class;", env);
  MUST_GET_METHOD_ID(ConstructorIDs.getName,       MethodClass, NULL, "getName","()Ljava/lang/String;", env);
  MUST_GET_METHOD_ID(ConstructorIDs.getExceptions, MethodClass, NULL, "getExceptionTypes","()[Ljava/lang/Class;", env);

  MethodClass = VMENV FindClass(env, "java/lang/reflect/Modifier");
  ModifierStringID = VMENV GetStaticMethodID(env, MethodClass, "toString", "(I)Ljava/lang/String;");
  if(ModifierStringID == NULL)
     fprintf(stderr, "Exception getting static method for Modifier - toString\n");fflush(stderr);    
 
 return(NULL);
}


