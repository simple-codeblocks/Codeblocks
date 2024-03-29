// SqPlus.h
// Created by John Schultz 9/05/05, major update 10/05/05.
// Template function call design from LuaPlusCD by Joshua C. Jensen,
// inspired by luabind which was inspired by boost::python.
// Const argument, const member functions, and Mac OS-X changes by Simon Michelmore.
// DECLARE_INSTANCE_TYPE_NAME changes by Ben (Project5) from http://www.squirrel-lang.org/forums/.
// Added Kamaitati's changes 5/28/06.
// Free for any use.

#ifndef _SQ_PLUS_H_
#define _SQ_PLUS_H_

#include <stdlib.h>

#ifdef __APPLE__
  #include <malloc/malloc.h>
#else
  // C::B patch: support for FreeBSD
  #ifdef __FreeBSD__
    #include <stdlib.h>
  #else
    #include <malloc.h>
  #endif
#endif
#include <memory.h>
#if defined(_MSC_VER) || defined(__BORLANDC__)
  #include <tchar.h>
  #ifndef UNICODE
    #define SCSNPRINTF _snprintf
    #define SCPUTS puts
  #else
    #define SCSNPRINTF _snwprintf
    #define SCPUTS _putws
  #endif
#else
  // C::B patch: Convert conflicting _T() to sqT() --> applies to nearly *all* files!
//  #define _T(n) n
  #define sqT(n) n
  #define SCSNPRINTF snprintf
  #include <stdio.h> // for snprintf
  #define SCPUTS puts
#endif

#if 1
#define SQ_CALL_RAISE_ERROR SQTrue
#else
#define SQ_CALL_RAISE_ERROR SQFalse
#endif

#include "squirrel.h"

// C::B patch: so it builds on 64bit, ecapsulate bool/int/float using Squirrel types (this patch applies everywhere, where threse types are used)
typedef SQInteger BOOL_T;
typedef SQInteger INT_T;
typedef SQFloat   FLOAT_T;
#define TRUE 1
#define FALSE 0

#include "SquirrelObject.h"
#include "SquirrelVM.h"
#include "SquirrelBindingsUtils.h"

