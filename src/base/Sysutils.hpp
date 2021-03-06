//---------------------------------------------------------------------------
#ifndef SysutilsH
#define SysutilsH

#include <WinBase.h>

#include <Classes.hpp>
#include <headers.hpp>

namespace Sysutils {

//---------------------------------------------------------------------------
typedef int TDayTable[12];
extern const TDayTable MonthDays[];

//---------------------------------------------------------------------------
class Exception : public std::runtime_error, public TObject
{
public:
  explicit Exception(const wchar_t * Msg);
  explicit Exception(const UnicodeString & Msg);
  explicit Exception(Exception * E);
  explicit Exception(std::exception * E);
  explicit Exception(const UnicodeString & Msg, int AHelpContext);
  explicit Exception(Exception * E, int Ident);
  explicit Exception(int Ident);
  ~Exception() throw() {}

  template<typename T>
  bool InheritsFrom() const { return dynamic_cast<const T *>(this) != nullptr; }

public:
  UnicodeString Message;

protected:
  // UnicodeString FHelpKeyword;
};

//---------------------------------------------------------------------------
class EAbort : public Exception
{
public:
  EAbort(const std::wstring & what) : Exception(what)
  {}
};

class EAccessViolation : public Exception
{
public:
  EAccessViolation(const std::wstring & what) : Exception(what)
  {}
};

class EFileNotFoundError : public Exception
{
public:
  EFileNotFoundError() : Exception("")
  {}
};

//---------------------------------------------------------------------------

class EOSError : public Exception
{
public:
  EOSError(const UnicodeString & Msg, DWORD code) : Exception(Msg),
    ErrorCode(code)
  {
  }
  DWORD ErrorCode;
};

void RaiseLastOSError(DWORD Result = 0);
//---------------------------------------------------------------------------

struct TFormatSettings : public TObject
{
public:
  TFormatSettings(int /* LCID */) {}
  static TFormatSettings Create(int LCID ) { return TFormatSettings(LCID); }
  unsigned char CurrencyFormat;
  unsigned char NegCurrFormat;
  wchar_t ThousandSeparator;
  wchar_t DecimalSeparator;
  unsigned char CurrencyDecimals;
  wchar_t DateSeparator;
  wchar_t TimeSeparator;
  wchar_t ListSeparator;
  UnicodeString CurrencyString;
  UnicodeString ShortDateFormat;
  UnicodeString LongDateFormat;
  UnicodeString TimeAMString;
  UnicodeString TimePMString;
  UnicodeString ShortTimeFormat;
  UnicodeString LongTimeFormat;
  unsigned short TwoDigitYearCenturyWindow;
};

void GetLocaleFormatSettings(int LCID, TFormatSettings & FormatSettings);
// int GetDefaultLCID();

//---------------------------------------------------------------------------

UnicodeString ExtractShortPathName(const UnicodeString & Path1);
UnicodeString ExtractDirectory(const UnicodeString & Path, wchar_t Delimiter = '/');
UnicodeString ExtractFilename(const UnicodeString & Path, wchar_t Delimiter = '/');
UnicodeString ExtractFileExtension(const UnicodeString & Path, wchar_t Delimiter = '/');
UnicodeString ChangeFileExtension(const UnicodeString & Path, const UnicodeString & Ext, wchar_t Delimiter = '/');

UnicodeString IncludeTrailingBackslash(const UnicodeString & Str);
UnicodeString ExcludeTrailingBackslash(const UnicodeString & Str);
UnicodeString ExtractFileDir(const UnicodeString & Str);
UnicodeString ExtractFilePath(const UnicodeString & Str);
UnicodeString GetCurrentDir();

UnicodeString IncludeTrailingPathDelimiter(const UnicodeString & Str);

UnicodeString StrToHex(const UnicodeString & Str, bool UpperCase = true, char Separator = '\0');
UnicodeString HexToStr(const UnicodeString & Hex);
uintptr_t HexToInt(const UnicodeString & Hex, uintptr_t MinChars = 0);
UnicodeString IntToHex(uintptr_t Int, uintptr_t MinChars = 0);
char HexToChar(const UnicodeString & Hex, uintptr_t MinChars = 0);

UnicodeString ReplaceStrAll(const UnicodeString & Str, const UnicodeString & What, const UnicodeString & ByWhat);
UnicodeString SysErrorMessage(int Code);

bool TryStrToDateTime(const UnicodeString & StrValue, TDateTime & Value, TFormatSettings & FormatSettings);
UnicodeString DateTimeToStr(UnicodeString & Result, const UnicodeString & Format,
                            TDateTime DateTime);
UnicodeString DateTimeToString(TDateTime DateTime);
unsigned int DayOfWeek(const TDateTime & DateTime);

TDateTime Date();
void DecodeDate(const TDateTime & DateTime, unsigned short & Y,
  unsigned short & M, unsigned short & D);
void DecodeTime(const TDateTime & DateTime, unsigned short & H,
  unsigned short & N, unsigned short & S, unsigned short & MS);

UnicodeString FormatDateTime(const UnicodeString & Fmt, TDateTime DateTime);
TDateTime SystemTimeToDateTime(const SYSTEMTIME & SystemTime);

TDateTime EncodeDate(int Year, int Month, int Day);
TDateTime EncodeTime(unsigned int Hour, unsigned int Min, unsigned int Sec, unsigned int MSec);

UnicodeString Trim(const UnicodeString & Str);
UnicodeString TrimLeft(const UnicodeString & Str);
UnicodeString TrimRight(const UnicodeString & Str);
UnicodeString UpperCase(const UnicodeString & Str);
UnicodeString LowerCase(const UnicodeString & Str);
inline wchar_t UpCase(const wchar_t Ch);
inline wchar_t LowCase(const wchar_t Ch);
UnicodeString AnsiReplaceStr(const UnicodeString & Str, const UnicodeString & From, const UnicodeString & To);
intptr_t AnsiPos(const UnicodeString & Str2, wchar_t Ch);
intptr_t Pos(const UnicodeString & Str2, const UnicodeString & Substr);
UnicodeString StringReplace(const UnicodeString & Str, const UnicodeString & From, const UnicodeString & To, const TReplaceFlags & Flags);
bool IsDelimiter(const UnicodeString & Delimiters, const UnicodeString & Str, intptr_t Index);
intptr_t FirstDelimiter(const UnicodeString & Delimiters, const UnicodeString & Str);
intptr_t LastDelimiter(const UnicodeString & Delimiters, const UnicodeString & Str);
//---------------------------------------------------------------------------

intptr_t CompareText(const UnicodeString & Str1, const UnicodeString & Str2);
intptr_t AnsiCompare(const UnicodeString & Str1, const UnicodeString & Str2);
intptr_t AnsiCompareStr(const UnicodeString & Str1, const UnicodeString & Str2);
bool AnsiSameText(const UnicodeString & Str1, const UnicodeString & Str2);
bool SameText(const UnicodeString & Str1, const UnicodeString & Str2);
intptr_t AnsiCompareText(const UnicodeString & Str1, const UnicodeString & Str2);
intptr_t AnsiCompareIC(const UnicodeString & Str1, const UnicodeString & Str2);
bool AnsiContainsText(const UnicodeString & Str1, const UnicodeString & Str2);

int StringCmp(const wchar_t * S1, const wchar_t * S2);
int StringCmpI(const wchar_t * S1, const wchar_t * S2);

//---------------------------------------------------------------------------
UnicodeString IntToStr(intptr_t Value);
UnicodeString Int64ToStr(__int64 Value);
intptr_t StrToInt(const UnicodeString & Value);
__int64 ToInt(const UnicodeString & Value);
intptr_t StrToIntDef(const UnicodeString & Value, intptr_t DefVal);
__int64 StrToInt64(const UnicodeString & Value);
__int64 StrToInt64Def(const UnicodeString & Value, __int64 DefVal);
bool TryStrToInt(const std::wstring & StrValue, intptr_t & Value);
bool TryStrToInt(const std::wstring & StrValue, __int64 & Value);

//---------------------------------------------------------------------------
double StrToFloat(const UnicodeString & Value);
double StrToFloatDef(const UnicodeString & Value, double DefVal);
UnicodeString FormatFloat(const UnicodeString & Format, double Value);
//---------------------------------------------------------------------------
TTimeStamp DateTimeToTimeStamp(TDateTime DateTime);
//---------------------------------------------------------------------------

__int64 FileRead(HANDLE Handle, void * Buffer, __int64 Count);
__int64 FileWrite(HANDLE Handle, const void * Buffer, __int64 Count);
__int64 FileSeek(HANDLE Handle, __int64 Offset, DWORD Origin);

//---------------------------------------------------------------------------

enum FileAttributesEnum
{
  faReadOnly = 0x00000001,
  faHidden = 0x00000002,
  faSysFile = 0x00000004,
  faVolumeId = 0x00000008,
  faDirectory = 0x00000010,
  faArchive = 0x00000020,
  faSymLink = 0x00000040,
  faAnyFile = 0x0000003f,
};

bool FileExists(const UnicodeString & FileName);
bool RenameFile(const UnicodeString & From, const UnicodeString & To);
bool DirectoryExists(const UnicodeString & Dir);
UnicodeString FileSearch(const UnicodeString & FileName, const UnicodeString & DirectoryList);

inline DWORD FileGetAttr(const UnicodeString & FileName);
inline DWORD FileSetAttr(const UnicodeString & FileName, DWORD LocalFileAttrs);

bool ForceDirectories(const UnicodeString & Dir);
bool DeleteFile(const UnicodeString & File);
bool CreateDir(const UnicodeString & Dir);
bool RemoveDir(const UnicodeString & Dir);

//---------------------------------------------------------------------------
template <class Base, class Derived>
bool InheritsFrom(const Base * t)
{
  return dynamic_cast<const Derived *>(t) != nullptr;
}

//---------------------------------------------------------------------------
UnicodeString Format(const wchar_t * Format, ...);
UnicodeString Format(const wchar_t * Format, va_list Args);
AnsiString Format(const char * Format, ...);
AnsiString Format(const char * Format, va_list Args);
UnicodeString FmtLoadStr(intptr_t Id, ...);
//---------------------------------------------------------------------------
UnicodeString WrapText(const UnicodeString & Line, intptr_t MaxWidth = 40);
//---------------------------------------------------------------------------
UnicodeString TranslateExceptionMessage(std::exception * E);
//---------------------------------------------------------------------------

void AppendWChar(UnicodeString & Str2, const wchar_t Ch);
void AppendChar(std::string & Str2, const char Ch);

void AppendPathDelimiterW(UnicodeString & Str2);
void AppendPathDelimiterA(std::string & Str2);

UnicodeString ExpandEnvVars(const UnicodeString & Str2);

//---------------------------------------------------------------------------

UnicodeString StringOfChar(const wchar_t Ch, intptr_t Len);

char * StrNew(const char * Str2);

UnicodeString ChangeFileExt(const UnicodeString & FileName, const UnicodeString & Ext);
UnicodeString ExtractFileExt(const UnicodeString & FileName);
UnicodeString ExpandUNCFileName(const UnicodeString & FileName);

//---------------------------------------------------------------------------
typedef WIN32_FIND_DATA TWin32FindData;
typedef UnicodeString TFileName;

struct TSystemTime
{
  Word wYear;
  Word wMonth;
  Word wDayOfWeek;
  Word wDay;
  Word wHour;
  Word wMinute;
  Word wSecond;
  Word wMilliseconds;
};

struct TFileTime
{
  Integer LowTime;
  Integer HighTime;
};

struct TSearchRec
{
  Integer Time;
  Int64 Size;
  Integer Attr;
  TFileName Name;
  Integer ExcludeAttr;
  THandle FindHandle;
  TWin32FindData FindData;
};

//---------------------------------------------------------------------------

DWORD FindFirst(const UnicodeString & FileName, DWORD LocalFileAttrs, TSearchRec & Rec);
DWORD FindNext(TSearchRec & Rec);
DWORD FindClose(TSearchRec & Rec);

//---------------------------------------------------------------------------
void InitPlatformId();
bool Win32Check(bool RetVal);
//---------------------------------------------------------------------------
class EConvertError : public Exception
{
public:
  EConvertError(const UnicodeString & Msg) :
    Exception(Msg)
  {}
};

//---------------------------------------------------------------------------
UnicodeString UnixExcludeLeadingBackslash(const UnicodeString & Path);
//---------------------------------------------------------------------------
extern int RandSeed;
extern void Randomize();
//---------------------------------------------------------------------------
TDateTime IncYear(const TDateTime & AValue, const Int64 ANumberOfYears = 1);
TDateTime IncMonth(const TDateTime & AValue, const Int64 NumberOfMonths = 1);
TDateTime IncWeek(const TDateTime & AValue, const Int64 ANumberOfWeeks = 1);
TDateTime IncDay(const TDateTime & AValue, const Int64 ANumberOfDays = 1);
TDateTime IncHour(const TDateTime & AValue, const Int64 ANumberOfHours = 1);
TDateTime IncMinute(const TDateTime & AValue, const Int64 ANumberOfMinutes = 1);
TDateTime IncSecond(const TDateTime & AValue, const Int64 ANumberOfSeconds = 1);
TDateTime IncMilliSecond(const TDateTime & AValue, const Int64 ANumberOfMilliSeconds = 1);

Boolean IsLeapYear(Word Year);

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

class TCriticalSection : public TObject
{
public:
  TCriticalSection();
  ~TCriticalSection();

