\name{getForeignReferences}
\alias{getForeignReferences}
\title{Get the names of the objects in the Omegahat system}
\description{
 This queries the Omegahat manager for the names of
the different objects it manages for use 
by \code{\link{.Java}}, \code{\link{.JavaConstructor}}
and \code{\link{.OmegahatExpression}} calls.
This allows the elements of either the named or anonymous or both databases
to be queried.
}
\usage{
getForeignReferences(which=c(named = T, anonymous = F))
}
\arguments{
  \item{which}{a  logical vector identifying the named (\code{TRUE})
and anonymous (\code{FALSE}) databases. The default
is both.}
}
\details{
 This uses the \code{\link{.Java}}
function to invoke the \code{getReferences} method
of the Omegahat manager/evaluator.
This aids one to query and control the interface manager.
}
\value{
 A list with the same length as the argument
\code{which}. Each element is a list
of 
This is not working as designed yet.
We need to add a converter for
an \code{InterfaceReference}.
}
\references{\url{http://www.omegahat.org/RSJava}
\author{Duncan Temple Lang, John Chambers}

\seealso{
\code{\link{.Java}}
\code{\link{.JavaConstructor}}
}

\examples{
  getForeignReferences()

    # Just the named values
  getForeignReferences(T)

    # Just the anonymous references
  getForeignReferences(F)
}
\keyword{Java}
\keyword{programming}
\keyword{interface}
