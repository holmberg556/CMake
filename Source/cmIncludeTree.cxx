/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#include "cmIncludeTree.h"

#include <string>
#include <stdio.h>

class cmIncludeTree
{
public:
  cmIncludeTree()
  {
    Level = 0;
    File = fopen("cmake.tree", "w");
    if (File == NULL)
    {
      perror("fopen");
      exit(1);
    }
  }

  ~cmIncludeTree()
  {
    fclose(File);
  }

  void WriteEntry(std::string const& path)
  {
    for (int i=0; i<Level; i++)
    {
      fprintf(File, ".   ");
    }
    fprintf(File, "%s\n", path.c_str());
  }

  void Enter(std::string const& path)
  {
    WriteEntry(path);
    Level++;
  }

  void Leave(std::string const& path)
  {
    Level--;
  }

private:
  int Level;
  FILE * File;
};

static cmIncludeTree& theTree()
{
  static cmIncludeTree theTree_obj;
  return theTree_obj;
}

cmIncludeTreeLevel::cmIncludeTreeLevel(std::string const& path)
{
  Path = path;
  theTree().Enter(Path);
}

cmIncludeTreeLevel::~cmIncludeTreeLevel()
{
  theTree().Leave(Path);
}
