#This is an example of parts of an RPM spec file for the Ember client media package.
#Only the most basic data is provided; it's mainly meant as a skeleton for new distros that want
#to provide Ember to start from. 
%define name @PACKAGE@-media
%define version @VERSION@
%define release distro_specific

Summary: Media files needed for the Ember virtual worlds client
Name: %{name}
Version: %{version}
Release: %{release}
Copyright: GPL
Source0: %{name}-%{version}.tar.bz2
URL: http://www.worldforge.org/dev/eng/clients/ember
Group: Amusements/Games
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Prefix: %{_prefix}
BuildArchitectures: noarch
Requires: distro_specific
BuildRequires: distro_specific

%description
Ember is a client for virtual worlds using the Worldforge system.
It can be used both for building as well as exploring virtual worlds.
This is the media for use with Ember.

distro_specific_build_directives_should_go_here
