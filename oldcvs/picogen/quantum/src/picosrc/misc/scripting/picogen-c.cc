/***************************************************************************
 *            PicoPico.cc
 *
 *  May 03 12:20:00 2008
 *  Copyright  2008  Sebastian Mach
 *  seb@greenhybrid.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

// -lboost_regex

/// \todo URGENT check each parsing function (and each other) for correct and complete deletes

/*
// left for fun: i just tried to statically initialize a std::map ... but it failed to compile
static const std::pair<std::string,int> binopPrecPairs[] = {
     std::pair<std::string,int>("*",20)
    ,std::pair<std::string,int>("/",20)
    ,std::pair<std::string,int>("+",40)
    ,std::pair<std::string,int>("-",40)
};
static const int numBinOps = sizeof(binopPrecPairs)/sizeof(binopPrecPairs[0]);
static std::map<std::string,int> binOpPrec(
    std::list<std::pair<std::string,int>(binopPrecPairs,binopPrecPairs+numBinOps).begin(),
    std::list<std::pair<std::string,int>(binopPrecPairs,binopPrecPairs+numBinOps).end()
);
*/

#include <vector>
#include <string>
#include <iostream>
#include <boost/regex.hpp>

#include <picogen/picogen.h>
#include <picogen/misc/scripting/picogen-c.h>
#include <picogen/misc/scripting/AST.h>
#include <picogen/misc/scripting/AST2LLVM.h>

using namespace std;
using namespace boost;






typedef enum {
    int_token,
    float_token,
    id_token,
    keyword_token,
    typename_token,
    other_token,
    omitted_token
} TokenType;



struct TokenDescriptor {
    const TokenType tokenType;
    const std::string name;
    const boost::regex regex;
    const bool omit;
    TokenDescriptor (TokenType tokenType, std::string name, boost::regex regex, bool omit = false)
            : tokenType (tokenType), name (name), regex (regex), omit (omit) {
    }
};



// \d is any digit (as with [[:digit:]])
static const TokenDescriptor tokenDescriptors[] = {
    TokenDescriptor (float_token,    "float",      regex ("-?\\d+\\.\\d+"))
    ,TokenDescriptor (int_token,      "int",        regex ("-?\\d+"))
    ,TokenDescriptor (keyword_token,  "keyword",    regex ("if|else|do|while|for"))
    ,TokenDescriptor (typename_token, "typename",   regex ("int|float"))
    ,TokenDescriptor (id_token,       "identifier", regex ("([[:alpha:]]|_)([[:alpha:]]|[[:digit:]]|_)*"))
    ,TokenDescriptor (other_token,    "other",      regex ("\\+|-|\\*|/|<|>|\\(|\\)|=|;|\\{|\\}"))
    ,TokenDescriptor (omitted_token,  "",           regex ("[[:space:]]+"), true)
};
static const int tokenDescriptorCount = sizeof (tokenDescriptors) / sizeof (tokenDescriptors[0]);



struct Token {
    const TokenDescriptor * tokenDescriptor;
    std::string value;
    /// \todo remember start/end of token (for good dumping of error messages)
    Token (const TokenDescriptor *tokenDescriptor, std::string value)
            : tokenDescriptor (tokenDescriptor), value (value) {
    }
};


//==-------------------------------------------------------------==//
// outline of PicoPico grammar
//==-------------------------------------------------------------==//
//
// *   >> zero or more
// +   >> one or more
// ?   >> zero or one
// |   >> or
// ::= >> is defined by
// 'x' >>
//
// atom        ::= float | integer | id
// parameters  ::= expression? ( expression ',' )*
// call        ::= id '(' parameters ')'
// expression  ::=   call
//                 | atom
//                 | '(' expression ')'
//                 | expression '*' expression
//                 | expression '/' expression
//                 | expression '+' expression
//                 | expression '-' expression
// statement   ::= expression ';'
//
//==-------------------------------------------------------------==//




//==-------------------------------------------------------------==//
// parsing
//==-------------------------------------------------------------==//

