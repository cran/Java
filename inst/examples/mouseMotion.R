 panel <- .JavaConstructor("JPanel")
 .Java(panel,"setLayout", .JavaConstructor("BorderLayout"))
 label <- .JavaConstructor("JLabel", "(   ,   )")
 comp <- .JavaConstructor("JPanel")
 .Java(panel,"add","North", label)
 .Java(panel,"add","Center", comp)
 .JavaConstructor("GenericFrame", panel, T)
 
 .Java(comp,"setBackground",.Java("Color", "red"))     

  compiler <- .JavaConstructor("ForeignReferenceClassGenerator", "java.awt.event.MouseMotionListener", "RMouseListener")
 .Java(.Java("__Evaluator","dynamicClassLoader"), "defineClass", compiler)
  mouseListener <- function(jlabel) {
    x <- 1
    y <- 1        
   mouseMoved <- function(ev) {
     x <<- .Java(ev, "getX")
     y <<- .Java(ev, "getY")         
     # print(c(x,y))
     .Java(jlabel, "setText", paste("(",x,", ",y,")", sep=""))
   }
   return(list(mouseMoved = mouseMoved,
               where=function(){c(x,y)}))
 }
 l <- mouseListener(label)    
 r <- foreignReference(l)
 .Java(comp,"addMouseMotionListener", .JavaConstructor("RMouseListener",r))




