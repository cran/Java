\name{setJavaConverter}
\alias{setJavaConverter}
\title{Add a converter from Java to an R object}
\description{
 Register a C routine which converts a Java object
to an R object. This occurs when a value is returned
from a Java method (or constructor) call via
\code{\link{.Java}} or \code{\link{.JavaConstructor}}.
}
\usage{
setJavaConverter(handler, matcher=-1, autoArray=T, description=NULL, userObject=NULL)
}
\arguments{
  \item{handler}{The name of a C routine that 
performs the conversion from the Java object
to the R object.
This is given the Java object, the class of that
object, the JNI environment and the 
element in the converter object
is to be called when the the \code{matcher}
determines that 
}
  \item{matcher}{
The name of a routine that is used
to determine whether this converter
can handle a specific object.
This can also be specified as an element
of the vector \code{.javaMatchFunctions},
either as (part of) a name of an element
or the integer value. These are then used
to identify one of the built-in converter matching functions.
}
  \item{autoArray}{A logical value indicating whether
this converter routine can be called element-wise
for an array of the class type it matches (TRUE) , or 
whether it wishes to defer the handling of such an array
to another converter or deal with it all in one step.
 }
  \item{description}{A string that describes the action of the converter
(e.g. the type of source Java class and target R object on which it operates).
This is stored with the internal converter and accessible to users via 
the \code{\link{getJavaConverterDescriptions}}.
 }
  \item{userObject}{
If the \code{matcher} argument identifies one of the built-in
matching routines (i.e. assignable from, instance of, equals)
this is interpreted as a Java class identifier.
That is either a class name (which is resoloved, and expanded as necessary, by Omegahat)
and used to parameterize the particular use matching routine.
}
  \item{register}{a logical value indicating whether this call
should also notify Java that the specified class (i.e. that given
in \code{userObject} is convertible. This calls
\code{\link{setJavaConvertible}} with the 
class and matching mechanism specified for this function.
}
}

\value{
 This returns the expanded named of the class used to parameterize the 
matching function and the identifier for the matching 
function itself.
  \item{match}{the value passed to the C routine identifying the matching
function. This is either an element from \code{.javaMatchFunctions}
(hence a named integer) or a string identifying the C routine.}
  \item{class}{The name of the class used to parameterize the matching
function, if the latter is one of the built-in routines named in \code{.javaMatchFunctions}. 
The class name is resolved  by Omegahat and converted to use
`/' instead of `.' to separate the Java packages. This is so that it can be easily used in
the native C code.}
\item{index}{the position in the list into which this converter was added.
This is useful if we want to remove the converter at a later stage via
\code{\link{removeJavaConverter}}.}
\item{description}{the description argument passed to this function call.
As with the \code{index} field, this is useful when
we wish to remove the converter as it acts as an identifier for
the converter. See \code{\link{removeJavaConverter}.}
}
\references{\url{http://www.omegahat.org}, \url{http://www.javasoft.com}}
\author{Duncan Temple Lang}
\note{In the near future, we will re-establish the mechanism
for specifying R functions or closure instances for the 
handler and matcher.
This has become more complicated than intended and will
probably be restricted to work only for C routines.
All of the cases have not been tested entirely.
}

\seealso{
\code{\link{getJavaConverterDescriptions}}
\code{\link{getNumJavaConverters}}
\code{\link{setJavaConvertible}}
}

\examples{
 setJavaConverter(.RSJava.symbol("RealVariableConverter"),
                   matcher="AssignableFrom",
                   autoArray=T, description="Omegahat RealVariable to numeric vector",
                     userObject="RealVariable")
}
\keyword{Java}
\keyword{programming}
\keyword{interface}

