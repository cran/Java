\name{.OmegahatExpression}
\alias{.OmegahatExpression}
\title{Execute an Omegahat/Java expression}
\description{
 This evaluates the specified expression in the 
Omegahat sub-system, resolving references from
the Omegahat databases and the list of arguments
provided in this call.
}
\usage{
.OmegahatExpression(expr, ..., .name=NULL, .sigs="")
}
\arguments{
  \item{expr}{A string value that is a valid Omegahat expression.}
  \item{\dots}{a collection of named arguments which are 
converted to Java objects
and available to the Omegahat expression when it is evaluated
using the names of the arguments.
}
  \item{.name}{The name to use to store the result in the omegahat named reference
database. If this is missing, an  anonymous reference is returned or
the value converted to an R object. If the result of the Java method
can be converted, this argument can be used to prohibit this conversion
and leave the Java value in Omegahat for use in future \code{.Java} calls.}
  \item{.sigs}{not really needed here, but can be used
to control the conversion of the arguments
in \dots}
\item{.convert}{logical value indicating
whether the Omegahat interpreter should attempt 
to convert the result of the expression
to an R object (TRUE), or alternatively
just assign the value to a local database and return a reference.
This is useful when one wishes to avoid converting
an object back to its R counterpart because you wish to use it in
subsequent \code{\link{.OmegahatExpression}} or
\code{\link{.Java}} calls.
}
}
\details{
 This can be used to create functions, assign multiple
values in a single call, create arrays easily, etc.
One of the drawbacks of using this is that
the details of the Omegahat and Java languages
are exposed to the code that calls them in this manner.
By using the \code{\link{.Java}}
and \code{\link{.JavaConstructor}} functions,
one can easily subsititute different
implementations that for example, use CORBA
to invoke methods in remote objects written
in different languages.
\\
In some ways, this has similarities to 
\code{\link{substitute}}.
}
\value{
 The result of the Omegahat evaluation of the expression,
converted from a Java object to an R object using
the basic and extensible conversion mechanism between 
Java and R.
}

\seealso{
\code{\link{.Java}}
\code{\link{.JavaConstructor}}
}

\examples{
 .OmegahatExpression("show(1::10)")
 .OmegahatExpression("show(1::z);", z=10)
}
\keyword{programming}
\keyword{interface}
