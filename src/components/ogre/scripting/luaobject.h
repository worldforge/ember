// luaobject.h
//
// Copyright 2008 by Nigel Atkinson suprapilot+LuaCode@gmail.com 
//
//    This library is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    A copy of theGNU General Public License is availible at:
//    <http://www.gnu.org/licenses/>.
// 
// Makes interfacing with Lua easier.  
// Tested with Lua 5.1.3 and 5.1.2.
// MSVC 7.12 (2003) and GCC (g++) 4.1.2

// 8th May 2008 Nigel
// Added LuaException class and changed all throws to use it.  User code can use it too.
// It grabs any string, if any, from the top of the Lua stack.


#ifndef __LUAOBJECT_H
#define __LUAOBJECT_H

#include <lua.hpp>
#include <string>
#include <sstream>

// Used when the object to set the LuaRef to, on creation, is on the top of the lua stack.
struct fromStack
{
	lua_State *mL;

	fromStack( lua_State *L ) : mL(L)
	{;}
};

// Pops a number of objects off the Lua stack once out of scope.
// Used to ensure the stack is left as it was, even if exceptions occur.
class luaPop
{
	lua_State *mL;
	int mNum;

public:

	luaPop( lua_State *L, int num=1 ): mL(L), mNum(num)
	{;}

	~luaPop()
	{
		lua_pop( mL, mNum );
	}
};

class LuaRef;

// Basic "ANY" class for representing Lua objects.
// Used in calling Lua functions as parameters.
class LuaVal
{
	int mType;
	double d;
	std::string str;
	lua_CFunction func;
	LuaRef *obj;

public:
	LuaVal() : mType( LUA_TNIL ), obj(NULL)
	{;}

	LuaVal( double n ) : mType( LUA_TNUMBER ), d( n ), obj(NULL)
	{;}

	LuaVal( std::string n ) : mType( LUA_TSTRING ), str( n ), obj(NULL)
	{;}

	LuaVal( const char *n ) : mType( LUA_TSTRING ), str( n ), obj(NULL)
	{;}

	LuaVal( lua_CFunction n ) : mType( LUA_TFUNCTION ), func( n ), obj(NULL)
	{;}

	LuaVal( LuaRef *o ) : mType( LUA_TLIGHTUSERDATA ), obj(o)
	{;}

	void push( lua_State *L );
};


// Exception class
class LuaException
{
	std::string error;
	std::string luaError;
	std::string file;
	long line;
	lua_State *mL;

	// Do we we need a copy constuctor? I think the default supplied one works.

public:
	LuaException( lua_State *L, const char *str, const char *filename, long fileline ) : error(str), file(filename), line(fileline)
	{
		if( lua_gettop( L ) != 0 )
			if( lua_isstring( L, -1 ) )
				luaError = lua_tostring( L, -1 );
		mL = L;
	}

	std::string getError() { return error; }
	std::string getLuaError() { return luaError; }
	lua_State *getLuaState() { return mL; }
	std::string getErrorForDisplay()
	{
		std::stringstream ss;

		ss << "*** " << error << " ***" << std::endl; 
		ss << "*** " << luaError << " ***" << std::endl;
		ss << "*** In file: " << file << " Line: " << line << " ***" << std::endl;

		return ss.str();
	}

	~LuaException() throw () {}
};

// A Lua Object.  Represents a Lua object within a Lua state.
// Each is referenced, so the Lua GC will not garbage collect this object while this instance is in scope.
// Once out of scope, the Lua object could be garbage collected, depending on any other references.
class LuaRef
{
	int mRef;			// Index to reference of object.
	lua_State *mL;		// and the vm

	// Private internal class for returning as a proxy object.  This is used when asked for a table element.
	// This class can not be instantated outside of LuaRef.  It lets you use syntax like the following:
	// 
	// tbl["index"] = something;
	//
	// given that "tbl" is a LuaRef.
	
