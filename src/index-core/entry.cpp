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

#include "entry.h"

#include <sstream>
#include <iomanip>

#include <fstream>

// Creates the entry ID of string type and format it with the following :
//   - 8 hexadecimals digits
//   - letters in uppercase
std::string entry_name_of_uint(unsigned int id)
{
	std::stringstream ss;
	ss << std::setbase(16) << id;

	std::string ret(ss.str());

	while(ret.length() < 8)
		ret.insert(0, "0");
	
	for(auto it = ret.begin(); it != ret.end(); it++)
		if(*it >= 'a' && *it <= 'f')
			*it -= 'a' - 'A';
	
	return ret;
}

// Add the entry in the tag file.
// Each entry follow this syntax : ID "title" "filetype" "author" D/M/Y
void add_entry_in_tag(const index_entry& entry, const std::string& tag)
{
	std::ofstream file(tag.c_str(), std::ios::app);

	file << entry_name_of_uint(entry.id);
	file << " \"" << entry.title << "\" \"" << entry.filetype << "\" \"" << entry.author << "\" ";
	file << entry.date;
}

