//
// Created by Smile on 2021/6/28.
//

#ifndef LLVM_STRINGENCRYPTION_H
#define LLVM_STRINGENCRYPTION_H
#include "llvm/Pass.h"
namespace llvm {
ModulePass *createStringEncryptionPass();
} // namespace llvm
#endif // LLVM_STRINGENCRYPTION_H