## some notes

#### 资源

栈空间

> * 分配简单，移动栈指针esp即可
> * 当前换出执行完后立即自动释放
> * 后进先出，因此不可能出现内存碎片
> * 函数返回后，栈上创建的对象立即被销毁。因此对象所占据的空寂随时可能被后续的代码所改写
> * 栈内存是不分享的，栈上的对象通常是没有多线程竞争问题的



<!DOCTYPE html>
<html>
<head>
    <style>
        /* 控制图片并排显示的样式 */
        .half {
            display: flex;          /* 使用 Flexbox 布局 */
            justify-content: space-between; /* 图片之间均匀分布 */
            margin: 0;             /* 清除默认外边距 */
            padding: 0;            /* 清除默认内边距 */
            width: 50%;           /* 宽度占满容器 */
        }
        .half img {
            width: 49%;           /* 每张图片宽度略小于50%，避免挤在一起 */
            height: auto;         /* 高度自适应 */
            object-fit: contain;  /* 保持图片比例 */
        }
    </style>
</head>
<body>
    <figure class="half">
        <img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250407220158867.png" alt="图片1"><img src="https://raw.githubusercontent.com/wangbanjin1/pictures/main/image-20250407220225659.png" alt="图片2">
    </figure>
</body>
</html>









