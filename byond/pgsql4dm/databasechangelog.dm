#ifndef LIBPGSQL4DM_WITHOUT_CHANGELOG

#include <Deadron/Test>
#include <Deadron/TextHandling>
#include <Deadron/XML>

// Feel free to replace this with your own log function.
/pgsql4dm/Changelog/proc/Log(message)
{
	world.log << message;
}

/pgsql4dm/Changelog
{
	var
		file;
		pgsql4dm/Connection/connection;
}

/pgsql4dm/Changelog/New(pgsql4dm/Connection/connection, file)
{
	. = ..()

	src.file										= file;
	src.connection									= connection
}

/pgsql4dm/Changelog/proc/_ReverseText(text)
{
	var/result										= "";

	for (var/i = length(text), i > 0, i = i - 1)	{ result = result + ascii2text(text2ascii(text, i)); }

	return result
}

/pgsql4dm/Changelog/proc/Process()
{
	try
	{
		src.connection.exec({"CREATE TABLE IF NOT EXISTS databasechangelog
			(
			   file character varying(255) NOT NULL,
			   id character varying(80) NOT NULL,
			   author character varying(80) NOT NULL,
			   hash character varying(32) NOT NULL,
			   CONSTRAINT databasechangelog_pkey PRIMARY KEY (file, id, author)
			);"});
	}
	catch (var/exception/ex)						{ throw(ex); }

	_Process();
}

/pgsql4dm/Changelog/proc/_Process()
{
	var/id, author;

	try
	{
		Log("Reading changelog \"[src.file]\"");

		var/XML/Element/changelog					= xmlRootFromFile(src.file);
		var/temp, pos, failOnError, sql, hash;

		for (var/XML/Element/line in changelog.ChildElements())
		{
			if		(line.Tag() == "include")
			{
				if (line.Attribute("relativeToChangelogFile") == "true")
				{
					pos								= length("[file]") - findtext(_ReverseText("[file]"), "/");

					if (pos > 0)					{ temp = "[copytext("[file]", 1, pos + 2)]"; }
					else							{ temp = ""; }
				}
				else								{ temp = ""; }

				temp								= "[temp][line.Attribute("file")]";

				var/pgsql4dm/Changelog/c			= new(src.connection, temp)
				c._Process();
			}
			else if (line.Tag() == "changeSet")
			{
				id									= line.Attribute("id");
				author								= line.Attribute("author");
				failOnError							= line.Attribute("failOnError") != "false";
				sql									= "";

				for (var/XML/Element/changeSetAction in line.ChildElements())
				{
					if (changeSetAction.Tag() == "sql")
					{
						sql							= "[sql][changeSetAction.Text()];"
					}
				}

				hash								= md5("[sql]");

				var/pgsql4dm/ResultSet/rs			= src.connection.query("SELECT hash FROM databasechangelog WHERE \"file\" = $1 AND id = $2 AND author = $3", "[src.file]", "[id]", "[author]");

				if (rs.next())
				{
					if (hash != rs.getString(1))	{ throw EXCEPTION("The calculated signature (\"[hash]\") is different from the one stored in the database (\"[rs.getString(1)]\")!"); }
				}
				else
				{
					try								{ src.connection.exec(sql); }
					catch (var/exception/ex)
					{
						if (failOnError)			{ throw(ex); }
						else						{ Log("Error while executing changeset [id] ([author]), but continuing because failOnError=false\n[ex.name]"); }
					}

					try								{ src.connection.exec("INSERT INTO databasechangelog (file, id, author, hash) VALUES($1, $2, $3, $4)", "[src.file]", "[id]", "[author]", "[hash]"); }
					catch (var/exception/ex2)		{ Log("Error while inserting into changelog.\n[ex2]"); }

					Log("Executed changeset [id] ([author]) with signature \"[hash]\"");
				}
			}
		}
	}
	catch (var/exception/ex)
	{
		Log("Error while executing changeset [id] ([author])\n[ex]");
	}
}

#endif