set -e

LLVM_INSTALL_DIR="/home/canesche/git/llvm-project/build/" # </path/to/llvm/>, if you use binary installation, you can leave empty this string 
LLVM_OPT="/home/canesche/git/llvm-project/build/bin/opt" # </path/to/opt>
CLANG="/home/canesche/git/llvm-project/build/bin/clang" # </path/to/clang>
PATH_LIB="build/lib/libRangeAnalysis.so"

mkdir -p build
mkdir -p results

echo "Building the Range Analysis pass"
cmake -DLLVM_INSTALL_DIR=$LLVM_INSTALL_DIR -G "Unix Makefiles" -B build/ .
cd build
cmake --build .
cd ..

echo ""
echo "Benchmarks toys"

EXAMPLE=benchmarks/test3

mkdir -p "results/test2"

echo "Executing the pass for bench: "$EXAMPLE
# analysis pass
$CLANG -fno-discard-value-names -Xclang -disable-O0-optnone -c -emit-llvm $EXAMPLE".c" -o $EXAMPLE".bc"
$LLVM_OPT -instnamer -mem2reg -break-crit-edges $EXAMPLE".bc" -o $EXAMPLE".ll"

$LLVM_OPT -load $PATH_LIB -vssa $EXAMPLE".ll" -o $EXAMPLE".vssa.ll"
$LLVM_OPT -stats -load $PATH_LIB -dead-code-elimination $EXAMPLE".vssa.ll" -S -o=$EXAMPLE".sol.ll"

$LLVM_OPT -dot-cfg $EXAMPLE".vssa.ll" -disable-output
dot -Tpdf -o function_antes.pdf .foo.dot 

opt -dce -stats $EXAMPLE".ll" -o $EXAMPLE".dce.ll"
opt -dot-cfg $EXAMPLE".dce.ll" -disable-output
dot -Tpdf -o function_dce.pdf .foo.dot 

$LLVM_OPT -dot-cfg $EXAMPLE".sol.ll" -disable-output
dot -Tpdf -o function_depois.pdf .foo.dot 

echo "clean .ll and .bc"
rm benchmarks/*.ll
rm benchmarks/*.bc