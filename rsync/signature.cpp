#include "signature.h"

#include "sha1.h"

#include <strstream>

using namespace rsyn;

#ifdef _DEBUG
//#define DEBUG_VERBOSE
#endif

#ifdef DEBUG_VERBOSE
#include <iostream>
#endif

const BlockData& Signature::get(int64_t id) const
{
	static BlockData null_block;
	const auto it = _sign_map.find(id);
	if (it == _sign_map.end())
	{
#ifdef DEBUG_VERBOSE
		std::cout << "Signature::get id:" << id << " not found" << std::endl;
#endif
		return null_block;
	}

#ifdef DEBUG_VERBOSE
	std::cout << "Signature::get id:" << id << " found strong:" << it->second.strong << " index:" << it->second.index << std::endl;
#endif

	return it->second;
}

// get block number by strong checksum
int Signature::getBlockIndex(const std::string& strong) const
{
	for (const auto& it : _sign_map)
	{
		if (it.second.strong == strong)
			return it.second.index;
	}

	return -1;
}

void Signature::add(int64_t id, const std::string& strong, int index)
{
#ifdef DEBUG_VERBOSE
	std::cout << "Signature::add id:" << id << " strong:" << strong << " index:" << index << std::endl;
#endif

	_sign_map[id] = {strong, index};
}

bool Signature::operator == (const Signature& other) const
{
	return _sign_map == other._sign_map;
}

std::string rsyn::strong_signature(const std::vector<byte>& block)
{
	std::istrstream stream(reinterpret_cast<const char*>(block.data()), block.size());
	SHA1 sha1;
	sha1.update(stream);
	return sha1.final();
}


// parse a file and construct a list of original parts
bool rsyn::extract_original_parts(const Signature& sign, const IStream& stream, std::vector<strOriginalPart>& original_part)
{
	std::vector<byte> block;
	while (stream.read_block(block) == result::ok)
	{
		// strong signature
		auto strong_sig = strong_signature(block);
		auto index = sign.getBlockIndex(strong_sig);
		if (index != -1)
		{
			strOriginalPart p{index, block};
			original_part.push_back(std::move(p));
		}
	}

	return true;
}
