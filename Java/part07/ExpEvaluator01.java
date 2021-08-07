import java.lang.*;

//////////////////////////////////
// List of operators supported by 
// the evaluator
//
//
enum OPERATOR{
        //--- supports +,-,*/
        PLUS,MINUS,MUL,DIV,UNARY_PLUS,UNARY_MINUS
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

public class ExpEvaluator01
{
	public static void main(String [] args )
	{

              Expr r = new BinaryExpr(new Number(2),
                new BinaryExpr(new Number(3), new Number(4), OPERATOR.MUL),
                OPERATOR.PLUS);
           
            r.accept(new ReversePolishEvaluator());
            System.out.println("----");

            double n = r.accept(new TreeEvaluatorVisitor());
            System.out.println(n);

	}

}