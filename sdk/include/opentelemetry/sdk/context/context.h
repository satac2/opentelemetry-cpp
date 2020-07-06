#pragma once

#include "opentelemetry/common/attribute_value.h"
#include "opentelemetry/context/context.h"

#include <atomic>
#include <map>
#include <string>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace context
{
class Context final : public opentelemetry::context::Context
{
public:
  // The Key class is used to obscure access from the
  // user to the context map. The identifier is used as a key
  // to the context map.
  class Key : opentelemetry::context::Context::Key
  {

  public:
    // Returns the key identifier.
    std::string GetIdentifier();

    // Returns the key name.
    std::string GetName();

  private:
    friend class Context;

    Key();

    Key(std::string name);

    std::string name_;

    std::string identifier_;
  };

  // The Id class is used to keep track of the next avaiable
  // context id number.
  class Id
  {
  private:
    friend class Context;

    // Contains the next available context id number.
    static std::atomic<int> next_;

    Id();

    // Returns the next available context id number.
    int GetNext();
  };

  // Creates a key with the passed in name and returns it.
  Key CreateKey(std::string key_name);

  Context();

  // Constructs a new context object with the passed in keys and values.
  template <class T, nostd::enable_if_t<trace::detail::is_key_value_iterable<T>::value> * = nullptr>
  Context(const T &attributes)
  {
    id_ = Id().GetNext();
    key_vals_.insert(attributes.begin(), attributes.end());
  }

  // Accepts an iterable, combines it with the existing iterable
  // and constructs a new context object contains the two combined.
  template <class T, nostd::enable_if_t<trace::detail::is_key_value_iterable<T>::value> * = nullptr>
  Context WriteValues(T &attributes)
  {
    T attr_copy = attributes;
    attr_copy.insert(key_vals_.begin(), key_vals_.end());

    return Context(attr_copy);
  }

  // Returns the value associated with the passed in key.
  common::AttributeValue GetValue(Key key);

  Context(const Context &other);

  bool operator==(const Context &other);

private:
  int id_;

  std::map<std::string, common::AttributeValue> key_vals_;
};
std::atomic<int> Context::Id::next_(1);
}  // namespace context
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
