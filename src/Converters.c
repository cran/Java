# line 1000 "Converters.cweb"
#include "Converters.h"

#include "Rdefines.h"
#include "RtoJava.h"

#include "JNIUtils.h"


#include "Reflectance.h"
#include "CtoJava.h" /* getInterfaceManager()  */
#include "ErrorHandling.h"


#include "RFunctionListener.h"

# line 55 "Converters.cweb"
RSFromJavaConverter *FromJavaConverters = NULL;
RSToJavaConverter   *ToJavaConverters = NULL;

# line 747 "Converters.cweb"
/*
   Method id for assignig a value to the anonymous database, used in
   anonymousAssign.   
 */   
jmethodID ManagerAssignMethodID = NULL;

# line 183 "Converters.cweb"
USER_OBJECT_
userLevelJavaConversion(JNIEnv *env, jobject obj, const char *dataType, jboolean isArray, int arrayLength, int *ok)
{
 RSFromJavaConverter *tmp = FromJavaConverters;
 jclass klass;
 int match;
 jobject tmpObject;
 USER_OBJECT_ value;
#ifdef LDEBUG 
 int ctr = 0;
#endif

 if(isArray && arrayLength > 0) {
   tmpObject = VMENV GetObjectArrayElement(env, obj, 0);
 } else
    tmpObject = obj;

 klass =  VMENV GetObjectClass(env, tmpObject);

 while(tmp) {
#ifdef LDEBUG
fprintf(stderr, "[userLevelJavaConversion] %s %d\n",dataType, (int)arrayLength);
#endif
    match = tmp->match(obj, klass, env, tmp);

#ifdef LDEBUG    
fprintf(stderr, "Converter match #%d %d\n", ctr++, match);fflush(stderr);
#endif
    
    if(match) {
      *ok  = 1;
      if(isArray) {
       int i;
       PROTECT(value = NEW_LIST(arrayLength));
       for(i = 0; i < arrayLength ; i++) {
         tmpObject = VMENV GetObjectArrayElement(env, obj, i);
         SET_VECTOR_ELT(value, i, tmp->converter(tmpObject, klass, env, tmp));
       }
       UNPROTECT(1);
      } else {
        value = tmp->converter(obj, klass, env, tmp);
      }
      return(value);
    }

     tmp = tmp->next;
 }

 *ok = 0;
 return(NULL);
}

# line 239 "Converters.cweb"
int
addFromJavaConverter(RSFromJavaConverter *el)
{
 int ctr = 0;
  el->next = NULL;
  if(FromJavaConverters == NULL) {
    FromJavaConverters = el;
  } else { 
    RSFromJavaConverter *tmp;
    tmp = FromJavaConverters;
    while(tmp->next) {
     ctr++;
     tmp = tmp->next;
    }

    tmp->next = el;
    ctr++;
  }

 return(ctr);
}

# line 514 "Converters.cweb"
RSFromJavaConverter *
addFromJavaConverterInfo(FromJavaConverterMatch match, FromJavaConverter converter, jboolean autoArray, void *userData, char *description, int *index)
{
 int which;
 RSFromJavaConverter *cvt = (RSFromJavaConverter*) malloc(sizeof(RSFromJavaConverter)); 
  cvt->match = match;
  cvt->converter = converter;
  cvt->autoArray = autoArray;
  cvt->userData = userData;
  cvt->next = NULL;
  cvt->description = description;

  which = addFromJavaConverter(cvt);
  if(index)
     *index = which;

 return(cvt);
}


# line 416 "Converters.cweb"
/*
  Tests whether the class of the object, pre-computed and supplied via the type
  argument, is exactly the same as the class stored in the converter's user data.
 */
jboolean 
SimpleExactClassMatch(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter)
{
 jclass expected = (jclass) converter->userData;
 jboolean ans;
#ifdef LDEBUG
 jboolean isCopy; 
 fprintf(stderr, "Matching %s and %s\n", getClassName(env, type,&isCopy), getClassName(env, expected, &isCopy));fflush(stderr);
#endif
 
 ans = VMENV IsSameObject(env, type, expected) == JNI_TRUE;
 
 return(ans);
}


/*
  Tests whether the specified object is an instance of the class that was stored as the user data
  in the converter instance. This works in the same way as instanceof
  in Java.
 */
