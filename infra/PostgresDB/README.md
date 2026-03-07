##

########## Access database commands

# Access the DB docker in a bash terminal
docker exec -it condo_db bash

# Connect to the database using the default condo_user
psql -U condo_user -d condo

#If you see the following in termianl you are connected
condo=#

# Usefull DB commands
\l  -> List databases
\du -> List users
\dt -> List tables
\d  -> Verify a table structure
\i <SCRIPT> -> Run a sql script

# How migrations are applied (logic)
1. Ensure schema_migrations exists
2. Read applied versions from DB
3. Read migration files from disk
4. For each file not yet applied:
   - BEGIN
   - Run SQL
   - INSERT version into schema_migrations
   - COMMIT

# run migrations manually
docker compose run --rm migrate
