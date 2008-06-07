#! /bin/sh
tolua++ -n atlas_adapters bindings.pkg | sed -e 's/const,/const /g' > bindings.cpp