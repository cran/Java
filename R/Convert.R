getNumJavaConverters <-
#
# Returns the number of converters
# registered internally in a C list/table
# for performing the conversion between R and Java. 
#   F represents from Java to R,
#   T represents from Java to R.
# This is vectorized in the type or direction of converter.

# By default, it returns both.

function(which = c(fromJava=F, toJava=T))
{
 ans <- integer(0)
 for(i in which)
  ans <- c(ans, .Call("RS_JAVA_getNumConverters",as.logical(i))) 

 names(ans) <- names(which)

 ans
}


getJavaConverterDescriptions <-
function(which = c(fromJava=F, toJava=T))
{
 ans <- vector("list", length(which))
 for(i in 1:length(which)) {
  ans[[i]] <- .Call("RS_JAVA_getConverterDescriptions", as.logical(which[i]))
 }

 names(ans) <- names(which)

 ans
}



setJavaConvertible <-

# klass should be a reference to a class or a character string
# identifying a class by name which the interface manager will resolve.
# ok is a logical indicating whether objects of that class are to be
# considered convertible (True) or not (False)

# The matching argument specifies how to compare classes with the one
# being registered. By default, an object of a class C is only considered
# convertible when compared with klass if k and C are the same class.
# Other values (1 and 2 respectively) allow the object being converted
# to be 1) an instanceof klass, allowing for sub-classes and comparson of interfaces
# and  2) for the class `klass' to be assignable from the class C.
# See the Javadoc for ConvertibleClassifier.
#
function(klass, ok = T, matching=0)
{
 if(!missing(matching) & is.character(matching)) {
   tmp <- pmatch(matching, .javaMatchFunctions())
    if(is.na(tmp))
     stop("Unrecognized match type")

    matching <- .javaMatchFunctions[tmp]
 }

  .Java(NULL, "setConvertible", klass, as.integer(matching), ok)
}

.javaMatchFunctions <- c(InstanceOf=1,Equals=2,AssignableFrom=3)

setJavaConverter <-
#
# registers the specified object as a converter
# If handler is a character vector (of length 1 or 2) this
# is treated as the name of a C routine.
# Otherwise, if it is a function or list (expected to be a closure) 
# it is assumed that this can convert the object to R.
function(handler, matcher = -1,  autoArray = T, description=NULL, userObject=NULL, register=T)
{
 autoArray <- as.logical(autoArray)
 description <- as.character(description)
 val <- list()
 class(val) <- "FromJavaConverter"
 if(!missing(matcher)) {
   if(is.character(matcher)) {
     if(!is.loaded(matcher)) {
       x <- pmatch(matcher, names(.javaMatchFunctions))
       if(is.na(x)) {
           stop(paste("There is no such native symbol",matcher))
       }
       matcher <- x
       if(!missing(userObject)) {
         userObject <- .Java(NULL, "expandClassName", as.character(userObject)[1], T)
         val$class <- userObject
       }
       val$match <- .javaMatchFunctions[as.integer(x)]
     }
   } else {
     matcher <- .javaMatchFunctions[as.integer(matcher)]
   }
 }

 if(is.character(handler)) {
  if(!is.loaded(handler))
    stop(paste("No such C symbol available ",handler))
 } else if(!is.function(handler))
     stop("handler must be a function or a string identifying an internal routine")

 val$index <- .Call(.RSJava.symbol("registerConverter"), handler, matcher, autoArray, description, userObject)

  val$description <- description
  class(val) <- "JavaConverter"

  if(register) {
    setJavaConvertible(val$class, matching=val$match)
  }

 return(val)
}


removeJavaConverter <- 
# removes a converter from the appropriate internal list
# identified by the fromJava argument.
#
# Which element is to be removed is specified by
# id which is either an integer (using 1-based counting)
# or a string giving the description of the converter.
# See getJavaConverterDescriptions().
function(id, fromJava =T)
{
 if(is.integer(id) | is.numeric(id))
   id <- as.integer(as.integer(id)-1)
 else if(!is.character(id))
   UseMethod("removeJavaConverter")


 .Call(.RSJava.symbol("removeConverter"), id, as.logical(fromJava), is.character(id))
}

removeJavaConverter.FromJavaConverter <-
function(id, fromJava=T)
{
 removeJavaConverter(id$description)
}

setJavaFunctionConverter <-
#
# Registers an object converter and matcher
# that are implemented as R functions rather
# than C routines.
function(handler, match, description=NULL, fromJava=T)
{
 if(!fromJava)
   stop("No support for function converters from R to Java yet.")

  funs <- list(handler, match)
  index <- .Call("RS_JAVA_setFunctionConverter", funs, as.character(description), as.logical(fromJava))
  val <- list(handler=handler, match=match,index=index,description=description)
  class(val) <- c("FromJavaFunctionConverter", "FromJavaConverter")

  val
}
