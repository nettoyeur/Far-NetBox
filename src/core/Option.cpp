//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <Common.h>
#include "Option.h"
#include "TextsCore.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TOptions::TOptions() :
  FSwitchMarks(L"-/"),
  FSwitchValueDelimiters(L":="),
  FNoMoreSwitches(false),
  FParamCount(0)
{
}
//---------------------------------------------------------------------------
void TOptions::ParseParams(const UnicodeString & Params)
{
  UnicodeString Param;
  UnicodeString ParamsLocal = Params;
  // CutToken(Params, Param); // To remove program name
  while (CutToken(ParamsLocal, Param))
  {
    Add(Param);
  }
}
//---------------------------------------------------------------------------
void TOptions::Add(const UnicodeString & Value)
{
  if (!FNoMoreSwitches &&
      (Value.Length() == 2) &&
      (Value[1] == Value[2]) &&
      (FSwitchMarks.Pos(Value[1]) > 0))
  {
    FNoMoreSwitches = true;
  }
  else
  {
    bool Switch = false;
    intptr_t Index = 0; // shut up
    if (!FNoMoreSwitches &&
        (Value.Length() >= 2) &&
        (FSwitchMarks.Pos(Value[1]) > 0))
    {
      Index = 2;
      Switch = true;
      while (Switch && (Index <= Value.Length()))
      {
        if (Value.IsDelimiter(FSwitchValueDelimiters, Index))
        {
          break;
        }
        // this is to treat /home/martin as parameter, not as switch
        else if ((Value[Index] != L'?') && ((UpCase(Value[Index]) < L'A') || (UpCase(Value[Index]) > L'Z')))
        {
          Switch = false;
          break;
        }
        ++Index;
      }
    }

    if (Switch)
    {
      TOption Option;
      Option.Type = otSwitch;
      Option.Name = Value.SubString(2, Index - 2);
      Option.Value = Value.SubString(Index + 1, Value.Length());
      Option.Used = false;
      FOptions.push_back(Option);
    }
    else
    {
      TOption Option;
      Option.Type = otParam;
      Option.Value = Value;
      Option.Used = false;
      FOptions.push_back(Option);
      ++FParamCount;
    }
  }
}
//---------------------------------------------------------------------------
UnicodeString TOptions::GetParam(intptr_t Index)
{
  assert((Index >= 1) && (Index <= FParamCount));

  UnicodeString Result;
  size_t I = 0;
  while ((I < FOptions.size()) && (Index > 0))
  {
    if (FOptions[I].Type == otParam)
    {
      --Index;
      if (Index == 0)
      {
        Result = FOptions[I].Value;
        FOptions[I].Used = true;
      }
    }
    ++I;
  }

  return Result;
}
//---------------------------------------------------------------------------
bool TOptions::GetEmpty() const
{
  return FOptions.empty();
}
//---------------------------------------------------------------------------
bool TOptions::FindSwitch(const UnicodeString & Switch,
  UnicodeString & Value, intptr_t & ParamsStart, intptr_t & ParamsCount)
{
  ParamsStart = 0;
  intptr_t Index = 0;
  bool Found = false;
  while ((Index < static_cast<intptr_t>(FOptions.size())) && !Found)
  {
    if (FOptions[Index].Type == otParam)
    {
      ParamsStart++;
    }
    else if (FOptions[Index].Type == otSwitch)
    {
      if (AnsiSameText(FOptions[Index].Name, Switch))
      {
        Found = true;
        Value = FOptions[Index].Value;
        FOptions[Index].Used = true;
      }
    }
    ++Index;
  }

  ParamsCount = 0;
  if (Found)
  {
    ParamsStart++;
    while ((Index + ParamsCount < static_cast<intptr_t>(FOptions.size())) &&
           (FOptions[Index + ParamsCount].Type == otParam))
    {
      ParamsCount++;
    }
  }
  else
  {
    ParamsStart = 0;
  }

  return Found;
}
//---------------------------------------------------------------------------
bool TOptions::FindSwitch(const UnicodeString & Switch, UnicodeString & Value)
{
  intptr_t ParamsStart;
  intptr_t ParamsCount;
  return FindSwitch(Switch, Value, ParamsStart, ParamsCount);
}
//---------------------------------------------------------------------------
bool TOptions::FindSwitch(const UnicodeString & Switch)
{
  UnicodeString Value;
  intptr_t ParamsStart;
  intptr_t ParamsCount;
  return FindSwitch(Switch, Value, ParamsStart, ParamsCount);
}
//---------------------------------------------------------------------------
bool TOptions::FindSwitch(const UnicodeString & Switch,
  TStrings * Params, intptr_t ParamsMax)
{
  UnicodeString Value;
  intptr_t ParamsStart;
  intptr_t ParamsCount;
  bool Result = FindSwitch(Switch, Value, ParamsStart, ParamsCount);
  if (Result)
  {
    if ((ParamsMax >= 0) && (ParamsCount > ParamsMax))
    {
      ParamsCount = ParamsMax;
    }

    intptr_t Index = 0;
    while (Index < ParamsCount)
    {
      Params->Add(GetParam(ParamsStart + Index));
      ++Index;
    }
    ParamsProcessed(ParamsStart, ParamsCount);
  }
  return Result;
}
//---------------------------------------------------------------------------
UnicodeString TOptions::SwitchValue(const UnicodeString & Switch,
  const UnicodeString & Default)
{
  UnicodeString Value;
  FindSwitch(Switch, Value);
  if (Value.IsEmpty())
  {
    Value = Default;
  }
  return Value;
}
//---------------------------------------------------------------------------
bool TOptions::SwitchValue(const UnicodeString & Switch, bool Default, bool DefaultOnNonExistence)
{
  bool Result = false;
  intptr_t IntValue = 0;
  UnicodeString Value;
  if (!FindSwitch(Switch, Value))
  {
    Result = DefaultOnNonExistence;
  }
  else if (Value.IsEmpty())
  {
    Result = Default;
  }
  else if (SameText(Value, "on"))
  {
    Result = true;
  }
  else if (SameText(Value, "off"))
  {
    Result = false;
  }
  else if (TryStrToInt(Value, IntValue))
  {
    Result = (IntValue != 0);
  }
  else
  {
    throw Exception(FMTLOAD(URL_OPTION_BOOL_VALUE_ERROR, Value.c_str()));
  }
  return Result;
}
//---------------------------------------------------------------------------
bool TOptions::SwitchValue(const UnicodeString & Switch, bool Default)
{
  return SwitchValue(Switch, Default, Default);
}
//---------------------------------------------------------------------------
bool TOptions::UnusedSwitch(UnicodeString & Switch) const
{
  bool Result = false;
  size_t Index = 0;
  while (!Result && (Index < FOptions.size()))
  {
    if ((FOptions[Index].Type == otSwitch) &&
        !FOptions[Index].Used)
    {
      Switch = FOptions[Index].Name;
      Result = true;
    }
    ++Index;
  }

  return Result;
}
//---------------------------------------------------------------------------
void TOptions::ParamsProcessed(intptr_t ParamsStart, intptr_t ParamsCount)
{
  if (ParamsCount > 0)
  {
    assert((ParamsStart >= 0) && ((ParamsStart - ParamsCount + 1) <= FParamCount));

    size_t Index = 0;
    while ((Index < FOptions.size()) && (ParamsStart > 0))
    {
      if (FOptions[Index].Type == otParam)
      {
        --ParamsStart;

        if (ParamsStart == 0)
        {
          while (ParamsCount > 0)
          {
            assert(Index < FOptions.size());
            assert(FOptions[Index].Type == otParam);
            FOptions.erase(FOptions.begin() + Index);
            --FParamCount;
            --ParamsCount;
          }
        }
      }
      ++Index;
    }
  }
}
