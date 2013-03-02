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

#include "search.h"
#include "sort.h"

#include "../index-core/color.h"

#include <algorithm>

namespace
{
	inline bool isset_exec(Option opt)
	{
		if(opt.isset("--exec"))
			return true;
		else if(opt.isset("-e"))
			return true;
		else if(opt.isset("--execlist"))
			return true;
		else if(opt.isset("-E"))
			return true;

		return false;
	}

	// A little function the get the tags list.
	index_tags get_tags(Option opt)
	{
		index_tags tags;

		if(opt.isset("--tags"))
			tags = tags_of_string(opt.get("--tags"));
		else if(opt.isset("-t"))
			tags = tags_of_string(opt.get("-t"));
		else
			throw std::string("lack of tag list to sort entries.");
	
		return tags;
	}

	// Procedure to purge the entries list by author's name and file's extension.
	void purge_by_author_and_extension(std::map<unsigned int, index_entry>& entries, Option opt)
	{
		// Purge by author.
		if(opt.isset("--by"))
			purge_list_from_author(entries, opt.get("--by"));
		else if(opt.isset("-b"))
			purge_list_from_author(entries, opt.get("-b"));

		// Purge by extension.
		if(opt.isset("--extension"))
			purge_list_from_extension(entries, opt.get("--extension"));
		else if(opt.isset("-x"))
			purge_list_from_extension(entries, opt.get("-x"));
	}

	// Print the entries with color.
	void print_entry(std::pair<unsigned int, index_entry> pair)
	{
		index_entry entry = pair.second;

		std::string title = std::string("\"") + entry.title + '"';
		std::string author = std::string("\"") + entry.author + '"';
		std::string filetype = std::string("(") + entry.filetype + ')';

		print_color(entry_name_of_uint(entry.id) + ' ', GREEN);
		print_color(title + ' ', PURPLE, DEFAULT, true);
		print_color(filetype + ' ', PURPLE, DEFAULT, true);
		print_color(author + ' ', CYAN);
		print_color(entry.date + '\n', GREY);
	}

	// Print the entries with color and full sentences.
	void print_verbose_entry(std::pair<unsigned int, index_entry> pair)
	{
		index_entry entry = pair.second;

		std::string title = std::string("\"") + entry.title + '"';
		std::string author = std::string("\"") + entry.author + '"';
		std::string filetype = std::string("(") + entry.filetype + ')';

		print_color(entry_name_of_uint(entry.id) + ' ', GREEN);
		print_color("titled ");
		print_color(title + ' ', PURPLE, DEFAULT, true);
		print_color(filetype + ' ', PURPLE, DEFAULT, true);
		print_color("by ");
		print_color(author + ' ', CYAN);
		print_color("created the ");
		print_color(entry.date + '\n', GREY);
	}
}

void search(Option opt)
{
	std::map<unsigned int, index_entry> entries;
	index_tags tags = get_tags(opt);
	
	create_list_from_tag_list(entries, tags);

	// If an author or an extension if looked for, we must purge the list.
	purge_by_author_and_extension(entries, opt);

	// The input is the most important, we must now filter it.
	std::string input;

	if(opt.isset("--input"))
		input = opt.get("--input");
	else if(opt.isset("-i"))
		input = opt.get("-i");
	
	std::list<std::string> filter = parse_input_list(input);
	purge_list_from_input(entries, filter);

	// We have now 2 possibilities : display the results or execute a command on them.
	// Checks in the option system if we must execute a command.
	if(isset_exec(opt))
	{
		std::string exec;

		if(opt.isset("--exec"))
			exec = opt.get("--exec");
		else if(opt.isset("-e"))
			exec = opt.get("-e");

		// Execute a command on the only one result.
		if(entries.size() == 1)
			execute_command(entries.begin()->first, exec);
		else
		{
			std::string execlist;

			if(opt.isset("--execlist"))
				execlist = opt.get("--execlist");
			else if(opt.isset("-E"))
				execlist = opt.get("-E");

			// Execute a command on each result.
			execute_list_command(entries, exec);
		}
	}

	else
	{
		if(opt.isset("--verbose") || opt.isset("-v"))
			std::for_each(entries.begin(), entries.end(), print_verbose_entry);
		else
			std::for_each(entries.begin(), entries.end(), print_entry);
	}
}

