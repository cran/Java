\name{.JavaConstructor}
\alias{.JavaConstructor}
\alias{.JNew}
\title{Create a Java object}
\description{
 Creates a Java object by calling a constructor from
the desired class. The object is (almost always)
stored in the Omegahat
session and a reference to it returned.
One must create and initialize the Java virtual machine 
before calling this function. See
\code{\link{.JavaInit}}. 
\code{.JNew} is a simple alias of \code{.JavaConstructor}.
}
\usage{
.JavaConstructor(className, ..., .name="", .sigs="", .convert=T)
.JNew(className, ..., .name="", .sigs="", .convert=T)
}
\arguments{
  \item{className}{The name of the Java class to be instantiated.
This can be either the full name or a partially qualified 
name which will use the Omegahat class locator mechanism to find
the class. It is better (but less convenient) to give
the full name as this avoids the lengthy one-time
construction of the class lists in Omegahat.
It makes sense to give partially qualified names
for a) the user's convenience,
b) when  one expects to substitute
different packages with same-named classes that can be 
used in place of each other.
}
  \item{\dots}{the arguments used to identify and be passed to the constructor
in the target class being instantiated.}
  \item{.name}{The name to use to store the result in the omegahat named reference
database. If this is missing, an  anonymous reference is returned or
the value converted to an R object. If the result of the Java method
can be converted, this argument can be used to prohibit this conversion
and leave the Java value in Omegahat for use in future \code{.Java} calls.}
  \item{.sigs}{A character vector of class identifiers that help to identify
the Java method to be invoked. This is used to avoid ambiguity introduced
by Java's polymorphism/overloaded names and the automatic/implicit conversion
performed between R and Java objects.}
\item{.convert}{
 a logical value indicating whether the Omegahat interpreter
should attempt to convert the newly created object to 
an R object (TRUE) or simply leave it in the Omegahat database.
This is ignored if a value for \code{.name} is supplied.
}
}
\details{
This creates a new Java object by first converting
the R arguments to Java objects and then looking
for a constructor  in the target class that accepts
arguments of these types.
The resulting Java object is available for
future computations as arguments
to \code{\link{.Java}}, \code{.JavaConstructor},
and \code{\link{.OmegahatExpression}}.
At present, the object must be explicitly freed by the caller.
This is always true if a value is given for the \code{.name}
argument.
}
\value{
If a value for the argument \code{.name}
is provided, this returns a \code{NamedReference}
to a Java object stored in the Omegahat session.
Otherwise, usually an \code{AnonymousReference}
is returned.
However, if a converter to R exists for the particular Java class
being created \textit{and} no value for the
\code{.name} argument is given in the call,
the Java object will be converted directly to an R object.
This is sometimes useful when the constructor
populates the object's fields and one has no further user
for the object itself, but just its contents.
For example, the basic constructor for the class
\code{StatDataURL} takes a URL name
and reads its contents. A converter could be registered
for this class that returns the lines of text.
}
\references{\url{http://www.omegahat.org/RSJava}
\author{Duncan Temple Lang, John Chambers }
\note{Uses the Omegahat interactive Java environment.}

\seealso{
\code{\link{.Java}}
\code{\link{.OmegahatExpression}}
}

\examples{
 tmp <- .JavaConstructor("util.Vector", as.integer(10))
 .Java(tmp, "add", "This is a string")
 .Java(tmp, "add", 1.5)

 b <- .JavaConstructor("JButton","R Java Button")
 .Java(tmp, "add", b)
 
 f <- .JavaConstructor("GenericFrame", b)

 f <- .JNew("GenericFrame", b)
}
\keyword{programming}
\keyword{interface}