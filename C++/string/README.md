# 字符串

## 1、 基础知识

**字符串**是由0或多个字符组成的有限串行。

**C风格字符串**：

（1）由双引号括起的字符串常量，如“abc”。为了兼容C语言，C++中由编译器统一在其末尾添加空字符（NULL或’\0’）。

（2）末尾添加了’\0’的字符数组。

**注意**：char* pc = “abc”；//由于pc指向的字符串常量位于字符常量区，因此不可通过指针pc对字符串进行修改，最好的方法是采用如下方法定义：const char* pc = “abc”；

**子序列**：字符串中任意个相对位置不变的字符组成的字符串，如“ab”，“ac”，“a”均为“abc”的子序列。

**子串**：字符串中任意连续个字符组成的子序列，如“ab”,“bc”，“abc”,“”均为“abc”的子串。

**字符串前缀**：包含第一个字符，但不包含最后一个字符的子串，如“a”，“ab”，“abc”均为“abcd”的前缀。

**字符串后缀**：不包含第一个字符，但包含最后一个字符的子串，如“d”，“cd”，“bcd”均为“abcd”的后缀。

**部分匹配值**：字符串前缀集合和字符串后缀集合中最长公共元素的长度。如字符串“abab”的前缀集合为{“a”,“ab”,“aba”}，后缀集合为{“b”,“ab”,“bab”}，公共元素仅有“ab”，因此其为最长公共元素，长度为2。

 

                             **表1** 字符串处理函数

| strlen(s)        | 返回字符串长度，不包括结束字符NULL                           |
| ---------------- | ------------------------------------------------------------ |
| strcmp(s1,s2)    | 比较两个字符串是否相同，相等返回0；s1大于s2返回正数；小于则返回负数 |
| strcpy(s1,s2)    | 将s2复制给s1，并返回s1                                       |
| strcat(s1,s2)    | 将s2连接到s1后，并返回s1                                     |
| strncpy(s1,s2,n) | 将s2的前n个字符复制给s1，并返回s1                            |
| strncat(s1,s2,n) | 将s2的前n个字符连接到s1后，并返回s1                          |

‘A’~’Z’:97-127

‘a’~’z’:65-90

‘0’~’1’:48-57

## 2、 字符串处理函数的实现

### 1、strlen

~~~c++
int strlen(const char* pc)
{
	assert(pc!=NULL);
	return pc == NULL ? 0 : 1+strlen(++pc);
}
~~~



### 2、strcmp

```c++
int strcmp(const char* pc1, const char* pc2)
{
	assert(pc1! = NULL && pc2 != NULL);

	int iRet = 0;

	while (!(iRet=*(unsigned char*)pc1-*(unsigned char*)pc2) && *pc1){
		pc1++;
		pc2++;
	}

	if (iRet>0)       iRet = 1;
	else if (iRet<0)	iRet = -1;

	return iRet;
}
```

注意：只能比较字符串，不能比较数字等其他形式的参数

### 3、strcat

```c++
char* strcat(char* pc1, const char* pc2)
{
	assert(pc1! = NULL && pc2 != NULL);

	char* pcDest = pc1;

	while (*pc1++);

	while (*pc1++=*pc2++);

	return pcDest;
}
```

### 4、strcpy

```c++
char* strcpy(char* pc1, const char* pc2)
{
	assert(pc1! = NULL && pc2 != NULL);

	char* pcDest = pc1;

	while ((*pc1++=*pc2++)!='\0');

	return pcDest;
}
```

疑问：while循环中为(*pc1++=*pc2++)!='\0' 而不是 *pc1++=*pc2++ ？

## 3、 memset 和 memcpy

- void* memcpy(void* pDest, const void* pSrc, size_t n); 

功能：从源src所指向的内存地址的起始位置开始拷贝n个字节到目标dest所指向的内存地址的起始位置中。函数返回指向dest的指针。

- void* memset(void* s,int ch , size_t n); 

功能：将s中前n个字节用ch替换并返回s，作用是在一段内存块中填充某个给定的值，它是对较大的结构体或数组进行清零操作的一种最快方法。

- void *memmove(void *dest, const void *src, size_t n); 

功能：将src指针指向的内存区的前n个字节数据拷贝到dest指向的内存区内。src和dest指向内存区可以重叠，memmove可以保证拷贝的正确性。
