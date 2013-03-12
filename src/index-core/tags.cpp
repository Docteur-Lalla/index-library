/*
 * Copyright (c) 2013, Kévin Le Bon
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Kévin Le Bon nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "tags.h"
#include "color.h"

// Takes the tag list and create a long string from the concatenation of each tag.
std::string string_of_tags(const std::list<std::string>& tags)
{
	std::string ret("");

	for(auto it = tags.begin(); it != tags.end(); it++)
		ret += *it + '/';
	
	return ret;
}

// Takes a string and split it into a tag list.
std::list<std::string> tags_of_string(const std::string& str)
{
	std::list<std::string> ret;
	
	if(!str.empty())
	{
		std::string current;

		for(auto it = str.begin(); it != str.end(); it++)
		{
			// We build the tag until we get a '/'.
			if(*it != '/')
				current += *it;
			else
			{
				ret.push_back(current);
				current.clear();
			}
		}

		// If the string is not '/' terminated, we missed the last tag.
		if(str.back() != '/')
			ret.push_back(current);
	}
	
	return ret;
}

