\name{.Java}
\alias{.Java}
\title{Invokes a Java method}
\description{
 Calls a Java method, transferring R arguments to the
Java (Omegahat) system as needed.
This can be used to call methods in the Omegahat evaluator,
not just on previously created user-level objects.
}
\usage{
.Java(.qualifier, .methodName, ..., .name=NULL, .sigs="")
}
\arguments{
  \item{.qualifier}{ The Java object whose method is to be invoked.
This is the `this' in the Java call and is typically a
reference obtained as the result of a  previous call
to \code{.Java} or \code{\link{.JavaConstructor}}. \\
If this is NULL or omitted, the Omegahat evaluator
looks first for an Omegahat function and then a method
within its own object.}
  \item{.methodName}{The name of the method (or function if \code{.qualifier} is \code{NULL}) 
that is to be invoked in the Java object.}
  \item{\dots}{arguments to be passed to the Java method call.
Any values that are named (i.e. x = 1) are assigned
persistently to the Omegahat \textit{named reference database} and can be referenced
directly in future calls. }
  \item{.name}{The name to use to store the result in the omegahat named reference
database. If this is missing, an  anonymous reference is returned or
the value converted to an R object. If the result of the Java method
can be converted, this argument can be used to prohibit this conversion
and leave the Java value in Omegahat for use in future \code{.Java} calls.}
  \item{.sigs}{A character vector of class identifiers that help to identify
the Java method to be invoked. This is used to avoid ambiguity introduced
by Java's polymorphism/overloaded names and the automatic/implicit conversion
performed between R and Java objects.}
\item{.convert}{a logical value that indicates whether the 
Omegahat manager should attempt to convert the result
of the method call. This is usually \code{TRUE}, but
can be explicitly specified to avoid (arrays of) primitive object
being converted to an R object when it is to be used in a subsequent
\code{.Java} call.
 }
}
\details{
 This invokes a Java method on the target object
by first converting the R arguments to Java objects
and then searching the Java object for a method
that accepts these Java argument types.
Then it invokes the method and converts the result
to a Java object using the basic and extensible conversion mechanism
between Java and R.
}
\value{
 The return value of the Java method invocation, converted
to an R object. If the Java value is considered convertible,
one of the registered converters is called. By default,
these handle primitives (scalars) and Java collections.
The user can register others. If no converter is found,
a reference to the Java object is returned as an R object.
If the \code{.name} argument was supplied in the 
call to this R function, the reference is a 
\code{NamedReference}. Otherwise, it is a \code{AnonymousReference}.
}
\references{\url{http://www.omegahat.org/RSJava}
\author{Duncan Temple Lang, John Chambers }
\note{Uses the Omegahat interactive Java environment.}

\seealso{
\code{\link{.JavaConstructor}}
\code{\link{.OmegahatExpression}}
\code{\link{getJavaConverterDescriptions}}
% Want something about references here.
\code{\link{foreignReference}}
}

\examples{
  v <- .JavaConstructor("java.util.Vector", as.integer(10))
 .Java(v, "add", "A string element")
 .Java(v, "add", .JavaConstructor("java.util.Hashtable", as.integer(3)))
 .Java(v, "size")

 props <- .Java("System", "getProperties")
 props[["java.class.path"]]

  props <- .Java("System", "getProperties", .convert=F)
  props$getProperty("java.class.path")
}
\keyword{programming}
\keyword{interface}