namespace SqPlus {

// === Class Inheritance Support ===
// Inheritance in Squirrel allows one class to inherit a base class's functions and variables.
// Variables are merged: if Derived has a var name 'val' and Base has a var of the same name,
// the resulting var 'val' will take Derived's initialization value.
// Functions are not merged, and can be called via Squirrel scoping rules.

// Define SQ_USE_CLASS_INHERITANCE to enable class inheritance support
// (requires slightly more memory and adds some CPU overhead).
// Can also be useful for debugging, as class type information is checked before
// dispatching instance function calls and before accessing instance member variables.
#define SQ_USE_CLASS_INHERITANCE

// Comment out to turn off instance type info support (to save a small amount of memory).
#define SQ_SUPPORT_INSTANCE_TYPE_INFO

// === Constant argument and constant member function support ===
// Define SQPLUS_CONST_OPT before including SqPlus.h for constant argument + constant member function support.
// C::B patch: Enable this option
#define SQPLUS_CONST_OPT

// === Uncomment to support std::string ===
//#define SQPLUS_SUPPORT_STD_STRING

// === Uncomment to support typedef std::basic_string<SQChar> sq_std_string ===
//#define SQPLUS_SUPPORT_SQ_STD_STRING

// === Uncomment to support NULL INSTANCE arguments ===
//#define SQPLUS_SUPPORT_NULL_INSTANCES

// === Uncomment to support C++ style inheritance
#define CPP_STYLE_INHERITANCE

// === Uncomment to skip sq_argassert() ===
//#define SQ_SKIP_ARG_ASSERT

template<class T> struct TypeWrapper {};
struct SquirrelNull {};
struct SQAnything { void * anything; }; // Needed for binding pointers to variables (cannot dereference void *).
typedef SQAnything * SQAnythingPtr;
typedef SQChar * SQCharPtr;

// === Do not use directly: use one of the predefined sizes below ===

struct ScriptStringVarBase {
  //const unsigned char MaxLength; // Real length is MaxLength+1.
  const SQInteger MaxLength; // C::B patch- to eliminate compiler warning
  SQChar s[1];
  ScriptStringVarBase(SQInteger _MaxLength) : MaxLength(_MaxLength) {}
  operator SQChar * () { return &s[0]; }
  operator void * () { return (void *)&s[0]; }
  const SQChar * operator = (const SQChar * _s) {
    return safeStringCopy(s,_s,MaxLength);
  }
  // Special safe string copy where MaxLength is 1 less than true buffer length.
  // strncpy() pads out nulls for the full length of the buffer specified by MaxLength.
  static inline SQChar * safeStringCopy(SQChar * d,const SQChar * s,SQInteger MaxLength) {
    SQInteger i=0;
    while (s[i]) {
      d[i] = s[i];
      i++;
      if (i == MaxLength) break;
    } // while
    d[i] = 0; // Null terminate.
    return d;
  } // safeStringCopy
};

// === Do not use directly: use one of the predefined sizes below ===

template<SQInteger MAXLENGTH> // MAXLENGTH is max printable characters (trailing NULL is accounted for in ScriptStringVarBase::s[1]).
struct ScriptStringVar : ScriptStringVarBase {
  SQChar ps[MAXLENGTH];
  ScriptStringVar() : ScriptStringVarBase(MAXLENGTH) {
    s[0] = 0;
  }
  ScriptStringVar(const SQChar * _s) : ScriptStringVarBase(MAXLENGTH) {
    *this = _s;
  }
  const SQChar * operator = (const SQChar * _s) {
    return safeStringCopy(s,_s,MaxLength);
  }
  const SQChar * operator = (const ScriptStringVar & _s) {
    return safeStringCopy(s,_s.s,MaxLength);
  }
  bool operator == (const ScriptStringVar & _s) {
    return _strcmp(s,_s.s) == 0;
  }
  bool compareCaseInsensitive(const ScriptStringVar & _s) {
    return _stricmp(s,_s.s) == 0;
  }
};

// === Fixed size strings for scripting ===

typedef ScriptStringVar<8>   ScriptStringVar8;
typedef ScriptStringVar<16>  ScriptStringVar16;
typedef ScriptStringVar<32>  ScriptStringVar32;
typedef ScriptStringVar<64>  ScriptStringVar64;
typedef ScriptStringVar<128> ScriptStringVar128;
typedef ScriptStringVar<256> ScriptStringVar256;

// === Script Variable Types ===

enum ScriptVarType {VAR_TYPE_NONE=-1,VAR_TYPE_INT=0,VAR_TYPE_FLOAT,VAR_TYPE_BOOL,VAR_TYPE_CONST_STRING,VAR_TYPE_STRING,VAR_TYPE_USER_POINTER,VAR_TYPE_INSTANCE};

template <typename T>
struct TypeInfo {
  const SQChar * typeName;
  enum {TypeID=VAR_TYPE_NONE,Size=0};
};

// === Common Variable Types ===

template<>
struct TypeInfo<INT_T> {
  const SQChar * typeName;
  TypeInfo() : typeName(sqT("int")) {}
  enum {TypeID=VAR_TYPE_INT,Size=sizeof(INT_T)};
  operator ScriptVarType() { return ScriptVarType(TypeID); }
};

template<>
struct TypeInfo<FLOAT_T> {
  const SQChar * typeName;
  TypeInfo() : typeName(sqT("float")) {}
  enum {TypeID=VAR_TYPE_FLOAT,Size=sizeof(FLOAT_T)};
  operator ScriptVarType() { return ScriptVarType(TypeID); }
};

template<>
struct TypeInfo<bool> {
  const SQChar * typeName;
  TypeInfo() : typeName(sqT("bool")) {}
  enum {TypeID=VAR_TYPE_BOOL,Size=sizeof(BOOL_T)};
  operator ScriptVarType() { return ScriptVarType(TypeID); }
};

template<>
struct TypeInfo<SQUserPointer> {
  const SQChar * typeName;
  TypeInfo() : typeName(sqT("SQUserPointer")) {}
  enum {TypeID=VAR_TYPE_USER_POINTER,Size=sizeof(SQUserPointer)};
  operator ScriptVarType() { return ScriptVarType(TypeID); }
};

template<>
struct TypeInfo<SQAnything> {
  const SQChar * typeName;
  TypeInfo() : typeName(sqT("SQUserPointer")) {}
  enum {TypeID=VAR_TYPE_USER_POINTER,Size=sizeof(SQUserPointer)};
  operator ScriptVarType() { return ScriptVarType(TypeID); }
};

template<>
struct TypeInfo<const SQChar *> {
  const SQChar * typeName;
  TypeInfo() : typeName(sqT("const SQChar *")) {}
  enum {TypeID=VAR_TYPE_CONST_STRING,Size=sizeof(const SQChar *)};
  operator ScriptVarType() { return ScriptVarType(TypeID); }
};

template<>
struct TypeInfo<ScriptStringVarBase> {
  const SQChar * typeName;
  TypeInfo() : typeName(sqT("ScriptStringVarBase")) {}
  enum {TypeID=VAR_TYPE_STRING,Size=sizeof(ScriptStringVarBase)};
  operator ScriptVarType() { return ScriptVarType(TypeID); }
};

// === Fixed String Variants ===

template<>
struct TypeInfo<ScriptStringVar8> {
  const SQChar * typeName;
  TypeInfo() : typeName(sqT("ScriptStringVar8")) {}
  enum {TypeID=VAR_TYPE_STRING,Size=sizeof(ScriptStringVar8)};
  operator ScriptVarType() { return ScriptVarType(TypeID); }
};

template<>
struct TypeInfo<ScriptStringVar16> {
  const SQChar * typeName;
  TypeInfo() : typeName(sqT("ScriptStringVar16")) {}
  enum {TypeID=VAR_TYPE_STRING,Size=sizeof(ScriptStringVar16)};
  operator ScriptVarType() { return ScriptVarType(TypeID); }
};

template<>
struct TypeInfo<ScriptStringVar32> {
  const SQChar * typeName;
  TypeInfo() : typeName(sqT("ScriptStringVar32")) {}
  enum {TypeID=VAR_TYPE_STRING,Size=sizeof(ScriptStringVar32)};
  operator ScriptVarType() { return ScriptVarType(TypeID); }
};

template<>
struct TypeInfo<ScriptStringVar64> {
  const SQChar * typeName;
  TypeInfo() : typeName(sqT("ScriptStringVar64")) {}
  enum {TypeID=VAR_TYPE_STRING,Size=sizeof(ScriptStringVar64)};
  operator ScriptVarType() { return ScriptVarType(TypeID); }
};

template<>
struct TypeInfo<ScriptStringVar128> {
  const SQChar * typeName;
  TypeInfo() : typeName(sqT("ScriptStringVar128")) {}
  enum {TypeID=VAR_TYPE_STRING,Size=sizeof(ScriptStringVar128)};
  operator ScriptVarType() { return ScriptVarType(TypeID); }
};

template<>
struct TypeInfo<ScriptStringVar256> {
  const SQChar * typeName;
  TypeInfo() : typeName(sqT("ScriptStringVar256")) {}
  enum {TypeID=VAR_TYPE_STRING,Size=sizeof(ScriptStringVar256)};
  operator ScriptVarType() { return ScriptVarType(TypeID); }
};

enum VarAccessType {VAR_ACCESS_READ_WRITE=0,VAR_ACCESS_READ_ONLY=1<<0,VAR_ACCESS_CONSTANT=1<<1,VAR_ACCESS_STATIC=1<<2};

// See VarRef and ClassType<> below: for instance assignment.
typedef void (*CopyVarFunc)(void * dst,void * src);

// === Variable references for script access ===

#define SQ_PLUS_TYPE_TABLE sqT("__SqTypes")

struct VarRef {
  // In this case 'offsetOrAddrOrConst' is simpler than using an anonymous union.
  void * offsetOrAddrOrConst; // Instance member variable offset from 'this' pointer base (as size_t), or address if static variable (void *), or constant value.
  ScriptVarType type;         // Variable type (from enum above).
  SQUserPointer instanceType; // Unique ID for the containing class instance (for instance vars only). When the var is an instance, its type is encoded in copyFunc.
  CopyVarFunc copyFunc;       // Function pointer to copy variables (for instance variables only).
  //short size;                 // Currently for debugging only (size of item when pointer to item is dereferenced). Could be used for variable max string buffer length.
  SQInteger size;                 // Currently for debugging only (size of item when pointer to item is dereferenced). Could be used for variable max string buffer length.
  short access;               // VarAccessType.
  const SQChar * typeName;    // Type name string (to create instances by name).
  VarRef() : offsetOrAddrOrConst(0), type(VAR_TYPE_NONE), instanceType((SQUserPointer)-1), copyFunc(0), size(0), access(VAR_ACCESS_READ_WRITE) {}
  VarRef(void * _offsetOrAddrOrConst, ScriptVarType _type, SQUserPointer _instanceType, CopyVarFunc _copyFunc, SQInteger _size,VarAccessType _access,const SQChar * _typeName) :
         offsetOrAddrOrConst(_offsetOrAddrOrConst), type(_type), instanceType(_instanceType), copyFunc(_copyFunc), size(_size), access(_access), typeName(_typeName) {
#ifdef SQ_SUPPORT_INSTANCE_TYPE_INFO
    SquirrelObject typeTable = SquirrelVM::GetRootTable().GetValue(SQ_PLUS_TYPE_TABLE);
    if (typeTable.IsNull()) {
      typeTable = SquirrelVM::CreateTable();
      SquirrelObject root = SquirrelVM::GetRootTable();
      root.SetValue(SQ_PLUS_TYPE_TABLE,typeTable);
    } // if
    typeTable.SetValue(INT_T((size_t)copyFunc),typeName);
#endif
  }
};

typedef VarRef * VarRefPtr;

// Internal use only.
inline void getVarNameTag(SQChar * buff,INT_T maxSize,const SQChar * scriptName) {
//  assert(maxSize > 3);
#if 1
  SQChar * d = buff;
  d[0] = '_';
  d[1] = 'v';
  d = &d[2];
  maxSize -= (2+1); // +1 = space for null.
  SQInteger pos=0;
  while (scriptName[pos] && pos < maxSize) {
    d[pos] = scriptName[pos];
    pos++;
  } // while
  d[pos] = 0; // null terminate.
#else
  SCSNPRINTF(buff,maxSize,sqT("_v%s"),scriptName);
#endif
} // getVarNameTag

// Internal use only.
SQInteger setVarFunc(HSQUIRRELVM v);
SQInteger getVarFunc(HSQUIRRELVM v);
SQInteger setInstanceVarFunc(HSQUIRRELVM v);
SQInteger getInstanceVarFunc(HSQUIRRELVM v);

// === BEGIN Helpers ===

inline void createTableSetGetHandlers(SquirrelObject & so) {
  SquirrelObject delegate = so.GetDelegate();
  if (!delegate.Exists(sqT("_set"))) {
    delegate = SquirrelVM::CreateTable();
    SquirrelVM::CreateFunction(delegate,setVarFunc,sqT("_set"),sqT("sn|b|s")); // String var name = number(SQInteger or float) or bool or string.
    SquirrelVM::CreateFunction(delegate,getVarFunc,sqT("_get"),sqT("s"));      // String var name.
    so.SetDelegate(delegate);
  } // if
} // createTableSetGetHandlers

inline VarRefPtr createVarRef(SquirrelObject & so,const SQChar * scriptVarName) {
  VarRefPtr pvr=0;
  ScriptStringVar256 scriptVarTagName; getVarNameTag(scriptVarTagName,sizeof(scriptVarTagName),scriptVarName);
  if (!so.GetUserData(scriptVarTagName,(SQUserPointer *)&pvr)) {
    so.NewUserData(scriptVarTagName,sizeof(*pvr));
    if (!so.GetUserData(scriptVarTagName,(SQUserPointer *)&pvr)) throw SquirrelError(sqT("Could not create UserData."));
  } // if
  return pvr;
} // createVarRef

template<typename T>
void validateConstantType(T /*constant*/) {
  switch(TypeInfo<T>()) {
  case VAR_TYPE_INT:
  case VAR_TYPE_FLOAT:
  case VAR_TYPE_BOOL:
  case VAR_TYPE_CONST_STRING:
    break;
  default:
    throw SquirrelError(sqT("validateConstantType(): type must be INT, FLOAT, BOOL, or CONST CHAR *."));
  } // case
} // validateConstantType

inline void createInstanceSetGetHandlers(SquirrelObject & so) {
  if (!so.Exists(sqT("_set"))) {
    SquirrelVM::CreateFunction(so,setInstanceVarFunc,sqT("_set"),sqT("sn|b|s|x")); // String var name = number(SQInteger or float) or bool or string or instance.
    SquirrelVM::CreateFunction(so,getInstanceVarFunc,sqT("_get"),sqT("s"));      // String var name.
  } // if
} // createInstanceSetGetHandlers

// === END Helpers ===

// === Class Type Helper class: returns a unique number for each class type ===

template<typename T>
struct ClassType {
  static SQUserPointer type(void) { return (SQUserPointer)&copy; }
  static CopyVarFunc getCopyFunc(void) { return (CopyVarFunc)&copy; }
  static void copy(T * dst,T * src) {
    *dst = *src;
  } // copy
};

// === Bind a global or pre-allocated (not instance) class member variable or constant (for tables only (not classes)) ===

template<typename T>
void BindVariable(SquirrelObject & so,T * var,const SQChar * scriptVarName,VarAccessType access=VAR_ACCESS_READ_WRITE) {
  VarRefPtr pvr = createVarRef(so,scriptVarName);
  *pvr = VarRef(var,TypeInfo<T>(),0,ClassType<T>::getCopyFunc(),sizeof(*var),access,TypeInfo<T>().typeName);
  createTableSetGetHandlers(so);
} // BindVariable

// === Bind a constant by value: INT_T, FLOAT_T, BOOL_T, or CONST CHAR * (for tables only (not classes)) ===

template<typename T>
void BindConstant(SquirrelObject & so,T constant,const SQChar * scriptVarName) {
  validateConstantType(constant);
  VarRefPtr pvr = createVarRef(so,scriptVarName);
  struct CV {
    T var;
  } cv; // Cast Variable helper.
  cv.var = constant;
  *pvr = VarRef(*(void **)&cv,TypeInfo<T>(),0,0,sizeof(constant),VAR_ACCESS_CONSTANT,TypeInfo<T>().typeName);
  createTableSetGetHandlers(so);
} // BindConstant

template<typename T>
void BindVariable(T * var,const SQChar * scriptVarName,VarAccessType access=VAR_ACCESS_READ_WRITE) {
  SquirrelObject so = SquirrelVM::GetRootTable();
  BindVariable(so,var,scriptVarName,access);
} // BindVariable

template<typename T>
void BindConstant(T constant,const SQChar * scriptVarName) {
  SquirrelObject so = SquirrelVM::GetRootTable();
  BindConstant(so,constant,scriptVarName);
} // BindConstant

// === Register a class instance member variable or constant. var argument provides type and offset ( effectively &((ClassType *)0)->var ) ===

// classType is the type of the member variable's containing class.
template<typename T>
void RegisterInstanceVariable(SquirrelObject & so,SQUserPointer classType,T * var,const SQChar * scriptVarName,VarAccessType access=VAR_ACCESS_READ_WRITE) {
  VarRef * pvr = createVarRef(so,scriptVarName);
  void * offsetOrAddrOrConst = (void *)var; // var must be passed in as &obj->var, where obj = 0 (the address is the offset), or as static/global address.
  *pvr = VarRef(offsetOrAddrOrConst,TypeInfo<T>(),classType,ClassType<T>::getCopyFunc(),sizeof(*var),access,TypeInfo<T>().typeName);
  createInstanceSetGetHandlers(so);
} // RegisterInstanceVariable

template<typename T>
void RegisterInstanceConstant(SquirrelObject & so,SQUserPointer classType,T constant,const SQChar * scriptVarName) {
  validateConstantType(constant);
  VarRef * pvr = createVarRef(so,scriptVarName);
  struct CV {
    T var;
    size_t pad;
  } cv; // Cast Variable helper.
  cv.var = constant;
  *pvr = VarRef(*(void **)&cv,TypeInfo<T>(),classType,0,sizeof(constant),VAR_ACCESS_CONSTANT,TypeInfo<T>().typeName);
  createInstanceSetGetHandlers(so);
} // RegisterInstanceConstant

//////////////////////////////////////////////////////////////////////////
/////////// BEGIN Generalized Class/Struct Instance Support //////////////
//////////////////////////////////////////////////////////////////////////

//BOOL_T CreateNativeClassInstance(HSQUIRRELVM v,const SQChar * classname,SQUserPointer ud,SQRELEASEHOOK hook); // In SquirrelBindingUtils.cpp.

// Create native class instance and leave on stack.
inline BOOL_T CreateConstructNativeClassInstance(HSQUIRRELVM v,const SQChar * className) {
  SQInteger oldtop = sq_gettop(v);
  sq_pushroottable(v);
  sq_pushstring(v,className,-1);
  if (SQ_FAILED(sq_rawget(v,-2))) { // Get the class (created with sq_newclass()).
    sq_settop(v,oldtop);
    return FALSE;
  } // if
#if 0
  sq_remove(v,-3); // Remove the root table.
  sq_push(v,1);    // Push the 'this'.
#else // Kamaitati's change. 5/28/06 jcs.
  sq_remove(v,-2); // Remove the root table.
  sq_pushroottable(v); // Push the 'this'.
#endif
  if (SQ_FAILED(sq_call(v,1,SQTrue,SQ_CALL_RAISE_ERROR))) { // Call ClassName(): creates new instance and calls constructor (instead of sq_createinstance() where constructor is not called).
    sq_settop(v,oldtop);
    return FALSE;
  } // if
  sq_remove(v,-2); // Remove the class.
  //  SQInteger newtop = sq_gettop(v);
  return TRUE;
} // CreateConstructNativeClassInstance

// Create new instance, copy 'classToCopy', and store result on stack.
template<typename T>
inline BOOL_T CreateCopyInstance(const SQChar * className,const T & classToCopy) {
  HSQUIRRELVM v = SquirrelVM::GetVMPtr();
  if (!CreateConstructNativeClassInstance(v,className)) {
    return FALSE;
  } // if
  SQUserPointer up=0;
  sq_getinstanceup(v,-1,&up,ClassType<T>::type());
  if (!up) return FALSE;
  T * newClass = (T *)up;
  *newClass = classToCopy; // <TODO> Optimized version that uses the copy constructor.
  return TRUE;
} // CreateCopyInstance

// Create a new copy of type 'className' and copy 'classToCopy', return result via SquirrelObject.
template<typename T>
inline SquirrelObject NewClassCopy(const SQChar * className,const T & classToCopy) {
  SquirrelObject ret;
  if (CreateCopyInstance(className,classToCopy)) {
    ret.AttachToStackObject(-1);
    sq_poptop(SquirrelVM::GetVMPtr());
  } else {
    throw SquirrelError(sqT("NewClassCopy(): could not create class"));
  } // if
  return ret;
} // NewClassCopy

// Return a new class copy on the stack from a varArgs function call.
template<typename T>
inline SQInteger ReturnCopy(HSQUIRRELVM v,const T & classToCopy) {
  SquirrelObject so(NewClassCopy(GetTypeName(classToCopy),classToCopy));
  return StackHandler(v).Return(so);
} // ReturnCopy

// Katsuaki Kawachi's GetInstance<> exception change. 6/27/06 jcs

// Get an instance of type T from the stack at idx (for function calls).
template<typename T,bool ExceptionOnError>
T * GetInstance(HSQUIRRELVM v,SQInteger idx) {
  SQUserPointer up=0;
  sq_getinstanceup(v,idx,&up,ClassType<T>::type());
  if (ExceptionOnError) { // This code block should be compiled out when ExceptionOnError is false. In any case, the compiler should not generate a test condition (include or exclude the enclosed code block).
    if (!up) throw SquirrelError(sqT("GetInstance: Invalid argument type"));
  } // if
  return (T *)up;
} // GetInstance

// NAME and macro changes from Ben's (Project5) forum post. 2/26/06 jcs
// Kamaitati's NULL_INSTANCE support. 5/28/06 jcs

#ifdef SQPLUS_SUPPORT_NULL_INSTANCES

#define DECLARE_INSTANCE_TYPE_NAME_(TYPE,NAME) namespace SqPlus { \
  inline const SQChar * GetTypeName(const TYPE & /*n*/) { return sqT(#NAME); } \
  inline void Push(HSQUIRRELVM v,TYPE * value) { \
    if (!value)  sq_pushnull(v); \
    else if (!CreateNativeClassInstance(v,GetTypeName(*value),value,0)) \
      throw SquirrelError(sqT("Push(): could not create INSTANCE (check registration name)")); } \
  inline void Push(HSQUIRRELVM /*v*/,TYPE & value) { if (!CreateCopyInstance(GetTypeName(value),value)) throw SquirrelError(sqT("Push(): could not create INSTANCE copy (check registration name)")); } \
  inline bool Match(TypeWrapper<TYPE &>,HSQUIRRELVM v,SQInteger idx) { return  GetInstance<TYPE,false>(v,idx) != NULL; } \
  inline bool Match(TypeWrapper<TYPE *>,HSQUIRRELVM v,SQInteger idx) { \
    return (sq_gettype(v,idx)==OT_NULL) || (GetInstance<TYPE,false>(v,idx) != NULL); } \
  inline TYPE & Get(TypeWrapper<TYPE &>,HSQUIRRELVM v,SQInteger idx) { return *GetInstance<TYPE,true>(v,idx); } \
  inline TYPE * Get(TypeWrapper<TYPE *>,HSQUIRRELVM v,SQInteger idx) { \
    if (sq_gettype(v,idx)==OT_NULL) return NULL; \
    return GetInstance<TYPE,true>(v,idx); } \
  template<> \
  struct TypeInfo<TYPE> { \
    const SQChar * typeName; \
    TypeInfo() : typeName(sqT(#NAME)) {} \
    enum {TypeID=VAR_TYPE_INSTANCE,Size=sizeof(TYPE)}; \
    operator ScriptVarType() { return ScriptVarType(TypeID); } \
  }; \
} // nameSpace SqPlus

#else

#define DECLARE_INSTANCE_TYPE_NAME_(TYPE,NAME) namespace SqPlus { \
  inline const SQChar * GetTypeName(const TYPE & /*n*/)            { return sqT(#NAME); } \
  inline void Push(HSQUIRRELVM v,TYPE * value)                 { if (!CreateNativeClassInstance(v,GetTypeName(*value),value,0)) throw SquirrelError(sqT("Push(): could not create INSTANCE (check registration name)")); } \
  inline void Push(HSQUIRRELVM /*v*/,TYPE & value)                 { if (!CreateCopyInstance(GetTypeName(value),value)) throw SquirrelError(sqT("Push(): could not create INSTANCE copy (check registration name)")); } \
  inline bool	Match(TypeWrapper<TYPE &>,HSQUIRRELVM v,SQInteger idx) { return  GetInstance<TYPE,false>(v,idx) != NULL; } \
  inline bool	Match(TypeWrapper<TYPE *>,HSQUIRRELVM v,SQInteger idx) { return  GetInstance<TYPE,false>(v,idx) != NULL; } \
  inline TYPE & Get(TypeWrapper<TYPE &>,HSQUIRRELVM v,SQInteger idx) { return *GetInstance<TYPE,true>(v,idx); } \
  inline TYPE * Get(TypeWrapper<TYPE *>,HSQUIRRELVM v,SQInteger idx) { return  GetInstance<TYPE,true>(v,idx); } \
  template<> \
  struct TypeInfo<TYPE> { \
    const SQChar * typeName; \
    TypeInfo() : typeName(sqT(#NAME)) {} \
    enum {TypeID=VAR_TYPE_INSTANCE,Size=sizeof(TYPE)}; \
    operator ScriptVarType() { return ScriptVarType(TypeID); } \
  }; \
} // nameSpace SqPlus

#endif

// TYPE or NAME below must match the string name used in SqClassDef<>, otherwise name lookup won't match and Squirrel will throw a "can't create instance" error.
#ifndef SQPLUS_CONST_OPT
#define DECLARE_INSTANCE_TYPE(TYPE) DECLARE_INSTANCE_TYPE_NAME_(TYPE,TYPE)
#define DECLARE_INSTANCE_TYPE_NAME(TYPE,NAME) DECLARE_INSTANCE_TYPE_NAME_(TYPE,NAME)
#else
#define SQPLUS_DECLARE_INSTANCE_TYPE_CONST
#include "SqPlusConst.h"
#endif

//////////////////////////////////////////////////////////////////////////
//////////// END Generalized Class/Struct Instance Support ///////////////
//////////////////////////////////////////////////////////////////////////

#ifndef SQ_SKIP_ARG_ASSERT
  #define sq_argassert(arg,_index_) if (!Match(TypeWrapper<P##arg>(),v,_index_)) return sq_throwerror(v,sqT("Incorrect function argument"))
#else
  #define sq_argassert(arg,_index_)
#endif

// === Return value variants ===

template<class RT>
struct ReturnSpecialization {

  // === Standard Function calls ===

  static SQInteger Call(RT (*func)(),HSQUIRRELVM v,SQInteger /*index*/) {
    RT ret = func();
    Push(v,ret);
    return 1;
  }

  template<typename P1>
  static SQInteger Call(RT (*func)(P1),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    RT ret = func(
      Get(TypeWrapper<P1>(),v,index + 0)
    );
    Push(v,ret);
    return 1;
  }

  template<typename P1,typename P2>
  static SQInteger Call(RT (*func)(P1,P2),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    RT ret = func(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1)
    );
    Push(v,ret);
    return 1;
  }

  template<typename P1,typename P2,typename P3>
  static SQInteger Call(RT (*func)(P1,P2,P3),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    RT ret = func(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2)
    );
    Push(v,ret);
    return 1;
  }

  template<typename P1,typename P2,typename P3,typename P4>
  static SQInteger Call(RT (*func)(P1,P2,P3,P4),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    RT ret = func(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3)
    );
    Push(v,ret);
    return 1;
  }

  template<typename P1,typename P2,typename P3,typename P4,typename P5>
  static SQInteger Call(RT (*func)(P1,P2,P3,P4,P5),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    sq_argassert(5,index + 4);
    RT ret = func(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3),
      Get(TypeWrapper<P5>(),v,index + 4)
    );
    Push(v,ret);
    return 1;
  }

  template<typename P1,typename P2,typename P3,typename P4,typename P5,typename P6>
  static SQInteger Call(RT (*func)(P1,P2,P3,P4,P5,P6),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    sq_argassert(5,index + 4);
    sq_argassert(6,index + 5);
    RT ret = func(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3),
      Get(TypeWrapper<P5>(),v,index + 4),
      Get(TypeWrapper<P6>(),v,index + 5)
    );
    Push(v,ret);
    return 1;
  }

  template<typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7>
  static SQInteger Call(RT (*func)(P1,P2,P3,P4,P5,P6,P7),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    sq_argassert(5,index + 4);
    sq_argassert(6,index + 5);
    sq_argassert(7,index + 6);
    RT ret = func(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3),
      Get(TypeWrapper<P5>(),v,index + 4),
      Get(TypeWrapper<P6>(),v,index + 5),
      Get(TypeWrapper<P7>(),v,index + 6)
    );
    Push(v,ret);
    return 1;
  }

  // === Member Function calls ===

  template <typename Callee>
  static SQInteger Call(Callee & callee,RT (Callee::*func)(),HSQUIRRELVM v,SQInteger /*index*/) {
    RT ret = (callee.*func)();
    Push(v,ret);
    return 1;
  }

  template <typename Callee,typename P1>
  static SQInteger Call(Callee & callee,RT (Callee::*func)(P1),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    RT ret = (callee.*func)(
      Get(TypeWrapper<P1>(),v,index + 0)
    );
    Push(v,ret);
    return 1;
  }

  template<typename Callee,typename P1,typename P2>
  static SQInteger Call(Callee & callee,RT (Callee::*func)(P1,P2),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    RT ret = (callee.*func)(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1)
    );
    Push(v,ret);
    return 1;
  }

