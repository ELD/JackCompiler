# JackCompiler Project 10/11 README

1. Eric Dattore
2. No one else assisted me with this assignment
3. Long story short - too long. Serious estimate: 10-12 hours
4. **Build Instructions and Usage**: Simply typing `make -j[N]` where `-j[N]` is the number of threads you want to build with (the more threads, the faster it compiles; the Makefile supports incremental builds) builds the core project. Once the project has been built, the executable will be placed in the `target/` directory with a name of `jackcompiler`. In order to run the VM simply type `./target/jackcompiler [filename|path]`. The VM will accept any path anywhere in the filesystem and will place the translated `.vm` file in the root directory of this project. For example, if you invoked the compiler as such: `./target/jackcompiler ../10/ArrayTest/` the resulting `.vm` file(s) would be created as `./Main.xml`.

5. **Dependencies**: The project depends on the Boost.Filesystem library, the Boost.Tokenizer library, and miscellaneous Boost libraries. It's best to install the entire suite of Boost libraries. This requires the installation of the Boost libraries. In order to link correctly with the Boost libraries, the Makefile assumes you have a $BOOST_LIBS environment variable  pointing to the root directory of the Boost library installation. For example, on my machine it’s set to `/usr/local/Cellar/boost/1.57.0`.

6. **Build Notes**: The project was developed on a 15” Retina Macbook Pro running Mac OS X Yosemite and compiled with Clang 3.6.
