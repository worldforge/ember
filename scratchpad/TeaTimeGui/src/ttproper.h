//////////////////////////////////////////////////////////////////////////////
//   File name:  ttproper.h
//
//   Unit:       Properties
//
//   Project:    TTGui & general
//   Author:     Hans H„ggstr”m,  hans.haggstrom@helsinki.fi
//   Purpose:    Load and saveable class properties easily.
//
//
//   Work diary:
// Date      Time     Lines   Things done
// ~~~~~~~~~ ~~~~~~~~ ~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~
// 18.10.98           76
// 29.11              85      Hmm.. slow progress..  Off to play Quake. .. .
// 29.11              128     Allright.  But now i'll go play quake. bye =)
//
// Notes & things to do:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  * How to implement the edit controls?
//
//
//////////////////////////////////////////////////////////////////////////////

#ifndef TTPROPER_H
#define TTPROPER_H

#include <stdio.h>
#include <iostream.h>
#include <string>

/*==========================================================================*/
class TTPropertyClass;

typedef *(void ( string msg, int errType, int line,
                 string filename, TTPropertyClass *pclass ))   TTErrorFuncP;
typedef *(bool (istream in,  void *variable, int *line, TTErrorFuncP ef)) TTLoaderFuncP;
typedef *(bool (ostream out, void *variable, int *line, TTErrorFuncP ef)) TTSaverFuncP;

/*==========================================================================*/
class TTPropertyType {
  public:
    TTPropertyType();
    TTPropertyType( string name_, string desc_,
                    TTLoaderFuncP loader_, TTSaverFuncP saver_
                    /*, edit control func ptrs?*/);
    ~TTPropertyType();

  protected:
    string        name;
    string        desc;
    TTLoaderFuncP loader;
    TTSaverFuncP  saver;

    /* Editing controls stuff */

    static PtrList<TTPropertyType*> registeredPropertyTypes;

}; // class TTPropertyType.

/*==========================================================================*/
class TTProperty {
  public:
    TTProperty();
    TTProperty( string name_, string type_, void *value_ );
    ~TTProperty();

  protected:
    string          name;
    string          type; // To store the type string if it wasn't recognized.
    TTPropertyType *propertyType;
    void           *value;

}; // class TTProperty.


/*==========================================================================*/
class TTPropertyClass {
  public:
    TTPropertyClass();
    TTPropertyClass( const TTPropertyClass &source );
    virtual ~TTPropertyClass();

    virtual bool registerProperty( string name_, string type_, void *value );
    // Adds the specified value to the property list, the value _must_ be a
    // pointer to a non-static field of the same class instance from
    // whitch the registerProperty was called (or NULL, in which case the call is ignored).

    virtual load( istream in,  int *line, TTErrorFuncP errorFunc );
    // Loads the values of the registered properties of this class from specified istream.

    virtual save( ostream out, int *line, TTErrorFuncP errorFunc );
    // Saves the registered properties of this class in specified ostream.

    virtual string getClassName() {
      // Returns classname (should return same name as used in code (?)):
      return "TTPropertyClass";
    }

    virtual string getName() { return name; }
    // Returns name of the current class instance (eg. "TTButton7").

    // static find class instance with specified name ...

    virtual bool inherits( string ancestor ) {
      // Returns true if ancestor classtype is same as the type of this class
      // or any of its ancestors.
      return ancestor == getClassName();
    }

  protected:

    string name;
    // Name id of this class instance.

    PtrList<TTProperty*> properties;
    // List of all registered properties of this class.

    static PtrList<TTPropertyClass*> classlist;
    // List with all TTPropertyClasses and decendants.  Used for finding
    // a class instance based on name, etc.

}; // class TTPropertyClass.

/*==========================================================================*/
#endif


