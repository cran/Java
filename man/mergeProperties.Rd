\name{mergeProperties}
\alias{mergeProperties}
\title{Creates the union of  two named character vectors, converting to
a Java-like property table}
\description{
 Utility function to merge the two named objects, with elements single character strings,
 and convert the result to a vector of Java property settings.
}
\usage{
mergeProperties(props, default, convert=T)
}
\arguments{
 \item{props}{named character vector of properties.}
 \item{default}{named character vector of properties
with which the elements in \code{props} are to be merged. 
 The values in \code{props} take 
}
 \item{convert}{a logical value which, if \code{TRUE} causes
the the resulting character vector to be converted
to Java property specifications for use in intializing
the Java Virtual machine, each of the form
 \code{-Dname=value}
}
}
\value{
 Augments the \code{default}
with the named values that are in
\code{props} and not in \code{default}
and also replaces those shared by both vectors
with those in \code{props}.
If the argument \code{convert} is 
\code{TRUE}, elements
of the vector are converted to Java
properties suitable for initializing the JVM.
}

\seealso{
\code{\link{mergePath}}
\code{\link{.JavaInit}}
\code{\link{javaConfig}}
}


\examples{
 mergeProperties(c(java.compiler="", myProperty="abc", "X_Y"="Hi there"), javaConfig()$properties)
}
\keyword{ ~keyword }%-- one or more ...
