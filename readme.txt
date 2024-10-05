Automatically generate a pure C binding for a C++ library.

Current features:
classes (that's it, I know)

TODO:
detect included headers, and attempt to connect them (eg. if you have two headers make the c bindings use eachother).  This could also be in the form of taking multiple header files as an input, and producing only one file as output.
For structs, automatically create default constructor (or juts port the struct if it's using only C types)
Enum members

MAYBE EVENTUALLY features:
detect the size of a class, and use char array padding to put them on the stack (optionally, as of course heap allocation will always be supported)
Rather than outputing a C header and C++ source, output C header and source, and predict c++ name mangling, call con/destructors etc...
