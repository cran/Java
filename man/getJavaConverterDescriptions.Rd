\name{getJavaConverterDescriptions}
\alias{getJavaConverterDescriptions}
\title{Retrieves descriptions for the registered converters between R and Java objects.}
\description{
 The conversion between R and Java objects is controlled by a list
of actions. Each contains a description string to help the user
understand what it does. This function returns these descriptions
for the converters in one or both directions (i.e. from R to Java or
vice-versa).
}
\usage{
getJavaConverterDescriptions(which=c(fromJava = F, toJava = T))
}
\arguments{
  \item{which}{
A logical vector in which \code{FALSE} identifies
the converters from Java to R and \code{TRUE}
indicates from R to Java.
}
}
\details{
This examines the internal C data structures
used to maintain the conversion tables.
}
\value{
 Returns a list with the same length  as \code{which}
in which each element is a character vector containing
the description strings from the different
registered conveters for that conversion direction.
These include the default converters that handle
the conversion of primitives between the two systems.
}
\references{\url{http://www.omegahat.org/RSJava}}
\author{Duncan Temple Lang}

\seealso{ 
\code{\link{getNumJavaConverters}}
\code{\link{setJavaConvertible}}
}

\examples{
  getNumJavaConverters()
  getNumJavaConverters(T)
  getNumJavaConverters(F)
}
\keyword{Java}
\keyword{interface}
