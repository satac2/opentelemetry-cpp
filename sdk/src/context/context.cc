
#include "opentelemetry/sdk/context/context.h"
#include "opentelemetry/common/attribute_value.h"

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <atomic>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
  namespace context
  {
    // Context::Key Methods
    
    // Returns the Key identifier string
    std::string Context::Key::GetIdentifier() { return std::string(identifier_); }
    
    // Returns the Key name string_view
    nostd::string_view Context::Key::GetName() { return key_name_; }

    Context::Key::Key() = default;
    
    Context::Key::Key(nostd::string_view key_name) : key_name_{key_name}
    {
      std::stringstream ss;
      ss << (void *)this;
      nostd::string_view temp_view;
      temp_view = ss.str();

      memcpy(identifier_, temp_view.data(), temp_view.size());
    }
    
    // Context::Id Methods
    
    Context::Id::Id() = default;
    
    //Returns the next available context id and increments the counter
    int Context::Id::GetNext(){ 
      return next_++;
    }

    // Context methods

    /* Creates a key with the passed in name and returns it. */
    Context::Key Context::CreateKey(nostd::string_view key_name) { return Key(key_name); }

    Context::Context(){
      id_ =  0;
    }
    
    /* Returns the value associated with the passed in key */
    common::AttributeValue Context::GetValue(Key key)
    {
      return key_vals_[key.GetIdentifier()];
    }

    bool Context::operator==(const Context &other){
      return (id_ == other.id_);
    }
  }
}
OPENTELEMETRY_END_NAMESPACE

