#include <iostream>
#include <string>
#include <cstdlib>
#include "IntArray.h"

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: ./multiplyByXGenerator.out <positive_constant>\n";
        return 1;
    }

    int x = std::atoi(argv[1]);
    if (x <= 0) {
        std::cerr << "Constant must be positive.\n";
        return 1;
    }

    std::string constStr = argv[1];
    std::string funcName = "multiplyBy" + constStr;
    int nameLength = funcName.length();

    std::string mangled =
        "_Z" + std::to_string(nameLength) +
        funcName +
        "P8IntArray";

    std::cout << ".file\t\"multiplyBy" << constStr << ".cpp\"\n";
    std::cout << "\t.text\n";
    std::cout << "\t.globl\t" << mangled << "\n";
    std::cout << "\t.type\t" << mangled << ", @function\n";
    std::cout << mangled << ":\n";
    std::cout << ".LFB0:\n";
    std::cout << "\t.cfi_startproc\n";
    std::cout << "\tendbr64\n";
    std::cout << "\tcmpl\t$0, (%rdi)\n";
    std::cout << "\tjle\t.L1\n";
    std::cout << "\tmovl\t$0, %eax\n";
    std::cout << ".L3:\n";
    std::cout << "\tmovq\t8(%rdi), %rdx\n";
    std::cout << "\tleaq\t(%rdx,%rax,4), %rdx\n";
    std::cout << "\timull\t$" << x << ", (%rdx), %ecx\n";
    std::cout << "\tmovl\t%ecx, (%rdx)\n";
    std::cout << "\taddq\t$1, %rax\n";
    std::cout << "\tcmpl\t%eax, (%rdi)\n";
    std::cout << "\tjg\t.L3\n";
    std::cout << ".L1:\n";
    std::cout << "\tret\n";
    std::cout << "\t.cfi_endproc\n";
    std::cout << ".LFE0:\n";
    std::cout << "\t.size\t" << mangled << ", .-" << mangled << "\n";

    return 0;
}