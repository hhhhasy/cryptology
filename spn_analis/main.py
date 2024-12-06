import random
import numpy as np
import  json


S = [14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7]
S_reverse=[14,3,4,8,1,12,10,15,7,13,9,6,11,2,0,5]
P = [1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16]
K=[0,0,1,1,1,0,1,0,1,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,1,0,0,0,1,1,1,1]
NR = 4
T=8000




def cal_k(K, k, r):
    i = 4 * (r) - 4
    j = i + 16
    t = 0
    for index in range(i, j):
        k[t] = K[index]
        t += 1


def intToBinary(num):
    # 将整数转换为4位二进制列表
    binary = [0] * 4
    index = 3
    while index >= 0:
        binary[index] = num & 1
        num >>= 1
        index -= 1
    return binary


def cal_v(U, v):
    for i in range(4):
        num = 0
        t = 3
        for j in range(i * 4, i * 4 + 4):
            num += (2 ** t) * U[j]
            t -= 1

        num = S[num]
        binary = intToBinary(num)
        t = 0
        for j in range(i * 4, i * 4 + 4):
            v[j] = binary[t]
            t += 1


def cal_ciphertext(s):



    x = [int(s[i]) for i in range(16)]


    w = [0] * 16
    k = [0] * 16
    U = [0] * 16
    v = [0] * 16

    # 初始赋值
    for i in range(16):
        w[i] = x[i]

    # 进行NR-1轮加密
    for i in range(1, NR):
        cal_k(K, k, i)

        for j in range(16):
            U[j] = 0 if k[j] == w[j] else 1

        cal_v(U, v)

        w = [v[P[j] - 1] for j in range(16)]

    # 最后一轮加密
    cal_k(K, k, NR)

    for i in range(16):
        U[i] = 0 if k[i] == w[i] else 1

    cal_v(U, v)
    cal_k(K, k, NR + 1)
    #print(k)

    result =''
    for i in range(16):
        if k[i] == v[i]:
            result += '0'
        else:
            result += '1'
    return result


def generate_binary_numbers(n):
    # 生成从 0 到 2^n - 1 的十进制数
    number=T
    list=[]
    for i in range(2 ** n):
        # 将十进制数转换为 n 位的二进制数，使用 zfill(n) 来确保它是 n 位
        binary_str = bin(i)[2:].zfill(n)
        list.append(binary_str)
    list=random.sample(list,number)
    return list


# 生成数据集
plaintexts=generate_binary_numbers(16)

Plain_ciphertext_pairs={}
for i in range(len(plaintexts)):
    ciphertext=cal_ciphertext(plaintexts[i])
    Plain_ciphertext_pairs.update({plaintexts[i]: ciphertext})

with open('data1.txt', 'w') as file:
    json.dump(Plain_ciphertext_pairs, file)



count_l2l4 = np.zeros((16, 16), dtype=int)
count_l1l3 = np.zeros((16, 16), dtype=int)
Max1=-1
Max2=-1
Maxkey1=''
Maxkey2=''
Maxkey3=''
Maxkey4=''

for X,Y in Plain_ciphertext_pairs.items():
    for i in range(16):
        for j in range(16):
            L1=intToBinary(i)
            L2=intToBinary(j)
            Y2=[int(Y[4]),int(Y[5]),int(Y[6]),int(Y[7])]
            Y4 =[int(Y[12]), int(Y[13]), int(Y[14]), int(Y[15])]
            V2= [a ^ b for a, b in zip(L1, Y2)]
            V4= [a ^ b for a, b in zip(L2, Y4)]
            U2 = intToBinary(S_reverse[int(''.join(map(str, V2)), 2)])
            U4 = intToBinary(S_reverse[int(''.join(map(str, V4)), 2)])
            Z=int(X[4])^int(X[6])^int(X[7])^U2[1]^U2[3]^U4[1]^U4[3]
            if(Z==0):
                count_l2l4[i][j]+=1


for i in range(16):
    for j in range(16):
        count_l2l4[i][j]=abs(count_l2l4[i][j]-T/2)
        if(count_l2l4[i][j]>Max1):
            Max1=count_l2l4[i][j]
            max2=intToBinary(i)
            max4=intToBinary(j)
            Maxkey2=''.join(map(str, max2))
            Maxkey4= ''.join(map(str, max4))

key2= [int(char) for char in Maxkey2]
key4= [int(char) for char in Maxkey4]



for X,Y in Plain_ciphertext_pairs.items():
    for i in range(16):
        for j in range(16):
            L1 = intToBinary(i)
            L3 = intToBinary(j)

            # 提取密文
            Y1 = [int(Y[0]), int(Y[1]), int(Y[2]), int(Y[3])]
            Y3 = [int(Y[8]), int(Y[9]), int(Y[10]), int(Y[11])]
            Y2 = [int(Y[4]), int(Y[5]), int(Y[6]), int(Y[7])]
            Y4 = [int(Y[12]), int(Y[13]), int(Y[14]), int(Y[15])]

            V1 = [a ^ b for a, b in zip(L1, Y1)]
            V2 = [a ^ b for a, b in zip(key2, Y2)]
            V3 = [a ^ b for a, b in zip(L3, Y3)]
            V4 = [a ^ b for a, b in zip(key4, Y4)]

            U1 = intToBinary(S_reverse[int(''.join(map(str, V1)), 2)])
            U2 = intToBinary(S_reverse[int(''.join(map(str, V2)), 2)])
            U3 = intToBinary(S_reverse[int(''.join(map(str, V3)), 2)])
            U4 = intToBinary(S_reverse[int(''.join(map(str, V4)), 2)])

            z1 = int(X[0]) ^ int(X[1]) ^ int(X[3]) ^ (U1[0]) ^ (U2[0]) ^ (U3[0]) ^ (U4[0])
            if z1 == 0:
                count_l1l3[i][j] += 1

            z2 = int(X[8]) ^ int(X[9]) ^ int(X[11]) ^ (U1[2]) ^ (U2[2]) ^ (U3[2]) ^ (U4[2])
            if z2 == 0:
                count_l1l3[i][j] += 1


for i in range(16):
    for j in range(16):
        count_l1l3[i][j]=abs(count_l1l3[i][j]-T/2)
        if count_l1l3[i][j] > Max2:
            Max2 = count_l1l3[i][j]
            max1 = intToBinary(i)
            max3 = intToBinary(j)
            Maxkey1 = ''.join(map(str, max1))
            Maxkey3 = ''.join(map(str, max3))


Maxkey=''+Maxkey1+Maxkey2+Maxkey3+Maxkey4
print(Maxkey)
