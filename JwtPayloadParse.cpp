#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

// Base64URL 디코딩 함수
std::string base64_url_decode(const std::string& input) {
    std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz"
                               "0123456789+/";
    std::string result = input;
    for (char& c : result) {
        if (c == '-') c = '+';
        else if (c == '_') c = '/';
    }
    
    // 패딩 추가
    while (result.size() % 4 != 0) {
        result += '=';
    }

    std::vector<unsigned char> decoded_data((result.size() * 3) / 4);
    int val = 0, valb = -8;
    size_t idx = 0;
    for (unsigned char c : result) {
        if (base64_chars.find(c) == std::string::npos) break;
        val = (val << 6) + base64_chars.find(c);
        valb += 6;
        if (valb >= 0) {
            decoded_data[idx++] = (val >> valb) & 0xFF;
            valb -= 8;
        }
    }
    return std::string(decoded_data.begin(), decoded_data.begin() + idx);
}

// JWT 페이로드 디코딩 함수
std::string decode_jwt_payload(const std::string& jwt) {
    size_t pos1 = jwt.find('.');
    size_t pos2 = jwt.find('.', pos1 + 1);
    
    if (pos1 == std::string::npos || pos2 == std::string::npos) {
        throw std::invalid_argument("Invalid JWT format");
    }
    
    std::string payload_base64 = jwt.substr(pos1 + 1, pos2 - pos1 - 1);
    return base64_url_decode(payload_base64);
}

int main() {
    std::string jwt = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
                      "eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIyfQ."
                      "SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c";
    
    try {
        std::string payload = decode_jwt_payload(jwt);
        std::cout << "Decoded Payload: " << payload << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error decoding JWT: " << e.what() << std::endl;
    }

    return 0;
}
