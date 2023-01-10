# Lab2

## Exercise 1: 位操作

获取某个bit：

```c
/* Returns the Nth bit of X. Assumes 0 <= N <= 31. */
unsigned get_bit(unsigned x, unsigned n) {
    /* YOUR CODE HERE */
    unsigned mask = 1 << n;
    return (x & mask) >> n; 
}
```



设置某个bit：这里异或比较不好想到（感觉其他操作也可以实现就是会麻烦？？？）；

```c
void set_bit(unsigned *x, unsigned n, unsigned v) {
    /* YOUR CODE HERE */
    unsigned bit = get_bit(*x,n); // 用到了上面的get_bit
    // if bit == 1, v == 1  (~v) << n
    //    bit == 1, v == 0  (~v) << n
    //    bit == 0, v == 1  v << n
    //    bit == 0, v == 0  v << n
    unsigned mask = (bit ^ v) << n;
    *x = mask ^ *x;
    return;
}
```



反转某个位：

```c
/* Flips the Nth bit in X. Assumes 0 <= N <= 31.*/
void flip_bit(unsigned *x, unsigned n) {
    /* YOUR CODE HERE */
    unsigned origin = get_bit(*x,n);
    unsigned flip = (~origin) & 1;
    set_bit(x,n,flip);
}
```





## Exercise 2: 用GDB定位段错误

在程序崩溃后，我们可以使用 `p *pointer(某个指针)`来查看具体是什么发生了错误；





## Exercise 3: Valgrind

Valgrind运行的环境和普通环境不太一样（所以当valgrind的结果和普通环境不一样的时候不用奇怪）；



访问出错的内存，会有这样：

```bash
==10170== Invalid read of size 1
==10170==    at 0x4C34D04: strlen (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==10170==    by 0x10879F: make_Str (bork.c:22)
==10170==    by 0x108978: translate_to_bork (bork.c:56)
==10170==    by 0x1089F2: main (bork.c:68)
```



如果没有回收内存，会有这样：

```bash
==29797== HEAP SUMMARY:
==29797==     in use at exit: 8 bytes in 1 blocks
==29797==   total heap usage: 11 allocs, 10 frees, 1,061 bytes allocated

==29797== LEAK SUMMARY:
==29797==    definitely lost: 8 bytes in 1 blocks
==29797==    indirectly lost: 0 bytes in 0 blocks
==29797==      possibly lost: 0 bytes in 0 blocks
==29797==    still reachable: 0 bytes in 0 blocks
==29797==         suppressed: 0 bytes in 0 blocks
==29797== Rerun with --leak-check=full to see details of leaked memory
```



带上--leak-check=full 参数，来显示更多细节：

```bash
$ valgrind --leak-check=full ./bork hello
```



这下Heap Summary更加详细了，告诉我们哪些地方分配的内存没有回收：

```bash
==32334== HEAP SUMMARY:
==32334==     in use at exit: 8 bytes in 1 blocks
==32334==   total heap usage: 11 allocs, 10 frees, 1,061 bytes allocated
==32334== 
==32334== 8 bytes in 1 blocks are definitely lost in loss record 1 of 1
==32334==    at 0x4C31B0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==32334==    by 0x108784: alloc_str (in /home/cc/cs61c/fa22/staff/cs61c-tac/bork)
==32334==    by 0x10884E: concat (in /home/cc/cs61c/fa22/staff/cs61c-tac/bork)
==32334==    by 0x108A30: main (in /home/cc/cs61c/fa22/staff/cs61c-tac/bork)
```





## Exercise 4: Memory Management

注意一下set的时候，如果要扩容需要扩容 loc+1个数，而且需要释放原数组并且修改v->size；

```c
void vector_set(vector_t *v, size_t loc, int value) {
    /* What do you need to do if the location is greater than the size we have
     * allocated?  Remember that unset locations should contain a value of 0.
     */
    /* YOUR CODE HERE */
    if(loc >= v->size){
        int* origin = v->data;
        v->data = malloc(sizeof(int) * (loc + 1));
        for (size_t i = 0; i < v->size; i++){
            v->data[i] = origin[i];
        }
        free(origin); // 释放原数组;
        for (size_t i = v->size; i < loc + 1; i++){
            v->data[i] = 0;
        }
        v->size = loc + 1; // 更新v->size;
    }
    v->data[loc] = value;
}
```











