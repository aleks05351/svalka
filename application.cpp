#include <iostream>
#include <string>
#include <mysql.h>
#include <sys/ioctl.h>
#include <unistd.h>

using namespace std;

const char Host[] = {"localhost"};
const char User[] = {"root"};
const char Pass[] = {"_Kaa300164_"};
const char DBase[] = {"webdb"};

void Print(struct winsize w, short int check) {
   cout << "\033c";
   if(check == 0) {
      for(int i = 0; i < w.ws_col/2 - 3; i++)
         cout << ' ';
      cout << "WebApp" << endl; 
      for(int i = 0; i < 5*w.ws_row/12; i++)
         cout << endl;
      for(int i = 0; i < w.ws_col/2 - 12; i++)
         cout << ' ';
      cout << "\x1b[43m/users\x1b[0m " << endl;
      for(int i = 0; i < w.ws_col/2 + 5; i++)
         cout << ' ';
      cout << "______________" << endl;
      for(int i = 0; i < w.ws_col/2 - 12; i++)
         cout << ' ';
      cout << "\x1b[0m/by-login?login=|______________|" << endl;
      for(int i = 0; i < w.ws_col/2 - 12; i++)
	 cout << ' ';
      cout << "/by-id?id=|___|" << endl;
   }
   else if(check == 1) {
      for(int i = 0; i < w.ws_col/2 - 3; i++)
         cout << ' ';
      cout << "WebApp" << endl; 
      for(int i = 0; i < 5*w.ws_row/12; i++)
         cout << endl;
      for(int i = 0; i < w.ws_col/2 - 12; i++)
         cout << ' ';
      cout << "\x1b[0m/users " << endl;
      for(int i = 0; i < w.ws_col/2 + 5; i++)
         cout << ' ';
      cout << "______________" << endl;
      for(int i = 0; i < w.ws_col/2 - 12; i++)
         cout << ' ';
      cout << "/by-login?login=|\x1b[43m______________\x1b[0m|" << endl; 
      for(int i = 0; i < w.ws_col/2 - 12; i++)
	 cout << ' ';
      cout << "/by-id?id=|___|" << endl;
   }
   else {
      for(int i = 0; i < w.ws_col/2 - 3; i++)
         cout << ' ';
      cout << "WebApp" << endl; 
      for(int i = 0; i < 5*w.ws_row/12; i++)
         cout << endl;
      for(int i = 0; i < w.ws_col/2 - 12; i++)
         cout << ' ';
      cout << "\x1b[0m/users " << endl;
      for(int i = 0; i < w.ws_col/2 + 5; i++)
         cout << ' ';
      cout << "______________" << endl;
      for(int i = 0; i < w.ws_col/2 - 12; i++)
         cout << ' ';
      cout << "/by-login?login=|______________|" << endl; 
      for(int i = 0; i < w.ws_col/2 - 12; i++)
	 cout << ' ';
      cout << "/by-id?id=|\x1b[43m___\x1b[0m|" << endl;
   }
}

void Blink(struct winsize w, short int check) {
   cout << "\033c";
   if(check == 1) {
      for(int i = 0; i < w.ws_col/2 - 3; i++)
         cout << ' ';
      cout << "WebApp" << endl; 
      for(int i = 0; i < 5*w.ws_row/12; i++)
         cout << endl;
      for(int i = 0; i < w.ws_col/2 - 12; i++)
         cout << ' ';
      cout << "\x1b[0m/users " << endl;
      for(int i = 0; i < w.ws_col/2 + 5; i++)
         cout << ' ';
      cout << "______________" << endl;
      for(int i = 0; i < w.ws_col/2 - 12; i++)
         cout << ' ';
      cout << "/by-login?login=|\x1b[5;43m______________\x1b[0m|" << endl;
      for(int i = 0; i < w.ws_col/2 - 12; i++)
	 cout << ' ';
      cout << "/by-id?id=|___|" << endl;

   }
   else {
      for(int i = 0; i < w.ws_col/2 - 3; i++)
         cout << ' ';
      cout << "WebApp" << endl; 
      for(int i = 0; i < 5*w.ws_row/12; i++)
         cout << endl;
      for(int i = 0; i < w.ws_col/2 - 12; i++)
         cout << ' ';
      cout << "\x1b[0m/users " << endl;
      for(int i = 0; i < w.ws_col/2 + 5; i++)
         cout << ' ';
      cout << "______________" << endl;
      for(int i = 0; i < w.ws_col/2 - 12; i++)
         cout << ' ';
      cout << "/by-login?login=|______________|" << endl;
      for(int i = 0; i < w.ws_col/2 - 12; i++)
	 cout << ' ';
      cout << "/by-id?id=|\x1b[5;43m___\x1b[0m|" << endl;
   }
}

