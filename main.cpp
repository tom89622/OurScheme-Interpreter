# include <iostream>
# include <stdio.h>
# include <string.h>
# include <string>
# include <stdlib.h>
# include <iomanip>
# include <sstream>
# include <vector>
# include <stdexcept>

using namespace std ;

class Node {
  public :
  string mData ;
  Node *mLeft ;
  Node *mRight ;
  
  Node() {
    mData.clear() ;
    mLeft = NULL ;
    mRight = NULL ;
  } // Node()
  
} ;

class StoreArea {
  public:
  vector<Node*> mDefines ;
  vector<Node*> mLocal ;
};

int gLn = 1, gCol = 1 ; // count input line and column
Node* gRoot ;


Node* EvalS_exp( Node* head, StoreArea &storeArea ) ; // will trun a node is evaluated
bool GetS_exp( vector<string> &tokenString, string perToken, Node* perNode ) ;
string GetToken() ;
void InitialStoreArea( StoreArea &storeArea ) ;
Node* Eval_Lambda( Node* head, StoreArea &storeArea ) ;
Node* Eval_Let( Node* head, StoreArea &storeArea ) ;
Node* Eval_Define( Node* head, StoreArea &storeArea ) ;
Node* Eval_DefineFunc( Node* head, StoreArea &storeArea ) ;
Node* Eval_Func( Node* head, StoreArea &storeArea ) ;
Node* Eval_Cons( Node* head, StoreArea &storeArea ) ;
Node* Caculate( Node* head, StoreArea &storeArea ) ;
Node* TestPrimitive_predicates( Node* head, StoreArea &storeArea ) ;
Node* Eval_Compare( Node* head, StoreArea &storeArea ) ;
Node* Eval_String( Node* head, StoreArea &storeArea ) ;
Node* Eval_eqv( Node* head, StoreArea &storeArea ) ;
Node* Eval_equal( Node* head, StoreArea &storeArea ) ;
bool Euqal_traversal( Node* node1, Node* node2 ) ;
int CheckArgumentNum( Node* head ) ;
string ThisNodeIsIns( Node* head, StoreArea &storeArea ) ;
bool InputString( string &token ) ;
string WhichType( string token ) ;
bool ShouldExit( Node* head ) ;
bool IsWhiteSpace( char inputChar ) ;
bool IsSeparator( char inputChar ) ;
bool IsSeparator( string inputString ) ;
bool IsNum( char test ) ;
bool IsInt( string token ) ;
bool IsFloat( string token ) ;
bool IsIns( string data ) ;
void PrintS_exp( Node* head, int &stage, bool &newS_exp ) ;
void PrintForEscape( string token ) ;
void PrintNonComment( string token ) ; 
bool StringProcessing( vector<string> &atom ) ;
void ClearTree( Node* head ) ;
void CopyTree( Node* oldTree, Node* newTree ) ;
int InsPurcedure( Node* head ) ;
string Itoa( int num ) ;
double Stod( string in ) ;
string Purcedure2Normal( string ins ) ;
void PrintErrorS_exp( Node* head ) ;


int main() {
  vector<string> tokenString ;
  vector<Node*> trees ;  
  int uTestNum, skipNum = 0 ;
  Node *head = new Node() ;
  StoreArea storeArea ;
  string nextToken ; // = GetToken() ;
  bool inputSucess = false, syntaxError = false ; // keep getToken until input legal
  bool evalError = false ;
  bool newS_exp = false, checkNextS_exp = false ;
  bool exit = false ;
  // string tokenType = WhichType( token ) ;
  
  printf( "Welcome to OurScheme!\n\n> " ) ;
  cin >> uTestNum ;
  cin.get() ; // skip '\n'
  // printf( "> " ) ;
  try {
    InitialStoreArea( storeArea ) ;
    while ( !inputSucess ) {
      gLn = 1 ;
      gCol = 0 ; 
      
      try { 
        nextToken = GetToken() ; // get token string
        inputSucess = true ;
      } // try
      catch( char const* error  ) {
        if ( !strcmp( error, "ERROR (no more input) : END-OF-FILE encountered" ) )
          throw "ERROR (no more input) : END-OF-FILE encountered" ;
        else  {
          char tmp ;
          printf( "%s\n> ", error ) ;
          while ( IsWhiteSpace( cin.peek() ) ) {
            if ( cin >> noskipws >> tmp ) ; // get nextToken string
            else throw "ERROR (no more input) : END-OF-FILE encountered" ; // throw exception
          } // while()
          
          if ( cin.peek() == '\n' ) {
            if ( cin >> noskipws >> tmp ) ; // get nextToken string
            else throw "ERROR (no more input) : END-OF-FILE encountered" ; // throw exception
          } // if
        } // else
        
        inputSucess = false ;
      } // catch
      
    } // while()
    
      // cout << "out while" << inputSucess << endl ;
    try {
      head->mData = "(" ;
      GetS_exp( tokenString, nextToken, head ) ;
    } // try
    catch( char const* error ) {
      if ( !strcmp( error, "ERROR (no more input) : END-OF-FILE encountered" ) ) 
        throw "ERROR (no more input) : END-OF-FILE encountered" ;
      else  {
        // cout << error ;
        char tmp ;
        printf( "%s\n> ", error ) ;
        while ( cin.peek() != '\n' ) {
          if ( cin >> noskipws >> tmp ) ; // get nextToken string
          else throw "ERROR (no more input) : END-OF-FILE encountered" ; // throw exception
        } // while()
        
        if ( cin.peek() == '\n' ) {
          if ( cin >> noskipws >> tmp ) ; // get nextToken string
          else throw "ERROR (no more input) : END-OF-FILE encountered" ; // throw exception
        } // if
        
        // Didn't need eat '\n'
        error = NULL ;
        syntaxError = true ;
        tokenString.clear() ;
      } // else
    } // catch
    
    while ( !ShouldExit( head ) && !exit ) {
      char tmp ;
      
      if ( !syntaxError
           && ( head->mData.empty() ? true : head->mData.at( 0 ) != ';' ) ) { 
      // when non syntaxerror, evaluate s_exp
        try {
          bool noReturnIsOK = false ;
          // cout << "---start evaluate---\n" ;
          // PrintErrorS_exp( head ) ;
          gRoot = head ;
          Node* curS_expCopy = new Node() ;
          CopyTree( head, curS_expCopy ) ;
          if ( head != NULL && head->mLeft != NULL 
               && ( !head->mLeft->mData.compare( "define" ) 
                    || !head->mLeft->mData.compare( "clean-environment" ) ) ) {
            noReturnIsOK = true ;            
          } // if
          
          storeArea.mLocal.clear() ;
          head = EvalS_exp( head, storeArea ) ; // EvalS_exp will return a node which is evaluated
          if ( head != NULL && ShouldExit( head ) && WhichType( gRoot->mLeft->mData ) == "QUOTE" ) 
            exit = true ;
          evalError = false ;
          if ( head != NULL && !head->mData.compare( "\0" ) )
            head->mData = "(" ;
          if ( head != NULL && IsIns( head->mData ) ) {
            Node* insNode = new Node() ;
            insNode->mData = "#<procedure " + head->mData + ">" ;
            // head = insNode ;
          } // if
        
          if ( head == NULL && !noReturnIsOK ) {
            printf( "ERROR (no return value) : " ) ;
            PrintErrorS_exp( curS_expCopy ) ;
            printf( "\n> " ) ;
          } // if
                   
          for ( int i = 0 ; i < storeArea.mDefines.size() ; i++ ) {
            if ( storeArea.mDefines.at( i ) != NULL
                 && IsIns( storeArea.mDefines.at( i )->mData ) ) {
              storeArea.mDefines.at( i )->mData 
                = "#<procedure " + storeArea.mDefines.at( i )->mData + ">" ;                     
            } // if
            
            if ( storeArea.mDefines.at( i )->mLeft != NULL
                 && !storeArea.mDefines.at( i )->mLeft->mData.compare( "#<procedure lambda>" ) ) {
              storeArea.mDefines.erase( storeArea.mDefines.begin() + i ) ;     
            } // if
            
          } // for
          // cout << "\n---end evaluate---\n" ;
          // printf( "\n> " ) ;
        } // try
        catch ( const char* error ) { // error message is wrong!!!!!!!!
          evalError = true ;
          string tmp( error ) ;
          if ( tmp.length() > 25 
               && !strcmp( tmp.substr( 0, 25 ).c_str(), "ERROR (unbound symbol) : " ) ) {
            // cout << error ;
          } // if
          else if ( !strcmp( error, "ERROR (no return value) : " ) ) {
            cout << error ;
            PrintErrorS_exp( head ) ;
          } // if
          
          for ( int i = 0 ; i < storeArea.mDefines.size() ; i++ ) {
            if ( storeArea.mDefines.at( i )->mLeft != NULL 
                 && !storeArea.mDefines.at( i )->mLeft->mData.compare( "#<procedure lambda>" ) ) {
              storeArea.mDefines.erase( storeArea.mDefines.begin() + i ) ;
            } // if
          } // for
          
          printf( "\n> " ) ;
        } // catch()
        
        if ( !evalError && head != NULL && !exit ) { 
        // when non evalError and not define, pretty print
          int stage = 0, count ;
          PrintS_exp( head, stage, newS_exp ) ;
          
          if ( head != NULL && !head->mData.compare( "(" ) )
            head->mData = "\0" ;
          if ( stage-- ) { // cover ( 1 . nil )'s last ')'
            count = stage ;
            while ( count-- ) printf( "  " ) ;
            printf( ")\n" ) ;
          } // if
          
          printf( "\n> " ) ;
        } // if
        
        newS_exp = false ;
      } // if
      
      if ( !exit ) {
        while ( IsWhiteSpace( cin.peek() ) && cin.peek() != '\n' ) { // skip the whiteSpace after S_exp
          if ( cin.get() == EOF ) throw "ERROR (no more input) : END-OF-FILE encountered" ;
          // gCol++ ;
          skipNum++ ;
          // cout << "counting\n" ;
          checkNextS_exp = true ;
        } // while()
        
        if ( cin.peek() == ';' ) { // if next char is comment, skip it
          while ( cin.peek() != '\n' ) {
            if ( cin.get() == EOF ) throw "ERROR (no more input) : END-OF-FILE encountered" ;  
          } // while()
        } // if
        
        if ( cin.peek() == '\n' ) { // if next char is endline, skip it and don't count the line
          if ( cin.get() == EOF ) throw "ERROR (no more input) : END-OF-FILE encountered" ;
          checkNextS_exp = false ;
        } // if
        
        inputSucess = false ;
        // trees.push_back( head ) ;
        ClearTree( head ) ;
        head = new Node() ;
        tokenString.clear() ;
        syntaxError = false ; // if last time is error don't print
        evalError  = false ;
        
        while ( !inputSucess ) {
          gLn = 1 ;
          checkNextS_exp ? gCol = skipNum : gCol = 0 ;
          try { 
            nextToken = GetToken() ; // get token string
            // cout << "test line and column :" << gLn << ", " << gCol << endl ;
            inputSucess = true ;
          } // try
          catch( char const* error  ) {
            if ( !strcmp( error, "ERROR (no more input) : END-OF-FILE encountered" ) )
              throw "ERROR (no more input) : END-OF-FILE encountered" ;
            else  {
              char tmp ;
              printf( "%s\n> ", error ) ;
              while ( IsWhiteSpace( cin.peek() ) ) {
                if ( cin >> noskipws >> tmp ) ; // get nextToken string
                else throw "ERROR (no more input) : END-OF-FILE encountered" ; // throw exception
              } // while()
              
              if ( cin.peek() == '\n' ) {
                if ( cin >> noskipws >> tmp ) ; // get nextToken string
                else throw "ERROR (no more input) : END-OF-FILE encountered" ; // throw exception
              } // if
            } // else
            
            inputSucess = false ;  
          } // catch
          
          skipNum = 0 ; 
          checkNextS_exp = false ;
        } // while()
        
        
        try {
          // gLn > 1 ? gLn-- : gLn ;
          head->mData = "(" ;
          GetS_exp( tokenString, nextToken, head ) ; 
          
        } // try
        catch( char const* error ) {
          if ( !strcmp( error, "ERROR (no more input) : END-OF-FILE encountered" ) )
            throw "ERROR (no more input) : END-OF-FILE encountered" ;
          else  {
            char tmp ;
            printf( "%s\n> ", error ) ;
            while ( cin.peek() != '\n' ) { // skip this line
              if ( cin >> noskipws >> tmp ) ; // get nextToken string
              else throw "ERROR (no more input) : END-OF-FILE encountered" ; // throw exception
            } // while()
            
            if ( cin.peek() == '\n' ) {
              if ( cin >> noskipws >> tmp ) ; // get nextToken string
              else throw "ERROR (no more input) : END-OF-FILE encountered" ; // throw exception
            } // if
            
            syntaxError = true ;
            tokenString.clear() ;
          } // else
        } // catch
        
      } // if
    } // while()
  } // try
  catch( char const* error ) {
    cout << error ;
  } // catch
  printf( "\nThanks for using OurScheme!" ) ;
} // main()
 
