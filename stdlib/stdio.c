#include <stdint.h>

typedef struct { int64_t length; char* data; } CuprumString;

// raw write syscall
static int64_t sys_write(int64_t fd, const char* buf, int64_t len) {
    int64_t ret;
    __asm__ volatile (
        "syscall"
        : "=a"(ret)
        : "0"(1), "D"(fd), "S"(buf), "d"(len)
        : "rcx", "r11", "memory"
    );
    return ret;
}

void print(CuprumString s) {
    sys_write(1, s.data, s.length);
}

void println(CuprumString s) {
    sys_write(1, s.data, s.length);
    sys_write(1, "\n", 1);
}

void print_char(char c) {
    sys_write(1, &c, 1);
}

void print_int(int32_t n) {
    if (n < 0) {
        sys_write(1, "-", 1);
        n = -n;
    }

    char buf[12];
    int i = 11;
    buf[i] = '\0';
    if (n == 0) { sys_write(1, "0", 1); return; }
    while (n > 0) {
        buf[--i] = '0' + (n % 10);
        n /= 10;
    }
    int len = 11 - i;
    sys_write(1, buf + i, len);
}

void print_long(int64_t n) {
    if (n < 0) {
        sys_write(1, "-", 1);
        n = -n;
    }

    char buf[21];
    int i = 20;
    buf[i] = '\0';
    if (n == 0) { sys_write(1, "0", 1); return; }
    while (n > 0) {
        buf[--i] = '0' + (n % 10);
        n /= 10;
    }
    int len = 20 - i;
    sys_write(1, buf + i, len);
}

void print_float(float f) {
    // print integer part + 6 decimal places
    if (f < 0) { sys_write(1, "-", 1); f = -f; }
    int32_t ipart = (int32_t) f;
    print_int(ipart);
    sys_write(1, ".", 1);
    float frac = f - (float) ipart;
    for (int i = 0; i < 6; i++) {
        frac *= 10;
        int digit = (int) frac;
        char c = '0' + digit;
        sys_write(1, &c, 1);
        frac -= digit;
    }
}