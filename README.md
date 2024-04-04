# assembler
versatile c++14 64-bit shellcode crafter/assembler. [theoretically c++17 due to the unique_ptr and make_unique calls though this isnt really important and those werent implemented in that version]

only a few instructions and registers are suppported, preferably for the 64-bit set but 32-bit also works fine on some configurations (immediate bytes such as imm64 have been made compatible)

this project is not really comparable to any big assembly tools such as asmjit, capstone, zydis or such, but suopport is easily addable ad it is just a fun project to test things around
