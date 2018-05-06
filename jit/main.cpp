#include <iostream>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <sys/mman.h>
#include <string>
#include "CodeWithCycles.h"

const std::vector<byte_type> code_example_with_cycles = { // NOLINT
        
        /* 
          int x = 0; 
        */
        
        0x55, // 	pushq	%rbp
        0x48, 0x89, 0xe5, // 	movq	%rsp, %rbp
        0xc7, 0x45, 0xfc, 0x00, 0x00, 0x00, 0x00, // 	movl	$0, -4(%rbp)
        
        /* 
          for (int i = 0; i < XXXX; ++i) { 
        		x += i;
          }
        */
        
        0xc7, 0x45, 0xf8, 0x00, 0x00, 0x00, 0x00, //	movl	$0, -8(%rbp)
        0x83, 0x7d, 0xf8, 0x05, //	cmpl	$5, -8(%rbp)
        0x0f, 0x8d, 0x17, 0x00, 0x00, 0x00, //	jge	23 <__Z1fv+0x33>
        0x8b, 0x45, 0xf8, //	movl	-8(%rbp), %eax
        0x03, 0x45, 0xfc, //	addl	-4(%rbp), %eax
        0x89, 0x45, 0xfc, //	movl	%eax, -4(%rbp)
        0x8b, 0x45, 0xf8, //	movl	-8(%rbp), %eax
        0x83, 0xc0, 0x01, //	addl	$1, %eax
        0x89, 0x45, 0xf8, //	movl	%eax, -8(%rbp)
        0xe9, 0xdf, 0xff, 0xff, 0xff, //	jmp	-33 <__Z1fv+0x12>
        
        /* 
          for (int j = 0; j < 100; ++j) {
		        x += j;
	        }
        */
        
        0xc7, 0x45, 0xf4, 0x00, 0x00, 0x00, 0x00, // 	movl	$0, -4(%rbp)
        0x83, 0x7d, 0xf4, 0x64, //	cmpl	$5, -8(%rbp)
        0x0f, 0x8d, 0x17, 0x00, 0x00, 0x00, //	jge	23 <__Z1fv+0x33>
        0x8b, 0x45, 0xf4, //	movl	-8(%rbp), %eax
        0x03, 0x45, 0xfc, //	addl	-4(%rbp), %eax
        0x89, 0x45, 0xfc, //	movl	%eax, -4(%rbp)
        0x8b, 0x45, 0xf4, //	movl	-8(%rbp), %eax
        0x83, 0xc0, 0x01, //	addl	$1, %eax
        0x89, 0x45, 0xf4, //	movl	%eax, -8(%rbp)
        0xe9, 0xdf, 0xff, 0xff, 0xff, //	jmp	-33 <__Z1fv+0x12>
        
        /* 
          return x; 
        */
        
        0x8b, 0x45, 0xfc, //	movl	-4(%rbp), %eax
        0x5d, //	popq	%rbp
        0xc3,  //retqa
  
};
const int iterations_offset = 21;

std::vector<byte_type> GetCode(int iterations_count) {
  auto code = code_example_with_cycles;
  code[iterations_offset] = static_cast<byte_type>(iterations_count);
  return code;
}

int main() {
  std::cout << "enter number of iterations of the main cycle : " << std::endl;
  int iterations;
  while (true) {
    std::cin >> iterations;
    if (iterations > 220 || iterations < 0) {
      std::cout << "please, enter positive number in [0..222]" << std::endl;
    } else {
      break;
    }
  }
  std::cout
          << "program contains 2 cycles and calculates 0+1+2+...+(N-1)  +  0+1+2+...+99"
          << std::endl;
  auto code = CodeWithCycles(GetCode(iterations));
  auto execute = [&code] {
    std::cout << "executing..." << std::endl;
    std::cout << "return code : " << code.Execute() << std::endl;
  };
  execute();
  std::cout << "unwrapping cycles..." << std::endl;
  code.FindForLoopsAndUnwrap();
  std::cout << "all found cycles successfully unwrapped !!!" << std::endl;
  execute();
  return 0;
}