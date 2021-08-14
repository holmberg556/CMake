/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#ifndef cmIncludeTree_h
#define cmIncludeTree_h

#include "cmConfigure.h" // IWYU pragma: keep

#include <string>

class cmIncludeTreeLevel
{
public:
  cmIncludeTreeLevel(std::string const& path);
  ~cmIncludeTreeLevel();

private:
  std::string Path;
};

#endif
