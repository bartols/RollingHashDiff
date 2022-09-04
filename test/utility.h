#pragma once

#include "rsync/rsync.h"
#include "rsync/sha1.h"
#include "rsync/rolling_checksum.h"

#include <strstream>

inline std::vector<byte> from_string(const char* str)
{
	return {str, str + strlen(str)};
}

inline void add_block(rsyn::Signature& sign_res, const std::vector<byte>& data, int index)
{
	rsyn::RollingChecksum rcsum;
	auto weak_sig = rcsum.calculate(data);

	// strong signature
	std::istrstream stream(reinterpret_cast<const char*>(data.data()), data.size());
	SHA1 sha1;
	sha1.update(stream);

	auto strong_sig = sha1.final();

	sign_res.add(weak_sig, strong_sig, index);
}
