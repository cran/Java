\name{getJavaHandler}
\alias{getJavaHandler}
\title{Obtains the current R foreing reference manager}
\description{
 This queries the C code to retrieve the object that
handlers exporting R objects to foreign systems such
as Omegahat and Java.
}
\usage{
getJavaHandler(obj)
}
\value{
 A list (or object) that provides the functions 
needed by a reference manager.
See \code{\link{setJavaHandler}}
and \code{\link{javaHandlerGenerator}}
for a description of these methods.
}
\references{\url{http://www.omegahat.org/RSJava}}
\author{Duncan Temple Lang, John Chambers}
\seealso{\code{\link{setJavaHandler}}
\code{\link{javaHandlerGenerator}} }

\examples{
  old <- getJavaHandler()
  old$references()
  old$total()
  old$addReference(rnorm(100))
  old$addReference(foreignReference(rnorm(100), "mydata"))
  old$remove("mydata")
}
\keyword{Java}
\keyword{programming}
\keyword{interface}

