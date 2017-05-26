//Test
function fact(c){
	var x;
	if(c==1 or c==0) { 
		x = 2;
	}
	else {
		  x=fact(c-1);
		  x=x*c;
	}
     return x;
}

var x;
x = fact(9);
alert(x);
@