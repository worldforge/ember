#!/bin/sh


echo
echo "***"
echo "*** CppUnit is not installed!"
echo "***"
echo "*** Not running unit tests..."
echo "***"
echo

# Fail gracefully so we can still make tarballs, even if we don't
# have CppUnit installed.
exit 0
