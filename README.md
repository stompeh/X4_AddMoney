# X4_AddMoney

X4 Foundations any version

Adds credits to the player without the use of a mod. Simply run and become rich.  

The parameter used to invoke the AddPlayerMoney functions is an unknown 8 byte type, at this time. Might be a pointer to something because the money value given to the player is huge.  

Ex: 0x51F4D5C00 (8byte pointer) is around 22 billion  
AddPlayerMoney(0x51F4D5C00);  
...and in this moment I'm wondering if it's the memory address of the injected "uint64_t money_amount"
