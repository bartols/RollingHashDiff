

#include "rsync.h"

#include "rolling_checksum.h"

#ifdef _DEBUG
//#define DEBUG_VERBOSE
#endif

using namespace rsyn;

// calculate signature from basis_file stream
bool rsyn::signature(const IStream& basis_file, Signature& signature)
{
	int block_idx = 0;
	RollingChecksum rcsum;

	std::vector<byte> block;
	while (basis_file.read_block(block) == result::ok)
	{
		// last part of file
		if (block.size() < basis_file.block_size())
			continue;

		// weak signature
		auto weak_sig = rcsum.calculate(block);

		// strong signature
		auto strong_sig = strong_signature(block);

		// search block
		auto entry = signature.get(weak_sig);

		// if not present
		if (!entry.valid())
		{
			signature.add(weak_sig, strong_sig, block_idx);
			++block_idx;
		}
		// if different checksum
		else if (strong_sig != entry.strong)
		{
			// something strange happens
			return false;
		}
	}
	return true;
}

// using signature create a delta class from new file
bool rsyn::delta(const Signature& signature, const IStream& new_file, Delta& delta)
{
	RollingChecksum rcsum;
	bool roll = false;
	int64_t check_sum;
	std::vector<byte> block;
	byte remove_byte{0};

	while (true)
	{
		if (roll)
		{
			byte add_byte;
			// read byte
			if (new_file.read_byte(add_byte) != result::ok)
			{
				// add remaining block
				delta.add_changes(block);
				break;
			}

			check_sum = rcsum.roll(remove_byte, add_byte);
			
			// update block
			block.push_back(add_byte);
		}
		else
		{
			// read block
			if (new_file.read_block(block) != result::ok)
				break;
			check_sum = rcsum.calculate(block);
		}

		roll = false;

		// search block
		const auto& entry = signature.get(check_sum);
		// if found
		if (entry.valid())
		{
			// check strong signature
			auto strong_sig = strong_signature(block);
			if (strong_sig == entry.strong)
			{
				// add existing block
				delta.add_block(entry.index);
				continue;
			}
		}

		// if not found (or strong not match) rool 1 byte
		roll = true;
		remove_byte = block[0];
		block.erase(block.begin());
		delta.add_byte(remove_byte);
	}

	return true;
}

// using signature create a delta file from new file
bool rsyn::delta(const Signature& signature, const IStream& new_file, OStream& delta_file)
{
	return false;
}

// patch using signature and delta class to produce output_file
bool rsyn::patch(const Signature& signature, const IStream& basis_file, const Delta& delta, OStream& output_file)
{
	// create original part db
	std::vector<strOriginalPart> original_part;
	if (!extract_original_parts(signature, basis_file, original_part))
		return false;

	// for each part of Delta
	for (const auto& part : delta)
	{
		// if an old part
		if (std::holds_alternative<int>(part))
		{
			// find in original part
			auto index = std::get<0>(part);
			auto it = std::find_if(original_part.begin(), original_part.end(), [index](const auto& part) {
				return part.index == index;
				});
			if (it == original_part.end())
				return false;

			output_file.write((*it).block);;
		}
		else
		// if new
		{
			output_file.write(std::get<1>(part));
		}
	}
	return true;
}

// patch basis_file using delta_file to produce output_file (recalculate signature)
bool rsyn::patch(const IStream& basis_file, const IStream& delta_file, OStream& output_file)
{
	return false;
}

