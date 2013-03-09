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

#include "remove.h"
#include "../index-core/local.h"

#include <cstdio>
#include <fstream>
#include <dirent.h>

// Search the entry ID in the tag file and returns -1 if it did not find it else, it return the line count.
int search_in_file(const std::string& id, const std::string& filename)
{
	std::string line;
	std::ifstream file(filename.c_str());

	int count = 0;

	while(getline(file, line))
	{
		std::string ID(line.substr(0, 8));

		if(id == ID)
			return count;
		
		count++;
	}

	return -1;
}

// Rewrite the tag file without the removed line.
void transfert_file(const std::string& infile, int count)
{
	std::string content;
	std::string line;

	std::ifstream file(infile.c_str());

	int c = 0;

	while(getline(file, line))
	{
		if(count != c)
			content += line + '\n';
	
		c++;
	}

	file.close();

	std::ofstream out(infile.c_str(), std::ios::trunc);

	out << content;
}

// Remove specified entry in the entries directory.
void remove_entry(const std::string& ID, const index_local& local)
{
	std::ifstream file((local.entries + '/' + ID).c_str());

	if(file)
	{
		file.close();
		remove((local.entries + '/' + ID).c_str());
	}

	else
		throw std::string("unfound entry #") + ID + '.';
}

void index_remove(Option opt)
{
	std::string ID;

	if(opt.isset("--entry"))
		ID = opt.get("--entry");
	else if(opt.isset("-n"))
		ID = opt.get("-n");
	else
		throw std::string("entry ID unspecified and needed to remove the entry.");
	
	index_local local = generate_local();

	// Remove the file in the entries' directory.
	remove_entry(ID, local);

	DIR* tags = opendir(local.tags.c_str());
	dirent* ent;

	// Verbose mode.
	if(opt.isset("--verbose") || opt.isset("-v"))
	{
		print_color("removed entry ");
		print_color(ID, GREEN);
		print_color(" from tag files ");
	}
	
	while(ent = readdir(tags))
	{
		if(ent->d_name == "." || ent->d_name == "..")
			continue;

		else // We must test each tag file to search a sign of the specified entry.
		{
			int count = search_in_file(ID, local.tags + '/' + ent->d_name);
			
			if(count > -1)
			{
				transfert_file(local.tags + '/' + ent->d_name, count);

				if(opt.isset("--verbose") || opt.isset("-v"))
					print_color(std::string(ent->d_name) + '/', CYAN);
			}
		}
	}

	if(opt.isset("--verbose") || opt.isset("-v"))
		print_color(".\n");
}

