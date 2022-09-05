#pragma once

#include "streams.h"
#include "signature.h"
#include "delta.h"

namespace rsyn {

	// calculate signature from basis_file stream
	bool signature(const IStream& basis_file, Signature& signature);

	// using signature create a delta class from new file
	bool delta(const Signature& signature, const IStream& new_file, Delta& delta);

	// using signature create a delta file from new file
	bool delta(const Signature& signature, const IStream& new_file, OStream& delta_file);

	// patch using signature and delta class to produce output_file
	bool patch(const Signature& signature, const IStream& basis_file, const Delta& delta, OStream& output_file);

	// patch basis_file using delta_file to produce output_file (recalculate signature)
	bool patch(const IStream& basis_file, const IStream& delta_file, OStream& output_file);

} // namespace rsyn