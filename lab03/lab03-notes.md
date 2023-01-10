# Lab 3: RISC-V Assembly

## Venus Setup

维纳斯的帮助文档：[venus reference](https://cs61c.org/fa22/resources/venus-reference/)



## Exercise 1: 远程连接Venus

根据文档一步步设置Venus连接；





## Exercise 2: Translating from C to RISC-V

看文档怎么一步步将C语言翻译成RISC-V的：

```c
#include <stdio.h>

int n = 9;

// Function to find the nth Fibonacci number
int main(void) {
    int curr_fib = 0, next_fib = 1;
    int new_fib;
    for (int i = n; i > 0; i--) {
        new_fib = curr_fib + next_fib;
        curr_fib = next_fib;
        next_fib = new_fib;
    }
    printf("%d\n", curr_fib);
    return 0;
}
```



汇编程序：

```assembly
.data
n: .word 9

.text
main:
    add t0, x0, x0 # curr_fib = 0
    addi t1, x0, 1 # next_fib = 1
    la t3, n # load the address of the label n
    lw t3, 0(t3) # get the value that is stored at the adddress denoted by the label n
fib:
    beq t3, x0, finish # exit loop once we have completed n iterations
    add t2, t1, t0 # new_fib = curr_fib + next_fib;
    mv t0, t1 # curr_fib = next_fib;
    mv t1, t2 # next_fib = new_fib;
    addi t3, t3, -1 # decrement counter
    j fib # loop
finish:
    addi a0, x0, 1 # argument to ecall to execute print integer
    addi a1, t0, 0 # argument to ecall, the value to be printed
    ecall # print integer ecall
    addi a0, x0, 10 # argument to ecall to terminate
    ecall # terminate ecall
```



首先是最上面的data：

```txt
.data
n: .word 9
```

- `n` 是变量名
- `.word` 表示数据的大小是一个**字**
- `9` 是变量的值



main函数，跟在.text节之后：

```assembly
.text
main:
    add t0, x0, x0 # curr_fib = 0
    addi t1, x0, 1 # next_fib = 1
```



看看地址的加载：

```assembly
la t3, n # load the address of the label n
lw t3, 0(t3) # get the value that is stored at the adddress denoted by the label n
```

这两行类似于：

```c
t3 = &n;
t3 = *n;
```

la：将n的地址加载到t3；



为什么不能直接将n的值给到t3？

> Now, you're probably thinking, "Why can't we directly set `t3` to `n`?" In the `.text` section, there is no way that we can directly access `n`. (Think about it. We can't say `add t3, n, x0`. The arguments to `and` must be registers and `n` is not a register.) The only way that we can access it is by obtaining the address of `n`. Once we obtain the address of `n`, we need to dereference it which can be done with `lw`. `lw` will reach into memory at the address that you specify and load in the value stored at that address. In this case, we specified the address of `n` and added an offset of `0`.

因为add的操作数必须是寄存器，不是地址，所以必须把n的值先放到一个寄存器，只能先把n的地址加载，再解引用得到n的值。



看看循环体：

```assembly
fib:
    beq t3, x0, finish # exit loop once we have completed n iterations
    add t2, t1, t0 # new_fib = curr_fib + next_fib;
    mv t0, t1 # curr_fib = next_fib;
    mv t1, t2 # next_fib = new_fib;
    addi t3, t3, -1 # decrement counter
    j fib # loop
finish:
```



调用prinf：

```assembly
finish:
    addi a0, x0, 1 # argument to ecall to execute print integer
    addi a1, t0, 0 # argument to ecall, the value to be printed
    ecall # print integer ecall
```

ecall这条指令是所有系统调用的通用指令，根据a0的不同，调用不同的函数；

比如 `a0 = 1` 是调用printf，要打印的放在 `a1`；

> Printing is a system call. You'll learn more about these later in the semester, but a system call is essentially a way for your program to interact with the Operating System. To make a system call in RISC-V, we use a special instruction called `ecall`. To print out an integer, we need to pass two arguments to `ecall`. The first argument specifies what we want `ecall` to do (in this case, print an integer). To specify that we want to print an integer, we pass a `1`. The second argument is the integer that we want to print out.
>
> In C, we are used to functions looking like `ecall(1, t0)`. In RISC-V, we cannot pass arguments in this way. To pass an argument, we need to place it in an argument register (`a0`-`a7`). When the function executes, it will look in these registers for the arguments. (If you haven't seen this in lecture yet, you will soon). The first argument should be placed in `a0`, the second in `a1`, etc.



Next, let's terminate our program! This also requires `ecall`

```assembly
addi a0, x0, 10 # argument to ecall to terminate
ecall # terminate ecall
```

In this case, `ecall` only needs one argument. Setting `a0` to `10` specifies that we want to terminate the program.



## Exercise 3: 熟悉Venus

自己玩一玩Venus；





## Exercise 4: Array Practice

考虑现在有一个 函数f 定义在{-3, -2, -1, 0, 1, 2, 3}上，规则如下：

```txt
f(-3) = 6
f(-2) = 61
f(-1) = 17
f(0) = -38
f(1) = 19
f(2) = 42
f(3) = 5
```

在 `discrete_fn.s`实现这个函数, 不要使用分支和跳转指令。



因为已经给了我们一个数组 output `6, 61, 17, -38, 19, 42, 5`，那么就利用这个数组，处理好偏移量就好了：

```assembly
.globl f # this allows other files to find the function f

# f takes in two arguments:
# a0 is the value we want to evaluate f at
# a1 is the address of the "output" array (defined above).
# The return value should be stored in a0
f:
    addi a1,a1,12 # 将output[3]的地址加载到a1
    slli a0,a0,2 # 偏移量 * 4字节
    add a1,a1,a0 # 计算正确位置
    lw a0,0(a1) # 存入答案
    # This is how you return from a function. You'll learn more about this later.
    # This should be the last line in your program.
    jr ra  
```





## Exercise 5: 迭代实现阶乘

实现factorial函数就好了，没有测试，自己对照阶乘表看看输出；

```assembly
.globl factorial

.data
n: .word 8

.text
# Don't worry about understanding the code in main
# You'll learn more about function calls in lecture soon
main:
    la t0, n
    lw a0, 0(t0)
    jal ra, factorial

    addi a1, a0, 0
    addi a0, x0, 1
    ecall # Print Result

    addi a1, x0, '\n'
    addi a0, x0, 11
    ecall # Print newline

    addi a0, x0, 10
    ecall # Exit

# factorial takes one argument:
# a0 contains the number which we want to compute the factorial of
# The return value should be stored in a0
factorial:
    # YOUR CODE HERE
    addi t0,x0,1
    addi t1,a0,1
    addi a0,x0,1
loop:
    beq t0, t1, finish # if t0 == t1 finish
    mul a0,t0,a0
    addi t0,t0,1
    j loop  # jump to loop
finish:
    # This is how you return from a function. You'll learn more about this later.
    # This should be the last line in your program.
    jr ra
```





