
#include <iostream>

#include "Profiler.h"

void function1()
{
  PROFILE_FUNCTION();

  for (int i = 0; i < 10000; i++)
  {
    std::cout << sqrt(i) << std::endl;
  }
}

void function2()
{
  PROFILE_FUNCTION();

  for (int i = 0; i < 10000; i++)
  {
    std::cout << sqrt(i) << "\n";
  }
}

int main()
{
  Profiler::Get().Start("Profile Session 1", "profile_result2.json");
  function1();
  function2();
  Profiler::Get().End();
  return 0;
}