  template<typename Callee,typename P1,typename P2,typename P3>
  static SQInteger Call(Callee & callee,RT (Callee::*func)(P1,P2,P3),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    RT ret = (callee.*func)(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2)
    );
    Push(v,ret);
    return 1;
  }

  template<typename Callee,typename P1,typename P2,typename P3,typename P4>
  static SQInteger Call(Callee & callee,RT (Callee::*func)(P1,P2,P3,P4),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    RT ret = (callee.*func)(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3)
    );
    Push(v,ret);
    return 1;
  }

  template<typename Callee,typename P1,typename P2,typename P3,typename P4,typename P5>
  static SQInteger Call(Callee & callee,RT (Callee::*func)(P1,P2,P3,P4,P5),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    sq_argassert(5,index + 4);
    RT ret = (callee.*func)(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3),
      Get(TypeWrapper<P5>(),v,index + 4)
    );
    Push(v,ret);
    return 1;
  }

  template<typename Callee,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6>
  static SQInteger Call(Callee & callee,RT (Callee::*func)(P1,P2,P3,P4,P5,P6),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    sq_argassert(5,index + 4);
    sq_argassert(6,index + 5);
    RT ret = (callee.*func)(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3),
      Get(TypeWrapper<P5>(),v,index + 4),
      Get(TypeWrapper<P6>(),v,index + 5)
    );
    Push(v,ret);
    return 1;
  }

  template<typename Callee,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7>
  static SQInteger Call(Callee & callee,RT (Callee::*func)(P1,P2,P3,P4,P5,P6,P7),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    sq_argassert(5,index + 4);
    sq_argassert(6,index + 5);
    sq_argassert(7,index + 6);
    RT ret = (callee.*func)(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3),
      Get(TypeWrapper<P5>(),v,index + 4),
      Get(TypeWrapper<P6>(),v,index + 5),
      Get(TypeWrapper<P7>(),v,index + 6)
    );
    Push(v,ret);
    return 1;
  }

