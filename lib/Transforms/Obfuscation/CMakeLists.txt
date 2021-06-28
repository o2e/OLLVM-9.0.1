add_llvm_library(LLVMObfuscation
        CryptoUtils.cpp
        Substitution.cpp
        BogusControlFlow.cpp
        Utils.cpp
        SplitBasicBlocks.cpp
        Flattening.cpp
        EncodeFuncName.cpp
        StringEncryption.cpp
        )

add_dependencies(LLVMObfuscation intrinsics_gen)
