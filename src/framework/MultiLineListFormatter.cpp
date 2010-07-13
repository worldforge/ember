// Copyright (C) 2009 Alistair Riddoch
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#include "MultiLineListFormatter.h"

#include <iostream>

namespace Ember
{

MultiLineListFormatter::MultiLineListFormatter(std::iostream & s, Atlas::Bridge & b) :
	Atlas::Formatter(s, b)
{
}

void MultiLineListFormatter::mapListItem(const std::string & name)
{
	Atlas::Formatter::mapListItem(name);
	m_indent += m_spacing;
	m_stream << std::endl;
}

void MultiLineListFormatter::listMapItem()
{
	m_stream << std::string(m_indent, ' ');
	Atlas::Formatter::listMapItem();
	m_stream << std::endl;
}

void MultiLineListFormatter::listListItem()
{
	m_stream << std::string(m_indent, ' ');
	Atlas::Formatter::listListItem();
	m_indent += m_spacing;
	m_stream << std::endl;
}

void MultiLineListFormatter::listIntItem(long l)
{
	m_stream << std::string(m_indent, ' ');
	Atlas::Formatter::listIntItem(l);
	m_stream << std::endl;
}

void MultiLineListFormatter::listFloatItem(double d)
{
	m_stream << std::string(m_indent, ' ');
	Atlas::Formatter::listFloatItem(d);
	m_stream << std::endl;
}

void MultiLineListFormatter::listStringItem(const std::string&s)
{
	m_stream << std::string(m_indent, ' ');
	Atlas::Formatter::listStringItem(s);
	m_stream << std::endl;
}

void MultiLineListFormatter::listEnd()
{
	m_indent -= m_spacing;
	m_stream << std::string(m_indent, ' ');
	Atlas::Formatter::listEnd();
}
}
