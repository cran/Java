\name{mergePath}
\alias{mergePath}
\title{Merges classpath specifications}
\description{
 Merges two character vectors of classpaths
for use in creating  arguments
to initialize the Java Virtual Machine
within R. This avoids duplicates.
}
\usage{
mergePath(path, default, collapse=NULL)
}
\arguments{
 \item{path}{character vector of path elements}
 \item{default}{a  character vector containing
the default or previous classpath elements}
 \item{collapse}{a character vector used the value
of a the \code{collapse} argument in a call
to \code{\link{paste}}.
If this is non-null, the resulting
vector is converted to a single string.
This is usually given as ":"
on Unix machines and ";" on Windows machines.
}
}
\value{
 A character vector containing the
union of the two arguments.
If the \code{collapse} argument
is specified, the elements of the resulting character
vector are concatenated/pasted together to yield
a single string.
}
\references{\url{http://www.omegahat.org/RSJava}
\author{Duncan Temple Lang, John Chambers }


\seealso{
\code{\link{.JavaInit}}
\code{\link{javaConfig}}
}

\examples{
  mergePath("~/Java/MySQL/mm.mysql-2.0.1/mysql.jar",c("$OMEGA_HOME/Jars/antlr.jar","$OMEGA_HOME/Jars/jhall.jar"))
  mergePath("~/Java/MySQL/mm.mysql-2.0.1/mysql.jar",c("$OMEGA_HOME/Jars/antlr.jar","$OMEGA_HOME/Jars/jhall.jar"), collapse=":")
}
\keyword{programming}
\keyword{interface}
