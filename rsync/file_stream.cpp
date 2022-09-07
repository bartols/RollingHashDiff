#include "file_stream.h"

using namespace rsyn;

FileIStream::FileIStream(const std::string& file_name, std::size_t block_size)
	:IStream(block_size),
	_stream(file_name, std::ifstream::binary)
{
}

// read a block
result FileIStream::read_block(std::vector<byte>& block) const
{
	block.resize(block_size());
	std::size_t read_len;
	
	// read
	auto res = read(block.data(), block_size(), read_len);

	// if error
	if (res != result::ok)
	{
		return res;
	}

	block.resize(read_len);
	return res;
}

// read a byte
result FileIStream::read_byte(byte& ch) const
{
	std::size_t read_len;
	return read(&ch, 1, read_len);
}

// restart form head the reading position
void FileIStream::restart()
{
	_eof = false;
	_stream.seekg(0, std::ios::beg);
}

result FileIStream::read(byte* data, std::size_t len, std::size_t& read_len) const
{
	read_len = 0;
	if (!_stream.is_open())
		return result::file_not_exist;

	if (_eof || _stream.eof())
		return result::eof;

	_stream.read(reinterpret_cast<char*>(data), len);
	read_len = _stream.gcount();

	if (_stream.eof() )
	{
		if(read_len == 0)
			return result::eof;

		// to return eof on next read
		_eof = true;
		return result::ok;
	}

	if (_stream.fail())
		return result::error;

	return result::ok;
}


FileOStream::FileOStream(const std::string& file_name)
	:_stream(file_name, std::ios::out | std::ios::binary | std::ios::trunc)
{
}

// write block
result FileOStream::write(const std::vector<byte>& block)
{
	if (!_stream.is_open())
		return result::error;

	_stream.write(reinterpret_cast<const char*>(block.data()), block.size());

	return _stream.good() ? result::ok : result::error;
}
