#include "gtest/gtest.h"

#include "rsync/rsync.h"
#include "mockStream.h"

#include "utility.h"

TEST(RDiff, SignatureEmpty)
{
	std::vector<byte> buffer;
	MockIStream is(buffer, 3);

	rsyn::Signature sign;

	ASSERT_TRUE(rsyn::signature(is, sign));
	ASSERT_TRUE(sign.empty());
}

TEST(RDiff, SignatureLessThanBlockSize)
{
	std::vector<byte> buffer = from_string("aa");
	MockIStream is(buffer, 3);

	rsyn::Signature sign;
	rsyn::Signature sign_res;
	add_block(sign_res, from_string("aa"), 0);

	ASSERT_TRUE(rsyn::signature(is, sign));
	ASSERT_EQ(sign, sign_res);
}

TEST(RDiff, SignatureBlockEqual)
{
	std::vector<byte> buffer = from_string("aaabbbaaabbba");
	MockIStream is(buffer, 3);

	rsyn::Signature sign;

	ASSERT_TRUE(rsyn::signature(is, sign));

	rsyn::Signature sign_res;
	add_block(sign_res, from_string("aaa"), 0);
	add_block(sign_res, from_string("bbb"), 1);
	add_block(sign_res, {'a'}, 4);

	ASSERT_EQ(sign, sign_res);
}