	class tableElement
	{
		// Allow outside class access to members, and private constructor.
		friend class LuaRef;
		
		std::string mKey;		// Element index
		int mRef;			// Reference to table
		lua_State *mL;
	
		// Private constructor.  
		tableElement( lua_State *L, int ref, const char *key ) : mKey(key), mRef( ref ), mL(L)
		{ ; }

	public:

		// Put table element on top of Lua stack.
		void push()
		{
			lua_rawgeti( mL, LUA_REGISTRYINDEX, mRef );
			lua_getfield( mL, -1, mKey.c_str() );
		}

		// Return the 'type' of the table element.
		int type()
		{
			int ret;

			push();
			ret =  lua_type( mL, -1 );
			lua_pop( mL, 2 );			// Remove index and table from stack.

			return ret;
		}

		double operator = ( double f )
		{
			luaPop p(mL);
			lua_rawgeti( mL, LUA_REGISTRYINDEX, mRef );
			lua_pushnumber( mL, f );
			lua_setfield( mL, -2, mKey.c_str() );

			return f;
		}

		std::string & operator = ( std::string & str )
		{
			operator = ( str.c_str() );			// Just re-use the const char * code.

			return str;
		}

		const char * operator = ( const char *str )
		{
			luaPop p(mL);
			lua_rawgeti( mL, LUA_REGISTRYINDEX, mRef );
			lua_pushstring( mL, str );
			lua_setfield( mL, -2, mKey.c_str() );

			return str;
		}

		lua_CFunction operator = ( lua_CFunction func )
		{
			luaPop p(mL);
			lua_rawgeti( mL, LUA_REGISTRYINDEX, mRef );
			lua_pushcfunction( mL, func );
			lua_setfield( mL, -2, mKey.c_str() );

			return func;
		}

		LuaRef & operator = ( LuaRef &obj )
		{
			luaPop p(mL);

			lua_rawgeti( mL, LUA_REGISTRYINDEX, mRef );
			obj.push();
			lua_setfield( mL, -2, mKey.c_str() );

			return obj;
		}

		operator double()
		{
			luaPop p1(mL,2);		// Removes index and table from Lua stack once out of scope.

			push();	
			
			return lua_tonumber( mL, -1 );
		}

		operator std::string()
		{
			push();	
			std::string str( lua_tostring( mL, -1 ) );
			lua_pop( mL, 2 );		// Removes index and table from Lua stack
				
			return str;
		}

		operator LuaRef()
		{
			luaPop p( mL, 2);
			
			push();
			fromStack fs( mL );
						
			return LuaRef(fs);
		}
	};

public:

	LuaRef( lua_State *L ) : mRef( LUA_NOREF ), mL( L )			// For "new" objects, that will be assigned a value later.
	{;}

	LuaRef( lua_State *L, const char *name ) : mL( L )			// Reference an existing object.  Note that numbers and strings
	{															// will be copied.  Any changes will not change the original.
		lua_getglobal( mL, name );
		mRef = luaL_ref( mL, LUA_REGISTRYINDEX );
	}

	LuaRef( fromStack fs ) : mL( fs.mL )						// Reference an existing object that is on top of the Lua stack.
	{															// Note same cavet as above.
		mRef = luaL_ref( mL, LUA_REGISTRYINDEX );				// Removes from stack.
	}

	LuaRef( tableElement & te ) : mL( te.mL )					// Reference a table element.
	{
		lua_rawgeti( te.mL, LUA_REGISTRYINDEX, te.mRef );
		lua_getfield( te.mL, -1, te.mKey.c_str() );
		mRef = luaL_ref( mL, LUA_REGISTRYINDEX );
	}

	LuaRef(const LuaRef &obj ) : mL( obj.mL )
	{
		LuaRef& objRef = const_cast<LuaRef&>(obj);
		objRef.push();
		mRef = luaL_ref( mL, LUA_REGISTRYINDEX );
	}

