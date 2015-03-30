# C++ Project Skeleton
This project skeleton was created for my own person needs for C++ projects. More specifically, I needed it for some school projects. The skeleton comes with a whole host of folders as well as a ready-to-use Makefile. You just need to set a few values in the Makefile corresponding to your target's name (executable) and you're ready to go. There are targets for the actual executable, one for testing, and one for experimental. The rationale behind the targets is as follows:
- The actual executable target exists to build your actual project, pretty self-explanatory
- The test target exists so you can write unit tests using a C++ framework (such as Boost Unit Test Framework) and build and run the test suites.
- The experimentals target exists so you can have a sandbox to play around in and try out without muddying up your production or testing code.

If there are any targets you won't use, just delete that segment in the Makefile, or don't call that target with the `make` command. Each of the targets are commented thoroughly for your understanding of what each oen does.

I've also set up flags for libraries and headers that you can set for your project's particular needs.
