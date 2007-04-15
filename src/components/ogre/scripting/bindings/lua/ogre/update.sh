#! /bin/sh
tolua++ -n Ogre Ogre.pkg | sed -e 's/const,/const /g' > lua_Ogre.cpp