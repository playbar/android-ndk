// --~--~--~--~----~--~--~--~----~--~--~--~----~--~--~--~----~--~--~--~--
// Copyright 2013-2015 Qualcomm Technologies, Inc.
// All rights reserved.
// Confidential and Proprietary – Qualcomm Technologies, Inc.
// --~--~--~--~----~--~--~--~----~--~--~--~----~--~--~--~----~--~--~--~--
#pragma once

#include <symphony/taskfactory.hh>
#include <symphony/tuner.hh>

#include <symphony/internal/patterns/pfor_each.hh>
#include <symphony/internal/util/templatemagic.hh>

namespace symphony {

namespace internal{

namespace pointkernel{

template<typename RT, typename... Args>
class pointkernel;
};

};

namespace pattern {

template <typename T1, typename T2> class pfor;

template <typename UnaryFn>
pfor<UnaryFn, void> create_pfor_each(UnaryFn&& fn);

};

namespace beta {

namespace pattern {

template<typename KernelTuple, typename ArgTuple>
symphony::pattern::pfor<KernelTuple, ArgTuple>
create_pfor_each_helper(KernelTuple&& ktpl, ArgTuple&& atpl);

};

};

namespace pattern {

template <typename T1, typename T2>
class pfor {
public:

  template <size_t Dims>
  void run(const symphony::range<Dims>& r,
           const symphony::pattern::tuner& t = symphony::pattern::tuner().set_cpu_load(100)) {
    symphony::internal::pfor_each_run_helper(r, _ktpl, t,
      typename symphony::internal::integer_list<std::tuple_size<decltype(_atpl)>::value>::type(), _atpl);
  }

  template <size_t Dims>
  void operator()(const symphony::range<Dims>& r,
                  const symphony::pattern::tuner& t = symphony::pattern::tuner().set_cpu_load(100)) {
    run<Dims>(r, t);
  }

#ifndef _MSC_VER

private:
  template<typename KT, typename AT>
  friend pfor<KT, AT> symphony::beta::pattern::create_pfor_each_helper(KT&& ktpl, AT&& atpl);
#else
public:
#endif

  T1 _ktpl;
  T2 _atpl;

  pfor(T1&& ktpl, T2&& atpl) : _ktpl(ktpl), _atpl(atpl) {}
};

template <typename RT, typename... PKType, typename T2>
class pfor<symphony::internal::pointkernel::pointkernel<RT,PKType...>,T2> {

  using pointkernel_type = symphony::internal::pointkernel::pointkernel<RT,PKType...>;
public:

  template <size_t Dims>
  void run(const symphony::range<Dims>& r,
           const symphony::pattern::tuner& t = symphony::pattern::tuner().set_cpu_load(100)) {

    symphony::internal::pfor_each_run_helper(r, _pk, t,
      typename symphony::internal::integer_list<std::tuple_size<decltype(_atpl)>::value>::type(), _atpl);
  }

  template <size_t Dims>
  void operator()(const symphony::range<Dims>& r,
                  const symphony::pattern::tuner& t = symphony::pattern::tuner().set_cpu_load(100)) {
    run<Dims>(r, t);
  }

#ifndef _MSC_VER

private:

  template<typename RetType, typename... PKArgs, typename ArgTuple>
  friend pfor<symphony::internal::pointkernel::pointkernel<RetType,PKArgs...>, ArgTuple>
  symphony::beta::pattern::create_pfor_each_helper(symphony::internal::pointkernel::pointkernel<RetType,PKArgs...>& pk,
                                                   ArgTuple&& atpl);
#else
public:
#endif
  pointkernel_type& _pk;
  T2 _atpl;

  pfor(pointkernel_type& pk, T2&& atpl) : _pk(pk), _atpl(atpl) {}
};

template <typename T1>
class pfor<T1, void> {
public:
  template <typename InputIterator>
  void run(InputIterator first,
           InputIterator last,
           const symphony::pattern::tuner& t = symphony::pattern::tuner()) {
    symphony::internal::pfor_each_internal
      (nullptr, first, last, std::forward<T1>(_fn), 1, t);
  }

  template <typename InputIterator>
  void operator()(InputIterator first,
                  InputIterator last,
                  const symphony::pattern::tuner& t = symphony::pattern::tuner()) {
    run(first, last, t);
  }

  template <typename InputIterator>
  void run(InputIterator first,
           const size_t stride,
           InputIterator last,
           const symphony::pattern::tuner& t = symphony::pattern::tuner()) {
    symphony::internal::pfor_each_internal
      (nullptr, first, last, std::forward<T1>(_fn), stride, t);
  }

  template <typename InputIterator>
  void operator()(InputIterator first,
                  const size_t stride,
                  InputIterator last,
                  const symphony::pattern::tuner& t = symphony::pattern::tuner()) {
    run(first, stride, last, t);
  }

  template <size_t Dims>
  void run(const symphony::range<Dims>& r,
           const symphony::pattern::tuner& t = symphony::pattern::tuner()) {
    symphony::internal::pfor_each_internal
      (nullptr, r, std::forward<T1>(_fn), 1, t);
  }

