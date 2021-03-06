#ifndef COMPASS_RT_X86_IMPL_H_
#define COMPASS_RT_X86_IMPL_H_

#include "detail/ct/detect_os.hpp"
#include "detail/ct/detect_compiler.hpp"
#include "detail/ct/detect_arch.hpp"

#include "detail/rt/x86_cpuid.hpp"
#include "detail/tags.hpp"
#include "detail/bit_view.hpp"
#include "detail/definitions.hpp"

#include <iostream>
#include <string>


namespace compass {

  namespace runtime {

    namespace detail {


      static bool works(ct::x86_tag) {

        auto regs = rt::cpuid(0);

        if(regs.size())
          return true;
        else
          return false;

      }


      static std::string vendor(ct::x86_tag) {


        std::array<std::uint32_t,4> regs = rt::cpuid_to_int(0);

        std::string vendor_name = "";

        if(!regs.empty()){
          vendor_name.resize(3*4);

          std::copy(reinterpret_cast<char*>(&regs[ct::ebx]),reinterpret_cast<char*>(&regs[ct::ebx])+4,
                    vendor_name.begin());
          std::copy(reinterpret_cast<char*>(&regs[ct::edx]),reinterpret_cast<char*>(&regs[ct::edx])+4,
                    vendor_name.begin()+4);
          std::copy(reinterpret_cast<char*>(&regs[ct::ecx]),reinterpret_cast<char*>(&regs[ct::ecx])+4,
                    vendor_name.begin()+8);
        }

        return vendor_name;


      }

      static std::string brand(ct::x86_tag) {

        std::string value = "";
        auto regs = rt::cpuid_to_int(0x80000000);
        if(regs[ct::eax] >= 0x80000004){

          value.resize(48);
          char* value_begin = &value[0];
          for(std::uint32_t i = 2; i<5;++i){
            auto ret = rt::cpuid_to_int(0x80000000 + i);

            for(std::uint32_t r = 0; r<4;++r){
              std::uint32_t* tgt = reinterpret_cast<std::uint32_t*>(value_begin + (i-2)*16u + r*4u);
              *tgt = ret[r];
            }

          }

        }

        return value;

      }


      static std::string device_name(ct::x86_tag) {

        std::string brand_str = compass::runtime::detail::brand(ct::x86_tag());
        std::string vendor = compass::runtime::detail::vendor(ct::x86_tag());
        std::size_t find_pos = 0;
        if((find_pos = vendor.find("Genuine"))!=std::string::npos){
            vendor.erase(find_pos,7);
        }

        std::string value = "";

        if(brand_str.find(vendor) != std::string::npos){
        //based on the Intel chip test strings that are known
          auto second_bracket_itr = brand_str.rfind(")");
          auto last_at_itr = brand_str.rfind("@");
          value = brand_str.substr(second_bracket_itr+1,last_at_itr-(second_bracket_itr+1));

          if((find_pos = value.find(" CPU "))!=std::string::npos){
            value.erase(find_pos,5);
          }

          if((find_pos = value.find(" CPU "))!=std::string::npos){
            value.erase(find_pos,5);
          }

          value.erase(remove_if(value.begin(), value.end(), isspace), value.end());
        }
        return value;

      }


      static bool has(feature::sse , ct::x86_tag){

        std::array<std::bitset<32>,4> regs = rt::cpuid(1);

        if(regs.empty()){
          std::cerr << "unsupported cpuid level detected\n";
        }

        bool value = regs[ct::edx].test(25);
        return value;
      }

      static bool has(feature::sse2 , ct::x86_tag){

        std::array<std::bitset<32>,4> regs = rt::cpuid(1);

        if(regs.empty()){
          std::cerr << "unsupported cpuid level detected\n";
        }

        bool value = regs[ct::edx].test(26);
        return value;
      }

      static bool has(feature::sse3 , ct::x86_tag){

        std::array<std::bitset<32>,4> regs = rt::cpuid(1);

        if(regs.empty()){
          std::cerr << "unsupported cpuid level detected\n";
        }

        bool value = regs[ct::ecx].test(9) | regs[ct::ecx].test(0) | regs[ct::ecx].test(3);
        return value;
      }


      static bool has(feature::sse4 , ct::x86_tag){

        std::array<std::bitset<32>,4> regs = rt::cpuid(1);

        if(regs.empty()){
          std::cerr << "unsupported cpuid level detected\n";
        }

        bool value = regs[ct::ecx].test(19) | regs[ct::ecx].test(20);

        return value;
      }

      static bool has(feature::avx , ct::x86_tag){

        std::array<std::bitset<32>,4> regs = rt::cpuid(1);

        if(regs.empty()){
          std::cerr << "unsupported cpuid level detected\n";
        }

        bool value = regs[ct::ecx].test(28);

        return value;
      }

      static bool has(feature::avx2 , ct::x86_tag){

        auto regs = rt::cpuid_to_int(7,0,0,0);

        bool value = compass::utility::bit_view<std::uint32_t>(regs[ct::ebx]).test(5);

        return value;
      }

    };
  };

};
#endif /* COMPASS_RT_X86_IMPL_H_ */
