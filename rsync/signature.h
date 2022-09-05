#pragma once

#include <unordered_map>
#include <string>
#include <cstdint>

#include "streams.h"

namespace rsyn {
	
	// block data
	struct BlockData {
		std::string strong;
		int index = 0;

		bool valid() const	{ return !strong.empty(); }

		bool operator == (const BlockData& other) const
		{
			return strong == other.strong &&
				index == other.index;
		}
	};

	// signature
	class Signature
	{
	public:
		// get strong checksum from weak
		const BlockData& get(int64_t id) const;

		// get block number by strong checksum
		int getBlockIndex(const std::string& strong) const;

		// add a block
		void add(int64_t id, const std::string& strong, int index);

		// equal operator
		bool operator == (const Signature& other) const;

		// is empty
		bool empty() const		{ return _sign_map.empty(); }
	private:
		std::unordered_map<int64_t, BlockData> _sign_map;
	};

	// todo move to another file
	
	// calculate a strong signature
	std::string strong_signature(const std::vector<byte>& block);

	// an original part
	struct strOriginalPart {
		int index = 0;
		std::vector<byte> block;		// todo easily optimization using position on file/stream
	};
	// todo vector but maybe a better container (flat map ?)

	// parse a file and construct a list of original parts
	bool extract_original_parts(const Signature& sign, const IStream& stream, std::vector<strOriginalPart>& original_part);

} // namespace rsyn