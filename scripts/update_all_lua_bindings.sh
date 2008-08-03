#!/bin/sh
(cd ../src/main/bindings/lua; ./update.sh)
(cd ../src/framework/bindings/lua; ./update.sh)
(cd ../src/components/ogre/scripting/bindings/lua; ./update.sh)
(cd ../src/components/ogre/widgets/adapters/atlas/bindings/lua; ./update.sh)
(cd ../src/services/bindings/lua; ./update.sh)
