#include "assembler.h"

std::vector<std::string> assembler::split(const std::string& s, char delimiter)
{
    if (s.empty())
        return std::vector<std::string>();

    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

//how does this work?
//well for 64-bit registers, for example RDI
//we extract the REX prefix for 64-bit operands, in this case 0x48 for rax,rdi,rsi,etc
//and then the OPCODE for mov r64, imm64 (immediate value/holder), which in this case is 0xBF for RDI, but differs as for example on RSI it is set to 0xBE ( ;) )
std::pair<std::vector<unsigned char>, std::vector<std::string>> assembler::assemble(const std::string& assembly)
{
    static bool indirect_jmp; // edit manually
    std::vector<unsigned char> bytes;
    std::vector<std::string> placeholder_lines;

    //split asm instr by every newline (std::endl)
    std::vector<std::string> lines = this->split(assembly, '\n');

    //process the input
    for (const std::string& line : lines) {
        std::vector<std::string> tokens = this->split(line, ' ');
        if (tokens.size() >= 2) {
            std::string mnemonic = tokens[0];
            std::string operand = tokens[1];
            if (mnemonic == "mov") {
                if (operand == "rax,") {
                    bytes.push_back(0x48);
                    bytes.push_back(0xb8);
                }
                else if (operand == "rdi,") {
                    bytes.push_back(0x48);
                    bytes.push_back(0xbf);
                }
                else if (operand == "rsi,") {
                    bytes.push_back(0x48);
                    bytes.push_back(0xbe);
                }


                else {
                    std::cerr << "[asm] unsupported register: " << operand << std::endl;
                    return {};
                }
                //extract the immediate value (64-bit)
                std::string imm_str = tokens[2];
                if (imm_str.substr(0, 2) == "0x") {
                    placeholder_lines.push_back(imm_str);
                    imm_str = "0"; //placeholder
                }
                uint64_t imm;
                std::stringstream ss;
                ss << std::hex << imm_str;
                ss >> imm;
                //copy the immediate value holder
                unsigned char* p_imm = reinterpret_cast<unsigned char*>(&imm);
                for (int i = 0; i < 8; ++i) {
                    bytes.push_back(p_imm[i]);
                }
            }
            else if (mnemonic == "jmp") {
                //relative jmp with 32-bit displacement
                if(!indirect_jmp)
                    bytes.push_back(0xe9);
                else
                {
                    bytes.push_back(0xff);
                    bytes.push_back(0x25);
                }


                //extract the displacement holder/value
                std::string disp_str = operand;
                if (disp_str.substr(0, 2) == "0x") {
                    placeholder_lines.push_back(disp_str);
                    disp_str = "0"; // Placeholder
                }
                int32_t disp;
                std::stringstream ss;
                ss << std::hex << disp_str;
                ss >> disp;

                //copy the displacement holder
                unsigned char* p_disp = reinterpret_cast<unsigned char*>(&disp);
                for (int i = 0; i < 4; ++i) {
                    bytes.push_back(p_disp[i]);
                }
            }
            else {
                std::cerr << "[asm] unsupported mnemonic: " << mnemonic << std::endl;
                return std::pair<std::vector<unsigned char>, std::vector<std::string>>();
            }
        }
    }

    return { bytes, placeholder_lines };
}