int main() {
   short int check = 0;
   char c;
   string login, id;
   MYSQL *link;
   link = mysql_init(0);
   if(!link) {
      cout << "Can't get connection to database descriptor!" << endl;
      return 1;
   }
   if(!mysql_real_connect(link, Host, User, Pass, DBase, 0, 0, 0)) {
      fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(link));
      return 1;
   }

   MYSQL_RES *result = 0;
   MYSQL_ROW row;
 
   struct winsize w;
   ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
   
   string url, query;
   
   while(1) {
      Print(w, check);
      c = getchar();
      while(!isspace(c) && c != 126) { 
         if(c == 106) {
            check++;
         }
	 else if(c == 107) {
            check--;
	 }
	 check %= 3;
	 Print(w, check);
	 while((c = getchar()) != '\n');
	 c = getchar();
      }
      if(c == 126) {
         if(result != NULL)
            mysql_free_result(result);
         mysql_close(link);
         return 0;
      }
      if(check == 0) {
         cout << "\033c";
	 query = "SELECT id, login FROM users WHERE status <> 0;"; 
         if(mysql_query(link, query.c_str())) {
    	    cout << "Can't execute the query!" << endl;
   	    mysql_close(link);
	    return 1;
         }
         result = mysql_store_result(link);
         if(!result) {
	    cout << "The extraction of the result failed!" << endl;
	    mysql_close(link);
	    return 1;
         }
         if(mysql_num_rows(result) > 0) {
            while((row = mysql_fetch_row(result))) {
               for(int i = 0; i < mysql_num_fields(result); i++) 
		  cout << row[i] << "      |      ";
	       cout << endl;
	    }
         }
         else 
	    cout << "NOTHING!" << endl;
	 c = getchar();
      }
      else if(check == 1) {
	 Blink(w, check);
	 cin >> login;  
         c = getchar();
	 cout << "\033c";
         query = "SELECT * FROM users WHERE login = '" + login + "\'";
         if(mysql_query(link, query.c_str())) {
            cout << "Can't execute the query!" << endl;
	    mysql_close(link);
	    return 1;
         }
	 result = mysql_store_result(link);
	 if(!result) {
            cout << "The extraction of the result failed!" << endl;
	    mysql_close(link);
	    return 1;
	 }
	 if(mysql_num_rows(result) > 0) {
            while((row = mysql_fetch_row(result))) {
	       for(int i = 0; i < mysql_num_fields(result); i++) 
	          cout << row[i] << "    |      ";
               cout << endl;
	    }
         }
	 else
	    cout << "NOTHING!" << endl;
	 c = getchar();
      }
      else {
	 Blink(w, check);
	 cin >> id;
	 c = getchar();
	 cout  << "\033c";
	 query = "SELECT * FROM users WHERE id = '" + id + "\'";
	 if(mysql_query(link, query.c_str())) {
            cout << "Can't execute the query!" << endl;
	    mysql_close(link);
	    return 1;
         }
	 result = mysql_store_result(link);
	 if(!result) {
            cout << "The extraction of the result failed!" << endl;
	    mysql_close(link);
	    return 1;
	 }
	 if(mysql_num_rows(result) > 0) {
            while((row = mysql_fetch_row(result))) {
	       for(int i = 0; i < mysql_num_fields(result); i++) 
	          cout << row[i] << "    |      ";
               cout << endl;
	    }
         }
	 else
	    cout << "NOTHING!" << endl;
	 c = getchar();
      }
      mysql_free_result(result);
   }
}
