/*
    Copyright (C) 2002  Lakin Wecker aka nikal

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef COLOR_H
#define COLOR_H

#include <assert.h>

namespace dime 
{
/**
 * Generic Color Class.
 *
 * Color class which defines +,*,/,- operators.
 * 
 *
 *
 * @author Lakin Wecker
 *
 */
    class Color 
    {

        //======================================================================
        // Private Variables
        //======================================================================
    private:	
         /**
          * Stores the R (red) value of the color
          */
        float myR;
        /**
         * Stores the G (green) value of the color
         */
        float myG;
         /**
         * Stores the B (blue) value of the color
         */
        float myB;
	/**
	 * Stores the A (alpha) value of the color
	 */
	float myA;
         

        //======================================================================
        // Public Methods
        //======================================================================
    public:
        
        //----------------------------------------------------------------------
        // Constructors
        
        /**
         * Creates a new instance of the class with "black" as the color
         */
        Color()
            : myR(0), myG(0), myB(0)
        {	
        }
        
        /**
         * Creates a new instance of the class with r,g,b values
         */
        Color(float r, float g, float b, float a = 1.0f)
	  : myR(r), myG(g), myB(b), myA(a)
        {
        }
        
        Color( const Color &source )
        {
            *this = source;
        }
        
        /**
         * Assignment operator.
         */
        Color &operator= ( const Color &source )
        {
            // Copy fields from source class to this class here.
            myR = source.myR;
            myG = source.myG;
            myB = source.myB;
	    myA = source.myA;

            // Return this object with new value
            return *this;
        }
        
        //----------------------------------------------------------------------
        // Destructor
        
        virtual ~Color()
        {
        }

        /*
         * Operator + works as expected.  It adds each field of the right hand side
         * object to the left hand side object and returns the result
         */
        
        Color operator +(const Color & rhs)
        {
            float r, g, b, a;
            r = rhs.myR+myR;
            g = rhs.myG+myG;
            b = rhs.myB+myB;
	    a = rhs.myA+myA;
            return Color(r,g,b,a);
        }

        /*
         * Operator - works as expected. It subtracts each field of the rhs
         * object from the lhs object and returns the result
         */
        Color operator -(const Color & rhs)
        {
            float r, g, b, a;
            r = myR-rhs.myR;
            g = myG-rhs.myG;
            b = myB-rhs.myB;
	    a = myA-rhs.myA;
            return Color(r,g,b,a);
        }
		
        /*
         * Operator / works as expected. It divides each field of the lhs
         * object by the rhs object and returns the result
         */
        Color operator/(const float & rhs)
        {
            assert(rhs != 0);

            float r, g, b, a;
            r = myR/rhs;
            g = myG/rhs;
            b = myB/rhs;
	    a = myA/rhs;
            return Color(r,g,b,a);
        }
	
        /*
         * Operator * works as expected. It multiplys each field by the scalar "rhs"
         */
        Color operator*(const float & rhs)
        {
            float r, g, b, a;
            r = myR*rhs;
            g = myG*rhs;
            b = myB*rhs;
	    a = myA*rhs;
            return Color(r,g,b,a);
        }

        //----------------------------------------------------------------------
        // Getters
        
        /*
         * returns the R(red) value of the color
         */
        float getR() const 
        {
            return myR;
        }

        /*
         * returns the G(green) value of the color
         */
        float getG() const
        {
            return myG;
        }
        
        /*
         * returns the B(blue) value of the Color
         */
        float getB() const
        {
            return myB;	
        }
		
        /*
         * returns the A(alpha) value of the Color
         */
        float getA() const
        {
            return myA;
        }


        //----------------------------------------------------------------------
        // Setters

        /*
         * Sets the R(red) value of the color
         */
        void setR(float r)
        {
            myR = r;
        }

        /*
         * Sets the G(green) value of the color
         */
        void setG(float g)
        {
            myG = g;	
        }
		
        /*
         * Sets the B(blue) value of the color
         */
        void setB(float b)
        {
            myB = b;	
        }
		
        /*
         * Sets the A(alpha) value of the color
         */
        void setA(float a)
        {
            myA = a;	
        }
    };
}


#endif
