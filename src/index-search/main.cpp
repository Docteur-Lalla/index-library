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
#include "../index-core/color.h"

// Function to initialize the option system.
Option init_option_system(int argc, char* argv[])
{
	// Creation of the classic option table.
	std::vector<std::string> vec = vector_of_array(argc, argv);
	std::map<std::string, bool> table(classic_option_table());

	// Addition of the specific options of index-search.
	// The input of the search.
	table["--input"] = true;
	table["-i"] = true;

	// If we have only one entry, we execute a command on it.
	table["--exec"] = true;
	table["-e"] = true;

	// Execute a command on each entry set in output.
	table["--execlist"] = true;
	table["-E"] = true;

	Option opt(vec);
	opt.parse(table);

	return opt;
}

int main(int argc, char* argv[])
{
	try
	{
		Option opt(init_option_system(argc, argv));

		// Check the version option and the help option.
		if(opt.isset("--version") || opt.isset("-V"))
			print_color("index-search 0.1 alpha, part of index-library 0.1 alpha.\n");
		else if(opt.isset("--help") || opt.isset("-h"))
			print_color("Type 'man index-search' in your terminal to get helped.");
		else
			search(opt);
	}

	catch(const std::string& str)
	{
		print_color(str + '\n', RED, DEFAULT, true);
	}

	return 0;
}
