//////////////////////////////////////////////////////////////////////
//
// 
//
//
//
// BNF Grammar for Evaluator
//
//
// <expr> ::= <term> | <term> { '+' | '-' } <expr>
//
// <term> ::= <factor> | <factor> { '*' | '/' } <term>
//
// <factor> ::= <TOK_DOUBLE> | ( <expr> ) | { + | - } <factor>
//
//
//
//
//
//

using System;
using System.Reflection;
using System.Reflection.Emit;
using System.Threading;
using System.Collections.Generic;
///////////////////////////////////////////////////////////////////////////////
//
//
// enum TOKEN - Symbolic constants for Lexical Tokens
//
// class Lexer - Lexical Analyzer module. ( proof of concept endeavour )
//
// class CLRParser - A recursive descent parser 



   public enum OPERATOR
    {
        //--- supports +,-,*/
       ILLEGAL = -1, PLUS, MINUS, MUL, DIV
    }

    /// <summary>
    ///      One can store the stack frame inside this class
    /// </summary>
    public class RUNTIME_CONTEXT{
        public RUNTIME_CONTEXT() {}
    }
   
    /// <summary>
    ///     Expression is what you evaluates for it's Value
    /// </summary>
    /// 
    public abstract class Expr {
        public abstract double Evaluate(RUNTIME_CONTEXT cont);
    }
    /// <summary>
    ///      one can store number inside the class
    /// </summary>
   public class NumericConstant : Expr {
        private double _value;
        ///     Construction does not do much , just keeps the 
        ///     value assigned to the private variable
        public NumericConstant(double value){_value = value;}
        /// <summary>
        ///     While evaluating a numeric constant , return the _value
        /// </summary>
        /// <param name="cont"></param>
        /// <returns></returns>
        public override double Evaluate(RUNTIME_CONTEXT cont) { return _value;}

    }

    /// <summary>
    ///     This class supports Binary Operators like + , - , / , *
    /// </summary>
    public class BinaryExpr : Expr
    {
        private Expr _ex1, _ex2;
        private OPERATOR  _op;
        /// <summary>
        /// 
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <param name="op"></param>

        public BinaryExpr(Expr a, Expr b, OPERATOR op){ _ex1 = a;  _ex2 = b;  _op = op;}

        /// <summary>
        ///     While evaluating a numeric constant , return the _value
        /// </summary>
        /// <param name="cont"></param>
        /// <returns></returns>
        public override double Evaluate(RUNTIME_CONTEXT cont)
        {


            switch (_op)
            {
                case OPERATOR.PLUS:
                    return _ex1.Evaluate(cont) + _ex2.Evaluate(cont);
                case OPERATOR.MINUS:
                    return _ex1.Evaluate(cont) - _ex2.Evaluate(cont);
                case OPERATOR.DIV:
                    return _ex1.Evaluate(cont) / _ex2.Evaluate(cont);
                case OPERATOR.MUL:
                    return _ex1.Evaluate(cont) * _ex2.Evaluate(cont);

            }

            return Double.NaN;

        }

    }

 /// <summary>
    ///     This class supports Unary Operators like + , - , / , *
    /// </summary>
    public class UnaryExpr : Expr
    {
        private BinaryExpr  _ex1;
        public UnaryExpr(Expr a, OPERATOR op)
        {
         
            _ex1 = new BinaryExpr(new NumericConstant(0),a,op);
             
        }
        public override double Evaluate(RUNTIME_CONTEXT cont)
        {
             return _ex1.Evaluate(cont);
           
        }

    }

 
  ////////////////////////////////////////////////
  //
  // Lexical Tokens
  //
  //
  //
  public enum TOKEN
  {  
    ILLEGAL_TOKEN=-1, // Not a Token
    TOK_PLUS=1, // '+'
    TOK_MUL, // '*'
    TOK_DIV, // '/'
    TOK_SUB, // '-'
    TOK_OPAREN, // '('
    TOK_CPAREN, // ')'
    TOK_DOUBLE, // '('
    TOK_NULL // End of string
  }
  //////////////////////////////////////////////////////////
  //
  // A naive Lexical analyzer which looks for operators , Parenthesis
  // and number. All numbers are treated as IEEE doubles. Only numbers
  // without decimals can be entered. Feel free to modify the code
  // to accomodate LONG and Double values
  public class Lexer
  {
    String IExpr; // Expression string
    int index ; // index into a character
    int length; // Length of the string
    double number; // Last grabbed number from the stream
    /////////////////////////////////////////////
    //
    // Ctor
    //
    //
    public Lexer(String Expr)
    {
      IExpr = Expr;
      length = IExpr.Length;
      index = 0;
    }
    /////////////////////////////////////////////////////
    // Grab the next token from the stream
    //
    //
    //
    //
    public TOKEN GetToken()
    {
      TOKEN tok = TOKEN.ILLEGAL_TOKEN;
      ////////////////////////////////////////////////////////////
      //
      // Skip the white space
      //
      while (index < length &&
      (IExpr[index] == ' ' || IExpr[index]== '\t') )
        index++;
      //////////////////////////////////////////////
      //
      // End of string ? return NULL;
      //
      if ( index == length)
        return TOKEN.TOK_NULL;
      /////////////////////////////////////////////////
      //
      //
      //
      switch(IExpr[index])
      {
        case '+':
          tok = TOKEN.TOK_PLUS;
          index++;
          break;
        case '-':
          tok = TOKEN.TOK_SUB;
          index++;
          break;
        case '/':
          tok = TOKEN.TOK_DIV;
          index++;
          break;
        case '*':
          tok = TOKEN.TOK_MUL;
          index++;
          break;
        case '(':
          tok = TOKEN.TOK_OPAREN;
          index++;
          break;
        case ')':
          tok = TOKEN.TOK_CPAREN;
          index++;
          break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
          String str="";
          while ( index < length &&
          ( IExpr[index]== '0' ||
          IExpr[index]== '1' ||
          IExpr[index]== '2' ||
          IExpr[index]== '3' ||
          IExpr[index]== '4' ||
          IExpr[index]== '5' ||
          IExpr[index]== '6' ||
          IExpr[index]== '7' ||
          IExpr[index] == '8'||
          IExpr[index]== '9' ))
          {
            str += Convert.ToString(IExpr[index]);
            index ++;
          }
          number = Convert.ToDouble(str);
          tok = TOKEN.TOK_DOUBLE;
        }
        break;
        default:
          Console.WriteLine("Error While Analyzing Tokens");
          throw new Exception();
      }
    return tok;
    }


    public double GetNumber() { return number; }
}



     

 /// <summary>
    /// 
    /// </summary>
    public class RDParser : Lexer
    {
        TOKEN Current_Token;


        public RDParser(String str)
            : base(str)
        {


        }

        /// <summary>
        ///      
        /// </summary>
        /// <returns></returns>
        public Expr CallExpr()
        {
            Current_Token = GetToken();
            return Expr();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public Expr Expr()
        {
            TOKEN l_token;
            Expr RetValue = Term();
            while (Current_Token == TOKEN.TOK_PLUS || Current_Token == TOKEN.TOK_SUB)
            {
                l_token = Current_Token;
                Current_Token = GetToken();
                Expr e1 = Expr();
                RetValue = new BinaryExpr(RetValue, e1,
                    l_token == TOKEN.TOK_PLUS ? OPERATOR.PLUS : OPERATOR.MINUS);

            }

            return RetValue;

        }
        /// <summary>
        /// 
        /// </summary>
        public Expr Term()
        {
            TOKEN l_token;
            Expr RetValue = Factor();

            while (Current_Token == TOKEN.TOK_MUL || Current_Token == TOKEN.TOK_DIV)
            {
                l_token = Current_Token;
                Current_Token = GetToken();


                Expr e1 = Term();
                RetValue = new BinaryExpr(RetValue, e1,
                    l_token == TOKEN.TOK_MUL ? OPERATOR.MUL : OPERATOR.DIV);

            }

            return RetValue;
        }

        /// <summary>
        ///    
        /// </summary>
        public Expr Factor()
        {
            TOKEN l_token;
            Expr RetValue = null;
            if (Current_Token == TOKEN.TOK_DOUBLE)
            {

                RetValue = new NumericConstant(GetNumber());
                Current_Token = GetToken();

            }
            else if (Current_Token == TOKEN.TOK_OPAREN)
            {

                Current_Token = GetToken();

                RetValue = Expr();  // Recurse

                if (Current_Token != TOKEN.TOK_CPAREN)
                {
                    Console.WriteLine("Missing Closing Parenthesis\n");
                    throw new Exception();

                }
                Current_Token = GetToken();
            }

            else if (Current_Token == TOKEN.TOK_PLUS || Current_Token == TOKEN.TOK_SUB)
            {
                l_token = Current_Token;
                Current_Token = GetToken();
                RetValue = Factor();

                RetValue = new UnaryExpr(RetValue,
                     l_token == TOKEN.TOK_PLUS ? OPERATOR.PLUS : OPERATOR.MINUS);
            }
            else
            {

                Console.WriteLine("Illegal Token");
                throw new Exception();
            }


            return RetValue;

        }

 
class Temp {
//////////////////////////////////////////////////////////////////
//
// Entry point for the Test Driver
//
//
//
public static void Main(string[] args)
{
  if ( args.Length == 0 || args.Length > 1 )
  {  
    Console.WriteLine("Usage : Expr \"<expr>\" \n");
    Console.WriteLine(" eg:- Expr \"2*3+4\" \n");
    Console.WriteLine(" Expr \"-2-3\" \n");
    return;
  }




  try {
    RDParser parser = new RDParser (args[0]);
    Expr nd  = parser.CallExpr();
    Console.WriteLine("The Evaluated Value is {0} \n",nd.Evaluate(null) );
  
  
          
     Console.Read();
          
             
  }
  catch(Exception e )
  {
    Console.WriteLine("Error Evaluating Expression\n");
    Console.WriteLine(e);
    return;
  }
}

}


  }



