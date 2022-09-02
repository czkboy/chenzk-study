
// 这个头文件包含一个模板类 
allocator，用于管理内存的分配、释放，对象的构造、析构

/*

全大写为 define const enum
成员变量 M开头
全局变量 G开头
static S开头
内部使用 
“__”开头为内部（私有）函数变量或者同名的包函数 
“_”为其他的一般内部部分 注变量常为“__”




*/
#pragma once

#include <new>      // for placement new
#include <cstddef>  // for ptrdiff_t size_t
#include <cstdlib>  // for exit
#include <climits>  // for UINT_MAX
#include <iostream> // for cerr



enum { ALIGN = 8 };
enum { MAX_BYTES = 128 };
enum { NFREELISTS = MAX_BYTES / ALIGN };

#define FREE_INDEX(args) (args/ALIGN -1)
#define UP_ROUND(args) ((args+ALIGN-1)& ~(ALIGN -1))

#if 0
#   include <new>
#   define __THROW_BAD_ALLOC throw bad_alloc()
#elif !defined(__THROW_BAD_ALLOC)
#   include <iostream>
#   define __THROW_BAD_ALLOC cerr << "out of memory" << endl; exit(1)
#endif

namespace kstd
{

    /*
    new 处理函数set_new_handler()
是为分配函数在凡是内存分配尝试失败时调用的函数。其目�
��是三件事之一：

    1) 令更多内存可用
    2) 终止程序（例如通过调用 std::terminate
    3) 抛出 std::bad_alloc 或自 std::bad_alloc 导出的类型的异常。
    */

    //创建内存
    template <class T>
    inline T* __allocate(size_t size)
    {
        

       
        return (T)malloc(size);
    }
    //销毁内存
    template <class T>
    inline void __deallocate(T* buffer)
    {
       free(buffer);
    }
    //构造函数 placement new
    template <class T1, class T2>
    inline void __construct(T1* p, const T2& value)
    {
        new (p) T1(value);
    }
    template <class T>
    inline void __construct(T* ptr)
    {
        new (ptr) T;
    }

    //析构
    template <class T>
    inline void __destroy(T* ptr)
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
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

    private:
        union _obj {
            _obj* next;
            char unuse[1];
        };
        static size_t _S_heap_size;

        
       
        _obj* free_list[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
        _obj* start, * end;

    public:
        static pointer allocate(size_type n)
        {
            size_type size = UP_ROUND(n);
            if(size>MAX_BYTES)
                __allocate((size_type)n);
            else {
                _obj** index=free_list + FREE_INDEX(size);
                _obj* result = *index;
                if (0 == result) {
                    return refill(size);
                }
                
                *index = result->next;
                return result;

            }

        }
        static pointer refill(size_type n)
        {
            
            
            int nobj = 20;
            char* chunk = alloc_chunk(n,nobj);//这里会爆出异常 
__THROW_BAD_ALLOC

            
            if (1 == nobj)return chunk;
            /*
            for (__i = 1; ; __i++) {
                __current_obj = __next_obj;
                __next_obj = (_Obj*)((char*)__next_obj + __n);
                if (__nobjs - 1 == __i) {  
                    __current_obj->_M_free_list_link = 0;
                    break;
                    //gnu gcc不喜欢正常循环20次 喜欢直接break出
                }
                else {
                    __current_obj->_M_free_list_link = __next_obj;
                }
            }
            */
            //_obj** index = free_list + FREE_INDEX(size);
            _obj* result,*current;
            result = chunk;
            _obj* pro = (_obj*)((char*)chunk + size);
            current = pro;//哨兵
            for (int i = 1; i < nobj; ++i) {
                pro= (_obj*)((char*)chunk + size);//
这里不强制转换可能跳的数量不对 每次跳sizeof(char) 位才对
                current->next = pro;
                current = pro;
            }
            current->next = nullptr;
            return result;
  
            
        }
        static size_type alloc_chunk(size_type __size, int& __nobjs) {
            /*
            四种情况
            先看看能不能从pool里面割出20个（20是经验值 
就如vector扩容有时是2倍有时是1.5倍）
            不能看看能不能割出一个
            一个都不能看看能不能往池子里装水
            
还是不能就调用一级适配器调用malloc让操作系统想办法
            */

            size_type left_bytes =  end - start ;//水池剩下的水
            size_type total_bytes= __size*__nobjs;
            _obj* result;
            if (left_bytes >= total_bytes) {

                result = start;
                start = start + total_bytes;
                return result;
            }
            else if (left_bytes >= __size) {
                __nobjs = (int)(left_bytes/__size);
                total_bytes = __size * __nobjs;

                result = start;
                start = start + total_bytes;
                return result;
                
            }
            else {
                size_t __bytes_to_get = 2 * total_bytes + UP_ROUND(
_S_heap_size >> 4);

                _obj** __my_free_list;
                if (left_bytes > 0) {
                    __my_free_list =
                        free_list + FREE_INDEX(left_bytes);

                    ((_obj*)start)->next = *__my_free_list;
                    *__my_free_list = (_obj*)start;
                }
                start = (char*)__allocate(__bytes_to_get);
                if (0 == start) {
                    _obj* __p;
                    for (auto __i = __size; __i <= MAX_BYTES;i+=ALIGN ) {
                        __my_free_list = free_list + FREE_INDEX(__i);
                        __p = *__my_free_list;
                        if (0 != __p) {
                            start = __p;
                            end = __p + __i;
                            __p = __p->next;
                            return(alloc_chunk(__size, __nobjs));
                        }
                    }
                    //山穷水尽 因为前面就是malloc的 
没必要再试了 直接报错
                    __THROW_BAD_ALLOC

                }

            }
               

        }



        //static void deallocate(pointer ptr);

    };



} // namespace kstd



