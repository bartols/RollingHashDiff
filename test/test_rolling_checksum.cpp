#include "gtest/gtest.h"

#include "rsync/rolling_checksum.h"

#include "utility.h"

TEST(RollingChecksum, TestEmpty)
{
	//std::vector<byte> data{0, 1, 2};
	std::vector<byte> data;
	rsyn::RollingChecksum rc;
	
	EXPECT_EQ(rc.calculate(data), 1);
}

TEST(RollingChecksum, TestRoll)
{
	int64_t c1, c2;

	{
		std::vector<byte> buffer = from_string("abc");
		rsyn::RollingChecksum rc;

		c1 = rc.calculate(buffer);
	}

	{
		rsyn::RollingChecksum rc;
		std::vector<byte> buffer = from_string("wab");
		c2 = rc.calculate(buffer);
		c2 = rc.roll('w', 'c');
 	}


	EXPECT_EQ(c1, c2);
}

TEST(RollingChecksum, TestRoll2)
{
	int64_t c1, c2;

	{
		std::vector<byte> buffer = from_string("abc");
		rsyn::RollingChecksum rc;

		c1 = rc.calculate(buffer);
	}

	{
		rsyn::RollingChecksum rc;
		std::vector<byte> buffer = from_string("wzy");
		c2 = rc.calculate(buffer);
		c2 = rc.roll('w', 'a');
		c2 = rc.roll('z', 'b');
		c2 = rc.roll('y', 'c');
	}


	EXPECT_EQ(c1, c2);
}
