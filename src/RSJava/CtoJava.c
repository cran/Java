
# line 118 "CtoJava.cweb"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

#include "CtoJava.h"

#include "ErrorHandling.h"
#include "JNIUtils.h"

#if defined(_R_)
#include "RSCommon.h"
#include "Rinternals.h"
#include "Rdefines.h"
#endif

#include "ManagerInterfaceUtils.h"


# line 874 "CtoJava.cweb"
static const char *create_Java_classes(JNIEnv *env);



# line 41 "CtoJava.cweb"
#ifndef LDEBUG
#define LDEBUG 0
#endif

# line 138 "CtoJava.cweb"
static JavaVM *jvm;
static JDK1_1InitArgs vm1_args;

#ifdef JNI_VERSION_1_2 

static JavaVMInitArgs vm2_args;
/* Classpath. */
#define N_JDK_OPTIONS 3
#define VMARGS_TYPE JavaVMInitArgs
static JavaVMOption *vm2_options;
static JavaVMInitArgs *vm_args;


#else  /* So its not JNI_VERSION 1.2 */


static JDK1_1InitArgs vm2_args;
#define VMARGS_TYPE JDK1_1InitArgs
static JDK1_1InitArgs *vm_args;


#endif  /* finished the version 1.1 material */


/* the interfaceManager object & its methods */
/* static jclass interfaceManagerClass; */
static jobject interfaceManager;


/* These are the current manager classes. We will phase in the second
   of these as we move to a more Omega-based evaluation mechanism.
*/
#define INTERFACE_MANAGER_CLASSNAME "org/omegahat/Interfaces/NativeInterface/InterfaceManager"
#define OMEGA_INTERFACE_MANAGER_CLASSNAME "org/omegahat/Interfaces/NativeInterface/OmegaInterfaceManager"

# line 201 "CtoJava.cweb"
/* a global ENV pointer, initialized when the virtual machine is
   created, and never changed.  It can be used to detect whether the
   current environment differs from the initial one (as, e.g., when
   the environment is passed in to a native Java method 
*/
static JNIEnv * std_env = NULL;


# line 349 "CtoJava.cweb"
#define ENSURE_VM (std_env!=NULL ? NULL : (interface_start_VM(NULL, NULL,\
                                                          NULL, NULL, NULL,0), NULL))

# line 379 "CtoJava.cweb"
static jmethodID doubleValueID, floatValueID, intValueID, longValueID,
  shortValueID, byteValueID, booleanValueID, charValueID;


# line 385 "CtoJava.cweb"
static const char *do_native_setup(jobject Manager, jobject nullObj, JNIEnv *env);
static const char *init_interfaceManagerClass(jobject Manager, JNIEnv *env);


# line 393 "CtoJava.cweb"
jclass OmegahatInterfaceManagerClass;


# line 403 "CtoJava.cweb"
jobject NullObject=NULL;

# line 600 "CtoJava.cweb"
  /** Reference to the Java class `ForeignReference', resolved when the JVM is initialized.
    */
 jclass MetaForeignReferenceClass = NULL;

  /** Constructor for ForeignReferenceClass which passes the id, the target class name, and the
     base interfaces to implement.
   */
 static jmethodID MetaForeignReferenceConstructorId = 0;

static const char *RS(initForeignReferences)(JNIEnv *env);


# line 838 "CtoJava.cweb"
/* the array type names */
static char *typeNames[] = { "boolean", "byte", "char", "short",
                              "int", "long", "float", "double",
                              "String", "Object", NULL };

/* the predefined array and primitive classes */
static jclass arrayClasses[N_JAVA_TYPES], primitiveClasses[N_JAVA_TYPES];

static char *typeSigs[] = {"Z", "B", "C", "S", "I", "J", "F", "D",
                           "Ljava/lang/String;", "Ljava/lang/Object;",
                         NULL};
static char *arrayTypeSigs[] = {"[Z", "[B", "[C", "[S", "[I", "[J", "[F", "[D",
                                "[Ljava/lang/String;", "[Ljava/lang/Object;",
                         NULL};
static char *wrapperNames[] = { "java/lang/Boolean", "java/lang/Byte", 
                               "java/lang/Character", "java/lang/Short",
                               "java/lang/Integer", "java/lang/Long",
                               "java/lang/Float", "java/lang/Double",
                               "java/lang/String", "java/lang/Object",
                               NULL};

static char *wrapperSigs[] = { "Ljava/lang/Boolean;", "Ljava/lang/Byte;", 
                               "Ljava/lang/Char;", "Ljava/lang/Short;",
                               "Ljava/lang/Integer;", "Ljava/lang/Long;",
                               "Ljava/lang/Float;", "Ljava/lang/Double;",
                               "Ljava/lang/String;", "Ljava/lang/Object;",
                               NULL};

/* the wrapper classes corresponding to primitive types, and the
   corresponding constructor methods from the primitive */
static jclass wrapperClasses[N_JAVA_TYPES];
static jmethodID wrapperConstructor[N_JAVA_TYPES];