Node* EvalS_exp( Node* head, StoreArea &storeArea ) {
  if ( head == NULL ) {
    Node* head = new Node() ;
    head->mData = "nil" ;
    return head ;
  } // if
  else if ( head->mLeft == NULL && head->mRight == NULL ) { // leaf.
    if ( WhichType( head->mData ) == "INT" || WhichType( head->mData ) == "FLOAT" 
         || WhichType( head->mData ) == "STRING" || WhichType( head->mData ) == "nil" 
         || WhichType( head->mData ) == "#t"  ) {
      // cout << "leaf atom : " << head->mData << endl ;
      return head ; // Is ATOM but not SYMBOL
    } // if
    else if ( WhichType( head->mData ) == "SYMBOL" 
              || IsIns( Purcedure2Normal( head->mData ) ) ) { // Is symbol
      bool findDefined = false ;
      string  pureIns = Purcedure2Normal( head->mData ) ;
      // cout << "\n----" << pureIns << endl ;
      if ( !storeArea.mLocal.empty() ) { // find local var first
        string pureFunc ;
        for ( int i = storeArea.mLocal.size()-1 ; i >= 0 ; i-- ) {
          if ( storeArea.mLocal.at( i )->mLeft != NULL
               && CheckArgumentNum( storeArea.mLocal.at( i ) ) == 3 ) {
            pureFunc = Purcedure2Normal( storeArea.mLocal.at( i )->mLeft->mData ) ;
            
          } // if
          
          if ( IsIns( pureIns ) ) {
            string compared = Purcedure2Normal( storeArea.mLocal.at( i )->mData ) ;
            // cout << ins << compared << endl ;
            if ( !pureIns.compare( "'" ) ) pureIns = "quote" ;
            if ( !compared.compare( pureIns.c_str() ) ) {
              return storeArea.mLocal.at( i ) ;
            } // if
          } // if
          else if ( storeArea.mLocal.at( i )->mLeft != NULL 
                    && !pureFunc.compare( pureIns )
                    && CheckArgumentNum( storeArea.mLocal.at( i ) ) == 3 ) {
            Node* tmp = new Node() ;
            tmp->mData = "#<procedure " 
                         + Purcedure2Normal( storeArea.mLocal.at( i )->mLeft->mData ) + ">" ;
            return tmp ;      
          } // else if
          else if ( storeArea.mLocal.at( i )->mLeft != NULL 
                    && !storeArea.mLocal.at( i )->mLeft->mData.compare( pureIns ) ) {
            findDefined = true ;
            // head = storeArea.defines.at(i)->mRight ;
            
            if ( !storeArea.mLocal.at( i )->mRight->mLeft->mData.compare( "\0" ) ) 
              storeArea.mLocal.at( i )->mRight->mLeft->mData = "(" ;
            
            return storeArea.mLocal.at( i )->mRight->mLeft ;
          } // else if
          
        } // for
      } // if
      
      if ( !storeArea.mDefines.empty() ) {
        for ( int i = 0 ; i < storeArea.mDefines.size() ; i++ ) {
          // cout << "find in global variable\n" ;
          string pureFunc ;
          if ( storeArea.mDefines.at( i )->mLeft != NULL
               && CheckArgumentNum( storeArea.mDefines.at( i ) ) == 3 )
            pureFunc = Purcedure2Normal( storeArea.mDefines.at( i )->mLeft->mData ) ;
          if ( IsIns( pureIns ) ) {
            string compared = Purcedure2Normal( storeArea.mDefines.at( i )->mData ) ;
            if ( !pureIns.compare( "'" ) ) pureIns = "quote" ;
            if ( !compared.compare( pureIns.c_str() ) ) {
              storeArea.mDefines.at( i )->mData = Purcedure2Normal( storeArea.mDefines.at( i )->mData ) ;
              storeArea.mDefines.at( i )->mData = "#<procedure " + storeArea.mDefines.at( i )->mData + ">" ;
              return storeArea.mDefines.at( i ) ;
            } // if
          } // if
          else if ( storeArea.mDefines.at( i )->mLeft != NULL 
                    && !pureFunc.compare( pureIns )
                    && CheckArgumentNum( storeArea.mDefines.at( i ) ) == 3 ) {
            Node* tmp = new Node() ;
            tmp->mData = "#<procedure " 
                         + Purcedure2Normal( storeArea.mDefines.at( i )->mLeft->mData ) + ">" ;
            return tmp ; 
                
          } // else if
          else if ( storeArea.mDefines.at( i )->mLeft != NULL 
                    && !storeArea.mDefines.at( i )->mLeft->mData.compare( pureIns ) ) {
            findDefined = true ;
            // if ( storeArea.mDefines.at( i )->mRight->mLeft != NULL )
              // InsPurcedure( storeArea.mDefines.at( i )->mRight->mLeft ) ;
              
            if ( storeArea.mDefines.at( i )->mRight != NULL 
                 && storeArea.mDefines.at( i )->mRight->mLeft != NULL
                 && !storeArea.mDefines.at( i )->mRight->mLeft->mData.compare( "lambda" ) )
              storeArea.mDefines.at( i )->mRight->mLeft->mData = "#<procedure lambda>" ;
            if ( storeArea.mDefines.at( i )->mRight != NULL 
                 && storeArea.mDefines.at( i )->mRight->mLeft != NULL
                 && !storeArea.mDefines.at( i )->mRight->mLeft->mData.compare( "#<procedure lambda>" ) ) {
              for ( int i = 0; i < storeArea.mDefines.size() ; i++ ) {
                if ( storeArea.mDefines.at( i )->mLeft != NULL
                     && !storeArea.mDefines.at( i )->mLeft->mData.compare( "#<procedure lambda>" ) ) {
                  storeArea.mDefines.erase( storeArea.mDefines.begin() + i ) ;
                } // if
              } // for
              
              storeArea.mDefines.push_back( storeArea.mDefines.at( i )->mRight ) ;
            } // if
            
            if ( storeArea.mDefines.at( i )->mRight != NULL 
                 && storeArea.mDefines.at( i )->mRight->mLeft != NULL
                 && !storeArea.mDefines.at( i )->mRight->mLeft->mData.compare( "\0" ) ) 
              storeArea.mDefines.at( i )->mRight->mLeft->mData = "(" ;
            
            // PrintErrorS_exp( storeArea.mDefines.at( i )->mRight->mLeft ) ;
            return storeArea.mDefines.at( i )->mRight->mLeft ;
          } // else if
        } // for
      } // if
      
      if ( !findDefined ) { // Didn't find defined symbol
        string error = "ERROR (unbound symbol) : " + head->mData + "\n" ;
        const char* ERRORMESSAGE = error.c_str() ;
        printf( "%s", ERRORMESSAGE ) ;
        throw ERRORMESSAGE ;
      } // if
      
      return NULL ; // Won't process it
    } // else if 
    else {
      // cout << "Something didn't consider about it.\n" ;
    } // else
  } // else if
  else { // if head isn't NULL keep process current S_exp
    string curIns = "\0" ;
    if ( head->mRight != NULL ) { // ----Is list ?
      Node* cur = head->mRight ;
      for (  ; cur != NULL ; cur = cur->mRight ) {
        if ( cur->mRight == NULL && cur->mLeft == NULL 
             && WhichType( cur->mData ) == "nil" ) {
          ;
        } // if
        else if ( cur->mData.compare( "\0" ) && cur->mData.compare( "(" ) ) {
          head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
          printf( "ERROR (non-list) : " ) ;
          PrintErrorS_exp( head ) ;
          throw "ERROR (non-list) : " ;
        } // if
      } // for
    } // if
    
    try {
      // check left node is legal or not.
      string insCheck ;
      // PrintErrorS_exp( head ) ;
      head->mLeft = EvalS_exp( head->mLeft, storeArea ) ;
    } // try
    catch ( const char* error ) {
      if ( !strcmp( error, "ERROR (no return value) : " ) ) {
        printf( "ERROR (no return value) : " ) ;
        PrintErrorS_exp( head->mLeft ) ;
        error = "error" ;
      } // if 
      
      throw error ;
    } // catch
    
    try {
      // PrintErrorS_exp( head ) ;
      if ( head->mLeft->mData.size() < 20 ) {
        if ( IsIns( head->mLeft->mData ) ) {
          int index = -1 ;
          for ( int i = 0 ; i < storeArea.mDefines.size() ; i++ ) {
            string tmp = Purcedure2Normal( storeArea.mDefines.at( i )->mData ) ;
            if ( !tmp.compare( head->mLeft->mData ) ) {
              index = i ;
              i = storeArea.mDefines.size() ;
            } // if
          } // for
          
          if ( index != -1 && storeArea.mDefines.at( index ) != head->mLeft ) {
            printf( "ERROR (attempt to apply non-function) : %s\n", head->mLeft->mData.c_str() ) ;
            throw "ERROR (attempt to apply non-function) : " ;
          } // if
        } // if
      } // if
      
      string leftestType = ThisNodeIsIns( head->mLeft, storeArea ) ;
      if ( !leftestType.compare( "ATOM" )
           || !leftestType.compare( "UnboundSymbol" )
           || !leftestType.compare( "nil" )
           || !leftestType.compare( "error" ) ) {
        if ( !leftestType.compare( "UnboundSymbol" ) ) {
          string error = "ERROR (unbound symbol) : " + head->mLeft->mData + "\n" ;
          const char* ERRORMESSAGE = error.c_str() ;
          printf( "%s", ERRORMESSAGE ) ;
          throw ERRORMESSAGE ;
        } // if
        
        // cout << leftestType << endl ;
        printf( "ERROR (attempt to apply non-function) : " ) ;
        if ( !leftestType.compare( "ATOM" ) ) {
          if ( WhichType( leftestType ) == "FLOAT" )
            printf( "%.3f\n", Stod( head->mLeft->mData ) ) ;
          else
            printf( "%s\n", head->mLeft->mData.c_str() ) ;
        } // if
        else 
          PrintErrorS_exp( head ) ;  
        throw "ERROR (attempt to apply non-function) : " ;     
      } // if
      else if ( !leftestType.compare( "Function" ) ) { // Evaluate function
        return Eval_Func( head, storeArea ) ;
      } // else if     
      // PrintErrorS_exp( head ) ;
    } // try
    catch ( const char* error ) {
      throw error ;
    } // catch
    
    // cout << "-doing ins.-\n" ;
    // evaluate current S_exp
    // It's instruction's previous layer
    // head = EvalS_exp( head, storeArea ) ;
    curIns = Purcedure2Normal( head->mLeft->mData ) ;

    // cout << "cur ins :" << curIns << endl ;
    // PrintErrorS_exp( head ) ;
    if ( !curIns.compare( "list" ) ) {
      if ( head->mRight != NULL ) {
        head = head->mRight ;
        head->mData = "(" ;
      } // if
      else {
        Node* re = new Node() ;
        re->mData = "nil" ;
        return re ; // non argument
      } // else
      
      for ( Node* walk = head ; walk != NULL ; walk = walk->mRight ) {
        // if ( !walk->mData.compare( "(" ) ) walk->mData = "\0" ;
        if ( walk->mLeft != NULL ) {
          try {
            walk->mLeft = EvalS_exp( walk->mLeft, storeArea ) ;
          } // try
          catch ( const char* error ) {
            string tmp( error ) ;
            if ( !strcmp( error, "ERROR (no return value) : " ) ) {
              error = "ERROR (unbound parameter) : " ;
              cout << error ;
              PrintErrorS_exp( walk->mLeft ) ;
            } // if
            
            throw error ;
          } // catch
        } // if
        else {
          printf( "ERROR (unbound parameter) : " ) ;
          PrintErrorS_exp( walk->mLeft ) ;
          throw "ERROR (unbound parameter) : " ;
        } // else
      } // for
        
    } // if
    else if ( !curIns.compare( "define" ) && head == gRoot ) {
      if ( head->mRight != NULL && head->mRight->mLeft != NULL 
           && head->mRight->mLeft->mData.compare( "\0" ) 
           && head->mRight->mLeft->mData.compare( "(" )
           && CheckArgumentNum( head->mRight ) != 2 ) {
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (DEFINE format) : " ) ;
        PrintErrorS_exp( head ) ;
        throw "ERROR (DEFINE format) : " ;
      } // if
      else if ( CheckArgumentNum( head ) <= 2 ) {
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (DEFINE format) : " ) ;
        PrintErrorS_exp( head ) ;
        throw "ERROR (DEFINE format) : " ;
      } // else if
      
      try {
        Eval_Define( head, storeArea ) ; // try dedined name and do define. It's symbol?
        return NULL ;
      } // try
      catch ( const char* error ) {
        if ( !strcmp( error, "ERROR (DEFINE format) : " ) ) {
          head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
          PrintErrorS_exp( head ) ;
        } // if
        
        throw error ;
      } // catch
    } // else if
    else if ( !curIns.compare( "cons" ) ) {
      
      if ( CheckArgumentNum( head->mRight ) != 2 ) { // when cons' argument not equal two.
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (incorrect number of arguments) : %s\n", curIns.c_str() ) ;
        throw "ERROR (incorrect number of arguments) : cons" ;
      } // if
      
      try {
        // head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        return Eval_Cons( head, storeArea ) ; // do Cons.
      } // try
      catch ( const char* error ) {
        
        throw error ;
      } // catch
    } // else if
    else if ( !curIns.compare( "quote" ) 
              || !curIns.compare( "'" ) ) {
      if ( CheckArgumentNum( head->mRight ) != 1 ) { // when cons' argument not equal one.
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (incorrect number of arguments) : quote\n" ) ;
        throw "ERROR (incorrect number of arguments) : quote" ;
      } // if
      
      Node* s_exp = head->mRight->mLeft ;
      !head->mRight->mLeft->mData.compare( "\0" ) ? 
        head->mRight->mLeft->mData = "(" : head->mRight->mLeft->mData ;
      // cout << "inini" << endl ;
      
      head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
      return head->mRight->mLeft ;
      
    } // else if
    else if ( !curIns.compare( "car" ) ) {
      if ( CheckArgumentNum( head->mRight ) != 1 ) { // when cons' argument not equal one.
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (incorrect number of arguments) : %s\n", curIns.c_str() ) ;
        throw "ERROR (incorrect number of arguments) : car" ;
      } // if
      
      Node* original = head ;
      head = head->mRight->mLeft ;
            
      try {
        head = EvalS_exp( head, storeArea ) ;
        if ( head != NULL && head->mLeft == NULL && head->mRight == NULL ) {
          if ( WhichType( head->mData ) == "SYMBOL" || WhichType( head->mData ) == "INT" 
               || WhichType( head->mData ) == "FLOAT" || WhichType( head->mData ) == "STRING" 
               || WhichType( head->mData ) == "nil" || WhichType( head->mData ) == "#t" ) {
            if ( WhichType( head->mData ) == "FLOAT" )
              printf( "ERROR (car with incorrect argument type) : %.3f\n", Stod( head->mData ) ) ;
            else
              printf( "ERROR (car with incorrect argument type) : %s\n", head->mData.c_str() ) ;
            throw "ERROR (car with incorrect argument type) : " ;
          } // if
        } // if
      } // try
      catch ( const char* error ) {
        string tmp( error ) ;
        if ( !strcmp( error, "ERROR (no return value) : " ) ) {
          error = "ERROR (unbound parameter) : " ;
          cout << error ;
          PrintErrorS_exp( head ) ;
        } // if
        
        throw error ;
      } // catch
      
      original->mLeft->mData = Purcedure2Normal( original->mLeft->mData ) ;
      return head->mLeft ;
    } // else if
    else if ( !curIns.compare( "cdr" ) ) {
      if ( CheckArgumentNum( head->mRight ) != 1 ) { // when cons' argument not equal one.
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (incorrect number of arguments) : %s\n", curIns.c_str() ) ;
        throw "ERROR (incorrect number of arguments) : cdr" ;
      } // if
      
      Node* original = head ;
      head = head->mRight->mLeft ;
            
      try {
        head = EvalS_exp( head, storeArea ) ;
        // cout << "cdr return : " << head->mLeft->mData << endl ;
        if ( head != NULL && head->mLeft == NULL && head->mRight == NULL ) {
          if ( WhichType( head->mData ) == "SYMBOL" || WhichType( head->mData ) == "INT" 
               || WhichType( head->mData ) == "FLOAT" || WhichType( head->mData ) == "STRING" 
               || WhichType( head->mData ) == "nil" || WhichType( head->mData ) == "#t" ) {
            if ( WhichType( head->mData ) == "FLOAT" )
              printf( "ERROR (cdr with incorrect argument type) : %.3f\n", Stod( head->mData ) ) ;
            else
              printf( "ERROR (cdr with incorrect argument type) : %s\n", head->mData.c_str() ) ;
            throw "ERROR (cdr with incorrect argument type) : " ;
          } // if
        } // if
      } // try
      catch ( const char* error ) {
        string tmp( error ) ;
        if ( !strcmp( error, "ERROR (no return value) : " ) ) {
          error = "ERROR (unbound parameter) : " ;
          cout << error ;
          PrintErrorS_exp( head ) ;
        } // if
        
        throw error ;
      } // catch
      // !head->mRight->mData.compare( "\0" ) ? head->mRight->mData = "(" : head->mLeft->mData ;
      if ( head->mRight == NULL ) {
        Node* nilNode = new Node() ;
        nilNode->mData = "nil" ;
        return nilNode ;  
      } // if
      
      original->mLeft->mData = Purcedure2Normal( original->mLeft->mData ) ;
      return head->mRight ;
    } // else if
    else if ( !curIns.compare( "+" ) || !curIns.compare( "-" )
              || !curIns.compare( "*" ) || !curIns.compare( "/" ) ) {
      if ( CheckArgumentNum( head->mRight ) < 2 ) { // when cons' argument not equal one. 
        string errormessage = "ERROR (incorrect number of arguments) : " + curIns ;
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (incorrect number of arguments) : %s\n", curIns.c_str() ) ;
        throw errormessage.c_str() ;
      } // if
      
      try {
        head = Caculate( head, storeArea ) ;
      } // try
      catch ( const char* error ) {
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        throw error ;
      } // catch
      // !head->mRight->mData.compare( "\0" ) ? head->mRight->mData = "(" : head->mLeft->mData ;
      return head ;
    } // else if
    else if ( !curIns.compare( "atom?" ) || !curIns.compare( "pair?" )
              || !curIns.compare( "list?" ) || !curIns.compare( "null?" ) 
              || !curIns.compare( "integer?" ) || !curIns.compare( "real?" ) 
              || !curIns.compare( "number?" ) || !curIns.compare( "string?" ) 
              || !curIns.compare( "boolean?" ) || !curIns.compare( "symbol?" ) ) {
      if ( CheckArgumentNum( head->mRight ) != 1 ) { // when cons' argument not equal one. 
        string errormessage = "ERROR (incorrect number of arguments) : " + curIns ;
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (incorrect number of arguments) : %s\n", curIns.c_str() ) ;
        throw errormessage.c_str( ) ;
      } // if
      
      try {
        head = TestPrimitive_predicates( head, storeArea ) ;
      } // try
      catch ( const char* error ) {
        throw error ;
      } // catch
      
      return head ; // return the ans.
    } // else if
    else if ( !curIns.compare( "not" ) ) {
      if ( CheckArgumentNum( head->mRight ) != 1 ) { // when cons' argument not equal one. 
        string errormessage = "ERROR (incorrect number of arguments) : " + curIns ;
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (incorrect number of arguments) : %s\n", curIns.c_str() ) ;
        throw errormessage.c_str() ;
      } // if
      
      Node* returnNode = new Node() ;
      string compare ;
      try {
        compare = EvalS_exp( head->mRight->mLeft, storeArea )->mData ;
      } // try
      catch ( const char* error ) {
        string tmp( error ) ;
        if ( !strcmp( error, "ERROR (no return value) : " ) ) {
          error = "ERROR (unbound parameter) : " ;
          cout << error ;
          PrintErrorS_exp( head ) ;
        } // if
        
        throw error ;
      } // catch
      
      if ( WhichType( compare ) == "nil" ) returnNode->mData = "#t" ;
      else returnNode->mData = "nil" ;
      // else cout << "wtf\n" ;
      return returnNode ;
    } // else if
    else if ( !curIns.compare( "and" ) ) {
      if ( CheckArgumentNum( head->mRight ) < 2 ) { // when cons' argument not equal one. 
        string errormessage = "ERROR (incorrect number of arguments) : " + curIns ;
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (incorrect number of arguments) : %s\n", curIns.c_str() ) ;
        throw errormessage.c_str() ;
      } // if
      
      for ( Node* cur = head->mRight ; cur != NULL ; cur = cur->mRight ) { // evaluate each argument
        Node* returnNode ;
        try {
          returnNode = EvalS_exp( cur->mLeft, storeArea ) ;
          if ( returnNode == NULL ) {
            string error = "ERROR (unbound condition) : " ;
            cout << error ;
            PrintErrorS_exp( cur->mLeft ) ;
            throw "ERROR (unbound condition) : " ;
          } // if
        } // try
        catch ( const char* error ) {
          string tmp( error ) ;
          // cur->mLeft->mData = Purcedure2Normal( cur->mLeft->mData ) ;          
          if ( !strcmp( error, "ERROR (no return value) : " ) ) {
            error = "ERROR (unbound condition) : " ;
            cout << error ;
            PrintErrorS_exp( cur->mLeft ) ;
          } // if
          
          throw error ;
        } // catch
     
        if ( WhichType( returnNode->mData ) == "nil" ) {
          
          return returnNode ;
        } // if
        else if ( cur->mRight == NULL || WhichType( cur->mRight->mData ) == "nil" ) {
          return returnNode ;
        } // else if
      } // for
      
      // ClearTree( head ) ; // didn't clearn
      
    } // else if
    else if ( !curIns.compare( "or" ) ) {
      if ( CheckArgumentNum( head->mRight ) < 2 ) { // when cons' argument not equal one. 
        string errormessage = "ERROR (incorrect number of arguments) : " + curIns ;
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (incorrect number of arguments) : %s\n", curIns.c_str() ) ;
        throw errormessage.c_str() ;
      } // if
      
      for ( Node* cur = head->mRight ; cur != NULL ; cur = cur->mRight ) { // evaluate each argument
        Node* returnNode ;
        try {
          returnNode = EvalS_exp( cur->mLeft, storeArea ) ;
          if ( returnNode == NULL ) {
            string error = "ERROR (unbound condition) : " ;
            cout << error ;
            PrintErrorS_exp( cur->mLeft ) ;
            throw "ERROR (unbound condition) : " ;
          } // if
        } // try
        catch ( const char* error ) {
          string tmp( error ) ;
          // cur->mLeft->mData = Purcedure2Normal( cur->mLeft->mData ) ;          
          if ( !strcmp( error, "ERROR (no return value) : " ) ) {
            error = "ERROR (unbound condition) : " ;
            cout << error ;
            PrintErrorS_exp( cur->mLeft ) ;
          } // if
          
          throw error ;
        } // catch
        
        if ( WhichType( returnNode->mData ) != "nil" ) return returnNode ;
        else if ( cur->mRight == NULL || WhichType( cur->mRight->mData ) == "nil" ) {
          return returnNode ;
        } // else if
      } // for
      
      // ClearTree( head ) ; // perhaps non error
      
    } // else if
    else if ( !curIns.compare( ">" ) || !curIns.compare( ">=" ) 
              || !curIns.compare( "<" ) || !curIns.compare( "<=" )
              || !curIns.compare( "=" ) ) {
      if ( CheckArgumentNum( head->mRight ) < 2 ) { // when cons' argument not equal one. 
        string errormessage = "ERROR (incorrect number of arguments) : " + curIns ;
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (incorrect number of arguments) : %s\n", curIns.c_str() ) ;
        throw errormessage.c_str() ;
      } // if
      
      // for( int i = 0 ; i < storeArea.mDefines.size() ; i++ )
      //   PrintErrorS_exp( storeArea.mDefines.at(i)) ;
      try {
        head = Eval_Compare( head, storeArea ) ;
      } // try
      catch ( const char* error ) {
        throw error ;
      } // catch
      
    } // else if
    else if ( !curIns.compare( "string-append" ) || !curIns.compare( "string>?" )
              || !curIns.compare( "string<?" ) || !curIns.compare( "string=?" ) ) {
      if ( CheckArgumentNum( head->mRight ) < 2 ) { // when cons' argument not equal one. 
        string errormessage = "ERROR (incorrect number of arguments) : " + curIns ;
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (incorrect number of arguments) : %s\n", curIns.c_str() ) ;
        throw errormessage.c_str() ;
      } // if
      
      try {
        head = Eval_String( head, storeArea ) ;
      } // try
      catch ( const char* error ) {
        throw error ;
      } // catch
      
    } // else if
    else if ( !curIns.compare( "eqv?" ) ) {
      if ( CheckArgumentNum( head->mRight ) != 2 ) { // when cons' argument not equal one. 
        string errormessage = "ERROR (incorrect number of arguments) : " + curIns ;
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (incorrect number of arguments) : %s\n", curIns.c_str() ) ;
        throw errormessage.c_str() ;
      } // if
      
      try {
        head = Eval_eqv( head, storeArea ) ;
      } // try
      catch ( const char* error ) {
        throw error ;
      } // catch
      
    } // else if
    else if ( !curIns.compare( "equal?" ) ) {
      if ( CheckArgumentNum( head->mRight ) != 2 ) { // when cons' argument not equal one. 
        string errormessage = "ERROR (incorrect number of arguments) : " + curIns ;
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (incorrect number of arguments) : %s\n", curIns.c_str() ) ;
        throw errormessage.c_str() ;
      } // if
      
      try {
        head = Eval_equal( head, storeArea ) ;
      } // try
      catch ( const char* error ) {
        throw error ;
      } // catch
      
    } // else if
    else if ( !curIns.compare( "begin" ) ) {
      if ( CheckArgumentNum( head->mRight ) < 1 ) { // when cons' argument not equal one. 
        string errormessage = "ERROR (incorrect number of arguments) : " + curIns ;
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (incorrect number of arguments) : %s\n", curIns.c_str() ) ;
        throw errormessage.c_str() ;
      } // if
      
      Node* reNode ;
      for ( Node* cur = head->mRight ; cur != NULL ; cur = cur->mRight ) {
        try {
          reNode = EvalS_exp( cur->mLeft, storeArea ) ;
        } // try
        catch ( const char* error ) {
          string tmp( error ) ;
          if ( strcmp( error, "ERROR (no return value) : " ) ) {
            head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
            // cur->mLeft->mData = Purcedure2Normal( cur->mLeft->mData ) ;
            throw error ;
          } // if
          else if ( !strcmp( error, "ERROR (no return value) : " ) ) {
            // cur->mLeft->mData = Purcedure2Normal( cur->mLeft->mData ) ; // ! /7.16
            reNode = NULL ;
          } // else if
        } // catch
      } // for
      
      if ( reNode == NULL ) {
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        throw "ERROR (no return value) : " ;
      } // if
      
      return reNode ;
    } // else if
    else if ( !curIns.compare( "if" ) ) {
      int argNum = CheckArgumentNum( head->mRight ) ;
      Node* reNode ;
      Node* arg1, *arg2 ;
      
      if ( argNum != 2 && argNum != 3 ) { // when cons' argument not equal one. 
        string errormessage = "ERROR (incorrect number of arguments) : " + curIns ;
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (incorrect number of arguments) : %s\n", curIns.c_str() ) ;
        throw errormessage.c_str() ;
      } // if
      
      try {
        arg1 = EvalS_exp( head->mRight->mLeft, storeArea ) ;
      } // try
      catch ( const char* error ) {
        string tmp( error ) ;
        if ( !strcmp( error, "ERROR (no return value) : " ) ) {
          error = "ERROR (unbound test-condition) : " ;
          cout << error ;
          PrintErrorS_exp( head->mRight->mLeft ) ;
        } // if
        
        throw error ;
      } // catch
      
      if ( argNum == 2 ) {
        if ( WhichType( arg1->mData ) == "nil" ) {
          head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
          throw "ERROR (no return value) : " ;
          
        } // if
        
        try {
          arg2 = EvalS_exp( head->mRight->mRight->mLeft, storeArea ) ;
        } // try
        catch ( const char* error ) {
          throw error ;
        } // catch
        
        if ( WhichType( arg1->mData ) != "nil" ) {
          reNode = arg2 ;
        } // if
        else {  // no false option
          head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
          throw "ERROR (no return value) : " ;
        } // else
      } // if
      else if ( argNum == 3 ) {
        Node* arg3 ;
        if ( arg1 != NULL && WhichType( arg1->mData ) != "nil" ) {
          try {
            arg2 = EvalS_exp( head->mRight->mRight->mLeft, storeArea ) ;
          } // try
          catch ( const char* error ) {
            throw error ;
          } // catch
          
          reNode = arg2 ;
        } // if
        else if ( arg1 != NULL && WhichType( arg1->mData ) == "nil" ) {
          try {
            arg3 = EvalS_exp( head->mRight->mRight->mRight->mLeft, storeArea ) ;
          } // try
          catch ( const char* error ) {
            throw error ;
          } // catch
            
          reNode = arg3 ;
        } // else if
        else {  // no false option
          throw "ERROR (no return value) : " ;
          // cout << "Something didn't consider about.\n" ;
        } // else
        
        
      } // else if
      
      return reNode ;
    } // else if
    else if ( !curIns.compare( "cond" ) ) {
      int argNum = CheckArgumentNum( head->mRight ) ;
      Node* reNode = NULL ;
      
      if ( argNum < 1 ) { // when cons' argument not equal one. 
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (COND format) : " ) ;
        PrintErrorS_exp( head ) ;
        throw "ERROR (COND format) : " ;
      } // if
      
      
      Node* cur = head->mRight ;
      for (  ; cur != NULL ; cur = cur->mRight ) {
        if ( cur->mRight == NULL && cur->mLeft == NULL 
             && WhichType( cur->mData ) == "nil" ) {
          ;
        } // if
        else if ( !cur->mLeft->mData.compare( "\0" ) || !cur->mLeft->mData.compare( "(" ) ) {
          // this S_exp's arg > 1 --> legal
          if ( CheckArgumentNum( cur->mLeft ) <= 1 ) {
            head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
            printf( "ERROR (COND format) : " ) ;
            PrintErrorS_exp( head ) ;
            throw "ERROR (COND format) : " ;
          } // if
        } // else if
        else if ( cur->mLeft->mData.compare( "\0" ) && cur->mLeft->mData.compare( "(" ) ) {
          head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
          printf( "ERROR (COND format) : " ) ;
          PrintErrorS_exp( head ) ;
          throw "ERROR (COND format) : " ;
        } // if
      } // for
      
      for ( Node* walk = head->mRight ; walk != NULL  ; walk = walk->mRight ) {
        Node* arg = walk->mLeft ;
        Node* condition ;
        int thisArgNum = CheckArgumentNum( arg ) ;
        if ( thisArgNum > 1 ) {
          if ( arg->mLeft->mData.compare( "else" ) || walk->mRight != NULL ) {
            try {
              condition = EvalS_exp( arg->mLeft, storeArea ) ;
            } // try
            catch ( const char* error ) {
              string tmp( error ) ;
              // cout << "test : " << error << endl ;
              head->mRight->mLeft->mData = Purcedure2Normal( head->mRight->mLeft->mData ) ;          
             
              if ( !strcmp( error, "ERROR (no return value) : " ) ) {
                error = "ERROR (unbound test-condition) : " ;
                cout << error ;
                PrintErrorS_exp( arg->mLeft ) ;
              } // if
              
              throw error ;
            } // catch
            
            if ( condition == NULL ) return NULL ;
          } // if
            
          if ( WhichType( condition->mData ) != "nil"
               || ( !arg->mLeft->mData.compare( "else" ) && walk->mRight == NULL ) ) {
            for ( Node* cur = arg->mRight ; cur != NULL ; cur = cur->mRight ) {
              try {
                reNode = EvalS_exp( cur->mLeft, storeArea ) ;
                
              } // try
              catch ( const char* error ) {
                if ( strcmp( error, "ERROR (no return value) : " ) ) {
                  head->mRight->mLeft->mData = Purcedure2Normal( head->mRight->mLeft->mData ) ;  
                  throw error ;
                } // if
                else if ( !strcmp( error, "ERROR (no return value) : " ) )
                  reNode = NULL ;
              } // catch
              // reNode != NULL && reNode->mLeft != NULL ? 
              // cout << reNode->mLeft->mData  << "<--" << endl : cout << "NULL<--\n" ;
            } // for
            
            if ( reNode == NULL ) {
              head->mRight->mLeft->mData = Purcedure2Normal( head->mRight->mLeft->mData ) ;  
              throw "ERROR (no return value) : " ;
            } // if
            
            return reNode ;
          } // if 
        } // if
        else if ( thisArgNum < 2 ) {
          printf( "ERROR (COND format) : " ) ;
          PrintErrorS_exp( head ) ;
          throw "ERROR (COND format) : " ;
        } // else if
        // else cout << "Something impossible.\n" ;
        
      } // for
      
      return NULL ;
    } // else if
    else if ( !curIns.compare( "clean-environment" ) && head == gRoot ) {
      int argNum = CheckArgumentNum( head->mRight ) ;
      
      if ( argNum > 0 ) { // when cons' argument not equal one. 
        string errormessage = "ERROR (incorrect number of arguments) : " + curIns ;
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (incorrect number of arguments) : %s\n", curIns.c_str() ) ;
        throw errormessage.c_str() ;
      } // if
      
      storeArea.mLocal.clear() ;
      for ( int i = 0 ; i < storeArea.mDefines.size() ; i++ ) {
        ClearTree( storeArea.mDefines.at( i ) ) ; 
        storeArea.mDefines.at( i ) = NULL ; 
      } // for
      
      for ( int i = 0 ; i < storeArea.mLocal.size() ; i++ ) {
        ClearTree( storeArea.mLocal.at( i ) ) ; 
        storeArea.mLocal.at( i ) = NULL ; 
      } // for
      
      
      storeArea.mDefines.resize( 0 ) ;
      storeArea.mLocal.resize( 0 ) ;
      // cout << storeArea.mDefines.size() << endl ;
      head->mData = "environment cleaned" ;
      head->mRight = NULL ;
      head->mLeft = NULL ;
      InitialStoreArea( storeArea ) ;
    } // else if
    else if ( !curIns.compare( "let" ) ) {
      int argNum = CheckArgumentNum( head->mRight ) ;
      if ( argNum < 2 ) { // when cons' argument not equal one. 
        string errormessage = "ERROR (LET format) : " ;
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (LET format) : " ) ;
        PrintErrorS_exp( head ) ;
        throw errormessage.c_str() ;
      } // if
      
      head = Eval_Let( head, storeArea ) ;
      // storeArea.mLocal.clear() ; // what time does the local variable should be clear?
      return head ;
    } // else if
    else if ( !curIns.compare( "lambda" ) ) {
      try {
        head = Eval_Lambda( head, storeArea ) ;
      } // try
      catch( const char* error ) {
        throw error ;
      } // catch
      
      return head ;
    } // else if
    else if ( !curIns.compare( "exit" ) && head == gRoot ) {
      int argNum = CheckArgumentNum( head->mRight ) ;
      if ( argNum > 0 ) { // when cons' argument not equal one. 
        string errormessage = "ERROR (incorrect number of arguments) : " + curIns ;
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (incorrect number of arguments) : %s\n", curIns.c_str() ) ;
        throw errormessage.c_str() ;
      } // if
      
      head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
      return head ;
    } // else if
    else { // ins level error
      // cout << "test : " << gRootIns << endl ;
      if ( !curIns.compare( "clean-environment" ) ) {
        printf( "ERROR (level of CLEAN-ENVIRONMENT)\n" ) ;
        throw "ERROR (level of CLEAN-ENVIRONMENT)" ;
      } // if
      else if ( !curIns.compare( "define" ) ) {
        printf( "ERROR (level of DEFINE)\n" ) ;
        throw "ERROR (level of DEFINE)" ;
      } // if
      else if ( !curIns.compare( "exit" ) ) {
        printf( "ERROR (level of EXIT)\n" ) ;
        throw "ERROR (level of EXIT)" ;
      } // if
      else {
        printf( "ERROR (attempt to apply non-function) : " ) ;
        PrintErrorS_exp( head->mLeft ) ;  
        throw "ERROR (attempt to apply non-function) : " ; 
      } // else
      
    } // else
    
    return head ;
  } // else
  
  return NULL ;
} // EvalS_exp()

