# line 61 "RtoJava.cweb"
#include "RtoJava.h"
#include "References.h"
#include "JNIUtils.h"
#include "ErrorHandling.h"
#include "RStoJava.h"
#include "Converters.h"

#include "Reflectance.h"
#include "RReferenceCall.h"

 /* Added for R and S to have a common internal NULL reference. */
#ifndef NULL_USER_OBJECT
#define NULL_USER_OBJECT S_void
#endif

# line 190 "RtoJava.cweb"
static void *local_alloc(size_t n);

# line 632 "RtoJava.cweb"
  static void msg_and_clear_exception(const char *msg);


# line 696 "RtoJava.cweb"
const char  * const BaseOmegahatReferenceClass = "OmegahatReference";
const char  * const NamedOmegahatReferenceClass = "NamedOmegahatReference";
const char  * const AnonymousOmegahatReferenceClass = "AnonymousOmegahatReference";

const char  * const NamedRReferenceClass = "NamedRReference";
const char  * const AnonymousRReferenceClass = "AnonymousRReference";



# line 749 "RtoJava.cweb"
char *
javaReferenceSignature(USER_OBJECT_ obj, int checkArray)
{
 char *sig = NULL;

 if(IS(obj, AnonymousOmegahatReferenceClass)) {
   sig = ANONYMOUS_JAVA_REFERENCE_SIG;   
 } else if(IS(obj, NamedOmegahatReferenceClass)) {
   sig = NAMED_JAVA_REFERENCE_SIG;
 } else if(IS(obj, NamedRReferenceClass)) {
   sig = NAMED_R_REFERENCE_SIG;
 } else if(IS(obj, AnonymousRReferenceClass)) {
   sig = ANONYMOUS_R_REFERENCE_SIG;
 }

 
#if 0 
 else if(checkArray == TRUE) {
      /* Since we are allowed arrays, we check that this object
         is a list and that each element is some sort of reference.
       */
    if(IS_LIST(obj)) {
      int ok = TRUE, i;
      for(i = 0; i < GET_LENGTH(obj); i++) {
        if(IS(RECURSIVE_DATA(obj)[i], BaseOmegahatReferenceClass) == FALSE) {
          ok = FALSE;
          break;
        }
      }
     if(ok == TRUE) {
       /* Glue these two strings together in the pre-processor. */
      sig = "["/**/BASE_JAVA_REFERENCE_SIG;       
     }
    } else
       sig = BASE_JAVA_REFERENCE_SIG;
 }
#endif
 else {
   sig = BASE_JAVA_REFERENCE_SIG;
 }

return(sig);
}



# line 592 "RtoJava.cweb"
/* catch exception, if any; When S is in charge turn an exception into
   an S error.  Otherwise return TRUE if there is an exception.   If
   error==FALSE, clear the exception */
boolean
s_catch_exception(boolean asError)
{
  char *msg; const char *name;
  JNIEnv *env = interface_VM_env();

  msg = (char *) get_Java_exception(NULL, env);
  if(msg == NULL)
      return(FALSE);
  if(IS_S_ENVIRONMENT) {
    /* prepare an S error message before clearing Java exception. */
#if 0      
    name = get_Java_exception_class(env);
    PROBLEM "Java exception(%s): \"%s\"", name, msg 
    END_MESSAGE;
    msg = GET_MESSAGE;
#else
    msg = "Error in Java";
#endif
     
    clear_Java_exception(env);
    if(asError) {
      PROBLEM "%s", msg 
      ERROR
    } else {
      PROBLEM "Clearing %s", msg WARN;
    }
  } else {
    if(!error)
      clear_Java_exception(env);
    return TRUE;
  }

  return(FALSE);
}


# line 637 "RtoJava.cweb"
static void 
msg_and_clear_exception(const char *msg)
{
  if(!msg || !*msg)
    s_catch_exception(TRUE);
  else {
    PROBLEM "%s", msg END_MESSAGE;
    msg = GET_MESSAGE;
    clear_Java_exception(SgetJNIEnv());
    PROBLEM "%s", msg ERROR;
  }
}
    


# line 675 "RtoJava.cweb"
void
s_start_VM(char **classPathEls, char **extraProperties, long *numExtraProperties)
{
  const char *msg;
  msg = create_Java_vm(classPathEls[0], NULL, NULL,
                         extraProperties, *numExtraProperties, 0, 1);

  if(msg) PROBLEM
            "Couldn't start Java Virtual Machine: %s",msg
            ERROR;

   *numExtraProperties = TRUE;

    /* Now make certain the basic converters for the reflect meta objects
       and references are available.
     */
  RS_JAVA(registerDefaultConverters(getJNIEnv()));
}



