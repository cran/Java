#
# The idea is to take a collection of methods
# from a Java interface and provide a template or
# skeleton of the corresponding R closure that would 
# implement these.

interfaceGenerator <- function(name, file = "")
{

 jmethods <- .Java(NULL, "getMethods", name)

 cat("# Template implementation of R closure for Java interface", name, "\n\n", file=file)
 cat(paste(name,"generator",sep="."), file=file, append=T)
 cat(" <- function() {\n\n", file=file, append=T)
 for(i in jmethods) {
   cat(i$name," <- function(", file=file, append=T)
   if(length(i$Parameters) > 0)
     cat(paste(letters[1:length(i$Parameters)],sep=", "), file=file,append=T)
   cat(")  {\n", file=file, append=T)
   cat("}\n\n", file=file, append=T)
   cat("attributes(",i$name,")$jsignature <- ",paste("\"", paste(i$Parameters,sep=","),"\"",sep="") ,file=file, append=T)
   cat("\n\n", file=file, append=T)
 }

 mnames <- sapply(jmethods, function(x) x$name)

 cat("return(list(",paste(mnames,mnames,sep="=", collapse=", "), "))", file=file, append=T)

 cat("\n\n} # end of closure\n\n", file=file,append=T)
 


}
