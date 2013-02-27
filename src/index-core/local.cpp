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

namespace
{
	void analyze_line(index_local& local, std::string line)
	{
		if(line.front() == '#')
			return;

		std::string cmd = line.substr(0, line.find_first_of('='));

		if(cmd == "MAIN")
			local.main = line.substr(line.find_first_of('=') + 1);
		else if(cmd == "TAGS")
			local.tags = line.substr(line.find_first_of('=') + 1);
		else if(cmd == "ENTRY")
			local.entries = line.substr(line.find_first_of('=') + 1);
		else
			local.cmds[cmd] = line.substr(line.find_first_of('=') + 1);
	}

}

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
