#include "Profiler.h"

#include <algorithm>
#include <thread>

void Profiler::Start(const std::string& name, const std::string& outputPath)
{
  mOutputStream.open(outputPath);
  WriteHeader();
  mCurrentProfile = new Profile{ name };
}

void Profiler::End()
{
  WriteFooter();
  mOutputStream.close();
  delete mCurrentProfile;
  mCurrentProfile = nullptr;
  mNumOfProfiles = 0;
}

void Profiler::WriteHeader()
{
  mOutputStream << "{\"otherData\": {},\"traceEvents\":[";
  mOutputStream.flush();
}

void Profiler::WriteFooter()
{
  mOutputStream << "]}";
}

void Profiler::WriteProfile(const ProfileResult& result)
{
  if (mNumOfProfiles++ > 0)
  {
    mOutputStream << ",";
  }

  std::string name = result.name;
  std::replace(name.begin(), name.end(), '"', '\'');

  mOutputStream << "{";
  mOutputStream << "\"cat\":\"function\",";
  mOutputStream << "\"dur\":" << (result.end - result.start) << ',';
  mOutputStream << "\"name\":\"" << name << "\",";
  mOutputStream << "\"ph\":\"X\",";
  mOutputStream << "\"pid\":0,";
  mOutputStream << "\"tid\":" << result.threadId << ",";
  mOutputStream << "\"ts\":" << result.start;
  mOutputStream << "}";

  mOutputStream.flush();
}

ProfileTimer::ProfileTimer(const std::string& name) : mName(name), mStopped(false)
{
  mStartPoint = std::chrono::high_resolution_clock::now();
}

ProfileTimer::~ProfileTimer()
{
  if (!mStopped) {
    Stop();
  }
}

void ProfileTimer::Stop()
{
  const auto endPoint = std::chrono::high_resolution_clock::now();

  const long long start = std::chrono::time_point_cast<std::chrono::microseconds>(mStartPoint).time_since_epoch().count();
  const long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count();

  const unsigned int threadID = static_cast<unsigned int>(std::hash<std::thread::id>{}(std::this_thread::get_id()));

  Profiler::Get().WriteProfile({ mName, start, end, threadID });

  mStopped = true;
}
