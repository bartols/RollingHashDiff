#pragma once

#include "rsync_defs.h"
#include <vector>

namespace rsyn {

	enum class result {
		ok,
		eof,
	};

	// input stream
	class IStream
	{
	public:
		IStream() = delete;
		/// <summary>
		/// Construct a stream that read in block size
		/// </summary>
		/// <param name="block_size">block size</param>
		explicit IStream(std::size_t block_size)
			:_block_size(block_size) {}
		virtual ~IStream() = default;

		/// <summary>
		/// read a block
		/// </summary>
		/// <param name="block">block read from stream, size is block size or less</param>
		/// <returns>result</returns>
		virtual result read_block(std::vector<byte> & block) const = 0;

		/// <summary>
		/// read a byte
		/// </summary>
		/// <param name="ch">byte read from stream</param>
		/// <returns>result</returns>
		virtual result read_byte(byte& ch) const = 0;

		/// <summary>
		/// restart form head the reading position
		/// </summary>
		virtual void restart() = 0;

		/// <summary>
		/// block size
		/// </summary>
		std::size_t block_size() const { return _block_size; }

	protected:
		std::size_t _block_size;
	};

	// output stream
	class OStream
	{
	public:
		virtual ~OStream() = default;

		virtual bool write(const std::vector<byte>& block) = 0;
	};

	// input/output stream
	class IOStream
	{
	public:
		virtual ~IOStream() = default;

	};

} // namespace rsyn
