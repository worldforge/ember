#This is an example of parts of an RPM spec file for the Ember client.
#Only the most basic data is provided; it's mainly meant as a skeleton for new distros that want
#to provide Ember to start from. 
%define name @PACKAGE@
%define version @VERSION@
%define release distro_specific

Summary: A virtual worlds client for Worldforge
Name: %{name}
Version: %{version}
Release: %{release}
License: GPLv3+
Source0: %{name}-%{version}.tar.gz
URL: http://www.worldforge.org/dev/eng/clients/ember
Group: Amusements/Games
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Prefix: %{_prefix}
Requires: distro_specific
BuildRequires: distro_specific

%description
Ember is a client for virtual worlds using the Worldforge system.
It can be used both for building as well as exploring virtual worlds.

distro_specific_build_directives_should_go_here
