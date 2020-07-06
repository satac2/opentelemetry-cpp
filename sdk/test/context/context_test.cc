#include "opentelemetry/sdk/context/context.h"

#include <gtest/gtest.h>

namespace nostd   = opentelemetry::nostd;
namespace common  = opentelemetry::common;
namespace context = opentelemetry::sdk::context;

// Test ensurs that the context object doe not change when you write to it */
TEST(ContextTest, ContextImmutability)
{

  using M                       = std::map<std::string, common::AttributeValue /*std::string*/>;
  context::Context test_context = context::Context();

  context::Context::Key test_key = test_context.CreateKey("test_key");
  context::Context::Key foo_key  = test_context.CreateKey("test_key");

  M m1 = {{std::string(test_key.GetIdentifier()), "123"},
          {std::string(foo_key.GetIdentifier()), "456"}};

  context::Context foo_context = test_context.WriteValues(m1);

  EXPECT_EQ(nostd::get<nostd::string_view>(foo_context.GetValue(test_key)), "123");
  EXPECT_EQ(nostd::get<nostd::string_view>(foo_context.GetValue(foo_key)), "456");

  EXPECT_THROW(nostd::get<nostd::string_view>(test_context.GetValue(test_key)),
               nostd::bad_variant_access);
  EXPECT_THROW(nostd::get<nostd::string_view>(test_context.GetValue(foo_key)),
               nostd::bad_variant_access);
}

// Tests whether the new Context Objects inherits the keys and values
// of the original context object.
TEST(ContextTest, ContextInheritance)
{

  using M                       = std::map<std::string, common::AttributeValue>;
  context::Context test_context = context::Context();

  context::Context::Key test_key  = test_context.CreateKey("test_key");
  context::Context::Key foo_key   = test_context.CreateKey("foo_key");
  context::Context::Key other_key = test_context.CreateKey("other_key");

  M m1 = {{std::string(test_key.GetIdentifier()), "123"},
          {std::string(foo_key.GetIdentifier()), "456"}};
  M m2 = {{std::string(other_key.GetIdentifier()), "000"}};

  context::Context foo_context   = test_context.WriteValues(m1);
  context::Context other_context = foo_context.WriteValues(m2);

  EXPECT_EQ(nostd::get<nostd::string_view>(other_context.GetValue(test_key)), "123");
  EXPECT_EQ(nostd::get<nostd::string_view>(other_context.GetValue(foo_key)), "456");
}

// Tests that when you add a key value pair where the key is already in
// existance, they key is overwritten.
TEST(ContextTest, ContextKeyOverwrite)
{

  using M                       = std::map<std::string, common::AttributeValue>;
  context::Context test_context = context::Context();

  context::Context::Key test_key  = test_context.CreateKey("test_key");
  context::Context::Key foo_key   = test_context.CreateKey("foo_key");
  context::Context::Key other_key = test_context.CreateKey("other_key");

  M m1 = {{std::string(test_key.GetIdentifier()), "123"},
          {std::string(foo_key.GetIdentifier()), "456"}};
  M m2 = {{std::string(test_key.GetIdentifier()), "000"}};

  context::Context foo_context   = test_context.WriteValues(m1);
  context::Context other_context = foo_context.WriteValues(m2);

  EXPECT_EQ(nostd::get<nostd::string_view>(other_context.GetValue(test_key)), "000");
  EXPECT_NE(nostd::get<nostd::string_view>(other_context.GetValue(test_key)), "123");
}

// Tests that copying a context copies the key value pairs properly.
TEST(ContextTest, ContextCopyBasic)
{

  using M                       = std::map<std::string, common::AttributeValue>;
  context::Context test_context = context::Context();

  context::Context::Key test_key  = test_context.CreateKey("test_key");
  context::Context::Key foo_key   = test_context.CreateKey("foo_key");
  context::Context::Key other_key = test_context.CreateKey("other_key");

  M m1 = {{std::string(test_key.GetIdentifier()), "123"},
          {std::string(foo_key.GetIdentifier()), "456"}};
  M m2 = {{std::string(other_key.GetIdentifier()), "000"}};

  context::Context foo_context    = test_context.WriteValues(m1);
  context::Context other_context  = foo_context.WriteValues(m2);
  context::Context copied_context = other_context;

  EXPECT_EQ(nostd::get<nostd::string_view>(copied_context.GetValue(test_key)), "123");
  EXPECT_EQ(nostd::get<nostd::string_view>(copied_context.GetValue(foo_key)), "456");
}

// Tests that the comparison compares properly.
TEST(ContextTest, ContextCompareAssignedContexts)
{

  using M                       = std::map<std::string, common::AttributeValue>;
  context::Context test_context = context::Context();

  context::Context::Key test_key  = test_context.CreateKey("test_key");
  context::Context::Key foo_key   = test_context.CreateKey("foo_key");
  context::Context::Key other_key = test_context.CreateKey("other_key");

  M m1 = {{std::string(test_key.GetIdentifier()), "123"},
          {std::string(foo_key.GetIdentifier()), "456"}};
  M m2 = {{std::string(other_key.GetIdentifier()), "000"}};

  context::Context foo_context    = test_context.WriteValues(m1);
  context::Context other_context  = foo_context.WriteValues(m2);
  context::Context copied_context = other_context;

  EXPECT_TRUE(copied_context == other_context);
  EXPECT_FALSE(copied_context == foo_context);
}

// Tests that two empty lists return false when compared
TEST(ContextTest, ContextCompareEmptyContexts)
{
  context::Context test_context = context::Context();
  context::Context foo_context  = context::Context();

  EXPECT_FALSE(test_context == foo_context);
}

// Tests that comparison after a copy constructor returns true
TEST(ContextTest, ContextCompareCopiedContext)
{
  using M = std::map<std::string, common::AttributeValue>;

  context::Context test_context = context::Context();

  context::Context::Key test_key = test_context.CreateKey("test_key");
  context::Context::Key foo_key  = test_context.CreateKey("foo_key");

  M m1 = {{std::string(test_key.GetIdentifier()), "123"},
          {std::string(foo_key.GetIdentifier()), "456"}};

  context::Context foo_context = test_context.WriteValues(m1);
  context::Context other_context(foo_context);

  EXPECT_TRUE(other_context == foo_context);
}
