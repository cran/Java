\name{setJavaConvertible}
\alias{setJavaConvertible}
\title{Register a Java class as being convertible to an R object}

\description{
 When a value is to be returned from Java to R, the Omegahat evaluator
examines a table to determine if an object of that type can be
converted to R.  This function manipulates that table and allows one
to control for what types of objects conversion is attempted.  
}

\usage{
setJavaConvertible.java(klass, ok=T, matching=0)
}
\arguments{
  \item{klass}{The name of a Java class or interface
which is to be added or removed from the set of known
convertible classes. 
This can be a partially qualified class name
that is resolved by the Omegahat evaluator.}
  \item{ok}{logical value indicating whether objects of
class \code{klass} are to be considered convertible or not
by the Omegahat sub-system. }
  \item{matching}{
an integer value from the set 0, 1 and 2.
These values indicate how comparisons between the registerd
class and the object to be converted are performed.
\item{0}{An exact match, meaning that the class of the object
must be the same as the class being registered as convertible.}
\item{1}{the target object must be an instance of the class being
registered, meaning that it implements this Java interface
or is an instance of a class that is derived from the registered one.}
\item{2}{the target object must be compatible with the registered
class in the sense of \code{isAssignableFrom} method between two classes.}
These constants are defined in the Java interface \code{ConvertibleClassifierInt}.
}
}
\details{
 Simply calls \code{setJavaConvertible} in the Omegahat evaluator which passes
the request on to the \code{ConverterClassifierInt} object
employed by that evaluator.
}
\value{
 NULL corresponding to a call to a Java method
returning \code{void}.
}
\references{\url{http://www.omegahat.org/RSJava}}
\author{Duncan Temple Lang}

\seealso{
\code{\link{setJavaConverter}}
\code{\link{getJavaConverterDescriptions}}
\code{\link{getNumJavaConverters}}
}

\examples{

}
\keyword{Java}
\keyword{programming}
\keyword{interface}
