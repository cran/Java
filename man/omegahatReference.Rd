\name{omegahatReference}
\alias{omegahatReference}
\title{Creates a local object representing a Java reference}
\description{
 On occassion, one can lose a reference to a Java object
stored in the Omegahat databases. If one knows its identifier (i.e. Omegahat name)
and whether it is an anonymous or named reference, one can recreate
an R object that refers to this Java object and use this R object in
subsequent calls to the Java interface.
}
\usage{
omegahatReference(key, named=T)
}
\arguments{
  \item{key}{the name used by Omegahat to store the Java object.}
  \item{named}{a logical value indicating whether this is a named
(\code{TRUE})
or anonymous (\code{FALSE}) reference.}
}
\value{
An object of class either
\code{AnononymousOmegahatReference}
or 
\code{NamedOmegahatReference}.
This has fields

  \item{key}{The Omegahat name by which the Java object is
known. This is the value of the argument \code{key}.}
  \item{className}{the class name of the Java object.
This is always empty.
}
}
\references{ ~put references to the literature/web site here ~ }
\author{Duncan Temple Lang}
\seealso{
\code{\link{.Java}}
\code{\link{.JavaConstructor}}
}

\examples{
  omegahatReference("x")
  omegahatReference("x",F)
}
\keyword{Java}
