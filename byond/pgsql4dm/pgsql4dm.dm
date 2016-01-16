/*
    pgsql4dm: PostgreSQL database operations for BYOND worlds
    Copyright (C) 2015  NullQuery (http://www.byond.com/members/NullQuery)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Override these to provide alternative locations to the shared library.

#ifndef LIBPGSQL4DM_DLL_WIN32
#define LIBPGSQL4DM_DLL_WIN32 "./libpgsql4dm.dll"
#endif

#ifndef LIBPGSQL4DM_DLL_UNIX
#define LIBPGSQL4DM_DLL_UNIX "./libpgsql4dm.so"
#endif

#define LIBPGSQL4DM_TRACKER_URL "http://www.byond.com/forum/?forum=146927"

var/pgsql4dm/_global/pgsql4dm = new

/pgsql4dm/_global/proc/callProc(function, ...)
{
	var/list/L										= args.Copy(2);

	return call(world.system_type == MS_WINDOWS ? LIBPGSQL4DM_DLL_WIN32 : LIBPGSQL4DM_DLL_UNIX, function)(arglist(L));
}

/pgsql4dm/Connection/var/conninfo;

/pgsql4dm/Connection/New(conninfo)
{
	src.conninfo									= conninfo;

	return ..();
}

/pgsql4dm/Connection/Del(conninfo)
{
	pgsql4dm.callProc("dispose", src.conninfo);

	return ..()
}

/pgsql4dm/Connection/proc/exec(query, ...)
{
	var/list/L										= new/list();
	L.Add("execu");
	L.Add(src.conninfo);
	var/first										= TRUE;
	for (var/argument in args)						{ L.Add("[!first && argument == null ? "_<_NULL_>_" : argument]"); first = FALSE; }

	var/res											= pgsql4dm.callProc(arglist(L));

	if      (res == "-1")							{ throw EXCEPTION("An unknown error occurred. Please leave a detailed bug report at '[LIBPGSQL4DM_TRACKER_URL]'. (Error code: 1)"); }
	else if (res == "-2")							{ throw EXCEPTION("Invalid number of arguments."); }
	else if (res == "-3")							{ throw EXCEPTION("An unknown error occurred. Please leave a detailed bug report at '[LIBPGSQL4DM_TRACKER_URL]'. (Error code: 3)"); }
	else
	{
		if (findtext(res, "_<SQLERROR>_"))			{ throw EXCEPTION(copytext(res, 13)); }
		else										{ return text2num(res); }
	}
}

/pgsql4dm/Connection/proc/query(query, ...)
{
	var/list/L										= new/list();
	L.Add("exec");
	L.Add(src.conninfo);
	var/first										= TRUE;
	for (var/argument in args)						{ L.Add("[!first && argument == null ? "_<_NULL_>_" : argument]"); first = FALSE; }

	var/res											= pgsql4dm.callProc(arglist(L));

	if      (res == "-1")							{ throw EXCEPTION("An unknown error occurred. Please leave a detailed bug report at '[LIBPGSQL4DM_TRACKER_URL]'. (Error code: 1)"); }
	else if (res == "-2")							{ throw EXCEPTION("Invalid number of arguments."); }
	else if (res == "-3")							{ throw EXCEPTION("An unknown error occurred. Please leave a detailed bug report at '[LIBPGSQL4DM_TRACKER_URL]'. (Error code: 3)"); }
	else
	{
		if (findtext(res, "_<SQLERROR>_"))			{ throw EXCEPTION(copytext(res, 13)); }
		else										{ return new/pgsql4dm/ResultSet(res); }
	}
}

/pgsql4dm/ResultSet/var/id;

/pgsql4dm/ResultSet/New(id)
{
	src.id											= id;

	return ..()
}

/pgsql4dm/ResultSet/Del()
{
	try												{ callProc("dispose"); }
	catch ()										{ /* no problem */ }

	return ..()
}

/pgsql4dm/ResultSet/proc/callProc(function, ...)
{
	var/list/L = new/list();
	L.Add("result");
	L.Add("[src.id]");
	for (var/argument in args)						{ L.Add("[argument]"); }

	var/res											= pgsql4dm.callProc(arglist(L));

	if      (res == "-1")							{ throw EXCEPTION("An unknown error occurred. Please leave a detailed bug report at '[LIBPGSQL4DM_TRACKER_URL]'. (Error code: 1)"); }
	else if (res == "-2")							{ throw EXCEPTION("Invalid number of arguments."); }
	else if (res == "-3")							{ throw EXCEPTION("ResultSet not found. (id = [src.id])"); }
	else if (res == "-4")							{ throw EXCEPTION("Invalid function."); }
	else if (res == "-5")							{ throw EXCEPTION("Invalid column name."); }
	else if (res == "-6")							{ throw EXCEPTION("Invalid position."); }
	else											{ return text2num(res); }
}

/pgsql4dm/ResultSet/proc/next()						{ return callProc("next"); }
/pgsql4dm/ResultSet/proc/previous()					{ return callProc("previous"); }
/pgsql4dm/ResultSet/proc/isBeforeFirst()			{ return callProc("isBeforeFirst"); }
/pgsql4dm/ResultSet/proc/isAfterLast()				{ return callProc("isAfterLast"); }
/pgsql4dm/ResultSet/proc/isFirst()					{ return callProc("isFirst"); }
/pgsql4dm/ResultSet/proc/isLast()					{ return callProc("isLast"); }
/pgsql4dm/ResultSet/proc/first()					{ return callProc("first"); }
/pgsql4dm/ResultSet/proc/last()						{ return callProc("last"); }
/pgsql4dm/ResultSet/proc/getColumnNumber(column)	{ return callProc("getNumber", column); }

/pgsql4dm/ResultSet/proc/getString(column)
{
	var/res											= pgsql4dm.callProc("get", "[src.id]", isnum(column) ? "i" : "c", "[column]");

	if      (res == "_<PGSQL4DM_ERROR>_-1")			{ throw EXCEPTION("An unknown error occurred. Please leave a detailed bug report at '[LIBPGSQL4DM_TRACKER_URL]'. (Error code: 1)"); }
	else if (res == "_<PGSQL4DM_ERROR>_-2")			{ throw EXCEPTION("Invalid number of arguments."); }
	else if (res == "_<PGSQL4DM_ERROR>_-3")			{ throw EXCEPTION("ResultSet not found (id = [src.id])."); }
	else if (res == "_<PGSQL4DM_ERROR>_-4")			{ throw EXCEPTION("Invalid column name."); }
	else if (res == "_<PGSQL4DM_ERROR>_-5")			{ throw EXCEPTION("Invalid column position."); }
	else if (res == "_<PGSQL4DM_NULL>_")			{ return null; }
	else											{ return res; }
}

/pgsql4dm/ResultSet/proc/getNumber(column, defaultValue = 0)
{
	try
	{
		var/res										= getString(column);
		var/n										= text2num(res);

		if (res != -1.#INF && res != 1.#INF)		{ return n; }
		else										{ return defaultValue; }
	}
	catch (var/ex)									{ throw(ex); }
}

#undef LIBPGSQL4DM_TRACKER_URL
