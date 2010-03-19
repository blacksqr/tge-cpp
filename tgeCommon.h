//   Copyright 2009 Jeffrey Bian
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//
//   tgeCommon.h
//   @purpose:  Basic stuff about Tcl-Game-Extension (TGE).
//   @author:   Jeffrey Bian
//	 @note:		No UNICODE support for now for simplicity. Never feed MBCS / Unicode
//				chars!!!!
//	 @version	1.0
//   @date:		Sep 2009

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _WITH_OGRE 0
#include <string>
#include <list>
#include <vector>
#include <cassert>
#include <map>
#include <boost/function.hpp>
#include <boost/bind.hpp>
extern "C" {
#include <tcl.h>
}
if _WITH_OGRE==1
#include <OgrePrerequisites.h>
#include <OgreString.h>
#include <OgreQuaternion.h>
#include <OgreVector4.h>
#include <OgreVector3.h>
#include <OgreVector2.h>
#include <OgreAny.h>
#endif

#pragma comment(lib, "tcl85.lib")
#ifndef NULL
#define NULL 0
#endif

// And once Factory create called, creates A Cmd with the same name of the object
// e.g., ActorPump create Jeff; // Jeff is a new command now! and also set a var with same name
//								// The pointer address of that obj in CPP
//		Jeff pukes;
//		puts $::Jeff; ---> 0x130322
// Signature for mapped action handle functions:
//		CmdStatus handler(
//				const Obj& argumentFirst,
//				const Obj& argumentRest,
//				int argumentRestNumber
//				)
//
#define SCRIPT_ACCESS_BEGIN virtual CmdStatus scriptAccess(const String& action, const Obj& arg1, const Obj& argList, int argc) {
#define SCRIPT_ACCESSOR_MAP(name, func) if(action.compare( #name ) == 0) { return func(arg1, argList, argc) ;} else
#define SCRIPT_ACCESSOR_MAP_DIRECT(name)  SCRIPT_ACCESSOR_MAP(name, name)
#define SCRIPT_ACCESS_END if (0){;return TGE_ERROR;};return TGE_ERROR;}

namespace tge
{
	class Engine;
	class Interpreter;
	class Obj;
	class Result;
	class Cmd;
	class UnknownCmd;
	class ScriptIntf;
	class ScriptIntfFactory;
	typedef int int32;
	typedef unsigned int uint32;
#ifdef __OgrePrerequisites_H__
	typedef float Real;
	typedef Ogre::String String;
	typedef std::vector<String> StringVector;
	typedef Ogre::Vector2 Vector2;
	typedef Ogre::Vector3 Vector3;
	typedef Ogre::Vector4 Vector4;
	typedef Ogre::Quaternion Quaternion;
	typedef Ogre::Any Any;
#else
	using Ogre::Real;
	using Ogre::StringVector;
	using Ogre::String;
	using Ogre::Vector2;
	using Ogre::Vector3;
	using Ogre::Vector4;
	using Ogre::Quaternion;
	using Ogre::Any;
#endif
	typedef std::list<Obj*> ObjList;


	enum CmdStatus
	{
		TGE_OK = TCL_OK,
		TGE_ERROR = TCL_ERROR,
	};

	class except
	{
	public:
		except(const String& what) : what_(what) {}
		const String& what(void) const {return what_;}
	protected:
		String what_;
	};

}