# line 1571 "CtoJava.cweb"
const char *
interface_start_VM( char *classpath,  char *omega_home,
                         char *interface_lib_path, 
                         char *java_lib_path, char **user_props, int n_props)
{
  const char *msg;

fprintf(stderr, "In interface_start_VM\n");fflush(stderr);
  
  if(omega_home == NULL)
    omega_home = getenv("OMEGA_HOME");

  if(omega_home != NULL) {
    if(!classpath)
      classpath = getenv("OMEGA_CLASS_PATH");
    if(!classpath)
      classpath = getenv("CLASSPATH");
    if(!classpath) {
        fprintf(stderr, "Can't create Virtual Machine: no class path defined");
        exit(1);
    }

    if(!interface_lib_path) {
      interface_lib_path = calloc(strlen(omega_home) +
                                  strlen("/interfaceManagerX"), 1);
      sprintf(interface_lib_path, "%s/interfaceManager",omega_home);
    } 

    if(!java_lib_path) {
      java_lib_path = getenv("JNI_LIBS");
      if(!java_lib_path) {
           fprintf(stderr, "Can't create Virtual Machine: no Java run-time library defined");
           exit(1);
      }
    }
    msg = create_Java_vm(classpath, interface_lib_path, java_lib_path,
                         user_props, n_props, 1, 1);
  }
  else  {
       fprintf(stderr, "Can't create Virtual Machine: environment variable OMEGA_HOME not defined");
       exit(1);
  }

 return(msg); 
}
    


# line 210 "CtoJava.cweb"
/* create and initialize the virtual machine.  Returns NULL or an
   error message. (use exists_vm to avoid the message if VM exists) */
const char *
create_Java_vm(char *user_classpath, const char *javaLibPath,
                const char *interfaceLibraryFile, 
                 char ** user_properties, 
                  int n_properties, int RequireLibraries, jboolean initializeNative) 
{
    jint res; 
    char *classpath;

    if(std_env) {
      const char *msg;
      if(jvm) {
        msg = "There seems to be a Java Virtual Machine already; no action taken";
      } else
	msg = "It appears that the JVM has been terminated. Cannot restart it";
      return msg;
    }

    /* 
       Append user_classpath to the end of default system class path
     */
    if(!user_classpath)
      /* use the CLASSPATH environment variable as default */
      user_classpath = getenv("CLASSPATH");
    if(!user_classpath) user_classpath = "";

    vm_args = (VMARGS_TYPE *) &vm2_args;
#if defined(JNI_VERSION_1_2)
    vm_args->version = JNI_VERSION_1_2;
#else
     vm_args->version = 0x00010001;
     vm_args->verbose = 1;
#endif
    /* check the version: if 1.2 not available compute a class path */
    if(JNI_GetDefaultJavaVMInitArgs(vm_args) != 0) {
      vm_args = (VMARGS_TYPE *)(&vm1_args);
#if defined(JNI_VERSION_1_1)
      vm_args->version = JNI_VERSION_1_1;
#endif
      vm1_args.classpath = user_classpath;
      if(JNI_GetDefaultJavaVMInitArgs(vm_args) != 0)
        return "Neither 1.1x nor 1.2x version of JDK seems supported";
    }

#if defined(JNI_VERSION_1_2)
    else {
      char *interfaceLibraryProperty, *java_lib_path; 
      int i;
      int total_num_properties, propNum = 0;

      total_num_properties = N_JDK_OPTIONS+n_properties;

      if(RequireLibraries) {
        total_num_properties += 2;
      }
      vm2_options = (JavaVMOption *) calloc(total_num_properties, sizeof(JavaVMOption));
      vm2_args.version = JNI_VERSION_1_2;
      vm2_args.options = vm2_options;
      vm2_args.ignoreUnrecognized = JNI_TRUE;

      classpath = calloc(strlen("-Djava.class.path=") + strlen(user_classpath)+1, sizeof(char));
      sprintf(classpath, "-Djava.class.path=%s", user_classpath);

#if 0      
           /* disable JIT */
      vm2_options[propNum++].optionString = "-Djava.compiler=NONE";
#endif      
           /* user classes */
      vm2_options[propNum++].optionString = classpath;   

      if(RequireLibraries) {
        if(!interfaceLibraryFile || !*interfaceLibraryFile)
          return "no location for interface library supplied";

        interfaceLibraryProperty = calloc(strlen("-DinterfaceLibraryFile=") +
        strlen(interfaceLibraryFile)+1, sizeof(char));
        sprintf(interfaceLibraryProperty, "-DinterfaceLibraryFile=%s", 
                               interfaceLibraryFile);
        if(!javaLibPath || !*javaLibPath)
         return "no location for java library supplied";

        java_lib_path = calloc(strlen("-Djava.library.path=") 
                           + strlen(javaLibPath)+1, sizeof(char));
        sprintf(java_lib_path, "-Djava.library.path=%s", javaLibPath);
          /*this library*/
        vm2_options[propNum++].optionString = interfaceLibraryProperty;
          /* path for dynamic loading*/
        vm2_options[propNum++].optionString = java_lib_path;  
      }

                              /*   print JNI-related messages */
/*  
        vm2_options[propNum++].optionString = "-verbose:class,jni";
*/

      for(i=0; i<n_properties; i++, propNum++)
        vm2_options[propNum].optionString = user_properties[i];

     vm2_args.nOptions = propNum;
    }
    /* Create the Java VM */
    res = JNI_CreateJavaVM(&jvm,(void *)&std_env,(void *)vm_args);
#else

     tmp = (char*)malloc(strlen(user_classpath) +strlen(vm_args->classpath) + 2);
     strcpy(tmp, user_classpath);
     strcat(tmp,":");
     strcat(tmp,vm_args->classpath);
     vm_args->classpath = tmp;

#endif
    if (res != 0)
        return "Can't create Java Virtual Machine";
#if LDEBUG
    else
       printf("Got it\n"); fflush(stdout);
#endif

    if(initializeNative == JNI_TRUE) {
     const char *msg;	
       msg = do_native_setup(NULL, NULL, std_env);
       if(msg)
	  return(msg);
    }

    return NULL;
}

