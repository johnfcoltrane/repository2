#include <gtest/gtest.h>

// ����2�s�� gtest/gtest.h �ɓ���Ƃ��΃��C�u���������N������Ȃ�����
#pragma comment( lib, "gtestd.lib" )
#pragma comment( lib, "gtest_maind.lib" )

TEST(GTestSample, Assert) {
	EXPECT_EQ(1, 1);
	EXPECT_EQ(2, 2); // 2, 3 
}

int main(int argc, char* argv[]){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
	return 0;
}