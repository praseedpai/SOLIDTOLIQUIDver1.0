import java.lang.*;
import java.util.*;
import java.util.stream.*;

import java.util.stream.Collectors.*;
//import static java.util.streams.Collectors.toList;


//////////////////////////////////
// List of operators supported by 
// the evaluator
//
//
enum OPERATOR{
        //--- supports +,-,*/
        ILLEGAL,PLUS,MINUS,MUL,DIV,UNARY_PLUS,UNARY_MINUS
}

////////////////////////////
// Kinds of Expr -> Operator or a Value?
//
enum ExprKind{
        OPERATOR , VALUE
}
//////////////////////////////////////////
//
//
class EXPR_ITEM {

    public EXPR_ITEM(){
            op = OPERATOR.ILLEGAL;
    }
    public boolean SetOperator( OPERATOR op ){
            this.op = op;
            this.knd = ExprKind.OPERATOR;
            return true;
    }
    public boolean SetValue(double value) {
            this.knd = ExprKind.VALUE;
            this.Value = value;
            return true;
    }

    public String toString() {

           if ( this.knd ==  ExprKind.VALUE )
		return "" +this.Value;
           else

                return "Operator";
           
    }

    public ExprKind knd;
    public double Value;
    public OPERATOR op;
}

/// Base class for all Expression
/// supports accept method to implement 
/// so called "Double Dispatch". Search for 
/// "Double Dispatch" and Visitor to understand more 
/// about this strategy
/// 
interface Expr{
        public abstract double accept(IExprVisitor expr_vis);
}

////////////////////////////////
// The Visitor interface contains methods for 
// each of the concerte node 
//
//
interface IExprVisitor{
         double Visit(Number num);
         double Visit(BinaryExpr bin);
         double Visit(UnaryExpr un);
}

///////////////////////////////
// A class to represent IEEE 754 interface
//
//
class Number implements Expr {
       private double NUM;
       public double getNUM() { return NUM;} 
       public void SetNUM(double num)   { NUM = num; }
       public Number(double n) { this.NUM = n; }
       public  double accept(IExprVisitor expr_vis)
       {
            return expr_vis.Visit(this);
       }
}

///////////////////////////////////////
// Model BinaryExpr
//
//
class BinaryExpr implements Expr
{
       private Expr left;
       private Expr right;
       private OPERATOR OP;

        

       public  BinaryExpr(Expr l,Expr r , OPERATOR op )
       {
	  left = l; right = r; OP = op;
       }
       public OPERATOR getOP() { return OP; }
       public Expr getLeft() { return left; }
       public Expr getRight() { return right; }
       public  double accept(IExprVisitor expr_vis)
       {
            return expr_vis.Visit(this);
       }
}
////////////////////////////////////
// class to Model UnaryExpr
//
//
class UnaryExpr implements Expr
{
       
       private Expr right;
       private OPERATOR OP;
       public  UnaryExpr(Expr r , OPERATOR op )
       {
	  right = r; OP = op;
       }
       public OPERATOR getOP() { return OP; }
       public Expr getRight() { return right; }
       public  double accept(IExprVisitor expr_vis)
       {
            return expr_vis.Visit(this);
       }
}

//////////////////////////////////////////////////
// A Visitor to Evaluate Expression
//
//
class ReversePolishEvaluator implements IExprVisitor {

        public double Visit(Number num)
        {
            System.out.println(num.getNUM() + " ");
            return 0;
        }
        public double Visit(BinaryExpr bin)
        {
            bin.getLeft().accept(this);
            bin.getRight().accept(this);
	    OPERATOR temp = bin.getOP();
            if (temp == OPERATOR.PLUS)
		System.out.println(" + ");
            else if (temp == OPERATOR.MUL)
                System.out.println(" * ");
            else if (temp == OPERATOR.DIV)
                System.out.println(" / ");
            else if (temp == OPERATOR.MINUS)
                System.out.println(" - ");

            return Double.NaN;

        }
        public double Visit(UnaryExpr un)
        {
            OPERATOR temp = un.getOP();
            un.getRight().accept(this);
            if (temp == OPERATOR.UNARY_PLUS)
                System.out.println("  + ");
            else if (temp == OPERATOR.UNARY_MINUS)
                System.out.println("  - ");
            return Double.NaN;

        }

    }

////////////////////////////////
//
// A Tree Walker Evaluator
//
class TreeEvaluatorVisitor implements IExprVisitor{
        
