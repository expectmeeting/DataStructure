# 大数

# 1、初始化函数

- 新生成一个BIGNUM结构

> BIGNUM *BN_new(void);

- 释放一个BIGNUM结构，释放完后a=NULL;

> void BN_free(BIGNUM *a);

- 初始化所有项均为0

> void BN_init(BIGNUM *);

- 将a中所有项均赋值为0，但是内存并没有释放

> void BN_clear(BIGNUM *a);

- 相当与将BN_free和BN_clear综合，要不就赋值0，要不就释放空间。

> void BN_clear_free(BIGNUM *a);
>
> # 2、复制以及交换函数
>
> - 将b复制给a,正确返回a，错误返回NULL
>
> > BIGNUM *BN_copy(BIGNUM *a, const BIGNUM *b);
>
> - 新建一个BIGNUM结构，将a复制给新建结构返回，错误返回NULL
>
> > BIGNUM *BN_dup(const BIGNUM *a);
>
> - 交换a,b
>
> > BIGNUM *BN_swap(BIGNUM *a, BIGNUM *b);

# 3、值测试

- 判断a与b是否相等

> int BN_cmp (BIGNUM *a, BIGNUM *b);

- 判断a与b的绝对值是否相等

> int BN_ucmp (BIGNUM *a, BIGNUM *b);

- 判断a是不是为0

> int BN_is_zero(BIGNUM *a);

- 判断a是不是1

> int BN_is_one(BIGNUM *a);

- 判断a是不是值w

> int BN_is_word(BIGNUM *a, BN_ULONG w);

- 判断a是不是一个奇数

> int BN_is_odd(BIGNUM *a);

# 4、取位函数

- 返回a的字节数

> int BN_num_bytes(const BIGNUM *a);

- 返回a的二进制位数

> int BN_num_bits(const BIGNUM *a);

- 返回有意义比特的位数，例如0x00000432 为11。

> int BN_num_bits_word(BN_ULONG w);

# 5、赋值与取值

- 设置a为1

> int BN_one(BIGNUM *a);

- 设置a为0

> int BN_zero(BIGNUM *a);

- 取a为二进制到to中，返回字符串长度

> int BN_bn2bin(const BIGNUM *a, unsigned char *to);

- 赋二进制值s到ret中，返回ret

> BIGNUM *BN_bin2bn(const unsigned char *s, int len, BIGNUM *ret);

- 取a的16进制值，返回一个字符串的指针。此指针要使用完后，手动使用OPENSSL_free释放

> char *BN_bn2hex(const BIGNUM *a);

- 取a的10进制值，返回一个字符串的指针。此指针要使用完后，手动使用OPENSSL_free释放

> char *BN_bn2dec(const BIGNUM *a);

- 赋16进制值str到*a中，返回成功与否

> int BN_hex2bn(BIGNUM **a, const char *str);

- 赋10进制值str到*a中，返回成功与否

> int BN_dec2bn(BIGNUM **a, const char *str);

# 6、计算

- 计算a与b的和，值储存在r中。如果成功，返回1；否则，返回0

> int BN_add(BIGNUM *r, const BIGNUM *a, const BIGNUM *b);

- 计算a与b的差，值储存在r中。返回1或者0

> int BN_sub(BIGNUM *r, const BIGNUM *a, const BIGNUM *b);

- 计算a与b的积，值储存在r中。返回1或者0。ctx为一个上下文相关的结构，可以用BN_CTX_new与BN_CTX_free来创建与释放它

> int BN_mul(BIGNUM *r, BIGNUM *a, BIGNUM *b, BN_CTX *ctx);

- 计算a的平方，值储存在r中。返回1或者0

> int BN_sqr(BIGNUM *r, BIGNUM *a, BN_CTX *ctx);

- 计算a与d的商，值储存在dv中，余数储存在rem中。返回1或者0

> int BN_div(BIGNUM *dv, BIGNUM *rem, const BIGNUM *a, const BIGNUM *d, BN_CTX *ctx);

- 计算a与m的模，值储存在rem中。返回1或者0

> int BN_mod(BIGNUM *rem, const BIGNUM *a, const BIGNUM *m, BN_CTX *ctx);

- 计算a与m的模，并且结果如果小于0，就加上m，值储存在r中。返回1或者0

> int BN_nnmod(BIGNUM *r, const BIGNUM *a, const BIGNUM *m, BN_CTX *ctx);