int
exists_Java_vm()
{
return(std_env ? 1 : 0);
}



# line 411 "CtoJava.cweb"
static const char *
do_native_setup(jobject Manager, jobject nullObj, JNIEnv *env)
{
  const char *msg = NULL;
  /* Should only be called once.  Check needed both for multiple calls
   from the Java constructor, and also for recursion from the
   constructor, when initial call was from create_Java_vm; in this
   latter case, however, we didn't get the special NullObject until
   this second call.  */
  if(NullObject == NULL)
    NullObject = nullObj;

  if(OmegahatInterfaceManagerClass) /* already initialized. */
     return(msg);

  msg = init_interfaceManagerClass(Manager, env);
  if(msg) 
    return(msg); /* or throw exception ? */;
  msg = create_Java_classes(env);

  return(msg); /* or throw exception ? */;
}



# line 483 "CtoJava.cweb"
static const char *
init_interfaceManagerClass(jobject Manager, JNIEnv *env)
{
  jmethodID constructor;
  jclass hashClass; 
  char buf[256]; 
  int i;

  if(Manager != NULL) {
      interfaceManager = Manager;
  } else {
       /* So we create the Manager object.
	  We do so by looking for the name of the class
	  to use in the System property `InterfaceManagerClass'
	  If that is not set, we use ou default
	    (org.omegahat.Interfaces.NativeInterface.OmegaInterfaceManager)

	  If we cannot find the class, it is likely that user will want
	  to restart the VM. In that case
	*/
    jboolean isCopy = FALSE;
    const char *className = NULL;

    className = RS_JAVA(getSystemProperty)("InterfaceManagerClass", &isCopy, env);

    
    if(className == NULL) {
       className = OMEGA_INTERFACE_MANAGER_CLASSNAME;
       isCopy = FALSE;
    }
    
    OmegahatInterfaceManagerClass = VMENV FindClass(env, className);
    if(OmegahatInterfaceManagerClass == NULL) {
       RS_JAVA(terminateJava)();
       std_env = NULL;
fprintf(stderr, "JVM %p Env %p\n", jvm, std_env);fflush(stderr);	
       return("Cannot find the Omegahat interface manager class. Check you classpath!");
    }

      
     if(isCopy) {
        /* Free the string classname. Can't do it here easily.
	   This one got away.
	 */
	 
     }

    
    if(OmegahatInterfaceManagerClass == NULL) {
       RS_JAVA(terminateJava)();
       std_env = NULL;
fprintf(stderr, "JVM %p Env %p\n", jvm, std_env);fflush(stderr);
       return get_Java_exception("can't find OmegaInterfaceManager class",env);
    }

    MUST_GET_METHOD_ID(constructor, OmegahatInterfaceManagerClass, NULL, "<init>", "()V", env);
    interfaceManager = VMENV NewObject(env, OmegahatInterfaceManagerClass, constructor);
  }

  hashClass = VMENV FindClass(env, "java/util/Hashtable");
  if(hashClass == NULL)
    return("can't find class java/lang/Hashtable");

  initializeOmegahatManagerMethods(OmegahatInterfaceManagerClass, env);

  
  
# line 663 "CtoJava.cweb"
  MUST_GET_METHOD_ID(intValueID, VMENV FindClass(env, "java/lang/Integer"),
                     NULL, "intValue", "()I", env);
  MUST_GET_METHOD_ID(longValueID, VMENV FindClass(env, "java/lang/Long"),
                     NULL, "longValue", "()J", env);
  MUST_GET_METHOD_ID(shortValueID, VMENV FindClass(env, "java/lang/Short"),
                     NULL, "shortValue", "()S", env);
  MUST_GET_METHOD_ID(floatValueID, VMENV FindClass(env, "java/lang/Float"),
                     NULL, "floatValue", "()F", env);
  MUST_GET_METHOD_ID(doubleValueID, VMENV FindClass(env, "java/lang/Double"),
                     NULL, "doubleValue", "()D", env);
  MUST_GET_METHOD_ID(booleanValueID, VMENV FindClass(env, "java/lang/Boolean"),
                     NULL, "booleanValue", "()Z", env);
  MUST_GET_METHOD_ID(charValueID, VMENV FindClass(env, "java/lang/Character"),
                     NULL, "charValue", "()C", env);

  MUST_GET_METHOD_ID(byteValueID, VMENV FindClass(env, "java/lang/Byte"),
                     NULL, "byteValue", "()B", env);

    /* find the constructor methods for the primitive (wrapper) classes
     */
  for(i=0; i<JAVA_STRING; i++) {
    sprintf(buf, "(%s)V", typeSigs[i]);
    wrapperClasses[i] = VMENV FindClass(env, wrapperNames[i]);
    if(wrapperClasses[i] == NULL)
         break; /* there better be an exception */
    wrapperClasses[i] = VMENV NewGlobalRef(env, wrapperClasses[i]);
    wrapperConstructor[i] = get_method_id(wrapperClasses[i], NULL, "<init>", buf, env);
    if(wrapperConstructor[i] == NULL)
       break;
  }


# line 550 "CtoJava.cweb"
    
   RS(initForeignReferences)(env);
     /* check for an exception in the loop. */
  return(get_Java_exception(NULL, env));
}



