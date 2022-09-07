#pragma once

#include "streams.h"

#include <string>
#include <fstream>

namespace rsyn {

	// Input stream from file
	class FileIStream : public IStream
	{
	public:
		FileIStream(const std::string& file_name, std::size_t block_size);

		// read a block
		result read_block(std::vector<byte>& block) const override;

		// read a byte
		result read_byte(byte& ch) const override;

		// restart form head the reading position
		void restart() override;

	private:
		mutable std::ifstream _stream;
		mutable bool _eof = false;
		result read(byte* data, std::size_t len, std::size_t& read_len) const;
	};

	// Output stream
	class FileOStream : public OStream
	{
	public:
		FileOStream(const std::string& file_name);
	
		// write block
		result write(const std::vector<byte>& block) override;
	private:
		std::ofstream _stream;
	};

} // namespace rsyn