static int getTokenPrecedence (const std::vector<Token>::const_iterator &curr, const std::vector<Token>::const_iterator &end) {
    /*
    static std::map<int, char> binopPrecedence; // will be initialized once in PicoPico
    //--------------------------------------------------------
    static bool binopPrecedenceInitialized = false;
    if( !binopPrecedenceInitialized ){
        binopPrecedenceInitialized = true;
        binopPrecedence['+'] = 20;
        binopPrecedence['-'] = 20;
        binopPrecedence['*'] = 40;
        binopPrecedence['/'] = 40;
    }
    //--------------------------------------------------------
    if( binopPrecedence.end() == binopPrecedence.find(ch) )
        return -1;
    return binopPrecedence[ch];
    */
    if (curr == end)
        return -1;
    if (curr->value == "<" || curr->value == ">")
        return 10;
    if (curr->value == "+" || curr->value == "-")
        return 20;
    if (curr->value == "*" || curr->value == "/")
        return 40;
    return -1;
}



static bool tokenEquals (string checkee, const std::vector<Token>::const_iterator &curr, const std::vector<Token>::const_iterator &end) {
    // hehe, "checkee"...like it :)
    if (curr == end)
        return false;
    return curr->value == checkee;
}



static ExprAST *parsePrimary (std::vector<Token>::const_iterator &curr, const std::vector<Token>::const_iterator &end);
static ExprAST *parseStatement (std::vector<Token>::const_iterator &curr, const std::vector<Token>::const_iterator &end);
static BlockAST *parseBlock (std::vector<Token>::const_iterator &curr, const std::vector<Token>::const_iterator &end);




static ExprAST *parseIntExpr (
    std::vector<Token>::const_iterator &curr,
    const std::vector<Token>::const_iterator &end
) {
    // convert token value-string to integer
    int result = 0;
    /// \todo write overflow save str-to-float converter
    sscanf (curr->value.c_str(), "%i", &result);
    ++curr;
    return new IntExprAST (result);
    /*
    int result = 0;
    int mag = 1;

    std::string::const_iterator d = curr->value.end();
    if (curr->value.size() > 0) do {
            --d;
            switch (*d) {
                case '0':
                    result += 0*mag;
                    break;
                case '1':
                    result += 1*mag;
                    break;
                case '2':
                    result += 2*mag;
                    break;
                case '3':
                    result += 3*mag;
                    break;
                case '4':
                    result += 4*mag;
                    break;
                case '5':
                    result += 5*mag;
                    break;
                case '6':
                    result += 6*mag;
                    break;
                case '7':
                    result += 7*mag;
                    break;
                case '8':
                    result += 8*mag;
                    break;
                case '9':
                    result += 9*mag;
                    break;
            };
            mag *= 10;
        } while (d != curr->value.begin());

    ++curr;
    return new IntExprAST (result);*/
}



static ExprAST *parseFloatExpr (
    std::vector<Token>::const_iterator &curr,
    const std::vector<Token>::const_iterator &end
) {
    // convert token value-string to float
    float result = 0;
    /// \todo write overflow save str-to-float converter
    sscanf (curr->value.c_str(), "%f", &result);
    ++curr;
    return new FloatExprAST (result);
}



static IdExprAST *parseIdExpr (
    std::vector<Token>::const_iterator &curr,
    const std::vector<Token>::const_iterator &end
) {
    return new IdExprAST ( (curr++)->value);
}




static IdExprAST *parseIdentifier (
    std::vector<Token>::const_iterator &curr,
    const std::vector<Token>::const_iterator &end
) {
    if (curr != end) {
        switch (curr->tokenDescriptor->tokenType) {
            // identifier-Token.
            case id_token:
                return parseIdExpr (curr, end);
            default:
                ;
        }
    }
    return 0;
}



static ExprAST *parseBinOpRhs (
    // standard parameters
    std::vector<Token>::const_iterator &curr,
    const std::vector<Token>::const_iterator &end,
    // additional parameters
    int exprPrec,
    ExprAST *lhs
    // --
) {
    if (curr == end)
        return lhs;

    while (1) {
        const int tokPrec = getTokenPrecedence (curr,end);

        if (tokPrec < exprPrec) {
            return lhs;
        }

        const char binOp = curr->value[0];
        ++curr;

        ExprAST *rhs = parsePrimary (curr, end);
        if (0 == rhs)
            return 0;

        // look ahead next token precedence
        const int nextTokPrec = getTokenPrecedence (curr,end);
        if (tokPrec < nextTokPrec) {
            rhs = parseBinOpRhs (curr, end, tokPrec+1, rhs);
            if (0 == rhs)
                return 0;
        }

        // construct binary expression ast
        lhs = new BinaryExprAST (binOp, lhs, rhs);
    }

    return lhs;
}



