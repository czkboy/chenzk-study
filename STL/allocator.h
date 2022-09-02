#ifndef KSTD_ALLOCATOR_H_
#define KSTD_ALLOCATOR_H_

// 这个头文件包含一个模板类 allocator，用于管理内存的分配、释放，对象的构造、析构
// 暂不支持标准库容器 todo::支持萃取

#include <new>      // placement new
#include <cstddef>  // ptrdiff_t size_t
#include <cstdlib>  // exit
#include <climits>  // UINT_MAX
#include <iostream> // cerr

namespace kstd
{

  /*
  new 处理函数set_new_handler()是为分配函数在凡是内存分配尝试失败时调用的函数。其目的是三件事之一：

  1) 令更多内存可用
  2) 终止程序（例如通过调用 std::terminate
  3) 抛出 std::bad_alloc 或自 std::bad_alloc 导出的类型的异常。
  */

  //创建内存
  template <class T>
  inline T *_allocate(ptrdiff_t size, T *)
  {
    // set_new_handler(0);

    // there are no arguments to ‘set_new_handler’
    // that depend on a template parameter, so a declaration of
    //‘set_new_handler’ must be available [-fpermissive]

    T *tmp = (T *)(::operator new((size_t)(size * sizeof(T))));
    if (!tmp)
    {
      std::cerr << "out of memory" << std::endl;
      exit(1);
    }
    return tmp;
  }
  //销毁内存
  template <class T>
  inline void _deallocate(T *buffer)
  {
    ::operator delete(buffer);
  }
  //构造函数 placement new
  template <class T1, class T2>
  inline void _construct(T1 *p, const T2 &value)
  {
    new (p) T1(value);
  }
  template <class T>
  inline void _construct(T* ptr)
  {
    new (ptr) T;
  }

  //析构
  template <class T>
  inline void _destroy(T *ptr)
  {
    ptr->~T();
  }

  // 模板类：allocator
  // 模板函数代表数据类型
  template <class T>
  class allocator
  {
  public:
    typedef T value_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

  public:
    static pointer allocate(size_type n, const void *hint = 0)
    {
      return _allocate((difference_type)n, (pointer)0);
    }

    static void deallocate(pointer ptr);

    static void construct(pointer ptr, const_reference value);
    static void construct(pointer ptr);
    static void destroy(pointer ptr);
  };

  template <class T>
  void allocator<T>::deallocate(pointer ptr)
  {
    _deallocate(ptr);
  }

  template <class T>
  void allocator<T>::construct(pointer ptr, const_reference value)
  {
    _construct(ptr, value);
  }

  template <class T>
  void allocator<T>::construct(pointer ptr)
  {
    _construct(ptr);
  }

  template <class T>
  void allocator<T>::destroy(pointer ptr)
  {
    _destroy(ptr);
  }

} // namespace kstd
#endif // KSTD_ALLOCATOR_H_
