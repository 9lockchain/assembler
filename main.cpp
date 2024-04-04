#include "assembler.h"

int main() {
    std::unique_ptr<assembler> c_assembler = std::make_unique<assembler>();

    //simple stub to give execution by putting our target as the RAX register and then placing a JMP to it, calling execution on its cores
    //0x123456789 are our placeholder bytes, which if found, the assembler detects and then prints out
    //but again, this stub is just an example and has nothing to do with our actual program, anything can be input
    std::string mov_rax_jmp_dest = "mov rax, 0x123456789\njmp 0x123456789";

    printf("[asm] given bytes: %s\n", mov_rax_jmp_dest.c_str());
    printf("[asm] crafting shellcode..\n");

    auto result = c_assembler.get()->assemble(mov_rax_jmp_dest);
    //extract the std::pair into usable values
    std::vector<unsigned char> bytes = result.first;
    std::vector<std::string> placeholders = result.second;

    std::cout << "[asm] generated shellcode:";
    for (unsigned char byte : bytes)
        printf("\\x%02x", byte); //using stdio's printf impl here as this is just a simpler way rather than calling std::cout on a std::hex configuration

    std::cout << std::endl;

    //this will only work if an abnormal instr layout is detected (e.g.: 0xsomething, the something will be extracated and then be added to the plcaeholders vector)
    std::cout << "[asm] detected placeholder bytes:";
    for (const std::string& placeholder : placeholders)
        std::cout << placeholder << " ";

    return 0;
}