#ifdef SQPLUS_CONST_OPT
#define SQPLUS_CALL_CONST_MFUNC_RET0
#include "SqPlusConst.h"
#endif
};

// === No return value variants ===

template<>
struct ReturnSpecialization<void> {

  // === Standard function calls ===

  static SQInteger Call(void (*func)(),HSQUIRRELVM v,SQInteger /*index*/) {
		(void)v;
		func();
		return 0;
	}

	template<typename P1>
	static SQInteger Call(void (*func)(P1),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
		func(
			Get(TypeWrapper<P1>(),v,index + 0)
		);
		return 0;
	}

	template<typename P1,typename P2>
	static SQInteger Call(void (*func)(P1,P2),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
		func(
			Get(TypeWrapper<P1>(),v,index + 0),
			Get(TypeWrapper<P2>(),v,index + 1)
		);
		return 0;
	}

  template<typename P1,typename P2,typename P3>
  static SQInteger Call(void (*func)(P1,P2,P3),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    func(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2)
    );
    return 0;
  }

  template<typename P1,typename P2,typename P3,typename P4>
  static SQInteger Call(void (*func)(P1,P2,P3,P4),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    func(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3)
    );
    return 0;
  }

  template<typename P1,typename P2,typename P3,typename P4,typename P5>
  static SQInteger Call(void (*func)(P1,P2,P3,P4,P5),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    sq_argassert(5,index + 4);
    func(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3),
      Get(TypeWrapper<P5>(),v,index + 4)
    );
    return 0;
  }

  template<typename P1,typename P2,typename P3,typename P4,typename P5,typename P6>
  static SQInteger Call(void (*func)(P1,P2,P3,P4,P5,P6),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    sq_argassert(5,index + 4);
    sq_argassert(6,index + 5);
    func(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3),
      Get(TypeWrapper<P5>(),v,index + 4),
      Get(TypeWrapper<P6>(),v,index + 5)
    );
    return 0;
  }

  template<typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7>
  static SQInteger Call(void (*func)(P1,P2,P3,P4,P5,P6,P7),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    sq_argassert(5,index + 4);
    sq_argassert(6,index + 5);
    sq_argassert(7,index + 6);
    func(
      Get(TypeWrapper<P1>(),v,index + 0),
      Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3),
      Get(TypeWrapper<P5>(),v,index + 4),
      Get(TypeWrapper<P6>(),v,index + 5),
      Get(TypeWrapper<P7>(),v,index + 6)
    );
    return 0;
  }

  // === Member function calls ===

	template<typename Callee>
	static SQInteger Call(Callee & callee,void (Callee::*func)(),HSQUIRRELVM,SQInteger /*index*/) {
		(callee.*func)();
		return 0;
	}

	template<typename Callee,typename P1>
	static SQInteger Call(Callee & callee,void (Callee::*func)(P1),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
		(callee.*func)(
			Get(TypeWrapper<P1>(),v,index + 0)
		);
		return 0;
	}

	template<typename Callee,typename P1,typename P2>
	static SQInteger Call(Callee & callee,void (Callee::*func)(P1,P2),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
		(callee.*func)(
			Get(TypeWrapper<P1>(),v,index + 0),
			Get(TypeWrapper<P2>(),v,index + 1)
		);
		return 0;
	}

	template<typename Callee,typename P1,typename P2,typename P3>
	static SQInteger Call(Callee & callee,void (Callee::*func)(P1,P2,P3),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
		(callee.*func)(
			Get(TypeWrapper<P1>(),v,index + 0),
			Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2)
		);
		return 0;
	}

	template<typename Callee,typename P1,typename P2,typename P3,typename P4>
	static SQInteger Call(Callee & callee,void (Callee::*func)(P1,P2,P3,P4),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
		(callee.*func)(
			Get(TypeWrapper<P1>(),v,index + 0),
			Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3)
		);
		return 0;
	}

	template<typename Callee,typename P1,typename P2,typename P3,typename P4,typename P5>
	static SQInteger Call(Callee & callee,void (Callee::*func)(P1,P2,P3,P4,P5),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    sq_argassert(5,index + 4);
		(callee.*func)(
			Get(TypeWrapper<P1>(),v,index + 0),
			Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3),
      Get(TypeWrapper<P5>(),v,index + 4)
		);
		return 0;
	}

	template<typename Callee,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6>
	static SQInteger Call(Callee & callee,void (Callee::*func)(P1,P2,P3,P4,P5,P6),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    sq_argassert(5,index + 4);
    sq_argassert(6,index + 5);
		(callee.*func)(
			Get(TypeWrapper<P1>(),v,index + 0),
			Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3),
      Get(TypeWrapper<P5>(),v,index + 4),
      Get(TypeWrapper<P6>(),v,index + 5)
		);
		return 0;
	}

	template<typename Callee,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7>
	static SQInteger Call(Callee & callee,void (Callee::*func)(P1,P2,P3,P4,P5,P6,P7),HSQUIRRELVM v,SQInteger index) {
    sq_argassert(1,index + 0);
    sq_argassert(2,index + 1);
    sq_argassert(3,index + 2);
    sq_argassert(4,index + 3);
    sq_argassert(5,index + 4);
    sq_argassert(6,index + 5);
    sq_argassert(7,index + 6);
		(callee.*func)(
			Get(TypeWrapper<P1>(),v,index + 0),
			Get(TypeWrapper<P2>(),v,index + 1),
      Get(TypeWrapper<P3>(),v,index + 2),
      Get(TypeWrapper<P4>(),v,index + 3),
      Get(TypeWrapper<P5>(),v,index + 4),
      Get(TypeWrapper<P6>(),v,index + 5),
      Get(TypeWrapper<P7>(),v,index + 6)
		);
		return 0;
	}

#ifdef SQPLUS_CONST_OPT
#define SQPLUS_CALL_CONST_MFUNC_NORET
#include "SqPlusConst.h"
#endif

};

// === STANDARD Function return value specialized call handlers ===

template<typename RT>
SQInteger Call(RT (*func)(),HSQUIRRELVM v,SQInteger index) {
  return ReturnSpecialization<RT>::Call(func,v,index);
}

template<typename RT,typename P1>
SQInteger Call(RT (*func)(P1),HSQUIRRELVM v,SQInteger index) {
  return ReturnSpecialization<RT>::Call(func,v,index);
}

template<typename RT,typename P1,typename P2>
SQInteger Call(RT (*func)(P1,P2),HSQUIRRELVM v,SQInteger index) {
  return ReturnSpecialization<RT>::Call(func,v,index);
}

template<typename RT,typename P1,typename P2,typename P3>
SQInteger Call(RT (*func)(P1,P2,P3),HSQUIRRELVM v,SQInteger index) {
  return ReturnSpecialization<RT>::Call(func,v,index);
}

template<typename RT,typename P1,typename P2,typename P3,typename P4>
SQInteger Call(RT (*func)(P1,P2,P3,P4),HSQUIRRELVM v,SQInteger index) {
  return ReturnSpecialization<RT>::Call(func,v,index);
}

template<typename RT,typename P1,typename P2,typename P3,typename P4,typename P5>
SQInteger Call(RT (*func)(P1,P2,P3,P4,P5),HSQUIRRELVM v,SQInteger index) {
  return ReturnSpecialization<RT>::Call(func,v,index);
}

template<typename RT,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6>
SQInteger Call(RT (*func)(P1,P2,P3,P4,P5,P6),HSQUIRRELVM v,SQInteger index) {
  return ReturnSpecialization<RT>::Call(func,v,index);
}

template<typename RT,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7>
SQInteger Call(RT (*func)(P1,P2,P3,P4,P5,P6,P7),HSQUIRRELVM v,SQInteger index) {
  return ReturnSpecialization<RT>::Call(func,v,index);
}

// === MEMBER Function return value specialized call handlers ===

template<typename Callee,typename RT>
SQInteger Call(Callee & callee, RT (Callee::*func)(),HSQUIRRELVM v,SQInteger index) {
  return ReturnSpecialization<RT>::Call(callee,func,v,index);
}

template<typename Callee,typename RT,typename P1>
SQInteger Call(Callee & callee,RT (Callee::*func)(P1),HSQUIRRELVM v,SQInteger index) {
  return ReturnSpecialization<RT>::Call(callee,func,v,index);
}

template<typename Callee,typename RT,typename P1,typename P2>
SQInteger Call(Callee & callee,RT (Callee::*func)(P1,P2),HSQUIRRELVM v,SQInteger index) {
  return ReturnSpecialization<RT>::Call(callee,func,v,index);
}

template<typename Callee,typename RT,typename P1,typename P2,typename P3>
SQInteger Call(Callee & callee,RT (Callee::*func)(P1,P2,P3),HSQUIRRELVM v,SQInteger index) {
  return ReturnSpecialization<RT>::Call(callee,func,v,index);
}

template<typename Callee,typename RT,typename P1,typename P2,typename P3,typename P4>
SQInteger Call(Callee & callee,RT (Callee::*func)(P1,P2,P3,P4),HSQUIRRELVM v,SQInteger index) {
  return ReturnSpecialization<RT>::Call(callee,func,v,index);
}

template<typename Callee,typename RT,typename P1,typename P2,typename P3,typename P4,typename P5>
SQInteger Call(Callee & callee,RT (Callee::*func)(P1,P2,P3,P4,P5),HSQUIRRELVM v,SQInteger index) {
  return ReturnSpecialization<RT>::Call(callee,func,v,index);
}

template<typename Callee,typename RT,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6>
SQInteger Call(Callee & callee,RT (Callee::*func)(P1,P2,P3,P4,P5,P6),HSQUIRRELVM v,SQInteger index) {
  return ReturnSpecialization<RT>::Call(callee,func,v,index);
}

template<typename Callee,typename RT,typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7>
SQInteger Call(Callee & callee,RT (Callee::*func)(P1,P2,P3,P4,P5,P6,P7),HSQUIRRELVM v,SQInteger index) {
  return ReturnSpecialization<RT>::Call(callee,func,v,index);
}

#ifdef SQPLUS_CONST_OPT
#define SQPLUS_CALL_CONST_MFUNC_RET1
#include "SqPlusConst.h"
#endif

// === Direct Call Standard Function handler ===

template<typename Func>
struct DirectCallFunction {
  static inline SQInteger Dispatch(HSQUIRRELVM v) {
    StackHandler sa(v);
    SQInteger paramCount = sa.GetParamCount();
    Func * func = (Func *)sa.GetUserData(paramCount);
    return Call(*func,v,2);
  } // Dispatch
};

// === Direct Call Member Function handler ===

template<typename Callee,typename Func>
class DirectCallMemberFunction {
public:
  static inline SQInteger Dispatch(HSQUIRRELVM v) {
    StackHandler sa(v);
    SQInteger paramCount = sa.GetParamCount();
    unsigned char * ud = (unsigned char *)sa.GetUserData(paramCount);
    // C::B patch: Handle invalid instance type here
    if (!*(Callee**)ud) {
      return sq_throwerror(v,sqT("Invalid Instance Type"));
    } // if
    return Call(**(Callee**)ud,*(Func*)(ud + sizeof(Callee*)),v,2);
  } // Dispatch
};

