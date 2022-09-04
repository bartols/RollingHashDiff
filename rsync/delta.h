#pragma once

#include <string>
#include <cstdint>

#include <vector>
#include <variant>

#include "rsync_defs.h"

#ifdef _DEBUG
//#define DEBUG_VERBOSE
#endif

#ifdef DEBUG_VERBOSE
#include <iostream>
#include <string>
#endif

namespace rsyn {

	class Delta
	{
	public:

		// add changes part
		void add_changes(const std::vector<byte>& block)
		{
#ifdef DEBUG_VERBOSE
			std::string str(block.begin(), block.end());
			std::cout << "add_changes: [" << str << "]" << std::endl;
#endif
			_parts.emplace_back(block);
		}
		
		// add a byte to an original part
		void add_byte(byte b)
		{
#ifdef DEBUG_VERBOSE
			std::cout << "add_byte: [" << b << "]" << std::endl;
#endif
			if (_parts.empty())
			{
				add_changes({b});
				return;
			}

			auto& last = _parts.back();
			if (std::holds_alternative<int>(last))
			{
				add_changes({b});
			}
			else
			{
				std::get<1>(last).push_back(b);
			}
		}

		// add original part
		void add_block(int index)
		{
#ifdef DEBUG_VERBOSE
			std::cout << "add_block: [" << index << "]" << std::endl;
#endif
			_parts.emplace_back(index);
		}

		// only for test pourpouse
		std::string dump() const
		{
			std::string res;
			for (const auto& part : _parts)
			{
				if (std::holds_alternative<int>(part))
				{
					res += "<b*" + std::to_string(std::get<0>(part)) + "/>";
				}
				else
				{
					const auto& block = std::get<1>(part);
					res += std::string(block.begin(), block.end());
				}
			}
			return res;
		}
	private:
		std::vector<std::variant<int, std::vector<byte>>> _parts;
	};

} // namespace rsyn
