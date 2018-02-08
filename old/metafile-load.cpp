#if 0  
  ifstream Metafile("Metafile");

  string word;
  do {
    // Read
    Metafile >> word;

    if(word == "meta") {
    top:
      // Define
      string word;
      string word2;
      string definition;

      Metafile >> word;

      while(true) {
	Metafile >> word2;

	if(word2 == "meta") {
	  dictionary[word] = definition;
	  debug && cout << word << ": " << definition << endl;
	  goto top;
	}

	if(Metafile.eof()) {
	  Definition def;

	  def.args = args;
	  def.definition = definition;
	  def.words = words;
	  dictionary[word] = def;
	  debug && cout << word << ": " << definition << endl;
	  break;
	}

	if(dictionary.count(word2)) {
	  word2 = dictionary[word2];
	}

	definition += word2 + " ";
	debug && cout << ".";
      }
    } else {
      // Expand
      if(dictionary.count(word)) {
	cout << dictionary[word];
      } else {
	cout << word << " ";
      }
    }
  } while(!Metafile.eof());
#endif
