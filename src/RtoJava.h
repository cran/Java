# line 106 "RtoJava.cweb"
#ifndef INTERFACE_MANAGER_STOJAVA_H
#define INTERFACE_MANAGER_STOJAVA_H 1

#include "RSCommon.h"

#undef FALSE
#undef TRUE
typedef enum {FALSE, TRUE}  boolean; 

/* From memory.c in R/src/main. */
extern void R_PreserveObject(SEXP object);
extern void R_ReleaseObject(SEXP object);

#define GET_MODE(x) (x)->sxpinfo.type

/* some defines conflict with with bool.h, included by jni.h; we trust
   & believe we don't need the defines in there, and suppress its loading */
#define _BOOL_H_  1
#include "jni.h"
#include "CtoJava.h"



# line 553 "RtoJava.cweb"
#define IS_S_ENVIRONMENT (env == getJNIEnv())

#define END_MESSAGE )
#define GET_MESSAGE R_problem_buf;}



# line 573 "RtoJava.cweb"
#define CATCH_EXCEPTION s_catch_exception(TRUE)
#define CLEAR_EXCEPTION s_catch_exception(FALSE);

/*#define S_EXCEPTION_NAME "org/omegahat/S/Java/SEvaluationException"*/
#define S_EXCEPTION_NAME NULL

#define ERROR_OR_EXCEPTION ), set_error_buf(S_evaluator), \
 (IS_S_ENVIRONMENT ? Recover(error_buf, NULL_ENTRY, S_evaluator) : \
 (void) throw_Java_exception(error_buf, S_EXCEPTION_NAME, env))



# line 136 "RtoJava.cweb"
jobject userReferenceToJava(JNIEnv *env, USER_OBJECT_ obj, java_type which);

# line 199 "RtoJava.cweb"
USER_OBJECT_
s_from_java_basic(JNIEnv *env, void *java_object, const char *dataType,
                    boolean catch, boolean checkForReference);

# line 411 "RtoJava.cweb"
USER_OBJECT_ asOmegaReferenceUserObject(JNIEnv *env, jobject omegaObject);

# line 479 "RtoJava.cweb"
USER_OBJECT_ createUserReferenceObject(JNIEnv *env, const char *SclassName, jstring value, jstring className);


# line 586 "RtoJava.cweb"
extern boolean s_catch_exception(boolean asError);


# line 706 "RtoJava.cweb"
USER_OBJECT_ createRAnonymousReference(USER_OBJECT_ obj);

# line 735 "RtoJava.cweb"
char *javaReferenceSignature(USER_OBJECT_ obj, int checkArray);

# line 795 "RtoJava.cweb"
int IS(USER_OBJECT_ obj, const char * const name);


# line 132 "RtoJava.cweb"
#endif /* the conditional definition of the header contents */