bool GetS_exp( vector<string> &tokenString, string perToken, Node* perNode ) {
  string nextToken ;
  // follow syntax
  if ( WhichType( perToken ) == "SYMBOL" || WhichType( perToken ) == "INT" 
       || WhichType( perToken ) == "FLOAT" || WhichType( perToken ) == "STRING" 
       || WhichType( perToken ) == "nil" || WhichType( perToken ) == "#t" ) { // ATOM
     
    tokenString.push_back( perToken ) ;
    perNode->mData = perToken ;
    return true ;
  } // if
  else if ( WhichType( perToken ) == "LEFT-PAREN" ) {
    tokenString.push_back( perToken ) ;
    // perNode->mData = perToken ; // put '(' into mData
    perNode->mLeft = new Node() ;
    try { 
      nextToken = GetToken() ; // get nextToken string
    } // try
    catch( char const* error  ) {
      throw error ;
    } // catch
    
    if ( !perToken.compare( "(" ) && !nextToken.compare( ")" ) ) {
      if ( tokenString.size() > 0 ) {
        tokenString.at( tokenString.size()-1 ) = "()" ;
      } // if
      else {
        tokenString.push_back( "()" ) ;
      } // else
        
      perNode->mData = "()" ;
      perNode->mLeft = NULL ; // mRight should be NULL
      return true ;
    } // if
    
    try {
      if ( WhichType( nextToken ) != "DOT" && GetS_exp( tokenString, nextToken, perNode->mLeft ) ) {
        try { 
          nextToken = GetToken() ; // get nextToken string
        } // try
        catch( char const* error  ) {
          throw error ;
        } // catch
        
        while ( WhichType( nextToken ) != "DOT" && WhichType( nextToken ) != "RIGHT-PAREN" ) { 
          // Should get S_exp
          // tokenString.push_back( nextToken ) ;
          perNode->mRight = new Node() ;
          perNode = perNode->mRight ;
          perNode->mLeft = new Node() ;
          try {
            GetS_exp( tokenString, nextToken, perNode->mLeft ) ; 
          } // try
          catch( char const* error ) {
            throw error ;
          } // catch
          
          try { 
            nextToken = GetToken() ; // get nextToken string
          } // try
          catch( char const* error  ) {
            throw error ;
          } // catch
          
        } // while
        
        if ( WhichType( nextToken ) == "DOT" ) {
          tokenString.push_back( nextToken ) ;
          // perNode->mData = "." ;
          try { 
            nextToken = GetToken() ; // get nextToken string
          } // try
          catch( char const* error  ) {
            throw error ;
          } // catch
          
          perNode->mRight = new Node() ;
          if ( GetS_exp( tokenString, nextToken, perNode->mRight ) ) {
            try { 
              nextToken = GetToken() ; // get nextToken
            } // try
            catch( char const* error  ) {
              throw error ;
            } // catch
            
            if ( WhichType( nextToken ) == "RIGHT-PAREN" ) {
              tokenString.push_back( nextToken ) ;
              return true ;
            } // if 
            else { // ERROR (unexpected token) : ')' expected when token at Line X Column Y is >>...<<
              string errorTmp = "ERROR (unexpected token) : ')' expected when token at Line " ;
              char tmp[30] ;
              stringstream out ;
              out << gLn ;
              errorTmp = errorTmp + Itoa( gLn ) + " Column " + Itoa( gCol ) + " is >>" + nextToken + "<<\n" ;
              const char* ERRORMESSAGE = errorTmp.c_str() ;
              throw ERRORMESSAGE ; // error
              return false ; // shouldn't do it
            } // else
          } // if
          else {
            string errorTmp = "ERROR (unexpected token) : atom or '(' expected when token at Line " ;
            char tmp[30] ;
            errorTmp = errorTmp + Itoa( gLn ) + " Column " + Itoa( gCol ) + " is >>" + nextToken + "<<\n" ;
            const char* ERRORMESSAGE = errorTmp.c_str() ;
            throw ERRORMESSAGE ; // error
            return false ;
          } // else
        } // if
        else if ( WhichType( nextToken ) == "RIGHT-PAREN" ) {
          tokenString.push_back( nextToken ) ;
          return true ;
        } // else if
        else {
          string errorTmp = "ERROR (unexpected token) : ')' expected when token at Line " ;
          char tmp[30] ;
    
          errorTmp = errorTmp + Itoa( gLn ) + " Column " + Itoa( gCol ) + " is >>" + nextToken + "<<\n" ;
          const char* ERRORMESSAGE = errorTmp.c_str() ;
          throw ERRORMESSAGE ; // error
          return false ; // shouldn't do it
        } // else
      } // if
      else {    // ERROR (unexpected token) : atom or '(' expected when token at Line X Column Y is >>...<<
        string errorTmp = "ERROR (unexpected token) : atom or '(' expected when token at Line " ;
        char tmp[30] ;
  
        errorTmp = errorTmp + Itoa( gLn ) + " Column " + Itoa( gCol ) + " is >>" + nextToken + "<<\n" ;
        const char* ERRORMESSAGE = errorTmp.c_str() ;
        throw ERRORMESSAGE ;
      } // else
    } // try
    catch( char const* error  ) {
      throw error ;
    } // catch
  } // else if
  else if ( WhichType( perToken ) == "QUOTE" ) {
    tokenString.push_back( perToken ) ;
    perNode->mLeft = new Node() ;
    perNode->mLeft->mData = "quote" ;
    perNode->mRight = new Node() ;
    perNode = perNode->mRight ;
    perNode->mLeft = new Node() ;
    try { 
      nextToken = GetToken() ; // get nextToken string
    } // try
    catch( char const* error  ) {
      throw error ;
    } // catch
    
    try {   
      if ( GetS_exp( tokenString, nextToken, perNode->mLeft ) ) return true ;
      else { // ERROR (unexpected token) : atom or '(' expected when token at Line X Column Y is >>...<<
        string errorTmp = "ERROR (unexpected token) : atom or '(' expected when token at Line " ;
        char tmp[30] ;
  
        errorTmp = errorTmp + Itoa( gLn ) + " Column " + Itoa( gCol ) + " is >>" + nextToken + "<<\n" ;
        const char* ERRORMESSAGE = errorTmp.c_str() ;
        throw ERRORMESSAGE ; // error
        return false ;
      } // else
    } // try
    catch( char const* error ) {
      throw error ;
    } // catch
  } // else if
  else {
    string errorTmp = "ERROR (unexpected token) : atom or '(' expected when token at Line " ;
    char tmp[30] ;

    errorTmp = errorTmp + Itoa( gLn ) + " Column " + Itoa( gCol ) + " is >>" + perToken + "<<\n" ;
    const char* ERRORMESSAGE = errorTmp.c_str() ;
    throw ERRORMESSAGE ; // error
    return false ;
  } // else
} // GetS_exp()

