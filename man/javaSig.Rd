\name{javaSig}
\alias{javaSig}
\title{Converts an R type name to a Java type}
\description{
  When specifying a Java type in the \code{.sigs} argument
of the different method/constructor calls to influence which
method is dispatched in the remote system and how R
objects are converted, one must use the appropriate type
specifier. This function converts R types to the corresponding
Java string. This handles converting R primitive types
such as integer to \code{"I"}, double to \code{"D"},
logical to \code{"Z"}, etc. and
classes to \code{"Lpkg/subpkg/.../className;"}
}
\usage{
javaSig(name)
}
\arguments{
  \item{name}{The name of the R type whose corresponding
Java type identifier is being sought. If this
is specified as an object, its mode is take.
See \code{\link{RtoJavaSig}}}
\details{
This searches the mapping contained in 
\code{.JavaSigs}
}
\value{
 a string (character vector of length 1) with the
 Java type identifier corresponding to the input.
}
\references{\url{http://www.omegahat.org/RSJava}}
\author{John Chambers, Duncan Temple Lang}

\seealso{
\code{\link{RtoJavaSig}}
}

\examples{

}
\keyword{Java}
\keyword{interface}
