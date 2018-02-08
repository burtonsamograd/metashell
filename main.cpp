// 
// main.cpp
//
// Metashell (msh) is macro expanding shell.
//
// Input is collected in to the command buffer.
//
// The command buffer can be executed in /bin/sh using to 'go'
// command.
//
// All input is written to .msh-dribble and it is loaded
// '.msh-dribble' from the current directory at startup.
//
// 'go' commands are not executed on loading of .msh-dribble.
//
// Macros can be defined in the text using the following syntx:
//
//   define <word> <args..> as <words...>
//
// Debug output mode can be toggled by typing 'debug'.
//
//
// Burton Samograd - 2018
// License: AGPL

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>

using namespace std;

const string dribbleFile = ".msh-dribble";
bool debug = false;
static list<string> history;

typedef struct tag_Definition {
  list<string> args;
  string definition;
  list<string> words;
} Definition;

void help() {
  cerr << "" << endl;
  cerr << "Welcome to msh (metashell)." << endl;
  cerr << "" << endl;
  cerr << "Type words to enter them into the line buffer." << endl;
  cerr << "Type 'go' to execute the line buffer in /bin/sh." << endl;
  cerr << "Type 'clear' to clear the line buffer" << endl;
  cerr << "Type 'define <word> <args...> as <definition...> .' to define new words." << endl;
  cerr << "Type 'lookup <word> to get a word's definition." << endl;
  cerr << "Type 'words' to see all defined words." << endl;
  cerr << "" << endl;
};

void define(string& word, Definition& d) {
  cout << "define " << word << " ";
  for(auto& arg :  d.args) {
    cout << arg << " ";
  }
  cout << "as " << d.definition << endl;
}

typedef map<string, Definition> Dictionary;

string expand(string& word, Dictionary& dictionary) {
  string expansion;
  map<string, string> params;
  Definition& d = dictionary[word];

  for(auto arg : d.args) {
    cout << arg << " > ";
    cout.flush();
    cin >> params[arg];
  }
  
  for(auto word : d.words) {
    if(params.count(word)) {
      expansion += params[word] += "  ";
    } else {
      if(dictionary.count(word)) {
	expansion += dictionary[word].definition + " ";
      } else {
	expansion += word + " ";
      }
    }
  }
  
  return expansion; // d.definition + " ";
}

void process(istream& in, Dictionary& dictionary, bool noDribble = false, bool prompt = true, bool noExec = false) {
  ofstream dribble;
  string command;
  bool promptFlag = true;

  if(!noDribble) {
    dribble.open(dribbleFile, ofstream::app);
  }
  
  do {
    string word;
    
    promptFlag && prompt && cerr << "[ " << command << " ]" << endl << "$ ";
    promptFlag = false;
    
    // Read
    in >> word;

    if(word == "") 
      break;

    if(!noDribble || word != "go") {
      dribble << word << " ";
      dribble.flush();
    }

    if(word == "debug") {
      debug = !debug;
      if(debug) {
	cout << "Debugging is enabled." << endl;
      } else {
	cout << "Debugging is disabled." << endl;
      }
      continue;
      promptFlag = true;
    } if(word == "help") {
      help();
      promptFlag = true;
    } else if(word == "go") {
      !noExec && system(command.c_str());
      history.push_back(command);
      command = "";
      promptFlag = true;
    } else if(word == "clear") {
      command = "";
      promptFlag = true;
    } else if(prompt && word == "words") {
      
      for(auto it = dictionary.begin(); it != dictionary.end(); ++it) {
	string word = it->first;
	cout << word << endl;
	//Definition& d = dictionary[word];
  	//define(word, d);
      }
      promptFlag = true;
    } else if(prompt && word == "lookup") {
      string word;
      in >> word;

      Definition& d = dictionary[word];
      define(word, d);
      promptFlag = true;
    } else if(word == "define") {
      // Define
      string word;
      list<string> args;
      list<string>  words;
      string definition;

      in >> word;
      if(!noDribble) {
	dribble << word << " ";
      }

      do {
	string arg;
	cout << "> ";
	in >> arg;
	if(!noDribble) {
	  dribble << arg << " ";
	}

	if(arg == "as") {
	  break;
	}

	args.push_back(arg);
      } while(true);

      while(true) {
	string word2;
	in >> word2;

	words.push_back(word2);
	
	if(!noDribble) {
	  dribble << word2 << " ";
	}
	
	if(word2 == "go") {
	  Definition def;

	  words.pop_back();

	  def.args = args;
	  def.definition = definition;
	  def.words = words;
	  dictionary[word] = def;
	  debug && cout << word << ": " << definition << endl;
	  break;
	}

	if(dictionary.count(word2)) {
	  word2 = dictionary[word2].definition;
	}

	definition += word2 + " ";
	// debug && cout << ".";
      }
      promptFlag = true;
    } else {
      // Expand
      if(dictionary.count(word)) {
	command += expand(word, dictionary);
      } else {
	command += word + " ";
      }
      //cout << "[ " + command + " ]" << endl;
      promptFlag = true;
    }
    cout.flush();
  } while(!in.eof());
}

int main() {
  map<string, Definition> dictionary;

  help();

  ifstream dribble(dribbleFile, ifstream::in);

  process(dribble, dictionary, true, false, true);
  process(cin, dictionary);
  
  debug && cout << endl;
}
