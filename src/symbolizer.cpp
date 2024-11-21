#include <iostream>
#include <sched.h>
#include <string>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/personality.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "dwarf/dwarf++.hh"
#include "elf/elf++.hh"
#include "symbolizer.hpp"

using namespace yottabyte;

dwarf::die symbolizer::get_function_from_pc(uint64_t pc) {
  for (auto &cu: m_dwarf.compilation_units()) {
    if (die_pc_range(cu.root()).contains(pc)) {
      for (const auto& die : cu.root()) {
        if (die.tag == dwarf::DW_TAG::subprogram) {
          if ( die_pc_range(die).contains(pc)) {
            return die;
          }
        }
      }
    }
  }

  throw std::out_of_range{"Cannot find function"};
}

dwarf::line_table::iterator symbolizer::get_line_entry_from_pc(uint64_t pc) {
  for (auto &cu : m_dwarf.compilation_units()) {
    if (die_pc_range(cu.root()).contains(pc)) {
      auto &lt = cu.get_line_table();
      auto it = lt.find_address(pc);
      if (it == lt.end()) {
        throw std::out_of_range{"Cannot find line entry"};
      }
      else {
        return it;
      }
    }
  }
  throw std::out_of_range("Cannot find line entry");
}

void symbolizer::parse(uint64_t addr) {
  auto func = get_function_from_pc(addr);
  auto line = get_line_entry_from_pc(addr);

  std::cout << std::hex << addr << " : "<< line->file->path << '#' << dwarf::at_name(func) << '#' << std::dec << line->line << std::endl;

}

int main(int argc, char* argv[]){
  if (argc < 3) {
    std::cerr << "please specify program name with dwarf and addr need to be symbolized" << '\n';
    return -1;
  }

  auto prog = argv[1];

  auto addr = std::stol(argv[2], 0, 16); 

  symbolizer sbl(prog);
  sbl.parse(addr);
}