static ExprAST *parseExpr (
    std::vector<Token>::const_iterator &curr,
    const std::vector<Token>::const_iterator &end
) {
    // Prepare possible rollback
    const std::vector<Token>::const_iterator backup = curr;

    // Assignment-statement ?
    IdExprAST *tmpId = /*dynamic_cast<IdExprAST *> (lhs); //*/parseIdentifier (curr, end);
    if (0 != tmpId) {
        if (tokenEquals ("=", curr, end)) {
            ++curr;
            const ExprAST *rhs = parseExpr (curr, end);
            return new AssignmentExprAST (tmpId, rhs);
        }
        delete tmpId;
    }
    // Rollback.
    curr = backup;


    ExprAST *lhs = parsePrimary (curr, end);
    if (lhs == 0)
        return 0;


    return parseBinOpRhs (curr, end, 0, lhs);
}



static ExprAST *parseParenExpr (
    std::vector<Token>::const_iterator &curr,
    const std::vector<Token>::const_iterator &end
) {
    if (!tokenEquals ("(", curr, end))    // assert "(" to be the start token of this parenthesized expression
        return 0;
    ++curr; // eat '('
    ExprAST *ast = parseExpr (curr, end);
    if (curr != end && ast != 0) {
        if (curr->value != ")")
            return 0;
        ++curr;
        return ast;
    }
    return 0;
}



static ExprAST *parsePrimary (
    std::vector<Token>::const_iterator &curr,
    const std::vector<Token>::const_iterator &end
) {
    if (curr != end) {
        switch (curr->tokenDescriptor->tokenType) {
            // int-Token.
            case int_token:
                return parseIntExpr (curr, end);
            // float-Token.
            case float_token:
                return parseFloatExpr (curr, end);
            // identifier-Token.
            case id_token:
                return parseIdExpr (curr, end);
            // other Token.
            case other_token:
                if (curr->value == "(")
                    return parseParenExpr (curr, end);
                break;
            default:
                ;
        }
    }
    return 0;
}



/*
static ExprAST *parseTypename (
    std::vector<Token>::const_iterator &curr,
    const std::vector<Token>::const_iterator &end
) {
    if (curr != end) {
        switch (curr->tokenDescriptor->tokenType) {
            // identifier-Token.
            case typename_token:
                return parseIdExpr (curr, end);
            default:
                ;
        }
    }
    return 0;
}
*/

/*
static ExprAST *parseBracketedBlock( std::vector<Token>::const_iterator &curr, const std::vector<Token>::const_iterator &end ){
    if( !tokenEquals( "{", curr, end ) ){ // assert the body of the if be enclosed in "{"
        cout << "error: missing '{' in statement" << endl;
        return 0;
    }
    ++curr;
    ExprAST *body = parseBlock( curr, end );
    if( 0 == body ){
        cout << "error: parsing a bracketed statement failed" << endl;
        return 0;
    }
    if( !tokenEquals( "}", curr, end ) ){ // assert the body of the if be enclosed in "}"
        cout << "error: missing '}' in statement" << endl;
        return 0;
    }
    ++curr;
    return body;
}
*/


