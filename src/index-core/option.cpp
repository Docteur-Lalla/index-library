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

#include "option.h"

Option::Option(const std::vector<std::string>& line) : code(line)
{
}

void Option::parse(const std::map<std::string, bool>& table)
{
	std::map<std::string, bool> tab(table);

	for(auto it = code.begin(); it != code.end(); it++)
	{
		if(tab.find(*it) != tab.end())
		{
			if(tab[(*it)])
			{
				std::string key = *it;
				it++;

				if(it == code.end())
					throw std::string("option ") + key + " expected an argument.";

				opts[key] = *it;
			}

			else
				opts[*it] = "set";
		}

		else
			throw std::string("unknown option ") + *it + ".";
	}
}

// Returns true if the key exists.
bool Option::isset(const std::string& option) const
{
	for(auto it = opts.begin(); it != opts.end(); it++)
		if(it->first == option)
			return true;
	
	return false;
}

// Returns the value associed with the key.
std::string Option::get(const std::string& option)
{
	return opts[option];
}

// Returns the common option table of each index program.
std::map<std::string, bool> classic_option_table()
{
	std::map<std::string, bool> table;

	table["--tags"] = true;
	table["-t"] = true;

	table["--title"] = true;
	table["-T"] = true;

	table["--by"] = true;
	table["-b"] = true;

	table["--help"] = false;
	table["-h"] = false;

	table["--version"] = false;
	table["-V"] = false;

	table["--verbose"] = false;
	table["-v"] = false;

	return table;
}

std::vector<std::string> vector_of_array(int argc, char *argv[])
{
	std::vector<std::string> vec;

	for(int i = 1; i < argc; i++)
		vec.push_back(argv[i]);
	
	return vec;
}

