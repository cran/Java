\name{.javaConfig}
\alias{.javaConfig}
\non_function{}
\title{The default options for initializing the Java Virtual Machine}
\description{
 This is a list containing elements for
\item{classpath elements}{in which collections of Java classes
are found}
\item{system properties}{name-value pairs used to customize the JVM's environment,
the Omegahat classes and any other classes that read these properties}
\item{library path}{directories in which Java can find shared libraries}.
The values in this object are created during the configuration
of the package so that they refer to files in the directories
into which the package is installed.
}
\keyword{Java}