// Phat Function, doh.
static ExprAST *parseStatement (std::vector<Token>::const_iterator &curr, const std::vector<Token>::const_iterator &end) {
    if (curr != end) {
        ExprAST *tmpAST;

        // empty-statement ?
        if (tokenEquals (";", curr, end)) {
            ++curr;
            return 0;
        }

        // block-statement ?
        if (tokenEquals ("{", curr, end)) {
            cout << "teh block start" << endl;
            ++curr;
            tmpAST = parseBlock (curr, end);
            if (tokenEquals ("}", curr, end)) {
                cout << "teh block end: " << tmpAST << endl;
                ++curr;
            } else {
                if (0 != tmpAST)
                    delete tmpAST;
                tmpAST = 0;
            }
            return tmpAST;
        }

        // if-then[-else] ?
        if (tokenEquals ("if", curr, end)) {
            ++curr;
            // scan if-clause
            const ExprAST *if_clause = parseParenExpr (curr, end);
            if (0 == if_clause) {
                cout << "error: malformed if-clause" << endl;
                return 0;
            }
            // scan body
            const ExprAST *if_body = parseStatement (curr, end);
            // scan else
            ExprAST *else_body = 0;
            if (tokenEquals ("else", curr, end)) {
                ++curr;
                else_body = parseStatement (curr, end);
            } else {
            }
            return new IfBlockAST (if_clause, if_body, else_body);
        }

        // while-loop ?
        if (tokenEquals ("while", curr, end)) {
            ++curr;
            // scan condition
            const ExprAST *cond = parseParenExpr (curr, end);
            if (0 == cond) {
                cout << "error: malformed while-condition" << endl;
                return 0;
            }
            // scan body
            const ExprAST *body = parseStatement (curr, end);
            return new WhileLoopAST (cond, body);
        }

        // do-while-loop ?
        if (tokenEquals ("do", curr, end)) {
            ++curr;
            // scan body
            const ExprAST *body = parseStatement (curr, end);
            // scan condition
            if (!tokenEquals ("while", curr, end)) {
                cout << "error: missing 'while' in do-while-statement" << endl;
                if (0 != body) delete body;
                return 0;
            }
            ++curr;
            const ExprAST *cond = parseParenExpr (curr, end);
            if (0 == cond) {
                cout << "error: malformed do-while-condition" << endl;
                if (0 != body) delete body;
                if (0 != cond) delete cond;
                return 0;
            }
            // close with ";"
            if (!tokenEquals (";", curr, end)) {
                cout << "error: missing ';' after do-while-statement" << endl;
                if (0 != body) delete body;
                if (0 != cond) delete cond;
                return 0;
            }
            ++curr;
            return new DoWhileLoopAST (cond, body);
        }

        // for-loop ?
        if (tokenEquals ("for", curr, end)) {
            ++curr;

            if (!tokenEquals ("(", curr, end)) {
                cout << "error: wrong for-loop syntax (1) (must be like 'for( init; condition; iterative ) body')" << endl;
                return 0;
            }
            ++curr;

            const ExprAST *init = parseExpr (curr,end);
            if (!tokenEquals (";", curr, end)) {
                cout << "error: wrong for-loop syntax (2) (must be like 'for( init; condition; iterative ) body')" << endl;
                if (0 != init) delete init;
                return 0;
            }
            ++curr;

            const ExprAST *cond = parseExpr (curr,end);
            if (!tokenEquals (";", curr, end)) {
                cout << "error: wrong for-loop syntax (3) (must be like 'for( init; condition; iterative ) body')" << endl;
                if (0 != init) delete init;
                if (0 != cond) delete cond;
                return 0;
            }
            ++curr;

            const ExprAST *iter = parseExpr (curr,end);
            if (!tokenEquals (")", curr, end)) {
                cout << "error: wrong for-loop syntax (4) (must be like 'for( init; condition; iterative ) body')" << endl;
                if (0 != init) delete init;
                if (0 != cond) delete cond;
                if (0 != iter) delete iter;
                return 0;
            }
            ++curr;

            const ExprAST *body = parseStatement (curr, end);

            // let's now build a while-loop
            BlockAST *whileBody = new BlockAST();
            whileBody->addTail (body);
            whileBody->addTail (iter);
            const WhileLoopAST *while_ = new WhileLoopAST (cond, whileBody);
            // not ready yet, implement init
            BlockAST *fullCorpse = new BlockAST();
            fullCorpse->addTail (init);
            fullCorpse->addTail (while_);
            return fullCorpse;
        }


        // Prepare possible rollback
        const std::vector<Token>::const_iterator backup = curr;


        // Declaration?
        // --> Note: currently only support single declaration w/out init.
        if (curr != end && curr->tokenDescriptor->tokenType == typename_token) {
            //const string type = curr->value;
            bool isValidDatatype = false;
            Datatype dt;
            if ("int" == curr->value) {
                dt = int_type;
                isValidDatatype = true;
            } else if ("float" == curr->value) {
                dt = float_type;
                isValidDatatype = true;
            } else {
                isValidDatatype = false;
            }
            if (isValidDatatype) {
                ++curr;

                if (curr != end && curr->tokenDescriptor->tokenType == id_token) {
                    const string id = curr->value;
                    ++curr;
                    if (tokenEquals (";", curr, end)) {
                        ++curr;
                        return new DeclarationAST (dt, id);
                    }
                }
            }
            curr = backup; // rollback
        }


        // Try to parse a simple statement.
        tmpAST = parseExpr (curr, end);
        if (tmpAST != 0) {
            if (tokenEquals (";", curr, end)) {    // simple statement must end on ";"
                ++curr;
                return tmpAST;
            }
            delete tmpAST;
        }
        // Rollback.
        curr = backup;

        return 0;
    }
    return 0;
}