// === Direct Call Instance Member Function handler ===

#define SQ_CLASS_OBJECT_TABLE_NAME sqT("__ot")
#define SQ_CLASS_HIER_ARRAY sqT("__ca")

template<typename Callee,typename Func>
class DirectCallInstanceMemberFunction {
public:
  static inline SQInteger Dispatch(HSQUIRRELVM v) {
    StackHandler sa(v);
    Callee * instance = (Callee *)sa.GetInstanceUp(1,0);
    SQInteger paramCount = sa.GetParamCount();
    Func * func = (Func *)sa.GetUserData(paramCount);
    // C::B patch: Let the compiler search (comment out the whole block)
//#ifdef SQ_USE_CLASS_INHERITANCE
//    SquirrelObject so(sa.GetObjectHandle(1)); // 'this'
//    SQUserPointer typetag; so.GetTypeTag(&typetag);
//    SQUserPointer calleeType = ClassType<Callee>::type();
//    if (typetag != calleeType) {
//      SquirrelObject typeTable = so.GetValue(SQ_CLASS_OBJECT_TABLE_NAME);
//      instance = (Callee *)typeTable.GetUserPointer(INT_T((size_t)ClassType<Callee>::type())); // <TODO> 64-bit compatible version.
//      if (!instance) {
//        return sq_throwerror(v,sqT("Invalid Instance Type"));
//      } // if
//    } // if
//#endif
    // C::B patch: If not instance -> return Ok anyways
    if (!instance)
      return SQ_OK;
    return Call(*instance,*func,v,2);
  } // Dispatch
};

// === Direct Call Instance Member Function Variable Argument handler ===

template<typename Callee>
class DirectCallInstanceMemberFunctionVarArgs {
public:
  typedef SQInteger (Callee::*FuncType)(HSQUIRRELVM);
  static inline SQInteger Dispatch(HSQUIRRELVM v) {
    StackHandler sa(v);
    Callee * instance = (Callee *)sa.GetInstanceUp(1,0);
    SQInteger paramCount = sa.GetParamCount();
    FuncType func = *(FuncType *)sa.GetUserData(paramCount);
    // C::B patch: Let the compiler search (comment out the whole block)
//#ifdef SQ_USE_CLASS_INHERITANCE
//    SquirrelObject so(sa.GetObjectHandle(1)); // 'this'
//    SQUserPointer typetag; so.GetTypeTag(&typetag);
//    SQUserPointer calleeType = ClassType<Callee>::type();
//    if (typetag != calleeType) {
//      SquirrelObject typeTable = so.GetValue(SQ_CLASS_OBJECT_TABLE_NAME);
//      instance = (Callee *)typeTable.GetUserPointer(INT_T((size_t)ClassType<Callee>::type())); // <TODO> 64-bit compatible version.
//      if (!instance) {
//        return sq_throwerror(v,sqT("Invalid Instance Type"));
//      } // if
//    } // if
//#endif
    sq_poptop(v); // Remove UserData from stack: so sa.GetParamCount() returns actual param count.
    // C::B patch: If not instance -> return Ok anyways
    if (!instance)
      return SQ_OK;
    return (instance->*func)(v);
  } // Dispatch
};

// Code fragment useful for debugging new implementations.
#if 0
HSQOBJECT ho = sa.GetObjectHandle(paramCount);
SquirrelObject so(ho);
SQObjectType sot = so.GetType();
#endif

// === Standard function call ===

template<typename Func>
inline void sq_pushdirectclosure(HSQUIRRELVM v,Func func,SQUnsignedInteger nupvalues) {
  SQUserPointer up = sq_newuserdata(v,sizeof(func)); // Also pushed on stack.
  memcpy(up,&func,sizeof(func));
  sq_newclosure(v,DirectCallFunction<Func>::Dispatch,nupvalues+1);
} // sq_pushdirectclosure

// === Fixed Class pointer call (always calls with object pointer that was registered) ===

template<typename Callee,typename Func>
inline void sq_pushdirectclosure(HSQUIRRELVM v,const Callee & callee,Func func,SQUnsignedInteger nupvalues) {
  unsigned char * up = (unsigned char *)sq_newuserdata(v,sizeof(Callee*)+sizeof(func));  // Also pushed on stack.
  const SQUserPointer pCallee = (SQUserPointer)&callee;
  memcpy(up,&pCallee,sizeof(Callee*));
  memcpy(up + sizeof(Callee*),&func,sizeof(func));
  sq_newclosure(v,DirectCallMemberFunction<Callee,Func>::Dispatch,nupvalues+1);
} // sq_pushdirectclosure

// === Class Instance call: class pointer retrieved from script class instance ===

template<typename Callee,typename Func>
inline void sq_pushdirectinstanceclosure(HSQUIRRELVM v,const Callee & /*callee*/,Func func,SQUnsignedInteger nupvalues) {
  unsigned char * up = (unsigned char *)sq_newuserdata(v,sizeof(func));  // Also pushed on stack.
  memcpy(up,&func,sizeof(func));
  sq_newclosure(v,DirectCallInstanceMemberFunction<Callee,Func>::Dispatch,nupvalues+1);
} // sq_pushdirectinstanceclosure

// === Class Instance call: class pointer retrieved from script class instance (variable arguments) ===

template<typename Callee>
inline void sq_pushdirectinstanceclosurevarargs(HSQUIRRELVM v,const Callee & callee,SQInteger (Callee::*func)(HSQUIRRELVM),SQUnsignedInteger nupvalues) {
  unsigned char * up = (unsigned char *)sq_newuserdata(v,sizeof(func)); // Also pushed on stack.
  memcpy(up,&func,sizeof(func));
  sq_newclosure(v,DirectCallInstanceMemberFunctionVarArgs<Callee>::Dispatch,nupvalues+1);
} // sq_pushdirectinstanceclosurevarargs

// === Register a STANDARD function (table or class on stack) ===

template<typename Func>
inline void Register(HSQUIRRELVM v,Func func,const SQChar * name) {
  sq_pushstring(v,name,-1);
  sq_pushdirectclosure(v,func,0);
  sq_createslot(v,-3); // Stack is restored after this call (same state as before Register() call).
} // Register

// === Register a MEMBER function (table or class on stack) ===

template<typename Callee,typename Func>
inline void Register(HSQUIRRELVM v,Callee & callee,Func func,const SQChar * name) {
  sq_pushstring(v,name,-1);
  sq_pushdirectclosure(v,callee,func,0);
  sq_createslot(v,-3); // Stack is restored after this call (same state as before Register() call).
} // Register

// === Register a STANDARD global function (root table) ===

template<typename Func>
inline void RegisterGlobal(HSQUIRRELVM v,Func func,const SQChar * name) {
  sq_pushroottable(v);
  Register(v,func,name);
  sq_poptop(v); // Remove root table.
} // RegisterGlobal

template<typename Func>
inline void RegisterGlobal(Func func,const SQChar * name) {
  RegisterGlobal(SquirrelVM::GetVMPtr(),func,name);
} // RegisterGlobal

// === Register a MEMBER global function (root table) ===

template<typename Callee,typename Func>
inline void RegisterGlobal(HSQUIRRELVM v,Callee & callee,Func func,const SQChar * name) {
  sq_pushroottable(v);
  Register(v,callee,func,name);
  sq_poptop(v); // Remove root table.
} // RegisterGlobal

template<typename Callee,typename Func>
inline void RegisterGlobal(Callee & callee,Func func,const SQChar * name) {
  RegisterGlobal(SquirrelVM::GetVMPtr(),callee,func,name);
} // RegisterGlobal

// === Register a STANDARD function (hso is table or class) ===

template<typename Func>
inline void Register(HSQUIRRELVM v,HSQOBJECT hso,Func func,const SQChar * name) {
  sq_pushobject(v,hso);
  Register(v,func,name);
  sq_poptop(v); // Remove hso.
} // Register

// === Register a MEMBER function (hso is table or class) ===
// === Fixed Class pointer call (always calls with object pointer that was registered) ===

template<typename Callee,typename Func>
inline void Register(HSQUIRRELVM v,HSQOBJECT hso,Callee & callee,Func func,const SQChar * name) {
  sq_pushobject(v,hso);
  Register(v,callee,func,name);
  sq_poptop(v); // Remove hso.
} // Register

// === Register an INSTANCE MEMBER function ===
// === Class Instance call: class pointer retrieved from script class instance ===

template<typename Callee,typename Func>
inline void RegisterInstance(HSQUIRRELVM v,HSQOBJECT hclass,Callee & callee,Func func,const SQChar * name) {
  sq_pushobject(v,hclass);
  sq_pushstring(v,name,-1);
  sq_pushdirectinstanceclosure(v,callee,func,0);
  sq_createslot(v,-3);
  sq_poptop(v); // Remove hclass.
} // RegisterInstance


#ifdef _MSC_VER
#pragma warning(disable : 4995) // Deprecated _snprintf
#endif

// === Register an INSTANCE MEMBER function Variable Arguments ===
// typeMask: "*" means don't check parameters, typeMask=0 means function takes no arguments (and is type checked for that case).
// All the other Squirrel type-masks are passed normally.

template<typename Callee>
inline void RegisterInstanceVarArgs(HSQUIRRELVM v,HSQOBJECT hclass,Callee & callee,SQInteger (Callee::*func)(HSQUIRRELVM),const SQChar * name,const SQChar * typeMask=sqT("*")) {
  sq_pushobject(v,hclass);
  sq_pushstring(v,name,-1);
  sq_pushdirectinstanceclosurevarargs(v,callee,func,0);
  SQChar tm[64];
  SQChar * ptm = tm;
  SQInteger numParams = SQ_MATCHTYPEMASKSTRING;
  if (typeMask) {
    if (typeMask[0] == '*') {
      ptm       = 0; // Variable args: don't check parameters.
//      numParams = 0; // Clear SQ_MATCHTYPEMASKSTRING (does not mean match 0 params. See sq_setparamscheck()).
    } else {
      if (SCSNPRINTF(tm,sizeof(tm),sqT("x%s"),typeMask) < 0) { // Must be an instance.
        throw SquirrelError(sqT("RegisterInstanceVarArgs: typeMask string too long."));
      } // if
    } // if
  } else { // <TODO> Need to check object type on stack: table, class, instance, etc.
//    _snprintf(tm,sizeof(tm),"x"); // instance.
    tm[0] = 'x';
    tm[1] = 0;
  } // if
  if (ptm) { // If ptm == 0, don't check type.
    sq_setparamscheck(v,numParams,ptm); // Determine arg count from type string.
  } // if
#ifdef _DEBUG
  sq_setnativeclosurename(v,-1,name); // For debugging only.
#endif
  sq_createslot(v,-3);
  sq_poptop(v); // Remove hclass.
} // RegisterInstanceVarArgs

#ifdef _MSC_VER
#pragma warning(default : 4995)
#endif

// === Call Squirrel Functions from C/C++ ===
// No type checking is performed for Squirrel functions as Squirrel types are dynamic:
// Incoming types are passed unchanged to Squirrel functions. The parameter count is checked: an exception is thrown if mismatched.
// Return values must match the RT template argument type, else an exception can be thrown on return.

