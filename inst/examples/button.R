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

  .Java(b,"addActionListener", cb2(),.sigs="org/omegahat/R/Java/RFunctionListener")
