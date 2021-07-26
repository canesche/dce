//===----------------------- DeadCodeElimination.cpp ----------------------===//
//=== Performs a dead code elimination of the instruction of the function -===//
//
//					 The LLVM Compiler Infrastructure
//
// This file is distributed under the MIT Open Source
// License. See LICENSE.TXT for details.
//
// Copyright (C) 2021 Michael Canesche
//                    Caio Vinícius Raposo Ribeiro
//                    Alexander Thomas Mol Holquist
//      
//===----------------------------------------------------------------------===//
// This file contains a pass that performs dead code elimination (dce) analysis. 
// The objective of dce is to eliminate instruction and block considered dead.
// Ideally, dce should eliminate all dead code, however it is difficult to 
// determine everything that must be eliminated. Thus our pass is at least 
// better than a naive elimination. Eliminating where it is sure the 
// instruction or block is considered dead. 
//
//===----------------------------------------------------------------------===//

#ifndef _DEADE_CODE_ELIMINATION_H
#define _DEADE_CODE_ELIMINATION_H

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/User.h"
#include "llvm/PassAnalysisSupport.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "RangeAnalysis.h"
#include <queue>
#include <vector>
#include <map>

using namespace std;
using namespace llvm;

namespace {
    class RADeadCodeElimination : public llvm::FunctionPass {
        public:
            static char ID;
            RADeadCodeElimination() : FunctionPass(ID) { }
            ~RADeadCodeElimination();
            void getAnalysisUsage(AnalysisUsage &AU) const;
            bool eliminate_instructions(Function &F);
            bool eliminate_branch(Function &F);
            bool eliminate_phi_nodes(Function &F);
            bool eliminate_unconditional_branch(Function &F);
            bool runOnFunction(Function &);
            bool runOnBasicBlock(Function::iterator &);
            bool solveICmpInstruction(ICmpInst* I);
            bool solveBinaryInst(BasicBlock::iterator I);
            bool verify_equal(Range r1, Range r2);
            bool send_to_delete_instruction(Instruction *I);
            bool send_to_delete_branch(Instruction *I, int operand); 
        private:
            InterProceduralRA<Cousot>* ra;
            list<Instruction*> dead_instr;
            list<pair<Instruction*, Value *>> dead_op_bin;
            queue<pair<Instruction*, int>> dead_branch;
            map<string, string> map_label;
        };
}


#endif