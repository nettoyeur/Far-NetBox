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
const int cpaIncludeMaskOnly = 0x01;
const int cpaNoTransferMode =  0x02;
const int cpaNoIncludeMask =   0x04;
const int cpaNoClearArchive =  0x08;
const int cpaNoPreserveTime =  0x10;
const int cpaNoRights =        0x20;
const int cpaNoPreserveReadOnly = 0x40;
const int cpaNoIgnorePermErrors = 0x80;
const int cpaNoNewerOnly        = 0x100;
//---------------------------------------------------------------------------
struct TUsableCopyParamAttrs
{
  int General;
  int Upload;
  int Download;
};
//---------------------------------------------------------------------------
class TCopyParamType : public TObject
{
public:
  TCopyParamType();
  TCopyParamType(const TCopyParamType & Source);
  virtual ~TCopyParamType();
  TCopyParamType & operator =(const TCopyParamType & rhp);
  virtual void Assign(const TCopyParamType * Source);
  virtual void Default();
  UnicodeString ChangeFileName(const UnicodeString & FileName,
    TOperationSide Side, bool FirstLevel) const;
  DWORD LocalFileAttrs(const TRights & Rights) const;
  TRights RemoteFileRights(uintptr_t Attrs) const;
  bool UseAsciiTransfer(const UnicodeString & FileName, TOperationSide Side,
    const TFileMasks::TParams & Params) const;
  bool AllowResume(__int64 Size) const;
  inline UnicodeString ValidLocalFileName(const UnicodeString & FileName) const;
  UnicodeString ValidLocalPath(const UnicodeString & Path) const;
  bool AllowAnyTransfer() const;
  bool AllowTransfer(const UnicodeString & FileName, TOperationSide Side,
    bool Directory, const TFileMasks::TParams & Params) const;

  void Load(THierarchicalStorage * Storage);
  void Save(THierarchicalStorage * Storage) const;
  UnicodeString GetInfoStr(const UnicodeString & Separator, intptr_t Options) const;
  bool AnyUsableCopyParam(intptr_t Attrs) const;

  bool operator==(const TCopyParamType & rhp) const;

  const TFileMasks & GetAsciiFileMask() const { return FAsciiFileMask; }
  TFileMasks & GetAsciiFileMask() { return FAsciiFileMask; }
  void SetAsciiFileMask(const TFileMasks & Value) { FAsciiFileMask = Value; }
  const TFileNameCase & GetFileNameCase() const { return FFileNameCase; }
  void SetFileNameCase(TFileNameCase Value) { FFileNameCase = Value; }
  bool GetPreserveReadOnly() const { return FPreserveReadOnly; }
  void SetPreserveReadOnly(bool Value) { FPreserveReadOnly = Value; }
  bool GetPreserveTime() const { return FPreserveTime; }
  void SetPreserveTime(bool Value) { FPreserveTime = Value; }
  const TRights & GetRights() const { return FRights; }
  TRights & GetRights() { return FRights; }
  void SetRights(const TRights & Value) { FRights.Assign(&Value); }
  TTransferMode GetTransferMode() const { return FTransferMode; }
  void SetTransferMode(TTransferMode Value) { FTransferMode = Value; }
  UnicodeString GetLogStr() const;
  bool GetAddXToDirectories() const { return FAddXToDirectories; }
  void SetAddXToDirectories(bool Value) { FAddXToDirectories = Value; }
  bool GetPreserveRights() const { return FPreserveRights; }
  void SetPreserveRights(bool Value) { FPreserveRights = Value; }
  bool GetIgnorePermErrors() const { return FIgnorePermErrors; }
  void SetIgnorePermErrors(bool Value) { FIgnorePermErrors = Value; }
  TResumeSupport GetResumeSupport() const { return FResumeSupport; }
  void SetResumeSupport(TResumeSupport Value) { FResumeSupport = Value; }
  __int64 GetResumeThreshold() const { return FResumeThreshold; }
  void SetResumeThreshold(__int64 Value) { FResumeThreshold = Value; }
  wchar_t GetInvalidCharsReplacement() const { return FInvalidCharsReplacement; }
  void SetInvalidCharsReplacement(wchar_t Value) { FInvalidCharsReplacement = Value; }
  UnicodeString GetLocalInvalidChars() const { return FLocalInvalidChars; }
  bool GetCalculateSize() const { return FCalculateSize; }
  void SetCalculateSize(bool Value) { FCalculateSize = Value; }
  UnicodeString GetFileMask() const { return FFileMask; }
  void SetFileMask(const UnicodeString & Value) { FFileMask = Value; }
  const TFileMasks & GetIncludeFileMask() const { return FIncludeFileMask; }
  TFileMasks & GetIncludeFileMask() { return FIncludeFileMask; }
  void SetIncludeFileMask(const TFileMasks & Value) { FIncludeFileMask = Value; }
  bool GetClearArchive() const { return FClearArchive; }
  void SetClearArchive(bool Value) { FClearArchive = Value; }
  uintptr_t GetCPSLimit() const { return FCPSLimit; }
  void SetCPSLimit(uintptr_t Value) { FCPSLimit = Value; }
  bool GetNewerOnly() const { return FNewerOnly; }
  void SetNewerOnly(bool Value) { FNewerOnly = Value; }

public:
  static const wchar_t TokenPrefix = L'%';
  static const wchar_t NoReplacement = wchar_t(0);
  static const wchar_t TokenReplacement = wchar_t(1);

public:
  void SetLocalInvalidChars(const UnicodeString & Value);
  bool GetReplaceInvalidChars() const;
  void SetReplaceInvalidChars(bool Value);
  UnicodeString RestoreChars(const UnicodeString & FileName) const;
  void DoGetInfoStr(const UnicodeString & Separator, intptr_t Options,
    UnicodeString & Result, bool & SomeAttrIncluded) const;

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
  wchar_t FInvalidCharsReplacement;
  UnicodeString FLocalInvalidChars;
  UnicodeString FTokenizibleChars;
  bool FCalculateSize;
  UnicodeString FFileMask;
  TFileMasks FIncludeFileMask;
  bool FClearArchive;
  uintptr_t FCPSLimit;
  bool FNewerOnly;
};
//---------------------------------------------------------------------------
uintptr_t  GetSpeedLimit(const UnicodeString & Text);
UnicodeString SetSpeedLimit(uintptr_t Limit);
//---------------------------------------------------------------------------
#endif
