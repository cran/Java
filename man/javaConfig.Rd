\name{javaConfig}
\alias{javaConfig}
\title{Returns a configuration object for initializing the Java Virtual Machine}
\description{
 This integrates user arguments with default values
to create an object containing the information to parameterize
the Java Virtual Machine's and the Omegahat session's initial environments.
The object includes a classpath specification, system properties
for the JVM that can also be read by the Omegahat system to govern
how its elements are instantiated and also a library path
for loading shared libraries as JNI code for Java classes.
}
\usage{
javaConfig(classPath, properties, libraryPath, default=if (exists(".javaConfig")) .javaConfig else list())
}
\arguments{
 \item{classPath}{a character vector identifying locations of
Java classes in the form of URLs, Jar files or simple directories.
This is passed to the JVM as the argument to \code{classpath}. \\
If this is not specified, the environment variable 
\code{CLASSPATH} is queried and if this set, its elements 
are prefixed to the default ones.
}
 \item{properties}{a named character vector of system properties that 
are passed to the JVM initialization as \code{-Dname=value} for each element.}
 \item{libraryPath}{a character vector identifying directories which are
to be used by Java when loading shared libraries/DLLs via \code{System.loadLibrary()}}
 \item{default}{a list containing the default values for each of the 3 fields/groups
of parameters to which are added the user-specified values in the
corresponding earlier  arguments.
}
}
\value{
  A list that can be used to customize the initialization of
the Java Virtual Machine embedded within the R session.
  \item{classPath}{a character vector whose
elements are sources of Java class files. These can be
directories, URLs or Jar files (or any other form understood
by the JVM). This is collapsed and specified as the value of
the \code{classpath} argument to the JVM initialization.}
  \item{properties}{a named character vector whose values
correspond to \code{name=value} pairs that are passed to
the JVM as system properties in the form \code{-Dname=value}.}
  \item{libraryPath}{a character vector whose elements specify directories
 which are searched by the JVM when loading native code for a Java
class via the \code{System.loadLibrary()} method.
}
}
\references{\url{http://www.omegahat.org/RSJava}}
\author{John Chambers, Duncan Temple Lang}

\seealso{
\code{\link{.JavaInit}}
\code{\link{mergePath}}
\code{\link{mergeProperties}}
\code{.javaConfig}
}

\examples{
 javaConfig()
 javaConfig(classPath=c("/home/duncan/Java", "/home/duncan/xml.jar", 
                         paste(system.file("org/omegahat/Jars"), "DataStructures.jar",sep="/")))
}
\keyword{Java}
