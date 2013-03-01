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

#include "sort.h"
#include "../index-core/color.h"
#include "../index-core/local.h"

#include <fstream>

namespace
{
	void parse_string(const std::string& line, std::string& out, std::string::iterator& it)
	{
		for(; it != line.end() && *it != '"'; it++)
			out += *it;

		it++;
	}

	index_entry parse_tag_line(std::string line)
	{
		// We first get the ID.

		std::string strid = line.substr(0, line.find_first_of(' '));

		if(strid.length() != 8)
			throw std::string("invalid entry ID : ") + strid + '.';

		unsigned int id = std::stoul(strid, nullptr, 16);
		
		line = line.substr(9);
		// We are searching the title (that is between quotes).

		std::string title;
		auto it = line.begin();

		it++;

		parse_string(line, title, it);

		// The title is ready, we can get the extension.
		if(*it == ' ')
			it++;

		line.erase(line.begin(), it);

		it = line.begin();
		it++;

		std::string ext("");
		parse_string(line, ext, it);

		// Then, we get the author.

		std::string author("");

		if(*it == ' ')
			it++;

		it++;

		parse_string(line, author, it);
		it++;

		// Finally, we get the date !

		line.erase(line.begin(), it);
		std::string date(line);

		index_entry entry;
		entry.id = id;

		entry.filetype = ext;
		entry.author = author;
		entry.title = title;
		entry.date = date;

		return entry;
	}
}

std::list<std::string> parse_input_list(const std::string& input)
{
	std::list<std::string> ret;
	std::string current;

	for(auto it = input.begin(); it != input.end(); it++)
		if(*it == ' ' && !current.empty())
		{
			ret.push_back(current);
			current.clear();
		}

		else
			current += *it;
	
	if(!current.empty())
		ret.push_back(current);
	
	return ret;
}

// We take every entries from tag files.
void create_list_from_tag_list(std::map<int, index_entry>& entries, const index_tags& tags)
{
	index_local local = generate_local();

	for(auto name : tags)
	{
		std::ifstream file((local.tags + '/' + name).c_str());
		
		if(!file)
			throw std::string("unfound tag file ") + name + '.';

		std::string line;

		while(getline(file, line))
		{
			index_entry entry = parse_tag_line(line);

			// If the entry already exists, we just complete its tag list.
			if(entries.find(entry.id) == entries.end())
				entries[entry.id] = entry;
			
			entries[entry.id].tags.push_back(name);
		}
	}
}

// We pay attention to the author.
void purge_list_from_author(std::map<int, index_entry>& entries, const std::string& author)
{
	for(auto it = entries.begin(); it != entries.end(); it++)
		if(it->second.author != author)
			entries.erase(it);
}

// We pay attention to the extension.
void purge_list_from_extension(std::map<int, index_entry>& entries, const std::string& ext)
{
	for(auto it = entries.begin(); it != entries.end(); it++)
		if(it->second.filetype != ext)
			entries.erase(it);
}

// We pay attention to the input.
void purge_list_from_input(std::map<int, index_entry>& entries, const std::list<std::string>& input)
{
	for(auto key : input)
	{
		bool neg = true;

		if(key.front() == '-')
		{
			neg = false;
			key.erase(key.begin());
		}

		for(auto it = entries.begin(); it != entries.end(); it++)
			if(it->second.title.find(key) == std::string::npos == neg)
				entries.erase(it);
	}
}

