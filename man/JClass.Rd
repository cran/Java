\name{.JClass}
\alias{.JClass}
\title{Returns a reference to a Java class.}
\description{
This returns the class of the specified object
or that found by resolving the (partially qualfied)
class name. The resulting class reference can be used
to access static fields and method, and a description
of the class. This is most conveniently done
using the \code{$} operator. 
}
\usage{
.JClass(x, name = NULL)
}
\arguments{
  \item{x}{a (partially qualified) class name
or a reference to a Java object whose class name is
used to resolve the class.}
  \item{name}{an optional string to use as the name to use
for the resulting class reference in the Omegahat
database. This is passed as the \code{.name} argument
to \code{\link{.Java}}.
}
}
\details{
 This calls the Omegahat evaluator's
\code{findClass} method with the name of
the class to be found. 
}
\value{
 An anonymous reference to a Java class.
This can then be used to access static methods
and fields.
}
\references{\url{http://www.omegahat.org/RSJava}}
\author{Duncan Temple Lang, John Chambers}
\seealso{
\code{\link{.Java}}
}

\examples{
  jsys <- .JClass("System")
  jsys$getProperties()
  jsys$getProperty("java.class.path")

   rt <- .JClass("Runtime")$getRuntime()
   rt$exec("whoami")
   rt$exec(paste("find",system.file("scripts", pkg="Java")))

   frame <- .JNew("JFrame")
   frame$setBackground(.JClass("Color")$red())
} 
\keyword{Java}