	~LuaRef()													// We're gone.  Release reference to object.
	{
		luaL_unref( mL, LUA_REGISTRYINDEX, mRef );
	}

	// Place object on top of Lua stack.
	void push()
	{
		lua_rawgeti( mL, LUA_REGISTRYINDEX, mRef );
	}

	// Return the 'type' of the object
	int type()
	{
		int ret;

		push();
		ret =  lua_type( mL, -1 );
		lua_pop( mL, 1 );

		return ret;
	}

	// Create a new table.
	void createTable()
	{
		lua_newtable( mL );
		mRef = luaL_ref( mL, LUA_REGISTRYINDEX );
	}

	// Create a new table and associate a global variable with it.
	void createTable( const char *name )
	{
		createTable();
		store( name );
	}

	// Return a proxy to a table element givin an index.
	tableElement operator [] ( const char *key )
	{
		push();

		if( ! lua_istable( mL, -1 ) )
			throw LuaException( mL, "LuaRef operator [] used on a non Lua table", __FILE__, __LINE__ );

		lua_pop( mL, 1 );

		return tableElement( mL, mRef, key );
	}

	// Return a proxy to a table element givin an index.
	tableElement operator [] ( int key )
	{
		push();

		if( ! lua_istable( mL, -1 ) )
			throw LuaException( mL, "LuaRef operator [] used on a non Lua table", __FILE__, __LINE__ );

		lua_pop( mL, 1 );

		std::stringstream ss;

		ss << key;

		return tableElement( mL, mRef, ss.str().c_str() );
	}

	// Reference an object referenced by "obj"
	LuaRef & operator = ( LuaRef &obj )
	{
		luaL_unref( mL, LUA_REGISTRYINDEX, mRef );
		mL = obj.mL;
		obj.push();
		mRef = luaL_ref( mL, LUA_REGISTRYINDEX );

		return *this;
	}

	// Reference a number as a new object.
	double operator = ( double f )
	{
		luaL_unref( mL, LUA_REGISTRYINDEX, mRef );
		lua_pushnumber( mL, f );
		mRef = luaL_ref( mL, LUA_REGISTRYINDEX );
		return f;
	}

	// Reference a string as a new object.
	std::string & operator = ( std::string & str )
	{
		operator = ( str.c_str() );
		return str;
	}

	// Reference a string.
	const char * operator = ( const char *str )
	{
		luaL_unref( mL, LUA_REGISTRYINDEX, mRef );
		
		lua_pushstring( mL, str );
		mRef = luaL_ref( mL, LUA_REGISTRYINDEX );		
		return str;
	}

	// Reference a function.
	lua_CFunction operator = ( lua_CFunction func )
	{
		luaL_unref( mL, LUA_REGISTRYINDEX, mRef );
		lua_pushcfunction( mL, func );
		mRef = luaL_ref( mL, LUA_REGISTRYINDEX );
		return func;
	}

	// Associate referenced object with a name, or global variable.
	void store( const char *name )
	{
		push();		
		lua_setglobal( mL, name);
	}

	// Associate referenced object as a member of a table.
	void store( const char *name, LuaRef & table )
	{
		if( table.type() != LUA_TTABLE )
			throw LuaException( mL, "given object is not a table.", __FILE__, __LINE__ );

		table.push();
		push();
		lua_setfield( mL, -2, name );	// Pops value
		lua_pop( mL, 1 );	// Pops table
	}

	// Return as a number.
	operator double()
	{
		double ret = 0.0;
		luaPop p(mL);
		push();

		if( lua_isnumber( mL, -1 ) )
			ret = lua_tonumber( mL, -1 );
		else
			throw LuaException( mL, "LuaRef referenced object is not a number.", __FILE__, __LINE__ );

		return ret;
	}

	// Return as a string
	operator std::string()
	{
		std::string ret;
		luaPop p(mL);
		
		push();

		if( lua_isstring( mL, -1 ) )
			ret = lua_tostring( mL, -1 );
		else
			throw LuaException( mL, "LuaRef referenced object is not a string.", __FILE__, __LINE__ );

		return ret;
	}