# line 424 "RtoJava.cweb"
USER_OBJECT_
asOmegaReferenceUserObject(JNIEnv *env, jobject omegaObject)
{
 USER_OBJECT_ ans = NULL_USER_OBJECT;
 void *val;
 char *SclassName = NULL;
 jstring className;

 if(omegaObject == NullObject) {
     fprintf(stderr, "[asOmegaReferenceUserObject] Java object is null\n");fflush(stderr);
   return(ans);
 }

 if(OmegaNamedReferenceClass  == NULL) {
   if(initReferenceClasses(env) < 1) {
     fprintf(stderr, "Reference classes are not initialized\n");fflush(stderr);
      return(ans); /* Big problem! */
   }
 }

 if( VMENV IsInstanceOf(env, omegaObject, OmegaNamedReferenceClass) == JNI_TRUE) {
   SclassName = "NamedOmegahatReference";
 } else  if( VMENV IsInstanceOf(env, omegaObject, OmegaAnonymousReferenceClass) == JNI_TRUE) {
   SclassName = "AnonymousOmegahatReference";
 } else {
      /* If we wanted to report this as an error,
         then we would compute the name of the class of the
	 object in question. However, s_from_java_basic calls this
	 when it is attempting to find a suitable converter. So we would
	 have to add an argument to this routine to indicate whether it
	 is an error or not to get to this point.
       */
#if 0
     jboolean isCopy;
     jstring tmp = get_object_class_name(env, omegaObject);
     char* klassName = VMENV GetStringUTFChars(env, tmp, &isCopy);
     fprintf(stderr, "[asOmegaReferenceUserObject] Java object is not a reference. Class is %s\n", klassName);
     if(isCopy) {
       VMENV ReleaseStringUTFChars(env, tmp, klassName);
     }
#endif     
    return(ans);
 }

     /* Get the key/name of the object in the database. */
   val = VMENV CallObjectMethodA(env, omegaObject, ReferenceNameMethodID, NULL);
   className = VMENV CallObjectMethodA(env, omegaObject, ReferenceClassNameMethodID, NULL);
   ans =  createUserReferenceObject(env, (const char*)SclassName, (jstring) val, className);

 return(ans); 
}


# line 497 "RtoJava.cweb"
USER_OBJECT_
createUserReferenceObject(JNIEnv *env, const char *SclassName, jstring value, jstring realClassName)
{
 USER_OBJECT_ ans = NULL_USER_OBJECT;
 USER_OBJECT_ className, slotNames;

  PROTECT(ans = NEW_LIST(2));

     /* Set the `name' slot, which we happen to know is 
        slot 0!
      */
   RECURSIVE_DATA(ans)[0] = javaToUserString(env, value);

   RECURSIVE_DATA(ans)[1] = javaToUserString(env, realClassName);


  PROTECT(slotNames = allocVector(STRSXP, 2));
  STRING(slotNames)[0] =mkChar("key");
  STRING(slotNames)[1] =mkChar("className"); 
    
  setAttrib(ans, R_NamesSymbol, slotNames);
  UNPROTECT(1); /* slotNames */ 
 
  PROTECT(className = allocVector(STRSXP, 1));
  STRING(className)[0] = mkChar((char*)SclassName);
  setAttrib(ans, R_ClassSymbol, className);

  UNPROTECT(1); /* className */

 UNPROTECT(1);  /* ans */

 return(ans);
}



  
# line 144 "RtoJava.cweb"
jobject
userReferenceToJava(JNIEnv *env, USER_OBJECT_ obj, java_type which)
{
 char *name;
 jobject jobj;

 #if 0
  if(IS(obj, BaseJavaReferenceClass) == FALSE) {
     PROBLEM "Cannot convert object to OmegahatReference"
     ERROR;
  }
#endif
   /* Get the ID part. */
  name = CHAR_DEREF(STRING(RECURSIVE_DATA(obj)[0])[0]);
#if LDEBUG
printf("[userReferenceToJava] %s %d\n", name, which); fflush(stdout);
#endif

  if(which == RS_NAMED_REFERENCE || which == RS_ANONYMOUS_REFERENCE) {
    jclass klass;
    jmethodID mid;
    jstring jname;
       char *className;
        className = "org/omegahat/R/Java/RForeignReference";
       jname = VMENV NewStringUTF(env, name);
       klass = VMENV FindClass(env, className);
       mid = VMENV GetMethodID(env, klass, "<init>", "(Ljava/lang/String;)V");
       jobj = VMENV NewObject(env, klass, mid, jname);
       VMENV DeleteLocalRef(env, jname);    
  } else {
    jobj = createOmegaReference(env, name, NULL, which);
  }

  return(jobj);
}


