\name{getJavaMethods}
\name{getJavaConstructors}
\alias{getJavaMethods}
\alias{getJavaConstructors}
\title{List the methods or constructors of a Java object.}
\description{
 This is a convenient method for obtaining a list
of all the methods a Java object provides.
}
\usage{
getJavaMethods(what)
getJavaConstructors(what)
}
\arguments{
  \item{what}{
 the (partially qualified) name of a Java class,
or a reference to a Java object managed by the
Omegahat  evaluator. The latter contains the class
name of the object.
}
}
\details{
 This is a simple use of \code{\link{.Java}}
and the evaluator's methods \code{getMethods}
and \code{getConstructors}. The \code{getJavaMethods}
also adds the names to the resulting R list.
}
\value{
 A list of Java Method objects converted
to their R equivalents.
The names of the elements in the list
are given by the name of the Java method.
In the case of \code{getJavaConstructors},
no names are given since these have no
explicit name.
Each element describes the corresponding Java method
in terms of 
\item{} the number and types of arguments,
\item{} its accessibility, 
\item{} in which class it was defined,
\item{} the exceptions it may throw.
}
\references{\url{http://www.omegahat.org/RSJava}}
\author{Duncan Temple Lang}
\seealso{
\code{\link{.Java}}
\code{\link{.JavaConstructor}}
\code{\link{.JNew}}
}

\examples{
  v <- .JNew("util.Vector")
    # get all the methods
  getJavaMethods(v)
    # get all the add() methods
  getJavaMethods(v)[["add"]]

  getJavaConstructors("util.Vector")    
}
\keyword{Java}
