#pragma once

#include <bits/stdc++.h>
#include <atomic>
#include <map>
#include <vector>

#include "opentelemetry/nostd/string_view.h"
#include "opentelemetry/nostd/variant.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace context
{

  /*The context class provides a context identifier */
  class Context
  {

    public:
      /*The Key class is used to obscure access from the
       * user to the context map. The identifier is used as a key
       * to the context map.
       */
      class Key
      {
        private:
          friend class Context;

          /* GetIdentifier: returns the identifier */
          Key* GetIdentifier();

          /* Constructs a new Key with the passed in name and
           * identifier.
           */
          Key(nostd::string_view key_name, int identifier);

        public:
          /* Consructs a new Key with the passed in name and increments
           * the identifier then assigns it to be the key's identifier.
           */
          Key(nostd::string_view key_name);
         
         /* Key Destructor */ 
          ~Key();
      };

      /* Creates a context object with no key/value pairs */
      Context();

      /* Contructor, creates a context object from a map
       * of keys and identifiers
       */
      Context(Key key, int value);

      /* Accepts a new key/value pair and then returns a new
       * context that contains both the original pairs and the new pair.
       */
      Context WriteValue(Key key, int value);

      /* Accepts a vector of key value pairs and combines them with the
       * existing context map then returns a new context with the new
       * combined map */
      Context WriteValues(std::vector<std::pair<Key, int>> ctx_list);

      /* Class comparator to see if the context maps are the same. */
      bool operator==(const Context &context);

      /* Returns the value associated with the passed in key */
      int GetValue(Key key);

      /* Returns a Key that has the passed in name and the
       * next available identifier.*/
      Key CreateKey(nostd::string_view key_name);

      /* Copy constructors */
      Context(const Context &other);
      Context &operator=(const Context &other);

      /* Key Destructor */
      ~Context();

    private:

      /* A constructor that accepts a key/value map */
      Context(std::map<Key*, int>);
  };

  /* The RuntimeContext class provides a wrapper for
   * propogating context through cpp. */
  class RuntimeContext
  {

    public:
      /* The token class provides an identifier that is used by
       * the attach and detach methods to keep track of context
       * objects.
       */

      class Token
      {

        private:
          friend class RuntimeContext;

          /* A constructor that sets the token's Context object to the
           * one that was passed in.
           */
          Token(Context &ctx);

          /* Returns the stored context object */
          Context GetContext();
      };

      /* A default constructor that will set the context to
       * an empty context object.
       */
      RuntimeContext();

      /* A constructor that will set the context as the passed in context. */
      RuntimeContext(Context &context);

      /* Sets the current 'Context' object. Returns a token
       * that can be used to reset to the previous Context.
       */
      static Token Attach(Context &context);

      /* Return the current context. */
      static Context GetCurrent();

      /* Resets the context to a previous value stored in the
       * passed in token. Returns zero if successful, -1 otherwise
       */
      static int Detach(Token &token);

      /* RuntimeContext Destructor */
      ~RuntimeContext();
    private:
      static thread_local Context context_;
  };

  //thread_local Context RuntimeContext::context_ = Context();
}  // namespace context
OPENTELEMETRY_END_NAMESPACE
