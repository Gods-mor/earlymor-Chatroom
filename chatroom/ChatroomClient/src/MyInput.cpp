#include "MyInput.h"

bool dataInput(int& data) {
    if (!(std::cin >> data)) {
        // 清除错误状态和缓冲区
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "输入无效。" << std::endl;
        sleep(1);
        std::cout << "\033[A"
                  << "\33[2K\r"
                  << "\033[A"
                  << "\33[2K\r" << std::flush;
        return false;
    }
    std::cin.get();
    return true;
}

bool dataInput(std::string& data) {
    if (!(std::cin >> data)) {
        // 清除错误状态和缓冲区
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "输入无效。" << std::endl;
        sleep(1);
        std::cout << "\033[A"
                  << "\33[2K\r"
                  << "\033[A"
                  << "\33[2K\r" << std::flush;
        return false;
    }
    std::cin.get();
    return true;
}