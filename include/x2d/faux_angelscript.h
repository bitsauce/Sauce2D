#ifndef X2D_AS_H
#define X2D_AS_H

#include <stddef.h>
#ifndef _MSC_VER
#include <stdint.h>
#endif

typedef void asIScriptEngine;
typedef void asIScriptContext;
typedef void asIScriptModule;
typedef void asIObjectType;
typedef void asIScriptGeneric;
typedef void asIScriptFunction;

// asBYTE  =  8 bits
// asWORD  = 16 bits
// asDWORD = 32 bits
// asQWORD = 64 bits
// asPWORD = size of pointer
//
typedef unsigned char  asBYTE;
typedef unsigned short asWORD;
typedef unsigned int   asUINT;
#if (defined(_MSC_VER) && _MSC_VER <= 1200) || defined(__S3E__)
	// size_t is not really correct, since it only guaranteed to be large enough to hold the segment size.
	// For example, on 16bit systems the size_t may be 16bits only even if pointers are 32bit. But nobody
	// is likely to use MSVC6 to compile for 16bit systems anymore, so this should be ok.
	typedef size_t	       asPWORD;
#else
	typedef uintptr_t      asPWORD;
#endif
#ifdef __LP64__
    typedef unsigned int  asDWORD;
    typedef unsigned long asQWORD;
    typedef long asINT64;
#else
    typedef unsigned long asDWORD;
  #if defined(__GNUC__) || defined(__MWERKS__) || defined(__SUNPRO_CC)
    typedef uint64_t asQWORD;
    typedef int64_t asINT64;
  #else
    typedef unsigned __int64 asQWORD;
    typedef __int64 asINT64;
  #endif
#endif

// Is the target a 64bit system?
#if defined(__LP64__) || defined(__amd64__) || defined(__x86_64__) || defined(_M_X64)
	#ifndef AS_64BIT_PTR
		#define AS_64BIT_PTR
	#endif
#endif

typedef void (*asFUNCTION_t)();
typedef void (*asGENFUNC_t)(asIScriptGeneric *);
typedef void *(*asALLOCFUNC_t)(size_t);
typedef void (*asFREEFUNC_t)(void *);
typedef void (*asCLEANENGINEFUNC_t)(asIScriptEngine *);
typedef void (*asCLEANMODULEFUNC_t)(asIScriptModule *);
typedef void (*asCLEANCONTEXTFUNC_t)(asIScriptContext *);
typedef void (*asCLEANFUNCTIONFUNC_t)(asIScriptFunction *);
typedef void (*asCLEANOBJECTTYPEFUNC_t)(asIObjectType *);
typedef asIScriptContext *(*asREQUESTCONTEXTFUNC_t)(asIScriptEngine *, void *);
typedef void (*asRETURNCONTEXTFUNC_t)(asIScriptEngine *, asIScriptContext *, void *);

// This macro does basically the same thing as offsetof defined in stddef.h, but
// GNUC should not complain about the usage as I'm not using 0 as the base pointer.
#define asOFFSET(s,m) ((size_t)(&reinterpret_cast<s*>(100000)->m)-100000)

#define asFUNCTION(f) asFunctionPtr(f)
#if (defined(_MSC_VER) && _MSC_VER <= 1200) || (defined(__BORLANDC__) && __BORLANDC__ < 0x590)
// MSVC 6 has a bug that prevents it from properly compiling using the correct asFUNCTIONPR with operator >
// so we need to use ordinary C style cast instead of static_cast. The drawback is that the compiler can't
// check that the cast is really valid.
// BCC v5.8 (C++Builder 2006) and earlier have a similar bug which forces us to fall back to a C-style cast.
#define asFUNCTIONPR(f,p,r) asFunctionPtr((void (*)())((r (*)p)(f)))
#else
#define asFUNCTIONPR(f,p,r) asFunctionPtr((void (*)())(static_cast<r (*)p>(f)))
#endif

#ifndef AS_NO_CLASS_METHODS

