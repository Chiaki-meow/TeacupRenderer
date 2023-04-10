#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "../settings.h"
#include <cassert>
#include <exception>


#define MEMBER_FUNCTION(type, funcname, valname)		\
	public:												\
	void set##funcname(type val) { valname = val; }		\
	type get##funcname() const { return valname; }		\
	type valname;

#define VIR_MEMBER_FUNCTION(type, funcname, valname)			\
	public:														\
	virtual void set##funcname(type val) { valname = val; }		\
	virtual type get##funcname() const { return valname; }		\
	protected:													\
	type valname;

#define PI		3.1415926535898f
#define PI_2	6.2831853071795f