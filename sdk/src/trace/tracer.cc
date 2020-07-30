#include "opentelemetry/sdk/trace/tracer.h"

#include "opentelemetry/context/runtime_context.h"
#include "opentelemetry/sdk/common/atomic_shared_ptr.h"
#include "opentelemetry/version.h"
#include "src/trace/span.h"
#include "opentelemetry/trace/span.h"

#include <iostream>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace trace
{
Tracer::Tracer(std::shared_ptr<SpanProcessor> processor, std::shared_ptr<Sampler> sampler) noexcept
    : processor_{processor}, sampler_{sampler}
{}

void Tracer::SetProcessor(std::shared_ptr<SpanProcessor> processor) noexcept
{
  processor_.store(processor);
}

std::shared_ptr<SpanProcessor> Tracer::GetProcessor() const noexcept
{
  return processor_.load();
}

std::shared_ptr<Sampler> Tracer::GetSampler() const noexcept
{
  return sampler_;
}

nostd::shared_ptr<trace_api::Span> Tracer::StartSpan(
    nostd::string_view name,
    const trace_api::KeyValueIterable &attributes,
    const trace_api::StartSpanOptions &options) noexcept
{
  // TODO: replace nullptr with parent context in span context
  auto sampling_result =
      sampler_->ShouldSample(nullptr, trace_api::TraceId(), name, options.kind, attributes);
  if (sampling_result.decision == Decision::NOT_RECORD)
  {
    return nostd::shared_ptr<trace_api::Span>{new (std::nothrow)
                                                  trace_api::NoopSpan{this->shared_from_this()}};
  }
  else
  {
    auto span = nostd::shared_ptr<trace_api::Span>{new (std::nothrow) Span{
        this->shared_from_this(), processor_.load(), name, attributes, options}};
    
     //key_ = "span_key";
        
//    nostd::shared_ptr<trace_api::Span> shared_span(span.release());
     //token_ = context::RuntimeContext::Attach(context::RuntimeContext::GetCurrent().SetValue(key_, span));
     //std::cout << "span " << typeid(span).name() << std::endl;
     //std::cout << "span " << typeid(nostd::shared_ptr<opentelemetry::trace::Span>).name() << std::endl;
    
     context::RuntimeContext::Token token = context::RuntimeContext::Attach(context::RuntimeContext::GetCurrent().SetValue("span_key",(nostd::shared_ptr<opentelemetry::trace::Span>)span));
     
  nostd::get<nostd::shared_ptr<opentelemetry::trace::Span>>(context::RuntimeContext::GetCurrent().GetValue("span_key"))->token_ = token;

     std::cout << "attached" << std::endl;
     
     //context::Context temp_context = context::RuntimeContext::GetCurrent(); 
     //
     //context::RuntimeContext::Detach(token); 
     /*
     token_ = context::RuntimeContext::Attach(context::RuntimeContext::GetCurrent().SetValue("span_key",(int64_t)5));
     context::Context temp_context = context::RuntimeContext::GetCurrent(); 
     temp_context.GetValue("span_key");

     context::Context trial_context("test", 123);
     */
     //nostd::get<nostd::shared_ptr<opentelemetry::trace::Span>>(context::RuntimeContext::GetCurrent().GetValue("span_key"));
      
    // if the attributes is not nullptr, add attributes to the span.
    if (sampling_result.attributes)
    {
      for (auto &kv : *sampling_result.attributes)
      {
        span->SetAttribute(kv.first, kv.second);
      }
    }

    return span;
  }
}

void Tracer::ForceFlushWithMicroseconds(uint64_t timeout) noexcept
{
  (void)timeout;
}

void Tracer::CloseWithMicroseconds(uint64_t timeout) noexcept
{
  (void)timeout;
}
}  // namespace trace
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
