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
        str(int(bit1) ^ int(bit2) ^ int(bit3) ^ int(bit4)) for bit1, bit2, bit3, bit4 in zip(filedmult_2(s_0), filedmult_3(s_1), s_2, s_3))
    u_1 = ''.join(
        str(int(bit1) ^ int(bit2) ^ int(bit3) ^ int(bit4)) for bit1, bit2, bit3, bit4 in zip(filedmult_2(s_1), filedmult_3(s_2), s_3, s_0))
    u_2 = ''.join(
        str(int(bit1) ^ int(bit2) ^ int(bit3) ^ int(bit4)) for bit1, bit2, bit3, bit4 in zip(filedmult_2(s_2), filedmult_3(s_3), s_0, s_1))
    u_3 = ''.join(
        str(int(bit1) ^ int(bit2) ^ int(bit3) ^ int(bit4)) for bit1, bit2, bit3, bit4 in zip(filedmult_2(s_3), filedmult_3(s_0), s_1, s_2))
    new_state = state[:i * 8] + u_0 + state[i * 8 + 8:32 + i * 8] + u_1 + state[40 + i * 8:64 + i * 8] + u_2 + state[72 + i * 8:96 + i * 8] + u_3 + state[104 + i * 8:]
    return new_state

def cal_ciphertext(plaintext):
    groups = [plaintext[i:i + 8] for i in range(0, 128, 8)]
    rearranged_groups = []
    for col in range(4):
        for row in range(4):
            rearranged_groups.append(groups[row * 4 + col])
    state= ''.join(rearranged_groups)
    key=''+W[0]+W[1]+W[2]+W[3]
    groups = [key[i:i + 8] for i in range(0, 128, 8)]
    rearranged_groups = []
    for col in range(4):
        for row in range(4):
            rearranged_groups.append(groups[row * 4 + col])
    key = ''.join(rearranged_groups)
    state=''.join(str(int(a) ^ int(b)) for a, b in zip(state, key))

    for t in range(1,NR):

        number = [int(state[i:i + 8], 2) for i in range(0, 128, 8)]
        state=''.join(convert_to_8bit_binary(S_BOX[num]) for num in number)
        string = [state[i * 32:(i + 1) * 32] for i in range(4)]
        i=0
        while i <4:
            str_=string[i]
            j=i
            while j>0:
                str_=rotate_string(str_)
                j=j-1
            string[i]=str_
            i=i+1

        state=''+string[0]+string[1]+string[2]+string[3]
        hex_str = ''
        for i in range(0, 128, 4):
            bin_chunk = state[i:i + 4]
            hex_str += format(int(bin_chunk, 2), 'X')
        i=0
        while i<4:
            state=mixcolumn(state, i)
            i=i+1
        key=''+W[t*4]+W[t*4+1]+W[t*4+2]+W[t*4+3]
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
    key=''+W[40]+W[41]+W[42]+W[43]
    groups = [key[i:i + 8] for i in range(0, 128, 8)]
    rearranged_groups = []
    for col in range(4):
        for row in range(4):
            rearranged_groups.append(groups[row * 4 + col])
    key = ''.join(rearranged_groups)
    state = (''.join(str(int(a) ^ int(b)) for a, b in zip(state, key)))
    return state