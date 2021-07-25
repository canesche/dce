#include "DeadCodeElimination.h"

#define DEBUG_TYPE "dce"

STATISTIC(InstructionsEliminated, "Number of instructions eliminated");
STATISTIC(BasicBlocksEliminated,  "Number of basic blocks entirely eliminated");

namespace {

    RADeadCodeElimination::~RADeadCodeElimination() {}
      
    bool RADeadCodeElimination::runOnFunction(Function &F) {

        ra = &getAnalysis<InterProceduralRA<Cousot>>();
        InstructionsEliminated = 0;
        BasicBlocksEliminated = 0;

        for (Function::iterator bb = F.begin(), bbEnd = F.end(); bb != bbEnd; ++bb) {
            runOnBasicBlock(bb);
        }

        // eliminate dead instructions 
        eliminate_instructions();

        // eliminate dead branch 
        eliminate_branch(F);

        return false;
    }

    bool RADeadCodeElimination::runOnBasicBlock(Function::iterator &bb) {
        for (BasicBlock::iterator I = bb->begin(), IEnd = bb->end(); I != IEnd; ++I) {
            
            const Value *v = &(*I);
            Range r = ra->getRange(v);
            
            if (isInstructionTriviallyDead(&*I)) {
                dead_instr.push(&*I);
            } else if (ICmpInst *icmpInst = dyn_cast<ICmpInst>(&*I)) {
                if (solveICmpInstruction(icmpInst)) {
                    dead_instr.push(icmpInst);
                }
            } else if (isa<BinaryOperator>(I)) {
                solveBinaryInst(I);
            }

            if (!r.isUnknown()) {
                r.print(errs());
                errs() << *I << "\n";
            }
        }
        return false;
    }

    bool RADeadCodeElimination::eliminate_phi_nodes(Function &F) {
        queue<BasicBlock::iterator> Q;
        bool change = false;
        for (Function::iterator bb = F.begin(), bbEnd = F.end(); bb != bbEnd; ++bb) {
            for (BasicBlock::iterator I = bb->begin(), IEnd = bb->end(); I != IEnd; ++I) {
                if (PHINode *phi = dyn_cast<PHINode>(I)) {
                    if (phi->getNumOperands() == 1) {
                        I->replaceAllUsesWith(phi->getOperand(0));
                        Q.push(I);
                        change = true;
                    }
                }
            }
        }
        // remove instruction 
        while (!Q.empty()) {
            BasicBlock::iterator I = Q.front();
            Q.pop();
            if (!I->use_empty()) {
                I->replaceAllUsesWith(UndefValue::get(I->getType()));
            }
            I->eraseFromParent();
            ++InstructionsEliminated;
        }
        
        return change;
    }

    bool RADeadCodeElimination::eliminate_unconditional_branch(Function &F) {
        bool change = false;
        int count;

        //errs() << "ELIMINARION UNCONDITIONAL\n"; 

        queue<BasicBlock*> Q;
        for (BasicBlock &bb : F) {
            count = 0;
            
            for (BasicBlock::iterator I = bb.begin(), IEnd = bb.end(); I != IEnd; ++I) {
                count++;
            }
            Instruction *br = bb.getTerminator();
            if (count == 1 && br->getNumSuccessors() > 0 && bb.getSinglePredecessor()) {
                Q.push(&bb);
            }
            change = true;
        }

        while (!Q.empty()) {
            BasicBlock *bb = Q.front();
            BasicBlock *bb_before = bb->getSinglePredecessor();
            Q.pop();

            for (BasicBlock::iterator I = bb->begin(), IEnd = bb->end(); I != IEnd; ++I) {
                ++InstructionsEliminated;
            }

            BranchInst *Old = dyn_cast<BranchInst>((bb_before)->getTerminator());
            int j = 0;
            for (int i = 0; i < Old->getNumSuccessors(); ++i) {
                if (Old->getSuccessor(i) == &(*bb)) {
                    j = i; 
                    break;
                }
            } 

            BasicBlock *bb_after = bb->getTerminator()->getSuccessor(0); 

            // update phi-node
            for (BasicBlock::iterator I = bb_after->begin(), IEnd = bb_after->end(); I != IEnd; ++I) {
                if (PHINode *phi = dyn_cast<PHINode>(I)) {
                    for (int i = 0; i < phi->getNumOperands(); ++i) {
                        if (phi->getIncomingBlock(i) == bb) {
                            phi->setIncomingBlock(i, bb_before);
                        }
                    } 
                }
            }

            bb_before->getTerminator()->setSuccessor(j, bb->getTerminator()->getSuccessor(0));

            if (!bb->getTerminator()->use_empty()) {
                bb->getTerminator()->replaceAllUsesWith(UndefValue::get(bb->getTerminator()->getType()));
            }
            bb->getTerminator()->eraseFromParent();
            
            ++BasicBlocksEliminated;
            EliminateUnreachableBlocks(F, nullptr, true);
        }

        return change;
    }

