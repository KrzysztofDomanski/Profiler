#pragma once

#include <chrono>
#include <string>
#include <fstream>

#define PROFILE_SCOPE(name) ProfileTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)

struct ProfileResult
{
  std::string name;
  long long start;
  long long end;
  unsigned int threadId;
};

struct Profile
{
  std::string mName;
};

class Profiler
{
public:
  Profiler() : mCurrentProfile(nullptr), mNumOfProfiles(0) {}

  void Start(const std::string& name, const std::string& outputPath = "profile_result.json");

  void End();

  void WriteProfile(const ProfileResult& result);

  static Profiler& Get()
  {
    static Profiler instance;
    return instance;
  }
private:
  void WriteHeader();

  void WriteFooter();

  Profile* mCurrentProfile;
  std::ofstream mOutputStream;
  int mNumOfProfiles;
};


class ProfileTimer
{
public:
  ProfileTimer(const std::string& name);

  ~ProfileTimer();

  void Stop();
private:
  std::string mName;
  std::chrono::time_point<std::chrono::high_resolution_clock> mStartPoint;
  bool mStopped;
};
