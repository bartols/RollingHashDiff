#include "gtest/gtest.h"

#include "rsync/rsync.h"
#include "mockStream.h"

#include "utility.h"

TEST(Delta, EmptySignature)
{
	std::vector<byte> buffer = from_string("abcdefg");
	MockIStream is(buffer, 3);
	rsyn::Signature sign;

	rsyn::Delta delta;
	ASSERT_TRUE(rsyn::delta(sign, is, delta));

	ASSERT_EQ(delta.dump(), "abcdefg");
}

TEST(Delta, EmptySignatureEmptyFile)
{
	std::vector<byte> buffer;
	MockIStream is(buffer, 3);
	rsyn::Signature sign;

	rsyn::Delta delta;
	ASSERT_TRUE(rsyn::delta(sign, is, delta));

	ASSERT_EQ(delta.dump(), "");
}

TEST(Delta, EmptySignatureLittleFile)
{
	std::vector<byte> buffer = from_string("aa");
	MockIStream is(buffer, 3);
	rsyn::Signature sign;

	rsyn::Delta delta;
	ASSERT_TRUE(rsyn::delta(sign, is, delta));

	ASSERT_EQ(delta.dump(), "aa");
}

TEST(Delta, EqualFile)
{
	std::vector<byte> buffer = from_string("abc");
	MockIStream is(buffer, 3);
	rsyn::Signature sign;
	add_block(sign, from_string("abc"), 0);

	rsyn::Delta delta;
	ASSERT_TRUE(rsyn::delta(sign, is, delta));

	ASSERT_EQ(delta.dump(), "<b*0/>");
}

TEST(Delta, EqualFileDoublePlusNew)
{
	std::vector<byte> buffer = from_string("abcdefg");
	MockIStream is(buffer, 3);
	rsyn::Signature sign;
	add_block(sign, from_string("abc"), 0);
	add_block(sign, from_string("def"), 1);

	rsyn::Delta delta;
	ASSERT_TRUE(rsyn::delta(sign, is, delta));

	ASSERT_EQ(delta.dump(), "<b*0/><b*1/>g");
}

TEST(Delta, DifferentFile)
{
	std::vector<byte> buffer = from_string("abc");
	MockIStream is(buffer, 3);
	rsyn::Signature sign;
	add_block(sign, from_string("def"), 0);

	rsyn::Delta delta;
	ASSERT_TRUE(rsyn::delta(sign, is, delta));

	ASSERT_EQ(delta.dump(), "abc");
}

TEST(Delta, MoreDifferentFile)
{
	std::vector<byte> buffer = from_string("abczwyk");
	MockIStream is(buffer, 3);
	rsyn::Signature sign;
	add_block(sign, from_string("def"), 0);

	rsyn::Delta delta;
	ASSERT_TRUE(rsyn::delta(sign, is, delta));

	ASSERT_EQ(delta.dump(), "abczwyk");
}

TEST(Delta, MoreSameBlock)
{
	std::vector<byte> buffer = from_string("abcabcabc");
	MockIStream is(buffer, 3);
	rsyn::Signature sign;
	add_block(sign, from_string("abc"), 0);

	rsyn::Delta delta;
	ASSERT_TRUE(rsyn::delta(sign, is, delta));

	ASSERT_EQ(delta.dump(), "<b*0/><b*0/><b*0/>");
}

TEST(Delta, MoreSameTwoBlock)
{
	std::vector<byte> buffer = from_string("abcdefabcdefabcdef");
	MockIStream is(buffer, 3);
	rsyn::Signature sign;
	add_block(sign, from_string("abc"), 0);
	add_block(sign, from_string("def"), 1);

	rsyn::Delta delta;
	ASSERT_TRUE(rsyn::delta(sign, is, delta));

	ASSERT_EQ(delta.dump(), "<b*0/><b*1/><b*0/><b*1/><b*0/><b*1/>");
}

TEST(Delta, ChangeBlock)
{
	std::vector<byte> buffer = from_string("wabc");
	MockIStream is(buffer, 3);
	rsyn::Signature sign;
	add_block(sign, from_string("abc"), 0);

	rsyn::Delta delta;
	ASSERT_TRUE(rsyn::delta(sign, is, delta));

	ASSERT_EQ(delta.dump(), "w<b*0/>");
}

TEST(Delta, BlockChanges)
{
	std::vector<byte> buffer = from_string("abcx");
	MockIStream is(buffer, 3);
	rsyn::Signature sign;
	add_block(sign, from_string("abc"), 0);

	rsyn::Delta delta;
	ASSERT_TRUE(rsyn::delta(sign, is, delta));

	ASSERT_EQ(delta.dump(), "<b*0/>x");
}

TEST(Delta, ChangeBlockChange)
{
	std::vector<byte> buffer = from_string("wabcx");
	MockIStream is(buffer, 3);
	rsyn::Signature sign;
	add_block(sign, from_string("abc"), 0);

	rsyn::Delta delta;
	ASSERT_TRUE(rsyn::delta(sign, is, delta));

	ASSERT_EQ(delta.dump(), "w<b*0/>x");
}

TEST(Delta, ChangesBlockChanges)
{
	std::vector<byte> buffer = from_string("wyzabcxyz");
	MockIStream is(buffer, 3);
	rsyn::Signature sign;
	add_block(sign, from_string("abc"), 0);

	rsyn::Delta delta;
	ASSERT_TRUE(rsyn::delta(sign, is, delta));

	ASSERT_EQ(delta.dump(), "wyz<b*0/>xyz");
}

TEST(Delta, BlockChangesBlock)
{
	std::vector<byte> buffer = from_string("abcxyzabc");
	MockIStream is(buffer, 3);
	rsyn::Signature sign;
	add_block(sign, from_string("abc"), 0);

	rsyn::Delta delta;
	ASSERT_TRUE(rsyn::delta(sign, is, delta));

	ASSERT_EQ(delta.dump(), "<b*0/>xyz<b*0/>");
}

TEST(Delta, BlockChangesDifferentBlock)
{
	std::vector<byte> buffer = from_string("abcxyzdef");
	MockIStream is(buffer, 3);
	rsyn::Signature sign;
	add_block(sign, from_string("abc"), 0);
	add_block(sign, from_string("def"), 1);

	rsyn::Delta delta;
	ASSERT_TRUE(rsyn::delta(sign, is, delta));

	ASSERT_EQ(delta.dump(), "<b*0/>xyz<b*1/>");
}

TEST(Delta, AlotOfChanges)
{
	std::vector<byte> buffer = from_string("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Lorem ipsum acta est. Vestibulum eu sapien felis. Aenean id turpis. Vestibulum id turbis.");
	MockIStream is(buffer, 11);
	rsyn::Signature sign;
	add_block(sign, from_string("Lorem ipsum"), 0);
	add_block(sign, from_string("consectetur"), 1);
	add_block(sign, from_string("Vestibulum "), 2);

	rsyn::Delta delta;
	ASSERT_TRUE(rsyn::delta(sign, is, delta));

	ASSERT_EQ(delta.dump(), "<b*0/> dolor sit amet, <b*1/> adipiscing elit. <b*0/> acta est. <b*2/>eu sapien felis. Aenean id turpis. <b*2/>id turbis.");
}