template<typename RT>
struct SquirrelFunction {
  HSQUIRRELVM v;
  SquirrelObject object; // Table or class.
  SquirrelObject func;
  SquirrelFunction() : v(0) {}
  SquirrelFunction(HSQUIRRELVM _v,const SquirrelObject & _object,const SquirrelObject & _func) : v(_v), object(_object), func(_func) {}
  SquirrelFunction(const SquirrelObject & _object,const SquirrelObject & _func) : v(SquirrelVM::GetVMPtr()), object(_object), func(_func) {}
  SquirrelFunction(const SquirrelObject & _object,const SQChar * name) {
    v      = SquirrelVM::GetVMPtr();
    object = _object;
    func   = object.GetValue(name);
  }
  SquirrelFunction(const SQChar * name) {
    v      = SquirrelVM::GetVMPtr();
    object = SquirrelVM::GetRootTable();
    func   = object.GetValue(name);
  }

  // Release references and reset internal objects to null.
  void reset(void) {
    func.Reset();
    object.Reset();
  } // Reset

#define SQPLUS_CHECK_FNCALL(res) if (!SQ_SUCCEEDED(res)) throw SquirrelError(sqT("SquirrelFunction<> call failed"))

  RT operator()(void) {
    sq_pushobject(v,func.GetObjectHandle());
    sq_pushobject(v,object.GetObjectHandle());
    SQPLUS_CHECK_FNCALL(sq_call(v,1,SQTrue,SQ_CALL_RAISE_ERROR));
    return GetRet(TypeWrapper<RT>(),v,-1);
  }

  template<typename P1>
  RT operator()(P1 p1) {
    sq_pushobject(v,func.GetObjectHandle());
    sq_pushobject(v,object.GetObjectHandle());
    Push(v,p1);
    SQPLUS_CHECK_FNCALL(sq_call(v,2,SQTrue,SQ_CALL_RAISE_ERROR));
    return GetRet(TypeWrapper<RT>(),v,-1);
  }

  template<typename P1,typename P2>
  RT operator()(P1 p1,P2 p2) {
    sq_pushobject(v,func.GetObjectHandle());
    sq_pushobject(v,object.GetObjectHandle());
    Push(v,p1);
    Push(v,p2);
    SQPLUS_CHECK_FNCALL(sq_call(v,3,SQTrue,SQ_CALL_RAISE_ERROR));
    return GetRet(TypeWrapper<RT>(),v,-1);
  }

  template<typename P1,typename P2,typename P3>
  RT operator()(P1 p1,P2 p2,P3 p3) {
    sq_pushobject(v,func.GetObjectHandle());
    sq_pushobject(v,object.GetObjectHandle());
    Push(v,p1);
    Push(v,p2);
    Push(v,p3);
    SQPLUS_CHECK_FNCALL(sq_call(v,4,SQTrue,SQ_CALL_RAISE_ERROR));
    return GetRet(TypeWrapper<RT>(),v,-1);
  }

  template<typename P1,typename P2,typename P3,typename P4>
  RT operator()(P1 p1,P2 p2,P3 p3,P4 p4) {
    sq_pushobject(v,func.GetObjectHandle());
    sq_pushobject(v,object.GetObjectHandle());
    Push(v,p1);
    Push(v,p2);
    Push(v,p3);
    Push(v,p4);
    SQPLUS_CHECK_FNCALL(sq_call(v,5,SQTrue,SQ_CALL_RAISE_ERROR));
    return GetRet(TypeWrapper<RT>(),v,-1);
  }

  template<typename P1,typename P2,typename P3,typename P4,typename P5>
  RT operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5) {
    sq_pushobject(v,func.GetObjectHandle());
    sq_pushobject(v,object.GetObjectHandle());
    Push(v,p1);
    Push(v,p2);
    Push(v,p3);
    Push(v,p4);
    Push(v,p5);
    SQPLUS_CHECK_FNCALL(sq_call(v,6,SQTrue,SQ_CALL_RAISE_ERROR));
    return GetRet(TypeWrapper<RT>(),v,-1);
  }

  template<typename P1,typename P2,typename P3,typename P4,typename P5,typename P6>
  RT operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6) {
    sq_pushobject(v,func.GetObjectHandle());
    sq_pushobject(v,object.GetObjectHandle());
    Push(v,p1);
    Push(v,p2);
    Push(v,p3);
    Push(v,p4);
    Push(v,p5);
    Push(v,p6);
    SQPLUS_CHECK_FNCALL(sq_call(v,7,SQTrue,SQ_CALL_RAISE_ERROR));
    return GetRet(TypeWrapper<RT>(),v,-1);
  }

  template<typename P1,typename P2,typename P3,typename P4,typename P5,typename P6,typename P7>
  RT operator()(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6,P7 p7) {
    sq_pushobject(v,func.GetObjectHandle());
    sq_pushobject(v,object.GetObjectHandle());
    Push(v,p1);
    Push(v,p2);
    Push(v,p3);
    Push(v,p4);
    Push(v,p5);
    Push(v,p6);
    Push(v,p7);
    SQPLUS_CHECK_FNCALL(sq_call(v,8,SQTrue,SQ_CALL_RAISE_ERROR));
    return GetRet(TypeWrapper<RT>(),v,-1);
  }

};

// === Class/Struct registration ===

#define SQ_DELETE_CLASS(CLASSTYPE) if (up) { CLASSTYPE * self = (CLASSTYPE *)up; delete self;} return 0
#define SQ_DECLARE_RELEASE(CLASSTYPE) \
  static SQInteger release(SQUserPointer up, SQInteger /*size*/) { \
    SQ_DELETE_CLASS(CLASSTYPE); \
  }

template<typename T>
struct ReleaseClassPtrPtr {
  static SQInteger release(SQUserPointer up,SQInteger size) {
    if (up) {
      T ** self = (T **)up;
      delete *self;
    } // if
    return 0;
  } // release
};

template<typename T>
struct ReleaseClassPtr {
  static SQInteger release(SQUserPointer up,SQInteger size) {
    if (up) {
      T * self = (T *)up;
      delete self;
    } // if
    return 0;
  } // release
};

BOOL_T CreateClass(HSQUIRRELVM v,SquirrelObject & newClass,SQUserPointer classType,const SQChar * name,const SQChar * baseName=0);

#define SQ_ANCESTOR_CLASS_INDEX sqT("__ci")

// Call PostConstruct() at the end of custom constructors.
template<typename T>
inline SQInteger PostConstruct(HSQUIRRELVM v,T * newClass,SQRELEASEHOOK hook) {
#ifdef SQ_USE_CLASS_INHERITANCE
  StackHandler sa(v);
  HSQOBJECT ho = sa.GetObjectHandle(1); // OT_INSTANCE
  SquirrelObject instance(ho);
  INT_T classIndex = instance.GetValue(SQ_ANCESTOR_CLASS_INDEX).ToInteger();
  if (classIndex == -1) { // Is this the most-derived C/C++ class? If so, create all ancestors (if present).

    SquirrelObject newObjectTable = SquirrelVM::CreateTable();                 // 11/2/05: Create a new table for this instance.
    newObjectTable.SetUserPointer(INT_T((size_t)ClassType<T>::type()),newClass); // <TODO> 64-bit compatible version.
    instance.SetValue(SQ_CLASS_OBJECT_TABLE_NAME,newObjectTable);

    SquirrelObject classHierArray = instance.GetValue(SQ_CLASS_HIER_ARRAY);
    INT_T count = classHierArray.Len();
    if (count > 1) { // This will be true when more than one C/C++ class is in the hierarchy.
      --count; // Skip the most-derived class.
      for (INT_T i=0; i < count; i++) {
#ifdef CPP_STYLE_INHERITANCE // Kamaitati's changes for C++ inheritance support. jcs 5/28/06
        SquirrelObject so = classHierArray.GetValue(i);
        sq_pushobject(v,so.GetObjectHandle());
        SQUserPointer typeTag;
        sq_gettypetag(v,-1,&typeTag);
        newObjectTable.SetUserPointer(INT_T(size_t(typeTag)),newClass);
        sq_poptop(v);
#else
        instance.SetValue(SQ_ANCESTOR_CLASS_INDEX,i); // Store ancestor class index for recursive constructor calls to come.
        INT_T top = sq_gettop(v);
        SquirrelObject so = classHierArray.GetValue(i); // Need to create UserData struct: store pointer to class, set release hook.
        SquirrelObject func = so.GetValue(sqT("constructor"));
        sq_pushobject(v,func.GetObjectHandle());
        sq_pushobject(v,instance.GetObjectHandle()); // The 'instance' is the real Squirrel 'this' for all ancestors (as opposed to an instance created from the defining class, which does not happen).
        sq_call(v,1,SQFalse,SQ_CALL_RAISE_ERROR); // Call constructor: no arguments are passed other than the 'instance'.
        sq_settop(v,top);
#endif
      } // for
      instance.SetValue(SQ_ANCESTOR_CLASS_INDEX,SquirrelObject()); // Store an OT_NULL object to free SQ_ANCESTOR_CLASS_INDEX var.
    } // if
  } else { // Ancestor: Construct class and set release hook.

    SquirrelObject objectTable = instance.GetValue(SQ_CLASS_OBJECT_TABLE_NAME); // 11/2/05: Get the existing object table.
    objectTable.SetUserPointer(INT_T((size_t)ClassType<T>::type()),newClass);     // <TODO> 64-bit compatible version.

    INT_T top = sq_gettop(v);
    T ** ud = (T **)sq_newuserdata(v,sizeof(T *)); // Create UserData and push onto stack.
    *ud = newClass;
    // C::B patch: Disable releasing of objects (due to private/protected dtors) (Note: This is evil, but no other possibility found.)
//    sq_setreleasehook(v,-1,ReleaseClassPtrPtr<T>::release); // Set release hook for UserData on stack.
    SquirrelObject userData;
    userData.AttachToStackObject(-1);
    SquirrelObject classHierArray = instance.GetValue(SQ_CLASS_HIER_ARRAY);
    classHierArray.SetValue(classIndex,userData); // Replace the class entry with UserData: will be freed during most-derived class destruction.
    sq_settop(v,top);
    return TRUE;
  } // if
#endif
  sq_setinstanceup(v,1,newClass);
  sq_setreleasehook(v,1,hook);
  return 1;
} // PostConstruct

template<typename T>
struct ConstructReleaseClass {
  static SQInteger construct(HSQUIRRELVM v) {
    return PostConstruct<T>(v,new T(),release);
  } // construct
  // C::B patch: Add empty constructor
  static SQInteger no_construct(HSQUIRRELVM v) {
    return PostConstruct<T>(v,0,0);
  } // no_construct
  SQ_DECLARE_RELEASE(T)
};

