# X4_AddMoney

X4 Foundations any version

Adds credits to the player without the use of a mod. Simply run and become rich. X4 Foundations must be running.

Timeline:
- Finds running instance of X4.exe
- Locates the filepath X4.exe was executed from
- Gets function address of AddPlayerMoney export from X4.exe
- Allocates and writes the required parameter to pass into AddPlayerMoney function
- Creates remote thread and executes AddPlayerMoney with parameter
- Waits for thread to complete and frees allocated memory/handles/etc.

The parameter used to invoke the AddPlayerMoney functions is an unknown 8 byte type, at this time. Might be a pointer to something because the money value given to the player is huge.  

Ex: 0x51F4D5C00 (8byte pointer) is around 22 billion  
AddPlayerMoney(0x51F4D5C00);  
...and in this moment I'm wondering if it's the memory address of the injected "uint64_t money_amount"  

# Resources
Very good showcases from one of the writers of Windows Internals, Pavel Yosifovich.  
https://www.youtube.com/watch?v=IZULG6I4z5U  
https://www.youtube.com/@zodiacon
