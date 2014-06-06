/*
 *  xercesc_utils.cpp - part of SmartBody-lib
 *  Copyright (C) 2008  University of Southern California
 *
 *  SmartBody-lib is free software: you can redistribute it and/or
 *  modify it under the terms of the Lesser GNU General Public License
 *  as published by the Free Software Foundation, version 3 of the
 *  license.
 *
 *  SmartBody-lib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Lesser GNU General Public License for more details.
 *
 *  You should have received a copy of the Lesser GNU General Public
 *  License along with SmartBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Andrew n marshall, USC
 */

#include "vhcl.h"
#include <errno.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "sbm/xercesc_utils.hpp"

#include <xercesc/framework/MemBufInputSource.hpp>

#define USELOG (0)

using namespace std;

////////////////////////////////////////////////////////////////////////////////////

string xml_utils::xml_w2s( wstring w ) {

	string s( w.length(), 0 );
	s.assign( w.begin(), w.end() );
	return( s );
}

wstring xml_utils::xml_s2w( string s ) {

	wstring w( s.length(), 0 );
	copy( s.begin(), s.end(), w.begin() );
	return( w );
}

////////////////////////////////////////////////////////////////////////////////////

bool xml_utils::xml_translate( string *str_p, const XMLCh* xml_str )	{

	if( str_p == NULL ) return( false );
	if( xml_str == NULL ) return( false );

	char *tmp_cp = XMLString::transcode( xml_str );
	*str_p = string( tmp_cp );
	XMLString::release( &tmp_cp );
	return( true );
}

string xml_utils::xml_translate_string( const XMLCh* xml_str, string dfl )	{

	string tmp; if( xml_translate( &tmp, xml_str ) ) return( tmp );  return( dfl );
}

wstring xml_utils::xml_translate_wide( const XMLCh* xml_str, string dfl)	{

	return( xml_s2w( xml_translate_string( xml_str, dfl ) ) );
}

double xml_utils::xml_translate_double( const XMLCh* xml_str, double dfl )	{

	string tmp; if( xml_translate( &tmp, xml_str ) ) return( atof( tmp.c_str() ) );
	return( dfl );
}

float xml_utils::xml_translate_float( const XMLCh* xml_str, float dfl)	{

	return( (float)xml_translate_double( xml_str, (double)dfl ) );
}

int xml_utils::xml_translate_int( const XMLCh* xml_str, int dfl)	{

	return( (int)xml_translate_double( xml_str, (double)dfl ) );
}

////////////////////////////////////////////////////////////////////////////////////

void xml_utils::xml_parse_error( const XMLCh* attr, DOMElement* elem )	{

	string att = xml_translate_string( attr );
	string val = xml_translate_string( elem->getAttribute( attr ) );
	string tag = xml_translate_string( elem->getTagName() );
	LOG( "XML-ERR: att:'%s'; val:\"%s\"; tag:<%s .../>", att.c_str(), val.c_str(), tag.c_str() );
}

////////////////////////////////////////////////////////////////////////////////////

bool xml_utils::xml_parse_string( 
	string *str_p, 
	const XMLCh* attr, 
	DOMElement* elem,
	bool verbose
)	{

	if( str_p == NULL ) {
		LOG( "xml_parse_string ERR: NULL str_p" );
		return( false );
	}
	if( attr == NULL ) {
		LOG( "xml_parse_string ERR: NULL XMLCh attribute ptr" );
		return( false );
	}
	if( elem == NULL ) {
		LOG( "xml_parse_string ERR: NULL DOMElement ptr" );
		return( false );
	}

	const XMLCh* value = elem->getAttribute( attr );
	if( value == NULL ) {
		LOG( "xml_parse_string ERR: NULL value" );
		return( false );
	}
	if( value[ 0 ] == chNull ) {
		if( verbose ) xml_parse_error( attr, elem );
		return( false );
	}
	xml_translate( str_p, value );
	return( true );
}