string GetToken() {
  // cout << "the token in GetToken is :" << (char)cin.peek() << "," << cin.peek() << "<-------\n" ;
  // cout << "---Ready to GetToken---\n" ;
  string returnToken = "\0" ;
  bool hasAToken = false, allNonSeparator = true ;
  char cur, whiteSpace ;
  
  while ( !IsWhiteSpace( cin.peek() ) && cin.peek() != EOF ) { // need count line and column
    hasAToken = true ;
    if ( IsSeparator( cin.peek() ) && cur != '\\' && returnToken.compare( "\0" ) ) {
      return returnToken ;
    } // if
    else if ( IsSeparator( cin.peek() ) ) {
      char pre = cur ;
      if ( cin >> cur ) {
        if ( cur == '\n' ) {
          gCol = 0 ;
          gLn++ ;
        } // if
        else 
          gCol++ ;
      } // if
      else throw "ERROR (no more input) : END-OF-FILE encountered" ; // throw exception
      
      if ( cur == ';' ) { // process comment
        char tmp ;
        
        while ( cin.peek() != '\n' ) {
          if ( cin >> noskipws >> tmp ) {
            if ( tmp == '\n' ) {
              gCol = 0 ;
              gLn++ ;
            } // if
            else {
              gCol++ ; // get nextToken string
            } // else
          } // if
          else throw "ERROR (no more input) : END-OF-FILE encountered" ; // throw exception
        } // while
        
        if ( cin >> noskipws >> tmp ) {
          if ( tmp == '\n' ) {
            gCol = 0 ;
            gLn++ ;
          } // if
          else {
            gCol++ ; // get nextToken string
          } // else
        } // if
        else throw "ERROR (no more input) : END-OF-FILE encountered" ; // throw exception
        
        try {
          // gLn-- ;
          returnToken = GetToken() ;
        } // try
        catch ( const char* error ) {
          throw error ;
        } // catch
        
        return returnToken ; // false will error ?
      } // if
      
      if ( cur == '"' ) { // input string
        // cout << "start inpput String\n" ;
        try {
          if ( InputString( returnToken += cur ) )
            return returnToken ;
        } // try
        catch ( const char* error ) {
          throw error ;
        } // catch
      } // if
      
      
      // cout << "test in GetToken : " << returnToken + cur << endl ;
      return ( returnToken = returnToken + cur ) ;
    } // else if 
    else {
      if ( cin >> cur ) {
        if ( cur == '\n' ) {
          gCol = 0 ;
          gLn++ ;
        } // if
        else 
          gCol++ ;
      } // if 
      else throw "ERROR (no more input) : END-OF-FILE encountered" ;
      
      if ( !returnToken.compare( "\0" ) ) returnToken = cur ; // returnToken is NUUL
      else returnToken = returnToken + cur ;
    } // else
    
  } // while
  
  if ( !hasAToken ) {
    char whiteSpace ;
    if ( cin >> noskipws >> whiteSpace ) {
      if ( whiteSpace == '\n' ) {
        gCol = 0 ;
        gLn++ ;
      } // if
      else 
        gCol++ ;
    } // if
    else throw "ERROR (no more input) : END-OF-FILE encountered" ;
    
    try {
      returnToken = GetToken() ;
      
    } // try
    catch ( const char* error ) {
      throw error ;
    } // catch
    
    return returnToken ;
  } // if
  
  return returnToken ;
} // GetToken()

