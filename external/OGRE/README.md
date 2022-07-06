There's an issue with Ogre 13.4.1 and SOL, in that the Ogre::Any class exposes a ctor
which allows for any other type.
This doesn't play well with SOL, which we use for our Lua bindings, since it will follow
that ctor when doing type lookups, and assume that all types also can be converted to Ogre::Any.
This is bad, because it both adds bloat as well as compiler errors on some compilers (MSVC)
when it tries to generate type copy code as a result.

To avoid this we've provided our own version of OgreAny.h, copied from the Ogre 13.4.1 source.
The main change we've made is that the Ogre::Any conversion ctor now is "explicit", which makes
it inaccessible from SOL. Our version of the source needs to be included first in any Lua bindings
code.