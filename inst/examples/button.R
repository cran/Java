# This is currently deactivated.
# It relies on implicit conversion and compilation of new
# Java classes. It can be reactivated when we understand how
# users want to employ it.

  b <- .JavaConstructor("javax.swing.JButton","Click me")
  win <- .JavaConstructor("GenericFrame", b, T)
  cb1 <- function(window) {
              actionPerformed <- function(ev) {
                         .Java(window,"setVisible", F)
              }
           return(actionPerformed)
         }

  cb2 <- function() {
            n <- 0
             actionPerformed <- function(event) {
                                     n <<- n + 1
                                     print(event)
                                     print(.Java(event,"getActionCommand"))
                                     print(n)
                                 }
             return(actionPerformed)
          }

stop("This functionality has been deactivated. See DynamicButtonCallbac.R instead.")
  .Java(b,"addActionListener", cb2(),.sigs="org/omegahat/R/Java/RFunctionListener")