template<typename T>
inline SquirrelObject RegisterClassType(HSQUIRRELVM v,const SQChar * scriptClassName,const SQChar * baseScriptClassName=0) {
  SQInteger top = sq_gettop(v);
  SquirrelObject newClass;
  if (CreateClass(v,newClass,(SQUserPointer)ClassType<T>::type(),scriptClassName,baseScriptClassName)) {
    SquirrelVM::CreateFunction(newClass,&ConstructReleaseClass<T>::no_construct,sqT("constructor"));
#ifdef SQ_USE_CLASS_INHERITANCE
    // <NOTE> New member vars cannot be added to instances (OT_INSTANCE): additions must occur on the defining class (OT_CLASS), before any instances are instantiated.
    if (!newClass.Exists(SQ_CLASS_OBJECT_TABLE_NAME)) { // Will always get table from most-derived registered class.
      SquirrelObject objectTable = SquirrelVM::CreateTable();
      newClass.SetValue(SQ_CLASS_OBJECT_TABLE_NAME,objectTable); // Constructors must add their 'this' pointer indexed by type to this table. See PostConstruct() above.
                                                                 // 11/2/05: This table will behave as a static global for each instance unless overwritten during construction (see PostConstruct() above).
    } // if
    SquirrelObject classHierArray;
    if (!newClass.Exists(SQ_CLASS_HIER_ARRAY)) {   // Will always get table from most-derived registered class.
      classHierArray = SquirrelVM::CreateArray(0); // The only constructor called will be the most-derived class: this array contains all classes in the hierarchy to be constructed.
      newClass.SetValue(SQ_CLASS_HIER_ARRAY,classHierArray);
    } else {
      classHierArray = newClass.GetValue(SQ_CLASS_HIER_ARRAY);
    } // if
    classHierArray.ArrayAppend(newClass);          // Add the class to the hierarchy array. The array values will be released and replaced with UserData to free created ancestor classes.
    // C::B patch for 64bit builds
    newClass.SetValue(SQ_ANCESTOR_CLASS_INDEX,(SQInteger)-1); // When the class hierarchy is created, this var will be used to help in recursively creating ancestor classes.
#endif
  } // if
  sq_settop(v,top);
  return newClass;
} // RegisterClassType

// === Define and register a C++ class and its members for use with Squirrel ===
// Constructors+destructors are automatically created. Custom constructors must use the
// standard SQFUNCTION signature if variable argument types are required (overloads).
// See testSqPlus2.cpp for examples.

template<typename TClassType>
struct SQClassDef {
  HSQUIRRELVM v;
  const SQChar * name;
  const SQChar * base;
  SquirrelObject newClass;

#ifdef SQ_USE_CLASS_INHERITANCE
  // Optional base arg is the name of a base class to inherit from (must already be defined in the Squirrel VM).
  SQClassDef(HSQUIRRELVM _v,const SQChar * _name,const SQChar * _base=0) : v(_v), name(_name), base(_base) {
    newClass = RegisterClassType<TClassType>(v,name,base);
  }
  // Optional base arg is the name of a base class to inherit from (must already be defined in the Squirrel VM).
  SQClassDef(const SQChar * _name,const SQChar * _base=0) : name(_name), base(_base) {
    v = SquirrelVM::GetVMPtr();
    newClass = RegisterClassType<TClassType>(v,name,base);
  }
#else
  SQClassDef(HSQUIRRELVM _v,const SQChar * _name) : v(_v), name(_name) {
    newClass = RegisterClassType<TClassType>(v,name);
  }

  SQClassDef(const SQChar * _name) : name(_name) {
    v = SquirrelVM::GetVMPtr();
    newClass = RegisterClassType<TClassType>(v,name);
  }
#endif

  // C::B patch: Add empty constructor
  SQClassDef & emptyCtor() {
    SquirrelVM::CreateFunction(newClass,&ConstructReleaseClass<TClassType>::construct,sqT("constructor"));
    return *this;
  } // emptyCtor

  // Register a member function.
  template<typename Func>
  SQClassDef & func(Func pfunc,const SQChar * name_) {
    RegisterInstance(v,newClass.GetObjectHandle(),*(TClassType *)0,pfunc,name_);
    return *this;
  } // func

  // Register a variable-argument member function (supports variable+multiple return values).
  // typeMask: "*" means don't check parameters, typeMask=0 means function takes no arguments (and is type checked for that case).
  // All the other Squirrel type-masks are passed normally.
  template<typename Func>
  SQClassDef & funcVarArgs(Func pfunc,const SQChar * name_,const SQChar * typeMask=sqT("*")) {
    RegisterInstanceVarArgs(v,newClass.GetObjectHandle(),*(TClassType *)0,pfunc,name_,typeMask);
    return *this;
  } // funcVarArgs

  // === BEGIN static-member+global function registration ===

  // === This version is for static member functions only, such as custom constructors where 'this' is not yet valid ===
  // typeMask: "*" means don't check parameters, typeMask=0 means function takes no arguments (and is type checked for that case).
  // All the other Squirrel type-masks are passed normally.

  template<typename Func>
  SQClassDef & staticFuncVarArgs(Func pfunc,const SQChar * name_,const SQChar * typeMask=sqT("*")) {
    SquirrelVM::PushObject(newClass);
    SquirrelVM::CreateFunction(pfunc,name_,typeMask);
    SquirrelVM::Pop(1);
    return *this;
  } // staticFuncVarArgs

  // Register a standard global function (effectively embedding a global function in TClassType's script namespace: does not need or use a 'this' pointer).
  template<typename Func>
  SQClassDef & staticFunc(Func pfunc,const SQChar * name_) {
    Register(v,newClass.GetObjectHandle(),pfunc,name_);
    return *this;
  } // staticFunc

  // Register a function to a pre-allocated class/struct member function: will use callee's 'this' (effectively embedding a global function in TClassType's script namespace).
  template<typename Callee,typename Func>
  SQClassDef & staticFunc(Callee & callee,Func pfunc,const SQChar * name_) {
    Register(v,newClass.GetObjectHandle(),callee,pfunc,name_);
    return *this;
  } // staticFunc

  // === END static+global function registration ===

  // Register a member variable.
  template<typename VarType>
  SQClassDef & var(VarType TClassType::* pvar,const SQChar * name_,VarAccessType access=VAR_ACCESS_READ_WRITE) {
    struct CV {
      VarType TClassType::* var;
    } cv; // Cast Variable helper.
    cv.var = pvar;
    RegisterInstanceVariable(newClass,ClassType<TClassType>::type(),*(VarType **)&cv,name_,access);
    return *this;
  } // var

  // Register a member variable as a UserPointer (read only).
  template<typename VarType>
  SQClassDef & varAsUserPointer(VarType TClassType::* pvar,const SQChar * name_) {
    struct CV {
      VarType TClassType::* var;
    } cv; // Cast Variable helper.
    cv.var = pvar;
    RegisterInstanceVariable(newClass,ClassType<TClassType>::type(),*(SQAnything **)&cv,name_,VAR_ACCESS_READ_ONLY);
    return *this;
  } // varAsUserPointer

  template<typename VarType>
  SQClassDef & staticVar(VarType * pvar,const SQChar * name_,VarAccessType access=VAR_ACCESS_READ_WRITE) {
    struct CV {
      VarType * var;
    } cv; // Cast Variable helper.
    cv.var = pvar;
    RegisterInstanceVariable(newClass,ClassType<TClassType>::type(),*(VarType **)&cv,name_,VarAccessType(access|VAR_ACCESS_STATIC));
    return *this;
  } // staticVar

#ifdef SQPLUS_CONST_OPT
#define SQ_REG_CONST_STATIC_VAR
#include "SqPlusConst.h"
#endif

  // Register a constant (read-only in script, passed by value (only INT_T, FLOAT_T, or BOOL_T types)).
  template<typename ConstantType>
  SQClassDef & constant(ConstantType constant_,const SQChar * name_) {
    RegisterInstanceConstant(newClass,ClassType<TClassType>::type(),constant_,name_);
    return *this;
  } // constant

  // Register an enum as an integer (read-only in script).
  SQClassDef & enumInt(SQInteger constant_,const SQChar * name_) {
      RegisterInstanceConstant(newClass,ClassType<TClassType>::type(),constant_,name_);
      return *this;
  } // enumInt

};

// === BEGIN Function Call Handlers ===

inline void Push(HSQUIRRELVM v,char value)               { sq_pushinteger(v,value); }
inline void Push(HSQUIRRELVM v,unsigned char value)      { sq_pushinteger(v,value); }
inline void Push(HSQUIRRELVM v,short value)              { sq_pushinteger(v,value); }
inline void Push(HSQUIRRELVM v,unsigned short value)     { sq_pushinteger(v,value); }
inline void Push(HSQUIRRELVM v,int value)                { sq_pushinteger(v,value); }
inline void Push(HSQUIRRELVM v,unsigned int value)       { sq_pushinteger(v,value); }
inline void Push(HSQUIRRELVM v,long value)               { sq_pushinteger(v,value); }
inline void Push(HSQUIRRELVM v,long long value)          { sq_pushinteger(v,value); }
inline void Push(HSQUIRRELVM v,unsigned long value)      { sq_pushinteger(v,value); }
inline void Push(HSQUIRRELVM v,unsigned long long value) { sq_pushinteger(v,value); }
inline void Push(HSQUIRRELVM v,double value)             { sq_pushfloat(v,(FLOAT_T)value); }
inline void Push(HSQUIRRELVM v,float value)              { sq_pushfloat(v,(FLOAT_T)value); }
inline void Push(HSQUIRRELVM v,const SQChar * value)     { sq_pushstring(v,value,-1); }
inline void Push(HSQUIRRELVM v,const SquirrelNull &)     { sq_pushnull(v); }
inline void Push(HSQUIRRELVM v,SQFUNCTION value)         { sq_pushuserpointer(v,(void*)value); }
inline void Push(HSQUIRRELVM v,SQAnythingPtr value)      { sq_pushuserpointer(v,(void*)value); } // Cast to SQAnythingPtr instead of void * if USE_ARGUMENT_DEPENDANT_OVERLOADS can't be used by your compiler.
inline void Push(HSQUIRRELVM v,SquirrelObject & so)      { sq_pushobject(v,so.GetObjectHandle()); }


#define USE_ARGUMENT_DEPENDANT_OVERLOADS
#ifdef USE_ARGUMENT_DEPENDANT_OVERLOADS
#ifdef _MSC_VER
#pragma warning (disable:4675) // Disable warning: "resolved overload was found by argument-dependent lookup" when class/struct pointers are used as function arguments.
#endif
// === BEGIN Argument Dependent Overloads ===
inline void Push(HSQUIRRELVM v,bool value)                  { sq_pushbool(v,value); }               // Pass bool as SQInteger if USE_ARGUMENT_DEPENDANT_OVERLOADS can't be used by your compiler.
inline void Push(HSQUIRRELVM v,const void * value)          { sq_pushuserpointer(v,(void*)value); } // Pass SQAnythingPtr instead of void * "                                             "
inline void Push(HSQUIRRELVM v,const SQUserPointer & value) { sq_pushuserpointer(v,(void*)value); }
// === END Argument Dependent Overloads ===
#endif

#define SQPLUS_CHECK_GET(res) if (!SQ_SUCCEEDED(res)) throw SquirrelError(sqT("sq_get*() failed (type error)"))

