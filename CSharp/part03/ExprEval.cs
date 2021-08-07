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
       public abstract double accept(IExprVisitor expr_vis);
    }
    /// <summary>
    ///      one can store number inside the class
    /// </summary>
   public class NumericConstant : Expr {
        private double _value;
        ///     Construction does not do much , just keeps the 
        ///     value assigned to the private variable
        public NumericConstant(double value){_value = value;}
        public override double accept(IExprVisitor expr_vis)
        {
            return expr_vis.Visit(this);
        }
        public double NUM { get { return _value; } }
    }

     public class BinaryExpr : Expr
    {
        public Expr Left { get; set; }
        public Expr Right { get; set; }
        public OPERATOR OP { get; set; }
        public BinaryExpr(Expr l, Expr r, OPERATOR op)
        {
            Left = l; Right = r; OP = op;
        }
        public override double accept(IExprVisitor expr_vis)
        {
            return expr_vis.Visit(this);
        }

    }

 /// <summary>
    /// Class UnaryExpr models a unary expression of the form
    /// <OPER> <OPERAND> 
    /// </summary>
    public class UnaryExpr : Expr
    {
        public Expr Right;
        public OPERATOR OP;

        public UnaryExpr(Expr r, OPERATOR op)
        {
            Right = new BinaryExpr(new NumericConstant(0),
                r, op);
        }
        public override double accept(IExprVisitor expr_vis)
        {
            return expr_vis.Visit(this);
        }
    }

 //---------------------------------------------- Visitor Interface
  
    /// <summary>
    ///  Our Visitor Interface. The Purpose of seperating Processing
    ///  Of Nodes and Data Storage (heirarchy) is for various transformations on
    ///  the composites created.
    /// </summary>
    public interface IExprVisitor
    {
        double Visit(NumericConstant  num);
        double Visit(BinaryExpr bin);
        double Visit(UnaryExpr un);
    }

 /// <summary>
    ///  A Visitor which evaluates the Infix expression using a Stack
    ///  We will leverage stack implementation available with .NET 
    ///  collections API
    /// </summary>
    public class StackEvaluator : IExprVisitor
    {
        private Stack<double> eval_stack = new Stack<double>();

        public double get_value() { return eval_stack.Pop(); }
        public StackEvaluator()
        {
            eval_stack.Clear();
        }
        public double Visit(NumericConstant num)
        {
            eval_stack.Push(num.NUM);
            return 0;
        }
        public double Visit(BinaryExpr bin)
        {
            bin.Left.accept(this);
            bin.Right.accept(this);

            if (bin.OP == OPERATOR.PLUS)
                eval_stack.Push(eval_stack.Pop() + eval_stack.Pop());
            else if (bin.OP == OPERATOR.MUL)
                eval_stack.Push(eval_stack.Pop() * eval_stack.Pop());
            else if (bin.OP == OPERATOR.DIV)
                eval_stack.Push(eval_stack.Pop() / eval_stack.Pop());
            else if (bin.OP == OPERATOR.MINUS)
                eval_stack.Push(eval_stack.Pop() - eval_stack.Pop());

            return Double.NaN;

        }
        public double Visit(UnaryExpr un)
        {

            un.Right.accept(this);
            if (un.OP == OPERATOR.PLUS)
                eval_stack.Push(eval_stack.Pop());
            else if (un.OP == OPERATOR.MINUS)
                eval_stack.Push(-eval_stack.Pop());
            return Double.NaN;

        }

    }


 /// <summary>
    ///  Tree Evaluator - Evaluates Expression Tree through Depth 
    ///  First Traversal
    /// </summary>
    public class TreeEvaluatorVisitor : IExprVisitor
    {

        public double Visit(NumericConstant num)
        {
            return num.NUM;
        }
        public double Visit(BinaryExpr bin)
        {
            if (bin.OP == OPERATOR.PLUS)
                return bin.Left.accept(this) + bin.Right.accept(this);
            else if (bin.OP == OPERATOR.MUL)
                return bin.Left.accept(this) * bin.Right.accept(this);
            else if (bin.OP == OPERATOR.DIV)
                return bin.Left.accept(this) / bin.Right.accept(this);
            else if (bin.OP == OPERATOR.MINUS)
                return bin.Left.accept(this) - bin.Right.accept(this);

            return Double.NaN;

        }
        public double Visit(UnaryExpr un)

        {
            if (un.OP == OPERATOR.PLUS)
                return +un.Right.accept(this);
            else if (un.OP == OPERATOR.MINUS)
                return -un.Right.accept(this);
            return Double.NaN;

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
    Console.WriteLine("The Evaluated Value is {0} \n",nd );
  
   
              StackEvaluator s = new StackEvaluator();
              nd.accept(s);
              Console.WriteLine(s.get_value());
              Console.WriteLine();

         
      
  
  
          
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



