function Compose(a , b ) { return function(x) { return a(b(x)); }}

function sqr( a ) { return a*a; }
function cub( b ) { return b*b*b; }

var c = Compose(sqr,cub);
console.log( c(10) );