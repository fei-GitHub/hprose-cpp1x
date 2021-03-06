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
 * LastModified: Oct 19, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/io/Writer.h>

#include <gtest/gtest.h>

#include <random>

#define T(value, expected) { \
    std::ostringstream stream; \
    hprose::io::Writer writer(stream, true); \
    writer.serialize(value); \
    EXPECT_EQ(stream.str(), expected); \
}

TEST(Writer, SerializeNull) {
    T(nullptr, "n");
}

TEST(Writer, SerializeBool) {
    T(true, "t");
    T(false, "f");
}

TEST(Writer, SerializeDigit) {
    for (int i = 0; i <= 9; i++) {
        T(i, std::to_string(i));
    }
}

TEST(Writer, SerializeInteger) {
    std::random_device rd;

    std::uniform_int_distribution<int32_t> dis1(10); 
    for (int i = 0; i < 100; i++) {
        int32_t x = dis1(rd);
        T(x, "i" + std::to_string(x) + ";");
    }

    std::uniform_int_distribution<int64_t> dis2(static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1);
    for (int i = 0; i < 100; i++) {
        int64_t x = dis2(rd);
        T(x, "l" + std::to_string(x) + ";");
    }
}

TEST(Writer, SerializeInt8) {
    for (int8_t i = 0; i <= 9; i++) {
        T(i, std::to_string(i));
    }

    for (int8_t i = 9; i < 127; i++) {
        int8_t x = i + 1;
        T(x, "i" + std::to_string(x) + ";");
    }

    for (int8_t i = -128; i < 0; i++) {
        T(i, "i" + std::to_string(i) + ";");
    }
}

TEST(Writer, SerializeFloat) {
    T(std::numeric_limits<float>::quiet_NaN(), "N");
    T(std::numeric_limits<float>::infinity(), "I+");
    T(-std::numeric_limits<float>::infinity(), "I-");
    T(3.14159265358979323846f, "d3.14159;");

    T(std::numeric_limits<double>::quiet_NaN(), "N");
    T(std::numeric_limits<double>::infinity(), "I+");
    T(-std::numeric_limits<double>::infinity(), "I-");
    T(3.14159265358979323846, "d3.14159265358979;");

    T(std::numeric_limits<long double>::quiet_NaN(), "N");
    T(std::numeric_limits<long double>::infinity(), "I+");
    T(-std::numeric_limits<long double>::infinity(), "I-");
    T(3.14159265358979323846l, "d3.14159265358979324;");
}

TEST(Writer, SerializeComplex) {
    T(std::complex<float>(100.f), "d100;");
    T(std::complex<float>(0, 100.f), "a2{d0;d100;}");

    T(std::complex<double>(100.f), "d100;");
    T(std::complex<double>(0, 100.f), "a2{d0;d100;}");

    T(std::complex<long double>(100.f), "d100;");
    T(std::complex<long double>(0, 100.f), "a2{d0;d100;}");
}

TEST(Writer, SerializeString) {
    T(std::string(u8""), "e");
    T(std::string(u8"π"), "uπ");
    T(std::string(u8"你"), "u你");
    T(std::string(u8"你好"), R"(s2"你好")");
    T(std::string(u8"你好啊,hello!"), R"(s10"你好啊,hello!")");
    T(std::string(u8"🇨🇳"), "s4\"🇨🇳\"");
    T(std::string("\x80\x81\x82"), std::string("b3\"\x80\x81\x82\""));

    T(std::wstring(L""), "e");
    T(std::wstring(L"π"), "uπ");
    T(std::wstring(L"你"), "u你");
    T(std::wstring(L"你好"), R"(s2"你好")");
    T(std::wstring(L"你好啊,hello!"), R"(s10"你好啊,hello!")");
    T(std::wstring(L"🇨🇳"), R"(s4"🇨🇳")");

    T(std::u16string(u""), "e");
    T(std::u16string(u"π"), "uπ");
    T(std::u16string(u"你"), "u你");
    T(std::u16string(u"你好"), R"(s2"你好")");
    T(std::u16string(u"你好啊,hello!"), R"(s10"你好啊,hello!")");
    T(std::u16string(u"🇨🇳"), R"(s4"🇨🇳")");

    T(std::u32string(U""), "e");
    T(std::u32string(U"π"), "uπ");
    T(std::u32string(U"你"), "u你");
    T(std::u32string(U"你好"), R"(s2"你好")");
    T(std::u32string(U"你好啊,hello!"), R"(s10"你好啊,hello!")");
    T(std::u32string(U"🇨🇳"), R"(s4"🇨🇳")");
}

TEST(Writer, SerializeList) {
    std::array<int, 3> a1{ {1, 2, 3} };
    std::array<double, 3> a2{ {1, 2, 3} };
    std::array<bool, 3> a3{ {true, false, true} };
    std::array<int, 0> a4{};
    std::array<bool, 0> a5{};
    T(a1, "a3{123}");
    T(a2, "a3{d1;d2;d3;}");
    T(a3, "a3{tft}");
    T(a4, "a{}");
    T(a5, "a{}");

    T(std::vector<uint8_t>({'h', 'e', 'l', 'l', 'o'}), R"(b5"hello")");
    T(std::vector<uint8_t>(), R"(b"")");

    T(std::vector<int>({1, 2, 3}), "a3{123}");
    T(std::vector<double>({1, 2, 3}), "a3{d1;d2;d3;}");
    T(std::vector<bool>({true, false, true}), "a3{tft}");
    T(std::vector<int>(), "a{}");
    T(std::vector<bool>(), "a{}");

    T(std::deque<int>({1, 2, 3}), "a3{123}");
    T(std::forward_list<int>({1, 2, 3}), "a3{123}");
    T(std::list<int>({1, 2, 3}), "a3{123}");
    T(std::set<int>({1, 2, 3}), "a3{123}");
    T(std::multiset<int>({1, 2, 3}), "a3{123}");
}

TEST(Writer, SerializeTuple) {
    T(std::make_tuple(), "a{}");
    T(std::make_tuple(1, 3.14, true), "a3{1d3.14;t}");
}

TEST(Writer, SerializeMap) {
    std::map<int, float> map;
    T(map, "m{}");
    map[4] = 4.13;
    map[9] = 9.24;
    map[1] = 1.09;
    T(map, "m3{1d1.09;4d4.13;9d9.24;}");
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