bool xml_utils::xml_parse_double( 
	double *d_p,
	const XMLCh* attr, 
	DOMElement* elem,
	bool verbose
 )	{

	if( d_p == NULL )	{
		LOG( "xml_parse_double ERR: NULL d_p" );
		return( false );
	}

	string str;
	if( xml_parse_string( &str, attr, elem, verbose ) )	{
		*d_p = atof( str.c_str() );
		return( true );
	}
	return( false );
}

bool xml_utils::xml_parse_float( 
	float *f_p,
	const XMLCh* attr, 
	DOMElement* elem,
	bool verbose
 )	{

	if( f_p == NULL )	{
		LOG( "xml_parse_double ERR: NULL f_p" );
		return( false );
	}

	double d;
	if( xml_parse_double( &d, attr, elem, verbose ) )	{
		*f_p = (float)d;
		return( true );
	}
	return( false );
}

bool xml_utils::xml_parse_int( 
	int *i_p,
	const XMLCh* attr, 
	DOMElement* elem,
	bool verbose
 )	{

	if( i_p == NULL )	{
		LOG( "xml_parse_double ERR: NULL i_p" );
		return( false );
	}

	double d;
	if( xml_parse_double( &d, attr, elem, verbose ) )	{
		*i_p = (int)d;
		return( true );
	}
	return( false );
}

int xml_utils::xml_parse_double( 
	double *d_arr,
	int arr_count,
	const XMLCh* attr, 
	DOMElement* elem,
	bool verbose
)	{

#if 0
	for( int i = 0; i < arr_count; i++ )	{
		d_arr[ i ] = 0.0;
	}
#endif

	if( d_arr == NULL )	{
		LOG( "xml_parse_double ERR: NULL d_arr" );
		return( 0 );
	}

	string str;
	if( xml_parse_string( &str, attr, elem, verbose ) )	{

		istringstream parser( str );
		int i;
		for( i = 0; i < arr_count; i++ )	{
			if( !( parser >> d_arr[ i ] ) ) {
				return( i );
			}
		}
		return( i );
	}
	return( 0 );
}

int xml_utils::xml_parse_float( 
	float *f_arr,
	int arr_count,
	const XMLCh* attr, 
	DOMElement* elem,
	bool verbose
)	{

	if( f_arr == NULL )	{
		LOG( "xml_parse_float ERR: NULL f_arr" );
		return( 0 );
	}

	string str;
	if( xml_parse_string( &str, attr, elem, verbose ) )	{

		istringstream parser( str );
		int i;
		for( i = 0; i < arr_count; i++ )	{
			if( !( parser >> f_arr[ i ] ) ) {
				return( i );
			}
		}
		return( i );
	}
	return( 0 );
}

int xml_utils::xml_parse_int( 
	int *i_arr,
	int arr_count,
	const XMLCh* attr, 
	DOMElement* elem,
	bool verbose
)	{

	if( i_arr == NULL )	{
		LOG( "xml_parse_int ERR: NULL i_arr" );
		return( 0 );
	}

	string str;
	if( xml_parse_string( &str, attr, elem, verbose ) )	{

		istringstream parser( str );
		int i;
		for( i = 0; i < arr_count; i++ )	{
			if( !( parser >> i_arr[ i ] ) ) {
				return( i );
			}
		}
		return( i );
	}
	return( 0 );
}

////////////////////////////////////////////////////////////////////////////////////

string 
xml_utils::xml_parse_string( const XMLCh* A, DOMElement* E, string dfl, bool v )	{

	string tmp; if( xml_parse_string( &tmp, A, E, v ) ) return( tmp ); return( dfl );
}

double 
xml_utils::xml_parse_double( const XMLCh* A, DOMElement* E, double dfl, bool v )	{

	double tmp; if( xml_parse_double( &tmp, A, E, v ) ) return( tmp ); return( dfl );
}

