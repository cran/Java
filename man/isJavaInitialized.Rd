\name{isJavaInitialized}
\alias{isJavaInitialized}
\title{Determines whether the JVM has been created.}
\description{
  This determines whether the Java Virtual
machine has already been initialized within this
R session, usually via the \code{\link{.JavaInit}}
This is useful when we want to use the Java interface,
but want to avoid an error being thrown if the user hasn't
already created Java.
}
\usage{
isJavaInitialized(msg=NULL)
}
\arguments{
  \item{msg}{
A character string, which if specified and the
virtual machine has not been initialized, is passed
as the single argument in a call to 
\code{\link{stop}}.
}
}
\details{
This checks the state of the internal
C variables to determine if the user has 
initialized the JVM. It does not attempt to create
the JVM. This allows the user to specify different
arguments to customize the VM.
}
\value{
 A logical value indicating whether the JVM
has been created earlier (\code{TRUE})
or not (\code{FALSE}).
 If the \code{msg} argument is specified
and the JVM has not been initialized,
an error is thrown and there is no return value.
}
\references{\url{http://www.omegahat.org/RSJava}}
\author{Duncan Temple Lang}

\seealso{
\code{\link{.JavaInit}}
}

\examples{
  isJavaInitialized("You must first initialize the Java engine (via .JavaInit())")
  jlabel <- .JavaConstructor("javax.swing.JButton","Welcome the R-Java interface")
  f <- .JavaConstructor("org.omegahat.Environment.GUITools.GenericFrame", jlabel, T)

    # optionally execute code.
 if(isJavaInitialized()) {
   .Java("System", "getProperty", "java.class.path")
 }
}
\keyword{Java}
