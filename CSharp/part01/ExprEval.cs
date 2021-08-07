using System;
using System.Reflection;
using System.Reflection.Emit;
using System.Threading; 


namespace ExpressionEvaluator {

	public enum TOKEN {
			ILLEGAL_TOKEN=-1,
			TOK_PLUS=1,
			TOK_MUL,
			TOK_DIV, 
			TOK_SUB,
			TOK_OPAREN,
			TOK_CPAREN,
			TOK_DOUBLE,
			TOK_NULL 
	}
		
	public class Lexer
	{
			String IExpr;
			int    index = 1;
			int    length;
			double number;
			public Lexer(String Expr)
			{
				IExpr = Expr;
				length = IExpr.Length; 
				index = 0;
            			
			}
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			public TOKEN GetToken()
			{
				TOKEN tok = TOKEN.ILLEGAL_TOKEN;

				while (index < length && (IExpr[index] == ' ' || IExpr[index]== '\t') )
					index++;

				if ( index == length)
					return TOKEN.TOK_NULL;
 
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
				  
						while ( index < length &&( IExpr[index] == '0' || 
							IExpr[index]==  '1' ||
							IExpr[index] == '2' || 
							IExpr[index]== '3'  ||
							IExpr[index] == '4' || 
							IExpr[index]== '5'  ||
							IExpr[index] == '6' || 
							IExpr[index]== '7'  ||
							IExpr[index] == '8' || 
							IExpr[index]== '9'  ))
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
		/// 
		class Stack{
			double[] stk;
			int top_stack = 0; 
			public Stack(){stk = new double[256];top_stack = 0;}
			public void Clear(){top_stack = 0;}
			public void push(double dbl ){
				if ( top_stack == 255 ){Console.WriteLine("Stack OverFlow"); throw new Exception();}
				stk[top_stack++] = dbl;
			}
			public double  pop(){
				if ( top_stack == 0 ){Console.WriteLine("Stack UnderFlow"); throw new Exception();}
				return stk[--top_stack];
			}
                                    }

	
		class RDParser : Lexer{
			TOKEN Current_Token;
			Stack ValueStack = new Stack() ;
			public RDParser(String str):base(str){}
			public double CallExpr(){ 
				ValueStack.Clear();Current_Token= GetToken(); Expr();  double nd = ValueStack.pop();
				return nd;
			}
			public void Expr(){
				TOKEN l_token;
				Term();
				if  ( Current_Token == TOKEN.TOK_PLUS  || Current_Token == TOKEN.TOK_SUB ) {
					l_token = Current_Token;Current_Token = GetToken();
					Expr();
					double x = ValueStack.pop(); double y = ValueStack.pop();
					ValueStack.push( (l_token == TOKEN.TOK_PLUS ) ? (x + y) : (y-x) ); 

				}
                                                   }

			public void Term(){
				TOKEN l_token;
				Factor();
				if ( Current_Token == TOKEN.TOK_MUL  || Current_Token == TOKEN.TOK_DIV ) {
					l_token = Current_Token; Current_Token = GetToken();
					Term();
					double x = ValueStack.pop(); double y = ValueStack.pop();
					if ( x == 0 ) { Console.WriteLine("Division By Zero Error");throw new Exception();}
					ValueStack.push( (l_token == TOKEN.TOK_MUL ) ? (x * y) :  (y/x) ); 
				
				}
                                                    }

			public void Factor(){
				TOKEN l_token;
				if ( Current_Token == TOKEN.TOK_DOUBLE ){
					ValueStack.push(GetNumber()); Current_Token = GetToken();  
				} 
				else if ( Current_Token == TOKEN.TOK_OPAREN ){
					Current_Token = GetToken();   
					Expr();  // Recurse
				                 if ( Current_Token != TOKEN.TOK_CPAREN ){Console.WriteLine("Missing Closing Parenthesis\n"); throw new Exception();}   
					Current_Token = GetToken();            
				} 
				else if ( Current_Token == TOKEN.TOK_PLUS || Current_Token == TOKEN.TOK_SUB ){
					l_token = Current_Token; Current_Token = GetToken();
					Factor();
					double  x = ValueStack.pop();
					if ( l_token == TOKEN.TOK_SUB ) { x = -x;}
					ValueStack.push(x);  
   
				}
				else { Console.WriteLine("Illegal Token"); throw new Exception();} 
			}

                                     }
		 class EntryPoint {
			public static void Main(string[] args) {
 				 if ( args.Length == 0 || args.Length > 1 ) {  
    					Console.WriteLine("Usage : Expr \"<expr>\" \n");
    					Console.WriteLine(" eg:- Expr \"2*3+4\" \n");
    					Console.WriteLine(" Expr \"-2-3\" \n");
   					 return;
  			 	}

			try {
    				RDParser parser = new RDParser (args[0]);
    				double nd  = parser.CallExpr();
   				 Console.WriteLine("The Evaluated Value is {0} \n",nd );
 			}
  			catch(Exception e ) {
    				Console.WriteLine("Error Evaluating Expression\n");
    				Console.WriteLine(e);
    				return;
  			}
		}


	}
	
}