#!/usr/bin/perl -w

print "Please enter your name: ";
$realName = <>;
chomp $realName;
print "Please enter your handle: ";
$nickName = <>;
chomp $nickName;

print "Please Enter the Class Name: ";
$className = <>;
chomp $className;
my %myMembers;
my $curMemName;
my $curMemType;

print "Please enter a Member name for this class(enter 'xxx' to quit) ";
$curMemName = <>;
chomp $curMemName;
while($curMemName ne "xxx")
{
    print "please Enter the Type for memeber: $curMemName: ";
    $curMemType = <>;
    chomp $curMemType;
    
    $myMembers{$curMemName} = $curMemType;
    
    print "Please enter a Member name for this class(enter 'xxx' to quit) ";
    $curMemName = <>;
    chomp $curMemName;

}
open(HEADER, ">$className".".h");

print HEADER <<EOF;
/*
 Copyright (C) 2002  $realName
	
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

EOF
    
    
print HEADER "#ifndef " . uc $className . "_H\n";
print HEADER "#define " . uc $className . "_H\n";
print HEADER <<EOF;

// Included headers from the current project

// Included custom library headers

// Included system headers

namespace dime {

/**
 * Short sentence with a general description of the class, ending in period.
 *
 * More detailed description of the class, it's purpose, what it does,
 * how to use it and so on.
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * \@author $realName aka $nickName
 *
 * NOTE: You can also specify the author for individual methods
 * if different persons have created them.
 * It is also possible to have multiple \@author tags for a method.
 * Only add yourself as an \@author if you have done serious work
 * on a class/method, and can help fixing bugs in it, etc.
 * If you just fixed a bug or added a short code snipplet you
 * don't need to add yourself.
 *
 * \@see OtherSubsystem::AnOtherRelatedClass
 *
 * NOTE: Add other related classes here, doxygen will create links to them.
 */

class $className
{
    //======================================================================
    // Inner Classes, Typedefs, and Enums
    //======================================================================
    public:


    //======================================================================
    // Public Constants
    //======================================================================
    public:


    //======================================================================
    // Private Constants
    //======================================================================
    private:


    //======================================================================
    // Private Variables
    //======================================================================/
    private:

EOF

foreach $key (keys %myMembers)
{
    print HEADER "        $myMembers{$key} my"."$key;\n";
}

print HEADER <<EOF;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new $className using default values.
     */
     $className()
     {
     }

    /**
     * Copy constructor.
     */
    $className( const $className &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    $className &operator= ( const $className &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a $className instance.
     */
    virtual ~$className ()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    // Example of a getter method:

EOF

foreach $key (keys %myMembers)
{
    print HEADER "    /**\n";
    print HEADER "     * Gets the value of $key of this $className\n";
    print HEADER "     */\n";
    print HEADER "    $myMembers{$key} get"."$key()\n    {\n        return my"."$key".";\n    }\n\n";
}

print HEADER <<EOF;

    //----------------------------------------------------------------------
    // Setters

EOF
foreach $key (keys %myMembers)
{
    print HEADER "    /**\n";
    print HEADER "     * Sets the value of $key of this $className\n";
    print HEADER "     */\n";
    print HEADER "    void set"."$key( $myMembers{$key} )\n    {\n        my"."$key"." = $key;\n    }\n\n";
}
print HEADER <<EOF;

    //----------------------------------------------------------------------
    // Other public methods
    // NOTE: Group related public methods together and crate a separator comment like above for them.
    
    //======================================================================
    // Protected Methods
    //======================================================================
    protected:


    //======================================================================
    // Private Methods
    //======================================================================
    private:


    //======================================================================
    // Disabled constructors and operators
    //======================================================================
    private:


}; // End of $className

} // End of dime namespace

#endif
EOF

