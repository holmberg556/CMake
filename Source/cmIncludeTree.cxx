/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#include "cmIncludeTree.h"

#include <string>
#include <stdio.h>

class cmIncludeTree
{
  void _delayedInitialize()
  {
    if (File == nullptr)
    {
      Level = 0;
      File = fopen(Filename.c_str(), "w");
      if (File == NULL)
      {
        perror("fopen");
        exit(1);
      }
    }
  }

public:
  cmIncludeTree(std::string filename)
  {
    Filename = filename;
  }

  ~cmIncludeTree()
  {
    if (File != nullptr)
    {
      fclose(File);
    }
  }

  void WriteLine(std::string const& line)
  {
    _delayedInitialize();
    fprintf(File, "%s\n", line.c_str());
  }

  void Write(std::string const& line)
  {
    _delayedInitialize();
    fprintf(File, "%s", line.c_str());
  }

  void Enter(std::string const& path, cmIncludeTreeLevel::Type type)
  {
    std::string line;
    for (int i=0; i<Level; i++)
    {
      line += ".   ";
    }
    line += path;

    WriteLine(line);
    Level++;
  }

  void Leave(std::string const& path, cmIncludeTreeLevel::Type type)
  {
    Level--;
  }

protected:
  std::string Filename;
  int Level;
  FILE * File;
};

class cmTraceTree : public cmIncludeTree
{
  bool StartWritten;

public:
  cmTraceTree(std::string filename)
    : cmIncludeTree(filename), StartWritten(false)
  {
  }

  std::string GetJsonName(std::string const& what, std::string const& path, cmIncludeTreeLevel::Type type)
  {
    std::string typeStr;
    switch (type)
    {
    case cmIncludeTreeLevel::AddSubdirectoryType: typeStr = "add_subdirectory"; break;
    case cmIncludeTreeLevel::IncludeType:         typeStr = "include";          break;
    case cmIncludeTreeLevel::FunctionType:        typeStr = "function";         break;
    case cmIncludeTreeLevel::MacroType:           typeStr = "macro";            break;
    }
    std::string name = "[ \"" + what + "\", \"" + typeStr + "\", \"" + path + "\" ]";
    return name;
  }

  void Enter(std::string const& path, cmIncludeTreeLevel::Type type)
  {
    std::string str = GetJsonName("enter", path, type);
    LazyJson();
    Write(str);
    Level++;
  }

  void Leave(std::string const& path, cmIncludeTreeLevel::Type type)
  {
    std::string str = GetJsonName("leave", path, type);
    LazyJson();
    Write(str);
    Level--;
  }

  void LazyJson()
  {
    if (StartWritten)
    {
      Write(",\n  ");
    }
    else
    {
      Write("[\n  ");
      StartWritten = true;
    }
  }

  ~cmTraceTree()
  {
    if (StartWritten)
    {
      Write("\n]\n");
    }
  }
};

static cmTraceTree JsonTree("cmake.trace.json");
static cmIncludeTree Tree("cmake.tree");
static cmIncludeTree FilesTree("cmake.files.tree");
static cmIncludeTree FunctionsTree("cmake.functions.tree");

cmIncludeTreeLevel::cmIncludeTreeLevel(std::string const& path, Type type)
{
  Path = path;
  LevelType = type;

  JsonTree.Enter(Path, LevelType);

  if (LevelType == AddSubdirectoryType || LevelType == IncludeType)
  {
    Tree.Enter(Path, LevelType);
    FilesTree.Enter(Path, LevelType);
  }
  else if (LevelType == FunctionType)
  {
    Tree.Enter(Path, LevelType);
    FunctionsTree.Enter(Path, LevelType);
  }
  else if (LevelType == MacroType)
  {
    Tree.Enter(Path, LevelType);
    FunctionsTree.Enter(Path, LevelType);
  }
}

cmIncludeTreeLevel::~cmIncludeTreeLevel()
{
  JsonTree.Leave(Path, LevelType);

  if (LevelType == AddSubdirectoryType || LevelType == IncludeType)
  {
    Tree.Leave(Path, LevelType);
    FilesTree.Leave(Path, LevelType);
  }
  else if (LevelType == FunctionType)
  {
    Tree.Leave(Path, LevelType);
    FunctionsTree.Leave(Path, LevelType);
  }
  else if (LevelType == MacroType)
  {
    Tree.Leave(Path, LevelType);
    FunctionsTree.Leave(Path, LevelType);
  }
}
