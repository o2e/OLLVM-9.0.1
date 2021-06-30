O-LLVM 9.0.1
================================
#### 适配 NDK-21.3.6528147(ndk-r21d)
#### 编译命令

``` bash
cd OLLVM-9.0.1
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_CREATE_XCODE_TOOLCHAIN=ON ../
make -j8 #8是你的CPU线程数量 比如8核16线程可以写 -j16 
```
#### 整合到NDK
                
+ 打开路径 `OLLVM-9.0.1/build/bin/` 找到 `clang-9` `clang` `clang-format` `clang++` 四个文件
+ 复制替换文件到ndk里面去 `/Users/用户名/Library/Android/sdk/ndk/21.3.6528147/toolchains/llvm/prebuilt/darwin-x86_64/bin/`
+ 最后一步复制对应的头文件到NDK目录下(不然Android项目编译会报错找不到`stdarg`等头文件错误)，进入刚刚编译的build目录`OLLVM-9.0.1/build/lib/clang/9.0.1/include/`，然后复制如下文件`stdarg.h`、`stddef.h`、`__stddef_max_align_t.h`、`float.h`到 `/Users/用户名/Library/Android/sdk/ndk/21.3.6528147/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include` 文件夹下就彻底大功告成了。
+ 注意上面的路径是macOS系统的 系统不一样 安装路径和`darwin-x86_64`的名字也会不同

#### 注意事项
+ 项目在macOS 10.15.7 xcode 12.3 的环境上测试成功 Linux系统未测试理论通用 Windows系统有问题 需要修改代码才能用(手上没有Windows系统所以未实践)
+ Windows需要修改的代码[随机数问题](https://github.com/obfuscator-llvm/obfuscator/pull/89/commits/8b93673403f75d5766093469c7532bb9752e2d2d)、[头文件问题](https://github.com/obfuscator-llvm/obfuscator/pull/90/commits/f7003bb6199c2da345d749ed62503597f4198d26)
+ aarch64(包括MacBook M1芯片) 上需要修改的问题 [修改头文件](https://github.com/obfuscator-llvm/obfuscator/pull/160)
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
+ ndk-build 的用法就不用我特意说明了吧 毕竟那群混吃等死懒得研究新事物的搬砖工写的文章都是基于这玩意的(也可能是屎山 迁移不了cmake)。 (手动狗头
+ `sub_loop`、`split_num`、`bcf_loop`这些是弄几遍 `bcf_prob`是这代码每个基本块有40%的概率进行BCF混淆(0-100) 命令具体是干什么的自己搜索引擎去了解 懒得挨个写了
+ `"-std=c++14", "-frtti", "-fexceptions", "-DANDROID_TOOLCHAIN=clang", "-ffunction-sections", "-fdata-sections"` 这些和ollvm无太大关系 什么意思自己查
#### 补充知识
+ Android 中的ABI `armeabi-v7a`、`x86` 是32位的，`arm64-v8a`是64位的， `x86_64`这个是两开花的？ `arm`开头的是arm架构CPU用的的 `x86 x86_64`是台式机英特尔、AMD CPU用的(一般都是模拟器) 。远古时代的`armeabi`已经淘汰了，ndk不再支持。理论上只编译个`armeabi-v7a`就能通杀`arm`，再加个`x86`通杀模拟器吧

#### 更多玩法
+ `git patch` 这是个非常方便的东西 可以通过他来实现一键移植`ollvm`(合并代码补丁) 自己去下载`llvm`源码+`clang`源码(解压复制`clang-xxx`文件夹并改名成`clang`到`llvm`的`tools`里面) 然后回到`llvm`根目录 使用`git`命令 吧咱项目里面的`add_o-llvm(fixed).patch`文件打进你自己下载的`llvm`项目里面去就一键完成移植啦~


### 补充说明
+ 如需借鉴参考这个项目来写博客写文章请直接使用本项目的`GitHub`链接，不要克隆后用自己的地址分发，为了避免有人提交修复问题的时候弄得到处都是，所以需要统一在这个项目维护方便大家的使用，谢谢合作。
+ 以上内容都是个人理解 如有问题请提`issues`
