\name{interfaceGenerator}
\alias{interfaceGenerator}
\title{Generates a template "closure" to represent a Java interface/class.}
\description{
 To use an R variable as a Java object, one
can create a closure or list of functions
that implement the methods of that Java class.
The template of such a closure can be generated
automatically using the reflectance capabilities
of both Java and R. This function
generates such a template and can be used
as an initial step in implementing an R
version of a Java class.
}
\usage{
interfaceGenerator(name, file="")
}
\arguments{
  \item{name}{the name of the Java class or interface
whose methods are to be duplicated locally via R functions.
This is resolved using the usual Omegahat rules so this can
be a partially qualified class name.}
  \item{file}{The name of a file to which the template functions
are written. This can then be edited to provide an
implementation of the Java class via an R object.}
}
\details{
 This is useful for allowing an R object to be converted to a proxy
Java object.  For example, consider using an R object as a callback
for a Swing button. The object must implement the
\code{actionPerformed()} method of the \code{ActionListener}. This
function calls the Omegahat evaluator's \code{getMethods()} method to
retrieve a list of Java method descriptions and then converts
them to an R closure definition.
}

\value{
 This function has the side-effect of writing the
definition of a function closure definition 
to standard output (the console) or to a file.
In the future, we will generate the actual function
objects. The idea is merely to show the possibilities
available to us using  reflectance.
}
\references{\url{http://www.omegahat.org/RSJava}}
\author{Duncan Temple Lang}

\seealso{
\code{\link{foreignReference}}
}

\examples{
  interfaceGenerator("java.awt.event.ActionListener")
  interfaceGenerator("java.awt.event.ActionListener", "MyFile")
}
\keyword{Java}
\keyword{programming}
\keyword{interface}
