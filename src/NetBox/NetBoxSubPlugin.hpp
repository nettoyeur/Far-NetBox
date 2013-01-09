#pragma once

#include <FarPlugin.h>
#include <subplugin.hpp>

//------------------------------------------------------------------------------

class TBaseSubplugin : public TObject
{
  friend TWinSCPFileSystem;
public:
  explicit TBaseSubplugin();
  virtual ~TBaseSubplugin();

private:
};

//------------------------------------------------------------------------------
// Utility functions

// wchar_t * GuidToStr(const GUID & Guid, wchar_t * Buffer, size_t sz);