# line 883 "CtoJava.cweb"
static const char *
create_Java_classes(JNIEnv *env) 
{
  jobjectArray tmp; 
  jclass cls; int i; 
  jmethodID mid;

  cls = VMENV FindClass(env, "java/lang/Class");
  if(cls == NULL)
    return("can't get class \"java/lang/Class\"");

  mid = VMENV GetStaticMethodID(env, OmegahatInterfaceManagerClass,
                                "getPrimitiveClasses", "()[Ljava/lang/Class;");
  if(mid==NULL) return get_Java_exception("no getPrimitiveClasses method",env);
  tmp = (jobjectArray) VMENV CallStaticObjectMethod(env, OmegahatInterfaceManagerClass, mid);
  if(tmp==NULL) return get_Java_exception("no primitiveClasses array", env);
  if(VMENV GetArrayLength(env, tmp) < N_JAVA_TYPES)
    return get_Java_exception("fewer primitive classes than needed", env);
  for(i=0; i<N_JAVA_TYPES; i++) {
    primitiveClasses[i] = (jclass) VMENV GetObjectArrayElement(env, tmp, i);
    if(primitiveClasses[i] == NULL)
      return get_Java_exception(wrapperNames[i], env);
  }
  mid = VMENV GetStaticMethodID(env, OmegahatInterfaceManagerClass,
                                "getArrayClasses", "()[Ljava/lang/Class;");
  if(mid==NULL) return get_Java_exception("no getArrayClasses method", env);
  tmp = (jobjectArray) VMENV CallStaticObjectMethod(env, OmegahatInterfaceManagerClass, mid);
  if(tmp==NULL) return get_Java_exception("no arrayClasses array", env);
  if(VMENV GetArrayLength(env, tmp) < N_JAVA_TYPES)
    return get_Java_exception("fewer array classes than needed", env);
  for(i=0; i<N_JAVA_TYPES; i++) {
    arrayClasses[i] = (jclass) VMENV GetObjectArrayElement(env, tmp, i);
    if(arrayClasses[i] == NULL)
      return get_Java_exception(wrapperNames[i], env);
     arrayClasses[i] = VMENV NewGlobalRef(env, arrayClasses[i]);
  }
  return get_Java_exception(NULL, env);
}




# line 929 "CtoJava.cweb"
int
whatJavaType(void *javaObject, long *length_p, char **type_p, JNIEnv *env) {
  jobject obj; int i, length;
  obj = (jobject)javaObject;

# line 950 "CtoJava.cweb"
    /* do a linear search of primitive  & String types.  Might be nice to
       fashion something faster, but this will do. We start with
       String and Double, thinking these likely to occur fairly often. */
    for(i = JAVA_STRING; i >= 0; i--) {
      if(VMENV IsInstanceOf(env, obj, arrayClasses[i]) == JNI_TRUE) {
        length = 1; /* array */
        break;
      }
      if(VMENV IsInstanceOf(env, obj, primitiveClasses[i]) == JNI_TRUE) {
        length = -1; /* scalar */
        break;
      }
    }
 /* and finally, look for Object[] -- Object is the default */
 if(i < 0) {
   i = JAVA_OBJECT;
   if(VMENV IsInstanceOf(env, obj, arrayClasses[i])== JNI_TRUE)
     length = 1;
   else length = -1;
 }
 /* now compute the length & set the type if requested */
 if(length > 0) {
   length = VMENV GetArrayLength(env, obj);
   if(type_p) *type_p = arrayTypeSigs[i];
 }
 else if(type_p) *type_p = typeSigs[i];
 if(length_p) *length_p = length;
 return(i);
}
 
