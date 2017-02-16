#ifndef COMPASS_CT_MSVC_IMPL_H_

#include "detail/ct/detect_compiler.hpp"

#ifdef COMPASS_CT_COMP_MSVC

#define COMPASS_CT_MSVC_IMPL_H_

#include "intrin.h"

#include <array>
#include <bitset>
#include <cstdint>


#include "detail/tags.hpp"
#include "detail/definitions.hpp"

namespace compass {

  namespace runtime {

    

    static std::array<std::uint32_t,4> cpuid_to_int(std::uint32_t level,
                                                    std::uint32_t in_eax = 0,
                                                    std::uint32_t in_ebx = 0,
                                                    std::uint32_t in_ecx = 0,
                                                    std::uint32_t in_edx = 0){
		std::array<std::uint32_t, 4> regs = { in_eax,
				in_ebx,
				in_ecx,
				in_edx
		};

		std::int32_t* converted_regs = reinterpret_cast<std::int32_t*>(&regs[0]);
		__cpuid(converted_regs,
			(std::int32_t)level);
      
		return regs;
    }

  };



};

#endif /* COMPASS_CT_COMP_MSVC */
#endif /* COMPASS_CT_MSVC_IMPL_H_ */