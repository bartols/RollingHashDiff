#include "gtest/gtest.h"

#include "rsync/rsync.h"
#include "mockStream.h"

#include <numeric>

#include "utility.h"

// test a mock used in a test ? Yes I'm TDD addicted
TEST(MockIStream, read_block)
{
	std::vector<byte> buffer(26);
	std::iota(buffer.begin(), buffer.end(), 'a');
	MockIStream is(buffer, 3);

	std::vector<byte> block;
	ASSERT_EQ(is.read_block(block), rsyn::result::ok);
	ASSERT_EQ(block, std::vector<byte>({{'a', 'b', 'c' }}));

	ASSERT_EQ(is.read_block(block), rsyn::result::ok);
	ASSERT_EQ(block, std::vector<byte>({{'d', 'e', 'f' }}));
}

TEST(MockIStream, read_odd)
{
	std::vector<byte> buffer(4);
	std::iota(buffer.begin(), buffer.end(), 'a');
	MockIStream is(buffer, 3);

	std::vector<byte> block;
	ASSERT_EQ(is.read_block(block), rsyn::result::ok);
	ASSERT_EQ(block, std::vector<byte>({{'a', 'b', 'c' }}));

	ASSERT_EQ(is.read_block(block), rsyn::result::ok);
	ASSERT_EQ(block, std::vector<byte>({{'d'}}));
}

TEST(MockIStream, read_end)
{
	std::vector<byte> buffer(3);
	std::iota(buffer.begin(), buffer.end(), 'a');
	MockIStream is(buffer, 3);

	std::vector<byte> block;
	ASSERT_EQ(is.read_block(block), rsyn::result::ok);
	ASSERT_EQ(block, std::vector<byte>({{'a', 'b', 'c' }}));

	ASSERT_EQ(is.read_block(block), rsyn::result::eof);
}

TEST(MockIStream, read_block_then_byte)
{
	std::vector<byte> buffer = from_string("abcd");
	MockIStream is(buffer, 3);

	std::vector<byte> block;
	ASSERT_EQ(is.read_block(block), rsyn::result::ok);
	ASSERT_EQ(block, std::vector<byte>({{'a', 'b', 'c' }}));

	byte ch;
	ASSERT_EQ(is.read_byte(ch), rsyn::result::ok);

	ASSERT_EQ(ch, 'd');

	ASSERT_EQ(is.read_byte(ch), rsyn::result::eof);
}

TEST(MockIStream, read_byte_then_block)
{
	std::vector<byte> buffer = from_string("abcd");
	MockIStream is(buffer, 3);

	byte ch;
	ASSERT_EQ(is.read_byte(ch), rsyn::result::ok);

	ASSERT_EQ(ch, 'a');

	std::vector<byte> block;
	ASSERT_EQ(is.read_block(block), rsyn::result::ok);
	ASSERT_EQ(block, std::vector<byte>({{'b', 'c', 'd' }}));

	ASSERT_EQ(is.read_byte(ch), rsyn::result::eof);
}

TEST(MockIStream, read_block_then_byte_then_block)
{
	std::vector<byte> buffer = from_string("abcdefg");
	MockIStream is(buffer, 3);

	std::vector<byte> block;
	ASSERT_EQ(is.read_block(block), rsyn::result::ok);
	ASSERT_EQ(block, std::vector<byte>({{'a', 'b', 'c' }}));

	byte ch;
	ASSERT_EQ(is.read_byte(ch), rsyn::result::ok);

	ASSERT_EQ(ch, 'd');

	ASSERT_EQ(is.read_block(block), rsyn::result::ok);
	ASSERT_EQ(block, std::vector<byte>({{'e', 'f', 'g' }}));

	ASSERT_EQ(is.read_byte(ch), rsyn::result::eof);
}

TEST(MockIStream, empty)
{
	std::vector<byte> buffer;
	std::vector<byte> block;
	MockIStream is(buffer, 3);

	ASSERT_EQ(is.read_block(block), rsyn::result::eof);
}

TEST(MockIStream, bufferLessThanBlockSize)
{
	std::vector<byte> buffer{'a','a'};
	std::vector<byte> block;
	MockIStream is(buffer, 3);

	ASSERT_EQ(is.read_block(block), rsyn::result::ok);
	ASSERT_EQ(block.size(), 2);
}
