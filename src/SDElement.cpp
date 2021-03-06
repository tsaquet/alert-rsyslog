/**
 * @file SDElement.cpp
 * @author Thomas Saquet, Florent Poinsaut
 * @date 
 * @brief File containing example of doxygen usage for quick reference.
 *
 * Alert - rsyslog is a part of the Alert software
 * Copyright (C) 2013-2017
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 * 
 */

#include "SDElement.h"

using namespace std;

SDElement::SDElement(const string &content)
{
    setContent(content);

    if (_content.compare(""))
    {
        detectSDID();
        if (_sdParamsString.compare(""))
            splitSDParams();
    }
    else
        logger.entry("error") << "[SDElement] Content is empty";
}

SDElement::SDElement(const SDElement& orig)
{
    setContent(orig.getContent());
    setSDID(orig.getSDID());
    setSDParam(orig.getSDParams());
    setSDParamsString(orig.getSDParamsString());
}

SDElement::~SDElement()
{
}

void SDElement::setContent(string content)
{
    _content = content;

    return;
}

string SDElement::getContent() const
{
    return _content;
}

void SDElement::detectSDID()
{
    if (_content.compare(""))
    {
        boost::regex e("^(\\w+)@(\\d+) (.*)");
        boost::smatch what;

        if (boost::regex_match(_content, what, e, boost::match_extra))
        {
            if (what.size() == 4)
            {
                try
                {
                    setSDID(SDID(what[1], boost::lexical_cast<unsigned>(what[2])));
                }
                catch (boost::bad_lexical_cast &)
                {
                    logger.entry("error") << "[SDElement] PEN is not an unsigned on SD-Element Prop";
                }
                setSDParamsString(what[3]);
            }
            else
                logger.entry("error") << "[SDElement] Bad number of elements on SD-Element Prop";
        }
        else
            logger.entry("error") << "[SDElement] No Match found";
    }
    else
        logger.entry("error") << "[SDElement] No SD-Element Prop found";

    return;
}

void SDElement::setSDID(SDID sdID)
{
    _sdID = sdID;

    return;
}

SDID SDElement::getSDID() const
{
    return *_sdID;
}

void SDElement::setSDParamsString(string sdParamsString)
{
    _sdParamsString = sdParamsString;
    return;
}

string SDElement::getSDParamsString() const
{
    return _sdParamsString;
}

void SDElement::splitSDParams()
{
    vector<string> sSDParams;

    // nouveau 
    // offset=2 lotNum=12 lineNum=7 22="URL_Encode(data)"
    
    // ancien  
    // offset=2 4-1-3-4-1-1-1="U3VjaCBJbnN0YW5jZSBjdXJyZW50bHkgZXhpc3RzIGF0IHRoaXMgT0lE" 4-1-3-4-2-1-1="U3VjaCBJbnN0YW5jZSBjdXJyZW50bHkgZXhpc3RzIGF0IHRoaXMgT0lE"

    boost::split(sSDParams, _sdParamsString, boost::is_any_of(" "), boost::token_compress_on);
    // nouveau
    // offset=2 
    // lotNum=12 
    // lineNum=7 
    // 22="URL_Encode(data)"
    // 24="URL_Encode(data)"
    
    // ancien
    // offset=2
    // 4-1-3-4-1-1-1="U3VjaCBJbnN0YW5jZSBjdXJyZW50bHkgZXhpc3RzIGF0IHRoaXMgT0lE"
    // 4-1-3-4-2-1-1="U3VjaCBJbnN0YW5jZSBjdXJyZW50bHkgZXhpc3RzIGF0IHRoaXMgT0lE"

    for (unsigned i(0); i < sSDParams.size(); ++i)
    {
        addSDParam(SDParam(sSDParams[i]));
    }

    return;
}

void SDElement::setSDParam(vector<SDParam> sdParams)
{
    _sdParams = sdParams;

    return;
}


void SDElement::addSDParam(const SDParam &sdParam)
{
    _sdParams.push_back(sdParam);
    return;
}

vector<SDParam> SDElement::getSDParams() const
{
    return _sdParams;
}
