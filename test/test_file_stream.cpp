#include "gtest/gtest.h"

#include "rsync/rsync.h"
#include "rsync/file_stream.h"

#include "utility.h"

TEST(FileIStream, FileNotExist)
{
	rsyn::FileIStream fs("idontexist.txt", 3);

	std::vector<byte> data;
	ASSERT_EQ(fs.read_block(data), rsyn::result::file_not_exist);
}

TEST(FileIStream, FileEmpty)
{
	rsyn::FileIStream fs("iexistempty.txt", 3);

	std::vector<byte> data;
	ASSERT_EQ(fs.read_block(data), rsyn::result::eof);
}

TEST(FileIStream, FileExist)
{
	rsyn::FileIStream fs("iexist.txt", 3);

	std::vector<byte> data;
	ASSERT_EQ(fs.read_block(data), rsyn::result::ok);
}

TEST(FileIStream, FileExistEof)
{
	rsyn::FileIStream fs("iexist.txt", 3);

	std::vector<byte> data;
	ASSERT_EQ(fs.read_block(data), rsyn::result::ok);
	byte ch;
	ASSERT_EQ(fs.read_byte(ch), rsyn::result::eof);
}

TEST(FileIStream, ReadByteThenBlock)
{
	rsyn::FileIStream fs("iexist.txt", 3);

	byte ch;
	ASSERT_EQ(fs.read_byte(ch), rsyn::result::ok);

	std::vector<byte> data;
	ASSERT_EQ(fs.read_block(data), rsyn::result::eof);
}

TEST(FileIStream, ReadBlockThenRestart)
{
	rsyn::FileIStream fs("iexist.txt", 3);

	std::vector<byte> data;
	ASSERT_EQ(fs.read_block(data), rsyn::result::ok);

	fs.restart();

	ASSERT_EQ(fs.read_block(data), rsyn::result::ok);
	byte ch;
	ASSERT_EQ(fs.read_byte(ch), rsyn::result::eof);
}

TEST(FileIStream, OStream)
{
	{
		rsyn::FileOStream os("newfile.txt");

		std::vector<byte> data = from_string("abc");
		ASSERT_EQ(os.write(data), rsyn::result::ok);

		data = from_string("def");
		ASSERT_EQ(os.write(data), rsyn::result::ok);

		data = from_string("g");
		ASSERT_EQ(os.write(data), rsyn::result::ok);
	}

	rsyn::FileIStream fs("newfile.txt", 3);

	std::vector<byte> data;
	ASSERT_EQ(fs.read_block(data), rsyn::result::ok);
	ASSERT_EQ(data, std::vector<byte>({{'a', 'b', 'c' }}));

	ASSERT_EQ(fs.read_block(data), rsyn::result::ok);

	byte ch;
	ASSERT_EQ(fs.read_byte(ch), rsyn::result::ok);
	ASSERT_EQ(ch, 'g');
}
