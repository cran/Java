.RSJava.symbol <-
function(name)
{
paste("RS_JAVA_",name,sep="")
}

.JavaInit <-
  ## initialize the S-Java interface and start the Java Virtual Machine.
  ##
  ## Argument `classPath', if supplied, is a vector of directory paths
  ## to be added to the class path for the Java Virtual Machine when
  ## it starts up.  (This path always includes the locations of the
  ## classes used by the interfaceManager and the standard Omega classes.)
  ##
  ## This function is called from the .Java interface; except for the
  ## first call, it normally detects the `.JavaInitialized' object in
  ## frame 0 and does nothing.  On the first call, it collects the
  ## Java configuration information by a call to `.JavaConfig', uses
  ## that to set a shell variable and for arguments to the C routine
  ## that starts the VM.
  ##
  ## See `.JavaConfig' for ways to customize the configuration
  ## parameters of the Virtual Machine.
  function(config = NULL, default = javaConfig(), verbose=F, callbackHandler=javaHandlerGenerator())
  {
    if(exists(".JavaInitialized", frame=0))
        return(TRUE)

    classPath <- mergePath(config$classPath, default$classPath, collapse=":")
    if(verbose) 
      message("Using class path: ", classPath)
    libraryPath <- mergePath(config$libraryPath,default$libraryPath, collapse=":")
    properties <- mergeProperties(config$properties, default$properties)

   i <- grep("^-DinterfaceLibraryFile", properties)
    if(length(i)==0) interfaceLibrary <- ""
    else {
      interfaceLibrary <- substring(el(properties, i), nchar("-DinterfaceLibraryFile=")+1)
      properties <- properties[-1]
    }

#    .Call("s_start_VM", classPath, libraryPath, interfaceLibrary, properties)
#    .C("s_start_VM", classPath, libraryPath, interfaceLibrary, properties)

    .C("s_start_VM", as.character(classPath), as.character(properties),length(as.character(properties)))

    .Call("RJava_setDefaultHandlerFunction", callbackHandler)

     assign(".JavaInitialized" , T, immediate=T)
  }


.JavaTerminate <-
function()
{
 if(isJavaInitialized())
   .C("RS_JAVA_terminateJava") 

 T
}
