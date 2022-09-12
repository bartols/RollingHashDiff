#pragma once

#include "rsync_defs.h"

#include <cstdint>
#include <vector>

#define ADLER32_ALGO

#if defined(ADLER32_ALGO)

namespace rsyn {
	// based on https://github.com/lemire/rollinghashcpp
	class RollingChecksum
	{
	public:
		// calulate checksum for data
		int64_t calculate(const std::vector<byte>& data);

		// roll the checksum dicarding remove and including add
		int64_t roll(byte remove, byte add);
	private:
		std::size_t _block_size = 0;
		int32_t _sum1 = 0;
		int32_t _sum2 = 0;
	};

} // namespace rsyn

#else

namespace rsyn {

	// based on https://en.wikipedia.org/wiki/Adler-32
	class RollingChecksum
	{
	public:
		// calulate checksum for data
		int64_t calculate(const std::vector<byte>& data);

		// roll the checksum dicarding remove and including add
		int64_t roll(byte remove, byte add);
	private:
		std::size_t m_block_size = 0;

		int64_t m_r1 = 0;
		int64_t m_r2 = 0;
	};

} // namespace rsyn


#endif // RABIN_KARP_ALGO