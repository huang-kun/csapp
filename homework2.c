#include <stdio.h>
#include <assert.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, int len) {
    int i;
    for (i = 0; i < len; i++) {
        printf("%.2x", start[i]);
    }
    printf("\n");
}

void show_int(int x) {
    show_bytes((byte_pointer)&x, sizeof(int));
}

void show_float(float x) {
    show_bytes((byte_pointer)&x, sizeof(float));
}

void show_pointer(void *x) {
    show_bytes((byte_pointer)&x, sizeof(void *));
}

void show_short(short x) {
    show_bytes((byte_pointer)&x, sizeof(short));
}

void show_long(long x) {
    show_bytes((byte_pointer)&x, sizeof(long));
}

void show_double(double x) {
    show_bytes((byte_pointer)&x, sizeof(double));
}

// 2.55, 56, 57
void test_show_bytes(int val) {
    int ival = val;
    float fval = (float)ival;
    int *pval = &ival;
    short sval = (short)ival;
    long lval = (long)ival;
    double dval = (double)ival;

    printf("show int: ");
    show_int(ival);
    printf("show float: ");
    show_float(fval);
    printf("show pointer: ");
    show_pointer(pval);
    printf("show short: ");
    show_short(sval);
    printf("show long: ");
    show_long(lval);
    printf("show double: ");
    show_double(dval);
}

void test_endian() {
    int val = 0x87654321;
    byte_pointer valp = (byte_pointer)&val;
    show_bytes(valp, 1);
    show_bytes(valp, 2);
    show_bytes(valp, 3);
}

/**
 * 2.58 检查是小端序还是大端序，前者返回1，后者返回0
 
    使用一个整数1，然后截取其低位字节，返回。由于小端序是
    从低位字节开始的，所以输出1；而大端序是从高位字节开始
    的，所以输出0
 */ 
int is_little_endian() {
    int val = 0x1;
    byte_pointer valp = (byte_pointer)&val;
    return valp[0];
}

void test_is_little_endian() {
    int res = is_little_endian();
    printf("is little endian: %d\n", res);
}

/**
 * 2.59 合并字节：保留x的低位字节，剩余字节用y的填充
 * 
 * 低位字节使用掩码m
 * (x & m)截取低位字节
 * (y & ~m)截取剩余高位字节
 * 最后合并
 */
int merge_byte(int x, int y) {
    int m = 0xff;
    return (x & m) | (y & ~m);
}

void test_merge_byte() {
    int res = merge_byte(0x89abcdef, 0x76543210);
    printf("%.4x\n", res);
}

/**
 * 2.60 字节替换：要求参数x的字节i替换成字节b
 * 
 * 把参数x作为字节数组来看待，i表示从低位开始的下标。
 * 如果是小端序，直接替换下标i的字节，即可满足题目要求；
 * 如果是大端序，需要反转下标
 */
unsigned replace_byte(unsigned x, unsigned char b, int i) {
    int s = sizeof(unsigned);
    if (i < 0 || i >= s)
        return x;

    unsigned char *px = (unsigned char *)&x;

    if (is_little_endian())
        px[i] = b;
    else
        px[s-1-i] = b;

    return x;
}

void test_replace_byte() {
    unsigned r1 = replace_byte(0x12345678, 0xAB, 2);
    printf("r1 = %x\n", r1);
    unsigned r2 = replace_byte(0x12345678, 0xAB, 0);
    printf("r1 = %x\n", r2);
}

/*
 * 示例代码：截取参数x的最高位字节
 *
 * 比如x = 987654321，二进制表示为0x3ade68b1
 * 执行get_msb(x)结果是58，二进制为0x3a，其中3a就是x的最高位字节。
 *
 * 下面代码的方法是通过把最高位字节右移到最低位来实现去除最高位字节
 * 外的其他低位字节，再通过与掩码的位操作来消除算数右移补最高位的
 * 问题（比如最高位可能是1）。
 *
 * 参数x类型是int，sizeof(int)为4字节，除去最高位字节后剩余3个字节，
 * 每个字节占用8位，所以需要右移3*8=24位，而下面的<<3等同于乘以8，
 * 但是在代码层面更加高效。再右移后通过& 0xFF就可以截取最低位字节。
 * */
/* Get most significant byte from x */
int get_msb(int x) {
    /* Shift by w-8 */
    int shift_val = (sizeof(int) - 1) << 3;
    /* Arithmetic shift */
    int xright = x >> shift_val;
    /* Zero all but LSB */
    return xright & 0xFF;
}

