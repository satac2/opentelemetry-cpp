#pragma once

#include "opentelemetry/context/context.h"
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
    class Context final : public opentelemetry::context::Context
    {
      public:

        // The Key class is used to obscure access from the
        // user to the context map. The identifier is used as a key
        // to the context map.
        class Key :  opentelemetry::context::Context::Key 
      {

        public:
          std::string GetIdentifier();
          
          std::string GetName();

          Key();

          Key(std::string key_name);

        private:
          friend class Context;

          std::string key_name_;
          
          std::string identifier_;
          //char identifier_[50];
      };

        // The Id class is used to keep track of the next avaiable
        // context id number.
        class Id {
          private:
          friend class Context;

            static std::atomic<int> next_;
            Id();

            int GetNext(); 
        };
        

        /* Creates a key with the passed in name and returns it. */
        Key CreateKey(nostd::string_view key_name);

        Context();

        template <class T, nostd::enable_if_t<trace::detail::is_key_value_iterable<T>::value> * = nullptr>  
          Context(const T &attributes){
            id_ = Id().GetNext();
            key_vals_.insert(attributes.begin(), attributes.end());
          }


        template <class T, nostd::enable_if_t<trace::detail::is_key_value_iterable<T>::value> * = nullptr>
          Context WriteValues(T &attributes)
          { 
            attributes.insert(key_vals_.begin(), key_vals_.end());

            return Context(attributes);
          }

        /* Returns the value associated with the passed in key */
        common::AttributeValue GetValue(Key key);
        
        Context(const Context &other){
          key_vals_ = other.key_vals_;
          id_ = other.id_;
        }

        bool operator==(const Context &other);

      private:
        int id_;
        std::map<std::string, common::AttributeValue> key_vals_;

    };
        std::atomic<int> Context::Id::next_(1);
  }
}
OPENTELEMETRY_END_NAMESPACE