  template <size_t Dims>
  void operator()(const symphony::range<Dims>& r,
                  const symphony::pattern::tuner& t = symphony::pattern::tuner()) {
    run<Dims>(r, t);
  }

#ifndef _MSC_VER

private:
  friend pfor create_pfor_each<T1>(T1&& fn);

  template<typename Fn, typename... Args>
  friend symphony::task_ptr<void()> symphony::create_task(const pfor<Fn, void>& pf, Args&&...args);

#else
public:
#endif
  T1 _fn;
  explicit pfor(T1&& fn) : _fn(fn) {}

};

template <typename UnaryFn>
pfor<UnaryFn, void>
create_pfor_each(UnaryFn&& fn) {

  return pfor<UnaryFn, void>(std::forward<UnaryFn>(fn));
}

};

namespace beta {

namespace pattern {

template<typename KernelTuple, typename ArgTuple>
symphony::pattern::pfor<KernelTuple, ArgTuple>
create_pfor_each_helper(KernelTuple&& ktpl, ArgTuple&& atpl)
{
  return symphony::pattern::pfor<KernelTuple, ArgTuple>
    (std::forward<KernelTuple>(ktpl), std::forward<ArgTuple>(atpl));
}

template<typename RT, typename... PKArgs, typename ArgTuple>
symphony::pattern::pfor<symphony::internal::pointkernel::pointkernel<RT,PKArgs...>, ArgTuple>
create_pfor_each_helper(symphony::internal::pointkernel::pointkernel<RT,PKArgs...>& pk, ArgTuple&& atpl)
{
  return symphony::pattern::pfor<symphony::internal::pointkernel::pointkernel<RT,PKArgs...>, ArgTuple>
    (pk, std::forward<ArgTuple>(atpl));
}

template<typename T, typename... Args>
auto create_pfor_each(T&& type, Args&&... args)
  ->decltype(create_pfor_each_helper(std::forward<T>(type),
                                     std::forward<decltype(std::forward_as_tuple(args...))>(std::forward_as_tuple(args...)))) {

  auto arg_tuple = std::forward_as_tuple(args...);

  return create_pfor_each_helper(std::forward<T>(type),
                                 std::forward<decltype(arg_tuple)>(arg_tuple));
}

};

};

template <class InputIterator, typename UnaryFn>
symphony::task_ptr<void()>
pfor_each_async(InputIterator first, InputIterator last,
                UnaryFn fn,
                const symphony::pattern::tuner& tuner = symphony::pattern::tuner())
{
  return symphony::internal::pfor_each_async(fn, first, last, 1, tuner);
}

template <class InputIterator, typename UnaryFn>
symphony::task_ptr<void()>
pfor_each_async(InputIterator first, const size_t stride,
                InputIterator last, UnaryFn fn,
                const symphony::pattern::tuner& tuner = symphony::pattern::tuner())
{
  return symphony::internal::pfor_each_async(fn, first, last, stride, tuner);
}

template <size_t Dims, typename UnaryFn>
symphony::task_ptr<void()>
pfor_each_async(const symphony::range<Dims>& r,
                UnaryFn fn,
                const symphony::pattern::tuner& tuner = symphony::pattern::tuner())
{
  return symphony::internal::pfor_each_async(fn, r, 1, tuner);
}

template <size_t Dims, typename UnaryFn>
symphony::task_ptr<void()>
pfor_each_async(const symphony::range<Dims>& r,
                const size_t stride, UnaryFn fn,
                const symphony::pattern::tuner& tuner = symphony::pattern::tuner())
{
  return symphony::internal::pfor_each_async(fn, r, stride, tuner);
}

template<typename Fn, typename... Args>
symphony::task_ptr<void()> create_task(const symphony::pattern::pfor<Fn, void>& pf, Args&&...args)
{
  return symphony::internal::pfor_each_async(pf._fn, args...);
}

template<typename Fn, typename... Args>
symphony::task_ptr<void()> launch(const symphony::pattern::pfor<Fn, void>& pf, Args&&...args)
{
  auto t = symphony::create_task(pf, args...);
  t->launch();
  return t;
}

template <class InputIterator, typename UnaryFn>
void pfor_each(InputIterator first, InputIterator last,
               UnaryFn&& fn,
               const symphony::pattern::tuner& t = symphony::pattern::tuner())
{
  internal::pfor_each
    (nullptr, first, last, std::forward<UnaryFn>(fn), 1, t);
}

template <class InputIterator, typename UnaryFn>
void pfor_each(InputIterator first, const size_t stride,
               InputIterator last, UnaryFn&& fn,
               const symphony::pattern::tuner& t = symphony::pattern::tuner())
{

  internal::pfor_each
    (nullptr, first, last, std::forward<UnaryFn>(fn), stride, t);
}

template <size_t Dims, typename UnaryFn>
void pfor_each(const symphony::range<Dims>& r, UnaryFn&& fn,
               const symphony::pattern::tuner& t = symphony::pattern::tuner())
{
  internal::pfor_each(nullptr, r, std::forward<UnaryFn>(fn), t);
}

};