class asCUnknownClass;
typedef void (asCUnknownClass::*asMETHOD_t)();

struct asSFuncPtr
{
	asSFuncPtr(asBYTE f = 0)
	{
		for( size_t n = 0; n < sizeof(ptr.dummy); n++ )
			ptr.dummy[n] = 0;
		flag = f;
	}

	void CopyMethodPtr(const void *mthdPtr, size_t size)
	{
		for( size_t n = 0; n < size; n++ )
			ptr.dummy[n] = reinterpret_cast<const char *>(mthdPtr)[n];
	}

	union
	{
		// The largest known method point is 20 bytes (MSVC 64bit),
		// but with 8byte alignment this becomes 24 bytes. So we need
		// to be able to store at least that much.
		char dummy[25]; 
		struct {asMETHOD_t   mthd; char dummy[25-sizeof(asMETHOD_t)];} m;
		struct {asFUNCTION_t func; char dummy[25-sizeof(asFUNCTION_t)];} f;
	} ptr;
	asBYTE flag; // 1 = generic, 2 = global func, 3 = method
};

#if defined(__BORLANDC__)
// A bug in BCC (QC #85374) makes it impossible to distinguish const/non-const method overloads
// with static_cast<>. The workaround is to use an _implicit_cast instead.

 #if  __BORLANDC__ < 0x590
 // BCC v5.8 (C++Builder 2006) and earlier have an even more annoying bug which causes
 // the "pretty" workaround below (with _implicit_cast<>) to fail. For these compilers
 // we need to use a traditional C-style cast.
  #define AS_METHOD_AMBIGUITY_CAST(t) (t)
 #else
template <typename T>
  T _implicit_cast (T val)
{ return val; }
  #define AS_METHOD_AMBIGUITY_CAST(t) AS_NAMESPACE_QUALIFIER _implicit_cast<t >
 #endif
#else
 #define AS_METHOD_AMBIGUITY_CAST(t) static_cast<t >
#endif

#define asMETHOD(c,m) asSMethodPtr<sizeof(void (c::*)())>::Convert((void (c::*)())(&c::m))
#define asMETHODPR(c,m,p,r) asSMethodPtr<sizeof(void (c::*)())>::Convert(AS_METHOD_AMBIGUITY_CAST(r (c::*)p)(&c::m))

#else // Class methods are disabled

struct asSFuncPtr
{
	asSFuncPtr(asBYTE f)
	{
		for( int n = 0; n < sizeof(ptr.dummy); n++ )
			ptr.dummy[n] = 0;
		flag = f;
	}

	union
	{
		char dummy[25]; // largest known class method pointer
		struct {asFUNCTION_t func; char dummy[25-sizeof(asFUNCTION_t)];} f;
	} ptr;
	asBYTE flag; // 1 = generic, 2 = global func
};

#endif

//-----------------------------------------------------------------
// Function pointers

// Template function to capture all global functions,
// except the ones using the generic calling convention
template <class T>
inline asSFuncPtr asFunctionPtr(T func)
{
	// Mark this as a global function
	asSFuncPtr p(2);

#ifdef AS_64BIT_PTR
	// The size_t cast is to avoid a compiler warning with asFUNCTION(0) 
	// on 64bit, as 0 is interpreted as a 32bit int value
	p.ptr.f.func = reinterpret_cast<asFUNCTION_t>(size_t(func));
#else
	// MSVC6 doesn't like the size_t cast above so I
	// solved this with a separate code for 32bit.
	p.ptr.f.func = reinterpret_cast<asFUNCTION_t>(func);
#endif

	return p;
}

// Specialization for functions using the generic calling convention
template<>
inline asSFuncPtr asFunctionPtr<asGENFUNC_t>(asGENFUNC_t func)
{
	// Mark this as a generic function
	asSFuncPtr p(1);
	p.ptr.f.func = reinterpret_cast<asFUNCTION_t>(func);
	return p;
}

#ifndef AS_NO_CLASS_METHODS

// Method pointers

