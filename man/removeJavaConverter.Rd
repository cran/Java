\name{removeJavaConverter}
\alias{removeJavaConverter}
\title{Removes a converter for R and Java objects in the R-Java interface.}
\description{
This manipulates the internal list of object converters
that control how objects are transferred from R to Java
and from Java to R.
It allows the R user to remove entries and control
how objects are converted.
}
\usage{
removeJavaConverter(id, fromJava=T)
}
\arguments{
  \item{id}{the index or position of the converter to be removed
in the specified converter list.}
  \item{fromJava}{logical value indicating which set of converters
on which we are operating: from Java to R (\code{TRUE})
or from R to Java (\code{FALSE}). The latter is currently
not implemented and awaits the next version which will use C++.}
}
\value{
  Returns a integer identifying the position 
in the list in which the converter was located.
This is a named vector and the name  is the description
of the converter. This allows one to ensure that you got
the correct one.
}
\references{\url{http://www.omegahat.org/RSJava}
\author{Duncan Temple Lang}

\seealso{
\code{\link{setJavaConverter}}
\code{\link{setJavaConvertible}}
the \code{.convert} argument of
\code{\link{.Java}} and \code{\link{.JavaConstructor}}
}

\examples{
   # remove the Constructor converter
 removeJavaConverter(3)


   # add a converter
 setJavaConverter(.RSJava.symbol("RealVariableConverter"),
                   matcher="AssignableFrom",
                   autoArray=T, description="Omegahat RealVariable to numeric vector",
                     userObject="RealVariable")

   # and remove it by specifying its description.
 removeJavaConverter("Omegahat RealVariable to numeric vector")

   # add the converter again
 cvt <- setJavaConverter(.RSJava.symbol("RealVariableConverter"),
                   matcher="AssignableFrom",
                   autoArray=T, description="Omegahat RealVariable to numeric vector",
                     userObject="RealVariable")

   # and remove it by specifying its position
   # which is given to us by the setJavaConverter call.
 removeJavaConverter(cvt$index)
}
\keyword{Java}
