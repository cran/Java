javaConfig <-
  ## generator for the `javaConfig' class, objects used to define the
  ## initialization of the Java Virtual Machine.
  ##
  ## The `classPath' is  vector of strings, each defining a directory
  ## or Jar file to be included in the initial Java classpath.
  ##
  ## The `properties' is a class `named' object defining the
  ## properties; each element should be a single string, the value to
  ## be passed to Java for the corresponding property.
  ##
  ## The `libraryPath' is a vector of strings, each defining a
  ## directory to be included in the path recognized by the Java
  ## `loadLibrary' methods.
  ##
  ## It's expected that the object `.javaConfig' will have been defined with
  ## the appropriate default values for the local installation; if so, this
  ## object is used as the `default' for the arguments to `javaConfig'.
  ## The other arguments are merged with the corresponding default values; that is,
  ## the supplied path elements or properties are, respectively, put in front of
  ## or in place of the corresponding default values.
  ##
  ## If you want only the values supplied as explicit arguments, supply the `default'
  ## argument in the form `new("javaConfig")'.
  function(classPath=NULL, properties, libraryPath,
            default = if(exists(".javaConfig")) .javaConfig else list()) 
 {
   if(missing(classPath)) {
        # note that this requires R >= 1.2.0
        # with the Sys.getenv. We could just use getenv
        # but that is being deprecated so we put in both
        # as a reminder.
     if(exists("Sys.getenv")) {
       tmp <- Sys.getenv("CLASSPATH") 
     } else
       tmp <- getenv("CLASSPATH") 

     if(nchar(tmp) > 0)
       classPath <- strsplit(tmp,":")[[1]]
   }

    if(length(classPath) > 0)
      default$classPath <- mergePath(classPath, default$classPath)
    if(!missing(properties))
      default$properties <- mergeProperties(properties, default$properties, F)
    if(!missing(libraryPath))
      default$libraryPath <- mergePath(libraryPath, default$libraryPath)
    default
  }
#
#  Copyright (c) 1998, 1999 The Omega Project for Statistical Computing.
#       All rights reserved.#
