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

#include "add.h"

#include "../index-core/entry.h"
#include "../index-core/color.h"

#include <time.h>
#include <fstream>
#include <sstream>

namespace
{
	void print_added(const index_entry& entry)
	{
		print_color("Added entry ");
		print_color(std::string("\"") + entry.title + "\" ", PURPLE, DEFAULT, true);
		print_color(std::string("(") + entry.filetype + ") ", PURPLE, DEFAULT, true);
		print_color("by ");
		print_color(entry.author, CYAN);
		print_color(" in ");
		print_color(string_of_tags(entry.tags), GREEN);
		print_color(" the ");
		print_color(entry.date, GREY);
		print_color(".\n");
	}

	// A simple function to get the title of the entry.
	std::string get_title(Option opt)
	{
		std::string title;

		if(opt.isset("--title"))
			title = opt.get("--title");
		else if(opt.isset("-T"))
			title = opt.get("-T");
		else
			throw std::string("unspecified title for the entry.");

		return title;
	}

	// A simple function to get the author of the entry.
	std::string get_author(Option opt)
	{
		std::string author;

		if(opt.isset("--by"))
			author = opt.get("--by");
		else if(opt.isset("-b"))
			author = opt.get("-b");
		else
			author = "Someone";
	
		return author;
	}

	// A simple function to get the extension of the entry.
	std::string get_extension(Option opt)
	{
		std::string extension;

		if(opt.isset("--extension"))
			extension = opt.get("--extension");
		else if(opt.isset("-x"))
			extension = opt.get("-x");
		else
			extension = "txt";
	
		return extension;
	}

	// A simple function to get the tags list of the entry.
	index_tags get_tags(Option opt)
	{
		index_tags tags;

		if(opt.isset("--tags"))
			tags = tags_of_string(opt.get("--tags"));
		else if(opt.isset("-t"))
			tags = tags_of_string(opt.get("-t"));
		else
			throw std::string("no tag list specified to create the entry.");
	
		return tags;
	}

	// A function to get a formatted date of creation of the entry.
	std::string get_date()
	{
		time_t timer;

		tm* d;

		time(&timer);
		d = localtime(&timer);

		std::stringstream ss;
		ss << d->tm_mday << '/' << d->tm_mon + 1 << '/' << d->tm_year + 1900;
		return ss.str();
	}

	// A simple function to get the filename of the document to put in the library.
	std::string get_filename(Option opt)
	{
		std::string filename;

		if(opt.isset("--file"))
			filename = opt.get("--file");
		else if(opt.isset("-f"))
			filename = opt.get("-f");
		else
			throw std::string("no file to put in the library.");

		return filename;
	}

	void write_entry(const index_local& local, const std::string& filename, unsigned int id)
	{
		// We put the content of the file into the new entry in the entries directory.

		std::string outname = local.entries + '/' + entry_name_of_uint(id);

		std::ofstream out(outname.c_str(), std::ios::trunc);
		std::ifstream in(filename.c_str());

		std::string line;

		while(getline(in, line))
			out << line << std::endl;
	}
}

void add(Option opt)
{
	index_local local = generate_local();

	// This function must get the following information and create the entry with them :
	//   - Entry title
	//   - Entry author
	//   - Entry file type
	//   - Date of creation
	//   - Entry's tags
	//   - Entry's ID

	// First, the ID. We create IDs from 0 to the max. If it already exists, we test the following.

	unsigned int id = entry_count(local);

	// Then, the title, the extension and the author. The option system must have them.

	std::string title(get_title(opt));
	std::string author(get_author(opt));
	std::string extension(get_extension(opt));

	// Finally, we must get the date and the tag list.
	// The option system has the tag list and the date can be easily created.

	index_tags tags = get_tags(opt);
	std::string date(get_date());

	// We create the entry.

	index_entry entry;
	entry.id = id;
	entry.title = title;
	entry.filetype = extension;
	entry.author = author;
	entry.tags = tags;
	entry.date = date;

	std::string filename(get_filename(opt));

	// We create the entry file in the entries directory.
	write_entry(local, filename, id);

	// We add a line at each tag file specified.
	for(auto it = tags.begin(); it != tags.end(); it++)
		add_entry_in_tag(entry, local.tags + '/' + *it);
	
	// If the verbose option is set, let's add a sentence.

	if(opt.isset("--verbose") || opt.isset("-v"))
		print_added(entry);
}

