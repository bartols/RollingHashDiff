#include "gtest/gtest.h"

#include "rsync/rsync.h"
#include "mockStream.h"

#include "utility.h"

TEST(Patch, EqualFile)
{
	rsyn::Delta delta;
	delta.add_block(0);
	rsyn::Signature sign;
	add_block(sign, from_string("abc"), 0);

	std::vector<byte> data = from_string("abc");
	MockIStream is(data, 3);
	MockOStream os;
	ASSERT_TRUE(rsyn::patch(sign, is, delta, os));
	ASSERT_EQ(os.buffer(), "abc");
}