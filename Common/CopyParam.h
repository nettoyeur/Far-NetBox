//---------------------------------------------------------------------------
#ifndef CopyParamH
#define CopyParamH

#include "FileMasks.h"
#include "RemoteFiles.h"
//---------------------------------------------------------------------------
// When adding new options, mind TCopyParamType::GetLogStr()
enum TOperationSide { osLocal, osRemote, osCurrent };
enum TFileNameCase { ncNoChange, ncUpperCase, ncLowerCase, ncFirstUpperCase, ncLowerCaseShort };
// TScript::OptionProc depend on the order
enum TTransferMode { tmBinary, tmAscii, tmAutomatic };
enum TResumeSupport { rsOn, rsSmart, rsOff };
class THierarchicalStorage;
const int cpaExcludeMaskOnly = 0x01;
const int cpaNoTransferMode =  0x02;
const int cpaNoExcludeMask =   0x04;
const int cpaNoClearArchive =  0x08;
const int cpaNoPreserveTime =  0x10;
const int cpaNoRights =        0x20;
const int cpaNoPreserveReadOnly = 0x40;
const int cpaNoIgnorePermErrors = 0x80;
//---------------------------------------------------------------------------
struct TUsableCopyParamAttrs
{
  int General;
  int Upload;
  int Download;
};
//---------------------------------------------------------------------------
class TCopyParamType {
private:
  TFileMasks FAsciiFileMask;
  TFileNameCase FFileNameCase;
  bool FPreserveReadOnly;
  bool FPreserveTime;
  TRights FRights;
  TTransferMode FTransferMode;
  bool FAddXToDirectories;
  bool FPreserveRights;
  bool FIgnorePermErrors;
  TResumeSupport FResumeSupport;
  __int64 FResumeThreshold;
  char FInvalidCharsReplacement;
  std::wstring FLocalInvalidChars;
  std::wstring FTokenizibleChars;
  bool FCalculateSize;
  std::wstring FFileMask;
  TFileMasks FExcludeFileMask;
  bool FNegativeExclude;
  bool FClearArchive;
  unsigned long FCPSLimit;
  static const char TokenPrefix = '%';
  static const char NoReplacement = char(false);
  static const char TokenReplacement = char(true);

  static std::wstring Untokenize(std::wstring FileName);
  char * ReplaceChar(std::wstring & FileName, char * InvalidChar) const;
  std::wstring RestoreChars(std::wstring FileName) const;

public:
  TCopyParamType();
  TCopyParamType(const TCopyParamType & Source);
  virtual ~TCopyParamType();
  TCopyParamType & operator =(const TCopyParamType & rhp);
  virtual void Assign(const TCopyParamType * Source);
  virtual void Default();
  std::wstring ChangeFileName(std::wstring FileName,
    TOperationSide Side, bool FirstLevel) const;
  int LocalFileAttrs(const TRights & Rights) const;
  TRights RemoteFileRights(int Attrs) const;
  bool UseAsciiTransfer(std::wstring FileName, TOperationSide Side,
    const TFileMasks::TParams & Params) const;
  bool AllowResume(__int64 Size) const;
  std::wstring ValidLocalFileName(std::wstring FileName) const;
  std::wstring ValidLocalPath(std::wstring Path) const;
  bool AllowAnyTransfer() const;
  bool AllowTransfer(std::wstring FileName, TOperationSide Side,
    bool Directory, const TFileMasks::TParams & Params) const;

  void Load(THierarchicalStorage * Storage);
  void Save(THierarchicalStorage * Storage) const;
  std::wstring GetInfoStr(std::wstring Separator, int Attrs) const;

  bool operator==(const TCopyParamType & rhp) const;