# line 193 "RtoJava.cweb"
static void *local_alloc(size_t n)
{
  return (void *)Calloc(n, char);
}

# line 207 "RtoJava.cweb"
USER_OBJECT_
s_from_java_basic(JNIEnv *env, void *java_object, const char *dataType, 
                   boolean catch, boolean checkForReference)
{
  jboolean isArray; 
  long i, n = 0; 
  USER_OBJECT_ value;
  jobject obj;
  java_type which;
  const char *msg;
  jboolean builtIn = FALSE;
  jboolean isStringCopied;
  

   obj = (jobject)java_object;
  
   if(dataType == NULL) {
       jclass klass;
#ifdef LDEBUG  
fprintf(stderr,"Getting data type\n");fflush(stderr);
#endif       
      klass = VMENV GetObjectClass(env, obj);
      dataType = getClassName(env, klass, &isStringCopied);
#ifdef LDEBUG  
fprintf(stderr,"Got data type %s\n", dataType);fflush(stderr);
#endif       
   } 

   which = which_java_type((char *)dataType, &isArray, &builtIn);
#ifdef LDEBUG  
fprintf(stderr,"Converting from Java %s\n", dataType);fflush(stderr);
#endif
  
  if(isArray == JNI_TRUE)
    n = VMENV  GetArrayLength(env, obj);
  else 
    n = 1;

#ifdef LDEBUG
fprintf(stderr, "Got Java type (array) %d (length) %d (builtIn) %d (type)%d\n", (int)isArray, (int) n, (int)builtIn, (int) which);
#endif
  
   /* Now, if we are not dealing with a primitive or a reference,
      look to the user handlers.
    */
  if(which == JAVA_INVALID)
     builtIn = JNI_FALSE;

  if(!builtIn && FromJavaConverters) {
     int ok = 0;
       value = userLevelJavaConversion(env, obj, dataType, isArray, n, &ok);
     if(ok) {
       return(value);
     }
  }

  if(checkForReference &&
      (value = asOmegaReferenceUserObject(env, (jobject) java_object)) != NULL_USER_OBJECT) {
     return(value);
  }



#if LDEBUG
printf("[s_from_java_basic]  %d  %d\n",(int) which, (int) n);fflush(stdout);
#endif
  
  value = NULL;

  switch(which) {

    case JAVA_DOUBLE: 
      value = NEW_NUMERIC(n);
      PROTECT(value);
      msg = fromJavaType(env, obj, which, n, (void *)NUMERIC_POINTER(value), isArray, sizeof(double), local_alloc);
      break;

    case JAVA_FLOAT:
      value = NEW_NUMERIC(n);
      PROTECT(value);
      msg = fromJavaType(env, obj, which, n, (void *)NUMERIC_POINTER(value), isArray, sizeof(float), local_alloc);
      break;

    case JAVA_LONG:
    case JAVA_INT:
    case JAVA_SHORT:
       value = NEW_INTEGER(n);
       PROTECT(value);
       msg = fromJavaType(env, obj, which, n, (void *)INTEGER_POINTER(value), isArray, sizeof(long), local_alloc);
      break;

    case JAVA_BOOLEAN:
      {
        jboolean *tmp; 
        int *els;
        value = NEW_LOGICAL(n); 
        PROTECT(value);
        els = LOGICAL_DATA(value);
        /* the CtoJava code always uses char sizes here */
        tmp = Calloc(n, jboolean);
        msg = fromJavaType(env, obj, which, n, (void *)tmp, isArray, sizeof(jboolean), local_alloc);
        for(i=0; i<n; i++)
          els[i] = (tmp[i] == JNI_TRUE) ? TRUE : FALSE;
        break;
      }

    case JAVA_CHAR:
       value = NEW_CHARACTER(1);
       PROTECT(value);
       STRING(value)[0] = COPY_TO_USER_STRING("X");
       msg = fromJavaType(env, obj, which, n, (void *)CHARACTER_DATA(value)[0], isArray, sizeof(char), local_alloc);
       break;

    case JAVA_BYTE:
          /* Create a char * with the length n. */
    /*
XXXXXXXXXXX
      value = mkChar(n);
      msg = fromJavaType(env, obj, which, n, (void *)CHAR_DEREF(value), isArray, sizeof(char), local_alloc);
    */
      break;

    case JAVA_STRING:
     {
      const char *inString;
      jstring inEl;
      jboolean isCopy;
     msg = NULL;
/*      msg = fromJavaType(env, obj, which, n, (void *)CHARACTER_DATA(value), isArray, sizeof(char *), local_alloc);*/
       if(isArray) {
         int i;    
         value = NEW_CHARACTER(n); 
         PROTECT(value);                    
          for(i = 0; i < n ; i++) {
              inEl = VMENV GetObjectArrayElement(env, obj, i);
              if(inEl != NULL_JAVA_OBJECT) {
                inString = VMENV GetStringUTFChars(env, inEl, &isCopy);
                CHARACTER_DATA(value)[i] = COPY_TO_USER_STRING(inString);
                if(isCopy == JNI_TRUE)
                   VMENV ReleaseStringUTFChars(env, inEl, inString);
              }
          }
       } else {
          PROTECT(value = javaToUserString(env, obj));
       }

      }
      break;

    case JAVA_OBJECT: {
        USER_OBJECT_ *els;
        jstring Jtypei;
        jobject eli;
        const char *typei;
        jboolean isCopy;
        value = NEW_LIST(n); 
        PROTECT(value);
        els = RECURSIVE_DATA(value);
        msg = NULL;
        for(i=0; i<n; i++) {
          eli = (*env)-> GetObjectArrayElement(env, obj, i);
          Jtypei = get_object_class_name(env, eli);
          msg = (char *) get_Java_exception(NULL, env);
          if(msg != NULL) break;
          typei = (*env)-> GetStringUTFChars(env, Jtypei, &isCopy);
          els[i] = s_from_java_basic(env, eli, typei, catch, TRUE);
          if(isCopy == JNI_TRUE)
            (*env)-> ReleaseStringUTFChars(env, Jtypei, typei);
          (*env)-> DeleteLocalRef(env, eli);
        }
    } /* end of JAVA_OBJECT case. */

    default:
      fprintf(stderr, "Other cases %d\n",which);fflush(stderr);
      value = anonymousAssign(env, obj, NULL_JAVA_OBJECT);
  }


  if(!catch || (value && !msg)) {
      UNPROTECT(1);
      return value;

  } else if(!value) {
       PROBLEM "Unable to convert Java datatype \"%s\" to basic S type",
                 dataType 
       ERROR;
  } else {
      PROBLEM "Problem in converting Java datatype \"%s\": %s",
                  dataType, msg 
      ERROR;
  }
 return NULL_USER_OBJECT;
}


