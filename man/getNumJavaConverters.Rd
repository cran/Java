\name{getNumJavaConverters}
\alias{getNumJavaConverters}
\title{Returns the number of converters registered between R and Java}
\description{
 This returns the number of converters currently registered
between R and Java. The argument
specifies the desired direction of the conversion, by 
default querying both from Java to R and from R to Java.
}
\usage{
getNumJavaConverters(which=c(fromJava = F, toJava = T))
}
\arguments{
  \item{which}{ a logical  vector in which
\code{FALSE} indicates from Java to R
and \code{TRUE} indicates from R to Java.
}
}
\details{
 This accesses the internal C data structures
that maintain the converter lists. These
are in C so that the low-level JNI code can
access them directly without the overhead of
converting to reference objects and losing
contextual information.
}
\value{
  An integer vector with the same length 
as the argument \code{which}.
The value of each element is the number
of registered converters in the corresponding list.
}
\references{\url{http://www.omegahat.org/RSJava}}
\author{Duncan Temple Lang}

\seealso{
\code{\link{getJavaConverterDescriptions}}
}

\examples{
  getNumJavaConverters()
  getNumJavaConverters(F)
  getNumJavaConverters(T)
}
\keyword{Java}
\keyword{programming}
\keyword{interface}