static BlockAST *parseBlock (std::vector<Token>::const_iterator &curr, const std::vector<Token>::const_iterator &end) {
    BlockAST *block = 0;//new BlockAST();
    if (curr != end) {
        while (curr != end) {
            if (tokenEquals ("}", curr, end)) {    // this is more an soon-"}"-catch-optimisation. parseStatement() below should also return 0 when detecting a closing bracket
                return block;
            }

            ExprAST *tmp = 0;

            tmp = parseStatement (curr, end);
            if (0 != tmp) {
                if (0 == block) {   // first block element?
                    block = new BlockAST();
                }
                block->addTail (tmp);
            }
        }
    }
    return block;
}



static ExprAST *parseTopLevelExpression (std::vector<Token>::const_iterator &curr, const std::vector<Token>::const_iterator &end) {
    BlockAST *globalBlock = parseBlock (curr, end);
    if (0 != globalBlock) {
        // Reminder:
        //   explicit FunProtoAST (const std::string &name, const std::vector<Argument> &args)
        //   explicit FunAST (const FunProtoAST *prototype, const ExprAST *body)
        FunProtoAST *proto = new FunProtoAST ("", std::vector<FunProtoAST::Argument>());
        return new FunAST (proto, globalBlock);
    } else {
        return 0;
    }
}



namespace picogen { /// \todo find proper namespace for this

    PicoPico::PicoPico(const std::string &code, Flags flags, const ExprAST **ast) {

        match_results<std::string::const_iterator> results;
        string::const_iterator begin = code.begin();
        string::const_iterator end   = code.end();

        vector<Token> tokens;

        // tokenize
        while (begin != end) {
            bool found = false;
            for (int i=0; i<tokenDescriptorCount; ++i) {
                bool matches = regex_search (
                                   begin,
                                   end,
                                   results,
                                   tokenDescriptors[i].regex,
                                   match_continuous | match_nosubs | format_first_only
                               );
                if (matches) {
                    //cout << "match for '" << tokenDescriptors[i].name << "'! (" << string( results[0].first, results[0].second ) << ")" << endl;
                    begin = results[0].second;
                    found = true;
                    if (!tokenDescriptors[i].omit)
                        tokens.push_back (Token (&tokenDescriptors[i], string (results[0].first,results[0].second)));
                    break;
                }
            }
            if (!found) {
                cerr << "Error while tokenizing at \"" <<
                     ( (string (begin,end).length() > 25) // print maximum n chars
                       ? (string (begin,begin+25) + string ("......")) // (if rest of string.length > n, then cat '......' into output)
                       : string (begin, end)
                     )
                     << "\"" << endl;
                /// \todo throw or something
                break;
            }
        }

        // done
        vector<Token>::const_iterator it;
        if (true) {
            cout << endl;
            for (it=tokens.begin(); it != tokens.end(); ++it) {
                cout << "[" << it->tokenDescriptor->name << "(" << it->value << ")" << "]";
            }
            cout << endl;
        }

        // Parse.
        vector<Token>::const_iterator curr = tokens.begin();
        const ExprAST *ast_ = parseTopLevelExpression (curr, tokens.end());
        if (0 != ast_) {

            cout << "\nprogram's AST:{{\n";
            ast_->print (1);
            cout << "\n}}" << endl;
            AST2LLVM llvm;
            ast_->accept (llvm);
            llvm.compile();

            if ((flags & return_ast) == return_ast && 0 != ast) {
                *ast = ast_;
            } else {
                delete ast_;
            }
        } else {
            cout << "parse error or no program" << endl;
        }
    }


    PicoPico::~PicoPico() {
    }


} /* namespace picogen */
