# KMP算法

KMP算法是一种快速的字符串匹配算法，例如在字符串（主串）中查找子串（模式串），如给定长度为m字符串s =“aababcaabaacdcbab”,长度为n的模式串p=“aabaacd”，给出B在A中的位置。暴力解法的时间复杂度为**O(n*m)**,而KMP算法的时间复杂度为**O(n+m)**   

## 基本概念

为了便于理解KMP算法，这里先介绍需要理解的一些概念。  

* **子序列：** 字符串中任意个**相对位置**不变的字符组成的字符串，如“ab”，“ac”，“a”均为“abc”的子序列。

* **子串：** 字符串中任意**连续**个字符组成的子序列，如“ab”,“bc”，“abc”,“”均为“abc”的子串。

* **字符串前缀：** 包含第一个字符，但不包含最后一个字符的子串，如“a”，“ab”，“abc”均为“abcd”的前缀。

* **字符串后缀：** 不包含第一个字符，但包含最后一个字符的子串，如“d”，“cd”，“bcd”均为“abcd”的后缀。

* **部分匹配值：** 字符串前缀集合和字符串后缀集合中最长公共元素的长度。如字符串“abab”的前缀集合为{“a”,“ab”,“aba”}，后缀集合为{“b”,“ab”,“bab”}，公共元素仅有“ab”，因此其为最长公共元素，长度为2。

* **next数组：** next数组的大小与模式串相同，其next[j]的含义是P[j]之前的字符串P[0..j-1]的部分匹配值。 

## 算法思路

通过对上述概念的理解，能更清楚地明白KMP算法的思路。相比传统的暴力算法，KMP算法中指向目标串字符的i不会回退，指向模式串p中的j在匹配失败时，总是指向p中标号为next[j]的字符。如主串i=4,s[i]='b'与模式串j=4,p[j]='a'不匹配，如图1，则已匹配字符串为“aaba”，其部分匹配值为1，即字符串“aaba”从起始位置数起的前1个字符和从末尾位置数起的前1个字符相同(或理解为目标串中的子串“aaba”从末尾位置数起的前1个字符和匹配串中的子串“aaba”从起始位置数起的前1个字符已经匹配)。那么i指向的位置不需回退，只需将j的值改为next[j]即可(若部分匹配值为-1，则i++)，然后重新开始匹配，如图2。   

![图1](https://github.com/expectmeeting/DataStructure/blob/master/KMP/figure1.emf)

![图2](https://github.com/expectmeeting/DataStructure/blob/master/KMP/figure2.emf)    

## 求解next数组

按照next数组的定义，默认next[0]=-1,next[1]=0。下面给出图3来展示next数组的求解。  

![图3](https://github.com/expectmeeting/DataStructure/blob/master/KMP/gif1.gif)   

下面给出求解next数组的程序

~~~c
void getnext(const char *p,int * const next){
	assert(p && next);
	
	int n = strlen(p);
	next[0] = -1;
	next[1] = 0;
	
	int j = 2;
	int i = 0;//next[j-1]
	while (j < n){
		if (p[j-1] == p[i]){
			next[j++] = ++i;
		}
		else if(next[i] == -1){
			next[j++] = 0;
		}
		else{
			i = next[i];
		}
	}
}
~~~

