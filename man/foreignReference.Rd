\name{foreignReference}
\alias{foreignReference}
\title{Create a reference to an R object for use by a foreing system.}
\description{
 Create an object that is used to represent a local R object
 in a call to a foreign system (Java), optionally specifying
 a name by which it is to be stored locally and known externally
 and the name of one or more classes/interfaces that should be used
 to represent it. 
 The last of these is interpreted by the remote system.
}
\usage{
foreignReference(obj, id="", className=NULL, targetClasses=NULL, register=T)
}
\arguments{
 \item{obj}{The R value/object that is to be represented by this
reference. When methods are invoked on the reference, they are applied
to this object.}
 \item{id}{An identifier for the reference by which it can
be known to foreign systems and internally.
This is just a name.}
 \item{className}{ }
 \item{targetClasses}{ The name of a class or of interfaces which the foreign
system should use when representing this object. This allows the
reference to implement different e.g. Java interfaces so it can be
used as an argument to different methods.}
 \item{register}{a logical value indicating whether 
the object should be ``exported'' by the foreign system (\code{TRUE}),
or otherwise just a local object created to represent the R value
without making it accessible remotely.
If no value for \code{id} is supplied and the reference
is registered, a counter used to generate unique references
is incremented. (Is this true?)
}
}
\details{

}
\value{
 An object of class \code{foreignReference}.
}
\references{\url{http://www.omegahat.org/RSJava}
\author{Duncan Temple Lang, John Chambers }

\seealso{
\code{\link{.Java}}
\code{\link{.JavaConstructor}}
}

\examples{
  data(mtcars)
  foreignReference(mtcars, targetClasses="DataFrameInt") 
}

\keyword{programming}
\keyword{interface}