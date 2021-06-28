//
// Created by Smile on 2021/6/27.
//

#ifndef LLVM_ENCODEFUNCNAME_H
#define LLVM_ENCODEFUNCNAME_H
// LLVM include
#include "llvm/Pass.h"

namespace llvm {
Pass* createEncodeFuncName();
} // namespace llvm

#endif // LLVM_ENCODEFUNCNAME_H