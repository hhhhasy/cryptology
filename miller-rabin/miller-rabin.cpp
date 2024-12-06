#include<iostream>
#include<cmath>
using namespace std;

// 使用加法模拟乘法（分治法优化）
long long mockMul(long long a, long long b, long long mod) {
    // 基本情况：b == 0 或 b == 1
    if (b == 0) return 0;
    if (b == 1) return a % mod;

    // 如果 b 是偶数：a * b = (a * (b / 2)) + (a * (b / 2))
    if (b % 2 == 0) {
        long long half = mockMul(a, b / 2, mod);  // 递归计算一半
        return (half + half) % mod;  // 合并结果
    }
    // 如果 b 是奇数：a * b = (a * (b / 2)) + (a * (b / 2)) + a
    else {
        long long half = mockMul(a, b / 2, mod);  // 递归计算一半
        return (half + half + a) % mod;  // 合并结果并加上a
    }
}

// 使用加法模拟平方
long long mockSquare(long long a, long long mod) {
    return mockMul(a, a, mod);  // 使用 addMultiply 来实现平方
}

// 快速幂算法（使用加法模拟乘法）
long long modExp(long long base, long long exp, long long mod) {
    long long result = 1;
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

bool miller_rabin(long long n, int iterations = 10) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;

    long long t = n - 1;
    long long k = 0;
    long long m = t;

    while (m % 2 == 0) {
        m /= 2;
        k++;
    }

    srand(time(nullptr)); // 随机数种子

    for (int i = 0; i < iterations; i++) {
        long long a = 2 + rand() % (n - 3); // 随机选取 [2, n-2]
        long long b = modExp(a, m, n);
        if (b % n == 1 || b % n == n - 1) continue;

        bool passed = false;
        for (long long j = 0; j < k - 1; j++) {
            b = modExp(b, 2, n);
            if (b % n == n - 1) {
                passed = true;
                break;
            }
        }
        if (!passed) return false; // 如果一次测试失败，n 不是素数
    }
    return true; // 多次测试通过，n 是素数
}
int main() {
    long long n;
    cin >> n;
    if (miller_rabin(n)) {
        cout << "Yes";
    }
    else {
        cout << "No";
    }

    return 0;
}