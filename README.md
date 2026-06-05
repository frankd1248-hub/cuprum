# Ferrum

I have had varying levels of success with writing interpreters, so I
decided to try out a compiler to assembly!

This compiler lowers source code to x86-64 intel syntax assembly
for Linux. As such, for the forseeable future this will only be 
built for Linux.

I am using this project to learn more about parsing techniques,
building a versatile IR, and generating assembly code. It has been
teaching me a lot about assembly in general as well.

Right now, the language is not even Turing-complete, but the addition of loops
would make it so. Preferably, a string type would also be helpful.

If you have questions about how I chose the name "Ferrum",
it's Latin for *iron*, which fits well because assembly "runs" on
bare metal, which is iron most of the time.

I could have chosen Stannum, Aurum, Argentum, or Cuprum, but Ferrum 
sounded the best at the time.

For the (currently) best interpreter I have, check out:

[Burrito](https://github.com/frankd1248-hub/Burrito)