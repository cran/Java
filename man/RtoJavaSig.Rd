\name{RtoJavaSig}
\alias{RtoJavaSig}
\title{Returns the Java type identifier for an R object}
\description{
 This takes the given object and returns a string
 that can be used in the 
 This is of most use for primitivesso that one
 doesn't have to remember the different Java
 characters representing its primitive types.
 This is currently not useful for non-primitive objects (e.g. lists)
 until \code{\link{javaSig}} is enhanced.
}
\usage{
RtoJavaSig(obj)
}
\arguments{
  \item{obj}{Any R object, but the result is \textit{currently} only meaningful
if this is a primitive.}
}
\details{
 This examines the class and/or mode of the 
specified object and then calls \code{\link{javaSig}}
to find the name of the Java class corresponding
to the name of the given object's type.
}
\value{
 A character vector of length 1 identifying
the Java type (primitive or class)
corresponding to the type of the input object.
}
\references{\url{http://www.omegahat.org}, \url{http://www.javasoft.com}}
\author{John Chmabers, Duncan Temple Lang}
\note{This will be enhanced in future versions as more elaborate
conversion mechanisms are added.}

\seealso{\code{\link{javaSig}}}

\examples{
 javaSig(1)
 javaSig(as.integer(10))
 javaSig("a string")
 javaSig(list(a=1))
}
\keyword{Java}
\keyword{interface}
