

#include <gtest/gtest.h>
#include <cstdlib>
#include <iostream>
// #include "gtest/gtest-param-test.h"
// #include <benchmark/benchmark.h>

int add(int a, int b){
    return a + b;
}

bool larger(int a){
  return a > 1;
}

/**
 * 指定全局事件
 */
class FooEnvironment : public testing::Environment
{
public:
    virtual void SetUp() {
        std::cout << "+++++++++++++++++++++++  Foo FooEnvironment SetUP" << std::endl;
    }

    virtual void TearDown() {
        std::cout << "-----------------------  Foo FooEnvironment TearDown" << std::endl;
    }
};

/**
 * 直接测试，Test 宏
 *      没有使用 test suite
 */
TEST(testCase, test1) {
    EXPECT_EQ(4, add(1, 2)) << " - expect failed " << 3;
    ASSERT_EQ(3, add(1, 2));

    /**
     * 比较字符串
     */
    EXPECT_STREQ("111", "222");
}

TEST(testCase, abc) {
    /**
     * 出错时显示参数具体值
     */
    EXPECT_PRED2(add, 1, -1);
    // testing::StaticAssertTypeEq<int, T>();
}

TEST(testCase2, abc) {
  /**
   * 出错时显示参数具体值
   */
  EXPECT_PRED2(add, 1, -1);
  // testing::StaticAssertTypeEq<int, T>();
}

/**
 * 暂时取消的测试
 */
TEST(DISABLED_testCase, ddd) {
    EXPECT_PRED2(add, 1, -1);
}

// ======================================================
// ======================================================
/**
 * test fixture
 *      suit、case 环境
 */
class FooTest : public testing::Test {
protected:
  /**
   * suit 内执行一次
   */
  static void SetUpTestCase() {
    std::cout << "setup test case ------ " << std::endl;
  }
  static void TearDownTestCase() {
  }

  /**
   * 每个 case 执行一次
   */
  virtual void SetUp(){
      std::cout << "setup single case";
  }

  virtual void TearDown() {
  }
};

/**
 * 出错时，trace才会显示
 */
TEST_F(FooTest, t0) {
  SCOPED_TRACE("ERROR1 ====><><><><");
  EXPECT_EQ(0, 0);
  SUCCEED();
}

TEST_F(FooTest, t1) {
  SCOPED_TRACE("ERROR2 ====><><><><");
  EXPECT_EQ(0, 2);
  SUCCEED();
}

// ======================================================
// ======================================================
/**
 * death测试
 *      https://www.cnblogs.com/coderzh/archive/2009/04/08/1432043.html
 */
void FooDeath() {
  int *pInt = 0;
  *pInt = 42 ;
}

void FooAssert() {
   assert(0);
}

int ExitTests() {
  exit(1);
}

/**
 * 测试命名也用这个规范，DeathTest 后缀
 */
TEST(FooDeathTest, Demo)
{
  EXPECT_DEATH(FooDeath(), "");
  EXPECT_DEATH(FooAssert(), "");
}

TEST(NewExitTest, Demo) {
  EXPECT_EXIT(ExitTests(), testing::ExitedWithCode(1), "");
}

// ======================================================
// ======================================================
/**
 * 参数化设置 一
 *  1. 相同类型，值不相同，测试多个值情况下的执行
 *          可以使用Range、Combine
 *          https://github.com/google/googletest/blob/master/googletest/samples/sample7_unittest.cc
 *
 *  注意：INSTANTIATE_TEST_CASE_P is deprecated, please use INSTANTIATE_TEST_SUITE_P
 */
class LargerTest : public::testing::TestWithParam<int> {
};

TEST_P(LargerTest, CheckValue) {
  int n = GetParam();
  EXPECT_EQ(true, larger(n));
}

INSTANTIATE_TEST_SUITE_P(TrueReturnCase, LargerTest,
                         testing::Values(3, 5, 11, 23, 17, 20));

// ======================================================
// ======================================================
/**
 *  参数化设置 二
 *
 *  2. 不同类型，但是这些类型都是已知的，这个稍微简单一些
 *
 *      注册时，只需要指出所有的类型即可
 *      https://github.com/google/googletest/blob/master/googletest/samples/sample6_unittest.cc
 */
template <class T>
class TypeTest1 : public testing::Test {
public:
    T value_;
    bool TypeCheck() {
      return typeid(T) == typeid(bool);
    }

    static int values() { return 2; }
};

/**
 * TYPED_TEST_SUITE：必须使用 typedef
 * TYPED_TEST
 */
typedef testing::Types<bool, int> Implementations;
TYPED_TEST_SUITE(TypeTest1, Implementations);

TYPED_TEST(TypeTest1, Check) {
  /**
   * 这里必须用 this -> 因为是在模板类中
   */
  // TypeParam()
  EXPECT_EQ(TestFixture::values(), 2);
  EXPECT_EQ(true, this->TypeCheck());
}

// ======================================================
// ======================================================
/**
 *  参数化设置 三
 *
 *  3. 不同类型，但是这些类型不是已知的
 *      type-parameterized tests，注册时需要指出所有的 test case
 *      https://github.com/google/googletest/blob/master/googletest/samples/sample6_unittest.cc
 */
template <class T>
class TypeTest2 : public testing::Test {
public:
  T value_;
  bool TypeCheck() {
    return typeid(T) == typeid(bool);
  }

  static int values() { return 2; }
};

/**
 * TYPED_TEST_SUITE_P
 *
 * TYPED_TEST_P
 */
TYPED_TEST_SUITE_P(TypeTest2);

TYPED_TEST_P(TypeTest2, Check1) {
  /**
   * 这里必须用 this -> 因为是在模板类中
   */
  // TypeParam()
  EXPECT_EQ(TestFixture::values(), 2);
  EXPECT_EQ(true, this->TypeCheck());
}

TYPED_TEST_P(TypeTest2, Check2) {
  EXPECT_EQ(TestFixture::values(), 2);
  EXPECT_EQ(true, this->TypeCheck());
}

/**
 * Type-parameterized tests 模式，
 *
 * REGISTER_TYPED_TEST_SUITE_P：这里还没有开始任何真实的测试，并没有任何类型，还是抽象的
 *
 * INSTANTIATE_TYPED_TEST_SUITE_P：注册实际类型
 */
REGISTER_TYPED_TEST_SUITE_P(TypeTest2,        // test case的名字
                            Check1, Check2);  // test 名字


typedef testing::Types<bool, int> TypeLists;
INSTANTIATE_TYPED_TEST_SUITE_P(MyTypeTest2,   // Instance name，任意的名字
                               TypeTest2,     // Test case name
                               TypeLists);    // Type list

// ======================================================
// ======================================================
/**
 * 文档
 *      https://blog.csdn.net/linhai1028/article/details/81675724
 *      https://www.cnblogs.com/aoyihuashao/p/9203479.html
 *
 *      https://www.cnblogs.com/jycboy/p/AdvancedGuide2.html
 *      https://blog.csdn.net/W_Y2010/article/details/92405343
 *
 * 参数：
 *      https://www.cnblogs.com/coderzh/archive/2009/04/10/1432789.html
 *
 *      1. 输出为xml：./client --gtest_output=xml
 *      2. list：    ./client --gtest_list_tests
 *      3. filter:  ./client --gtest_filter=*.test*
 *      4. 立即停止： ./client --gtest_break_on_failure
 *      5. 执行时间：--gtest_print_time
 */
#include <queue>
int main(int argc,char **argv){
    testing::AddGlobalTestEnvironment(new FooEnvironment);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