	// Return as an object
	template<typename Treturn>
	Treturn asObject(const char* className)
	{
		luaPop p(mL);

		push();

		return static_cast<Treturn*>(luaL_checkudata(mL, -1, className));
	}

	// Return as an object
	template<typename Treturn>
	Treturn asObject(std::string className)
	{
		return asObject<Treturn>(className.c_str());
	}

	// The next few overloads of operator () allow calling a referenced Lua object (provided its a function),
	// with the same syntax as calling a C++ function.  Only the types LuaVal has conversions for can be used.
	// Upto 4 parameters, but more can be added.  Returns true on succesfull call.  No results are returned.

	LuaRef operator () ()
	{
		push();

		if( lua_isfunction( mL, -1 ) )
		{
			if( lua_pcall( mL, 0, 1, 0 ) != 0 )
				throw LuaException( mL, "Error running function in LuaRef operator ()", __FILE__, __LINE__ );
		}
		else
		{
            lua_pop( mL, 1 );
			throw LuaException( mL, "LuaRef operator () called but does not reference a function", __FILE__, __LINE__ );
		}
		
		fromStack fs( mL );

		return LuaRef( fs );
	}

	LuaRef operator () ( LuaVal p1 )
	{
		push();

		if( lua_isfunction( mL, -1 ) )
		{
			p1.push(mL);
			
			if( lua_pcall( mL, 1, 1, 0 ) != 0 )
				throw LuaException( mL, "Error running function in LuaRef operator ()", __FILE__, __LINE__ );
		}
		else
		{
            lua_pop( mL, 1 );
			throw LuaException( mL, "LuaRef operator () called but does not reference a function", __FILE__, __LINE__ );
		}
		
		fromStack fs( mL );

		return LuaRef( fs );
	}

	LuaRef operator () ( LuaVal p1, LuaVal p2 )
	{
		push();

		if( lua_isfunction( mL, -1 ) )
		{
			p1.push(mL);
			p2.push(mL);
			
			if( lua_pcall( mL, 2, 1, 0 ) != 0 )
				throw LuaException( mL, "Error running function in LuaRef operator ()", __FILE__, __LINE__ );
		}
		else
		{
            lua_pop( mL, 1 );
			throw LuaException( mL, "LuaRef operator () called but does not reference a function", __FILE__, __LINE__ );
		}
		
		fromStack fs( mL );

		return LuaRef( fs );
	}

	LuaRef operator () ( LuaVal p1, LuaVal p2, LuaVal p3 )
	{
		push();

		if( lua_isfunction( mL, -1 ) )
		{
			p1.push(mL);
			p2.push(mL);
			p3.push(mL);
			
			if( lua_pcall( mL, 3, 1, 0 ) != 0 )
				throw LuaException( mL,"Error running function in LuaRef operator ()" , __FILE__, __LINE__ );
		}
		else
		{
            lua_pop( mL, 1 );
			throw LuaException( mL, "LuaRef operator () called but does not reference a function", __FILE__, __LINE__ );
		}
		
		fromStack fs( mL );

		return LuaRef( fs );
	}

	LuaRef operator () ( LuaVal p1, LuaVal p2, LuaVal p3, LuaVal p4 )
	{
		push();

		if( lua_isfunction( mL, -1 ) )
		{
			p1.push(mL);
			p2.push(mL);
			p3.push(mL);
			p4.push(mL);
			
			if( lua_pcall( mL, 4, 1, 0 ) != 0 )
				throw LuaException( mL, "Error running function in LuaRef operator ()", __FILE__, __LINE__ );
		}
		else
		{
            lua_pop( mL, 1 );
			throw LuaException( mL,"LuaRef operator () called but does not reference a function" , __FILE__, __LINE__ );
		}
		
		fromStack fs( mL );

		return LuaRef( fs );
	}
};



#endif // __LUAOBJECT_H
