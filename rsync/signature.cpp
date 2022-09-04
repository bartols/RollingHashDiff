#include "signature.h"

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