# line 1002 "CtoJava.cweb"
enum java_types
which_java_type(char *signature, jboolean *isArray, jboolean *builtIn) {
  int fullSig;
  enum java_types value = JAVA_INVALID;

#if LDEBUG
printf("which_java_type %s\n", signature);fflush(stdout);
#endif
  
  /* Set the defaults. */
  if(builtIn)
    *builtIn = JNI_TRUE;

  if(isArray) {
     *isArray = JNI_FALSE;
  }


    /* Now we are ready. */
  if(*signature == '[') {
    if(isArray)
       *isArray = JNI_TRUE;
    signature++;
  }

  if(signature[1] == '\0') {
    switch(*signature) {
    case 'Z': 
        return JAVA_BOOLEAN;
    case 'B': 
        return JAVA_BYTE;
    case 'C': 
        return JAVA_CHAR;
    case 'S': 
        return JAVA_SHORT;
    case 'I': 
        return JAVA_INT;
    case 'J': 
        return JAVA_LONG;
    case 'F': 
        return JAVA_FLOAT;
    case 'D': 
        return JAVA_DOUBLE;
    case 'N': 
        return JAVA_NAMED_REFERENCE;
    case 'A': 
        return JAVA_ANONYMOUS_REFERENCE;
    case 'R': 
        return JAVA_GENERIC_REFERENCE;
    case 'W': 
        return RS_NAMED_REFERENCE;
    case 'X': 
        return RS_ANONYMOUS_REFERENCE;          

    default: /* unknown */

#ifdef LDEBUG    
     fprintf(stderr, "[which_java_type] Unknown data type %s\n", signature);fflush(stderr);
#endif        
     if(builtIn)
       *builtIn = JNI_FALSE;
      return JAVA_INVALID;
    }
  }

  fullSig = (signature[0] == 'L');
  if(fullSig)
    signature++;
  if(strncmp(signature, "java.lang.", 10) == 0 ||
    strncmp(signature, "java/lang/", 10) == 0 )
    signature += 10;
  switch(signature[0]) {
  case 'S':
    if(strncmp(signature, "String",6) == 0)
      { value = JAVA_STRING;
        signature+=6;
      }
    else if(strncmp(signature, "Short",5) == 0)
      { value = JAVA_SHORT;
        signature+=5;
      }
    break;
  case 'D':
    if(strncmp(signature, "Double", 6) == 0)
      { value = JAVA_DOUBLE;
        signature+=6;
      }
    break;
  case 'I':
    if(strncmp(signature, "Integer", 7) == 0)
      { value = JAVA_INT;
        signature+=7;
      }
      break;
  case 'F':
    if(strncmp(signature, "Float", 5) == 0)
      { value = JAVA_FLOAT; signature+=5;}
    break;
  case 'L':
    if(strncmp(signature, "Long", 4) == 0)
      { value = JAVA_LONG; signature+=4;}
    break;
  case 'B':
    if(strncmp(signature, "Byte", 4) == 0)
      { value = JAVA_LONG; 
        signature+=4;
      }
    else if(strncmp(signature, "Boolean", 7) == 0)
      { value = JAVA_BOOLEAN; 
        signature+=7;
      }
    break;
  case 'O':
    if(strncmp(signature, "Object", 6) == 0)
      { value = JAVA_OBJECT;
        signature+=6;
        if(builtIn)
          *builtIn = JNI_FALSE;
      }
    break;
   default:

      if(strcmp(signature,"org.omegahat.Interfaces.NativeInterface.NamedReference") == 0) {
        value = JAVA_NAMED_REFERENCE;    
        *builtIn = JNI_FALSE;
         return(value);
      } else if(strcmp(signature,"org.omegahat.Interfaces.NativeInterface.AnonymousReference") == 0) {
        *builtIn = JNI_FALSE;
        value = JAVA_ANONYMOUS_REFERENCE;
        return(value);
      } else if(strcmp(signature,"org.omegahat.Interfaces.NativeInterface.InterfaceReference") == 0) {
        *builtIn = JNI_FALSE;
        value = JAVA_GENERIC_REFERENCE;
        return(value);
      }

#if LDEBUG    
     fprintf(stderr, "[which_java_type] Unknown data type %s\n", signature);fflush(stderr);
#endif    
  }
  /* now it has to end with an optional ";" */
  if(*signature && strcmp(signature, ";")!=0)
    return JAVA_INVALID;
  return value;
}


