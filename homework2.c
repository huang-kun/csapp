#include <stdio.h>

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
}