Node* Eval_Lambda( Node* head, StoreArea &storeArea ) {
  Node* reNode ;
  Node* lambda = NULL ;
  vector< Node* > localDefines, perLocal ;
  // reNode->mData = "#<procedure lambda>" ;
  // cout << "test lambda start \n" ;
  // PrintErrorS_exp( head ) ;
  
  for ( int i = 0; i < storeArea.mDefines.size() ; i++ ) { // lambda function can 
    if ( storeArea.mDefines.at( i )->mLeft != NULL
         && !storeArea.mDefines.at( i )->mLeft->mData.compare( "#<procedure lambda>" ) ) {
      lambda = storeArea.mDefines.at( i ) ;
      i = storeArea.mDefines.size() ;
    } // if
  } // for
  
  if ( lambda != NULL && head->mRight != NULL && head->mRight->mLeft != NULL 
       && ( !head->mRight->mLeft->mData.compare( "\0" ) 
            || !head->mRight->mLeft->mData.compare( "(" )
            || !head->mRight->mLeft->mData.compare( "()" ) )
       && CheckArgumentNum( head ) > 2  ) {
    bool shouldDefine = true ;
    for ( int index = 0 ; index < storeArea.mDefines.size() ; index++ ) {
      if ( storeArea.mDefines.at( index )->mLeft != NULL && head->mRight->mLeft->mLeft != NULL ) {
        if ( !storeArea.mDefines.at( index )->mLeft->mData.compare( head->mRight->mLeft->mLeft->mData ) 
             && !storeArea.mDefines.at( index )->mRight->mLeft->mData.compare( "#<procedure lambda>" ) ) {
          shouldDefine = false ;                         
          index = storeArea.mDefines.size() ;
        } // if
      
      } // if
    } // for
    
    if ( shouldDefine ) lambda = NULL ;
  } // if
  
  if ( lambda != NULL ) { // eval lambda
    Node* funcParameter = lambda->mRight->mLeft ; // lambda function's arg num
    Node* args = head->mRight ;
    if ( CheckArgumentNum( args ) == CheckArgumentNum( funcParameter ) ) {
      Node* walkArg = args ;
      
      for ( Node* walkFunc = funcParameter; walkFunc != NULL && walkFunc->mLeft != NULL  
            ; walkFunc = walkFunc->mRight ) {
        // define local variable
        Node* localDefine = new Node() ;
        localDefine->mLeft = new Node() ;
        localDefine->mRight = new Node() ;
        localDefine->mData = "(" ;
        localDefine->mLeft->mData = walkFunc->mLeft->mData ; // localVar name
        localDefine->mRight->mLeft = new Node() ;
        try {
          localDefine->mRight->mLeft->mData = EvalS_exp( walkArg->mLeft, storeArea )->mData ; // arg
        } // try
        catch ( const char* error ) {
          head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
          // walkArg->mLeft->mData = Purcedure2Normal( walkArg->mLeft->mData ) ;
          if ( !strcmp( error, "ERROR (no return value) : " ) ) {
            error = "ERROR (unbound parameter) : " ;
            printf( "%s", error ) ;
            PrintErrorS_exp( walkArg->mLeft ) ;
            throw "error" ;
          } // if
          
          throw error ;
        } // catch
      
        localDefines.push_back( localDefine ) ;  
        walkArg = walkArg->mRight ;
      } // for
      
      for ( int index = 0 ; index < storeArea.mLocal.size() ; index++ ) {
        // store local variable to let next func can't use
        perLocal.push_back( storeArea.mLocal.at( index ) ) ;
      } // for
      
      for ( Node* content = lambda->mRight->mRight; content != NULL ; content = content->mRight ) {
        storeArea.mLocal.clear() ;
        for ( int i = 0; i < localDefines.size() ; i++ ) {        
          storeArea.mLocal.push_back( localDefines.at( i ) ) ;
        } // for
          
        try {  
          reNode = EvalS_exp( content->mLeft, storeArea ) ; // eval function content
        } // try
        catch ( const char* error ) {
          if ( strcmp( error, "ERROR (no return value) : " ) ) {
            head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
            storeArea.mLocal.clear() ;
            for ( int index = 0 ; index < perLocal.size() ; index++ ) {
              storeArea.mLocal.push_back( perLocal.at( index ) ) ;
            } // for
            
            throw error ;  
          } // if
          else if ( !strcmp( error, "ERROR (no return value) : " ) )
            reNode = NULL ;
            
        } // catch()  
      } // for
      
    } // if
    else {
      head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
      printf( "ERROR (incorrect number of arguments) : lambda\n" ) ;
      throw "function error\n" ;
    } // else
  } // if
  else if ( lambda == NULL && CheckArgumentNum( head->mRight ) == 0 ) {
    head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
    printf( "ERROR (LAMBDA format) : " ) ;
    PrintErrorS_exp( head ) ;
    throw "ERROR (LAMBDA format) : " ;
  } // if
  else if ( lambda == NULL && CheckArgumentNum( head->mRight ) >= 2 ) { // define lambda
    // Node* pairs = head->mRight->mLeft ;
    reNode = new Node() ;
    if ( head->mRight != NULL && head->mRight->mLeft != NULL 
         && head->mRight->mLeft->mData.compare( "\0" ) 
         && head->mRight->mLeft->mData.compare( "(" )
         && head->mRight->mLeft->mData.compare( "()" ) ) {
      head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
      printf( "ERROR (LAMBDA format) : " ) ;
      PrintErrorS_exp( head ) ;
      throw "ERROR (LAMBDA format) : " ;
    } // if
    else if ( head->mRight != NULL && head->mRight->mLeft != NULL 
              && head->mRight->mLeft->mLeft != NULL
              && ( WhichType( head->mRight->mLeft->mLeft->mData ) != "SYMBOL"
                   || IsIns( Purcedure2Normal( head->mRight->mLeft->mLeft->mData ) ) ) ) {
      head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
      printf( "ERROR (LAMBDA format) : " ) ;
      PrintErrorS_exp( head ) ;
      throw "ERROR (LAMBDA format) : " ;          
    } // else if
    else {
      if ( head->mRight != NULL && head->mRight->mLeft != NULL )
        for ( Node* walk = head->mRight->mLeft ; walk != NULL ; walk = walk->mRight ) {
          if ( walk->mData.compare( "\0" ) && walk->mData.compare( "(" ) ) {
            head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
            printf( "ERROR (LAMBDA format) : " ) ;
            PrintErrorS_exp( head ) ;
            throw "ERROR (LAMBDA format) : " ;    
          } // if
        } // for
      
    } // else
    
    for ( int i = 0 ; i < storeArea.mDefines.size() ; i++ ) {
      if ( storeArea.mDefines.at( i )->mLeft != NULL 
           && !storeArea.mDefines.at( i )->mLeft->mData.compare( "#<procedure lambda>" ) ) {
        storeArea.mDefines.erase( storeArea.mDefines.begin() + i ) ;
      } // if
    } // for
    
    storeArea.mDefines.push_back( head ) ;
    reNode->mData = "#<procedure lambda>" ;
  } // else if 
  else {
    head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
    printf( "ERROR (LAMBDA format) : " ) ;
    PrintErrorS_exp( head ) ;
    throw "ERROR (LAMBDA format) : " ;
  } // else
  // define lambda in global
  // call lamda when
  
  storeArea.mLocal.clear() ;
  for ( int index = 0 ; index < perLocal.size() ; index++ ) {
    storeArea.mLocal.push_back( perLocal.at( index ) ) ;
  } // for
  
  if ( reNode == NULL ) {
    // head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
    for ( int i = 0; i < localDefines.size() ; i++ ) {        
      storeArea.mLocal.pop_back() ;
    } // for
    
    throw "ERROR (no return value) : " ;
  } // if
  
  return reNode ;
} // Eval_Lambda()

Node* Eval_Let( Node* head, StoreArea &storeArea ) {
  Node* reNode, *copyPair ;
  Node* pairs = head->mRight->mLeft ;
  Node* s_exps = head->mRight->mRight ;
  vector< Node* > localDefines, copyPairs ;
  int copyIndex = 0 ;
  // check let should be a list of two-element lists.
  if ( pairs != NULL && pairs->mData.compare( "()" ) ) {
    for ( Node* check = pairs ; check != NULL ; check = check->mRight ) {
      if ( ( check->mLeft != NULL && CheckArgumentNum( check->mLeft ) != 2 )
           || ( check->mLeft != NULL && check->mLeft->mRight != NULL 
                && check->mLeft->mRight->mRight != NULL
                && WhichType( check->mLeft->mRight->mRight->mData ) != "nil" ) ) {
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (LET format) : " ) ;
        PrintErrorS_exp( head ) ;
        throw "ERROR (LET format) : " ;
      } // if
      else if ( check->mLeft != NULL && check->mLeft->mLeft != NULL
                && ( WhichType( check->mLeft->mLeft->mData ) != "SYMBOL"
                     || IsIns( Purcedure2Normal( check->mLeft->mLeft->mData ) ) ) ) {
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (LET format) : " ) ;
        PrintErrorS_exp( head ) ;
        throw "ERROR (LET format) : " ;
      } // else if
      else if ( check->mData.compare( "\0" ) && check->mData.compare( "(" ) ) {
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        printf( "ERROR (LET format) : " ) ;
        PrintErrorS_exp( head ) ;
        throw "ERROR (LET format) : " ;
      } // else if
    } // for
  } // if
  
  for ( Node* walk = pairs; walk != NULL ; walk = walk->mRight ) {
    if ( walk->mLeft != NULL ) {
      Node* localDefine = walk->mLeft ;
      copyPair = new Node() ;
      CopyTree( localDefine, copyPair ) ;
      copyPairs.push_back( copyPair ) ;
      try {
        localDefine->mRight->mLeft = EvalS_exp( localDefine->mRight->mLeft, storeArea ) ;
      } // try
      catch ( const char* error ) {
        string tmp( error ) ;
        localDefine->mRight->mLeft->mData 
          = Purcedure2Normal( localDefine->mRight->mLeft->mData ) ; 
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        if ( !strcmp( error, "ERROR (no return value) : " ) ) {
          error = "ERROR (no return value) : " ;
          cout << error ;
          PrintErrorS_exp( localDefine->mRight->mLeft ) ;
          error = "error" ;
        } // if
        
        throw error ;
      } // catch()
            
      localDefines.push_back( localDefine ) ;
    } // if
  } // for
    
  for ( Node* walk = pairs; walk != NULL ; walk = walk->mRight ) {
    if ( walk->mLeft != NULL ) {
      walk->mLeft = copyPairs.at( copyIndex++ ) ; 
    } // if
  } // for
  
  for ( Node* walk = s_exps; walk != NULL ; walk = walk->mRight ) {
    try {
      for ( int i = 0; i < localDefines.size() ; i++ ) {    
        storeArea.mLocal.push_back( localDefines.at( i ) ) ;
      } // for
      
      reNode = EvalS_exp( walk->mLeft, storeArea ) ;
    } // try
    catch ( const char* error ) {
      if ( strcmp( error, "ERROR (no return value) : " ) ) {
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        storeArea.mLocal.clear() ;
        throw error ;  
      } // if
      else if ( !strcmp( error, "ERROR (no return value) : " ) )
        reNode = NULL ;
      
    } // catch()
  } // for
  
  // storeArea.mLocal.clear() ;
  if ( reNode == NULL ) {
    head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
    throw "ERROR (no return value) : " ;  
  } // if
  
  return reNode ;
} // Eval_Let()

Node* Eval_Define( Node* head, StoreArea &storeArea ) { // initial head->mLeft->mData is defined name
  string definedName ;
  Node* arg1, *arg2 ;
  
  head = head->mRight ;
  // delete( trash->mLeft ) ; // delete the node point to "define"
  // delete( trash ) ; // delete the node "define"
  arg1 = head->mLeft ;
  arg2 = head->mRight->mLeft ;
  if ( !arg1->mData.compare( "\0" ) || !arg1->mData.compare( "(" ) ) {
    try {
      Eval_DefineFunc( head, storeArea ) ; // define func
      return NULL ;
    } // try
    catch ( const char* error ) {
      throw "ERROR (DEFINE format) : " ;
    } // catch()
  } // if
  else if ( WhichType( arg1->mData ) != "SYMBOL"
            || IsIns( Purcedure2Normal( arg1->mData ) ) ) {
    printf( "ERROR (DEFINE format) : " ) ;
    throw "ERROR (DEFINE format) : " ;
  } // if
  
  try {
    // cout << arg2->mData << endl ;
    head->mRight->mLeft = EvalS_exp( arg2, storeArea ) ;
    
  } // try
  catch ( const char* error ) {
    string tmp( error ) ;
    arg2->mLeft->mData = Purcedure2Normal( arg2->mLeft->mData ) ;          
    if ( !strcmp( error, "ERROR (no return value) : " ) ) {
      error = "ERROR (no return value) : " ;
      cout << error ;
      PrintErrorS_exp( arg2 ) ;
      error = "error\n" ;
    } // if
    
    throw error ;
  } // catch
  
  // check same name
  for ( int i = 0 ; i < storeArea.mDefines.size() ; i++ ) {
    if ( storeArea.mDefines.at( i )->mLeft != NULL 
         && !storeArea.mDefines.at( i )->mLeft->mData.compare( arg1->mData ) ) {
      storeArea.mDefines.erase( storeArea.mDefines.begin() + i ) ;
    } // if
  } // for
  
  if ( !head->mRight->mLeft->mData.compare( "#<procedure lambda>" ) ) {
    for ( int i = 0; i < storeArea.mDefines.size() ; i++ ) {
      if ( storeArea.mDefines.at( i )->mLeft != NULL
           && !storeArea.mDefines.at( i )->mLeft->mData.compare( "#<procedure lambda>" ) ) {
        Node* lambda = new Node() ;
        lambda->mLeft = new Node() ;
        lambda->mRight = new Node() ;
        lambda->mLeft->mData = arg1->mData ;
        lambda->mRight = storeArea.mDefines.at( i ) ;
        storeArea.mDefines.at( i ) = lambda ;
      } // if
    } // for
  } // if
  else
    storeArea.mDefines.push_back( head )  ;
    
  if ( WhichType( arg1->mData ) == "SYMBOL" && !IsIns( arg1->mData ) ) 
    printf( "%s defined\n\n> ",  arg1->mData.c_str() ) ;
  else {  // if definedName is functoin key word, it's error!!!
  
    head->mRight->mLeft->mData = Purcedure2Normal( head->mRight->mLeft->mData ) ;
    printf( "ERROR (DEFINE format) : " ) ;
    throw "ERROR (DEFINE format) : " ; // ------test
  } // else
  
  return NULL ; // useless
} // Eval_Define()

Node* Eval_DefineFunc( Node* head, StoreArea &storeArea ) {
  string functionName ;
  Node* arg1 = head->mLeft ;
  Node* arg2 = head->mRight ; // heree
  Node* reNode = new Node() ;
  
  if ( CheckArgumentNum( head ) > 2 )
    arg2 = head->mRight ;
  
  reNode->mLeft = new Node() ;
  reNode->mRight = new Node() ; 
  if ( WhichType( arg1->mLeft->mData ) == "SYMBOL"
       && !IsIns( Purcedure2Normal( arg1->mLeft->mData ) ) ) {
    functionName = arg1->mLeft->mData ;
  } // if
  
  if ( WhichType( arg1->mLeft->mData ) != "SYMBOL" || arg2 == NULL ) {
    printf( "ERROR (DEFINE format) : " ) ;
    throw "ERROR (DEFINE format) : " ;
  } // if
  else {
    for ( Node* walk = arg1 ; walk != NULL ; walk = walk->mRight ) {
      if ( walk->mLeft != NULL 
           && ( IsIns( walk->mLeft->mData ) || WhichType( walk->mLeft->mData ) != "SYMBOL" ) ) {
        printf( "ERROR (DEFINE format) : " ) ;
        throw "ERROR (DEFINE format) : " ;
        
      } // if
      else if ( walk->mData.compare( "\0" ) && walk->mData.compare( "(" ) ) {
        printf( "ERROR (DEFINE format) : " ) ;
        throw "ERROR (DEFINE format) : " ;
      } // else if
    } // for
  } // else
   
  reNode->mLeft->mData = functionName ; // define function name
  reNode->mRight->mLeft = arg1->mRight ;
  if ( reNode->mRight->mLeft == NULL ) {
    Node* tmp = new Node() ;
    reNode->mRight->mLeft = tmp ;
  } // if
  
  reNode->mRight->mRight = new Node() ;
  reNode->mRight->mRight->mLeft = new Node() ;
  reNode->mRight->mRight->mLeft = arg2 ;
  for ( int i = 0 ; i < storeArea.mDefines.size() ; i++ ) {
    if ( storeArea.mDefines.at( i )->mLeft != NULL 
         && !storeArea.mDefines.at( i )->mLeft->mData.compare( functionName ) ) {
      storeArea.mDefines.erase( storeArea.mDefines.begin() + i ) ;
    } // if
  } // for
  
  // PrintErrorS_exp( reNode ) ;
  storeArea.mDefines.push_back( reNode ) ;
  printf( "%s defined\n\n> ",  functionName.c_str() ) ; // wasn't found in second time
  functionName = "#<procedure " + functionName + ">" ;
  return NULL ;
} // Eval_DefineFunc()

