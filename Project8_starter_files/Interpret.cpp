//
// Created by DarkenedAsian on 5/1/2022.
//

#include "Interpret.h"
using namespace std;
unordered_map<string, int> expression_table;
vector<string> RPN_vector; //stack for RPN
stack<int> num_stack; //stack for converting sring arg into int

bool isOperator(string element){
    if(element == "+"||element == "-"||element == "*"||element == "/"||element == "%"||element == "&&"||element == "||"
    ||element == "<"||element == ">"||element == "=="||element == "!="||element == "<="||element == ">="||element == "!"||element == "~")
        return true;
    return false;
}

bool isNumber(string element){
    char e = element[0]; //first element
    if(e == '0'||e =='1'||e =='2'||e =='3'||e =='4'||e =='5'||e =='6'||e =='7'||e =='8'||e =='9')
        return true;
    return false;
}
/*
 * Converts rest of tokens in command line into elements on vector
 * Example: var x + * 3 7 * 9 2 : Will change  operators vector to { +,*,3,7,*,9,2 }
 * This allows for use of vector as stack data type, which makes computing reverse polish notation easier
 * */
void PushOnVector(){
    string text = "text";
    string output = "output";
    string set = "set";
    string variable = "var";
    string next = peek_next_token();
   while(next_token_type!= END && next != text && next != output && next != set && next != variable ){
       read_next_token();
       string token = string(next_token());
       if(token == "//"){
           skip_line();
           break;
       }
           RPN_vector.push_back(token);
           next = peek_next_token();
   }
}
/*
 * Helper function to compute_RPN
 * Utilizes global: num stack
 * Input should be an operator, function returns result of operator using num_stack elements as operands
 * */
int computeOperation(string op){
    int a,b;
    if(op == "~"){
        a = num_stack.top();
        num_stack.pop();
        return a*-1;
    }
    else if(op == "!"){
        a = num_stack.top();
        num_stack.pop();
        if(a){
            return 0;
        }
        else{
            return 1;
        }
    }
    a = num_stack.top();
    num_stack.pop();
    b = num_stack.top();
    num_stack.pop();
    if(op == "+"){return a+b;}
    else if(op == "-"){return a-b;}
    else if(op == "*"){return a*b;}
    else if(op == "/"){return a/b;}
    else if(op == "%"){return a&b;}
    else if(op == "&&"){return a&&b;}
    else if(op == "||"){return a||b;}
    else if(op == ">"){return a>b;}
    else if(op == "<"){return a<b;}
    else if(op == "=="){return a==b;}
    else if(op == "!="){return a!=b;}
    else if(op == "<="){return a<=b;}
    else if(op == ">="){return a>=b;}
    return 0;
}
/*
 * Utilizes global vector RPN_vector & unordered map expression_table
 * Returns output of stored RPN in vector
 * Clears RPN_vector after use
 * */
int compute_RPN(){
    int result;
    while(RPN_vector.size() > 0){
        string current_token = RPN_vector.back();
        //check if number, if it is then convert to int
        if(isNumber(current_token)){
            num_stack.push(stoi(current_token));
            RPN_vector.pop_back();
        }
        else if (isOperator(current_token)){
            result = computeOperation(current_token);
            RPN_vector.pop_back(); //consume op
            num_stack.push(result);
        }
        else{ //current_token is name of a var, since not num or op
            result = expression_table[current_token];
            num_stack.push(result); //since this will only happen when variable is used as value in RPN, replace var w/ num on stack
            RPN_vector.pop_back(); //consume var on RPN_vector
        }
    }
    return result;
}

void run(){
    string text = "text";
    string output = "output";
    string set = "set";
    string variable = "var";
    string token;

    do{
        //first token should always be an operator
        read_next_token();
        token = next_token();
        if(next_token_type == INVALID){}
        else if(token == "\\"){ skip_line();}
        else if(token == text){
            read_next_token();
            string print_text = next_token();
            if(print_text == "//"){skip_line();}
            cout << print_text;
            continue;
        }
        else if(token == output){
            PushOnVector();
            if(RPN_vector.size() == 1){//if num expressions = 1
                if(isNumber(RPN_vector.back())){ //case where you get output (number)
                    cout << stoi(RPN_vector.back()); //removed endl
                    RPN_vector.pop_back();
                }
                else{//case where you get output (variable)
                    cout << expression_table[RPN_vector.back()]; //removed endl
                    RPN_vector.pop_back();
                }

            }
            else{
                int value = compute_RPN();
                num_stack.pop();
                cout << value;//removed endly
            }
        }
        else if(token == set){
            //get name of var i.e map key
            read_next_token();
            string var_key = next_token();
            //check if var exists already
            if (expression_table.find(var_key) != expression_table.end())
            {
                cout << "variable " << var_key << " incorrectly re-initialized"; //removed endl
            }
            //push vector
            PushOnVector();
            // if vector size == 1, meaning only 1 element, then map == vector element
            if(RPN_vector.size() == 1){
                expression_table[var_key] = stoi(RPN_vector.back());
                RPN_vector.clear();
            }
                // else compute vector and store value in map
            else{
                int rpn_value = compute_RPN();
                num_stack.pop(); //prevents memory leak by popping result off stack
                expression_table[var_key] = rpn_value;
            }
        }
        else if(token == variable){
            //get name of var i.e map key
            read_next_token();
            string var_key = next_token();
            //check if variable already exists
            if (expression_table.find(var_key) != expression_table.end())
            {
                cout << "variable " << var_key << " incorrectly re-initialized";
            }
            //push vector
            PushOnVector();
            // if vector size == 1, meaning only 1 element, then map == vector element
            if(RPN_vector.size() == 1){
                expression_table[var_key] = stoi(RPN_vector.back());
                RPN_vector.clear();
            }
            // else compute vector and store value in map
            else{
                int rpn_value = compute_RPN();
                num_stack.pop(); //prevents memory leak by popping result off stack
                expression_table[var_key] = rpn_value;
            }
        }
    }while(next_token_type != END);
    int j = 0;
}
