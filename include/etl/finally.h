///\file


#ifndef ETL_FINALLY_INCLUDED
#define ETL_FINALLY_INCLUDED

#include "platform.h"
#include "type_traits.h"
#include "utility.h"

namespace etl
{
#if ETL_USING_CPP11
  //*****************************************************************************
  // final_action allows you to ensure something gets run at the end of a scope
  //*****************************************************************************
  template <class F>
  class final_action
  {
  public:
    explicit final_action(const F& ff) ETL_NOEXCEPT
      : f{ff}
    {
    }
    explicit final_action(F&& ff) ETL_NOEXCEPT
      : f{etl::move(ff)}
    {
    }

    ~final_action() ETL_NOEXCEPT
    {
      if (invoke)
        f();
    }

    final_action(final_action&& other) ETL_NOEXCEPT
      : f(etl::move(other.f)), invoke(etl::exchange(other.invoke, false))
    {
    }

    final_action(const final_action&) = delete;
    void operator=(const final_action&) = delete;
    void operator=(final_action&&) = delete;

  private:
    F    f;
    bool invoke = true;
  };

  //*****************************************************************************
  // finally() - convenience function to generate a final_action
  //*****************************************************************************
  template <class F>
  ETL_NODISCARD auto finally(F&& f) ETL_NOEXCEPT
  {
    return final_action<etl::decay_t<F>>{etl::forward<F>(f)};
  }
#endif
}
#endif