/**
 * 2.61 写一个c表达式，在下列条件下产生1，其他情况得到0，假设x是int类型
 * A. x的任何位都等于1
 * B. x的任何位都等于0
 * C. x的最低有效字节中的位都等于1
 * D. x的最高有效字节中的位都等于0
 * 
 * 要求遵守位级整数编码规则，以及不能使用==和!=
*/
int check_bits_a(int x) {
    return !(x ^ ~0);
}

int check_bits_b(int x) {
    return !(x ^ 0);
}

int check_bits_c(int x) {
    return !(x & 0xFF ^ 0xFF);
}

int check_bits_d(int x) {
    int n = (sizeof(int) - 1) << 3;
    int y = (x >> n) & 0xFF;
    return !(y ^ 0);
}

void test_check_bits() {
    assert(check_bits_a(-1) == 1);
    assert(check_bits_a(1) == 0);

    assert(check_bits_b(0) == 1);
    assert(check_bits_b(1) == 0);

    assert(check_bits_c(0x66283aff) == 1);
    assert(check_bits_c(0x83ac92bf) == 0);

    assert(check_bits_d(0x00bcef32) == 1);
    assert(check_bits_d(0x01234567) == 0);
}


/**
 * 2.62 编写函数，判断机器是否支持算术右移
*/
int int_shifts_are_arithmetic() {
    int i = -1;
    return i >> 1 == -1;
}

void test_int_shifts() {
    printf("arithmetic shifts %s\n", int_shifts_are_arithmetic() ? "works" : "not supported");
}


/**
 * 2.63 函数srl用算术右移实现逻辑右移；函数sra用逻辑右移实现算术右移
 * 
 * 除第一行外不能用右移或除法，该题没说不能用if判断。
*/
unsigned srl(unsigned x, int k) {
    // 指定算术右移
    unsigned xsra = (int) x >> k;
    
    // 既然算数右移把最高k位变成了1，
    // 所以后续的改动需要把最高k位都变成0

    // 计算需要左移的位数
    unsigned n = (sizeof(int) << 3) - k;
    // 生成最高k位是0，剩余位是1的掩码
    unsigned mask = ~(~0 << n);
    // 消除最高k位的值，实现逻辑右移
    return xsra & mask;
}

int sra(int x, int k) {
    // 指定逻辑右移
    int xsrl = (unsigned) x >> k;
    
    // 判断x的最高位，如果是1，就需要把最高k位的值还原成1
    unsigned w = sizeof(int) << 3;
    unsigned msb_mask = 1 << (w - 1);
    // 如果最高位是1
    if (!((unsigned)x & msb_mask ^ msb_mask)) {
        // 生成最高k位是1的掩码
        int mask = ~0 << (w - k);
        // 把最高k位的值都变成1
        return xsrl | mask;
    }
    // 如果最高位是0，就返回逻辑右移的结果
    return xsrl;
}

void test_srl_and_sra() {
    assert(srl(4, 2) == 1);
    assert(srl(-1, 24) == 0xff);
    assert(srl(0x87654321, 12) == 0x87654);

    assert(sra(10, 2) == 2);
    assert(sra(-1, 2) == -1);
    assert(sra(0x87654321, 12) == 0xfff87654);
}


/**
 * 2.64 判断奇数bit位是否存在1，假设w=32
 * 
 * 要求遵守位级整数编码规则，可以假设int有32位
*/
int any_odd_one(unsigned x) {
    // 解题思路：
    // 1.删除x所有偶数位的值后若为0，说明x没有奇数位存在1
    // 2.删除偶数位的方法是跟掩码0b0101...0101进行与操作，
    //   因为该掩码把偶数位都设置为0
    // 3.假设int为32位，相应的掩码就是0x55555555
    return (x & 0x55555555) != 0;
}

void test_any_odd_one() {
    assert(any_odd_one(0b1101) == 1);
    assert(any_odd_one(0b1001) == 1);
    assert(any_odd_one(0b1000) == 0);
    assert(any_odd_one(0b0100) == 1);
}


/**
 * 2.65 判断bit=1的个数是否为奇数
 * 
 * 要求遵守位级整数编码规则，可以假设int有32位
 * 最多包含12个算术运算、位运算和逻辑运算
*/
int odd_ones(unsigned x) {
    // 这个问题咨询了一位北大的实习生大神，他的思路是：
    // 把x的二进制位从中间对半分割，让左右两部分对齐后
    // 做异或，消除部分偶数个数的1，再循环上述步骤即可
    unsigned y = (x >> 16) ^ (x & 0xFFFF);
    y = (y >> 8) ^ (y & 0xFF);
    y = (y >> 4) ^ (y & 0xF);
    y = (y >> 2) ^ (y & 0b11);
    y = (y >> 1) ^ (y & 1);
    return y;
}

