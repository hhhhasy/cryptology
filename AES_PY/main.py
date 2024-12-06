S_BOX = [
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
]

RCON = [0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1B000000,
        0x36000000]
NR = 10


def hex_to_128bit_binary(hex_str):
    return ''.join(format(int(char, 16), '04b') for char in hex_str)


def convert_to_8bit_binary(num):
    return format(num, '08b')


def rotate_string(s):
    return s[8:] + s[:8]


def sub_bytes(temp):
    number = [int(temp[i:i + 8], 2) for i in range(0, 32, 8)]
    return ''.join(convert_to_8bit_binary(S_BOX[num]) for num in number)


def filedmult_2(byte_str):
    byte = int(byte_str, 2)
    result = byte << 1
    if result & 0x100:
        result ^= 0x1B
    result &= 0xFF
    return format(result, '08b')


def filedmult_3(byte_str):
    byte = int(byte_str, 2)
    result = int(filedmult_2(byte_str), 2) ^ byte
    return format(result, '08b')


def mixcolumn(state, i):
    s_0 = state[i * 8:i * 8 + 8]
    s_1 = state[i * 8 + 32:i * 8 + 40]
    s_2 = state[i * 8 + 64:i * 8 + 72]
    s_3 = state[i * 8 + 96:i * 8 + 104]
    u_0 = ''.join(
        str(int(bit1) ^ int(bit2) ^ int(bit3) ^ int(bit4)) for bit1, bit2, bit3, bit4 in
        zip(filedmult_2(s_0), filedmult_3(s_1), s_2, s_3))
    u_1 = ''.join(
        str(int(bit1) ^ int(bit2) ^ int(bit3) ^ int(bit4)) for bit1, bit2, bit3, bit4 in
        zip(filedmult_2(s_1), filedmult_3(s_2), s_3, s_0))
    u_2 = ''.join(
        str(int(bit1) ^ int(bit2) ^ int(bit3) ^ int(bit4)) for bit1, bit2, bit3, bit4 in
        zip(filedmult_2(s_2), filedmult_3(s_3), s_0, s_1))
    u_3 = ''.join(
        str(int(bit1) ^ int(bit2) ^ int(bit3) ^ int(bit4)) for bit1, bit2, bit3, bit4 in
        zip(filedmult_2(s_3), filedmult_3(s_0), s_1, s_2))
    new_state = state[:i * 8] + u_0 + state[i * 8 + 8:32 + i * 8] + u_1 + state[40 + i * 8:64 + i * 8] + u_2 + state[
                                                                                                               72 + i * 8:96 + i * 8] + u_3 + state[
                                                                                                                                              104 + i * 8:]
    return new_state


def generate_key(K):
    W = [K[32 * i:32 * (i + 1)] for i in range(4)]
    for i in range(4, 44):
        temp = W[i - 1]
        if i % 4 == 0:
            temp = rotate_string(temp)
            temp = sub_bytes(temp)
            temp = format(int(temp, 2) ^ RCON[i // 4 - 1], '032b')
        W.append(''.join(str(int(a) ^ int(b)) for a, b in zip(W[i - 4], temp)))
    return W



binary_string = hex_to_128bit_binary('000102030405060708090a0b0c0d0e0f')
W = generate_key(binary_string)


def cal_ciphertext(plaintext):
    groups = [plaintext[i:i + 8] for i in range(0, 128, 8)]
    rearranged_groups = []
    for col in range(4):
        for row in range(4):
            rearranged_groups.append(groups[row * 4 + col])
    state = ''.join(rearranged_groups)

    key = '' + W[0] + W[1] + W[2] + W[3]
    groups = [key[i:i + 8] for i in range(0, 128, 8)]
    rearranged_groups = []
    for col in range(4):
        for row in range(4):
            rearranged_groups.append(groups[row * 4 + col])
    key = ''.join(rearranged_groups)
    state = ''.join(str(int(a) ^ int(b)) for a, b in zip(state, key))
    hex_str = ''
    for i in range(0, 128, 4):
        bin_chunk = state[i:i + 4]
        hex_str += format(int(bin_chunk, 2), 'X')
    print(hex_str)



    for t in range(1, NR):

        number = [int(state[i:i + 8], 2) for i in range(0, 128, 8)]
        state = ''.join(convert_to_8bit_binary(S_BOX[num]) for num in number)
        hex_str = ''
        for i in range(0, 128, 4):
            bin_chunk = state[i:i + 4]
            hex_str += format(int(bin_chunk, 2), 'X')
        print(hex_str)

        string = [state[i * 32:(i + 1) * 32] for i in range(4)]
        i = 0
        while i < 4:
            str_ = string[i]
            j = i
            while j > 0:
                str_ = rotate_string(str_)
                j = j - 1
            string[i] = str_
            i = i + 1

        state = '' + string[0] + string[1] + string[2] + string[3]


        hex_str = ''
        for i in range(0, 128, 4):
            bin_chunk = state[i:i + 4]
            hex_str += format(int(bin_chunk, 2), 'X')
        i = 0
        while i < 4:
            state = mixcolumn(state, i)
            i = i + 1

        key = '' + W[t * 4] + W[t * 4 + 1] + W[t * 4 + 2] + W[t * 4 + 3]

        groups = [key[i:i + 8] for i in range(0, 128, 8)]
        rearranged_groups = []
        for col in range(4):
            for row in range(4):
                rearranged_groups.append(groups[row * 4 + col])
        key = ''.join(rearranged_groups)

        state = (''.join(str(int(a) ^ int(b)) for a, b in zip(state, key)))

    number = [int(state[i:i + 8], 2) for i in range(0, 128, 8)]
    state = ''.join(convert_to_8bit_binary(S_BOX[num]) for num in number)
    string = [(state[i:i + 32]) for i in range(0, 128, 32)]
    i = 0
    while i < 4:
        str_ = string[i]
        j = i
        while j > 0:
            str_ = rotate_string(str_)
            j = j - 1
        string[i] = str_
        i = i + 1
    state = '' + string[0] + string[1] + string[2] + string[3]
    key = '' + W[40] + W[41] + W[42] + W[43]
    groups = [key[i:i + 8] for i in range(0, 128, 8)]
    rearranged_groups = []
    for col in range(4):
        for row in range(4):
            rearranged_groups.append(groups[row * 4 + col])
    key = ''.join(rearranged_groups)

    state = (''.join(str(int(a) ^ int(b)) for a, b in zip(state, key)))
    return state



plaintext = hex_to_128bit_binary('00112233445566778899aabbccddeeff')
state = cal_ciphertext(plaintext)
groups = [state[i:i + 8] for i in range(0, 128, 8)]
rearranged_groups = []
for row in range(4):
    for col in range(4):
        rearranged_groups.append(groups[col * 4 + row])

state = ''.join(rearranged_groups)
hex_str = ''
for i in range(0, 128, 4):
    bin_chunk = state[i:i + 4]
    hex_str += format(int(bin_chunk, 2), 'X')
print(hex_str)

