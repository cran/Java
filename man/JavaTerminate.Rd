\name{.JavaTerminate}
\alias{.JavaTerminate}
\title{Terminates the Java Virtual Machine}
\description{
Unloads the Java Virtual Machine, releasing its 
resources and terminating the Omegahat session.
\textit{Once the JVM is terminated, it cannot
be restarted within this R session.}
}
\usage{
.JavaTerminate()
}
\details{
This just calls the internal routine which
notifies the JVM that it should terminate.
Exactly how this action is performed  depends
on the current state of the JVM and the threads that
are active.
}
\value{
 \code{T} indicating that the JVM is
terminated and should not be used.
}
\references{\url{http://www.omegahat.org/RSJava}}
\author{Duncan Temple Lang}

\seealso{
\code{\link{.JavaInit}}
}

\examples{
\dontrun{
   # active the JVM only to find out what
   # version of Java it supports. 
   # No further activity can take place in the 
   # Java session.
   #
 .JavaInit()
 jversion <- .Java("System", "getProperty", "java.version")
 .JavaTerminate()
}
}
\keyword{Java}

