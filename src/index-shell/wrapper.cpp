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

#include "wrapper.h"

int lua_print_color(lua_State* lua)
{
	int argc = lua_gettop(lua);
	int fore;
	int back;

	switch(argc)
	{
		case 1:
			print_color(lua_tostring(lua, 1));
			break;
		case 2:
			fore = lua_tointeger(lua, 2);
			print_color(lua_tostring(lua, 1), color_of_int(fore));
			break;
		case 3:
			fore = lua_tointeger(lua, 2);
			back = lua_tointeger(lua, 3);
			print_color(lua_tostring(lua, 1), color_of_int(fore), color_of_int(back));
			break;
		case 4:
			fore = lua_tointeger(lua, 2);
			back = lua_tointeger(lua, 3);
			print_color(lua_tostring(lua, 1), color_of_int(fore), color_of_int(back), lua_toboolean(lua, 4));
			break;
		default:
			break;
	}

	return 0;
}

