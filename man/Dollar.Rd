\name{AnonymousOmegahatReference}
\name{NamedOmegahatReference}
\alias{$.AnonymousOmegahatReference}
\alias{$.NamedOmegahatReference}
\title{Convenience syntax for accessing Java method or field}
\description{
These methods allow one to invoke Java methods
in the form
\code{ jobj$methodName(arg1,arg2,....)}
}
\usage{
$.AnonymousOmegahatReference(obj, name)
$.NamedOmegahatReference(obj, name)
}
\arguments{
  \item{obj}{the reference to the Java object.}
  \item{name}{the name of the Java method or field being accessed.}
}
\details{
 This is merely syntactic sugar for
\code{.Java(obj, name,)}
}
\value{
 The return value is a function which remembers
the arguments to this function call and, when called,
results in a call to \code{\link{.Java}} using 
those arguments.
}
\references{\url{http://www.omegahat.org/RSJava}}
\author{Duncan Temple Lang}
\seealso{
\code{\link{.Java}}
}

\examples{
 p <- .Java("System", "getProperties", .convert=F)
 p$getProperty("java.class.path")
}
\keyword{Java}