# line 1167 "CtoJava.cweb"
void *
toJavaType(JNIEnv *env, java_type which, long length, void *data, jboolean isArray) {
  jboolean isCopy;
  long i;
  
  if(isArray)
    switch(which) {
  case JAVA_DOUBLE: {
    jdoubleArray obj; jdouble *in, *out;
    obj = VMENV NewDoubleArray(env, length);
    out = VMENV GetDoubleArrayElements(env, obj, &isCopy);
    in = (jdouble *)data;
    for(i=0; i<length; i++) out[i] = in[i];
    if(isCopy == JNI_TRUE)
      VMENV ReleaseDoubleArrayElements(env, obj, out, 0);
    return (void *)obj;
  }
  case JAVA_FLOAT: {
    jfloatArray obj; jfloat *in, *out;
    obj = VMENV NewFloatArray(env, length);
    out = VMENV GetFloatArrayElements(env, obj, &isCopy);
    in = (jfloat *)data;
    for(i=0; i<length; i++) out[i] = in[i];
    if(isCopy == JNI_TRUE)
      VMENV ReleaseFloatArrayElements(env, obj, out, 0);
    return (void *)obj;
  }
  case JAVA_LONG: {
    jlongArray obj; jlong *in, *out;
    obj = VMENV NewLongArray(env, length);
    out = VMENV GetLongArrayElements(env, obj, &isCopy);
    in = (jlong *)data;
    for(i=0; i<length; i++) out[i] = in[i];
    if(isCopy == JNI_TRUE)
      VMENV ReleaseLongArrayElements(env, obj, out, 0);
    return (void *)obj;
  }
  case JAVA_INT: {
    jintArray obj; jint *in, *out;
    obj = VMENV NewIntArray(env, length);
    out = VMENV GetIntArrayElements(env, obj, &isCopy);
    in = (jint *)data;
    for(i=0; i<length; i++) out[i] = in[i];
    if(isCopy == JNI_TRUE)
      VMENV ReleaseIntArrayElements(env, obj, out, 0);
    return (void *)obj;
  }
  case JAVA_SHORT: {
    jshortArray obj; jshort *in, *out;
    obj = VMENV NewShortArray(env, length);
    out = VMENV GetShortArrayElements(env, obj, &isCopy);
    in = (jshort *)data;
    for(i=0; i<length; i++) out[i] = in[i];
    if(isCopy == JNI_TRUE)
      VMENV ReleaseShortArrayElements(env, obj, out, 0);
    return (void *)obj;
  }
  case JAVA_CHAR: {
    jcharArray obj; jchar *in, *out;
    obj = VMENV NewCharArray(env, length);
    out = VMENV GetCharArrayElements(env, obj, &isCopy);
    in = (jchar *)data;
    for(i=0; i<length; i++) out[i] = in[i];
    if(isCopy == JNI_TRUE)
      VMENV ReleaseCharArrayElements(env, obj, out, 0);
    return (void *)obj;
  }
  case JAVA_BYTE: {
    jbyteArray obj; jbyte *in, *out;
    obj = VMENV NewByteArray(env, length);
    out = VMENV GetByteArrayElements(env, obj, &isCopy);
    in = (jbyte *)data;
    for(i=0; i<length; i++) out[i] = in[i];
    if(isCopy == JNI_TRUE)
      VMENV ReleaseByteArrayElements(env, obj, out, 0);
    return (void *)obj;
  }
  case JAVA_BOOLEAN: {
    jbooleanArray obj; jboolean *in, *out;
    obj = VMENV NewBooleanArray(env, length);
    out = VMENV GetBooleanArrayElements(env, obj, &isCopy);
    in = (jboolean *)data;
    for(i=0; i<length; i++) out[i] = in[i];
    if(isCopy == JNI_TRUE)
      VMENV ReleaseBooleanArrayElements(env, obj, out, 0);
    return (void *)obj;
  }
  case JAVA_STRING: {
    jobjectArray obj;
    char **in;
    jstring outEl;
    obj = VMENV NewObjectArray(env, length, primitiveClasses[which], NULL);
#ifndef _R_    
    in = (char **)data;
#endif
    for(i=0; i<length; i++) {
#ifndef _R_      
        VMNewStringAssign(outEl, in[i], env);
#else
  outEl = VMENV NewStringUTF(env, CHAR(STRING_ELT((SEXP)data, i)));
//        VMNewStringAssign(outEl, CHAR(CHARACTER_DATA((SEXP)in)[i]), env);     
#endif  
        VMENV SetObjectArrayElement(env, obj, i, outEl);
        VMENV DeleteLocalRef(env, outEl);
        }
    return (void *)obj;
  }
  case JAVA_OBJECT: {
    jobjectArray obj; void **in; jobject outEl;
    obj = VMENV NewObjectArray(env, length, primitiveClasses[which], NULL);
    in = (void **)data;
    for(i=0; i<length; i++) {
        outEl = (jobject) in[i];
        VMENV SetObjectArrayElement(env, obj, i, outEl);
        }
    return (void *)obj;
  }
  default:
    fprintf(stderr, "Unhandled case **\n");fflush(stderr);
  }
  else switch(which) {
    /* as of now, I know no way to generate a primitive type object
       from the JNI.  The class defined by, e.g., Double.TYPE, to
       correspond to primitive type double has no constructors.
       Instead, we create an object from the corresponding wrapper class. */
  case JAVA_DOUBLE:
    return (void *) VMENV NewObject(env, wrapperClasses[which], wrapperConstructor[which],
                                    *((jdouble *)data));
  case JAVA_FLOAT:
    return (void *) VMENV NewObject(env, wrapperClasses[which], wrapperConstructor[which],
                                    *((jfloat *)data));
  case JAVA_LONG:
    return (void *) VMENV NewObject(env, wrapperClasses[which], wrapperConstructor[which],
                                    *((jlong *)data));
  case JAVA_INT:
    return (void *) VMENV NewObject(env, wrapperClasses[which], wrapperConstructor[which],
                                    *((jint *)data));
  case JAVA_SHORT:
    return (void *) VMENV NewObject(env, wrapperClasses[which], wrapperConstructor[which],
                                    *((jshort *)data));
  case JAVA_BYTE:
    return (void *) VMENV NewObject(env, wrapperClasses[which], wrapperConstructor[which],
                                    *((jbyte *)data));
  case JAVA_BOOLEAN:
    return (void *) VMENV NewObject(env, wrapperClasses[which], wrapperConstructor[which],
                                    *((jboolean *)data));
  case JAVA_STRING:
    return (void *)(VMENV NewStringUTF(env, *(char **)data));
  case JAVA_OBJECT:
    return (void *)data;

  default:
     fprintf(stderr, "Default (unhandled) case for toJavaObject\n"); fflush(stderr);
  }

 return((void *) NULL);  
}


