#pragma once

#include "rsync/streams.h"

class MockIStream : public rsyn::IStream
{
public:
	MockIStream(const std::vector<byte>& data, std::size_t block_size)
		:IStream(block_size),
		_input_data(data)
	{
	}

	rsyn::result read_block(std::vector<byte>& block) const override
	{
		if(_input_data.size() < block_size() )
			return rsyn::result::eof;

		if(_position >= _input_data.size())
			return rsyn::result::eof;

		std::size_t dim = _position + block_size() > _input_data.size() ?
			_input_data.size() - _position : block_size();

		block.clear();
		block.reserve(dim);
		block.insert(block.begin(), _input_data.begin() + _position, _input_data.begin() + _position + dim);
		_position += dim;

		return rsyn::result::ok;
	}

	rsyn::result read_byte(byte& ch) const override
	{
		if (_position >= _input_data.size())
			return rsyn::result::eof;

		ch = _input_data[_position];
		++_position;
		return rsyn::result::ok;
	}

	void restart() override { _position = 0; }

private:
	const std::vector<byte>& _input_data;
	mutable std::size_t _position = 0;
};

class MockOStream : public rsyn::OStream
{
public:
	rsyn::result write(const std::vector<byte>& block) override
	{
		_buffer.append(block.begin(), block.end());
		return rsyn::result::ok;
	}

	const std::string& buffer() const	{ return _buffer; }
private:
	std::string _buffer;
};