jboolean
InstanceOfClassMatch(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter)
{
 jclass expected = (jclass) converter->userData;
/* fprintf(stderr, "Comparing instanceof\n");fflush(stderr); */
 return(VMENV IsInstanceOf(env, obj, expected));
}

/*
  Tests whether the class of the object, pre-computed and supplied via the type
  argument, is compatible with the class that was stored as the user data
  in the converter instance.
 */
jboolean
AssignableFromClassMatch(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter)
{
 jclass expected = (jclass) converter->userData;
 return(VMENV IsAssignableFrom(env, type, expected));

}
# line 472 "Converters.cweb"
void
RS_JAVA(registerDefaultConverters)(JNIEnv *env)
{
 jclass klass;

   /* Keep this one first as it is likely to be used most often. */
 addFromJavaConverterInfo(RS_JAVA(ReferenceClassMatch), RS_JAVA(ReferenceConverter), JNI_TRUE, NULL, "Converts any Java InterfaceReference",NULL);
 
  klass = VMENV FindClass(env, "java/lang/reflect/Method");
  klass = VMENV NewGlobalRef(env, klass);
 
 addFromJavaConverterInfo(&SimpleExactClassMatch, RS_JAVA(MethodConverter), JNI_TRUE, klass, NULL, NULL);
 /*java/lang/reflect/Method");*/

  klass = VMENV FindClass(env, "java/lang/reflect/Constructor");
  klass = VMENV NewGlobalRef(env, klass); 

 addFromJavaConverterInfo(SimpleExactClassMatch, RS_JAVA(MethodConverter), JNI_TRUE, klass, NULL, NULL);


  klass = VMENV FindClass(env, "java/util/Properties");
  klass = VMENV NewGlobalRef(env, klass);  
  addFromJavaConverterInfo(InstanceOfClassMatch, RS_JAVA(PropertyConverter), JNI_FALSE, klass, NULL, NULL); 

 
   /* Now we move on to the conversion from R to Java. */
 
 addToJavaConverterInfo(SimpleRTargetMatch, RFunctionConverter, JNI_TRUE, "org/omegahat/R/Java/RFunctionListener", "RFunctionListener");
  
}

# line 586 "Converters.cweb"
int
getNumConverters(int which)
{
 int ctr = 0;
 if(which == 0) {

      RSFromJavaConverter *tmp = FromJavaConverters;
      while(tmp) {
        ctr++;
        tmp = tmp->next;
      }
 } else {
      RSToJavaConverter *tmp = ToJavaConverters;
      while(tmp) {
        ctr++;
        tmp = tmp->next;
      }
 }

 return(ctr);
}

# line 729 "Converters.cweb"
USER_OBJECT_
anonymousAssign(JNIEnv *env, jobject obj, jclass klass)
{
 jobject ref;
 USER_OBJECT_ ans;

  if(ManagerAssignMethodID == NULL) {
    initAssignMethod(env);
  }
 
  ref = VMENV CallObjectMethod(env, getInterfaceManager(), ManagerAssignMethodID, NULL_JAVA_OBJECT, obj, JNI_FALSE);
  ans = asOmegaReferenceUserObject(env, ref);

 return(ans);
}

# line 759 "Converters.cweb"
const char *
initAssignMethod(JNIEnv *env)
{
 jclass klass = VMENV FindClass(env, "org/omegahat/Interfaces/NativeInterface/OmegaInterfaceManager");
 MUST_GET_METHOD_ID(ManagerAssignMethodID, klass, NULL, "assignResult", "(Ljava/lang/String;Ljava/lang/Object;Z)Ljava/lang/Object;", env);

 return(NULL);
}    



# line 781 "Converters.cweb"
jobject
userLevelToJavaConversion(JNIEnv *env, USER_OBJECT_ obj, jclass targetClass, java_type which, char *sig, int arrayLen, jboolean isArray, int *done)
{
 
 jboolean match;
 jobject jobj;
 RSToJavaConverter *tmp = ToJavaConverters;
 
 int ctr = 0;
    while(tmp) {
        match = tmp->match(obj, targetClass, which, sig, isArray, arrayLen, env, tmp);
        if(match) {
fprintf(stderr, "Match for conversion to Java %d\n", ctr);fflush(stderr);
         *done = 1;
          jobj = tmp->converter(obj, targetClass, which, sig, isArray, arrayLen, env, tmp);
          return(jobj);
        }

     tmp = tmp->next;
     ctr++;
    } 


  return(NULL_JAVA_OBJECT);
}