# line 1350 "CtoJava.cweb"
const char *
fromJavaType(JNIEnv *env, jobject obj, java_type which, long n, void *ptr,
             jboolean isArray, int sz, alloc_fun alloc)
{
  jboolean isCopy; long i; jvalue scalar;
 if(isArray)
  switch(which) {
  case JAVA_DOUBLE:
    { jdouble *in = VMENV GetDoubleArrayElements(env, (jdoubleArray)obj, &isCopy);
    double *out = (double *)ptr;
    if(in) {
      if(sizeof(*in) == sizeof(*out)) memcpy(out, in, n*sizeof(*in));
      else for(i=0; i<n; i++)out[i] = in[i];
      if(isCopy) VMENV ReleaseDoubleArrayElements(env, obj, in, 0);
    }
    return get_Java_exception(NULL, env);
    }
  case JAVA_FLOAT:
    { jfloat *in = VMENV GetFloatArrayElements(env, (jfloatArray)obj, &isCopy);
    float *out = (float *)ptr;
    if(sizeof(*in) == sizeof(*out)) memcpy(out, in, n*sizeof(*in));
    else for(i=0; i<n; i++)out[i] = in[i];
    if(isCopy) VMENV ReleaseFloatArrayElements(env, obj, in, 0);
    return get_Java_exception(NULL, env);
    }
# line 1382 "CtoJava.cweb"
  case JAVA_INT:
    { jint *in = VMENV GetIntArrayElements(env, (jintArray)obj, &isCopy);
    if(sizeof(jint) == sz) {
      jint *out = (jint *)ptr;
      memcpy(out, in, n*sizeof(*in));
    }
    else if(sizeof(jlong) == sz) {
      jlong *out = (jlong *)ptr;
      for(i=0; i<n; i++)out[i] = in[i];
    }
    else if(sizeof(jshort) == sz) {
      jshort *out = (jshort *)ptr;
      for(i=0; i<n; i++)out[i] = in[i];
    }
    if(isCopy) VMENV ReleaseIntArrayElements(env, obj, in, 0);
    return get_Java_exception(NULL, env);
    }
  case JAVA_LONG:
    {
     jlong *in = VMENV GetLongArrayElements(env, (jlongArray)obj, &isCopy);
     double *out = (double *)ptr;
      for(i=0; i<n; i++)
	 out[i] = in[i];
     if(isCopy) VMENV ReleaseLongArrayElements(env, obj, in, 0);
     return get_Java_exception(NULL, env);
    }
  case JAVA_SHORT:
    {
      jshort *in = VMENV GetShortArrayElements(env, (jshortArray)obj, &isCopy);
    if(sizeof(jshort) == sz) {
      jshort *out = (jshort *)ptr;
      memcpy(out, in, n*sizeof(*in));
    }
    else if(sizeof(jint) == sz) {
      jint *out = (jint *)ptr;
      for(i=0; i<n; i++)out[i] = in[i];
    }
    else if(sizeof(jlong) == sz) {
      jlong *out = (jlong *)ptr;
      for(i=0; i<n; i++)out[i] = in[i];
    }
    if(isCopy) VMENV ReleaseShortArrayElements(env, obj, in, 0);
    return get_Java_exception(NULL, env);
    }
# line 1439 "CtoJava.cweb"
  case JAVA_STRING:
    { 
     jobjectArray in = (jobjectArray) obj;
     jstring inEl;
     const char *inString;
     char *outString;
     jboolean isCopy;
     char **out = (char **)ptr; /* not true for R, but no harm. */

     for(i=0; i<n; i++) {
      inEl = VMENV GetObjectArrayElement(env, in, i);
      if(inEl == NULL) 
         break;
      inString = VMENV GetStringUTFChars(env, inEl, &isCopy);
      if(inString == NULL) 
        break;
#if defined(_R_)
         /* Assumes we have derefernced a NEW_CHARACTER()
            to create ptr, i.e.
               ptr = CHARACTER_DATA(value) 
          */
       ((SEXP*)ptr )[i] = COPY_TO_USER_STRING(inString);
#else
      outString = out[i] = (*alloc)(strlen(inString)+1);
      strcpy(outString, inString);
#endif

      if(isCopy == JNI_TRUE)
        VMENV ReleaseStringUTFChars(env, inEl, inString);
      VMENV DeleteLocalRef(env, inEl);
     }
     return get_Java_exception(NULL, env);
    }

# line 1484 "CtoJava.cweb"
     default:
        return(get_Java_exception(NULL, env));
  }
  else switch(which) {
    case JAVA_STRING: {
      const char *in;
      char *out;
      in = VMENV GetStringUTFChars(env, (jstring)obj, &isCopy);
      out = (*alloc)(strlen(in)+1); strcpy(out, in);
      *(char **)ptr = out;
      return get_Java_exception(NULL, env);
      }
    case JAVA_DOUBLE:
      scalar.d = VMENV CallDoubleMethod(env, obj, doubleValueID);
      *(double *)ptr = scalar.d;
      return get_Java_exception(NULL, env);
    case JAVA_FLOAT:
      scalar.f = VMENV CallFloatMethod(env, obj, floatValueID);
      *(float *)ptr = scalar.f;
      return get_Java_exception(NULL, env);
    case JAVA_INT:
      scalar.i = VMENV CallIntMethod(env, obj, intValueID);
      *(int *)ptr = scalar.i;
      return get_Java_exception(NULL, env);
    case JAVA_SHORT:
      scalar.s = VMENV CallShortMethod(env, obj, shortValueID);
      *(int *)ptr = scalar.s;
      return get_Java_exception(NULL, env);
    case JAVA_LONG:
      scalar.j = VMENV CallLongMethod(env, obj, longValueID);
      *(double *)ptr = scalar.j;
      return get_Java_exception(NULL, env);
   
    case JAVA_BOOLEAN:
      scalar.z = VMENV CallBooleanMethod(env, obj, booleanValueID);
      *(long *)ptr = scalar.z;
      return get_Java_exception(NULL, env);
   
    case JAVA_CHAR:
      scalar.c = VMENV CallCharMethod(env, obj, charValueID);
      *(char *)ptr = scalar.c;
      return get_Java_exception(NULL, env);
    default:
       fprintf(stderr, "Default (unhandled) case for fromJavaType\n"); fflush(stderr);
  }
  return throw_Java_exception("No implementation for this dataType",  NULL, env);
}






# line 1631 "CtoJava.cweb"
JNIEnv *
getJNIEnv()
{
 return(std_env);
}