void test_odd_ones() {
    assert(odd_ones(0b1111) == 0);
    assert(odd_ones(0b1101) == 1);
    assert(odd_ones(0b1010) == 0);
    assert(odd_ones(0b1000) == 1);
    assert(odd_ones(0b101111) == 1);
    assert(odd_ones(0b101101) == 0);
    assert(odd_ones(0) == 0);
    assert(odd_ones(1) == 1);
}


/**
 * 2.66 生成掩码，要求除最左侧值为1以外的其他位都是0
 * 
 * 举例：0xff00 -> 0x8000, 0x6600 -> 0x4000
 * If x = 0, return 0
 * 
 * 要求遵守位级整数编码规则，最多包含15个算术运算、位运算和逻辑运算
 * 提示：先将x转换成[0...011...1]的位向量
*/
int leftmost_one(unsigned x) {
    return 1; //todo;
}

void test_leftmost_one() {
    assert(leftmost_one(0xff00) == 0x8000);
    assert(leftmost_one(0x6600) == 0x4000);
    assert(leftmost_one(0) == 0);
    assert(leftmost_one(1) == 1);
    assert(leftmost_one(3) == 2);
}


/**
 * 2.67 检查int是否为32位，不允许用sizeof
 * 
*/
int int_size_is_32() {
    // 1 << 32
    // warning: left shift count >= width of type [-Wshift-count-overflow]

    // int set_msb = 1 << 31;
    // int beyond_msb = 1 << 32;
    // return set_msb && !beyond_msb;
    return 0; //todo
}


/**
 * 2.68 生成掩码，让最低n位设成1，其他高位设成0
 * 
 * 举例：n=6 -> 0x3f, n=17 -> 0x1ffff
 * 取值：1 <= n <= w
 * 要求遵守位级整数编码规则，注意n=w的情况
*/
int lower_one_mask(int n) {
    // 如果左移位数n = w，结果取模，即 n mod w = 0
    // int类型假设32位，如果n=32，生成掩码就是0，而非期望的0xffffffff
    // 这个问题我参考了https://stackoverflow.com/a/4201315
    // 让左移的基数类型转为unsigned long (~0UL)，突破32位size的限制
    return ~(~0UL << n);
}

void test_lower_one_mask() {
    assert(lower_one_mask(1) == 0x1);
    assert(lower_one_mask(2) == 0x3);
    assert(lower_one_mask(6) == 0x3f);
    assert(lower_one_mask(17) == 0x1ffff);
    assert(lower_one_mask(31) == 0x7fffffff);
    assert(lower_one_mask(32) == 0xffffffff);
}


/**
 * 2.69 把最左边的n位与其他位交换位置
 * 
 * 举例：x = 0x12345678, w = 32,
 * n=4 -> 0x23456781, n=20 -> 0x67812345
 * 
 * 要求遵守位级整数编码规则，注意n=0的情况
*/
unsigned rotate_left(unsigned x, int n) {
    return 0; //todo
}

void test_rotate_left() {
    assert(rotate_left(0x12345678, 4) == 0x23456781);
    assert(rotate_left(0x12345678, 20) == 0x67812345);
    assert(rotate_left(0x12345678, 0) == 0x12345678);
}

/**
 * 2.70 判断整数 x 是否可以表示为 n 位的二进制补码形式
 * 
 * 举例：
 * fits_bits(5,3) == 0, 
 * 5的二进制表示为0101，需要4位才能表示，无法用3位表示成101（-3）
 * 
 * fits_bits(-4,3) == 1
 * -4的二进制补码形式为1100，也可以用3位表示，即110
 * 
 * 要求遵守位级整数编码规则
*/
int fits_bits(int x, int n) {
    return 0;
}

void test_fits_bits() {
    assert(fits_bits(5,3) == 0);
    assert(fits_bits(-4,3) == 1);
}


void print_args(int argc, char *argv[]) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("argv[%d]=%s\n", i, argv[i]);
    }
}

int main(int argc, char *argv[]) {
    // test_show_bytes(12345);
    // test_is_little_endian();
    // test_merge_byte();
    // test_replace_byte();
    // test_check_bits();
    // test_int_shifts();
    // test_srl_and_sra();
    // test_any_odd_one();
    test_odd_ones();
    // test_lower_one_mask();
}