Node* Eval_Func( Node* head, StoreArea &storeArea ) {
  Node* reNode ;
  Node* args = head->mRight ;
  Node* funcTree, *copyFunc ;
  vector< Node* > localDefines, contents, perLocal ;
  int funcIndex ;
  // find funcTree
  string pureIns = Purcedure2Normal( head->mLeft->mData ) ;
  
  for ( int i = 0; i < storeArea.mDefines.size() ; i++ ) {
    string pureFunc ;
    if ( storeArea.mDefines.at( i )->mLeft != NULL )
      pureFunc = Purcedure2Normal( storeArea.mDefines.at( i )->mLeft->mData ) ;
    if ( storeArea.mDefines.at( i )->mLeft != NULL 
         && !pureFunc.compare( pureIns )
         && CheckArgumentNum( storeArea.mDefines.at( i ) ) == 3 ) {
      // function 有大於兩個內容要用DS存輪流做 
      
      funcTree = storeArea.mDefines.at( i ) ;
      funcIndex = i ;
      copyFunc = new Node() ;
      CopyTree( funcTree, copyFunc ) ; // copy fist to second
    } // if
  } // for
  
  if ( CheckArgumentNum( args ) == CheckArgumentNum( funcTree->mRight->mLeft ) ) { 
  // arg is correct and each arg is S_exp
  // previous level error is not done
  // check function parameter
    Node* walkArg = args ;
    for ( Node* walkFunc = funcTree->mRight->mLeft; walkFunc != NULL && walkFunc->mLeft != NULL
          ; walkFunc = walkFunc->mRight ) {
      // define local variable
      Node* localDefine = new Node() ;
      string defineName = walkFunc->mLeft->mData ;
      localDefine->mLeft = new Node() ;
      localDefine->mRight = new Node() ;
      localDefine->mData = "(" ;
      localDefine->mLeft->mData = defineName ; // localVar name
      localDefine->mRight->mLeft = new Node() ;
      try {
        localDefine->mRight->mLeft = EvalS_exp( walkArg->mLeft, storeArea ) ; // arg       
      } // try
      catch ( const char* error ) {
        walkArg->mLeft->mData = Purcedure2Normal( walkArg->mLeft->mData ) ;
        if ( !strcmp( error, "ERROR (no return value) : " ) ) {
          error = "ERROR (unbound parameter) : " ;
          printf( "%s", error ) ;
          PrintErrorS_exp( walkArg->mLeft ) ;
          throw "error" ;
        } // if
        
        throw error ;
      } // catch
      
      localDefines.push_back( localDefine ) ;
      
      walkArg = walkArg->mRight ;
    } // for
  
   
    for ( int index = 0 ; index < storeArea.mLocal.size() ; index++ ) {
      perLocal.push_back( storeArea.mLocal.at( index ) ) ;
    } // for
    
    try {
      Node* tmp ;
      CopyTree( copyFunc, storeArea.mDefines.at( funcIndex ) ) ;
      for ( Node* walk = storeArea.mDefines.at( funcIndex )->mRight->mRight->mLeft 
            ; walk != NULL && walk->mLeft != NULL ; walk = walk->mRight ) { // copyFunc will renew a space
        tmp = new Node() ;
        CopyTree( walk->mLeft, tmp ) ;
        // storeArea.mLocal.clear() ;
        // PrintErrorS_exp( tmp ) ;
        storeArea.mLocal.clear() ;
        for ( int i = 0; i < localDefines.size() ; i++ ) {      
          // cout << "define : " << localDefines.at( i )->mLeft->mData << endl ;
          storeArea.mLocal.push_back( localDefines.at( i ) ) ;
        } // for
        
        try {
          reNode = EvalS_exp( tmp, storeArea ) ; // eval function content
        } // try
        catch ( const char* error ) {
          if ( strcmp( error, "ERROR (no return value) : " ) ) {
            CopyTree( copyFunc, storeArea.mDefines.at( funcIndex ) ) ;
            head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ; 
            
            storeArea.mLocal.clear() ;
            for ( int index = 0 ; index < perLocal.size() ; index++ ) {
              storeArea.mLocal.push_back( perLocal.at( index ) ) ;
            } // for
            
            throw error ;
          } // if
          else if ( !strcmp( error, "ERROR (no return value) : " ) )
            reNode = NULL ;
        } // catch
      } // for
      /*
      
      */
    } // try
    catch ( const char* error ) {
      string tmp( error ) ;
      CopyTree( copyFunc, storeArea.mDefines.at( funcIndex ) ) ;
      head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ; 
      
      storeArea.mLocal.clear() ;
      for ( int index = 0 ; index < perLocal.size() ; index++ ) {
        storeArea.mLocal.push_back( perLocal.at( index ) ) ;
      } // for
      
      throw error ;
    } // catch()
    
    if ( reNode == NULL ) {
      CopyTree( copyFunc, storeArea.mDefines.at( funcIndex ) ) ;
      head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
      storeArea.mLocal.clear() ;
      for ( int index = 0 ; index < perLocal.size() ; index++ ) {
        storeArea.mLocal.push_back( perLocal.at( index ) ) ;
      } // for
      
      throw "ERROR (no return value) : " ;
    } // if
  } // if
  else {
    CopyTree( copyFunc, storeArea.mDefines.at( funcIndex ) ) ;
    head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ; 
    
    storeArea.mLocal.clear() ;
    for ( int index = 0 ; index < perLocal.size() ; index++ ) {
      storeArea.mLocal.push_back( perLocal.at( index ) ) ;
    } // for
    
    head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ; 
    printf( "ERROR (incorrect number of arguments) : %s\n", pureIns.c_str() ) ;
    throw "function error\n" ;
    
  } // else
    
  // PrintErrorS_exp( head ) ;  
  // cout << reNode->mData << endl ;
  storeArea.mLocal.clear() ;
  for ( int index = 0 ; index < perLocal.size() ; index++ ) {
    storeArea.mLocal.push_back( perLocal.at( index ) ) ;
  } // for
    
  return reNode ;
} // Eval_Func()

Node* Eval_Cons( Node* head, StoreArea &storeArea ) { // two S_exps connection
  Node* initHead = head->mRight ;
  Node* original = head ;
  head = head->mRight ;
  
  try {
    initHead->mLeft = EvalS_exp( head->mLeft, storeArea ) ;
    if ( IsIns( initHead->mLeft->mData ) ) {
      initHead->mLeft->mData = "#<procedure " + initHead->mLeft->mData + ">" ;
    } // if
  } // try
  catch ( const char* error ) {
    head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ; 
    if ( !strcmp( error, "ERROR (no return value) : " ) ) {
      error = "ERROR (unbound parameter) : " ;
      printf( "%s", error ) ;
      PrintErrorS_exp( head->mLeft ) ;
      throw "error" ;
    } // if
    
    throw error ;
  } // catch
  
  head = head->mRight ;
  try {
    head = EvalS_exp( head->mLeft, storeArea ) ;
    if ( IsIns( head->mData ) ) {
      head->mData = "#<procedure " + head->mData + ">" ;
    } // if
    
    !head->mData.compare( "(" ) ? head->mData = "\0" : head->mData ; 
    // clear right node data to make sure pretty print
  } // try
  catch ( const char* error ) {
    head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
    if ( !strcmp( error, "ERROR (no return value) : " ) ) {
      error = "ERROR (unbound parameter) : " ;
      printf( "%s", error ) ;
      PrintErrorS_exp( head->mLeft ) ;
      throw "error" ;
    } // if
    
    throw error ;
  } // catch
  
  original->mLeft->mData = Purcedure2Normal( original->mLeft->mData ) ;
  initHead->mData = "(" ;
  initHead->mRight = head ;
  return initHead ;
} // Eval_Cons()

Node* Caculate( Node* head, StoreArea &storeArea ) {
  Node* trash = head ;
  Node* returnNode = new Node() ;
  stringstream output ;
  string opera = "\0", evalTmp ;
  bool isFloat = false ;
  double result, num ;
  head->mLeft->mData  = Purcedure2Normal( head->mLeft->mData ) ;
  opera = head->mLeft->mData ;
  try {
    head = head->mRight ;
    evalTmp = EvalS_exp( head->mLeft, storeArea )->mData ;
    if ( WhichType( evalTmp ) == "FLOAT" ) isFloat = true ;
    if ( WhichType( evalTmp ) != "FLOAT" && WhichType( evalTmp ) != "INT" ) {
      printf( "ERROR (%s with incorrect argument type) : ", opera.c_str() ) ;
      PrintErrorS_exp( EvalS_exp( head->mLeft, storeArea ) ) ;
      throw "ERROR ( with incorrect argument type) : " ;
    } // if
  } // try
  catch ( const char* error ) {
    trash->mLeft->mData = Purcedure2Normal( trash->mLeft->mData ) ;
    head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
    string tmp( error ) ;
    if ( !strcmp( error, "ERROR (no return value) : " ) ) {
      error = "ERROR (unbound parameter) : " ;
      cout << error ;
      PrintErrorS_exp( head->mLeft ) ;
    } // if
    
    // trash->mLeft->mData = "#<procedure " +  trash->mLeft->mData + ">" ;
    // head->mLeft->mData = "#<procedure " +  head->mLeft->mData + ">" ;
    throw error ;
  } // catch() 
  
  result =  Stod( evalTmp ) ;
  if ( head->mRight != NULL ) {
    for ( Node* cur = head->mRight ; cur != NULL ; cur = cur->mRight ) {
      try {
    
        evalTmp = EvalS_exp( cur->mLeft, storeArea )->mData ;
        ClearTree( cur->mLeft->mLeft ) ;
        ClearTree( cur->mLeft->mRight ) ;
        if ( WhichType( evalTmp ) == "FLOAT" ) isFloat = true ;
        if ( WhichType( evalTmp ) != "FLOAT" && WhichType( evalTmp ) != "INT" ) {
          printf( "ERROR (%s with incorrect argument type) : ", opera.c_str() ) ;
          PrintErrorS_exp( EvalS_exp( cur->mLeft, storeArea ) ) ;
          throw "ERROR ( with incorrect argument type) : " ;
        } // if
      
      } // try
      catch ( const char* error ) {
        cur->mLeft->mData = Purcedure2Normal( cur->mLeft->mData ) ;
        string tmp( error ) ;
        if ( !strcmp( error, "ERROR (no return value) : " ) ) {
          error = "ERROR (unbound parameter) : " ;
          cout << error ;
          PrintErrorS_exp( cur->mLeft ) ;
        } // if
        
        // cur->mLeft->mData = "#<procedure " +  cur->mLeft->mData + ">" ;
        throw error ;
      } // catch      
      
      num = Stod( evalTmp ) ;
      if ( !opera.compare( "+" ) ) result = result + num ;
      if ( !opera.compare( "-" ) ) result = result - num ;
      if ( !opera.compare( "*" ) ) result = result * num ;
      if ( !opera.compare( "/" ) ) {
        if ( num == 0 ) {
          printf( "ERROR (division by zero) : /\n" ) ;
          throw "ERROR (division by zero) : /" ;
        } // if
        
        if ( !isFloat ) {
          int re, n ;
          re = result ;
          n = num ;
          result = re / n ;
        } // if
        else 
          result = result / num ;
      } // if
    } // for
  } // if
  
  output << result ;
  returnNode->mData = output.str() ;
  if ( isFloat ) {
    bool hasDot = false ;
    for ( int i = 0 ; i < returnNode->mData.length() ; i++ )
      if ( returnNode->mData.at( i ) == '.' ) hasDot = true ;
    if ( !hasDot )
      returnNode->mData = returnNode->mData + "." ;
  } // if
  
  // cout << "test caculate result : " << returnNode->mData << endl ;
  return returnNode ;
} // Caculate()

Node* TestPrimitive_predicates( Node* head, StoreArea &storeArea ) {
  // test if true return head->mData = "#t"
  // if false return "nil"
  bool equal = false ;
  string curIns = "\0" ;
  Node* returnNode = new Node() ;
  Node* original = head ;
  
  curIns = Purcedure2Normal( head->mLeft->mData ) ;
  try {
    if ( !curIns.compare( "atom?" ) ) { // is atom?
      Node* compare = EvalS_exp( head->mRight->mLeft, storeArea ) ;
      if ( WhichType( compare->mData ) == "SYMBOL" || WhichType( compare->mData ) == "INT" 
           || WhichType( compare->mData ) == "FLOAT" || WhichType( compare->mData ) == "STRING" 
           || WhichType( compare->mData ) == "nil" || WhichType( compare->mData ) == "#t" ) {
        returnNode->mData = "#t" ;
      } // if
      else {
        returnNode->mData = "nil" ;
      } // else 
      
      // delete( compare ) ;
    } // if
    else if ( !curIns.compare( "pair?" ) ) {
      Node* compare = EvalS_exp( head->mRight->mLeft, storeArea ) ;
      if ( WhichType( compare->mData ) == "SYMBOL" || WhichType( compare->mData ) == "INT" 
           || WhichType( compare->mData ) == "FLOAT" || WhichType( compare->mData ) == "STRING" 
           || WhichType( compare->mData ) == "nil" || WhichType( compare->mData ) == "#t" ) {
        
        returnNode->mData = "nil" ;
      } // if
      else {
        returnNode->mData = "#t" ;
      } // else  
      // delete( cur ) ;
    } // else if
    else if ( !curIns.compare( "list?" ) ) {
      bool isList = true ;
      head = EvalS_exp( head->mRight->mLeft, storeArea ) ; // return a S_exp
      if ( head->mRight != NULL ) { // Is list ?
        Node* cur = head->mRight ;
        for (  ; cur != NULL ; cur = cur->mRight ) {
          if ( ( cur->mData.compare( "\0" ) && cur->mData.compare( "(" ) ) 
               && WhichType( cur->mData ) != "nil" ) {
            isList = false ;
          } // if
        } // for
        
        // delete( cur ) ;
      } // if
      else if ( head->mData.compare( "\0" ) && head->mData.compare( "(" ) )
        isList = false ;
      if ( isList ) {
        returnNode->mData = "#t" ;
      } // if
      else {
        returnNode->mData = "nil" ;
      } // else
    } // else if
    else if ( !curIns.compare( "null?" ) ) {
      head = EvalS_exp( head->mRight->mLeft, storeArea ) ; // return a S_exp
      if ( WhichType( head->mData ) == "nil" ) {
        returnNode->mData = "#t" ;
      } // if
      else {
        returnNode->mData = "nil" ;
      } // else
    } // else if
    else if ( !curIns.compare( "integer?" ) ) {
      head = EvalS_exp( head->mRight->mLeft, storeArea ) ; // return a S_exp
      if ( WhichType( head->mData ) == "INT" ) {
        returnNode->mData = "#t" ;
      } // if
      else {
        returnNode->mData = "nil" ;
      } // else
    } // else if
    else if ( !curIns.compare( "real?" ) || !curIns.compare( "number?" ) ) {
      head = EvalS_exp( head->mRight->mLeft, storeArea ) ; // return a S_exp
      if ( WhichType( head->mData ) == "FLOAT" || WhichType( head->mData ) == "INT" ) {
        returnNode->mData = "#t" ;
      } // if
      else {
        returnNode->mData = "nil" ;
      } // else
    } // else if
    else if ( !curIns.compare( "string?" ) ) {
      head = EvalS_exp( head->mRight->mLeft, storeArea ) ; // return a S_exp
      if ( WhichType( head->mData ) == "STRING" ) {
        returnNode->mData = "#t" ;
      } // if
      else {
        returnNode->mData = "nil" ;
      } // else
    } // else if
    else if ( !curIns.compare( "boolean?" ) ) {
      head = EvalS_exp( head->mRight->mLeft, storeArea ) ; // return a S_exp
      if ( WhichType( head->mData ) == "nil" || WhichType( head->mData ) == "#t" ) {
        returnNode->mData = "#t" ;
      } // if
      else {
        returnNode->mData = "nil" ;
      } // else
    } // else if
    else if ( !curIns.compare( "symbol?" ) ) {
      head = EvalS_exp( head->mRight->mLeft, storeArea ) ; // return a S_exp
      if ( WhichType( head->mData ) == "SYMBOL" ) {
        returnNode->mData = "#t" ;
      } // if
      else {
        returnNode->mData = "nil" ;
      } // else
    } // else if
    else {
      // cout << "something exception.\n" ;
    } // else
  } // try
  catch ( const char* error ) {
    head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
    string tmp( error ) ;
    if ( !strcmp( error, "ERROR (no return value) : " ) ) {
      error = "ERROR (unbound parameter) : " ;
      cout << error ;
      PrintErrorS_exp( head->mRight->mLeft ) ;
    } // if
    
    throw error ;
  } // catch
  
  original->mLeft->mData = curIns ;
  return returnNode ;
} // TestPrimitive_predicates()

