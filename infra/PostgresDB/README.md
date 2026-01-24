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
