\name{javaHandlerGenerator}
\alias{javaHandlerGenerator}
\title{Manages exporting of R objects to Java/Omegahat and calls from the latter to R objects.}
\description{
  This creates a closure that manages objects exported from
the R session to Java/Omegahat as arguments to constructors and methods
in that system. When the Java code invokes a method on such a reference,
the R object is resolved by this closure and the appropriate
R function invoked on that object.
}
\usage{
javaHandlerGenerator()
}
\details{
}
\value{
 A closure containing 
the ``methods''

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

}
\references{\url{http://www.omegahat.org/RSJava}}
\author{Duncan Temple Lang, John Chambers}

\seealso{
}

\examples{
\dontrun{
 .JavaInit(callbackHandler = javaHandlerGenerator())
}
}
\keyword{Java}

