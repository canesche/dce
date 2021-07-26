# DCE Range Analyses

Dead Code Elimination (DCE) using Range Analysis

# Students

[Michael Canesche - 2021660731](https://canesche.github.io/)

[Caio Vinícius Raposo Ribeiro - 2021679084](https://github.com/caioraposo)

[Alexander Thomas Mol Holquist - 2018066255](https://github.com/Yowgf)

Instructor: [Fernando Magno Quintão Pereira](https://github.com/pronesto)

# LLVM Version 

LLVM version 10.0.0

OS: Ubuntu Linux 20.04.2

# Config

First, let us define a set of path variables:

There is a shell script file 'run.sh'. Inside it you can set this information:

```
$> LLVM_INSTALL_DIR=</path/to/llvm/>
$> LLVM_OPT=</path/to/opt>
$> CLANG=</path/to/clang>
```

For run all fine, It's necessary the use of LLVM build from the git.

# How to use

To compile and run the program, execute the "run.sh" shell script.

# Results

All dot files generated are shown on the "results" file. With the dot before and after executing the pass. The number of instructions and block basic elimination can be viewed on executing the "run.sh" shell script.

# Observation

We use the name of the function ‘foo’ only to create the dot cfg using the pass of “-dot-cfg” automatically.