    bool RADeadCodeElimination::solveBinaryInst(BasicBlock::iterator I) {

        Range r1 = ra->getRange(I->getOperand(0));
        Range r2 = ra->getRange(I->getOperand(1));

        switch (I->getOpcode()) {
            case Instruction::And:
            case Instruction::Or:
            case Instruction::Xor:
                if (isa<ConstantInt>(I->getOperand(1))) {
                    ConstantInt *c = dyn_cast<ConstantInt>(I->getOperand(1));
                    //errs() << "value: " << c->getZExtValue() << "\n";
                    if (r1.getUpper().sle(r2.getLower())) {
                        // replace the value
                        I->replaceAllUsesWith(I->getOperand(0));
                        dead_instr.push(&*I);
                    }
                }
                break;
            default:
                break;
        }
        return false;
    }

    void RADeadCodeElimination::eliminate_branch(Function &F) {
        bool change = false;
        
        Instruction* I;
        int succ;
        while (!dead_branch.empty()) {
            change = true;

            I = dead_branch.front().first;
            succ = dead_branch.front().second;
            dead_branch.pop();

            BranchInst *Old = dyn_cast<BranchInst>(I);
            BranchInst *New = BranchInst::Create(Old->getSuccessor((succ+1)%2));
            
            BasicBlock *BB = &(*Old->getSuccessor(succ));
            queue<BasicBlock*> Q;
            Q.push(BB);

            while (!Q.empty()) {
                BB = Q.front();
                Q.pop();
                if (BasicBlock *PredBB = BB->getUniquePredecessor()) {
                    // counter the number of instructions will be remove by unlink in the block
                    for (BasicBlock::iterator ii = BB->begin(), IEnd = BB->end(); ii != IEnd; ++ii) {
                        InstructionsEliminated++;
                    }
                    BasicBlocksEliminated++;

                    Instruction *br = BB->getTerminator();
                    for (int i = 0; i < br->getNumSuccessors(); ++i)
                        Q.push(&(*br->getSuccessor(i)));
                }
            }
            // replace the old branch to new
            ReplaceInstWithInst(Old, New);
        }

        if (change) {
            // eliminate unreachable block from entry
            EliminateUnreachableBlocks(F, nullptr, true);

            // eliminate PHI-nodes with one operand
            eliminate_phi_nodes(F);
            
            // remove simple blocks with only have unconditional branch 
            eliminate_unconditional_branch(F);
        }
    }

    void RADeadCodeElimination::eliminate_instructions() {
        
        Instruction *I;
        int del_successor;
        while (!dead_instr.empty()) {
            I = dead_instr.front();
            dead_instr.pop();
            if (!I->use_empty()) {
                I->replaceAllUsesWith(UndefValue::get(I->getType()));
            }
            I->eraseFromParent();
            InstructionsEliminated++;
        }
        
    }

