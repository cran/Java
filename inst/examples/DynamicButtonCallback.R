#
# This combines the dynamic compilation of a Java class
# so as to use an R object as an implementation of a Java interface
# and also the dynamic callback to an R function.
#

compiler <- .JavaConstructor("ForeignReferenceClassGenerator", "java.awt.event.ActionListener", "RButtonListener")
dyn <- .Java("__Evaluator","dynamicClassLoader")
.Java(dyn,"defineClass", compiler)

 btnCallback <- function() {
   n <- 0
   actionPerformed <- function(event) {
     n <<- n + 1
     print(event)
     print(n)
   }
   return(list(actionPerformed=actionPerformed, count =function() n))
 }


cb <- btnCallback()
ref <- foreignReference(cb,"btnCB")

l <- .JavaConstructor("RButtonListener", ref)

b <- .JavaConstructor("javax.swing.JButton","Click me")
f <- .JavaConstructor("GenericFrame",b,T)

.Java(b,"addActionListener", l)
