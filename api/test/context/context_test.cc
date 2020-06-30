#include "opentelemetry/context/context.h"

#include <map>

#include <gtest/gtest.h>

using opentelemetry::context::Context;

// Tests that the context default constructor does not cause a crash
TEST(ContextTest, ContextDefaultConstructorCrash)
{
  Context context_test = Context();
}

// Tests that the context constructor does not cause a crash when a map
// is provided.
TEST(ContextTest, ContextIterableConstructorCrash)
{
  std::map<std::string, std::string> map_test = {{"test_key","123"}};
  Context context_test = Context(map_test);
}

// Tests that the WriteValues method does not crash
TEST(ContextTest, ContextWriteValuesCrash)
{
  std::map<std::string, std::string> map_test = {{"test_key","123"}};
  std::map<std::string, std::string> map_test_write = {{"foo_key","456"}};
  Context context_test = Context(map_test);
  context_test.WriteValues(map_test_write);
}

// Tests that the CreateKey method does not crash
TEST(ContextTest, ContextCreateKeyCrash)
{
  Context context_test;
  Context::Key test_key = context_test.CreateKey("key_name");
}

// Tests that the GetValue method does not crash
TEST(ContextTest, ContextGetValueCrash)
{
  std::map<std::string, std::string> map_test = {{"test_key","123"}};
  std::map<std::string, std::string> map_test_write = {{"foo_key","456"}};
  Context context_test = Context(map_test);
  Context::Key test_key = context_test.CreateKey("key_name");
  context_test.GetValue(test_key);
}

// Tests that the Context::Key GetIdentifier method does not crash
TEST(ContextTest, ContextKeyGetIdentifierCrash)
{
  Context context_test;
  Context::Key test_key = context_test.CreateKey("key_name");
  EXPECT_EQ(test_key.GetIdentifier(), "");
}

// Tests that the Context::Key GetName method does not crash
TEST(ContextTest, ContextKeyGetNameCrash)
{
  Context context_test;
  Context::Key test_key = context_test.CreateKey("key_name");
  test_key.GetName();
}

