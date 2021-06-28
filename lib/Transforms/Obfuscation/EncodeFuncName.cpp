//
// Created by Smile on 2021/6/27.
//

#include "llvm/Transforms/Obfuscation/EncodeFuncName.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
struct EncodeFuncName : public FunctionPass {
  static char ID; // Pass identification, replacement for typeid
  EncodeFuncName() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    if (F.getName().compare("JNI_OnLoad") != 0
        && F.getName().compare("main") != 0
        && !F.getName().contains("JNI_On")
        && !F.getName().contains("JavaVM")
        && !F.getName().contains("JNIEnv")
        && !F.getName().contains("pass_")
        && !F.getName().contains("__clang")
        && !F.getName().contains("__ndk")
        && !F.getName().contains("basic_")
        ) { //过滤的JNI_OnLoad方法 其他jni方法自己动态注册
      errs() << "EncodeFuncName: " << F.getName() << " -> ";
      // MD5的使用就抄llvm源码中的使用
      llvm::MD5 Hasher;
      llvm::MD5::MD5Result Hash;
      Hasher.update("antiy_");
      Hasher.update(F.getName());
      SmallString<32> HexString;
      llvm::MD5::stringifyResult(Hash, HexString);
      F.setName(HexString); //将加密后的函数名字设置回去
      errs().write_escaped(F.getName()) << '\n';
    }
    SmallString<32> smallString;
    return false;
  }
};
} // namespace

char EncodeFuncName::ID = 0;
static RegisterPass<EncodeFuncName> X("encode", "encode function name Pass");

Pass *llvm::createEncodeFuncName() {
  return new EncodeFuncName();
}

// static llvm::RegisterStandardPasses Y(
//     llvm::PassManagerBuilder::EP_EarlyAsPossible,
//     [](const llvm::PassManagerBuilder &Builder,
//        llvm::legacy::PassManagerBase &PM) { PM.add(new EncodeFuncName()); });