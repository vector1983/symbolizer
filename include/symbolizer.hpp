#ifndef YOTTABYTE_SYMBOLIZER_HPP
#define YOTTABYTE_SYMBOLIZER_HPP

#include <cstdint>
#include <utility>
#include <string>
#include <linux/types.h>

#include "dwarf/dwarf++.hh"
#include "elf/elf++.hh"

namespace yottabyte {
    class symbolizer {
    public:
        symbolizer (std::string dwarf_file_name)
             : m_dwarf_file_name{std::move(dwarf_file_name)} {
            auto fd = open(m_dwarf_file_name.c_str(), O_RDONLY);

            m_elf = elf::elf{elf::create_mmap_loader(fd)};
            m_dwarf = dwarf::dwarf{dwarf::elf::create_loader(m_elf)};
        }
        
        void parse(uint64_t);

    private:
        std::string m_dwarf_file_name;
        dwarf::dwarf m_dwarf;
        elf::elf m_elf;
        auto get_function_from_pc(uint64_t pc) -> dwarf::die;
        auto get_line_entry_from_pc(uint64_t pc) -> dwarf::line_table::iterator;
    };
}

#endif