JNIEnv *
getThreadJNIEnv(jboolean *wasAttached)
{
  JNIEnv *env;
  jint status;

#ifdef __cplusplus  
    status = getJavaVM()->GetEnv((void**)&env, JNI_VERSION_1_1);

    if(status == JNI_EVERSION)
       status = getJavaVM()->GetEnv((void**)&env, JNI_VERSION_1_2);

    if(status == JNI_EDETACHED) {
      getJavaVM()->AttachCurrentThread((void**)&env, (void*)NULL);
      *wasAttached = JNI_FALSE;
    } else if(status == JNI_OK)
      *wasAttached = JNI_TRUE;
    else {
      fprintf(stderr, "Something seriously wrong in getting JNIEnv\n");fflush(stderr);   
      return(NULL);
    }
#else
    status = (*getJavaVM())->GetEnv(getJavaVM(), (void**)&env, JNI_VERSION_1_1);

    if(status == JNI_EVERSION)
       status = (*getJavaVM())->GetEnv(getJavaVM(), (void**)&env, JNI_VERSION_1_2);

    if(status == JNI_EDETACHED) {
      (*getJavaVM())->AttachCurrentThread(getJavaVM(), (void**)&env, (void*)NULL);
      *wasAttached = JNI_FALSE;
    } else if(status == JNI_OK)
      *wasAttached = JNI_TRUE;
    else {
      fprintf(stderr, "Something seriously wrong in getting JNIEnv\n");fflush(stderr);   
      return(NULL);
    }    
#endif
    
    /*
      fprintf(stderr, "Got JNIEnv * %x ( ?= %x)\n", env, getJNIEnv());fflush(stderr);
     */
 return(env);
}

jint
releaseThreadJNIEnv(jboolean wasAttached)
{
  jint status = 0;
  if(wasAttached == JNI_FALSE) {
#ifdef DEBUG
    fprintf(stderr, "Must detach thread from JVM.\n");fflush(stderr);
#endif
#ifdef __cplusplus
    status = getJavaVM()->DetachCurrentThread();
#else
    status = (*getJavaVM())->DetachCurrentThread(getJavaVM());
#endif
    
    if(status != 0) {
      fprintf(stderr, "Error releasing the current thread.\n");fflush(stderr);
    }
  }

  return(status);
}  


JNIEnv *
interface_VM_env(void)
{
  return std_env;
}

# line 1718 "CtoJava.cweb"
JavaVM *
getJavaVM()
{
  return(jvm);
}  

void
RS_JAVA(terminateJava)()
{
 JavaVM *vm = getJavaVM();

  (*vm)->DestroyJavaVM(vm);
   jvm = NULL;
/*
  std_env = NULL;
*/
}  




# line 747 "CtoJava.cweb"
jobject getInterfaceManager(void) {
  return interfaceManager;
}

jobject setInterfaceManager(jobject value, JNIEnv *env) {
  jobject previous = interfaceManager;
  init_interfaceManagerClass(value, env);
  return previous;
}



# line 614 "CtoJava.cweb"
/**
  Lookup and cache handles for the MetaForeignReference class
  and also its constructor that takes a string and an array of strings (interfacese and classes
  that the reference should implement and extend).
 */
static const char *
RS(initForeignReferences)(JNIEnv *env)
{
  MetaForeignReferenceClass = VMENV FindClass(env, "org/omegahat/Interfaces/NativeInterface/MetaForeignReference");
  MUST_GET_METHOD_ID(MetaForeignReferenceConstructorId, MetaForeignReferenceClass,
                      NULL, "<init>", "(Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;)V", env);

  if(get_Java_exception(NULL, env)) {
      PROBLEM "error initializing foreign reference class"
      ERROR;
  }
  return(NULL);
}  


# line 640 "CtoJava.cweb"
/**
  Create an instance of a MetaForeignReference object. 
 */
jobject
RS(createForeignReference)(JNIEnv *env, jstring id, jstring className, jobjectArray types)
{
 const char *msg;    
 jobject obj =  VMENV NewObject(env, MetaForeignReferenceClass, MetaForeignReferenceConstructorId, id, className, types);

 if((msg = get_Java_exception(NULL, env))) {
   PROBLEM "creating foreign reference %s",msg
   ERROR;
  }

 return(obj);
}  

# line 563 "CtoJava.cweb"
const char *
RS_JAVA(getSystemProperty)(const char *name,  jboolean *isCopy, JNIEnv *env)
{
 jclass propClass;
 jmethodID mid;
 const char *value = NULL;
 jstring in, jval;
   
 *isCopy = FALSE;
 propClass = VMENV FindClass(env, "java/lang/System");
 if(propClass == NULL) {
fprintf(stderr, "Can't find class java.lang.System\n");fflush(stderr);
   return(NULL);
 }

  mid = VMENV GetStaticMethodID(env, propClass, "getProperty", "(Ljava/lang/String;)Ljava/lang/String;");
  if(mid == NULL) {
   const char *tmp = get_Java_exception("Failed to get the getProperty() method", env);
   fprintf(stderr, "%s\n", tmp); fflush(stderr);
   return(NULL);
  }

  in = VMENV NewStringUTF(env, name);

  jval = VMENV CallStaticObjectMethod(env, propClass, mid, in);

  if(jval && jval != NULL_JAVA_OBJECT) {
    value = VMENV GetStringUTFChars(env, jval, isCopy);
    value = strdup(value);
    *isCopy = TRUE;
  } 

 return(value);
}    


# line 1740 "CtoJava.cweb"
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
