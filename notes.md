## some notes

#### 资源

##### 栈空间

> * 分配简单，移动栈指针esp即可
> * 当前换出执行完后立即自动释放
> * 后进先出，因此不可能出现内存碎片
> * 函数返回后，**栈上创建的对象立即被销毁**。因此对象所占据的空寂随时可能被后续的代码所改写
> * 栈内存是不分享的，栈上的对象通常是没有多线程竞争问题的

<table>
    <tr>
        <td><center>
            <img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250407220158867.png" height="200" width="400">栈空间内存
        </center></td>
        <td><center>
           <img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250407220225659.png" height="200" width="400">内嵌函数
        </center></td>
    </tr>
</table>
1. 栈上对象的生命周期：

```cpp
{
    ...
    Obj obj{42};
    return &obj;
    ...
}
```

"}"会自动调用`obj`的析构函数，因此return该对象的指针将是未定义行为。

2. 栈展开：

```cpp
class Obj {
public:
  Obj() { puts("Obj()"); }
  ~Obj() { puts("~Obj()"); }
};
void foo(int n) {
  Obj obj;
  if (n == 42) {
    throw "life, the universe and everthing";
  }
}
int main() {
  try {
    foo(41);
    foo(42);
  } catch (const char *s) {
    puts(s);
  }
```

输出结果如下：

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250408164038261.png" alt="image-20250408164038261" style="zoom:80%;" />

在**发生异常**的时候或者**产生一个返回值之类正常退出作用域**的时候，编译器都会产生代码使得正确的析构代码被执行。

在大部分的语言里是没有决定性自动调用的析构函数，清理资源（释放锁、文件句柄等）需要手工调用类似dispose的方法。垃圾回收语言如Java可以自动回收内存。

析构函数和RAII是c++的最基本惯用手段。

##### 堆空间

> * 分配和释放算法复杂
> * 可能出现内存碎片
> * 考虑多线程的情况下，**内存分配和释放通常需要加锁**
> * 堆上对象在被释放前一直都可以被使用
> * 堆上对象**有可能被多个线程访问**，存在潜在的竞争性

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250408112421116.png" alt="image-20250408112421116" style="zoom:50%;" />

因此需要配对调用

> malloc/free                    fopen/fclose                       new /delete

##### RAII

 Resource Acquisition Is Initialization 资源获取即初始化

重点在于构造和析构行为，尤其是析构。用于帮助管理堆上的对象：对象很大；对象大小编译时不能确定；对象是函数的返回值，但是由于特殊原因不应该使用对象的值返回。

例如工厂方法示例中，我们的返回值是一个对象指针父类指针。

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250408165315147.png" alt="image-20250408165315147" style="zoom: 67%;" />

进一步地，可以完成一个RAII的帮助类

```cpp
```