# line 539 "Converters.cweb"
RSToJavaConverter *
addToJavaConverterInfo(ToJavaConverterMatch match, ToJavaConverter converter, jboolean autoArray, void *userData, char *description)
{
 RSToJavaConverter *cvt = (RSToJavaConverter*) malloc(sizeof(RSToJavaConverter)); 
  cvt->match = match;
  cvt->converter = converter;
  cvt->autoArray = autoArray;
  cvt->userData = userData;
  cvt->description = description;
  cvt->next = NULL;

  addToJavaConverter(cvt);

 return(cvt);
}

int
addToJavaConverter(RSToJavaConverter *cvt)
{
  int ctr = 0;
  RSToJavaConverter *tmp = ToJavaConverters;

  cvt->next = NULL;
  if(tmp == NULL) {
     ToJavaConverters = cvt;
  } else {
    while(tmp->next) {
       ctr++;
       tmp = tmp->next;
    }
    tmp->next = cvt;
  }      
  
  return(ctr);
}

# line 821 "Converters.cweb"
jboolean SimpleRTargetMatch(USER_OBJECT_ obj, jclass targetClass, java_type type, char *signature, jboolean isArray, int arrayLen, JNIEnv *env,  RSToJavaConverter *This)
{
#ifdef LDEBUG    
  fprintf(stderr, "%s == %s\n", signature, (char*)This->userData);fflush(stderr);
#endif
  
   /* Need to handle arrays, etc.
      by stripping off the [ and also the L<name>;
    */
  return(strcmp(signature, (char*)This->userData)==0 ? JNI_TRUE : JNI_FALSE);
}



# line 614 "Converters.cweb"
USER_OBJECT_
RS_JAVA(getNumConverters)(USER_OBJECT_ which)
{
 USER_OBJECT_ ans;
  ans = NEW_INTEGER(1);
 
  INTEGER_DATA(ans)[0] = getNumConverters(LOGICAL_DATA(which)[0]);
 return(ans);
}

# line 628 "Converters.cweb"
USER_OBJECT_
RS_JAVA(getConverterDescriptions)(USER_OBJECT_ which)
{
  int type = LOGICAL_DATA(which)[0];
  int n, i;
  const char *d;
  USER_OBJECT_ ans;
  RSFromJavaConverter *tmp0 = FromJavaConverters;
  RSToJavaConverter *tmp1 = ToJavaConverters;
  
  n = getNumConverters(type);

  PROTECT(ans = NEW_CHARACTER(n));
   for(i = 0; i < n ; i++) {
    if(type == 0) {
      d = tmp0->description;
      if(!d) {
         d = fromJavaConverterDescription(tmp0);
      }
      tmp0 = tmp0->next;      
    } else {
      d = tmp1->description;
      tmp1 = tmp1->next;
    }
    
    if(d)    
     SET_STRING_ELT(ans, i, COPY_TO_USER_STRING(d));
   }

  UNPROTECT(1);

 return(ans); 
}    

# line 669 "Converters.cweb"
char *
fromJavaConverterDescription(RSFromJavaConverter *cvt)
{
  char *ans = NULL;
  char *compare = NULL;

  if(cvt->description)
    return(cvt->description);

  if(cvt->match == SimpleExactClassMatch) {
    compare = "class == ";    
  } else if(cvt->match == InstanceOfClassMatch) {
    compare = "instanceof ";
  } else if(cvt->match == AssignableFromClassMatch) {
    compare = "assignable from ";
  }

  if(compare) {
     const char *tmp;
     jboolean isCopy;
     jstring jstr;
     JNIEnv *env = getJNIEnv();
     jclass klass = (jclass) cvt->userData;
     jstr = get_java_class_name(env, klass);
     tmp = VMENV GetStringUTFChars(env, jstr, &isCopy);

       /* Note that this will get freed automatically by R/S. */
     ans = (char *)S_alloc(strlen(tmp), sizeof(char));
     sprintf(ans, "%s%s", compare, tmp);
     
     if(isCopy == JNI_TRUE) {
          VMENV ReleaseStringUTFChars(env, jstr, tmp);
     }
  }

  return(ans);
}    



