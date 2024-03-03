// Utility
struct user* findUser(char* username);
struct user* createUser(char* username, char* pwd);

// Autenticazione 
bool login(char* username, char* pwd);
// bool signup(char* username, char* pwd);