\name{file.choose}
\alias{file.choose}
\title{ Java Graphical Dialog for selecting a file.}
\description{
  Allows the user to select a file by browsing her directories
visually using  the JFileChooser in the Java Swing  package.
}
\usage{
file.choose(dir="")
}

\arguments{
 \item{dir}{ The directory to use as the starting point of the 
selection and browsing in the JFileChooser. See the Swing documentation for 
how this is handledr.}
}
\details{
  This creates a dialog window and waits for the user to
click on the Open or Cancel button. The user can navigate directories,
create new folders, provide new filters, etc. before comitting to 
a selection or cancellation. In the meantime, the R process
blocks, waiting for the user's action.
}
\value{
 The fully qualified name of the  selected file or
NULL if the dialog was cancelled by the user.
}
\references{Swing, Java, \url{http://www.omegahat.org}}
\author{Duncan Temple Lang}
\note{ This requires the Java Virtual Machine (version 2 or higher) }


\seealso{ \code{\link{.Java}}, \code{\link{.JavaConstructor}} }

\examples{
  file.choose()
  file.choose("~")
}
\keyword{Java}, 
\keyword{environment},
\keyword{IO}, 
\keyword{programming}
\keyword{interface}.
