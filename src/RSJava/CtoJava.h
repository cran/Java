
# line 76 "CtoJava.cweb"
#ifndef INTERFACE_MANAGER_CTOJAVA_H
#define INTERFACE_MANAGER_CTOJAVA_H 1

#define RS(a) RS_##a
#define RS_JAVA(a) RS_JAVA_##a

#include <jni.h>
#include <malloc.h>


# line 186 "CtoJava.cweb"
extern JNIEnv *std_env;  /* initialized in create_Java_vm */


# line 365 "CtoJava.cweb"
extern jobject NullObject;

# line 398 "CtoJava.cweb"
extern jclass OmegahatInterfaceManagerClass;



# line 811 "CtoJava.cweb"
/* an enumerator for accessible Java types  */
enum java_types {JAVA_BOOLEAN, /* 0 */
                 JAVA_BYTE,    /* 1 */
                 JAVA_CHAR,    /* 2 */
                 JAVA_SHORT,   /* 3 */
                 JAVA_INT,     /* 4 */
                 JAVA_LONG,    /* 5 */
                 JAVA_FLOAT,   /* 6 */
                 JAVA_DOUBLE,  /* 7 */
                 JAVA_STRING,  /* 8 */
                 JAVA_OBJECT,  /* 9 */
                 JAVA_NAMED_REFERENCE, /* 10 */
                 JAVA_ANONYMOUS_REFERENCE, /* 11 */
                 JAVA_GENERIC_REFERENCE,  /* 12 */
                 RS_NAMED_REFERENCE,  /* 13 */
                 RS_ANONYMOUS_REFERENCE,  /* 14 */                 
                 JAVA_INVALID           /* 15 */
                };  

typedef enum java_types java_type;

/* #define N_JAVA_TYPES JAVA_INVALID */

#define N_JAVA_TYPES JAVA_NAMED_REFERENCE


# line 103 "CtoJava.cweb"
#ifdef _WIN32
#define PATH_SEPARATOR ';'
#else /* UNIX */
#define PATH_SEPARATOR ':'
#endif


#ifdef __cplusplus
#define VMENV (env)->
#else
#define VMENV (*env)->
#endif

# line 370 "CtoJava.cweb"
#define NULL_JAVA_OBJECT NullObject


# line 459 "CtoJava.cweb"
/* macro to require finding method id, else return exception message
 */
#define MUST_GET_METHOD_ID(id, cls1, cls2, which, sig, env) if((id = get_method_id(cls1, cls2, which, sig,env))==NULL)\
   return java_method_exception(which);


# line 727 "CtoJava.cweb"
             /* macros to guard conversions in the method interface */
#define VMNewStringAssign(name, string, env) {if((string) == NULL || \
    (name=VMENV NewStringUTF(env, (string)), get_Java_exception(NULL,env))) \
         return(nullStringException(string,env));}



# line 1329 "CtoJava.cweb"
typedef void *(*alloc_fun)();
#define LOCAL_ALLOC(n) (alloc ? (*alloc)(n) : malloc(n))



# line 91 "CtoJava.cweb"
#ifdef __cplusplus
extern "C" {
#endif
# line 191 "CtoJava.cweb"
extern const char *create_Java_vm(char *user_classpath, const char *javaLibPath,
                                  const char *interfaceLibraryFile,
                                  char ** user_properties, int n_properties, 
                                  int RequireLibraries, jboolean initializeNative);
extern int exists_Java_vm(void);



# line 559 "CtoJava.cweb"
const char *RS_JAVA(getSystemProperty)(const char *name,  jboolean *isCopy, JNIEnv *env);

# line 636 "CtoJava.cweb"
jobject RS(createForeignReference)(JNIEnv *env, jstring id, jstring className, jobjectArray types);


# line 742 "CtoJava.cweb"
extern jobject getInterfaceManager(void);
extern jobject setInterfaceManager(jobject value, JNIEnv *env);

# line 925 "CtoJava.cweb"
int whatJavaType(void *javaObject, long *length_p, char **type_p, JNIEnv *env);

# line 999 "CtoJava.cweb"
extern enum java_types which_java_type(char *signature, jboolean *isArray, jboolean *builtIn);

# line 1163 "CtoJava.cweb"
extern  void *toJavaType(JNIEnv *env, java_type which, long length, void *data,
                         jboolean isArray);
# line 1347 "CtoJava.cweb"
extern const char *fromJavaType(JNIEnv *env, jobject obj, java_type which, long n, void *ptr,
                                                     jboolean isArray, int sz, alloc_fun alloc);
# line 1565 "CtoJava.cweb"
extern const char *interface_start_VM(char *classpath, char *omega_home,
                               char *interface_lib_path, 
                               char *java_lib_path, char **
                                      user_props, int n_props);
# line 1620 "CtoJava.cweb"
JNIEnv *interface_VM_env(void);

JNIEnv *getJNIEnv();
JNIEnv *getThreadJNIEnv(jboolean *wasAttached);
jint   releaseThreadJNIEnv(jboolean wasAttached);


# line 1712 "CtoJava.cweb"
JavaVM *getJavaVM();
void RS_JAVA(terminateJava)();

# line 95 "CtoJava.cweb"
#ifdef __cplusplus
}
#endif
  
#endif /* end of conditional inclusion. */