Node* Eval_Compare( Node* head, StoreArea &storeArea ) {
  // need to eval each exp
  Node* reNode = new Node() ; 
  double compared, num ;
  string errorTmp ;
  string opera = "\0" ;
  opera = Purcedure2Normal( head->mLeft->mData ) ;
  head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
  try {
    Node* testNode ;
    reNode->mData = "#t" ;
    head = head->mRight ;
    testNode = EvalS_exp( head->mLeft, storeArea ) ;
    if ( testNode == NULL ) {
      throw "ERROR (no return value) : " ;
    } // if
    
    compared = Stod( testNode->mData ) ;
    errorTmp = testNode->mData ;
    if ( WhichType( errorTmp ) != "FLOAT" && WhichType( errorTmp ) != "INT" ) {
      printf( "ERROR (%s with incorrect argument type) : ", opera.c_str() ) ;
      PrintErrorS_exp( EvalS_exp( head->mLeft, storeArea ) ) ;
      throw "ERROR ( with incorrect argument type) : " ;
    } // if
  } // try
  catch ( const char* error ) {
    
    head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
    if ( !strcmp( error, "ERROR (no return value) : " ) ) {
      error = "ERROR (unbound parameter) : " ;
      printf( "%s", error ) ;
      PrintErrorS_exp( head->mLeft ) ;
      throw "error" ;
    } // if
    
    throw error ;
  } // catch
  
  if ( head->mRight != NULL ) {
    for ( Node* cur = head->mRight ; cur != NULL ; cur = cur->mRight ) {
      try {
        Node* testNode ;
        testNode = EvalS_exp( cur->mLeft, storeArea ) ;
        
        if ( testNode == NULL ) {
          throw "ERROR (no return value) : " ;
        } // if
        
        num = Stod( testNode->mData ) ;
        errorTmp = testNode->mData ;
        // cout << "test : " << errorTmp << WhichType( errorTmp ) << endl ; 
        if ( WhichType( errorTmp ) != "FLOAT" && WhichType( errorTmp ) != "INT" ) {
          printf( "ERROR (%s with incorrect argument type) : ", opera.c_str() ) ;
          PrintErrorS_exp( EvalS_exp( cur->mLeft, storeArea ) ) ;
          throw "ERROR ( with incorrect argument type) : " ;
        } // if
        // ClearTree( cur->mLeft->mLeft ) ;
        // ClearTree( cur->mLeft->mRight ) ;
        if ( !reNode->mData.compare( "#t" ) ) {
          if ( !opera.compare( ">" ) ) {
            if ( compared > num ) {
              compared = num ;
            } // if
            else {
              reNode->mData = "nil" ;
            } // else
          } // if
          
          if ( !opera.compare( ">=" ) ) {
            // cout << compared << "  " << num << endl ;
            if ( compared >= num ) {
              compared = num ;
            } // if
            else {
              reNode->mData = "nil" ;
            } // else
          } // if
          
          if ( !opera.compare( "<" ) ) {
            if ( compared < num ) {
              compared = num ;
            } // if
            else {
              reNode->mData = "nil" ;
            } // else
          } // if
          
          if ( !opera.compare( "<=" ) ) {
            if ( compared <= num ) {
              compared = num ;
            } // if
            else {
              reNode->mData = "nil" ;
            } // else
          } // if
          
          if ( !opera.compare( "=" ) ) {
            if ( compared == num ) ;
            else {
              reNode->mData = "nil" ;
            } // else
          } // if
        } // if
        
      } // try
      catch ( const char* error ) {
        if ( head->mLeft != NULL )
          head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        if ( !strcmp( error, "ERROR (no return value) : " ) ) {
          error = "ERROR (unbound parameter) : " ;
          printf( "%s", error ) ;
          PrintErrorS_exp( cur->mLeft ) ;
          throw "error" ;
        } // if
        
        throw error ;
      } // catch
    } // for
  } // if
  
  return reNode ;
} // Eval_Compare()

Node* Eval_String( Node* head, StoreArea &storeArea ) {
  Node* reNode = new Node() ; 
  string compared, num ;
  string opera = "\0" ;
  opera = Purcedure2Normal( head->mLeft->mData ) ;
  head->mLeft->mData = opera ;
  try {
    head = head->mRight ;
    if ( !opera.compare( "string-append" ) ) {
      reNode->mData = EvalS_exp( head->mLeft, storeArea )->mData ;
      if ( WhichType( reNode->mData ) != "STRING" ) {
        printf( "ERROR (%s with incorrect argument type) : ", opera.c_str() ) ;
        PrintErrorS_exp( EvalS_exp( head->mLeft, storeArea ) ) ;
        throw "ERROR ( with incorrect argument type) : " ;
      } // if
    } // if
    else {
      reNode->mData = "#t" ;
      compared = EvalS_exp( head->mLeft, storeArea )->mData ;
      if ( WhichType( compared ) != "STRING" ) {
        printf( "ERROR (%s with incorrect argument type) : ", opera.c_str() ) ;
        PrintErrorS_exp( EvalS_exp( head->mLeft, storeArea ) ) ;
        throw "ERROR ( with incorrect argument type) : " ;
      } // if
    } // else
  } // try
  catch ( const char* error ) {
    head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
    if ( !strcmp( error, "ERROR (no return value) : " ) ) {
      error = "ERROR (unbound parameter) : " ;
      printf( "%s", error ) ;
      PrintErrorS_exp( head->mLeft ) ;
      throw "error" ;
    } // if
    
    throw error ;
  } // catch
  
  if ( head->mRight != NULL ) {
    for ( Node* cur = head->mRight ; cur != NULL ; cur = cur->mRight ) {
      try {
        num = EvalS_exp( cur->mLeft, storeArea )->mData ;
        if ( WhichType( num ) != "STRING" ) {
          printf( "ERROR (%s with incorrect argument type) : ", opera.c_str() ) ;
          PrintErrorS_exp( EvalS_exp( cur->mLeft, storeArea ) ) ;
          throw "ERROR ( with incorrect argument type) : " ;
        } // if
        // ClearTree( cur->mLeft->mLeft ) ;
        // ClearTree( cur->mLeft->mRight ) ;
        if ( !opera.compare( "string-append" ) ) {
          reNode->mData = 
            reNode->mData.substr( 0, reNode->mData.length()-1 ) + num.substr( 1, num.length()-1 ) ;
        } // if
        
        if ( !reNode->mData.compare( "#t" ) ) {
          if ( !opera.compare( "string>?" ) ) {
            if ( compared > num ) {
              compared = num ;
            } // if
            else {
              reNode->mData = "nil" ;
            } // else
          } // if
          
          if ( !opera.compare( "string<?" ) ) {
            if ( compared < num ) {
              compared = num ;
            } // if
            else {
              reNode->mData = "nil" ;
            } // else
          } // if
          
          if ( !opera.compare( "string=?" ) ) {
            if ( compared == num ) {
              compared = num ;
            } // if
            else {
              reNode->mData = "nil" ;
            } // else
          } // if
        } // if
        
      } // try
      catch ( const char* error ) {
        head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
        if ( !strcmp( error, "ERROR (no return value) : " ) ) {
          error = "ERROR (unbound parameter) : " ;
          printf( "%s", error ) ;
          PrintErrorS_exp( cur->mLeft ) ;
          throw "error" ;
        } // if
        
        throw error ;
      } // catch
    } // for
  } // if
  
  
  head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
  return reNode ;
} // Eval_String()
 
Node* Eval_eqv( Node* head, StoreArea &storeArea ) {
  Node* arg1, *arg2 ;
  Node* reNode = new Node() ;
  
  reNode->mData = "nil" ;
  try {
    head = head->mRight ;
    arg1 = EvalS_exp( head->mLeft, storeArea ) ;
  } // try
  catch ( const char* error ) {
    head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
    if ( !strcmp( error, "ERROR (no return value) : " ) ) {
      error = "ERROR (unbound parameter) : " ;
      printf( "%s", error ) ;
      PrintErrorS_exp( head->mLeft ) ;
      throw "error" ;
    } // if
    
    throw error ;
  } // catch
  try {
    head = head->mRight ;
    arg2 = EvalS_exp( head->mLeft, storeArea ) ;
  } // try
  catch ( const char* error ) {
    head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
    if ( !strcmp( error, "ERROR (no return value) : " ) ) {
      error = "ERROR (unbound parameter) : " ;
      printf( "%s", error ) ;
      PrintErrorS_exp( head->mLeft ) ;
      throw "error" ;
    } // if
    
    throw error ;
  } // catch
  
  if ( arg1 == arg2 ) {
    reNode->mData = "#t" ;
    return reNode ;
  } // if
  
  if ( arg1->mData != "\0" && arg1->mData != "(" && arg2->mData != "\0" && arg2->mData != "(" ) { 
  // both arg are ATOM and eqv
    if ( WhichType( arg1->mData ) == "INT" || WhichType( arg1->mData ) == "FLOAT" 
         || WhichType( arg1->mData ) == "#t" || WhichType( arg1->mData ) == "nil" ) {
      if ( WhichType( arg2->mData ) == "INT" || WhichType( arg2->mData ) == "FLOAT" 
           || WhichType( arg2->mData ) == "#t" || WhichType( arg2->mData ) == "nil" ) {
        if ( !strcmp( arg1->mData.c_str(), arg2->mData.c_str() ) ) {
          reNode->mData = "#t" ;
          return reNode ;
        } // if      
      } // if  
    } // if
  } // if
  
  head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
  return reNode ;
} // Eval_eqv()

Node* Eval_equal( Node* head, StoreArea &storeArea ) {
  Node* arg1, *arg2 ;
  Node* reNode = new Node() ;
  int stage = 0 ;
  bool t = false ;
  reNode->mData = "nil" ;
  head = head->mRight ;
  try {
    arg1 = EvalS_exp( head->mLeft, storeArea ) ;
    // PrintS_exp( arg1, stage, t ) ;
  } // try
  catch ( const char* error ) {
    head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
    if ( !strcmp( error, "ERROR (no return value) : " ) ) {
      error = "ERROR (unbound parameter) : " ;
      printf( "%s", error ) ;
      PrintErrorS_exp( head->mLeft ) ;
      throw "error" ;
    } // if
    
    throw error ;
  } // catch
  
  try {
    head = head->mRight ;
    arg2 = EvalS_exp( head->mLeft, storeArea ) ;
  } // try
  catch ( const char* error ) {
    head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
    if ( !strcmp( error, "ERROR (no return value) : " ) ) {
      error = "ERROR (unbound parameter) : " ;
      printf( "%s", error ) ;
      PrintErrorS_exp( head->mLeft ) ;
      throw "error" ;
    } // if
    
    throw error ;
  } // catch
  // PrintS_exp( arg2, stage, t ) ;
  if ( Euqal_traversal( arg1, arg2 ) ) {
    reNode->mData = "#t" ;
  } // if
  
  head->mLeft->mData = Purcedure2Normal( head->mLeft->mData ) ;
  return reNode ;
} // Eval_equal()

bool Euqal_traversal( Node* node1, Node* node2 ) { 
  if ( node1 == NULL && node2 == NULL ) return true ;
  else if ( node1 == NULL && WhichType( node2->mData ) == "nil" ) return true ;
  else if ( node2 == NULL && WhichType( node1->mData ) == "nil" ) return true ;
  else if ( node1 == NULL || node2 == NULL ) {
    return false ;
  } // else if
  else if ( ( !strcmp( node1->mData.c_str(), "\0" ) && !strcmp( node2->mData.c_str(), "(" ) )
            || ( !strcmp( node2->mData.c_str(), "\0" ) && !strcmp( node1->mData.c_str(), "(" ) ) ) {
    if ( Euqal_traversal( node1->mLeft, node2->mLeft ) ) {
      if ( Euqal_traversal( node1->mRight, node2->mRight ) ) {
        return true ;
      } // if
    } // if
  } // else if
  else if ( !strcmp( node1->mData.c_str(), node2->mData.c_str() ) ) {
    if ( Euqal_traversal( node1->mLeft, node2->mLeft ) ) {
      if ( Euqal_traversal( node1->mRight, node2->mRight ) ) {
        return true ;
      } // if
    } // if
  } // else if
  else {
    return false ;
  } // else
  
  return false ;
} // Euqal_traversal() 

int CheckArgumentNum( Node* head ) { // check argument num
  int argumentNum = 0 ;
  if ( head == NULL ) return 0 ;
  else if ( head->mLeft != NULL ) argumentNum++ ;
  return ( argumentNum + CheckArgumentNum( head->mRight ) ) ;
} // CheckArgumentNum()

string ThisNodeIsIns( Node* head, StoreArea &storeArea ) {
  if ( head == NULL ) return "nil" ;
  else if ( head->mLeft == NULL && head->mRight == NULL ) { // leaf.
    if ( WhichType( head->mData ) == "INT" || WhichType( head->mData ) == "FLOAT" 
         || WhichType( head->mData ) == "STRING" || WhichType( head->mData ) == "nil"
         || WhichType( head->mData ) == "#t"  ) {
      // cout << "leaf atom : " << head->mData << endl ;
      return "ATOM" ; // Is ATOM but not SYMBOL
    } // if
    else if ( WhichType( head->mData ) == "SYMBOL" 
              || IsIns( Purcedure2Normal( head->mData ) ) ) { // Is symbol not Instruction
      bool findDefined = false ;
      string pureFunc ;
      string pureIns = Purcedure2Normal( head->mData ) ;
      
      for ( int i = storeArea.mLocal.size()-1 ; i >= 0 ; i-- ) {
        // cout << "test : " << head->mData << storeArea.mDefines.at( i )->mData << endl ;
        if ( storeArea.mLocal.at( i )->mLeft != NULL )
          pureFunc = Purcedure2Normal( storeArea.mLocal.at( i )->mLeft->mData ) ;
        if ( IsIns( pureIns ) ) {
          string compared = Purcedure2Normal( storeArea.mLocal.at( i )->mData ) ;
          if ( !pureIns.compare( "'" ) ) pureIns = "quote" ;
          if ( !compared.compare( pureIns.c_str() ) ) {
            // cout << head->mData << endl ;
            return "BoundSymbol" ;
          } // if
        } // if
        else if ( storeArea.mLocal.at( i )->mLeft != NULL 
                  && !pureFunc.compare( pureIns ) ) {
          findDefined = true ;
          if ( CheckArgumentNum( storeArea.mLocal.at( i ) ) == 3 )
            return "Function" ;
          else
            return "BoundSymbol" ;
        } // else if
      } // for
      
      for ( int i = 0 ; i < storeArea.mDefines.size() ; i++ ) {
        // cout << "test : " << head->mData << storeArea.mDefines.at( i )->mData << endl ;
        if ( storeArea.mDefines.at( i )->mLeft != NULL )
          pureFunc = Purcedure2Normal( storeArea.mDefines.at( i )->mLeft->mData ) ;
        if ( IsIns( pureIns ) ) {
          string compared = Purcedure2Normal( storeArea.mDefines.at( i )->mData ) ;
          if ( !pureIns.compare( "'" ) ) pureIns = "quote" ;
          if ( !compared.compare( pureIns.c_str() ) ) {
            // cout << head->mData << endl ;
            return "BoundSymbol" ;
          } // if
        } // if
        else if ( storeArea.mDefines.at( i )->mLeft != NULL 
                  && !pureFunc.compare( pureIns ) ) {
          findDefined = true ;
          if ( CheckArgumentNum( storeArea.mDefines.at( i ) ) == 3 )
            return "Function" ;
          else
            return "BoundSymbol" ;
        } // else if
      } // for
      
      if ( !findDefined ) { // Didn't find defined symbol
        return "UnboundSymbol" ;
      } // if
      
    } // else if
    else {
    } // else
  } // if
  else if ( ( !head->mData.compare( "\0" ) || !head->mData.compare( "(" ) ) ) {
    return ThisNodeIsIns( head->mLeft, storeArea ) ;
  } // else if
  
  return "error" ;
} // ThisNodeIsIns()

