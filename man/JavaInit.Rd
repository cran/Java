\name{.JavaInit}
\alias{.JavaInit}
\title{Initialize or terminate the Java Virtual Machine}
\description{
\code{.JavaInit} loads and starts the Java Virtual Machine
and the Omegahat session which brokers requests
to Java classes and objects.
The arguments to this function control
the initial configuration and environment of the
JVM.
}
\usage{
.JavaInit(config=NULL, default=javaConfig(), verbose=F, callbackHandler=javaHandler)
}
\arguments{
  \item{config}{an object containing elements to be used
in the classpath of the new JVM, system properties, and libraryPath elements
for loading code via JNI (from Java). See \code{\link{javaConfig}}}
  \item{default}{ the default configuration options (classpath, properties, etc.)
that are merged with those from the  \code{config} argument.}
  \item{verbose}{logical value indicating whether diagnostic
information should be displayed on the screen as the JVM
and Omegahat session are initialized. This is for use in debugging
failures or anomalies in the startup.}
  \item{callbackHandler}{an object (usually a closure)
that handles requests from the Java/Omegahat system
for method invocations on R objects exported to that
foreign system. This is usually
\code{\link{javaHandlerGenerator}}
}
\details{
 Creates and starts the JVM and Omegahat session.
Also, registers a  function or list of functions (closure)
to handle callbacks from Java to R objects and functions.
}
\value{
 A logical value indicating whether the initialization
was successful.
}
\references{\url{http://www.omegahat.org/RSJava}
\author{ Duncan Temple Lang, John Chambers}


\seealso{ 
\code{\link{javaConfig}}
}

\examples{
\dontrun{
 .JavaInit()
}
}
\keyword{programming}
\keyword{interface}
