/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

 Header:       FGXMLParse.cpp
 Author:       Jon Berndt
 Date started: 08/20/2004
 Purpose:      Config file read-in class and XML parser
 Called by:    Various

 ------------- Copyright (C) 2001  Jon S. Berndt (jon@jsbsim.org) -------------

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation; either version 2 of the License, or (at your option) any
 later version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 details.

 You should have received a copy of the GNU Lesser General Public License along
 with this program; if not, write to the Free Software Foundation, Inc., 59
 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 Further information about the GNU Lesser General Public License can also be
 found on the world wide web at http://www.gnu.org.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include <string>
#include <iostream>
#include <cstdlib>

#include "FGJSBBase.h"
#include "FGXMLParse.h"
#include "FGXMLElement.h"
#include "input_output/string_utilities.h"

using namespace std;

namespace JSBSim {

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

using namespace std;

FGXMLParse::FGXMLParse(void)
{
  current_element = document = nullptr;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void FGXMLParse::startXML(void)
{
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void FGXMLParse::reset(void)
{
  current_element = document = nullptr;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void FGXMLParse::endXML(void)
{
  // At this point, document should equal current_element ?
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void FGXMLParse::dumpDataLines(void)
{
  if (!working_string.empty()) {
    for (auto s: split(working_string, '\n'))
      current_element->AddData(s);
  }
  working_string.erase();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void FGXMLParse::startElement (const char * name, const XMLAttributes &atts)
{
  string Name(name);
  Element *temp_element;

  if (!document) {
    document = new Element(Name);
    current_element = document;
  } else {
    dumpDataLines();

    temp_element = new Element(Name);
    temp_element->SetParent(current_element);
    current_element->AddChildElement(temp_element);
    current_element = temp_element;
  }

  if (current_element == 0L) {
    cerr << "In file " << getPath() << ": line " << getLine() << endl
         << "No current element read (running out of memory?)" << endl;
    exit (-1);
  }

  current_element->SetLineNumber(getLine());
  current_element->SetFileName(getPath());

  for (int i=0; i<atts.size();i++) {
    current_element->AddAttribute(atts.getName(i), atts.getValue(i));
  }
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void FGXMLParse::endElement (const char * name)
{
  dumpDataLines();
  current_element = current_element->GetParent();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void FGXMLParse::data (const char * s, int length)
{
  working_string += string(s, length);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void FGXMLParse::pi (const char * target, const char * data)
{
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void FGXMLParse::warning (const char * message, int line, int column)
{
  cerr << "Warning: " << message << " line: " << line << " column: " << column << endl;
}

} // end namespace JSBSim
