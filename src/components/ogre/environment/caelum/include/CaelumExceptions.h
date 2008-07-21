/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2006-2007 Caelum team. See Contributors.txt for details.

Caelum is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Caelum is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Caelum. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CAELUMEXCEPTIONS_H
#define CAELUMEXCEPTIONS_H

#include "CaelumPrerequisites.h"

namespace caelum {

    /** Exception class for unsupported features.
     * @author Jesús Alonso Abad.
     */
    class CAELUM_EXPORT UnsupportedException : public Ogre::Exception {
	    public:
		    /// Constructor.
		    UnsupportedException (int number, const Ogre::String &description, const Ogre::String &source, const char *file, long line)
						    : Ogre::Exception (number, description, source, "UnsupportedException", file, line) { }
    };

#define CAELUM_THROW_UNSUPPORED_EXCEPTION(desc, src) \
        throw UnsupportedException(-1, (desc), (src), __FILE__, __LINE__);

} // namespace caelum

#endif // CAELUMEXCEPTIONS_H
