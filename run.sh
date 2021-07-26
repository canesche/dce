set -e

# CONFIG WHERE YOUR LLVM INSTALL, REQUIRE VERSION COMPILE FROM GIT
LLVM_INSTALL_DIR="/home/canesche/git/llvm-project/build/" # </path/to/llvm/>

# DON'T NEED CHANGE BELOW IF YOU COMPILE FROM GIT
LLVM_OPT=$LLVM_INSTALL_DIR"/bin/opt" # </path/to/opt>
CLANG=$LLVM_INSTALL_DIR"/bin/clang" # </path/to/clang>

#ADDING BENCHMARKS WITH SPACE BETWEEN WORDS
BENCH=(
	test1 test2 test3 test4 
	test5 test6 test7 test8 
	test9 test10 test11 test12 
	test13 test14 test15 test16
)

# PATH LIBRARY
PATH_LIB="build/lib/libRangeAnalysis.so"

mkdir -p build
mkdir -p results

echo "Building the Range Analysis pass"
cmake -DLLVM_INSTALL_DIR=$LLVM_INSTALL_DIR -G "Unix Makefiles" -B build/ .
cd build
cmake --build .
cd ..

echo ""
echo "Benchmarks"

for ((i = 0; i < ${#BENCH[@]}; i++)); do

	EXAMPLE=benchmarks/${BENCH[i]}
	DIR_RESULT="results/"${BENCH[i]}

	mkdir -p $DIR_RESULT

	echo ""
	echo "===-------------------------------------------------------------------------==="
	echo "Executing the pass for bench: "$EXAMPLE

	# analysis pass
	$CLANG -Wno-everything -fno-discard-value-names -Xclang -disable-O0-optnone -c -emit-llvm $EXAMPLE".c" -o $EXAMPLE".bc"
	$LLVM_OPT -instnamer -mem2reg -break-crit-edges $EXAMPLE".bc" -o $EXAMPLE".ll"
	   
	$LLVM_OPT -load $PATH_LIB -vssa $EXAMPLE".ll" -o $EXAMPLE".vssa.ll"

	# generate dot-cfg before
	$LLVM_OPT -dot-cfg $EXAMPLE".vssa.ll" -disable-output
	dot -Tpdf .foo.dot -o $DIR_RESULT/foo_antes.pdf 

	$LLVM_OPT -stats -load $PATH_LIB -dead-code-elimination $EXAMPLE".vssa.ll" -S -o=$EXAMPLE".sol.ll"
	
	# generate dot-cfg after
	$LLVM_OPT -dot-cfg $EXAMPLE".sol.ll" -disable-output
	dot -Tpdf .foo.dot -o $DIR_RESULT/foo_depois.pdf
	echo "-------------------------------------------------------------------------------"
done

echo "clean .ll and .bc"
rm benchmarks/*.ll benchmarks/*.bc
