#
# This retrieves the name of the superclass of an object.
#
# First we create a Hashtable object.
# Then we inline
 o <- .JavaConstructor("Hashtable")
 .Java(.Java( .Java(o, "getClass"), "getSuperclass"), "getName")

# The result is java.util.Dictionary.