# line 878 "Converters.cweb"
USER_OBJECT_ 
RS_JAVA(PropertyConverter)(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter)
{
  jmethodID sizeID, nextID, namesID, getPropertyID;
  jboolean isCopy;
  jstring jkey, jval;
  jclass klass;
  jobject enumeration;
  const char *tmpKey;
  USER_OBJECT_ ans, names;
  int n, i;

  sizeID = VMENV GetMethodID(env, type, "size", "()I");  
  namesID = VMENV GetMethodID(env, type, "propertyNames", "()Ljava/util/Enumeration;");
  klass = VMENV FindClass(env, "java/util/Enumeration");
  nextID = VMENV GetMethodID(env, klass, "nextElement", "()Ljava/lang/Object;");
  getPropertyID = VMENV GetMethodID(env, type, "getProperty", "(Ljava/lang/String;)Ljava/lang/String;");  

  n = VMENV CallIntMethod(env, obj, sizeID);
  PROTECT(ans = NEW_CHARACTER(n));
  PROTECT(names = NEW_CHARACTER(n));

  enumeration = VMENV CallObjectMethod(env, obj, namesID);
  
  for(i = 0; i < n; i++) {
     jkey = (jstring) VMENV CallObjectMethod(env, enumeration, nextID);
     tmpKey = VMENV GetStringUTFChars(env, jkey, &isCopy);
     SET_STRING_ELT(names, i, COPY_TO_USER_STRING(tmpKey));
     if(isCopy) {
       VMENV ReleaseStringUTFChars(env, jkey, tmpKey);
     }
     jval = VMENV CallObjectMethod(env, obj, getPropertyID, jkey);
     tmpKey = VMENV GetStringUTFChars(env, jval, &isCopy);
     SET_STRING_ELT(ans, i, COPY_TO_USER_STRING(tmpKey));
     if(isCopy) {
       VMENV ReleaseStringUTFChars(env, jval, tmpKey);
     }     
   }
  
   SET_NAMES(ans, names);

  UNPROTECT(2);

 return(ans); 
}    


# line 857 "Converters.cweb"
jboolean 
RS_JAVA(ReferenceClassMatch)(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter)
{
 jboolean ans = VMENV IsAssignableFrom(env, type, VMENV FindClass(env, "org/omegahat/Interfaces/NativeInterface/InterfaceReference"));

#ifdef LDEBUG 
  fprintf(stderr, "Matched a reference %d\n",(int)ans);fflush(stderr);
#endif
 
 return(ans);
}

# line 840 "Converters.cweb"
USER_OBJECT_ 
RS_JAVA(ReferenceConverter)(jobject obj, jclass type, JNIEnv *env, RSFromJavaConverter *converter)
{
   return(asOmegaReferenceUserObject(env, obj));
}


# line 926 "Converters.cweb"
typedef void * (*DL_FUNC)();
extern DL_FUNC R_FindSymbol(const char *, const char *);

