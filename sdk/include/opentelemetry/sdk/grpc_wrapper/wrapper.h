#pragma once

#include "opentelemetry/version.h"
#include "opentelemetry/trace/span_context.h"
#include "opentelemetry/nostd/span.h"

#include <string>
#include <grpcpp/grpcpp.h>


OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
  namespace grpc
  {
    
    constexpr int TraceIdIdx = 0;
    constexpr int TraceFlagsIdx = 16;
    constexpr int HasRemoteIdx = 17;

   /* 
    std::string ToGrpcHeader(const opentelemetry::trace::SpanContext context){

      return ""; 
    }*/

    std::string ToGrpcHeader(const opentelemetry::trace::SpanContext context){
      char str[2];
      
      str[0] = context.trace_flags().flags();  
      str[1] = context.HasRemoteParent();     
      
      return std::string(str);
    }

    opentelemetry::trace::SpanContext FromGrpcHeader(std::string header){
       return opentelemetry::trace::SpanContext(header[0], header[1]);
    }
      
  }
}
OPENTELEMETRY_END_NAMESPACE
