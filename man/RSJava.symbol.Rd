\name{.RSJava.symbol}
\alias{.RSJava.symbol}
\title{Expands a name to a C routine name in this package.}
\description{
In order to avoid conflicts with other packages having the same
C routine names, we use a macro \code{RS_Java} to identify
the names of C routines. This akes it easy for us to generate
unique names. This function allows callers of these routines
from R (via the \code{\link{.C}} and \code{\link{.Call}} functions)
to refer to them via their non-expanded name and have
this function perform the appropriate expansion.
Currently, this prefixes the regular name with
\code{"RS_JAVA_"}.
}
\usage{
.RSJava.symbol(name)
}
\arguments{
  \item{name}{The unexpanded name of the C routine, i.e. without
the prefix.}
}

\value{
 The string identifying the C routine corresponding to
the short (unexpanded) reference given in \code{name}.
}
\references{\url{http://www.omegahat.com/RSJava}}
\author{Duncan Temple Lang}

\examples{
  .C(.RSJava.symbol("isJVMInitialized"), logical(1))
}
\keyword{programming}
\keyword{interface}
