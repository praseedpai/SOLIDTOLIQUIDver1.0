Function.prototype.crockfordCurry = function () {
    "use strict";
    var slice = Array.prototype.slice,  args = slice.apply(arguments), that = this;
    return function () {return that.apply(null, args.concat(slice.apply(arguments)));};
};
function add(x) {
    "use strict";
    return function (y) {return x + y;};
}
 function multiply(x) {
    "use strict";
    return function (y) { return x * y; };
}
Function.prototype.haskellCurry = function () {
    "use strict";
    var slice = Array.prototype.slice, that = this;
    return function () {
        var jsEval,  jsApply, fn, args = slice.apply(arguments),result = args[0];
        jsEval = function () {
            args = slice.apply(arguments);  fn = that.call(null, result);
            jsApply.apply(null, args);
        };
        jsApply = function () {
            args = slice.apply(arguments);  result = fn.call(null, args[0]);
            if (args.length > 1) {  jsEval.apply(null, args.slice(1)); }
        };
        jsEval.apply(null, args.slice(1));
        return result;
    };
};

//Creating the curried functions
var adder2 = add.haskellCurry(),
    multiplier2 = multiply.haskellCurry();
//Finding the sum of 1, 2 & 3 with the curried function
console.log(adder2(1, 2, 3));
//Finding the product of 1, 2, 3, 4, 5 & 6 with the curried function
console.log(multiplier2(1, 2, 3, 4, 5, 6));
//Creating the curried functions
var adder = add.crockfordCurry();
var multiplier = multiply.crockfordCurry();
//Finding the sum of 1,2 & 3 with the curried function
console.log(adder(adder(1)(2))(3));
//Finding the product of 1,2 & 3 with the curried function
console.log(multiplier(multiplier(1)(2))(3));