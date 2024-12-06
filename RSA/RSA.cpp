#include <iostream>
#include <cmath>

// 扩展欧几里得算法求模逆
__int128 gcdExtended(long long a, long long b, long long* x, long long* y) {
    if (a == 0) {
        *x = 0, * y = 1;
        return b;
    }
    long long x1, y1;
    __int128 gcd = gcdExtended(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
    return gcd;
}

// 计算模逆
__int128 modInverse(long long e, long long phi) {
    long long x, y;
    __int128 g = gcdExtended(e, phi, &x, &y);
    if (g != 1) {
        std::cerr << "模逆不存在";
        return -1;
    }
    else {
        __int128 res = (x % phi + phi) % phi;
        return res;
    }
}

// 使用加法模拟乘法（分治法优化）
__int128 mockMul(__int128 a, __int128 b, __int128 mod) {
    // 基本情况：b == 0 或 b == 1
    if (b == 0) return 0;
    if (b == 1) return a % mod;

    // 如果 b 是偶数：a * b = (a * (b / 2)) + (a * (b / 2))
    if (b % 2 == 0) {
        __int128 half = mockMul(a, b / 2, mod);  // 递归计算一半
        return (half + half) % mod;  // 合并结果
    }
    // 如果 b 是奇数：a * b = (a * (b / 2)) + (a * (b / 2)) + a
    else {
        __int128 half = mockMul(a, b / 2, mod);  // 递归计算一半
        return (half + half + a) % mod;  // 合并结果并加上a
    }
}

// 使用加法模拟平方
__int128 mockSquare(__int128 a, __int128 mod) {
    return mockMul(a, a, mod);  // 使用 addMultiply 来实现平方
}

// 快速幂算法（使用加法模拟乘法）
__int128 modExp(__int128 base, __int128 exp, __int128 mod) {
    __int128 result = 1;
    base = base % mod;  // 确保 base < mod
    while (exp > 0) {
        if (exp & 1) {  // 如果当前指数为奇数
            result = mockMul(result, base, mod);  // 使用加法模拟乘法
        }
        base = mockSquare(base, mod);  // 使用加法模拟平方
        exp >>= 1;  // 指数右移
    }
    return result;
}

// 输出 __int128 类型的数字
void printInt128(__int128 num) {
    if (num == 0) {
        std::cout << 0;
        return;
    }

    // 将 __int128 拆分为高位和低位
    unsigned long long high = num >> 64;  // 高64位
    unsigned long long low = num & 0xFFFFFFFFFFFFFFFF;  // 低64位

    if (high > 0) {
        std::cout << high;
    }
    std::cout << low;
}

int main() {
    
    long long p, q, e, c;

    // 输入
    std::cin >> p >> q;
    std::cin >> e >> c;

    // 计算 n 和 φ(n)
    __int128 n = p * q;
    __int128 phi = (p - 1) * (q - 1);

    // 求解 d
    __int128 d = modInverse(e, phi);
    if (d == -1) {
        std::cerr << "无效的密钥";
        return 1;
    }

    // 解密
    __int128 m = modExp(c, d, n);

    // 输出解密结果
    printInt128(m);
    std::cout << std::endl;

    return 0;
}