// Declare a dummy class so that we can determine the size of a simple method pointer
class asCSimpleDummy {};
typedef void (asCSimpleDummy::*asSIMPLEMETHOD_t)();
const int SINGLE_PTR_SIZE = sizeof(asSIMPLEMETHOD_t);

// Define template
template <int N>
struct asSMethodPtr
{
	template<class M>
	static asSFuncPtr Convert(M Mthd)
	{
		// This version of the function should never be executed, nor compiled,
		// as it would mean that the size of the method pointer cannot be determined.

		int ERROR_UnsupportedMethodPtr[N-100];

		asSFuncPtr p(0);
		return p;
	}
};

// Template specialization
template <>
struct asSMethodPtr<SINGLE_PTR_SIZE>
{
	template<class M>
	static asSFuncPtr Convert(M Mthd)
	{
		// Mark this as a class method
		asSFuncPtr p(3);
		p.CopyMethodPtr(&Mthd, SINGLE_PTR_SIZE);
		return p;
	}
};

#if defined(_MSC_VER) && !defined(__MWERKS__)

// MSVC and Intel uses different sizes for different class method pointers
template <>
struct asSMethodPtr<SINGLE_PTR_SIZE+1*sizeof(int)>
{
	template <class M>
	static asSFuncPtr Convert(M Mthd)
	{
		// Mark this as a class method
		asSFuncPtr p(3);
		p.CopyMethodPtr(&Mthd, SINGLE_PTR_SIZE+sizeof(int));
		return p;
	}
};

template <>
struct asSMethodPtr<SINGLE_PTR_SIZE+2*sizeof(int)>
{
	template <class M>
	static asSFuncPtr Convert(M Mthd)
	{
		// On 32bit platforms with is where a class with virtual inheritance falls.
		// On 64bit platforms we can also fall here if 8byte data alignments is used.

		// Mark this as a class method
		asSFuncPtr p(3);
		p.CopyMethodPtr(&Mthd, SINGLE_PTR_SIZE+2*sizeof(int));

		// Microsoft has a terrible optimization on class methods with virtual inheritance.
		// They are hardcoding an important offset, which is not coming in the method pointer.

#if defined(_MSC_VER) && !defined(AS_64BIT_PTR)
			// Method pointers for virtual inheritance is not supported,
			// as it requires the location of the vbase table, which is 
			// only available to the C++ compiler, but not in the method
			// pointer. 

			// You can get around this by forward declaring the class and
			// storing the sizeof its method pointer in a constant. Example:

			// class ClassWithVirtualInheritance;
			// const int ClassWithVirtualInheritance_workaround = sizeof(void ClassWithVirtualInheritance::*());

			// This will force the compiler to use the unknown type
			// for the class, which falls under the next case


			// Copy the virtual table index to the 4th dword so that AngelScript
			// can properly detect and deny the use of methods with virtual inheritance.
			*(reinterpret_cast<asDWORD*>(&p)+3) = *(reinterpret_cast<asDWORD*>(&p)+2);
#endif

		return p;
	}
};

template <>
struct asSMethodPtr<SINGLE_PTR_SIZE+3*sizeof(int)>
{
	template <class M>
	static asSFuncPtr Convert(M Mthd)
	{
		// Mark this as a class method
		asSFuncPtr p(3);
		p.CopyMethodPtr(&Mthd, SINGLE_PTR_SIZE+3*sizeof(int));
		return p;
	}
};

template <>
struct asSMethodPtr<SINGLE_PTR_SIZE+4*sizeof(int)>
{
	template <class M>
	static asSFuncPtr Convert(M Mthd)
	{
		// On 64bit platforms with 8byte data alignment
		// the unknown class method pointers will come here.

		// Mark this as a class method
		asSFuncPtr p(3);
		p.CopyMethodPtr(&Mthd, SINGLE_PTR_SIZE+4*sizeof(int));
		return p;
	}
};

#endif

#endif // AS_NO_CLASS_METHODS

#endif // X2D_AS_H