  void Enter();
  void Leave();

  int GetAcquired() const { return FAcquired; }

private:
  CRITICAL_SECTION FSection;
  int FAcquired;
};
//---------------------------------------------------------------------------
UnicodeString StripHotkey(const UnicodeString & AText);
bool StartsText(const UnicodeString & ASubText, const UnicodeString & AText);
//---------------------------------------------------------------------------
struct TVersionInfo
{
  DWORD Major;
  DWORD Minor;
  DWORD Revision;
  DWORD Build;
};
#define MAKEVERSIONNUMBER(major,minor,revision) ( ((major)<<16) | ((minor)<<8) | (revision))
uintptr_t StrToVersionNumber(const UnicodeString & VersionMumberStr);
UnicodeString VersionNumberToStr(uintptr_t VersionNumber);
uintptr_t inline GetVersionNumber219() { return MAKEVERSIONNUMBER(2,1,9); }
uintptr_t inline GetVersionNumber2110() { return MAKEVERSIONNUMBER(2,1,10); }
uintptr_t inline GetVersionNumber2121() { return MAKEVERSIONNUMBER(2,1,21); }
uintptr_t inline GetCurrentVersionNumber() { return StrToVersionNumber(GetGlobalFunctions()->GetStrVersionNumber()); }
//---------------------------------------------------------------------------
template<class F>
class finally_type
{
public:
    explicit finally_type(F f) : function(f) {}
    ~finally_type() { try { function(); } catch (...) {} }
private:
    F function;
};

template<class F>
finally_type<F> finally(F f) { return finally_type<F>(f); }
//---------------------------------------------------------------------------
} // namespace Sysutils

using namespace Sysutils;

#endif