        public  double Visit(Number num)
        {
            return num.getNUM();
        }
        public double Visit(BinaryExpr bin)
        {
           OPERATOR temp = bin.getOP();
           if (temp == OPERATOR.PLUS)
                return bin.getLeft().accept(this) + bin.getRight().accept(this);
            else if (temp == OPERATOR.MUL)
            return bin.getLeft().accept(this) * bin.getRight().accept(this);
            else if (temp == OPERATOR.DIV)
            return bin.getLeft().accept(this) / bin.getRight().accept(this); 
            else if (temp == OPERATOR.MINUS)
              return bin.getLeft().accept(this) - bin.getRight().accept(this);

             return Double.NaN;

        }
        public double Visit(UnaryExpr un)
            
        {
           OPERATOR temp = un.getOP();
           if (temp == OPERATOR.UNARY_PLUS)
                return +  un.getRight().accept(this);
           else  if (temp == OPERATOR.UNARY_MINUS)
                return - un.getRight().accept(this);
             return Double.NaN;

        }
}

/////////////////////////////////////
// A Flattener for Expressions
//
class FlattenVisitor implements IExprVisitor {
        ArrayList<EXPR_ITEM> ils = null;

        private EXPR_ITEM MakeListItem(double num)
        {
            EXPR_ITEM temp = new EXPR_ITEM();
            temp.SetValue(num);
    
            return temp;
        }

        private EXPR_ITEM MakeListItem(OPERATOR op)
        {
            EXPR_ITEM temp = new EXPR_ITEM ();
            temp.SetOperator(op);

            return temp;
        }

        public List<EXPR_ITEM> FlattenedExpr()
        {
            return ils;
        }
        public FlattenVisitor()
        {
            ils = new ArrayList<EXPR_ITEM>();
        }
        public double Visit(Number num)
        {
        
            ils.add(MakeListItem(num.getNUM()));
            return 0;
        }
        public double Visit(BinaryExpr bin)
        {
            bin.getLeft().accept(this);
            bin.getRight().accept(this);

               ils.add(MakeListItem(bin.getOP()));
            

            return Double.NaN;

        }
        public double Visit(UnaryExpr un)
        {

            un.getRight().accept(this);
            ils.add(MakeListItem(un.getOP()));
            return 0;
          

        }

    }


 


public class ExprEvaluator02
{

        public static double Evaluate( List<EXPR_ITEM> ls)
        {
            Stack<Double> stk = new Stack<Double>();

            for( EXPR_ITEM s : ls )
            {
                if (s.knd == ExprKind.VALUE)
                    stk.push(s.Value);
                else
                {
                    switch(s.op) {
                        case PLUS:
                            stk.push(stk.pop() + stk.pop());
                            break;
                        case MINUS:
                            stk.push(stk.pop() - stk.pop());
                            break;
                        case DIV:
                            double n = stk.pop();
                            stk.push(stk.pop() / n);
                            break;
                        case MUL:
                            stk.push(stk.pop() * stk.pop());
                        case UNARY_PLUS:
                            break;
                        case UNARY_MINUS:
			    stk.push(-stk.pop());
			    break;

                            

                    }


                }


            }
            return stk.pop();
        }

        public static void Print( Expr r ) {
              ExprList(r).stream().forEach(System.out::println);

        }

        public static List<EXPR_ITEM> PrintOperands( Expr r )
        {
           return ExprList(r).stream().filter( a -> a.knd == ExprKind.VALUE ).collect(Collectors.toList());
        }

        public static void DumpListContents( List<EXPR_ITEM> ls )
        {
            ls.stream().forEach(System.out::println);

        }

        public static void DumpList( List<EXPR_ITEM> ls ){
		Iterator itr=ls.iterator();  
 	 	while(itr.hasNext()){ 
                   EXPR_ITEM nt = (EXPR_ITEM )itr.next();
                   System.out.println(nt);
  	        }  

        }

        public static List<EXPR_ITEM> ExprList(Expr r) {
         

          FlattenVisitor fl = new FlattenVisitor();
          r.accept(fl);
          return fl.FlattenedExpr();
          

        }
	public static void main(String [] args )
	{

              Expr r = new BinaryExpr(new Number(2),
                new BinaryExpr(new Number(3), new Number(4), OPERATOR.MUL),
                OPERATOR.PLUS);
           
            r.accept(new ReversePolishEvaluator());
            System.out.println("----");

           /// double n = r.accept(new TreeEvaluatorVisitor());
           // System.out.println(n);

          //  DumpList(ExprList(r));

          //  Print(r);

          //  System.out.println("----");

          //  DumpListContents(PrintOperands(r));
            System.out.println("----");
            double n = Evaluate(ExprList(r));
            System.out.println("-------------" + n );

	}

}