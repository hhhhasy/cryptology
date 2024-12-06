#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <iomanip>

#include <sstream>

using namespace std;
const int S_BOX[256] = {
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
};
const int NR = 10;

const uint32_t RCON[10] = {
    0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1B000000,
    0x36000000
};

std::string hex_to_128bit_binary(const std::string& hex_str) {
    std::string binary_str;
    for (char c : hex_str) {
        std::bitset<4> b(std::stoi(std::string(1, c), nullptr, 16));
        binary_str += b.to_string();
    }
    return binary_str;
}

std::string convert_to_8bit_binary(int num) {
    std::bitset<8> b(num);
    return b.to_string();
}

std::string sub_bytes(const std::string& temp) {
    std::string result;
    for (int i = 0; i < 32; i += 8) {
        int num = std::stoi(temp.substr(i, 8), nullptr, 2);
        result += convert_to_8bit_binary(S_BOX[num]);
    }
    return result;
}

std::string rotate_string(const std::string& s) {
    return s.substr(8) + s.substr(0, 8);
}

std::vector<std::string> generate_key(const std::string& K) {
    std::vector<std::string> W;
    for (int i = 0; i < 4; ++i) {
        W.push_back(K.substr(32 * i, 32));
    }
    for (int i = 4; i < 44; ++i) {
        std::string temp = W[i - 1];
        if (i % 4 == 0) {
            temp = rotate_string(temp);
            temp = sub_bytes(temp);
            std::bitset<32> temp_bit(temp);
            temp_bit ^= RCON[(i / 4) - 1];
            temp = temp_bit.to_string();
        }
        std::string new_word;
        for (size_t j = 0; j < 32; ++j) {
            new_word += std::to_string((W[i - 4][j] - '0') ^ (temp[j] - '0'));
        }
        W.push_back(new_word);
    }
    return W;
}

// filedmult_2 等价函数
std::string filedmult_2(const std::string& byte_str) {
    int byte = std::stoi(byte_str, nullptr, 2);
    int result = byte << 1;
    if (result & 0x100) {
        result ^= 0x1B;
    }
    result &= 0xFF;
    return convert_to_8bit_binary(result);
}

// filedmult_3 等价函数
std::string filedmult_3(const std::string& byte_str) {
    int byte = std::stoi(byte_str, nullptr, 2);
    int result = std::stoi(filedmult_2(byte_str), nullptr, 2) ^ byte;
    return convert_to_8bit_binary(result);
}

// mixcolumn 等价函数
std::string mixcolumn(const std::string& state, int i) {

    std::string s_0 = state.substr(i * 8, 8);
    std::string s_1 = state.substr(i * 8 + 32, 8);
    std::string s_2 = state.substr(i * 8 + 64, 8);
    std::string s_3 = state.substr(i * 8 + 96, 8);

    std::string u_0, u_1, u_2, u_3;
    for (size_t j = 0; j < 8; ++j) {
        u_0 += std::to_string((filedmult_2(s_0)[j] - '0') ^ (filedmult_3(s_1)[j] - '0') ^ (s_2[j] - '0') ^ (s_3[j] - '0'));
        u_1 += std::to_string((filedmult_2(s_1)[j] - '0') ^ (filedmult_3(s_2)[j] - '0') ^ (s_3[j] - '0') ^ (s_0[j] - '0'));
        u_2 += std::to_string((filedmult_2(s_2)[j] - '0') ^ (filedmult_3(s_3)[j] - '0') ^ (s_0[j] - '0') ^ (s_1[j] - '0'));
        u_3 += std::to_string((filedmult_2(s_3)[j] - '0') ^ (filedmult_3(s_0)[j] - '0') ^ (s_1[j] - '0') ^ (s_2[j] - '0'));
    }

    return state.substr(0, i * 8) + u_0 + state.substr(i * 8 + 8, 24) + u_1 + state.substr(i * 8 + 40, 24) + u_2 + state.substr(i * 8 + 72, 24) + u_3 + state.substr(i * 8 + 104);
}

std::string rearrange(std::string data) {
    std::vector<std::string> groups;
    for (size_t i = 0; i < data.length(); i += 8) {
        groups.push_back(data.substr(i, 8));
    }

    std::vector<std::string> rearranged_groups;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            rearranged_groups.push_back(groups[row * 4 + col]);
        }
    }

    std::string rearranged;
    for (const auto& grp : rearranged_groups) {
        rearranged += grp;
    }
    return rearranged;
}

// cal_ciphertext 等价函数
std::string cal_ciphertext(const std::string& plaintext, const std::vector<std::string>& W) {
    std::string state = rearrange(plaintext);
    std::string key = W[0] + W[1] + W[2] + W[3];
    key = rearrange(key);
    for (size_t i = 0; i < state.size(); ++i) {
        state[i] = (state[i] == key[i]) ? '0' : '1';
    }
    for (int t = 1; t < NR; ++t) {

        std::vector<int> number;
        for (size_t i = 0; i < 128; i += 8) {
            number.push_back(std::stoi(state.substr(i, 8), nullptr, 2));
        }
        state = "";
        for (int num : number) {
            state += convert_to_8bit_binary(S_BOX[num]);
        }
        std::vector<std::string> string;
        for (size_t i = 0; i < 4; ++i) {
            std::string str_ = state.substr(i * 32, 32);
            for (int j = i; j > 0; --j) {
                str_ = str_.substr(8) + str_.substr(0, 8);
            }
            string.push_back(str_);
        }
        state = string[0] + string[1] + string[2] + string[3];
        for (int i = 0; i < 4; ++i) {
            state = mixcolumn(state, i);

        }
        key = W[t * 4] + W[t * 4 + 1] + W[t * 4 + 2] + W[t * 4 + 3];
        key = rearrange(key);
        for (size_t i = 0; i < state.size(); ++i) {
            state[i] = (state[i] == key[i]) ? '0' : '1';
        }
    }

    std::vector<int> number;
    for (size_t i = 0; i < 128; i += 8) {
        number.push_back(std::stoi(state.substr(i, 8), nullptr, 2));
    }


    state = "";
    for (int num : number) {
        state += convert_to_8bit_binary(S_BOX[num]);
    }


    std::vector<std::string> string;
    for (size_t i = 0; i < 4; ++i) {
        std::string str_ = state.substr(i * 32, 32);
        for (int j = i; j > 0; --j) {
            str_ = str_.substr(8) + str_.substr(0, 8);
        }
        string.push_back(str_);
    }
    state = string[0] + string[1] + string[2] + string[3];

    key = W[40] + W[41] + W[42] + W[43];
    key = rearrange(key);
    for (size_t i = 0; i < state.size(); ++i) {
        state[i] = (state[i] == key[i]) ? '0' : '1';
    }

    return state;
}

std::string binaryToHex(const std::string& binary) {
    std::stringstream ss;
    ss << std::uppercase; // 设置为大写输出
    for (std::size_t i = 0; i < binary.length(); i += 4) {
        // 每四位二进制转换为一个十六进制数
        unsigned int val = std::bitset<4>(binary.substr(i, 4)).to_ulong();
        ss << std::hex << val;
    }
    return ss.str();
}

int main() {
    std::string hex_string;
    std::string plaintext;
    cin >> hex_string;
    cin >> plaintext;



    std::string binary_string = hex_to_128bit_binary(hex_string);
    std::string plaintext_string = hex_to_128bit_binary(plaintext);
    std::vector<std::string> W = generate_key(binary_string);
    std::string c = cal_ciphertext(plaintext_string, W);

    std::string binary_result = rearrange(c);

    std::string result = binaryToHex(binary_result);
    cout << result;

    return 0;
}