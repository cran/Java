gen <- .JavaConstructor("ForeignReferenceClassGenerator", "java.awt.event.ActionListener", "RActionListener")
.Java(gen, "superClassName", "org.omegahat.R.Java.RForeignReference")
.Java(gen,"make")
.Java(gen,"write", .JavaConstructor("File", "/tmp/RActionListener.class"))