/*
 * > [2023/07/15 OpenSource]
 * >
 * > Project: Estd Header v4
 * > Copyright: Lioncky(Personal)
 * > Project	: https://github.com/Lioncky/estd
 * > Detail : Build in China, begin this at 07/12, latest update 08/30.
 * More information about sprintf series api
 * https://learn.microsoft.com/zh-cn/cpp/c-runtime-library/format-specification-syntax-printf-and-wprintf-functions?view=msvc-170
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/
#pragma once
#ifndef _ESTD_H_
#	define _ESTD_H_
#	include <corecrt_malloc.h>
#	include <stdarg.h> // va_list
#	include <string.h> // strstr
#	include <stdlib.h> // atoll
#	include <limits.h>// int_max
#	include <stdio.h>// vsprintf

#ifndef esdef
#	define esame 0
#	define esdef  size_t(0)
#	define enull ((char*)"")
#	define espty estr(esdef,enull)
#	define esdo(x) extern void x(); x();
#	define esames(x,y) (strcmp(x,y)==0)
#	define ealloc (char*)::malloc  // mallocer
#	define efree(x) ::free((void*)x) // droper
#	define emss(x)  # x // macro 2(to) string
#	define enss(x) emss(x) // enum 2(to) string
#	define einfunc __FUNCTION__ "->" enss(__LINE__) ":"
#	define eoutf(func) __declspec(deprecated("Using " # func " instead..."))
typedef unsigned __int64 uiint;
typedef unsigned char byte;
typedef const char* ccstr;
typedef size_t szr,uintr;
typedef unsigned uint;
typedef intptr_t intr;
typedef __int64 iint;
typedef char* cstr;
typedef bool br;

# if _DEBUG && _WIN32
#	define dbk(...) __debugbreak();
#	define dbi(b,...) if(b) __debugbreak();
# else
#	define dbk(...)
#	define dbi
# endif
#endif

#ifndef __UWSTR__
#	define __UWSTR__ // Unique (Wide) String 
inline constexpr unsigned u_FNV_CA = 0x975563U;// 0x1000193;//  16777619U;
inline constexpr unsigned u_FNV_OS = 0x84664995U;// 0x811C9DC5; //  2166136261U;
constexpr unsigned _uu(const char* str) { unsigned u = *str; u |= *++str << 0x08; u |= *++str << 0x10; u |= *++str << 0x18; return u; }
constexpr unsigned _uw(const wchar_t* str) { auto v = u_FNV_CA; while (*str) v ^= u_FNV_OS * unsigned(*str++); return v ^ (v >> 16); }
constexpr unsigned _us(const char* str) { auto v = u_FNV_CA; while (*str) v ^= u_FNV_OS * unsigned(*str++); return v ^ (v >> 16); }
constexpr unsigned operator"" _uw(const wchar_t* str, size_t) { return _uw((wchar_t*)str); }
constexpr unsigned operator"" _us(const char* str, size_t) { return _us(str); }
constexpr unsigned operator"" _uu(const char* str, size_t) { return _uu(str); }
constexpr size_t operator"" _L(const wchar_t*, size_t wSize) { return wSize; }
constexpr size_t operator"" _l(const char*, size_t nSize) { return nSize; }
constexpr unsigned _fmtArgc(const char* var) {
	unsigned res = 0; while (*var) {
		if (*var++ == '%') {
			if (*var++ != '%')
				res++;
		}
	}
	return res;
}
constexpr unsigned _uxchg(const unsigned _u) { // 0x12345678 >> 0x78563412 LE->BigEndian
	return ((_u >> 24)) | ((_u << 24)) | ((_u & 0xFF00) << 8) | ((_u >> 8) & 0xFF00);
}
#define _uint(x) (*(unsigned*)(x)) // _us == uhash _uint == _uu
#endif

//
// 'ad' = 2 'abcd' = 4
// 
_Success_(return >= 0)
_Check_return_opt_ _CRT_STDIO_INLINE
constexpr int _istr_len(const int div) {
	if (!div) return  0;
	else if (div < 0x100) return  1;
	else if (div & (0xFF << 24)) return 4;
	else if (div & (0xFF << 16)) return 3;
	else return 2; // if (div & (0xFF << 8)) 
}

//
// #needfree
// 'utf16'->utf8 
// 
#define wcs2us _wcs2us
#define us2wcs _us2wcs
_Success_(return != 0)
_Check_return_opt_ _CRT_STDIO_INLINE
wchar_t* _us2wcs(
	_In_z_ const char* ustr,
	_Out_ size_t * opt = 0) {
	unsigned u = 0; wchar_t* bak, *dst;
	bak = dst = (wchar_t*)ealloc((strlen(ustr)+1) << 1);
	if (!dst) return nullptr;

	while (u = *ustr++) {
		if ((u & 0x80) == 0x00);
		else if ((u & 0xF0) == 0xE0) { // 3
			u &= 0x0F; u <<= 6;
			u |= (*ustr++ & 0x3F); u <<= 6;
			u |= (*ustr++ & 0x3F);
		}
		else if ((u & 0xE0) == 0xC0) { // 2
			u &= 0x0F; u <<= 6;
			u |= (*ustr++ & 0x3F);
		}
		else if ((u & 0xF8) == 0xF0) { // 4
			u &= 0x0F; u <<= 6;
			u |= (*ustr++ & 0x3F); u <<= 6;
			u |= (*ustr++ & 0x3F); u <<= 6;
			u |= (*ustr++ & 0x3F);
		}
		*dst++ = (wchar_t)u;
	} *dst = L'\0';	
	if (opt) *opt = dst - bak; *dst = '\0';
	return bak;
}

_Success_(return != 0)
_Check_return_opt_ _CRT_STDIO_INLINE
char* _wcs2us(
	_In_z_ const wchar_t* wbuf, 
	_Out_ size_t * opt = 0) {
	unsigned lowSurrogate, u=0;
	char* dst = ealloc((wcslen(wbuf) << 1) | 2), * bak = dst;
	if(!dst) return nullptr;
	while (u = *wbuf++) {

		if (u < 0x80) *dst++ = u;
		else if (u < 0x800) {
			*dst++ = 0xC0 | ((u & 0x7C0) >> 6);
			*dst++ = 0x80 | (u & 0x3F);
		}
		else if (u < 0xD800) {
		_UTF_3:
			*dst++ = 0xE0 | ((u & 0xF000) >> 12);
			*dst++ = 0x80 | ((u & 0xFC0) >> 6);
			*dst++ = 0x80 | ((u & 0x3F));
		}
		else if (u <= 0xDBFF) {  // HighSurrogate 
			lowSurrogate = (*wbuf++ & 0xFFFF);
			if (lowSurrogate >= 0xDC00 && lowSurrogate <= 0xDFFF) {  // LowSurrogate
				u = (((u - 0xD800) << 10) | (lowSurrogate - 0xDC00)) | 0x10000;
				*dst++ = 0xF0 | ((u & 0x1C0000) >> 18);
				*dst++ = 0x80 | ((u & 0x3F000) >> 12);
				*dst++ = 0x80 | ((u & 0xFC0) >> 6);
				*dst++ = 0x80 | (u & 0x3F);
			} else dbk ("ERROR NO Surrogate");
		}
		else goto _UTF_3;
	}
	if (opt) *opt = dst - bak;   *dst = '\0';
	return bak;
}

_Success_(return >= 0)
_Check_return_opt_ _CRT_STDIO_INLINE
int __CRTDECL vscprintf(
	_In_z_ _Printf_format_string_	char const* const _Format,
	va_list			_ArgList) {
	const int _Result = __stdio_common_vsprintf(
		_CRT_INTERNAL_LOCAL_PRINTF_OPTIONS | _CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR,
		0, 0, _Format, NULL, _ArgList);
	return _Result < 0 ? -1 : _Result;
}

_Success_(return >= 0)
_Check_return_opt_ _CRT_STDIO_INLINE
size_t __CRTDECL vsprintfs(
	_In_z_ _Always_(_Post_z_)		char* _Src,
	_In_										size_t _Len,
	_In_z_ _Printf_format_string_	char const* const _Format,
	va_list           _ArgList) {
	const size_t _Result = __stdio_common_vsprintf(
		_CRT_INTERNAL_LOCAL_PRINTF_OPTIONS | _CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR,
		_Src, _Len, _Format, NULL, _ArgList);
	return _Result < 0 ? -1 : _Result;
}

_Success_(return >= 0)
_Check_return_opt_ _CRT_STDIO_INLINE
int __CRTDECL scprintf(
	_In_z_ _Printf_format_string_	char const* const _Format,
	...) {
	va_list _ArgList; va_start(_ArgList, _Format);
	const int _Result = __stdio_common_vsprintf(
		_CRT_INTERNAL_LOCAL_PRINTF_OPTIONS | _CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR,
		0, 0, _Format, NULL, _ArgList);va_end(_ArgList);
	return _Result < 0 ? -1 : _Result;
}

template <size_t N>
_Success_(return >= 0)
_Check_return_opt_ _CRT_STDIO_INLINE
size_t __CRTDECL sprintfs(
	_In_z_ _Always_(_Post_z_)	 char(&_Src)[N],
	_In_z_ _Printf_format_string_	char const* const _Format, 
	...) {
	va_list _ArgList; va_start(_ArgList, _Format);
	const size_t _Result = __stdio_common_vsprintf(
		_CRT_INTERNAL_LOCAL_PRINTF_OPTIONS | _CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR,
		_Src, N, _Format, NULL, _ArgList);	va_end(_ArgList);
	return _Result < 0 ? -1 : _Result;
}

_Success_(return != nullptr)
_Check_return_opt_ _CRT_STDIO_INLINE
char* __CRTDECL esvprintf_s(
	_Out_									size_t * _Len,
	_In_z_ _Printf_format_string_	char const* const _Format,
	va_list _ArgList) {
	char* _s; size_t _u;
	_u = vscprintf(_Format, _ArgList);
	if (_u > 0) {
		if (_s = (va_list)malloc(_u <<= 1 )) {
			vsprintfs(_s, _u, _Format, _ArgList);
			if (_Len) *_Len = (_u >> 1);
			return _s;
		}
	}
	return nullptr;
}

_Success_(return != nullptr)
_Check_return_opt_ _CRT_STDIO_INLINE
char* __CRTDECL esprintf_s(
	_Out_ _Always_(_Post_z_) size_t* _Len,
	_In_z_ _Printf_format_string_	char const* const _Format,
	...) {
	va_list _ArgList;
	va_start(_ArgList, _Format);
	const auto* _Res = 
		esvprintf_s(_Len, _Format, _ArgList);
	va_end(_ArgList);
	return (char*)_Res;
}

//
// Use
// asptr for RAII
//
#ifndef esprintfree
#	define esprintfree	::free
#	define esprintfs			esprintf_s	
#	define esvprintfs			esvprintf_s

#	define esprintf(fmt,...)		esprintf_s(0,fmt,__VA_ARGS__)
#	define esvprintf(_Format,_ArgList) esvprintf_s(0,_Format,_ArgList)
#endif // asprintfree

// 
// #estr_encrypter
template <size_t N, size_t M=(N%4)?(N/4+1):(N/4)>
class ecstr {
public: 
	constexpr ecstr(const wchar_t(&str)[N]) {
		for (int i = 0, j = 0; i < M; i++) {
			m_enc[i] = str[j++]; if (j == N)break;
			m_enc[i] |= str[j++]<<8;if(j==N)break;
			m_enc[i] |= str[j++]<<16;if(j==N)break;
			m_enc[i] |= str[j++]<<24;if(j==N)break; 
	}	} // operator char*() noexcept -- Can't for constexpr
	operator char*() const noexcept { return (char*)m_enc; }
	char* operator*() const noexcept { return (char*)m_enc; }
	constexpr unsigned& operator[](_In_range_(0, M - 1) size_t _Pos) noexcept { return m_enc[_Pos]; }
	constexpr const unsigned& operator[](_In_range_(0, M - 1) size_t _Pos) const noexcept { return m_enc[_Pos]; }
private: unsigned m_enc[M]{};
};

//
// #estr_asprinter
typedef class estr {
public:
	size_t&size() { return this->esize; }
	uint u8lenth() { return this->_u8(); }
	int lenth() { return (int)this->esize; }

	char* data() { return this->esdata; }
	bool empty() { return this->esize==0; }
	const char* c_str() { return this->esdata; }
	unsigned length() { return (unsigned)this->esize; }
	char* data_end() { return this->esdata + this->esize - 1; }
	unsigned hash() { return this->esize ? _us(this->esdata) : ~0; }
	estr(size_t usz) : esize(usz) { if(esdata = ealloc(usz + sizeof(int))) *esdata='\0'; } // estr(0U) : dbg only 
	estr() : esdata(0), esize(0) {}

	estr(const char* fmt) : esdata(0) {
		this->assign(fmt, ~0);
	}
	estr(const wchar_t* fmt) : esize(0) {
		esdata = _wcs2us(fmt, &esize);
	}
	estr(va_list va, const char* fmt) : esize(0) {
		esdata = esvprintf_s(&esize, fmt, va);va_end(va);
	}
	estr(size_t ufmt, const char* fmt) : esdata(0) {
		this->assign(fmt, ufmt); // asdata need nullptr
	}

	template <size_t N>
	estr(const char(&fmt)[N], ...) : esdata(0) {
		if (strchr(fmt, '%')) {
			va_list va;  va_start(va, (char*)fmt);
			esdata = esvprintf_s(&esize, fmt, va); va_end(va);
		} else this->assign(fmt); 
		// can't use {constexpr (sizeof...(args) > 0)} with va_start
	}
	// #DEEPCOPY_FOR_RETURN
	estr(const estr& asp) : esdata(asp.esdata), esize(asp.esize) {
		*(size_t*)&asp.esdata = 0;
	}

	void assign(const char* src, size_t usr = ~0) {
		if (esdata) efree(esdata);

		this->esize = usr = (usr == ~0) ? strlen(src) : usr;
		if (this->esdata = ealloc(usr + 2)) {
			memcpy(this->esdata, src, usr);
			*(this->esdata + usr) = '\0';
		}
	}
	void append(const char* src, size_t usr = ~0) {
		if (!this->esize) {
			this->assign(src, usr);
			return;
		}
		if (usr == ~0) usr = strlen(src);
		if (char* dat = ealloc(this->esize + usr + 2)) {
			memcpy(dat, this->esdata, this->esize);
			memcpy(dat + this->esize, src, usr);
			*(dat + this->esize + usr) = '\0';

			efree(this->esdata);
			this->esize += usr;
			this->esdata = dat;
		}
	}
	void fmts(const char* fmt, ...) {
		va_list va;  va_start(va, fmt);
		char* _s = esvprintf_s(&esize, fmt, va); va_end(va);

		if (esdata) efree(esdata); this->esdata = _s;
	}
	void dump(estr esr) {
		if (this->esdata) efree(esdata);

		this->esdata = esr.esdata;
		this->esize = esr.esize;
		esr.esdata = nullptr;
	}
	void reserve(size_t usr) {
		if (esdata) efree(esdata);

		this->esdata = ealloc(usr + 2);
		this->esize = usr;
	}
	void clear() {
		if (esdata) efree(esdata);

		this->esdata = 0;
		this->esize = 0;
	}

	//
	// contain char or str
	//
	char* find(const char* buf) {
		return this->esdata ? strstr(this->esdata, buf) : nullptr;
	}
	bool contain(int _c) {
		return this->esdata ? !!strchr(this->esdata, _c) : false;
	}
	char* rfind(const char* sfind) {
		size_t ufind; char* endstr;

		if (this->esdata) {
			ufind = strlen(sfind);
			endstr = this->esdata + this->esize;
			do {
				if (0==memcmp(--endstr, sfind, ufind))
					return endstr;
			} while (endstr != this->esdata);
		}
		return nullptr;
	}
	size_t occurs(const char* _c, size_t _cz = ~0) {
		size_t uz = 0; char* _s;

		if (this->esdata) {
			if (_s = strstr(this->esdata, _c)) {
				if (_cz == ~0) _cz = strlen(_c);  
				do { uz++; _s += _cz; }
					while (_s = strstr(_s, _c));
				return uz;
			}
		}
		return uz;
	}

	//
	// extra safe api 
	// 
	int toi() {
		return this->esdata ? atoi(this->esdata) : 0;
	}
	long long tol() {
		return this->esdata ? atoll(this->esdata) : 0;
	}
	void byi(int _v) {
		char _num[0x20]{ 0 }; _itoa_s(_v, _num, sizeof _num, 10);
		this->assign(_num);
	}
	void byl(long long _v) {
		char _num[0x20]{ 0 }; _i64toa_s(_v, _num, sizeof _num, 10);
		this->assign(_num);
	}
	void byf(double _v) {
		char _num[0x20]{ 0 }; _gcvt_s(_num,_v,3);
		this->assign(_num);
	}
	void byhex(long long _v) {
		char _num[0x20]{ ' ',' ',0,0}; _i64toa_s(_v, _num + 2, sizeof _num - 2, 16); _strupr_s(_num); *(short*)_num = 'x0';
		this->assign(_num);
	}

	//
	// string replace api 
	//
	void rep(int _c, int _to) {
		if (!this->esize || !_to) return;

		if (_c &= 0xFF) {
			char* dat = this->esdata;
			while (dat = strchr(dat, _c)) {
				*dat++ = _to;
			}
		}
	}
	template <size_t N>
	void remove(const char(&_)[N]) {
		return this->_rep(_, N - 1, "", 0);
	}
	template <size_t N, size_t M>
	void replace(const char(&_)[N], const char(&_to)[M]) {
		return this->_rep(_,N-1,_to,M-1);
	}
	//
	// cvt url to utf8 string
	//
	void cvt_url() {
		char* _e, *ptr, *p; int iw = 0;
		if (this->esdata) {
			p = ptr = this->esdata;
			_e = this->esdata + this->esize;
			while (p < _e) {
				iw = *p; if (iw == '%') {
					iw = this->_hex_int(++p) << 4;
					iw |= this->_hex_int(++p);
				} p++; *(unsigned char*)ptr++ = iw;
			}*ptr = '\0'; this->esize = ptr - this->esdata;
		}
	}
	//
	// cmp left and same remove 
	//
	template <size_t _N,size_t N=_N-1>
	bool cmprm(const char(&sleft)[_N]) {
		if (this->esdata && esame == memcmp(sleft, this->esdata, N)) {
			this->esize -= N;
			memcpy(this->esdata, this->esdata + N, this->esize + 1);
			return true;
		}
		return false;
	}

	//
	// cmp take middle
	// left pos << 16 | right pos
	// check by "i & 0xFFFF" for valid two parts
	//
	template <size_t _M, size_t _N>
	unsigned cmpmid(const char(&sleft)[_M], const char(&sright)[_N], bool bContain = false) {
		constexpr size_t N = _N - 1;
		constexpr size_t M = _M - 1;
		unsigned ucode = 0; char* p;
		if (this->esdata && (p = strstr(this->esdata, sleft)) ) {
			if (!bContain) p += (_M - 1);
			ucode = unsigned((p - this->esdata)) << 16;
			if (p = strstr(p, sright)) {
				if (bContain) p += (_N - 1);
				ucode |= ((p - this->esdata) & 0xFFFF);
			} else ucode = 0;
		}
		return ucode;
	}
	estr getmid(unsigned ur) {
		unsigned ul = ur >> 16; ur &= 0xFFFF;
		return ur ? estr(ur - ul, this->data() + ul) : estr(esdef);
	}

	//
	// #endby [warning]
	// only can be just use for once
	// this->asize will plus 1
	// 
	void endby(int str) {
		if (this->esdata) {
			*(short*)(this->esdata + this->esize++) = str&0xFFFF;
		}
	}
	//
	// #endremove [warning]
	// this->asize will sub 1
	// 
	void endrm() {
		if (this->esdata) {
			*(this->esdata + --this->esize) = '\0';
		}
	}

	char* operator*() { return this->esdata; }
	unsigned operator-() { return this->hash(); }
	//
	void operator=(const char* str) { this->assign(str); }
	void operator=(const estr& str) { if (str.esdata != this->esdata) this->assign(str.esdata, str.esize); }
	//
	bool operator==(const char* str) { return esames(str, this->esdata); }
	template <size_t N> // cmp this left part
	bool operator>=(const char(&str)[N]) { return 0==memcmp(str, this->esdata, N - 1); }
	template <size_t N> // cmp obj left part
	bool operator<=(const char(&obj)[N]) { return 0==memcmp(this->esdata,obj, this->esize); }
	//
	template <size_t N>
	void operator+=(const char(&str)[N]) { this->append(str, N - 1); }
	void operator+=(const estr& asr) { this->append(asr.esdata, asr.esize); }
	template <size_t N> // add [asr = asr+"123"+"456"]
	estr& operator+(const char(&str)[N]) { this->append(str, N - 1); return *this; }
	estr& operator+(const estr& asr) { this->append(asr.esdata, asr.esize); return *this; }
	//
	template <size_t N> // xor "\x99"
	inline void operator^=(const char(&str)[N]) {for(size_t i=0;i<this->esize;i++) {for(size_t n=0;n<N-1;n++) {this->esdata[i]^=str[n];}}}

	operator int () { return (int)this->esize; }
	operator char* () { return this->esdata; }
	operator wchar_t* () { return this->as_wstr(); } // DANGEROUS
	operator unsigned char* () { return (unsigned char*)this->esdata; }

	// #WARNING MANUAL API
	// Using this for transplant other class which destruction by '::free'
	//
	char* forget() { auto* dat = this->esdata; this->esdata = nullptr; return dat; }

	~estr() { 
		if (esdata) 
			efree(esdata); 
	}
private:
	char* esdata;
	size_t esize;

	unsigned _u8() {
		int _u = 0; auto* pbyte = (unsigned char*)this->esdata;
		while (true) {
			switch (*pbyte++) {
				case 0: return _u; // return
				case 0xE4ui8:case 0xE7ui8:
				case 0xE5ui8:case 0xE8ui8:
				case 0xE6ui8:case 0xE9ui8:
					_u++; pbyte += 2;break;
				default:		_u++;		break;
			}
		}
	}
	static
	int _hex_int(const char* pCalc) { // 'a'->0xa
		int iRet = *pCalc;
		if ('a' <= iRet && 'f' >= iRet) {
			return iRet - 'a' + 10;
		}
		if ('A' <= iRet && 'F' >= iRet) {
			return iRet - 'A' + 10;
		}
		if ('0' <= iRet && '9' >= iRet) {
			return iRet - '0';
		}
		return 0;
	};
	void _rep(const char* _, size_t _u,const char* _to, size_t _uto) {
		if(!this->esdata) return;

		char* p, * bak, * n, * n_; size_t _uc;
		p = bak = this->esdata;
		if (p = strstr(bak, _)) {
			_uc = this->size() + 2;
			if (_u < _uto) _uc += this->occurs(_, _u) * (_uto - _u);		
			if (n = n_ = ealloc(_uc)) {
				do {
					memcpy(n, bak, p - bak); n += (p - bak);
					if(_uto) memcpy(n, _to, _uto),n += _uto;
					bak = p + _u;
				} while (p = strstr(bak, _));
				_u = strlen(bak); memcpy(n, bak, _u + 1); n += _u;
			}

			efree(this->esdata);
			this->esize = n - n_;
			this->esdata = n_;
		}
	}
	// 
	// This function is DANGEROUS
	// Cvt this to utf16 and then unsupport this->size()
	// Attention: Only for win32 api to pass this for one time
	// 
	wchar_t* as_wstr() {
		size_t u; wchar_t* w;
		if (this->esize != ~0) {

			if (w = _us2wcs(this->esdata, &u)) {
				efree(this->esdata);

				this->esdata = (char*)w;
				this->esize = ~0;
				return w;
			}
		}
		return (wchar_t*)this->esdata;
	}
} *estr_t;

typedef class estrs {
public:

	template <size_t _N, size_t N = _N - 1>
	estrs(const char* src, const char (&div)[_N]) {
		static_assert(_N - 1 != 0, "Invalid_div_data");
		char *p,*s,*e=(char*)""; p = s = (char*)src;
		size_t u = 0;
		
		// calc argv size
		while (s = strstr(s, div)) { s += N; u++; }; 
		this->esarg = new estr[++u];
		this->esargc = u; s = p;
		
		for (size_t i = 0; i < u; i++) {
			auto& asp = this->esarg[i];

			p = strstr(s, div);
			asp.assign(s, p ? p - s : ~0);
			s = p + N;
		}
	}

	estr& operator[] (const size_t uId) {
		if (uId > this->sizes()) {
			dbk(!this->sizes(), this);
			return *this->esarg;
		}
		return this->esarg[uId];
	}

	~estrs() {
		if (this->esarg) {
			delete[](this->esarg);
		}
	}
	size_t sizes() { return this->esargc; }

	// *********** ±ê×¼µü´ú ************ //
	class iterator {
	public:
		iterator(estr* t, size_t sz) : itptr(t), itpos(sz) {}
		iterator& operator++() { this->itpos++; return *this; }
		bool operator !=(iterator& it) { return this->itpos != it.itpos; }
		estr& operator*() { return itptr[itpos]; }
	private:	
		estr* itptr; size_t itpos;
	};
	iterator end() { return iterator(this->esarg, this->esargc); }
	iterator begin() { return iterator(this->esarg, 0); };
private:
	estr* esarg;
	size_t esargc;
} *estrs_t;

struct ewatch {
	~ewatch() { if (_fptr) efree(_fptr); }
	ewatch(void* _) :_fptr(_) {}
	void* _fptr;
};

// 
// #easyjson define 
// Using '#define ej_type' to disable this
// 

#ifndef ej_type
# define ej_type			 (1 << 24)
# define ej_value			0x00000000
# define ej_float			0x01000000
# define ej_bool			0x02000000
# define ej_string			0x04000000
# define ej_json			0x08000000

# define ej_new			0x10000000
# define ej_array			0x20000000
# define ej_null			0x80000000

# define ej_vals			(ej_array | ej_value)
# define ej_flts				(ej_array | ej_float)
# define ej_strs			(ej_array | ej_string)
# define ej_brs				(ej_array | ej_bool)
# define ej_jss				(ej_array | ej_json)
# define ej_attr			0xFF000000
# define ej_size			0x00FFFFFF

//
// easyjson
// attention: not support parse float array yet
// 
typedef class ejson {
public:
	ejson() :emap(0), esori(0), ename(0), esttr(0), esuh(0) {}
	template<size_t N> ejson(const char(&j_str)[N]) : ejson() { this->parse(j_str, N - 1); }

	bool parse(const char* j_str, size_t j_size = ~0, cstr jstr = 0) {
		if (this->esuh == 0) {

			if (this->esttr & ej_json) 
				this->destory();
			
			if ((jstr = zip_json(j_str, j_size))) {
				this->parse_json(this, jstr, (cstr)"Obj", 1);
				this->esori = jstr;
				this->esuh = 0;
				return true;
			}
			null_json(this);
		}
		return false;
	}

	bool has(const char* name) {
		dbi(!this->is_json(), this);

		auto i = this->count();
		auto _u = _us(name);
		while (i--) {
			auto& js = ((ejson*)this->emap)[i];
			if (js.esuh == _u)
				return true;
		}
		return false;
	}

	ejson& operator[] (const char *name) {
		dbi(!this->is_json(), this);

		auto i = this->count();
		auto _u = _us(name);
		while (i--) {
			auto& js = ((ejson*)this->emap)[i];
			if (js.esuh == _u)
				return js;
		}
		return *this; // no use for multi obj
	}

	ejson& operator[] (const size_t uId) {
		if (this->is_ary()) {
			if (uId < this->count()) {
				return ((ejson*)this->emap)[uId];
			}
			dbk(!this->count());
		}
		dbk(!this->is_ary(), this);
		return *this;
	}
	//
	// replace with string will alloc new memory
	//
	ejson &operator= (const char* val) {
		if (this->valid()) {
			this->destory();
				
			this->esori = _strdup(val);
			this->esttr = ej_new | ej_string | (ej_size & strlen(val));
			this->emap = (intptr_t)this->esori;
			if (*val >= '0' && *val <= '9' || *val == '-') {
				this->emap = intptr_t(atoll(val));
				this->esttr ^= ej_string;
			}
		}
		return *this;
	}

	//
	// motify value will not change the origin string
	//
	ejson& operator=(const int& val) { return operator=((size_t)(val)); }
	ejson& operator=(const size_t& val) {
		if (this->is_value()) { 
			this->by(val);
		}
		return *this;
	}
	bool operator==(const char* str) { return this->esori ? 0 == strcmp(str, this->esori) : false; }

	operator bool() { return this->valid(); }
	operator char*() { return this->operator*(); }
	char* operator*() { return (this->valid() && this->esori)? this->esori : (char*)""; }
	#ifdef _WIN64
	operator size_t () { return this->as<size_t>(); }
	#endif

	operator int () { return this->as<int>(); }
	operator long () { return this->as<long>(); }
	operator double () { return this->as<double>(); }
	operator float () { return float(this->as<double>()); }
	operator long long () { return this->as<long long>(); }
	operator unsigned  () { return this->as<unsigned>();  }
	operator unsigned long () { return this->as<unsigned>(); }
	operator unsigned char* () { return this->as<unsigned char*>(); }

	bool valid() { return !this->isfather(); }
	unsigned count() { return this->esttr & ej_size; }

	~ejson() { this->destory(); }
private:
	ccstr ename;
	cstr esori;

	iint emap;
	uint esttr; // attri
	uint esuh; // hash

	template <typename T>
	decltype(auto) as() { 
		return *(T*)&emap; 
	}

	template <typename T>
	void by(T val) {
		dbi(sizeof T > sizeof emap, "what?");
		if (sizeof T < sizeof emap) {
			this->emap = 0;
		} *(T*)&emap = val;
	}

	inline bool isfather() { return esuh == 0 && (esttr & ej_json); }
	inline bool is_value() { return (esttr & ej_type) == ej_value; }
	inline bool is_null() { return (esttr & ej_null) != 0; }

	inline bool is_vals()	{ return !!(esttr & ej_vals); }
	inline bool is_strs()	{ return !!(esttr & ej_strs); }
	inline bool is_jss()	{ return !!(esttr & ej_jss); }

	inline bool is_new()	{ return !!(esttr & ej_new); }
	inline bool is_json()	{ return !!(esttr & ej_json); }
	inline bool is_bool()	{ return !!(esttr & ej_bool); }
	inline bool is_ary()	{ return !!(esttr & ej_array); }
	inline bool is_str()	{ return !!(esttr & ej_string); }

	void destory() {
		if (this->isfather()) {
			delete[](ejson*)emap;
			::free(this->esori);
			return;
		}
		if (this->is_null() || !this->emap) {}
		else {
			if ((this->esttr & ej_new)) {
				if (this->esori) {
					::free(this->esori);
				}
			}
			if (this->esttr & ej_array) {
				delete[](ejson*)emap;
			}
		}
	}	

	static void null_json(ejson* jss) {
		auto& js = *jss;
		js.esori = (char*)"null";
		js.esuh = "null"_us;
		js.esttr = ej_null;
		js.emap = ~0;
	}
	// 1. compress and check valid 
	// 2. try recursion parse the json 
	// 3. package memory for every instence

	// 
	// compress the json and malloc
	static 
	char* zip_json(const char* j_str, size_t j_sz = ~0) {
		if (j_sz == ~0) j_sz = strlen(j_str);
		if (j_sz == 0) {
			dbk(j_str, j_sz);
			return nullptr;
		}
		char* jw, * jbak; 
		unsigned ins = 0, _c = 0;
		if (jw = ealloc(j_sz + 2)) {
			jbak = jw; 
			while (*j_str) {
				if (ins & 1) {
					if (*j_str == '\\') {
						if (*++j_str == '"') {
							*jw++ = '\''; // '
							j_str++;
							continue;
						}
					}
					else if (*j_str == '"') {
						ins ^= 1;
					}
					*jw++ = *j_str++;
				}
				else {
					// no ins kill space
					switch (*j_str) {
						case ':':  _c++; goto _def;
						case '"': ins^=1; goto _def;
						case '[':case '{': ins ^=2; goto _def;
						case ']':case '}': ins ^=2; goto _def;
						case ' ':case '\t':case '\n':case '\r': j_str++; break;
						default: _def:
							*jw++ = *j_str++;
							break;
					}
				}
			}
			if (ins == 0) {
				*jw = '\0';
				return jbak;
			}
			dbk(ins, "Json illegal");
			::free(jbak);
		}
		return nullptr;
	}
	
	static 
	void array_json(ejson* jss, cstr p) {
		ejson& js = *jss; cstr s;
		uint u = calcary_json(p); dbi(u == ~0);
		if (u > 0 && u != ~0) {
			jss = new ejson[u];
			js.emap = (intptr_t)jss;
			js.esttr = ej_array | (u & ej_size);

			if (*p == '[') p++;
			if (*p == '{') {
				js.esttr |= ej_json;
				do {
					s = next_json(p);if (s)*s ='\0';
					parse_json(jss++, p, (cstr)"Obj", 1);
					p = s + R"(},)"_l;
				} while (*p == '{');
				return;
			}
		
			if (*p == '"') {
				js.esttr |= ej_string; p++;
				do {
					s = strstr(p, R"(",")"); if (s)*s ='\0';
					auto& ss = *jss++; ss.esori = (cstr)p;
					ss.emap = (iint)(p); ss.ename = "Ary";
					ss.esttr = ej_string|(ej_size&(s?(s-p):strlen(p)-1)); // end of '"'
					if (s) p = s + R"(",")"_l; else *(p + ss.count()) = '\0';
				} while (s);
				return;
			}

			if ((*p >= '0' && *p <= '9') || (*p == '-')) {
				js.esttr |= ej_value;
				do {
					s = strstr(p, ","); if (s)*s = '\0';
					auto& ss = *jss++; ss.esori = (cstr)p;
					ss.emap = atoll(p); ss.ename = "Ary";
					ss.esttr = (ej_size&(s?(s-p):strlen(p)));
					if (s)p = s + ","_l;
				} while (s); 
				return;
			}
			js.esttr |= ej_bool;
			do {
				s = strstr(p, ","); if (s)*s = '\0';
				auto& ss = *jss++; ss.esori = (cstr)p; 
				ss.emap = (*p=='t')||(*p=='T'); ss.ename = "Ary";
				ss.esttr = (ej_size&(s?(s-p):strlen(p)));
				p = s + ","_l;
			} while (s);
			return;
		}
	}
	static 
	char* parse_json(ejson* jss, cstr v, cstr n, uint uh = 0) {
		auto& js = *jss;
		js.esuh = uh ? uh : _us(n);
		js.esori = v;  js.ename = n;
		switch (*v) {
			case '-':case '.':case '0':
			case '1':case '2':case '3':case '4':
			case '5':case '6':case '7':case '8':case '9':
				if(n = strchr(v, ','))*n='\0'; else n=v+strlen(v); dbi(!n, v);

				js.esori = v; js.emap = atoll(v);
				js.esttr = (strchr(v, '.')?ej_float:ej_value)|(ej_size&(n-v));
				if (js.esttr == ej_float) js.by(atof(v));
				return n;

			case 'n':case 'N': case 't':case 'T':case 'f':case 'F':
				n = strchr(v, ','); goto _here;

			case '"': n=strstr(v, R"(",)");if (n)n++;// sizeof(")
			_here: if (!n)n=v+strlen(v); dbi(!n, v);*n='\0';
				if (*v == '"') {
					v=v+1;*(n-1)='\0';
					js.esori = v; js.emap = iint(v);
					js.esttr = ej_string | (ej_size & (n++-v-1)); 
				}
				else {
					js.esori = v;
					if (*v == 't' || *v == 'T') {
						js.esttr = ej_bool;
						js.emap = 1;
					}
					else if (*v == 'f' || *v == 'F') {
						js.esttr = ej_bool;
						js.emap = 0;
					}
					else if (*v == 'n' || *v == 'N') {
						js.esttr = ej_null;
						js.emap = ~0;
					}
				}
				return n;
			case '[': {
				n = nextary_json(v); dbi(!n, v); if(n)*n++ = '\0';
				array_json(jss, v);
				return ++n;
			}
			case '{': {
				ejson *son; cstr s;
				n = next_json(v++); dbi(!n, "no next_json");if (n)*n++ = '\0';
				uh = calc_json(v);	 
				if (uh != ~0) {
					son = new ejson[uh];
					js.emap = (iint)son;
					js.esttr = ej_json | (uh & ej_size);

					while (uh--) {
						s = strchr(++v, '":'); dbi(!s, s, v); if (s)*(s++ - 1) = '\0';
						v = parse_json(son++, s, v); if(*(v+1)=='"')v++;
						if ( *v == ',') v++;
					}
				} else null_json(jss);
				return n;
			}			
			default: break;
		}
		return nullptr;
	}	
	static 
	char* next_json(char* js_) {
		bool bPass = 0, bAry = 0; 
		unsigned u = 0;
		while (*js_) {
			if (*js_ == '"') bPass ^= 1;
			else if (!bPass) {
				if (*js_ == '[' || *js_ == ']') bAry ^= 1;

				if (!bAry) {
					if (*js_ == '{') u++;
					else if (*js_ == '}') {
						if (--u == 0) {
							break;
						}
					}
				}
			}
			js_++;
		}
		return js_;
	}
	static
	char* nextary_json(char* js_) {
		bool bPass = 0; 
		while (*js_) {
			if (*js_ == '"') bPass ^= 1;
			else if (!bPass) {
				if (*js_ == ']')
					break;
			}
			js_++;
		}
		return js_;
	}
	static
	unsigned calc_json(cstr p) {
		unsigned ur = 0;
		bool bAry = 0;
		while (p && *p) {
			if (bAry && *p != ']') p++;
			else switch (*p++) {
				case '[': case ']': bAry ^= 1; break;
				case '{': 
					p = next_json(--p); break;
				case '"':
					p = strchr(p + 1, '"'); dbi(!p, p);
					if (p && *(p + 1) == ':') { // "xxx":""
						ur++; p += 2;
						if (*p == '"') {
							p = strchr(p + 1, '"'); dbi(!p, p);
							p++;
						}
					};
					break;
				default: break;
			}
		}
		return ur ? ur : ~0;
	}
	static
	unsigned calcary_json(cstr p) {
		unsigned ur = 0;
		if (*p == '[') p++;
		if (*p == '"') {
			while (p = strstr(p, R"(",")")) ur++,p+= R"(",)"_l;
			return ur ? ur + 1 : 0;
		}
		if (*p == '{') {
			while (p = strstr(p, R"(},{)")) ur++,p+= R"(},)"_l;
			return ur ? ur + 1 : 0;
		}
		// (*p >= '0' && 'p' <= '9') TRUE True False False
		while (p = strstr(p, R"(,)")) ur++, p++;
		return ur ? ur + 1 : 0;
	}
} *ejson_t;
# undef ej_type
# undef ej_value
# undef ej_float
# undef ej_bool
# undef ej_string
# undef ej_json

# undef ej_new 
# undef ej_array
# undef ej_null

# undef ej_vals
# undef ej_flts
# undef ej_strs
# undef ej_brs
# undef ej_jss
# undef ej_attr
# undef ej_size
#endif // asj_type

# ifdef _WIN32
# define ELL(x) L## x // estd str to L
# define EDFUNC(n) *_ec_ ## n // estd-decrypt 
# define ENFUNC(n) constexpr ecstr _ec_ ## n(ELL(# n))

# define EWATCH(n) ewatch _ew_ ## n((void*)n)
# define ECSTR(m,n) constexpr ecstr m(ELL(n))
typedef ejson Json, EOBJ; // estd object
typedef estrs ESTRS; // estd strings
typedef estr ESTR; // estd string
# endif

#endif // _ESTD_H_