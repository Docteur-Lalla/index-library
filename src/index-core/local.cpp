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

#include "local.h"
#include <fstream>

#include "color.h"

namespace
{
	// Each valid line is a command. It begins with a name.
	std::string get_command_name(const std::string& str)
	{
		std::string cmd;

		for(auto it = str.begin(); *it != '='; it++)
			cmd += *it;

		return cmd;
	}

	// Each command has a value.
	std::string get_command_value(const std::string& str)
	{
		std::string::const_iterator it = str.begin();
		std::string value;

		for(; *it != '='; it++);
		it++;

		// We are now in the right place to get the command value.
		for(; it != str.end(); it++)
			value += *it;

		return value;
	}

	// This procedure makes the difference between commentaries, empty lines and commands.
	void analyze_line(index_local& local, std::string line)
	{
		if(line.front() == '#') // commentary
			return;
		else if(line.empty()) // empty line
			return;

		// Commands

		std::string cmd = get_command_name(line);

		if(cmd == "MAIN")
			local.main = get_command_value(line);
		else if(cmd == "TAGS")
			local.tags = get_command_value(line);
		else if(cmd == "ENTRY")
			local.entries = get_command_value(line);
		//else
			//local.cmds[cmd] = get_command_value(line);
	}

}

// Gets informations from index.conf.
index_local generate_local()
{
	std::string index_conf("/etc/index.conf");
	std::ifstream file(index_conf.c_str());

	index_local local;

	if(file)
	{
		std::string line;

		while(getline(file, line))
			analyze_line(local, line);
	}

	else
		throw std::string("unfound /etc/index.conf.");

	return local;
}

