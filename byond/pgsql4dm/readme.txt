The following files are needed on Windows:
 - libpq.dll: The PostgreSQL Client Library
 - intl.dll: Dependency of libpq (Internationalisation)
 - libeay32.dll: Dependency of libpq (SSL)
 - ssleay32.dll: Dependency of libpq (SSL)
 - libpgsql4dm.dll: Bridge between libpq.dll and DM.

The following files are needed on Linux:
 - libpgsql.so
 - You will also need the "postgresql-devel" or "postgresql-dev" packages
   installed depending on your distribution.
