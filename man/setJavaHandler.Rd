\name{setJavaHandler}
\alias{setJavaHandler}
\title{Register a handler for exporting R objects to foreign systems.}
\description{
 Registers a manager for R objects that are to be used by 
foreign systems such as Omegahat and Java. 
The registration provides access to the object
from C routines that bridge the two systems - R and the foreign one.
}
\usage{
setJavaHandler(handler)
}
\arguments{
 \item{handler}{An object that manage the references
and provide the different functions
for accessing and manipulating those references.
These functions are described below and implemented
in \code{\link{javaHandlerGenerator()}}}
\details{
 This registers the handler object with the 
C code so that it is known to the Java methods.
The methods that must be provided
are
\item{\code{handler()}}{
brokers a method request for a reference
under the management of this handler,
taking care of passing the arguments,
identifying the appropriate method,
and catching errors.
}
\item{\code{createReference()}}{
creates an actual \code{foreignReference}
object by calling
the \code{\link{foreignReference}}
function.
}
\item{\code{addReference}}{
 adds an object to the list
being managed by this reference handler.
An explicit name can be provided in the call
to this method, or otherwise a unique one is generated
by the manager itself.
}
\item{\code{remove}}{
discards the identified object
from the list of objects being
managed by this reference handler.
}
\item{\code{getReference}}{
 retrieves a particular object
being managed by this reference manager
using the name of the reference.
}
\item{\code{references}}{
returns a (named) list of all the objects
being managed by this reference manager.}
\item{\code{total}}{returns the number of
references that have been managed by this object.
This is used in constructing new unique names when
an object is registered without an explicit identifier.}

The intent of the handler is to allow the management
of the objects being exported to the foreign
system(s) (Java and Omegahat) 
}
\value{
 The previous value of the  registered handler
that has been replaced with this value.
This allows one to temporarily replace  a handler
with a new version and then swap the original
back at the end of an operation/transaction.
}
\references{\url{http://www.omegahat.org/RSJava/Conversion.html}}
\author{Duncan Temple Lang}
\seealso{
\code{\link{getJavaHandler}}
}

\examples{
  old <- setJavaHandler(javaHandlerGenerator())
    ..

  setJavaHandler(old)
}
\keyword{programming}
\keyword{interface}
