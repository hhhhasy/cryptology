#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<cmath>

int naf[100]={0};  // Array to hold the NAF representation
int index = 0;

typedef struct {
    int x;
    int y;
} Point;


Point point_double(Point P1) {
    if (P1.x == 0 && P1.y == 0) {
        return P1;
    }

    Point result;

    if (P1.y == 0) {
        result.x = 0;
        result.y = 0;
        return result;
    }
    int reverse = 0;
    for (int i = 0; i < 11; i++) {
        if ((i * 2 * P1.y) % 11 == 1) {
            reverse = i;
            break;
        }
    }
    int lamda = ((3 * P1.x * P1.x + 1) * reverse) % 11;
    int x3 = ((lamda * lamda - P1.x - P1.x) % 11 + 11) % 11;
    int y3 = (((lamda * (P1.x - x3)) - P1.y) % 11 + 11) % 11;
    result.x = x3;
    result.y = y3;
    return result;
}


Point point_add(Point P1, Point P2) {
    if (P1.x == 0 && P1.y == 0) {
        return P2;
    }

    if (P2.x == 0 && P2.y == 0) {
        return P1;
    }

    Point result;
    if (P1.x == P2.x && P1.y == P2.y) {
        result = point_double(P1);
        return result;
    }
    
    
    int deltay = P2.y - P1.y;
    int deltax= P2.x - P1.x;
    if (deltax == 0) {
        result.x = 0;
        result.y = 0;
        return result;
    }

    int reverse = 0;
    if (deltax < 0) {
        deltax += 11;
    }

    if (deltay < 0) {
        deltay += 11;
    }
    for (int i = 0; i < 11; i++) {
        if ((i * deltax) % 11 == 1) {
            reverse = i;
            break;
        }
    }

    int lamda = (deltay * reverse) % 11;
    int x3 = ((lamda * lamda - P1.x - P2.x) % 11 + 11) % 11;
    int y3 = (((lamda * (P1.x - x3)) - P1.y) % 11 + 11) % 11;

    result.x = x3;
    result.y = y3;
    return result;
}



void to_NAF(int K) {
    index = 0;
    if (K == 0) {
        naf[index++] = 0;
        return;
    }
    while (K != 0) {
        // If K is odd, set the current NAF element to K mod 2, which is either 1 or -1
        if (K % 2 != 0) {
            if (K % 4 == 1) {
                naf[index++] = 1;  // NAF: 1 if K is 1 mod 4
            }
            else {
                naf[index++] = -1; // NAF: -1 if K is 3 mod 4
            }
            K -= naf[index - 1];  // Adjust K to remove the non-zero element
        }
        else {
            naf[index++] = 0; // If K is even, the corresponding NAF element is 0
        }
        K /= 2;  // Move to the next bit
    }
    for (int i = index - 1; i >= 0; i--) {
        std::cout << naf[i] << " ";
    }
    std::cout << std::endl;
}

Point cal_kx(Point p, int k) {
    Point p_r;
    p_r.x = p.x;
    p_r.y = 11 - p.y;
    to_NAF(k);
    Point Q;
    Q.x = 0;
    Q.y = 0;

    for (int i = index - 1; i >= 0; i--) {
        Q = point_double(Q);
        if (naf[i] == 1) {
            Q = point_add(Q, p);
        }
        if (naf[i] == -1) {
            Q = point_add(Q, p_r);
        }
        
    }
    return Q;
}




void cal_rs(int u, int x,int k, int m,long long & r, long long & s) {

    r = u % 13;
    int k_r=0;
    for (int i = 0; i < 13; i++) {
        if ((i * k) % 13 == 1) {
            k_r = i;
            break;
        }
    }

    long long c = 1;
    for (int i = 1; i <= x; i++) {
        c *= 2;
        c %= 13;
    }
    s = (k_r * (c + m * r)) % 13;

}
int main() {
    int x0, y0;
    std::cin >> x0 >> y0;
    int m, x, k;
    std::cin >> m >> x >> k;
    Point p;
    p.x = x0;
    p.y = y0;
    
    Point Q = cal_kx(p, k);
    std::cout << Q.x << " " << Q.y << std::endl;
    long long r, s;
    cal_rs(Q.x, x, k, m, r, s);
    std::cout << r << " " << s;

    system("pause");
    return 0;
}
