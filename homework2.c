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
 * B. x的任何位栋等于0
 * C. x的最低有效字节中的位都等于1
 * D. x的最高有效字节中的位都等于0
 * 
 * 要求遵守位级整数编码规则，以及不能使用==和!=
*/
int check_bits(int x) {
    return 1; //todo
}

void test_check_bits() {
    assert(check_bits(-1) == 1);
    assert(check_bits(0) == 1);
    assert(check_bits(0x66283aff) == 1);
    assert(check_bits(0x00bcef32) == 1);
    assert(check_bits(1) == 0);
    assert(check_bits(-2) == 0);
    assert(check_bits(0x83ac92bf) == 0);
    assert(check_bits(0x01234567) == 0);
}


/**
 * 2.62 编写函数，判断机器是否支持算术右移
*/
int int_shifts_are_arithmetic() {
    return 1; //todo
}


/**
 * 2.63 函数srl用算术右移实现逻辑右移；函数sra用逻辑右移实现算术右移
 * 
 * 除第一行外不能用右移或除法，该题没说不能用if判断。
*/
unsigned srl(unsigned x, int k) {
    // 指定算术右移
    unsigned xsra = (int) x >> k;
    //todo
    return 1;
}

int sra(int x, int k) {
    // 指定逻辑右移
    int xsrl = (unsigned) x >> k;
    //todo
    return 1;
}

void test_srl_and_sra() {
    assert(srl(4, 2) == 1);
    assert(srl(-1, 24) == 0xff);
    assert(srl(0x87654321, 3) == 0x00087654);

    assert(sra(10, 2) == 4);
    assert(sra(-1, 2) == -1);
    assert(sra(0x87654321, 3) == 0xfff87654);
}


/**
 * 2.64 判断奇数bit位是否存在1，假设w=32
 * 
 * 要求遵守位级整数编码规则，可以假设int有32位
*/
int any_odd_one(unsigned x) {
    return 1; //todo
}

void test_any_odd_one() {
    assert(any_odd_one(0b1101) == 1);
    assert(any_odd_one(0b1001) == 0);    
}


/**
 * 2.65 判断bit=1的个数是否为奇数
 * 
 * 要求遵守位级整数编码规则，可以假设int有32位
 * 最多包含12个算术运算、位运算和逻辑运算
*/
int odd_ones(unsigned x) {
    return 1; //todo
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
    int set_msb = 1 << 31;
    int beyond_msb = 1 << 32;
    return set_msb && !beyond_msb;
}


/**
 * 2.68 生成掩码，让最低n位设成1，其他高位设成0
 * 
 * 举例：n=6 -> 0x3f, n=17 -> 0x1ffff
 * 要求遵守位级整数编码规则，注意n=w的情况
*/
int lower_one_mask(int n) {
    return 0; //todo
}

void test_lower_one_mask() {
    assert(lower_one_mask(6) == 0x3f);
    assert(lower_one_mask(17) == 0x1ffff);
    assert(lower_one_mask(sizeof(size_t)) == ~0);
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
    // test_srl_and_sra();
    // test_any_odd_one();

}


