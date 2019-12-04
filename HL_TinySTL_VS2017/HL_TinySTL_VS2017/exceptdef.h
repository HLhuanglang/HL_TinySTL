/************************************************************
名称：exceptdef.h
作用：定义一些异常的宏函数
*************************************************************/

#ifndef EXCEPTDEF_H
#define EXCEPTDEF_H

#include<stdexcept> //标准库的异常类
#include<cassert> //断言

namespace TinySTL {

#define STL_DEBUG(expr)\
 assert(expr)

} //namespace TinySTL
#endif // !EXCEPTDEF_H

