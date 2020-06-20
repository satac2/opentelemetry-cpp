#pragma once

//#include <bits/stdc++.h>
#include <map>
#include <vector>
#include <sstream> 
#include <iostream>
#include <atomic>

#include "opentelemetry/nostd/utility.h"
#include "opentelemetry/nostd/string_view.h"
#include "opentelemetry/nostd/variant.h"
#include "opentelemetry/common/attribute_value.h"
#include "opentelemetry/version.h"
#include "opentelemetry/trace/key_value_iterable_view.h"
#include "opentelemetry/trace/key_value_iterable.h"
//#include "opentelemetry/context/key_value_iterable_modifiable.h"
#include "opentelemetry/trace/tracer.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace context
{



  /* The context class provides a context identifier */
  template<class T>
    class Context
    {

      public:

        /*The Key class is used to obscure access from the 
         * user to the context map. The identifier is used as a key 
         * to the context map.
         */ 
        class Key
        {

          public:
            
            nostd::string_view GetIdentifier() {return nostd::string_view(str_data);}

            nostd::string_view GetName() { return key_name_; }

            /* Constructs a new Key with the passed in name */
            Key(nostd::string_view key_name) : key_name_{key_name}
             {
              
              std::stringstream ss;
              ss << (void *)this;
              nostd::string_view test_view = "test";
              test_view = ss.str();
              
              memcpy(str_data, test_view.data(), test_view.size());
            }

          private:
              friend class Context;

            nostd::string_view key_name_;
            
            char str_data [50];

            const nostd::string_view identifier_;

            bool operator == (const Key& key){
              if(identifier_ == key.identifier_){
                return true;
              }
              return false;
            }
        };
        
        Key CreateKey(nostd::string_view key_name){

          return Key(key_name); 
        }

        /* Contructor, creates a context object from a map
         * of keys and identifiers
         */
        Context(const T &attributes):  key_vals(attributes) { 

          trace::KeyValueIterableView<T> iterable = trace::KeyValueIterableView<T>(attributes);
        }

        /* Contructor, creates a context object from a 
         * KeyValueIterableModfiable object.
         */
        Context() { 
        }

        /* Accepts a new iterable and then returns a new
         * context that contains both the original pairs 
         * and the new pair.
         */
        Context WriteValues(T &attributes) noexcept
        {

          trace::KeyValueIterableView<T> iterable = trace::KeyValueIterableView<T>(attributes);

          std::insert_iterator<T> back (attributes, std::begin(attributes));
          
          
          
          auto iter = std::begin(key_vals);
          auto last = std::end(key_vals);
          for (; iter != last; ++iter)
          {
            back = *iter;
          }
          return Context(attributes);
        }

        /* Returns the value associated with the passed in key */
        common::AttributeValue GetValue(Key key) { 
          auto iter = std::begin(key_vals);
          auto last = std::end(key_vals);
          int count = 0;
          for (; iter != last; ++iter)
          {
            if(key.GetIdentifier() == iter->first){
              return iter->second;
            }
            count++;
          }

          return "";
        }

      private:
        T key_vals;

    };
    
}  // namespace context
OPENTELEMETRY_END_NAMESPACE
