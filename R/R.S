.JavaSigs <-
  ## a named object to save users from remembering and typing the dreadful
  ## Java signature strings for some common classes
  list(
        "byte" = "B", "char" = "C", "double" = "D", "float" = "F", "int" = "I",
        "long" = "J", "short" = "S", "void" = "V", "boolean" = "Z",
        "String" = "Ljava/lang/String;")

javaSig <-
  ## expand a short form for a non-primitive class.
  function(name)
  {
   if(!is.character(name))
    name <- mode(name)

    try <- .JavaSigs[[name]]
    if(is.null(try))
      paste("Ljava/lang/", name, ";", sep="")
    else try
  }


RtoJavaSig <-
#
# Attempts to find the
#
function(obj)
{
 k <- class(obj)
 if(!is.null(k)) {
  name <- switch(k, k)
 } else {
   if(is.integer(obj)) 
    name <- "long"
   else
     name <- switch(mode(obj),
                  "numeric"="double",
                  "integer"="long",
                  "logical"="boolean",
                  "character"="String",
                  mode(obj)
                )
 }

  javaSig(name)
}

#
#  Copyright (c) 1998, 1999 The Omega Project for Statistical Computing.
#       All rights reserved.#



mergePath <-
## utility function to merge the two character vectors
  function(path, default, collapse=NULL) {
    if(length(path)>0)
      ## include only the new classes
      path <- path[is.na(match(path, default))]
    value <- c(path, default)
    if(length(collapse)>0) value <- paste(value, collapse=collapse)
    value
  }
      
mergeProperties <-
  ## utility function to merge the two named objects, with elements single character strings,
  ## and convert the result to a vector of Java property settings.
  function(props, default, convert = T)
  {
    if(length(props))
      default[names(props)] <- unclass(props)
    if(convert) { 
      if(length(default) > 0)
        paste("-D", names(default), "=",unlist(default), sep="")
      else
        return("")
    }
    else default
  }
#
#  Copyright (c) 1998, 1999 The Omega Project for Statistical Computing.
#       All rights reserved.#

.Java <-
#
# .qualifier:  an identifier for the object on which the method
#              should be invoked.
# 
# .methodName: the name of the method to invoke.
#
# ...:    arguments to the method. These are converted 
#         using the default mechanism, according to the .sigs
#         or alternatively can be deduced with a (somewhat expensive)
#         call to the Java manager to get the information about the 
#         methods.
#         Each of the elements in this argument list can be named.
#         If so, these names are used to store the converted
#         object in the Java database for use in future .Java calls 
#         within this session.
#
# .name: the name to use for the value returned from the method invocation.
#        If this is missing, an anonymous reference is returned (if
#        the method is not declared as void).
#
# .sigs:  a character vector which identifies the Java classes
#         to which the S argument objects should be converted.
#         
#
function(.qualifier, .methodName, ..., .name=NULL, .sigs="", .convert=T)
{
 isJavaInitialized("Java virtual machine has not been initialized. Call .JavaInit()")

 args <- list(...)
 argNames <- names(args)
 .methodName <- as.character(.methodName)

 .Call("RS_genericJavaCall", .qualifier, 
                             as.character(.methodName),
                             args, argNames,
                             as.character(.name),
                             as.character(.sigs),
                             as.logical(.convert))
}
.OmegahatExpression <-
function(expr, ..., .name=NULL, .sigs="", .convert=T)
{
 args <- list(...)
 argNames <- names(args)

 .Call("RS_OmegahatExpression", expr, args, argNames, 
           as.character(.name), as.character(.sigs), as.logical(.convert))
}

.JavaConstructor <-
#
# Creates a Java object of the specified class name
# by invoking that class' constructor method 
# identified by the arguments in ....
# If the arguments are ambiguous, the .sigs
# is a character vector of the same length as the arguments
# used  to specify the Java classes to which each argument
# should be converted.
# Finally, the new object is assigned to the database
# using the value of the argument .name.
# If this is missing, the object is stored in the anonymous
# database.
#
# The return value is a reference to the newly created Java object.
# It is either a JavaNamedObject or a JavaTempObject.


#
# Note that this could be implemented as a call to .Java
# rather than having C code. Simply call
# .Java("__Evaluator", "callConstructor", name, args, argNames, .name,)
#
#
#
function(className, ..., .name="", .sigs="", .convert=T) 
{
 isJavaInitialized("Java virtual machine has not been initialized. Call .JavaInit()")

 clss <- as.character(className)
 args <- list(...)
 argNames <- names(args)
 .Call("RS_ConstructorJavaCall", className, args, argNames,
                               as.character(.name), as.character(.sigs), as.logical(.convert))
}


.JNew <- .Alias(.JavaConstructor)

.JClass <-
function(x, name=NULL)
{
 if(is.character(x))
   .Java("__Evaluator", "findClass", as.character(x), .name = name)
 else
   UseMethod(".JClass")
}


.JClass.AnonymousOmegahatReference <-
function(x, name=NULL)
{
  .JClass(x[["className"]], name = name)
}

.JClass.NamedOmegahatReference <-
function(x, name = NULL)
{
  .JClass(x[["className"]], name = name)
}

#
#  Copyright (c) 1998, 1999 The Omega Project for Statistical Computing.
#       All rights reserved.#