float 
xml_utils::xml_parse_float( const XMLCh* A, DOMElement* E, float dfl, bool v )	{

	float tmp; if( xml_parse_float( &tmp, A, E, v ) ) return( tmp ); return( dfl );
}

int 
xml_utils::xml_parse_int( const XMLCh* A, DOMElement* E, int dfl, bool v )	{

	int tmp; if( xml_parse_int( &tmp, A, E, v ) ) return( tmp ); return( dfl );
}
	
	// end of recent additions... Apr 2011
////////////////////////////////////////////////////////////////////////////////////

bool xml_utils::XMLStringCmp::operator() (const XMLCh* x, const XMLCh* y ) const {
	int result = XMLString::compareString( x, y );
	return result<0;
}

DOMElement* xml_utils::getFirstChildElement( const DOMNode* node ) {
    node=node->getFirstChild();

    if( node!=NULL &&
		node->getNodeType()!=DOMNode::ELEMENT_NODE ) {
        node=getNextElement( node );
    }

    return (DOMElement*)node;
}

DOMElement* xml_utils::getNextElement( const DOMNode* node ) {
    node=node->getNextSibling();

    while( node!=NULL &&
		   node->getNodeType()!=DOMNode::ELEMENT_NODE ) {
        node=node->getNextSibling();
    }

    return (DOMElement*)node;
}


/**
 *   Parse a C String (char[]) as a XML Input Source.
 *   Check the parser for the actual DOMDocument.
 *
 *   returns the error count in the parser.
 *
 *   Potentially throws OutOfMemoryException, XMLException
 */
int xml_utils::parseCString( const char *str, AbstractDOMParser* parser ) {
	//if( str[0]=='<' && str[1]=='?' && str[2]=='x' && str[3]=='m' && str[4]=='l' ) {
	//	//  Xerces doesn't seem to like the XML prolog in C strings
	//	char *orig = str;
	//	str = &str[5];
	//	while( str[0] != '>' && str[0] != 0 )
	//		str++;
	//	if( str[0] == 0 )
	//      str=orig;  // revert
	//	else
	//		str++;
	//}

	const MemBufInputSource inputSource( (const XMLByte*)str, (unsigned int)strlen(str), "XMLBuffer" );
	parser->parse( inputSource );

	return parser->getErrorCount();
}

const char* xml_utils::asciiString( const XMLCh* wstr ) {
	char* str = new char[ XMLString::stringLen(wstr)+1 ];
	int i = 0;
    XMLCh c = wstr[0];
	while( c!=0 ) {
		if( c<128 )
			str[i] = (unsigned char)c;
		else
			str[i] = 127; // unrepresented character
		c = wstr[++i];
	}
	str[i] = 0;

	return str;
}

const float xml_utils::xcstof( const XMLCh* value ) {

	char *cp = XMLString::transcode( value );
	float f = (float)atof( cp );
	XMLString::release( &cp );
	return( f );
#if 0
	int len = XMLString::stringLen(str);
	const XMLCh* endp = str+len;  // \0 terminator 
	errno = 0;
	return (float) wcstod( str, (wchar_t**)&endp );
#endif
}

const double xml_utils::xcstod( const XMLCh* value ) {

	char *cp = XMLString::transcode( value );
	double d = atof( cp );
	XMLString::release( &cp );
	return( d );
#if 0
	int len = XMLString::stringLen(str);
	const XMLCh* endp = str+len;  // \0 terminator 
	errno = 0;
	return wcstod( str, (wchar_t**)&endp );
#endif
}