bool InputString( string &token ) {
  char cur ;
  bool mustEscap = false ;
  string errorTmp = "ERROR (no closing quote) : END-OF-LINE encountered at Line " ;
  while ( cin.peek() != '\n' && mustEscap ? true : cin.peek() != '"' && cin.peek() != EOF ) {
    char tmp[30] ;
    mustEscap = false ;
    if ( cin.peek() == '\n' || cin.peek() == EOF ) {
      gCol++ ;
      // Because error while clear until '\n', here didn't skip until '\n'
      errorTmp = errorTmp + Itoa( gLn > 0 ? gLn : gLn ) + " Column " + Itoa( gCol ) + "\n" ;
      const char* ERRORMESSAGE = errorTmp.c_str() ;
      throw ERRORMESSAGE ; // error
      return false ;
    } // if
    
    if ( cin >> noskipws >> cur ) gCol++ ;
    else throw "ERROR (no more input) : END-OF-FILE encountered" ;
    
    if ( cur == '\\' ) {
      token = token + cur ;
      if ( cin >> noskipws >> cur ) gCol++ ;
      else throw "ERROR (no more input) : END-OF-FILE encountered" ;
    
    } // if
    
    token = token + cur ;
  } // while
  
  if ( cin.peek() == '"' ) {
    if ( cin >> cur ) gCol++ ;
    else throw "ERROR (no more input) : END-OF-FILE encountered" ;
    token = token + cur ;
  } // if
  else if ( cin.peek() == EOF ) {
    gCol++ ;
    errorTmp = errorTmp + Itoa( gLn > 0 ? gLn : gLn ) + " Column " + Itoa( gCol ) + "\n" ;
    const char* ERRORMESSAGE = errorTmp.c_str() ;
    throw ERRORMESSAGE ; // error
  } // else if
  
  return true ;
} // InputString()

string WhichType( string token ) {
  if ( token == " " || token == "\t" || token == "\n" || token == "\r" || token == "\0" )
    return "WhiteSpace" ;
  else if ( !token.compare( "(" ) )
    return "LEFT-PAREN" ;
  else if ( !token.compare( ")" ) )
    return "RIGHT-PAREN" ;
  else if ( !token.compare( "." ) )
    return "DOT" ;
  else if ( !token.compare( "'" ) )
    return "QUOTE" ;
  else if ( token[0] == '"' && token.at( token.size()-1 ) == '"' ) {
    return "STRING" ;
  } // else if
  else if ( !token.compare( "nil" ) || !token.compare( "#f" ) || !token.compare( "()" ) )
    return "nil" ;
  else if ( !token.compare( "t" ) || !token.compare( "#t" ) )
    return "#t" ;
  else if ( IsInt( token ) )
    return "INT" ;
  else if ( IsFloat( token ) )
    return "FLOAT" ;
  else
    return "SYMBOL" ;
} // WhichType()

bool ShouldExit( Node* head ) {
  if ( ( head->mLeft != NULL && head->mRight != NULL
         && !head->mLeft->mData.compare( "exit" )
         && WhichType( head->mRight->mData ) == "nil" )
       || ( head->mLeft != NULL && !head->mLeft->mData.compare( "exit" )
            && head->mRight == NULL ) ) {
    return true ;
  } // if
  
  return false ;
} // ShouldExit()

bool IsWhiteSpace( char inputChar ) {
  if ( inputChar == 0 || inputChar == ' ' || inputChar == '\t' 
       || inputChar == '\n' || inputChar == '\r' || inputChar == '\0' )
    return true ;
    
  return false ;
} // IsWhiteSpace()

bool IsSeparator( char inputChar ) {
  if ( inputChar == '(' || inputChar == ')' || inputChar == '\'' 
       || inputChar == '"' || inputChar == ';' )
    return true ;
    
  return false ;
} // IsSeparator()

bool IsSeparator( string inputString ) {
  if ( !inputString.compare( "(" ) || !inputString.compare( ")" ) || !inputString.compare( "'" ) 
       || !inputString.compare( "\"" ) || !inputString.compare( ";" ) )
    return true ;
    
  return false ;
} // IsSeparator()

bool IsNum( char test ) {
  int testNum = test ;
  if ( testNum > 47 && test < 58 ) return true ;
  else return false ;
} // IsNum()

bool IsInt( string token ) {
  if ( token[0] == '+' || token[0] == '-' || IsNum( token[0] ) )
    for ( int i = 1 ; i < token.size() ; i++ ) {
      if ( !IsNum( token[i] ) ) return false ;
    } // for
    
  else
    return false ;
    
  if ( token.length() == 1 && ( token[0] == '+' || token[0] == '-' ) ) return false ;
  return true ;
} // IsInt()

bool IsFloat( string token ) {
  bool findDot = false, hasNum = false ;
  if ( token[0] == '+' || token[0] == '-' || token[0] == '.' || IsNum( token[0] ) ) {
    if ( token[0] == '.' ) findDot = true ; // start with dot
    if ( IsNum( token[0] ) ) hasNum = true ;
    for ( int i = 1 ; i < token.size() ; i++ ) {
      if ( IsNum( token[i] ) || ( ( token[i] == '.' ) && !findDot ) ) {
        if ( token[i] == '.' ) findDot = true ;
        if ( IsNum( token[i] ) ) hasNum = true ;
      } // if
      else return false ; 
    } // for
    
    if ( !hasNum ) return false ;
  } // if
  else return false ; 
  
  return true ;
} // IsFloat()

bool IsIns( string data ) {
  if ( !data.compare( "define" ) || !data.compare( "cons" )
       || !data.compare( "list" ) || !data.compare( "quote" )
       || !data.compare( "'" ) || !data.compare( "car" )
       || !data.compare( "cdr" ) || !data.compare( "atom?" )
       || !data.compare( "pair?" ) || !data.compare( "list?" )
       || !data.compare( "null?" ) || !data.compare( "integer?" )
       || !data.compare( "real?" ) || !data.compare( "number?" ) 
       || !data.compare( "string?" ) || !data.compare( "boolean?" )
       || !data.compare( "symbol?" ) || !data.compare( "+" )
       || !data.compare( "-" ) || !data.compare( "*" ) 
       || !data.compare( "/" ) || !data.compare( "not" )
       || !data.compare( "and" ) || !data.compare( "or" )
       || !data.compare( ">" ) || !data.compare( ">=" )
       || !data.compare( "<" ) || !data.compare( "<=" )
       || !data.compare( "=" ) || !data.compare( "string-append" )
       || !data.compare( "string>?" ) || !data.compare( "string<?" )
       || !data.compare( "string=?" ) || !data.compare( "eqv?" )
       || !data.compare( "equal?" ) || !data.compare( "begin" )
       || !data.compare( "if" ) || !data.compare( "cond" )
       || !data.compare( "clean-environment" ) || !data.compare( "exit" )
       || !data.compare( "let" ) || !data.compare( "lambda" ) ) { // not end !!
    return true ;
  } // if
  else
    return false ;
} // IsIns()

void InitialStoreArea( StoreArea &storeArea ) {
  storeArea.mDefines.clear() ;
  storeArea.mLocal.clear() ;
  Node* tmp = new Node() ;
  tmp->mData = "#<procedure define>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure cons>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure list>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure quote>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure car>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure cdr>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure atom?>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure pair?>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure list?>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure null?>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure integer?>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure real?>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure number?>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure string?>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure string?>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure boolean?>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure symbol?>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure +>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure ->" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure *>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure />" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure not>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure and>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure or>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure >>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure >=>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure <>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure <=>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure =>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure string-append>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure string>?>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure string<?>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure string=?>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure eqv?>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure equal?>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure begin>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure if>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure cond>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure clean-environment>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure exit>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure let>" ;
  storeArea.mDefines.push_back( tmp ) ;
  tmp = new Node() ;
  tmp->mData = "#<procedure lambda>" ;
  storeArea.mDefines.push_back( tmp ) ;
  
  return ;
} // InitialStoreArea()

void PrintS_exp( Node* head, int &stage, bool &newS_exp ) {
  int count ;
  if ( head == NULL ) return ;
  else if ( head != NULL && head->mLeft != NULL 
            && !head->mLeft->mData.compare( "\0" ) ) {
    count = stage ;
    if ( head->mData.compare( "(" ) ) while ( count-- ) printf( "  " ) ;
    // stage++ ;
    // printf( "( " ) ;
    head->mLeft->mData = "(" ;
    newS_exp = true ;
  } // else if
  else if ( ( !head->mData.compare( "\0" ) && !newS_exp ) ) {
    count = stage ;
    while ( count-- ) printf( "  " ) ;
  } // else if
  
  if ( head->mData.compare( "\0" ) ) {
    PrintNonComment( head->mData ) ;
    newS_exp = false ;
    if ( !head->mData.compare( "(" ) ) {
      stage++ ;
      printf( " " ) ;
      newS_exp = true ;
    } // if
    else 
      printf( "\n" ) ;
  } // if
  
  PrintS_exp( head->mLeft, stage, newS_exp ) ;
  if ( head->mRight != NULL && head->mRight->mLeft == NULL 
       && head->mRight->mRight == NULL ) {
    if ( WhichType( head->mRight->mData ) == "nil" ) {
      stage-- ;
      count = stage ;
      while ( count-- ) printf( "  " ) ;
      printf( ")\n" ) ;
      return ;
    }  // if
    
    count = stage ;
    while ( count-- ) printf( "  " ) ;
    printf( ".\n" ) ;
    count = stage ;
    while ( count-- ) printf( "  " ) ;
    
    if ( head->mRight->mData.compare( "\0" ) ) {
      PrintNonComment( head->mRight->mData ) ;
      printf( "\n" ) ;
      stage-- ;
      count = stage ;
      while ( count-- ) printf( "  " ) ;
      printf( ")\n" ) ;
      return ;
    } // if
  } // if
  else if ( head->mLeft != NULL && head->mRight == NULL && stage > 0 ) {
    stage-- ;
    count = stage ;
    while ( count-- ) printf( "  " ) ;
    printf( ")\n" ) ;
    return ;
  } // else if
  
  if ( head->mRight != NULL && !head->mRight->mData.compare( "(" ) ) head->mRight->mData = "\0" ;
  PrintS_exp( head->mRight, stage, newS_exp ) ;
  
  return ;
} // PrintS_exp()

void PrintForEscape( string token ) {
  // cout << "test sting :" << token << "<--" << endl ;
  for ( int i = 0 ; i < token.size() ; i++ ) {
    if ( token.size() >= 2 && token.at( i ) == '\\' ) { // find '\'
      if ( token.at( i+1 ) == 'n' ) {
        printf( "\n" ) ;
        i++ ;
      } // if
      else if ( token.at( i+1 ) == '"' ) {
        printf( "\"" ) ;
        i++ ;
      } // if
      else if ( token.at( i+1 ) == 't' ) {
        printf( "\t" ) ;
        i++ ;
      } // if
      else if ( token.at( i+1 ) == '\\' ) {
        printf( "\\" ) ;
        i++ ;
      } // if
      else printf( "%c", token.at( i ) ) ;
    } // if
    else if ( token.size() >= 3 && token.at( i ) == '\\'  ) {
      if ( token.at( i+1 ) == '\\' && token.at( i+2 ) == 'n' ) {
        printf( "\\n" ) ;
        i += 2 ;
      } // if
      else if ( token.at( i+1 ) == '\\' && token.at( i+2 ) == 't' ) {
        printf( "\\t" ) ;
        i += 2 ;
      } // else if
      else printf( "%c", token.at( i ) ) ;
    } // else if
    else {
      printf( "%c", token.at( i ) ) ;
    } // else
  } // for
  
  return ;
} // PrintForEscape()

void PrintNonComment( string token ) {
  // printf( " --%s--", WhichType( token ).c_str() ) ;
  // if ( IsIns( token ) ) printf( "#<procedure %s>", token.c_str() ) ;
  if ( !WhichType( token ).compare( "nil" ) ) printf( "nil" ) ;
  else if ( !WhichType( token ).compare( "#t" ) ) printf( "#t" ) ;
  else if ( !WhichType( token ).compare( "INT" ) ) { // take off '+' before int
    int output = atoi( token.c_str() ) ;
    printf( "%d", output ) ;
  } // else if 
  else if ( !WhichType( token ).compare( "QUOTE" ) ) printf( "quote" ) ;
  else if ( !WhichType( token ).compare( "FLOAT" ) ) {
    double output = Stod( token.c_str() ) ;
    printf( "%.3f", output ) ;
  } // else if
  else if ( token[0] == '"' 
            && token[ token.length()-1 ] == '"' )
    PrintForEscape( token ) ; // only String need escape !!
  else 
    printf( "%s", token.c_str() ) ;
  return ;
} // PrintNonComment()

bool StringProcessing( vector<string> &atom ) {    // let atom of string into one token
  for ( int i = 1 ; i < atom.size() ; i++ ) {
    atom.at( 0 ) = atom.at( 0 ) + atom.at( i ) ;
  } // for
  
  atom.resize( 1 ) ;
  return true ;
} // StringProcessing()

int InsPurcedure( Node* head ) {
  if ( head == NULL ) return 1 ;
  else if ( !head->mData.compare( "quote" ) || !head->mData.compare( "'" ) ) {
    return 0 ;
  } // else if
  else {
    if ( InsPurcedure( head->mLeft ) ) {
      if ( InsPurcedure( head->mRight ) ) {
        if ( IsIns( head->mData ) ) {
          head->mData = Purcedure2Normal( head->mData ) ;
          return 1 ;
        } // if
        else return 0 ;
      } // if
      else return 0 ;
    } // if
    else return 0 ;
  } // else
  
  return 1 ;
} // InsPurcedure()
 
string Itoa( int num ) {
  string re ;
  stringstream tmp ;
  tmp << num ;
  re = tmp.str() ;
  return re ;
} // Itoa()

double Stod( string in ) {
  stringstream tmp ;
  double num ;
  tmp << in ;
  tmp >> num ;
  return num ;
} // Stod()

void ClearTree( Node* head ) {
  if ( head == NULL ) return ;
  else {
    // ClearTree( head->mLeft ) ;
    // ClearTree( head->mRight ) ;
    // delete( head ) ;
    return ;
  } // else
} // ClearTree()

void CopyTree( Node* oldTree, Node* newTree ) {
  if ( oldTree == NULL )
    return ;
  else {
    newTree->mData = oldTree->mData ;
    if ( oldTree->mLeft != NULL )
      newTree->mLeft = new Node() ;
    if ( oldTree->mRight != NULL )
      newTree->mRight = new Node() ;

    CopyTree( oldTree->mLeft, newTree->mLeft ) ;
    CopyTree( oldTree->mRight, newTree->mRight ) ;
    return ;
  } // else
  
  return ;
} // CopyTree()

string Purcedure2Normal( string ins ) {
  string reIns = "\0" ;
  if ( !ins.compare( "clean-environment" ) ) reIns = "clean-environment" ;
  else if ( ins.length() >= 14 && ins.at( 0 ) == '#' ) {
    for ( int i = 12 ; i < ins.length() - 1 ; i++ ) {
      reIns = reIns + ins.at( i ) ;
    } // for
    
  } // if
  else {
    reIns = ins ;
  } // else
  
  return reIns ;
} // Purcedure2Normal()

void PrintErrorS_exp( Node* head ) {
  int stage = 0 ;
  bool newS_exp = false ;
  if ( head != NULL && !head->mData.compare( "\0" ) )
    head->mData = "(" ;
  PrintS_exp( head, stage, newS_exp ) ;
  return ;
} // PrintErrorS_exp()