inline bool	Match(TypeWrapper<bool>,HSQUIRRELVM v,SQInteger idx)               { return sq_gettype(v,idx) == OT_BOOL; }
inline bool	Match(TypeWrapper<char>,HSQUIRRELVM v,SQInteger idx)               { return sq_gettype(v,idx) == OT_INTEGER; }
inline bool	Match(TypeWrapper<unsigned char>,HSQUIRRELVM v, SQInteger idx)     { return sq_gettype(v,idx) == OT_INTEGER; }
inline bool	Match(TypeWrapper<short>,HSQUIRRELVM v,SQInteger idx)              { return sq_gettype(v,idx) == OT_INTEGER; }
inline bool	Match(TypeWrapper<unsigned short>,HSQUIRRELVM v,SQInteger idx)     { return sq_gettype(v,idx) == OT_INTEGER; }
inline bool	Match(TypeWrapper<int>,HSQUIRRELVM v,SQInteger idx)                { return sq_gettype(v,idx) == OT_INTEGER; }
inline bool	Match(TypeWrapper<unsigned int>,HSQUIRRELVM v,SQInteger idx)       { return sq_gettype(v,idx) == OT_INTEGER; }
inline bool	Match(TypeWrapper<long>,HSQUIRRELVM v,SQInteger idx)               { return sq_gettype(v,idx) == OT_INTEGER; }
inline bool Match(TypeWrapper<long long>,HSQUIRRELVM v,SQInteger idx)          { return sq_gettype(v,idx) == OT_INTEGER; }
inline bool	Match(TypeWrapper<unsigned long>,HSQUIRRELVM v,SQInteger idx)      { return sq_gettype(v,idx) == OT_INTEGER; }
inline bool Match(TypeWrapper<unsigned long long>,HSQUIRRELVM v,SQInteger idx) { return sq_gettype(v,idx) == OT_INTEGER; }
inline bool	Match(TypeWrapper<float>,HSQUIRRELVM v,SQInteger idx)              { SQInteger type = sq_gettype(v,idx); return type == OT_FLOAT; }
inline bool	Match(TypeWrapper<double>,HSQUIRRELVM v,SQInteger idx)             { SQInteger type = sq_gettype(v,idx); return type == OT_FLOAT; }
inline bool	Match(TypeWrapper<const SQChar *>,HSQUIRRELVM v,SQInteger idx)     { return sq_gettype(v,idx) == OT_STRING; }
inline bool	Match(TypeWrapper<HSQUIRRELVM>,HSQUIRRELVM /*v*/,SQInteger /*idx*/){ return true; } // See Get() for HSQUIRRELVM below (v is always present).
inline bool	Match(TypeWrapper<void*>,HSQUIRRELVM v,SQInteger idx)              { return sq_gettype(v,idx) == OT_USERPOINTER; }
inline bool	Match(TypeWrapper<SquirrelObject>,HSQUIRRELVM /*v*/,SQInteger /*idx*/) { return true; } // See sq_getstackobj(): always returns true.

inline void               Get(TypeWrapper<void>,HSQUIRRELVM /*v*/,int)                      { ; }
inline bool               Get(TypeWrapper<bool>,HSQUIRRELVM v,SQInteger idx)                { SQBool b; SQPLUS_CHECK_GET(sq_getbool(v,idx,&b)); return b != 0; }
inline char               Get(TypeWrapper<char>,HSQUIRRELVM v,SQInteger idx)                { INT_T i; SQPLUS_CHECK_GET(sq_getinteger(v,idx,&i)); return static_cast<char>(i); }
inline unsigned char      Get(TypeWrapper<unsigned char>,HSQUIRRELVM v,SQInteger idx)       { INT_T i; SQPLUS_CHECK_GET(sq_getinteger(v,idx,&i)); return static_cast<unsigned char>(i); }
inline short              Get(TypeWrapper<short>,HSQUIRRELVM v,SQInteger idx)               { INT_T i; SQPLUS_CHECK_GET(sq_getinteger(v,idx,&i)); return static_cast<short>(i); }
inline unsigned short     Get(TypeWrapper<unsigned short>,HSQUIRRELVM v,SQInteger idx)      { INT_T i; SQPLUS_CHECK_GET(sq_getinteger(v,idx,&i)); return static_cast<unsigned short>(i); }
inline int                Get(TypeWrapper<int>,HSQUIRRELVM v,SQInteger idx)                 { INT_T i; SQPLUS_CHECK_GET(sq_getinteger(v,idx,&i)); return i; }
inline unsigned int       Get(TypeWrapper<unsigned int>,HSQUIRRELVM v,SQInteger idx)        { INT_T i; SQPLUS_CHECK_GET(sq_getinteger(v,idx,&i)); return static_cast<unsigned int>(i); }
inline long               Get(TypeWrapper<long>,HSQUIRRELVM v,SQInteger idx)                { INT_T i; SQPLUS_CHECK_GET(sq_getinteger(v,idx,&i)); return static_cast<long>(i); }
inline long long          Get(TypeWrapper<long long>,HSQUIRRELVM v,SQInteger idx)           { INT_T i; SQPLUS_CHECK_GET(sq_getinteger(v,idx,&i)); return static_cast<long long>(i); }
inline unsigned long      Get(TypeWrapper<unsigned long>,HSQUIRRELVM v,SQInteger idx)       { INT_T i; SQPLUS_CHECK_GET(sq_getinteger(v,idx,&i)); return static_cast<unsigned long>(i); }
inline unsigned long long Get(TypeWrapper<unsigned long long>, HSQUIRRELVM v,SQInteger idx) { INT_T i; SQPLUS_CHECK_GET(sq_getinteger(v,idx,&i)); return static_cast<unsigned long long>(i); }
inline float              Get(TypeWrapper<float>,HSQUIRRELVM v,SQInteger idx)               { FLOAT_T f; SQPLUS_CHECK_GET(sq_getfloat(v,idx,&f)); return f; }
inline double             Get(TypeWrapper<double>,HSQUIRRELVM v,SQInteger idx)              { FLOAT_T f; SQPLUS_CHECK_GET(sq_getfloat(v,idx,&f)); return static_cast<double>(f); }
inline const SQChar *     Get(TypeWrapper<const SQChar *>,HSQUIRRELVM v,SQInteger idx)      { const SQChar * s; SQPLUS_CHECK_GET(sq_getstring(v,idx,&s)); return s; }
inline SquirrelNull       Get(TypeWrapper<SquirrelNull>,HSQUIRRELVM v,SQInteger idx)        { (void)v, (void)idx; return SquirrelNull();  }
inline void *             Get(TypeWrapper<void *>,HSQUIRRELVM v,SQInteger idx)              { SQUserPointer p; SQPLUS_CHECK_GET(sq_getuserpointer(v,idx,&p)); return p; }
inline HSQUIRRELVM        Get(TypeWrapper<HSQUIRRELVM>,HSQUIRRELVM v,SQInteger /*idx*/)     { sq_poptop(v); return v; } // sq_poptop(v): remove UserData from stack so GetParamCount() matches normal behavior.
inline SquirrelObject     Get(TypeWrapper<SquirrelObject>,HSQUIRRELVM v,SQInteger idx)      { HSQOBJECT o; SQPLUS_CHECK_GET(sq_getstackobj(v,idx,&o)); return SquirrelObject(o); }

#ifdef SQPLUS_SUPPORT_STD_STRING
inline void Push(HSQUIRRELVM v,const std::string& value) { sq_pushstring(v,value.c_str(),-1); }
inline bool Match(TypeWrapper<const std::string&>, HSQUIRRELVM v, SQInteger idx) { return sq_gettype(v,idx) == OT_STRING; }
inline std::string Get(TypeWrapper<const std::string&>,HSQUIRRELVM v,SQInteger idx) { const SQChar * s; SQPLUS_CHECK_GET(sq_getstring(v,idx,&s)); return std::string(s); }
#endif

// Added jflanglois suggestion, 8/20/06. jcs
#ifdef SQPLUS_SUPPORT_SQ_STD_STRING
typedef std::basic_string<SQChar> sq_std_string;
inline void Push(HSQUIRRELVM v,const sq_std_string & value) { sq_pushstring(v,value.c_str(),-1); }
inline bool Match(TypeWrapper<const sq_std_string &>, HSQUIRRELVM v, SQInteger idx) { return sq_gettype(v,idx) == OT_STRING; }
inline sq_std_string Get(TypeWrapper<const sq_std_string &>,HSQUIRRELVM v,SQInteger idx) { const SQChar * s; SQPLUS_CHECK_GET(sq_getstring(v,idx,&s)); return sq_std_string(s); }
#endif

// GetRet() restores the stack for SquirrelFunction<>() calls.
template<typename RT>
inline RT GetRet(TypeWrapper<RT>,HSQUIRRELVM v,SQInteger idx) { RT ret = Get(TypeWrapper<RT>(),v,idx); sq_pop(v,2); return ret; } // sq_pop(v,2): restore stack after function call.

// Specialization to support void return type.
inline void GetRet(TypeWrapper<void>,HSQUIRRELVM v,SQInteger /*idx*/) { sq_pop(v,2); }

// === END Function Call Handlers ===

// === Example SQClassDef usage (see testSqPlus2.cpp): ===

#if 0
  SQClassDef<NewTestObj> sqClass(sqT("NewTestObj");
  sqClass.func(NewTestObj::newtestR1,sqT("newtestR1"));
  sqClass.var(&NewTestObj::val,sqT("val"));
  sqClass.var(&NewTestObj::s1,sqT("s1"));
  sqClass.var(&NewTestObj::s2,sqT("s2"));
  sqClass.funcVarArgs(&NewTestObj::multiArgs,sqT("multiArgs"));

// Shorthand form:

  SQClassDef<NewTestObj>(sqT("NewTestObj").
    func(NewTestObj::newtestR1,sqT("newtestR1")).
    var(&NewTestObj::val,sqT("val")).
    var(&NewTestObj::s1,sqT("s1")).
    var(&NewTestObj::s2,sqT("s2")).
    funcVarArgs(NewTestObj::multiArgs,sqT("multiArgs"));
#endif

// === Macros for old style registration. SQClassDef registration is now easier to use (SQ_DECLARE_CLASS() is not needed) ===

#define SQ_DECLARE_CLASS(CLASSNAME)                                  \
static SQInteger _##CLASSNAME##_release(SQUserPointer up,SQInteger size) { \
  if (up) {                                                          \
    CLASSNAME * self = (CLASSNAME *)up;                              \
    delete self;                                                     \
  }                                                                  \
  return 0;                                                          \
}                                                                    \
static SQInteger _##CLASSNAME##_constructor(HSQUIRRELVM v) {               \
  CLASSNAME * pc = new CLASSNAME();                                  \
  sq_setinstanceup(v,1,pc);                                          \
  sq_setreleasehook(v,1,_##CLASSNAME##_release);                     \
  return 1;                                                          \
}

#define SQ_REGISTER_CLASS(CLASSNAME)                                 \
  RegisterClassType(SquirrelVM::GetVMPtr(),sqT(#CLASSNAME),_##CLASSNAME##_constructor)

#define SQ_REGISTER_INSTANCE(NEWSQCLASS,CCLASS,FUNCNAME)             \
  RegisterInstance(SquirrelVM::GetVMPtr(),NEWSQCLASS.GetObjectHandle(),*(CCLASS *)0,&CCLASS::FUNCNAME,sqT(#FUNCNAME));

#define SQ_REGISTER_INSTANCE_VARARGS(NEWSQCLASS,CCLASS,FUNCNAME)     \
  RegisterInstanceVarArgs(SquirrelVM::GetVMPtr(),NEWSQCLASS.GetObjectHandle(),*(CCLASS *)0,&CCLASS::FUNCNAME,sqT(#FUNCNAME));

#define SQ_REGISTER_INSTANCE_VARIABLE(NEWSQCLASS,CCLASS,VARNAME)     \
  RegisterInstanceVariable(NEWSQCLASS,&((CCLASS *)0)->VARNAME,sqT(#VARNAME));

#if defined(USE_ARGUMENT_DEPENDANT_OVERLOADS) && defined(_MSC_VER)
#pragma warning (default:4675)
#endif

}; // namespace SqPlus

#endif //_SQ_PLUS_H_