USER_OBJECT_
RS_JAVA(registerConverter)(USER_OBJECT_ shandler, USER_OBJECT_ smatcher, USER_OBJECT_ sautoArray, USER_OBJECT_ sdescription, USER_OBJECT_ suserData)
{
  JNIEnv *env = getJNIEnv();
  int autoArray = LOGICAL_DATA(sautoArray)[0];
  char *description;
  USER_OBJECT_ ans;
  FromJavaConverterMatch match;
  FromJavaConverter      handler;
  void *userData = (void*) suserData;
  int index;
  
  if(IS_CHARACTER(shandler)) {
    char *tmp1 = strdup(CHAR_DEREF(STRING_ELT(shandler, 0)));
    DL_FUNC m1 =  R_FindSymbol(tmp1, "");
      if(m1 == NULL) {
           /* Can't free tmp here! */
         PROBLEM "No symbol found named %s",
                         tmp1
         ERROR;
      }

     handler = (FromJavaConverter) m1;
  } else {

  }
  
  if(IS_CHARACTER(smatcher)) {
    char *tmp = strdup(CHAR_DEREF(STRING_ELT(smatcher, 0)));
    DL_FUNC m =  R_FindSymbol(tmp, "");
      if(m == NULL) {
           /* Can't free tmp here! */
         PROBLEM "No symbol found named %s",
                         tmp
         ERROR;
      }
     match = (FromJavaConverterMatch) m;
  } else {
    jclass klass;
    switch(INTEGER_DATA(smatcher)[0]) {
     case InstanceOf:
        match = InstanceOfClassMatch;
      break;
     case Equals:
        match = SimpleExactClassMatch;
      break;
     case AssignableFrom:
       match = AssignableFromClassMatch;
      break;
     default:
       PROBLEM "Unrecognized converter match function specifier: %d",
                (int) INTEGER_DATA(smatcher)[0]
       ERROR;
    }
     klass = VMENV FindClass(env, CHAR_DEREF(STRING_ELT(suserData, 0)));
     userData =  (void *) klass;  
  }

  description = strdup(CHAR_DEREF(STRING_ELT(sdescription, 0)));

  
  addFromJavaConverterInfo(match, handler, autoArray, userData, description, &index);
  PROTECT(ans = NEW_INTEGER(1));
   INTEGER_DATA(ans)[0] = index+1;
  UNPROTECT(1);
  return(ans);
}  

    

# line 268 "Converters.cweb"
RSFromJavaConverter *
removeFromJavaConverterByIndex(int which)
{
 int ctr = 0;
 RSFromJavaConverter *tmp, *prev;
   tmp = FromJavaConverters;

   if(which == 0) {
       tmp = FromJavaConverters;
       FromJavaConverters = FromJavaConverters->next;
       return(tmp);
   }
 
   while(tmp != NULL && ctr++ < which) {
      prev = tmp;
      tmp = tmp->next;
   }

   if(tmp == NULL)
      return((RSFromJavaConverter*) NULL);

     /* Now drop this element. */
   prev->next = tmp->next;

 return(tmp);
}


RSFromJavaConverter *
removeFromJavaConverterByDescription(char *desc, int *which)
{
 int ctr = 0;
 RSFromJavaConverter *tmp, *prev = NULL;
 char *tmpDesc; 
   tmp = FromJavaConverters;

   while(tmp != NULL) {
      tmpDesc = fromJavaConverterDescription(tmp);
 fprintf(stderr,"Checking element %s\n", tmpDesc);fflush(stderr);         
      if(tmpDesc != NULL && strcmp(tmpDesc, desc) ==0) {
             /* Now drop this element. */
        if(prev)
          prev->next = tmp->next;
        else
          FromJavaConverters = tmp->next;

        if(which)
          *which = ctr;
        return(tmp);
      }
      prev = tmp;
      tmp = tmp->next;
      ctr++;
   }

    return((RSFromJavaConverter*) NULL);
}


# line 349 "Converters.cweb"
USER_OBJECT_
RS_JAVA(removeConverter)(USER_OBJECT_ id, USER_OBJECT_ fromJava, USER_OBJECT_ useDescription)
{
  USER_OBJECT_ ans;
  int ok = -1;
  int which = -1;
  char *desc = NULL;

   if(LOGICAL_DATA(useDescription)[0]) {
     desc = CHAR_DEREF(STRING_ELT(id, 0));       
   } else
     which = INTEGER_DATA(id)[0];
  
  PROTECT(ans = NEW_INTEGER(1));  
  if(LOGICAL_DATA(fromJava)[0] != 0) {
     RSFromJavaConverter *el;
      if(desc != NULL) {
        el = removeFromJavaConverterByDescription(desc, &ok);
      } else
        el = removeFromJavaConverterByIndex(which);

     if(el != NULL) {
       USER_OBJECT_ names;
       char *tmp;
         PROTECT(names = NEW_CHARACTER(1));
           tmp = fromJavaConverterDescription(el);
           if(tmp != NULL)
             SET_STRING_ELT(names, 0, COPY_TO_USER_STRING(tmp));
           SET_NAMES(ans, names);
         UNPROTECT(1);
       free(el);
       if(desc == NULL)
         ok = which;       
     }
  } else {
     PROBLEM "Removing a from R to Java converter not supported yet!"
     ERROR;
  }

  INTEGER_DATA(ans)[0] = ok;
  UNPROTECT(1);
  
  return(ans);
}    


