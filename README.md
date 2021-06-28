O-LLVM 9.0.1
================================
####适配 NDK-21.3.6528147(ndk-r21d)
####编译命令

``` bash
cd OLLVM-9.0.1
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_CREATE_XCODE_TOOLCHAIN=ON ../
make -j8
```
####复制文件
                
+ 打开路径 `OLLVM-9.0.1/build/bin/` 找到 `clang-9` `clang` `clang-format` `clang++` 四个文件
+ 复制文件到ndk目录替换ndk自带的这四个文件 `/Users/Smile/Library/Android/sdk/ndk/21.3.6528147/toolchains/llvm/prebuilt/darwin-x86_64/bin/`
+ 上面的路径是macOS系统的 系统不一样 `darwin-x86_64`的名字不同

####注意事项
+ 项目在macOS 10.15.7 xcode 12.3 的环境上测试成功 Linux系统未测试理论通用 Windows系统有问题 需要修改代码才能用(手上没有Windows系统所以未实践)
+ Windows需要修改的代码[随机数问题](https://github.com/obfuscator-llvm/obfuscator/pull/89/commits/8b93673403f75d5766093469c7532bb9752e2d2d)、[头文件问题](https://github.com/obfuscator-llvm/obfuscator/pull/90/commits/f7003bb6199c2da345d749ed62503597f4198d26)
+ aarch64(包括MacBook M1) 上需要修改的问题 [修改头文件](https://github.com/obfuscator-llvm/obfuscator/pull/160)
+ `fla` 如果遇到奇怪问题可以看看这个[代码修改内容](https://github.com/obfuscator-llvm/obfuscator/pull/41/commits/6032fb15b49d28c2147bea997c47fab22d7f6cb8)

#### Android Cmake 食用方式
```
android {
		ndkVersion = "21.3.6528147"
		defaultConfig {
			externalNativeBuild {
				cmake {
					cppFlags "-std=c++14", "-frtti", "-fexceptions", "-DANDROID_TOOLCHAIN=clang", "-ffunction-sections", "-fdata-sections",
							"-mllvm", "-sub", "-mllvm", "-sub_loop=3",
							"-mllvm", "-fla",
							"-mllvm", "-split", "-mllvm", "-split_num=3",
							"-mllvm", "-bcf", "-mllvm", "-bcf_loop=3", "-mllvm", "-bcf_prob=40",
							"-mllvm", "-sobf", "-mllvm", "-aesSeed=0xada46ab5da824b96a18409c49dc91dc3"

					arguments "-DANDROID_TOOLCHAIN=clang",
							"-DANDROID_STL=c++_shared",
							"-DANDROID_CPP_FEATURES=rtti exceptions"
				}
				ndk {
					abiFilters 'armeabi-v7a', 'arm64-v8a', "x86", "x86_64"
				}
			}
		}
}
```
+ `sub_loop`、`split_num`、`bcf_loop`这些是弄几遍 `bcf_prob`是这代码每个基本块有40%的概率进行BCF混淆(0-100) 命令具体是干什么的自己搜索引擎去了解 懒得挨个写了
+ `"-std=c++14", "-frtti", "-fexceptions", "-DANDROID_TOOLCHAIN=clang", "-ffunction-sections", "-fdata-sections"` 这个和ollvm无关 什么意思自己查
####补充知识
+ Android 中的ABI `armeabi-v7a`、`x86` 是32位的，`arm64-v8a`是64位的， `x86_64`这个是两开花的？远古时代的`armeabi`已经淘汰了，ndk不再支持。
