#include "opentelemetry/sdk/context/context.h"
#include "opentelemetry/common/attribute_value.h"

#include <atomic>
#include <map>
#include <sstream>
#include <string>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace context
{
// Context::Key Methods //

// Returns the Key identifier string
std::string Context::Key::GetIdentifier()
{
  return std::string(identifier_);
}

// Returns the Key name string_view
std::string Context::Key::GetName()
{
  return name_;
}

Context::Key::Key() = default;

// Constructs a new context with the object's memory address as
// the identifier.
Context::Key::Key(std::string name) : name_{name}
{
  std::stringstream ss;
  ss << (void *)this;
  nostd::string_view temp_view;
  identifier_ = ss.str();
}

// Context::Id Methods //

Context::Id::Id() = default;

// Returns the next available context id and increments the counter
int Context::Id::GetNext()
{
  return next_++;
}

// Context methods //

// Creates a key with the passed in name and returns it.
Context::Key Context::CreateKey(std::string key_name)
{
  return Key(key_name);
}

Context::Context()
{
  id_ = Id().GetNext();
}

// Returns the value associated with the passed in key.
common::AttributeValue Context::GetValue(Key key)
{
  return key_vals_[key.GetIdentifier()];
}

Context::Context(const Context &other)
{
  key_vals_ = other.key_vals_;
  id_       = other.id_;
}

// The comparison operator makes use of the id_ which is assigned
// to a new value at object construction.
bool Context::operator==(const Context &other)
{
  return (id_ == other.id_);
}
}  // namespace context
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
