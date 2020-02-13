一、TinySTL项目简介
---
TinySTL是参考sgi版本的源码，同时也参考了github上几个比较优秀的开源项目。侯捷那本《STL源码剖析》你可以下载电子版的，纸质书买不买看你个人，推荐购买一本（二手即可）。

 关于项目功能测试，后期会参考google的开源项目gtest，设计一个相对简单的测试方案。项目初期我会写一点功能就写一点测试代码，同步更新在github上。
 
 项目我计划开发的内容有：(暂定)
 1. 容器：vector、list、deque、stack、map、set
 2. 算法：常见的（数量不定）
 
 开发内容顺序：(暂定)
 1. 初期：vector -> list -> deque和测试框架
 2. 中期：实现RB-tree数据结构->map、set，算法，stack
 3. 后期：实现hashtable数据结构，实现map、set

二、博客更新计划
---
博客地址：https://blog.csdn.net/HL545454/article/details/102995793

个人博客号：CSDN搜索 我梦i 或者 HL545454 点波关注呀~

《STL从零开始-宏观概述》系列博客主要是讲述设计思想、原理和框架的，不会设计到具体代码层面的分析，但是疑难点会结合代码分析。

《STL从零开始-代码实现》系列博客则主要侧重代码实现层面，会有大量代码分析，主要是把想法落地变成代码。如vector内部为何要定义 start、finish、end_of_storge，构造函数、析构函数如何利用这几个变量；alloc.h中，内存池memory_pool_start、memory_pool_end为何定义成static等细节问题。

博客可能更新的会很慢，因为写博客超级废时间的，需要组织好语言，还要考虑是不是讲清楚了。同时我也是新手，很多知识点也在努力学习，所以更新巨慢请见谅。但是我写了一份开发日记，里面有每一周的开发情况记录，记录了各种各样乱七八糟的问题，后续作为写博客的一个参考点吧~

三、相关资料
---
书籍资料下载：https://pan.baidu.com/s/1E9107zw3xDhHkPnmhfVTtg  提取码：wrs3

四、项目运行
---
1. vs2017（其他版本也可以）
2. git clone https://github.com/HLhuanglang/HL_TinySTL.git
3. 下载整个项目以后，进入HL_TinySTL\HL_TinySTL_VS2017 点击.sln即可
(ps:2020.2.13号，由于前段时间电脑进水了，导致资料丢失了很多，同时开发环境换成了vs2019。)
