file.choose <- 
function(dir="")
{
 if(!missing(dir))
   dir <- file.expand(dir)

 chooser <- .JavaConstructor("javax.swing.JFileChooser", as.character(dir))
 f       <- .JavaConstructor("javax.swing.JFrame")     
 if(.Java(chooser,"showOpenDialog", f) == 0) {
   file <- .Java(chooser,"getSelectedFile")
   return(.Java(file,"toString"))
 }

 invisible(return(NULL))
}

f <- file.choose()
