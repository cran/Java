\name{.JavaArrayConstructor}
\name{.JavaGetArrayElement}
\name{.JavaSetArrayElement}
\name{.JavaArrayLength}
\alias{.JavaArrayConstructor}
\alias{.JavaGetArrayElement}
\alias{.JavaSetArrayElement}
\alias{.JavaArrayLength}
\title{Create and access elements of Java arrays from R.}
\description{
  These functions allow one to create multi-dimensional 
Java arrays via R commands using the \code{\link{.Java}}
function. The get and set accessors work element-wise
and not in the vector fashion common in R and S.
One must create and initialize the Java virtual machine 
before calling any of these functions. See
\code{\link{.JavaInit}}. 
}
\usage{
.JavaArrayConstructor(klass, ..., dim=length(list(...)), .name=NULL, .convert=F)
.JavaGetArrayElement(jobj,..., .name=NULL, .convert=T)
.JavaSetArrayElement(jobj, value, ...)
.JavaArrayLength(jobj)
}
\arguments{
  \item{klass}{Typically a string (character vector of length 1)
identifying the name of the class of the element type in the array to be created.
This can also be a foreign reference to a Java class object
obtained via an earlier call to \code{\link{.Java}}
}
  \item{\dots}{In the \code{.JavaArrayConstructor}, these
are currently ignored. They are intended to be initializing values
that are used to populate the top-level values of the new array.
That is, they are used to set 
\code{arr[0], arr[1], arr[2], \dots}
  \item{dim}{When creating an array in \code{.JavaArrayConstructor},
these specify both the number of dimensions and the length of each dimension 
in the array to be created.
}
  \item{jobj}{This is the reference to the Java array returned
from an earlier call to \code{.JavaArrayConstructor}
or the return value from a call to \code{\link{.Java}}.}
  \item{value}{In \code{.JavaA}
}
\details{
  This uses the \code{\link{.Java}}
to call methods in the Omegahat Evaluator
which process the array request.
}
\value{
 \code{.JavaArrayConstructor} returns a reference
to the newly create Java array object.

 \code{.JavaArrayLength} returns a single integer
giving the length of the top-level dimension of the array.

 \code{.JavaGetArrayElement} returns the value of
the specified element of the given array, converted
to an R object as usual. Thus it may be a Java reference.

 \code{.JavaSetArrayElement} returns \code{NULL}.
}
\references{\url{http://www.javasoft.com}, \url{http://www.omegahat.org}}
\author{Duncan Temple Lang, John Chambers}

\seealso{
\code{\link{.Java}}
}

\examples{
 a <- .JavaArrayConstructor("String", dim=3)
 .JavaArrayLength(a)
 .JavaSetArrayElement(a, "First", 1)
 .JavaSetArrayElement(a, "Second", 2)
 .JavaSetArrayElement(a, "Third", 3)
 .JavaGetArrayElement(a, 2)
}
\keyword{Java}
\keyword{programming}
\keyword{interface}