JNIEXPORT jobject JNICALL Java_org_omegahat_interfaceManager_S_Test_foo
                   (JNIEnv *env, jobject obj,  jobject arg) 
{
  return get_object_class_name(env, arg);
}



# line 799 "RtoJava.cweb"
#if 0
#define IS(a,b)  TRUE
#endif

#include "Rinternals.h"

int IS(USER_OBJECT_ obj, const char * const name)
{
 int i, n;
 USER_OBJECT_ className;
 
 className = getAttrib(obj, R_ClassSymbol);
 n = GET_LENGTH(className);

  for(i=0;i<n;i++) {
    if(strcmp(name, CHAR_DEREF(STRING(className)[i])) == 0) {
#if LDEBUG      
printf("Is a %s\n", name);
PrintValue(obj);
#endif
       return(TRUE);
     }
  }
 return(FALSE); 
}  



# line 712 "RtoJava.cweb"
USER_OBJECT_
createRAnonymousReference(USER_OBJECT_ obj)
{
  USER_OBJECT_ expr, value;
  USER_OBJECT_ handler = RJava(defaultHandlerFunction)();
  USER_OBJECT_ createFun;

       /* The create reference method had better be the first element in the list! */
     createFun = LIST_POINTER(handler)[1];
     PROTECT(expr = allocVector(LANGSXP, 2));

       CAR(expr) = createFun;
       CAR(CDR(expr)) = obj;

       value = eval(expr, R_GlobalEnv);

     UNPROTECT(1);
  
 return(value);
}    


# line 828 "RtoJava.cweb"
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


