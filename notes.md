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
            <img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250407220158867.png" height="200" width="400"><br>栈空间内存
        </center></td>
        <td><center>
           <img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250407220225659.png" height="200" width="400"><br>内嵌函数
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

 Resource Acquisition Is Initialization **资源获取即初始化**

重点在于构造和析构行为，尤其是析构。用于帮助管理堆上的对象：对象很大；对象大小编译时不能确定；对象是函数的返回值，但是由于特殊原因不应该使用对象的值返回。

例如工厂方法示例中，我们的返回值是一个对象指针父类指针。

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250408165315147.png" alt="image-20250408165315147" style="zoom: 67%;" />

进一步地，可以完成一个RAII的帮助类

```cpp
class shape_wrapper {
public:
    explicit shape_wrapper(shape *ptr = nullptr)
      : ptr_(ptr) {
    }
    ~shape_wrapper() {
        delete ptr_;
    }
    shape *get() const {
        return ptr_;
    }
private:
    shape *ptr_;
};
void foo() {
    shape_wrapper ptr_wrapper(create_shape(shape_type::circle));
}
int main() {
    foo();
    return 0;
}
```

new和delete的设计思路

可以通过下图来表示

<center><img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250408201543298.png" alt="image-20250408201543298" style="zoom:50%;" /><br>new和delete背后原理<center/>

RAII不仅仅内存管理，也可以用于关闭文件(fstream的析构就会这样做)，此外还能释放同步锁或者释放其他重要的系统资源。

RAII在锁同步中的设计使用

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250408202822040.png" alt="image-20250408202822040" style="zoom: 67%;" />

RAII在容器中进行成员管理

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250408203030352.png" alt="image-20250408203030352" style="zoom:67%;" />

RAII的递归性

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250408203154502.png" alt="image-20250408203154502" style="zoom:67%;" />

##### 三法则

> * 若某个类需要用户定义的析构函数，用户定义的拷贝构造函数，或者用户定义的拷贝赋值运算符，则它几乎肯定三者全部都需要实现。

##### 引用语义和值语义

值语义的优点

> * 行为简单，符合直觉
> * 不容易发生竞争
> * 数据嵌套时内存相邻，性能高
> * 对象本身可以分配在栈上，栈上内存的分配释放开销非常低

缺点也很明显，容易不小心发生内存复制，那如何返回或者复制容器对象呢？

> * 函数如果返回临时对象，它是临时对象，在当前语句执行结束后则被销毁。
> * 如果我们要获得临时对象中的资源，我们会使用移动而不是构造

####  移动语义和右值引用

##### 左值

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250408205957423.png" alt="image-20250408205957423" style="zoom: 50%;" />

##### 右值

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250408210033800.png" alt="image-20250408210033800" style="zoom: 50%;" />

##### 将亡值

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250408210153909.png" alt="image-20250408210153909" style="zoom: 67%;" />

##### 移动和noexcept

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250408211534411.png" alt="image-20250408211534411" style="zoom:67%;" />

#####   五法则

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250408211706892.png" alt="image-20250408211706892" style="zoom:67%;" />

##### 右值引用的误用

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250408211955529.png" alt="image-20250408211955529" style="zoom:67%;" />

##### c++对象的生命周期

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250408213102839.png" alt="image-20250408213102839" style="zoom: 67%;" />

#### 手写智能指针

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250409151037429.png" alt="image-20250409151037429" style="zoom: 33%;" />

这里注意下，需要手动实现一个拷贝构造，为什么呢？尽管已经有了一个模板形式的通用写法，但是c++得编译器不会将其看作是已有的拷贝构造，所以自动生成一个默认的拷贝构造，但是对我们而言它得逻辑不完全正确，所以需要再手动实现一个。同时，如果手动提供了拷贝构造，编译器将不再提供默认的移动构造，此时模板实现的通用形式将能够被识别使用。

让对象支持移动？

> * 对象应该有分开的拷贝构造和移动构造函数
> * 对象应该有swap成员函数，支持和另外一个对象的快速交换成员

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250409151553631.png" alt="image-20250409151553631" style="zoom:33%;" />

make_unique的注意点

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250409151914653.png" alt="image-20250409151914653" style="zoom:33%;" />

传参一般使用普通指针或引用

#### 容器和类容器

##### string和序列容器

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250409160121286.png" alt="image-20250409160121286" style="zoom: 80%;" />

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250409160227904.png" alt="image-20250409160227904" style="zoom: 50%;" />

begin和end是迭代器，front和back是引用

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250409160325893.png" alt="image-20250409160325893" style="zoom: 67%;" />

接口使用建议

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250409160554804.png" alt="image-20250409160554804" style="zoom:50%;" />

##### array特点

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250409161007834.png" alt="image-20250409161007834" style="zoom:50%;" />

##### vector

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250409161353943.png" alt="image-20250409161353943" style="zoom:67%;" />

一点异常安全要求：

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250409163747240.png" alt="image-20250409163747240" style="zoom:67%;" />

##### 函数对象类型决定了其行为

#### 对象返回和异常

在函数输出数值的时候，尽量使用返回值而非输出参数

返回对象的优势

> * 代码直观更加容易理解
> * 乘法和假发可以组合在一行里写出来，无需中间变量
> * 性能高，没有不需要的复制发生

返回值优化，返回无名对象

```cpp
class A {
public:
    A() {
        cout << "Create A\n";
    }
    ~A() {
        cout << "Destroy A\n";
    }
    A(const A&) = delete;
    A(A&&) = delete;
};
A getA_unnamed() {
    return A();
}
int main() {
    auto a = getA_unnamed();
}
```

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250410153520192.png" alt="image-20250410153520192" style="zoom:67%;" />

返回多分支构造的情况下会优先发生移动构造，如果移动构造没定义会选择拷贝构造

返回对象的理由

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250410154003410.png" alt="image-20250410154003410" style="zoom:80%;" />

当然也有例外

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250410154035638.png" alt="image-20250410154035638" style="zoom:80%;" />

异常违反了c++零开销原则，但是大大增强了代码的表达能力和可读性

使用返回码会出现一个深层返错

异常安全

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250410155710973.png" alt="image-20250410155710973" style="zoom:67%;" />

异常优势

<img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250410160539897.png" alt="image-20250410160539897" style="zoom:80%;" />

c++易用性改进

![image-20250410172806662](https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250410172806662.png)

#### 视图

* string_view

  ![image-20250428153956420](C:\Users\wangbanjin\AppData\Roaming\Typora\typora-user-images\image-20250428153956420.png)

* span

  ![image-20250428155514439](https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250428155514439.png)

* Ranges里的视图

#### 模版基础
