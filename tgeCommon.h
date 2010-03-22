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
//	 @note:		1. No UNICODE support for now for simplicity. Never feed MBCS / Unicode
//				chars!!!!
//				2. Needs some Ogre support for types such as VectorX, Any, so I don't bother 
//				writing my own versions. Currently compiles under Ogre1.7 (dynamica linkage).
//	 @version	1.0
//   @date:		Sep 2009, March, 2010
//	 @modified:  
#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _WITH_OGRE 0
#include <string>
#include <list>
#include <vector>
#include <cassert>
#include <map>
#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>
extern "C" {
#include <tcl.h>
}
#include <OgrePrerequisites.h>
#include <OgreString.h>
#include <OgreStringVector.h>
#include <OgreQuaternion.h>
#include <OgreVector4.h>
#include <OgreVector3.h>
#include <OgreVector2.h>
#include <OgreAny.h>
// Below for visualC++ only
#pragma comment(lib, "tcl85.lib")
#ifndef NULL
#define NULL 0
#endif

// And once Factory create called, creates A Cmd with the same name of the object
// ActorPump create Jeff; 
// Jeff is a new command now! and also a var with same name
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
	using Ogre::Real;
	using Ogre::StringVector;
	using Ogre::String;
	using Ogre::Vector2;
	using Ogre::Vector3;
	using Ogre::Vector4;
	using Ogre::Quaternion;
	using Ogre::Any;

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
		void out(void) const
		{
			std::cerr<<what_<<std::endl;
		}
	protected:
		String what_;
	};

	// Foward decls
	class Cmd;
	class Engine;
	class Interprter;
	class Obj;
	class Result;
	class ScriptIntf;
	class ScriptIntfFactory;
	class UnknownCmd;
}
