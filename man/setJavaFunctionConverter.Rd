\name{setJavaFunctionConverter}
\alias{setJavaFunctionConverter}
\title{Registers a function to convert between R and Java objects}
\description{
This allows one to register two functions that are used to 
convert an object from Omegahat/Java to an R object.
One function (\code{match}) determines whether the
other function (\code{handler}) that actually performs the
computation is suitable for converting the target object.
The result of the \code{handler} function is an \R
object that represents the Java object being converted.
}
\usage{
setJavaFunctionConverter(handler, match, description=NULL, fromJava=T)
}
\arguments{
  \item{handler}{a function that takes two arguments: a reference to the Java
object to be converted and the name of is Java class.
This function should return the converted value or the reference
to the Java object if it cannot convert it meaningfully.}
  \item{match}{a function that deterines whether the associated
\code{handler} function is appropriate for converting the target
Java object. This function should expect two arguments: a reference to the Java
object and its class name. It \textit{must} return a logical value
indicating whether the \code{handler} is capable of converting
the Java object.}
  \item{description}{a descripion that is stored internally with the converter
and accessible to users via the function \code{\link{getJavaConverterDescriptions}}.}
  \item{fromJava}{a logical value indicating whether the functions
are intended for converting from Java to R or vice-versa.
Currently, the R to Java mechanism is not implemented.
}
}
\value{
 An object of class \texttt{"JavaFunctionConverter"}
with fields
 \item{index}{the position of this converter in the internal list of converters.
This is a useful identifier for removing the converter.}
 \item{description}{the value of the \code{description} argument.
This is also a useful and preferred identifier for removing the converter.}
 \item{handler}{the value of the \code{handler} argument.}
 \item{match}{the value of the \code{match} argument.}
}
\references{\url{http://www.omegahat.org/RSJava/index.html}
\author{Duncan Temple Lang}
\note{The R to Java converter mechanism will be added in the next
release.}

\seealso{
\code{\link{setJavaConverter}}
\code{\link{setJavaConvertible}}
\code{\link{.Java}}
\code{\link{.JavaConstructor}}
}

\examples{
  # this must be run wit the ROmegahatExamples.jar
  # file in the classpath (e.g. 
  #     .JavaInit(list(classPath=system.file("org/omegahat/Jars/ROmegahatExamples.jar")))
  # so as to be able to find RealVariable!

 if(!is.null(.Java("__Evaluator", "expandClassName", "RealVariable"))) {
   cvt <- setJavaFunctionConverter(function(jobj,className) {
                                        .Java(jobj, "getValues")
                                   },
                           function(jobj, className) {
                              return(className == "org.omegahat.DataStructures.Data.RealVariable")
                            },
                          "Omegahat RealVariable to numeric vector")

   setJavaConvertible("RealVariable")
   .JavaConstructor("RealVariable", rnorm(10))

     # now unregister the converter
   setJavaConvertible("RealVariable", F)
   removeJavaConverter(cvt$index)
 }
}
\keyword{Java}
\keyword{interface}
