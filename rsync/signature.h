#pragma once

#include <unordered_map>
#include <string>
#include <cstdint>

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

		// add a block
		void add(int64_t id, const std::string& strong, int index);

		// equal operator
		bool operator == (const Signature& other) const;

		// is empty
		bool empty() const		{ return _sign_map.empty(); }
	private:
		std::unordered_map<int64_t, BlockData> _sign_map;
	};

} // namespace rsyn