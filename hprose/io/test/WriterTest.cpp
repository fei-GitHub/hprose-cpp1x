/**********************************************************\
|                                                          |
|                          hprose                          |
|                                                          |
| Official WebSite: http://www.hprose.com/                 |
|                   http://www.hprose.org/                 |
|                                                          |
\**********************************************************/

/**********************************************************\
 *                                                        *
 * WriterTest.cpp                                         *
 *                                                        *
 * hprose writer test for cpp.                            *
 *                                                        *
 * LastModified: Oct 11, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/io/Writer.h>

#include <gtest/gtest.h>

TEST(Writer, WriteNull) {
    std::ostringstream stream;
    hprose::io::Writer writer(stream);
    writer.writeNull();
    EXPECT_EQ(stream.str(), "n");
}

TEST(Writer, WriteBool) {
    std::ostringstream stream;
    hprose::io::Writer writer(stream);
    writer.writeBool(true);
    EXPECT_EQ(stream.str(), "t");
    stream.str("");
    writer.writeBool(false);
    EXPECT_EQ(stream.str(), "f");
}

TEST(Writer, SerializeBool) {
    std::ostringstream stream;
    hprose::io::Writer writer(stream);
    writer.serialize(true);
    EXPECT_EQ(stream.str(), "t");
    stream.str("");
    writer.serialize(false);
    EXPECT_EQ(stream.str(), "f");
}

TEST(Writer, WriteDigit) {
    std::ostringstream stream;
    hprose::io::Writer writer(stream);
    for (int i = 0; i <= 9; i++) {
		stream.str("");
		writer.writeInteger(i);
	}
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