  // __property TFileMasks AsciiFileMask = { read = FAsciiFileMask, write = FAsciiFileMask };
  TFileMasks GetAsciiFileMask() { return FAsciiFileMask; }
  void SetAsciiFileMask(TFileMasks value) { FAsciiFileMask = value; }
  // __property TFileNameCase FileNameCase = { read = FFileNameCase, write = FFileNameCase };
  TFileNameCase GetFileNameCase() { return FFileNameCase; }
  void SetFileNameCase(TFileNameCase value) { FFileNameCase = value; }
  // __property bool PreserveReadOnly = { read = FPreserveReadOnly, write = FPreserveReadOnly };
  bool GetPreserveReadOnly() { return FPreserveReadOnly; }
  void SetPreserveReadOnly(bool value) { FPreserveReadOnly = value; }
  // __property bool PreserveTime = { read = FPreserveTime, write = FPreserveTime };
  bool GetPreserveTime() { return FPreserveTime; }
  void SetPreserveTime(bool value) { FPreserveTime = value; }
  // __property TRights Rights = { read = FRights, write = FRights };
  TRights GetRights() { return FRights; }
  void SetRights(TRights value) { FRights = value; }
  // __property TTransferMode TransferMode = { read = FTransferMode, write = FTransferMode };
  TTransferMode GetTransferMode() { return FTransferMode; }
  void SetTransferMode(TTransferMode value) { FTransferMode = value; }
  // __property std::wstring LogStr  = { read=GetLogStr };
  std::wstring GetLogStr() const;
  // __property bool AddXToDirectories  = { read=FAddXToDirectories, write=FAddXToDirectories };
  bool GetAddXToDirectories() { return FAddXToDirectories; }
  // __property bool PreserveRights = { read = FPreserveRights, write = FPreserveRights };
  bool GetPreserveRights() { return FPreserveRights; }
  void SetPreserveRights(bool value) { FPreserveRights = value; }
  // __property bool IgnorePermErrors = { read = FIgnorePermErrors, write = FIgnorePermErrors };
  bool GetIgnorePermErrors() { return FIgnorePermErrors; }
  void SetIgnorePermErrors(bool value) { FIgnorePermErrors = value; }
  // __property TResumeSupport ResumeSupport = { read = FResumeSupport, write = FResumeSupport };
  TResumeSupport GetResumeSupport() { return FResumeSupport; }
  void SetResumeSupport(TResumeSupport value) { FResumeSupport = value; }
  // __property __int64 ResumeThreshold = { read = FResumeThreshold, write = FResumeThreshold };
  __int64 GetResumeThreshold() { return FResumeThreshold; }
  void SetResumeThreshold(__int64 value) { FResumeThreshold = value; }
  // __property char InvalidCharsReplacement = { read = FInvalidCharsReplacement, write = FInvalidCharsReplacement };
  char GetInvalidCharsReplacement() { return FInvalidCharsReplacement; }
  void SetInvalidCharsReplacement(char value) { FInvalidCharsReplacement = value; }
  // __property bool ReplaceInvalidChars = { read = GetReplaceInvalidChars, write = SetReplaceInvalidChars };
  bool GetReplaceInvalidChars() const;
  void SetReplaceInvalidChars(bool value);
  // __property std::wstring LocalInvalidChars = { read = FLocalInvalidChars, write = SetLocalInvalidChars };
  std::wstring GetLocalInvalidChars() { return FLocalInvalidChars; }
  void SetLocalInvalidChars(std::wstring value);
  // __property bool CalculateSize = { read = FCalculateSize, write = FCalculateSize };
  bool GetCalculateSize() { return FCalculateSize; }
  void SetCalculateSize(bool value) { FCalculateSize = value; }
  // __property std::wstring FileMask = { read = FFileMask, write = FFileMask };
  std::wstring GetFileMask() { return FFileMask; }
  void SetFileMask(std::wstring value) { FFileMask = value; }
  // __property TFileMasks ExcludeFileMask = { read = FExcludeFileMask, write = FExcludeFileMask };
  TFileMasks GetExcludeFileMask() { return FExcludeFileMask; }
  void SetExcludeFileMask(TFileMasks value) { FExcludeFileMask = value; }
  // __property bool NegativeExclude = { read = FNegativeExclude, write = FNegativeExclude };
  bool GetNegativeExclude() { return FNegativeExclude; }
  void SetNegativeExclude(bool value) { FNegativeExclude = value; }
  // __property bool ClearArchive = { read = FClearArchive, write = FClearArchive };
  bool GetClearArchive() { return FClearArchive; }
  void SetClearArchive(bool value) { FClearArchive = value; }
  // __property unsigned long CPSLimit = { read = FCPSLimit, write = FCPSLimit };
  unsigned long GetCPSLimit() { return FCPSLimit; }
  void SetCPSLimit(unsigned long value) { FCPSLimit = value; }
};
//---------------------------------------------------------------------------
unsigned long GetSpeedLimit(const std::wstring & Text);
std::wstring SetSpeedLimit(unsigned long Limit);
//---------------------------------------------------------------------------
#endif
