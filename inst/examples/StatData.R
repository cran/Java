#
# This example shows how we can use a Java class 
# to perform computations and get the results back 
# to R. This particular case is a class (StatDataURL)
# that reads the contents of a URL line-by-line
# and provides a mechanism for returning those
# lines as an array of Java Strings. This is
# automatically converted to a character vector.

 u <- .JavaConstructor("StatDataURL", "http://www.omegahat.org/Scripts/AwtButton")
 els <- paste(.Java(u, "getContents"),collapse="\n")