- 计算a与b的和，再模m，值储存在r中。返回1或者0

> int BN_mod_add(BIGNUM *r, BIGNUM *a, BIGNUM *b, const BIGNUM *m, BN_CTX *ctx);

- 计算a与b的差，再模m，值储存在r中。返回1或者0

> int BN_mod_sub(BIGNUM *r, BIGNUM *a, BIGNUM *b, const BIGNUM *m, BN_CTX *ctx);

- 计算a与b的积，再模m，值储存在r中。返回1或者0

> int BN_mod_mul(BIGNUM *r, BIGNUM *a, BIGNUM *b, const BIGNUM *m, BN_CTX *ctx);

- 计算a的平方根，再模m，值储存在r中。返回1或者

> int BN_mod_sqr(BIGNUM *r, BIGNUM *a, const BIGNUM *m, BN_CTX *ctx);

- 计算a的p次方，值储存在r中。返回1或者0

> int BN_exp(BIGNUM *r, BIGNUM *a, BIGNUM *p, BN_CTX *ctx);

- 计算a的p次方，再模m，值储存在r中。返回1或者0
   int BN_mod_exp(BIGNUM *r, BIGNUM *a, const BIGNUM *p, const >BIGNUM *m, BN_CTX *ctx);
- 计算a与b的最大公约数，值储存在r中。返回1或者0

> int BN_gcd(BIGNUM *r, BIGNUM *a, BIGNUM *b, BN_CTX *ctx);

- 大数a加上w，值储存在a中，返回1或者0
   int BN_add_word(BIGNUM *a, BN_ULONG w);
- 大数a减去w，值储存在a中，返回1或者0

> int BN_sub_word(BIGNUM *a, BN_ULONG w);

- 大数a乘以w，值储存在a中，返回1或者0

> int BN_mul_word(BIGNUM *a, BN_ULONG w);

- 大数a除以w，值储存在a中，返回余数

> BN_ULONG BN_div_word(BIGNUM *a, BN_ULONG w);

- 大数a模w，返回余数

> BN_ULONG BN_mod_word(const BIGNUM *a, BN_ULONG w);

- 模逆

> BIGNUM *BN_mod_inverse(BIGNUM *r, BIGNUM *a, const BIGNUM *n,BN_CTX *ctx);

# 7、随机数

- 产生一个加密用的强bits的伪随机数，若top=-1，最高位为0，top=0，最高位为1，top=1,最高位和次高位为1，bottom为真，随机数为偶数

> int BN_rand(BIGNUM *rnd, int bits, int top, int bottom);

- 产生一个伪随机数

> int BN_pseudo_rand(BIGNUM *rnd, int bits, int top, int bottom);

- 产生的随机数0<rnd<range

> int BN_rand_range(BIGNUM *rnd, BIGNUM *range);

- 产生的伪随机数0<rnd<range

> int BN_pseudo_rand_range(BIGNUM *rnd, BIGNUM *range);

# 8、生成素数

- 产生一个bits位的素数

> BIGNUM *BN_generate_prime(BIGNUM *ret, int bits,int safe, BIGNUM *add,BIGNUM *rem, void (*callback)(int, int, void *), void *cb_arg);

- 判断是否为素数，返回0表示成功，1表示错误概率小于0.25，-1表示错误

> int BN_is_prime(const BIGNUM *p, int nchecks,void (*callback)(int, int, void *), BN_CTX *ctx, void *cb_arg);]

# 9、位数

- 将a中的第n位设置为1，假如a小于n位将扩展

> int BN_set_bit(BIGNUM *a, int n);

- 将a中的第n为设置为0，假如a小于n位将出错

> int BN_clear_bit(BIGNUM *a, int n);

- 测试是否已经设置，1表示已设置

> int BN_is_bit_set(const BIGNUM *a, int n);

- 将a截断至n位，假如a小于n位将出错

> int BN_mask_bits(BIGNUM *a, int n);

- a左移n位，结果存于r

> int BN_lshift(BIGNUM *r, const BIGNUM *a, int n);

- a左移1位，结果存于r

> int BN_lshift1(BIGNUM *r, BIGNUM *a);

- a右移n位，结果存于r

> int BN_rshift(BIGNUM *r, BIGNUM *a, int n);

- a右移1位，结果存于r

> int BN_rshift1(BIGNUM *r, BIGNUM *a);