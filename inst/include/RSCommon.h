
#ifndef RSCOMMON_H
#define RSCOMMON_H

#ifdef __cplusplus 
extern "C" {
#endif

  #if defined(_S4_)
  #define vector s_object
  #endif

  #if defined(_S_)
  /* || defined(_R_) */

  #include "S.h"

  #endif


  #if defined(_R_)
  #include <Rinternals.h>
  #include <Rdefines.h>
  #ifdef length
  #undef length
  #endif
  #ifdef GET_LENGTH
  #undef GET_LENGTH
  #define GET_LENGTH(x) Rf_length(x)
  #endif

  #ifdef append
  #undef append
  #endif

  #endif


  
  #if defined(_S4_) /* redefine vector and declare routines with  S_evaluator */

  #ifdef vector
  #undef vector
  #endif

  #define COPY_TO_USER_STRING(a) c_s_cpy(a, S_evaluator)
  #define LOCAL_EVALUATOR  S_EVALUATOR
  #define CREATE_FUNCTION_CALL(name, argList) alcf(name, argList, S_evaluator)

  #define CREATE_STRING_VECTOR(a) STRING_VECTOR(a, S_evaluator)

  #define NULL_USER_OBJECT S_void
  /* This is to keep R happy until it moves to char ** rather than
     SEXP * for character vectors.
  */
  #define CHAR_DEREF(x)   (x)


  #ifdef PROTECT
  #undef PROTECT
  #endif

  #define PROTECT(x)   (x) /**/
  #define UNPROTECT(x) /**/

   /* Note that this will override the one in S.h which is for S4, not S3, style classes. */
  #define SET_CLASS(obj,classname)  set_attr((obj), "class", (classname), S_evaluator);


  #endif /* end of this S4 */


  #if defined(_R_)
  #define CHAR_DEREF(x)   CHAR((x))

   /* SET_CLASS and SET_NAMES have been moved to Rdefines.h in the R distribution.*/
  #endif  /* of defined(_R_) */



  #if defined(_Octave_)

  #include <octave/oct.h>
  extern char error_buf[];
  #define PROBLEM sprintf(error_buf,
  #define ERROR  ); error(error_buf)

  #define STRING_VALUE(a)  a.all_strings()[0].c_str()

  #define GET_LENGTH(a)  getLength(a)

  #define LOCAL_EVALUATOR /**/
  #define COPY_TO_USER_STRING(a) strdup(a) /*XXX*/

  #endif /* end of defined(_Octave_)*/



#ifdef __cplusplus 
}
#endif

#endif /* end of RSCOMMON_H*/


