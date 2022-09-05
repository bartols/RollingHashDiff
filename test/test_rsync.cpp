#include "gtest/gtest.h"

#include "rsync/rsync.h"
#include "mockStream.h"

#include "utility.h"

#ifdef _DEBUG
//#define DEBUG_VERBOSE
#endif

#ifdef DEBUG_VERBOSE
#include <iostream>
#endif

class RSyncTest : public ::testing::Test {
protected:
	void apply_patch(const std::string& original, const std::string& modified, std::size_t block_size)
	{
		// signature
		std::vector<byte> data_original = from_string(original);
		MockIStream is_orig(data_original, block_size);
		rsyn::Signature sign;
		ASSERT_TRUE(rsyn::signature(is_orig, sign));

		// Delta
		std::vector<byte> data_modified = from_string(modified);
		MockIStream is_mod(data_modified, block_size);
		rsyn::Delta delta;
		ASSERT_TRUE(rsyn::delta(sign, is_mod, delta));

		// patch
		MockOStream os;
		is_orig.restart();
		ASSERT_TRUE(rsyn::patch(sign, is_orig, delta, os));
		ASSERT_EQ(os.buffer(), modified);

#ifdef DEBUG_VERBOSE
		std::cout << "Original :" << original << std::endl;
		std::cout << "Modified :" << modified << std::endl;
		std::cout << "Delta    :" << delta.dump() << std::endl;
		std::cout << "Result   :" << os.buffer() << std::endl;
#endif
	}
};

TEST_F(RSyncTest, EqualFile)
{
	apply_patch("abc", "abc", 3);
}

TEST_F(RSyncTest, BlockChange)
{
	apply_patch("abc", "abcd", 3);
}

TEST_F(RSyncTest, BlockChangeLong)
{
	apply_patch("abc", "abcdefffffffffff", 3);
}

TEST_F(RSyncTest, ChangeBlock)
{
	apply_patch("abc", "ddabc", 3);
}

TEST_F(RSyncTest, BlockChangeBlock)
{
	apply_patch("abc", "abcddabc", 3);
}

TEST_F(RSyncTest, ChangeBlockChange)
{
	apply_patch("abc", "wwwwabczzzz", 3);
}

TEST_F(RSyncTest, TwoBlock)
{
	apply_patch("abcdef", "abcdef", 3);
}

TEST_F(RSyncTest, TwoBlockChange)
{
	apply_patch("abcdef", "abcdefd", 3);
}

TEST_F(RSyncTest, longText)
{
	apply_patch("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Lorem ipsum acta est. Vestibulum eu sapien felis. Aenean id turpis. Vestibulum id turbis.", 
				"Lorem ipsum sit amet, consectetur dolor adipiscing elit. Gaude Lorem ipsum acta est. Sapien felis. Aenean id turpis. Vestibulum id turbis.", 11);
}


