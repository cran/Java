 handler <- function() {
   n <- 0
   actionPerformed <- function(event) {
     n <<- n + 1
     print(event)
     print(n)
   }
   return(actionPerformed)
 }


cb <- handler()
ref <- foreignReference(cb,"btnCB")

l <- .JavaConstructor("org.omegahat.R.Java.RManualFunctionActionListener", ref)

b <- .JavaConstructor("javax.swing.JButton","Click me")
f <- .JavaConstructor("GenericFrame",b,T)

.Java(b,"addActionListener", l)