    bool RADeadCodeElimination::solveICmpInstruction(ICmpInst* I) {
        
        Range r1 = ra->getRange(I->getOperand(0));
        Range r2 = ra->getRange(I->getOperand(1));
        Instruction *branch;

        errs() << *I << "\n";
        errs() << "[" << r1.getLower() << "," << r1.getUpper() << "] - [" << r2.getLower() << ", " << r2.getUpper() << "]\n";

        switch (I->getPredicate()){
            case CmpInst::ICMP_SLT: // r1 < r2
                // if always true, remove the false condition
                if (r1.getUpper().slt(r2.getLower())) {
                    dead_branch.push(make_pair(I->getParent()->getTerminator(), 1));
                    return true;
                } 
                // r1.1 >= r2.2 means always false
                if (r1.getLower().sge(r2.getUpper())) { 
                    dead_branch.push(make_pair(I->getParent()->getTerminator(), 0));
                    return true;
                }
                break;
            case CmpInst::ICMP_SLE: // r1 <= r2
                // if always true, remove the false condition
                if (r1.getUpper().sle(r2.getLower())) {
                    dead_branch.push(make_pair(I->getParent()->getTerminator(), 1));
                    return true;
                } 
                // r1.1 > r2.2 means always false
                if (r1.getLower().sgt(r2.getUpper()))
                    dead_branch.push(make_pair(I->getParent()->getTerminator(), 0));
                    return true;
                break;
            case CmpInst::ICMP_SGT: // r1 > r2
                // if always true, remove the false condition
                if (r1.getLower().sgt(r2.getUpper())) {
                    dead_branch.push(make_pair(I->getParent()->getTerminator(), 1));
                    return true;
                } 
                // r2.2 >= r1.1 means always false 
                if (r1.getUpper().sle(r2.getLower())) { 
                    dead_branch.push(make_pair(I->getParent()->getTerminator(), 0));
                    return true;
                }
                break;
            case CmpInst::ICMP_SGE: // r1 >= r2
                // if always true, remove the false condition
                if (r1.getLower().sge(r2.getUpper())) {
                    dead_branch.push(make_pair(I->getParent()->getTerminator(), 1));
                    return true;
                }
                // r2.2 > r1.1 means always false
                if (r1.getUpper().slt(r2.getLower())) 
                    dead_branch.push(make_pair(I->getParent()->getTerminator(), 0));
                    return true;
                break;
            case CmpInst::ICMP_EQ: // r1 == r2 
                // if always true, remove the false condition
                if (r1 == r2) {
                    dead_branch.push(make_pair(I->getParent()->getTerminator(), 1));
                    return true;
                } 
                // r1.2 < r2.1 || r2.2 < r1.1 means always false
                if (r1.getUpper().slt(r2.getLower()) || r2.getUpper().slt(r1.getLower())) { 
                    dead_branch.push(make_pair(I->getParent()->getTerminator(), 0));
                    return true;
                }
                break;
            case CmpInst::ICMP_NE: // r1 != r2
                // if always true, remove the false condition
                if (r1 != r2) { 
                    dead_branch.push(make_pair(I->getParent()->getTerminator(), 1)); 
                    return true;
                }
                // r1.2 == r2.2 && r1.1 == r1.2 && r2.1 == r2.2 means always false
                if (r1.getUpper().eq(r2.getUpper()) && r1.getLower().eq(r1.getUpper()) &&
                    r2.getLower().eq(r2.getUpper())) {
                    dead_branch.push(make_pair(I->getParent()->getTerminator(), 0));
                    return true;
                } 
                break;
            default:
                return false;
        }
        return false;
    }

    void RADeadCodeElimination::getAnalysisUsage(AnalysisUsage &AU) const {
        AU.setPreservesAll();
        AU.addRequired<InterProceduralRA<Cousot>>();
    }
}

char RADeadCodeElimination::ID = 0;
static RegisterPass<RADeadCodeElimination> X("dead-code-elimination", "A client that uses dead code elimination");
