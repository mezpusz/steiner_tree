Dual-ascent 2-approximation algorithm implementation for the [Steiner Tree Problem](https://en.wikipedia.org/wiki/Steiner_tree_problem). The implementation is based on papers of [Wong](https://link.springer.com/article/10.1007/BF02612335) and [Pajor et al](https://link.springer.com/article/10.1007/s12532-017-0123-4). The problem description, input format and examples can be found at the [SteinLib Testdata Library](http://steinlib.zib.de/steinlib.php).

## Requirements for Unix

cmake, make, clang, libc++-dev, libc++abi-dev

## Getting the test set

````bash
./get_testset.sh
````

## Building it locally

Linux/Mac:

````bash
./build_and_test.sh
````

Windows:

````bash
cmake . -G "Visual Studio <version>"
````

where <version> is your Visual Studio version (can be 14, 15, 16, etc.).
Then open the `.sln` file and build the project.

## Arguments

The only argument is an input file path which is required. The output file is written to ```<input_file>.out```.

## Input format

See [SteinLib format description](http://steinlib.zib.de/format.php).