void xml_utils::xmlToString( const DOMNode* node, string& converted ){ //recursively called fcn that traverses DOM tree
	//This function is only meant to handle Text and Element Node types (Attributes are part of Elements)
	switch( node->getNodeType() ) {
		case DOMNode::TEXT_NODE:
		{
			std::string temp;
			xml_utils::xml_translate(&temp, node->getTextContent());
			converted += temp;
			break;
		}

		case DOMNode::ELEMENT_NODE:
		{
			DOMElement *element= (DOMElement *)node; //instantiate an element using this node
			std::string tag;
			xml_utils::xml_translate(&tag, element->getTagName());
			DOMNamedNodeMap* attributes = element->getAttributes(); //gets all attributes and places them in a Node map

			converted += "<"; 
			converted += tag; 
			for( unsigned int i=0; i< (attributes->getLength()); i++ ) { //iterates through and includes all attributes
				converted += " ";
				std::string name = "";
				xml_utils::xml_translate(&name, attributes->item(i)->getNodeName());
				converted += name;
				converted += "=\"";
				std::string value = "";
				xml_utils::xml_translate(&value, attributes->item(i)->getNodeValue());
				converted += value;
				converted += "\"";
			}
			
			node = node->getFirstChild();
			if( node ) { //check children first
				converted += ">";  // end of start tag

				xmlToString( node, converted );
				node = node->getNextSibling();
				while( node ) {
					xmlToString( node, converted );
					node = node->getNextSibling();
				}

				// end tag
				converted += "</";
				converted += tag;
				converted += ">";
			} else {
				converted += " />";  // end of empty element
			}
			break;
		}

		//default:	// ignore other node types
	}
}


DOMDocument* xml_utils::parseMessageXml( XercesDOMParser* xmlParser, const char *str ) {
	try {
		// xml in a file?
		if( str[0]=='<' ) {
			if (USELOG) LOG("Parsing inline XML.");
			int numErrors = xml_utils::parseCString( str, xmlParser );
			if (numErrors > 0)
			{
				LOG("Found %d errors when parsing %s.", numErrors, str);
			}
		} else {
			if (USELOG) LOG("Parsing XML from file \"%s\"", str);
			std::ifstream ifs(str);
			std::string content( (std::istreambuf_iterator<char>(ifs) ),
								 (std::istreambuf_iterator<char>()    ) );
			int numErrors = xml_utils::parseCString(content.c_str(), xmlParser );
		}
		int errorCount = xmlParser->getErrorCount();
		if( errorCount > 0 ) {
			stringstream strstr;
			strstr << "xml_utils::parseMessageXml(): "<<errorCount<<" errors while parsing xml: ";
			LOG(strstr.str().c_str());
			return NULL;
		}
		return xmlParser->getDocument();
	} catch( const XMLException& e ) {
		std::string message = "";
		xml_utils::xml_translate(&message, e.getMessage());
		std::stringstream strstr;
		strstr << "xml_utils::parseMessageXml(): XMLException while parsing xml: " << message;
		LOG(strstr.str().c_str());
		return NULL;
	} catch( const SAXParseException& e ) {
		std::string message = "";
		xml_utils::xml_translate(&message, e.getMessage());
		std::stringstream strstr;
		strstr << "xml_utils::parseMessageXml(): SAXException while parsing xml: "<< message
				<< " (line "<< e.getLineNumber()<<", col "<< e.getColumnNumber()<<")";
		LOG(strstr.str().c_str());
		return NULL;
	} catch( const SAXException& e ) {
		std::string message = "";
		xml_utils::xml_translate(&message, e.getMessage());
		std::stringstream strstr;
		strstr << "xml_utils::parseMessageXml(): SAXException while parsing xml: "<<message;
		LOG(strstr.str().c_str());
		return NULL;
	} catch( const DOMException& e ) {
		std::string message = "";
		xml_utils::xml_translate(&message, e.getMessage());
		std::stringstream strstr;
		strstr << "xml_utils::parseMessageXml(): DOMException while parsing xml: "<<message;
		LOG(strstr.str().c_str());
		return NULL;
	}
}
		

std::string convertWStringToString(std::wstring w)
{
	std::string str(w.begin(), w.end());
	return str;
}
